/********************************************************************
 * Projeto: Consulta de CEP via HTTPS                               *
 * Autor: André Tritiack                                            *
 * Modificado por: Vitor Onofre Ramos                               *
 *                                                                  *
 * Este exemplo conecta o ESP32 a uma rede Wi-Fi e utiliza a        *
 * biblioteca WiFiClientSecure para realizar uma requisição HTTPS   *
 * à AwesomeAPI, que fornece a cotação do dólar em tempo real.      *
 *                                                                  *
 * IMPORTANTE:                                                      *
 * Este exemplo utiliza validação de certificado SSL/TLS, garantindo*
 * uma conexão segura com o servidor remoto.                        *
 *                                                                  *
 * Link da API: https://viacep.com.br
 * C:\Users\labsfiap>openssl s_client -showcerts -connect viacep.com.br:443
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

// Certificado SSL da API (atual em abril de 2025)
const char* root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEMjCCAxqgAwIBAgIBATANBgkqhkiG9w0BAQUFADB7MQswCQYDVQQGEwJHQjEb
MBkGA1UECAwSR3JlYXRlciBNYW5jaGVzdGVyMRAwDgYDVQQHDAdTYWxmb3JkMRow
GAYDVQQKDBFDb21vZG8gQ0EgTGltaXRlZDEhMB8GA1UEAwwYQUFBIENlcnRpZmlj
YXRlIFNlcnZpY2VzMB4XDTA0MDEwMTAwMDAwMFoXDTI4MTIzMTIzNTk1OVowezEL
MAkGA1UEBhMCR0IxGzAZBgNVBAgMEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE
BwwHU2FsZm9yZDEaMBgGA1UECgwRQ29tb2RvIENBIExpbWl0ZWQxITAfBgNVBAMM
GEFBQSBDZXJ0aWZpY2F0ZSBTZXJ2aWNlczCCASIwDQYJKoZIhvcNAQEBBQADggEP
ADCCAQoCggEBAL5AnfRu4ep2hxxNRUSOvkbIgwadwSr+GB+O5AL686tdUIoWMQua
BtDFcCLNSS1UY8y2bmhGC1Pqy0wkwLxyTurxFa70VJoSCsN6sjNg4tqJVfMiWPPe
3M/vg4aijJRPn2jymJBGhCfHdr/jzDUsi14HZGWCwEiwqJH5YZ92IFCokcdmtet4
YgNW8IoaE+oxox6gmf049vYnMlhvB/VruPsUK6+3qszWY19zjNoFmag4qMsXeDZR
rOme9Hg6jc8P2ULimAyrL58OAd7vn5lJ8S3frHRNG5i1R8XlKdH5kBjHYpy+g8cm
ez6KJcfA3Z3mNWgQIJ2P2N7Sw4ScDV7oL8kCAwEAAaOBwDCBvTAdBgNVHQ4EFgQU
oBEKIz6W8Qfs4q8p74Klf9AwpLQwDgYDVR0PAQH/BAQDAgEGMA8GA1UdEwEB/wQF
MAMBAf8wewYDVR0fBHQwcjA4oDagNIYyaHR0cDovL2NybC5jb21vZG9jYS5jb20v
QUFBQ2VydGlmaWNhdGVTZXJ2aWNlcy5jcmwwNqA0oDKGMGh0dHA6Ly9jcmwuY29t
b2RvLm5ldC9BQUFDZXJ0aWZpY2F0ZVNlcnZpY2VzLmNybDANBgkqhkiG9w0BAQUF
AAOCAQEACFb8AvCb6P+k+tZ7xkSAzk/ExfYAWMymtrwUSWgEdujm7l3sAg9g1o1Q
GE8mTgHj5rCl7r+8dFRBv/38ErjHT1r0iWAFf2C3BUrz9vHCv8S5dIa2LX1rzNLz
Rt0vxuBqw8M0Ayx9lt1awg6nCpnBBYurDC/zXDrPbDdVCYfeU0BsWO/8tqtlbgT2
G9w84FoVxp7Z8VlIMCFlA2zs6SFz7JsDoeA3raAVGI/6ugLOpyypEBMs1OUIJqsi
l2D4kF501KKaU73yqWjgom7C12yxow+ev+to51byrvLjKzg6CYG1a4XXvi3tPxq3
smPi9WIsgtRqAEFQ8TmDn5XpNpaYbg==
-----END CERTIFICATE-----
)EOF";

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
      String logradouro = myObject["logradouro"];
      String bairro = myObject["bairro"];
      String localidade = myObject["localidade"];
      String uf = myObject["uf"];

      Serial.print("Cep: ");
      Serial.println(cep);
      Serial.println("========================================================================");
      Serial.print("Logradouro: ");
      Serial.println(logradouro);
      Serial.println("========================================================================");
      Serial.print("Bairro: ");
      Serial.println(bairro);
      Serial.println("========================================================================");
      Serial.print("Localidade: ");
      Serial.println(localidade);
      Serial.println("========================================================================");
      Serial.print("Uf: ");
      Serial.println(uf);
      Serial.println("========================================================================");
    } else {
      Serial.println("WiFi desconectado");
    }
    lastTime = millis();
  }
}


// Função para requisição HTTPS GET
String httpGETRequest(const char* serverName) {
  WiFiClientSecure client;
  client.setCACert(root_ca);

  HTTPClient https;
  https.begin(client, serverName);
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
