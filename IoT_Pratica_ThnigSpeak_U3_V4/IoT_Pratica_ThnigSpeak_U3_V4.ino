//... Livro IoT na Prática - PROGRAMA 5 - Monitoramento do sensor DHT11 ...//
//... E envio dos valores de Temperatura e Umidade para o Thing Speak .....//

//............. BLOCO 1 - ADICIONAR BIBLIOTECAS AO PROJETO DE IOT .............//

#include "ThingSpeak.h" //Adiciona a Biblioteca do Thing Speak ao Projeto
#include <WiFi.h>       //Adiciona a Biblioteca de comunicação Wi-fi ao Projeto
#include "DHT.h"        //Adiciona a Biblioteca de interação com o DHT11 ao Projeto        

//............. BLOCO 2 - DEFINIÇÕES E CRIAÇÃO DE VARIÁVEIS DO PROJETO .............//
#define NOME_REDE "ElectronicsMC"       // Aqui você deve colocar o Nome de Sua Rede Wi-Fi
#define SENHA_REDE "@Efacil1750k"       // Aqui você deve colocar a Senha da Sua Rede Wi-Fi
#define WriteAPIKey "L9KRL420DF9CH9X5"  // Aqui você deve colocar a API KEY De Escrita do Thing Speak
#define Numero_Canal 0                  // Constante 0 para conexões 
#define DHTTYPE DHT11                   // Tipo do Sensor utilizado (DHT11 ou DHT22)
#define DHTPIN 33                       // Número do GPIO em que o DHT11 está conectado
int LED1 = 25;                          // LED Verde do Projeto
int LED2 = 26;                          // LED Vermelho do Projeto
int SENSOR_PRESENCA = 32;               // Sensor de presença do projeto de IoT
int ESTADO_SP = 0;                      // Variável para armazenar o valor lido do Sensor de Presença
int SENSOR_JANELA = 35;                 // Sensor que determina o estado de uma janela (aberta / fechada)
int ESTADO_SJ = 0;                      // Variável para armazenar o valor lido do Sensor de Janela
int SENSOR_LUZ = 34;                    // Pino que receberá o sinal do sensor de Luz LDR
int LUZ = 0;                            // Variável para armazenar o valor do ADC - Sensor de LUZ
float Temperatura = 0;                  // Variável para a leitura de temperatura do DHT11
float Umidade = 0;                      // Variável para a leitura de umidade do DHT11
String myStatus = "";                   // Variável para a leitura do status de comunicação com Thing Speak
WiFiClient  PCB_ESP32;                  // Da o nome PCB_ESP32 ao Objeto utilizado (WiFiClient)  
DHT SENSOR_DHT11(DHTPIN, DHTTYPE);      // Da o nome SENSOR_DHT11 ao Objeto Utilizado (DHT)

//............. BLOCO 3 - CONFIGURAÇÕES E INICIALIZAÇÕES DO PROJETO .............//
void setup() 
{
  pinMode(LED1, OUTPUT);          // Configura o GPIO25 como Saída 
  pinMode(LED2, OUTPUT);          // Configura o GPIO26 como Saída 
  pinMode(SENSOR_PRESENCA,INPUT); // Configura o GPIO32 como Entrada  
  pinMode(SENSOR_JANELA,INPUT);   // Configura o GPIO35 como Entrada 
  pinMode(SENSOR_LUZ,INPUT);      // Configura o GPIO34 como Entrada 
  
  Serial.begin(115200);           // Começa a comunicação Serial a 115200 bps
  WiFi.mode(WIFI_STA);            // Configura o modo de trabalho do ESP32 como Estação WI-FI
  ThingSpeak.begin(PCB_ESP32);    // Inicializa o Thing Speak usando o objeto Wi-Fi PCB_ESP32
  SENSOR_DHT11.begin();           // Inicializa a comunicação com o Sensor DHT11
}

//........ BLOCO 4 - LÓGICA DE CONTROLE DA PROGRAMAÇÃO E LOOP DE REPETIÇÃO ...........//
void loop() 
{ 
  int porcentagem = 0;                    // Cria uma variável local para cálculo de porcentagem
  
  if(WiFi.status() != WL_CONNECTED)       // Se a leitura do Status da Conexão Wifi for
  {                                       // For diferente de Conectado
    Serial.print("Realizando Conexao com a Rede Wi-Fi:"); // Mensagem pela Serial
    Serial.println(NOME_REDE);                            // Mensagem pela Serial        
    while(WiFi.status() != WL_CONNECTED)  // Enquanto não estiver conectado
    {
      WiFi.begin(NOME_REDE, SENHA_REDE);  // Realiza a conexão com a rede Wi-Fi WPA/WPA2
      Serial.print(".");                  // Mensagem pela Serial
      delay(2000);                        // Realiza tentativa de 2 em 2 segundos   
    } 
    Serial.println("");                   // Após Conectado ao Roteador
    Serial.print("Conectado na rede ");   // Mostra ao usuário o nome da Rede
    Serial.println(NOME_REDE);            // E também o IP gerado pelo Roteador
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } 
 // Conexão Realizada com Sucesso, Inicia-se o Envio de Dados ao Thing Speak

  Temperatura =  SENSOR_DHT11.readTemperature();  // Realiza a Leitura da Temperatura - DHT11
  Umidade = SENSOR_DHT11.readHumidity();          // Realiza a Leitura da Umidade - DHT11
  Serial.print("O valor da Umidade eh:");         // Escreve texto na Serial
  Serial.println(Umidade);                        // Escreve o valor da Umidade monitorada
  Serial.print("O valor da Temperatura eh:");     // Escreve texto na Serial
  Serial.println(Temperatura);                    // Escreve o valor da Temperatura monitorada
    
  LUZ = analogRead(SENSOR_LUZ);
  porcentagem = map(LUZ,0,4095,0,100);            // Converte o valor lido do ADC em porcentagem
  Serial.print("O valor do ADC eh:");             // Escreve texto na Serial
  Serial.println(LUZ);                            // Escreve o valor da conversão (ADC) monitorada
  Serial.print("O valor da % de Luz eh:");        // Escreve texto na Serial
  Serial.println(porcentagem);                    // Escreve o valor da porcentagem calculada
   
  ESTADO_SP = digitalRead(SENSOR_PRESENCA);           // 1 - PRESENÇA DETECTADA e 0 - SEM PRESENÇA
  Serial.print("O Estado do Sensor de Presença Eh:"); // Escreve texto na Serial
  Serial.println(ESTADO_SP);                          // Escreve o Estado do Sensor de Presença
  ESTADO_SJ = digitalRead(SENSOR_JANELA);             // 1 - JANELA FECHADA e 0 - JANELA ABERTA
  Serial.print("O Estado do Sensor de Janela Eh:");   // Escreve texto na Serial
  Serial.println(ESTADO_SJ);                          // Escreve o Estado do Sensor de Janela

  if(ESTADO_SJ == 0 || ESTADO_SP == 1)
  {
    digitalWrite(LED2, HIGH);  // Liga o Led Vermelho
    digitalWrite(LED1, LOW);   // Desliga o Led Verde  
  } 
  else
  {
    digitalWrite(LED1, HIGH);  // Liga o Led Verde 
    digitalWrite(LED2, LOW);   // Desliga o Led Vermelho
  }
  
  ThingSpeak.setField(1, Umidade);    // Seta o Valor da Umidade a ser Enviada  
  // Para o Field 1 do ThingSpeak Através do ThingSpeak.writeFields();
  ThingSpeak.setField(2, Temperatura);// Seta o Valor da Temperatura a ser Enviada
  // Para o Field 2 do ThingSpeak Através do ThingSpeak.writeFields();
  ThingSpeak.setField(3, porcentagem);// Seta o Valor da porcentagem a ser Enviada 
  // Para o Field 3 do ThingSpeak Através do ThingSpeak.writeFields();
  ThingSpeak.setField(4, ESTADO_SJ); // Seta o Valor de ESTADO_SJ a ser Enviada  
  // Para o Field 4 do ThingSpeak Através do ThingSpeak.writeFields();
  ThingSpeak.setField(5, ESTADO_SP); // Seta o Valor de ESTADO_SP a ser Enviada  
  // Para o Field 5 do ThingSpeak Através do ThingSpeak.writeFields();

  int x = ThingSpeak.writeFields(Numero_Canal, WriteAPIKey); 
  // Envia dados ao ThingSpeak e Salva o Retorno em X
  if(x == 200)    //Se o valor do Retorno foi 200
  {
    Serial.println("Dados Enviados com Sucesso ao Thing Speak"); 
    // Dados enviados com Sucesso
  }
  else //Se o valor do Retorno não foi 200
  {
    Serial.println("Problemas ao Atualizar o Thing Speak. HTTP Error Code " + String(x)); 
    // Erro de Envio
  }
    
  delay(15000); // Aguarda 15 Segundos para Realizar um novo 
                // tratamento dos Sensores e envio de informações ao Thing Speak
}
