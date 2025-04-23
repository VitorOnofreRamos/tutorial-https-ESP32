/********************************************************************
 * Projeto: Consulta da cotação do dólar (USD-BRL) via HTTPS        *
 * Autor: André Tritiack                                            *
 *                                                                  *
 * Este exemplo conecta o ESP32 a uma rede Wi-Fi e utiliza a        *
 * biblioteca WiFiClientSecure para realizar uma requisição HTTPS   *
 * à AwesomeAPI, que fornece a cotação do dólar em tempo real.      *
 *                                                                  *
 * ⚠️ IMPORTANTE:                                                   *
 * Utilizamos `client.setInsecure()` para **ignorar a validação do *
 * certificado SSL/TLS**. Essa prática é aceitável em protótipos,   *
 * mas **NÃO deve ser usada em ambientes de produção**, pois        *
 * compromete a segurança da conexão.                               *
 *                                                                  *
 * Link da API: https://economia.awesomeapi.com.br/json/last/USD-BRL
 ********************************************************************/

// Bibliotecas já instaladas
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Biblioteca a instalar
#include <Arduino_JSON.h>

// WiFi e Timer
const char* SECRET_SSID = "";
const char* SECRET_PW = "";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Buffer para armazenar o JSON
String jsonBuffer;

void setup() {
  Serial.begin(115200);

  WiFi.begin(SECRET_SSID, SECRET_PW);
  Serial.println("Conectando...");
  
  verificaWiFi();

  Serial.println("Timer programado para 10 segundos. Aguarde esse tempo para a leitura...");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      
      String serverPath = "https://viacep.com.br/ws/07077020/json";

      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Falha no formato dos dados!");
        return;
      }

      Serial.println("========================================================================");
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.println("========================================================================");

      String cep = myObject["cep"];

      Serial.print("Cep: ");
      Serial.println(cep);
      Serial.println("========================================================================");
    }
    else {
      Serial.println("WiFi desconectado");
    }

    lastTime = millis();
  }
}

// Função para requisição HTTPS GET
String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  client.setInsecure(); // Ignora validação do certificado

  HTTPClient https;

  https.begin(client, serverName);  // Inicia conexão HTTPS

  int httpResponseCode = https.GET();
  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();
  } else {
    Serial.print("Código de erro: ");
    Serial.println(httpResponseCode);
  }

  https.end();
  return payload;
}


// Função para conexão Wi-Fi
void verificaWiFi(){
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Tentando conectar à rede SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(SECRET_SSID, SECRET_PW);  
      Serial.print(".");
      delay(2000);     
    } 
    Serial.print("Rede conectada: ");
    Serial.println(WiFi.SSID());
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.println("");
  }
}
