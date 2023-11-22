// Importação de bibliotecas
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define PULSE_PER_BEAT    1           // Número de pulsos por batimento cardíaco
#define INTERRUPT_PIN     5           // Pino de interrupção
#define SAMPLING_INTERVAL 1000        // Intervalo de amostragem em milissegundos
#define SCREEN_WIDTH 128              // Largura do OLED, em pixels
#define SCREEN_HEIGHT 64              // Altura do OLED, em pixels
#define TOPICO_BPM       "topico_bpm"

#define PUBLISH_DELAY 1000   // Atraso da publicação MQTT em milissegundos

#define ID_MQTT "goodcare_mqtt" // id mqtt (para identificação de sessão)

int buzzer = 19; // Pino do Buzzer

const char *SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char *PASSWORD = "";        // Senha da rede WI-FI que deseja se conectar

// URL do broker MQTT que se deseja utilizar
const char *BROKER_MQTT = "broker.hivemq.com";

int BROKER_PORT = 1883; // Porta do Broker MQTT

unsigned long publishUpdate;

static char strBPM[10] = {0};

// Variáveis e objetos globais
WiFiClient espClient;         // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

void initWiFi(void);
void initMQTT(void);
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void checkWiFIAndMQTT(void);

/* Inicializa e conecta-se na rede WI-FI desejada */
void initWiFi(void)
{
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconnectWiFi();
}

/* Inicializa os parâmetros de conexão MQTT(endereço do broker, porta e seta
  função de callback) */
void initMQTT(void)
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Informa qual broker e porta deve ser conectado
  MQTT.setCallback(callbackMQTT);           // Atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

/* Função de callback  esta função é chamada toda vez que uma informação de um dos tópicos subescritos chega */
void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  String msg;

  // Obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  Serial.printf("Chegou a seguinte string via MQTT: %s do topico: %s\n", msg, topic);
}

/* Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
   em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito. */
void reconnectMQTT(void)
{
  while (!MQTT.connected()) {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_BPM);
    } else {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Nova tentativa de conexao em 2 segundos.");
      delay(2000);
    }
  }
}

/* Verifica o estado das conexões WiFI e ao broker MQTT. Em caso de desconexão (qualquer uma das duas), a conexão é refeita. */
void checkWiFIAndMQTT(void)
{
  if (!MQTT.connected())
    reconnectMQTT(); // se não há conexão com o Broker, a conexão é refeita

  reconnectWiFi(); // se não há conexão com o WiFI, a conexão é refeita
}

void reconnectWiFi(void)
{
  // se já está conectado a rede WI-FI, nada é feito.
  // Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

volatile uint16_t pulse;              // Variável que será incrementada na interrupção
uint16_t count;                       // Variável para armazenar o valor atual de pulse

float heartRate;                      // Frequência cardíaca calculada a partir de count

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;  // Mutex para garantir acesso seguro a pulse

void IRAM_ATTR HeartRateInterrupt() {
  portENTER_CRITICAL_ISR(&mux);  // Entra em uma seção crítica de interrupção
  pulse++;  // Incrementa a variável pulse de maneira segura
  portEXIT_CRITICAL_ISR(&mux);   // Sai da seção crítica de interrupção
}

// Declarando para o display SSD1306 ser conectado via I2C
#define OLED_RESET     -1 // Pino de Reset
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT); // Defini o pino do Buzzer como uma saída

  pinMode(INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), HeartRateInterrupt, RISING);  // Configura a interrupção no pino

  // Inicia o OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Limpa o display.
  display.clearDisplay();

  // Inicializa a conexao Wi-Fi
  initWiFi();

  // Inicializa a conexao ao broker MQTT
  initMQTT();
}

void loop() {
  HeartRate();

  /* Repete o ciclo após 1 segundo */
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    
    // Verifica o funcionamento das conexões WiFi e ao broker MQTT
    checkWiFIAndMQTT();

    // Formata as strings a serem enviadas para o dashboard (campos texto)
    sprintf(strBPM, "%.0f bpm", heartRate);

    // Envia as strings ao dashboard MQTT
    MQTT.publish(TOPICO_BPM, strBPM);

    // Keep-alive da comunicação com broker MQTT
    MQTT.loop();
  }
}

void HeartRate(void) {
  static unsigned long startTime;
  if (millis() - startTime < SAMPLING_INTERVAL) return; // Intervalo de amostragem
  startTime = millis();

  portENTER_CRITICAL(&mux);  // Entra em uma seção crítica
  count = pulse;  // Salva o valor atual de pulse e zera pulse
  pulse = 0;
  portEXIT_CRITICAL(&mux);   // Sai da seção crítica

  // Ajuste na fórmula para mapear a faixa de 0 Hz a 220 Hz para a frequência cardíaca em BPM
  heartRate = map(count, 0, 220, 0, 220);  // Mapeia a contagem para a faixa desejada

  // Imprimi a frequência cardíaca no OLED:
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("BPM: " + String(heartRate));
  display.display();
  display.clearDisplay();

  if (heartRate >0 && heartRate <= 50){ // Ativando o buzzer quando BPM é baixo
      tone(buzzer, 1500);
      delay(500);
      noTone(buzzer);
      delay(500);

      // Imprimindo mensagem sobre baixo BPM
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Pulso fraco!");
      display.display();
  } else if (heartRate >= 170) {
      tone(buzzer, 1500);
      delay(500);
      noTone(buzzer);
      delay(500);

      // Imprimindo mensagem sobre alto BPM
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Pulso acelerado, risco de infarto!");
      display.display();
  } else if (heartRate == 0) {
      tone(buzzer, 1000);
      delay(500);
      noTone(buzzer);
      delay(500);

      // Imprimindo mensagem sobre BPM zerado (parada cardíaca)
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Sem pulso! Parada Cardiaca detectada.");
      display.display();
  }

  Serial.println("Heart Rate: " + String(heartRate) + " BPM");
}