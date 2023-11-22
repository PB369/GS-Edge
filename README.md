<h1>CareBand</h1>
<h2>Unindo a inovação à uma vida mais saudável.</h2>

<h3>Descrição do Problema</h3>
<p>Todos os anos, cerca de 400 mil brasileiros morrem devido a problemas cardiovasculares. Ao mesmo tempo, 14 milhões de pessoas possuem alguma doença relacionada ao coração. Dentre as principais causas deste problema, podemos citar o sedentarismo, a hipertensão, o estresse, o tabagismo e o colesterol. Pessoas com diabetes possuem de duas a até quatro vezes mais chances de sofrer de um infarto.
</p>
<p>A dificuldade de fornecer um rápido socorro a quem necessite, bem como detectar com antecedência as doenças do coração são fatores contribuintes em grande parte dos casos de óbito. Encontrar meios para prevenir complicações no estado de saúde de pessoas com problemas cardiovasculares, bem como prestar o devido acompanhamento médico com dados detalhados e precisos, tanto para a prevenção como para o tratamento de doenças do coração, é um conjunto de desafios que a nossa equipe decidiu solucionar.
</p>
<p>Um outro problema identificado em nossas pesquisas e que trabalharemos em nossa solução foi a frequente saturação de leitos hospitalares, podendo resultar em uma demora e ineficiência da prestação de auxílio médico à população. Podemos tomar de exemplo o Distrito Federal (DF) e o estado de Santa Catarina, que no mês de abril deste ano, foi-se reportado que os leitos de Unidade de Terapia Intensiva (UTI) estavam com uma ocupação de praticamente 100%.
</p>

<h3>Solução</h3>
<p>A nossa proposta consiste em unificar a utilização de um smartband com uma inteligência artificial de gestão de leitos hospitalares e de auxílio ao transporte pré-hospitalar de pacientes.
</p>
<p>Nomeado como GoodCare, o objetivo do projeto é oferecer à população um dispositivo (smartband) de monitoramento da frequência cardíaca, da pressão arterial e do nível de sedentarismo, a fim de propor meios de prevenção e tratamento de problemas de saúde. Além disso, nosso smartband, através da disponibilização de um QR Code, contará com a utilização de realidade aumentada na aplicação de primeiros socorros e contendo um relatório sobre a situação da vítima, a fim de auxiliar as equipes de paramédicos e leigos na prestação de socorro. Ao mesmo tempo, a GoodCare se conecta a uma inteligência artificial chamada Ami, que presta um serviço de automação e simplificação do transporte de vítimas à hospitais, através da geração automática de rotas em GPS das ambulâncias para os hospitais mais próximos e com a maior capacidade de atendimento para o paciente em questão. Assim, também temos como objetivo oferecer uma automação e facilitação na parte de transporte pré-hospitalar de pacientes.
</p>
<p>A ideia é que, se o smartband detectar uma situação de grave emergência, como uma parada cardíaca, uma queda abrupta da pressão arterial e entre outras situações que exponham a vida do usuário em risco, ela notificará a Ami, informando o problema detectado, o momento em que ele foi identificado e a localização do usuário. Através desta notificação, a IA informará o hospital mais próximo da vítima sobre a situação, a fim de enviar uma equipe médica para socorrê-la. Ao mesmo tempo, a Ami oferecerá a rota mais adequada a um hospital para a ambulância encaminhar o paciente, levando em consideração o número de leitos disponíveis.</p>
<p>Levando em consideração o prazo de entrega deste projeto e também os recursos disponíveis para a construção da nossa solução, esta simulação conterá apenas uma pequena parcela da ideia total. Ao acessar o nosso projeto, você terá acesso à uma simulação dos batimentos cardíacos, podendo vizualizar o BPM em uma tela OLED e também em uma dashboard MQTT.</p>

<h3>Instruções de Uso</h3>
<p>Para acessar a nossa simulação siga o passo a passo listado abaixo:</p>
<ol>
  <li>Acesse o link do nosso projeto, construído na plataforma Wokwi (https://wokwi.com/projects/381932710841125889).</li>
  <li>Acesse o link da plataforma HiveMQ para vizualizar a transmição de dados via MQTT (https://www.hivemq.com/demos/websocket-client/).</li>
  <li>Na plataforma HiveMQ, clique no botão "Connect".</li>
  <li>Em seguida, na área "Subscriptions", clique no botão "Add New Topic Subscription".</li>
  <li>Insira no input "topic" o seguinte texto: "topico_bpm/#". Depois clique no botão "Subscribe".</li>
  <li>Na plataforma Wokwi, clique no botão verde para iniciar a simulação.</li>
  <li>Tento iniciado a simulação, você poderá clicar no chip "Pulse Generator Breakout" para alterar o valor dos batimentos cardíacos.</li>
  <li>Na plataforma HiveMQ, observe a chegada dos BPMs na seção "Messages".</li>
</ol>

<h3>Links</h3>
<ul>
  <li>Simulação: https://wokwi.com/projects/381932710841125889</li>
  <li>Plataforma HiveMQ: https://www.hivemq.com/demos/websocket-client/</li>
</ul>