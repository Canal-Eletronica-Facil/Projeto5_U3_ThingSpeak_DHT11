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

#define DHTTYPE DHT11    // Tipo do Sensor utilizado (DHT11 ou DHT22)
#define DHTPIN 33        // Número do GPIO em que o DHT11 está conectado

int LED1 = 25;           // LED Verde do Projeto
int LED2 = 26;           // LED Vermelho do Projeto
float Temperatura = 0;   // Variável para a leitura de temperatura do DHT11
float Umidade = 0;       // Variável para a leitura de umidade do DHT11
String myStatus = "";    // Variável para a leitura do status de comunicação com Thing Speak

WiFiClient  PCB_ESP32;    // Da o nome PCB_ESP32 ao Objeto utilizado (WiFiClient)  
DHT SENSOR_DHT11(DHTPIN, DHTTYPE); // Da o nome SENSOR_DHT11 ao Objeto Utilizado (DHT)

//............. BLOCO 3 - CONFIGURAÇÕES E INICIALIZAÇÕES DO PROJETO .............//
void setup() 
{
  pinMode(LED1, OUTPUT);  // Configura o GPIO25 como Saída de Sinal para controlar o LED1
  pinMode(LED2, OUTPUT);  // Configura o GPIO26 como Saída de Sinal para controlar o LED2
  
  Serial.begin(115200);  // Começa a comunicação Serial a 115200 bps
  WiFi.mode(WIFI_STA);   // Configura o modo de trabalho do ESP32 como Estação WI-FI
  ThingSpeak.begin(PCB_ESP32);  // Inicializa o Thing Speak usando o objeto Wi-Fi PCB_ESP32
  SENSOR_DHT11.begin();  // Inicializa a comunicação com o Sensor DHT11
}

//........ BLOCO 4 - LÓGICA DE CONTROLE DA PROGRAMAÇÃO E LOOP DE REPETIÇÃO ...........//
void loop() 
{ 
  if(WiFi.status() != WL_CONNECTED)  // Se a leitura do Status da Conexão Wifi for
  {                                  // For diferente de Conectado
         
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

  ThingSpeak.setField(1, Umidade); // Configura Parâmetros para enviar a Umidade ao "Field 1" do Thing Speak 
  ThingSpeak.setField(2, Temperatura); // Configura Parâmetros para enviar a Temperatura ao "Field 2" do Thing Speak

  if(Temperatura > 30.0)
  {
    digitalWrite(LED2, HIGH);  // Liga o Led Vermelho indicando Alarme
    digitalWrite(LED1, LOW);   // Desliga o Led Verde  
  } 
  else
  {
    digitalWrite(LED1, HIGH);  // Liga o Led Verde Indicando Temperatura ok
    digitalWrite(LED2, LOW);   // Desliga o Led Vermelho
  }

  int x = ThingSpeak.writeFields(Numero_Canal, WriteAPIKey); // Envia dados ao ThingSpeak e Salva o Retorno em X
  if(x == 200)    //Se o valor do Retorno foi 200
  {
    Serial.println("Dados Enviados com Sucesso ao Thing Speak"); // Dados enviados com Sucesso
  }
  else            //Se o valor do Retorno não foi 200
  {
    Serial.println("Problemas ao Atualizar o Thing Speak. HTTP Error Code " + String(x)); // Erro de Envio
  }
    
  delay(15000); // Aguarda 15 Segundos para Realizar um novo 
                // tratamento do Sensor e envio de informações ao Thing Speak
}
