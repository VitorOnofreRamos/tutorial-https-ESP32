/********************************************************************
 * Projeto: Consulta da cotação do dólar (USD-BRL) via HTTPS        *
 * Autor: André Tritiack                                            *
 *                                                                  *
 * Este exemplo conecta o ESP32 a uma rede Wi-Fi e utiliza a        *
 * biblioteca WiFiClientSecure para realizar uma requisição HTTPS   *
 * à AwesomeAPI, que fornece a cotação do dólar em tempo real.      *
 *                                                                  *
 * IMPORTANTE:                                                      *
 * Este exemplo utiliza validação de certificado SSL/TLS, garantindo*
 * uma conexão segura com o servidor remoto.                        *
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
const char* SECRET_SSID = "INSIRA O NOME DA SUA REDE WIFI";
const char* SECRET_PW = "INSIRA A SENHA DA SUA REDE WIFI";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Buffer para armazenar o JSON
String jsonBuffer;

// Certificado SSL da API (atual em abril de 2025)
const char* root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDejCCAmKgAwIBAgIQf+UwvzMTQ77dghYQST2KGzANBgkqhkiG9w0BAQsFADBX
MQswCQYDVQQGEwJCRTEZMBcGA1UEChMQR2xvYmFsU2lnbiBudi1zYTEQMA4GA1UE
CxMHUm9vdCBDQTEbMBkGA1UEAxMSR2xvYmFsU2lnbiBSb290IENBMB4XDTIzMTEx
NTAzNDMyMVoXDTI4MDEyODAwMDA0MlowRzELMAkGA1UEBhMCVVMxIjAgBgNVBAoT
GUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBMTEMxFDASBgNVBAMTC0dUUyBSb290IFI0
MHYwEAYHKoZIzj0CAQYFK4EEACIDYgAE83Rzp2iLYK5DuDXFgTB7S0md+8Fhzube
Rr1r1WEYNa5A3XP3iZEwWus87oV8okB2O6nGuEfYKueSkWpz6bFyOZ8pn6KY019e
WIZlD6GEZQbR3IvJx3PIjGov5cSr0R2Ko4H/MIH8MA4GA1UdDwEB/wQEAwIBhjAd
BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDwYDVR0TAQH/BAUwAwEB/zAd
BgNVHQ4EFgQUgEzW63T/STaj1dj8tT7FavCUHYwwHwYDVR0jBBgwFoAUYHtmGkUN
l8qJUC99BM00qP/8/UswNgYIKwYBBQUHAQEEKjAoMCYGCCsGAQUFBzAChhpodHRw
Oi8vaS5wa2kuZ29vZy9nc3IxLmNydDAtBgNVHR8EJjAkMCKgIKAehhxodHRwOi8v
Yy5wa2kuZ29vZy9yL2dzcjEuY3JsMBMGA1UdIAQMMAowCAYGZ4EMAQIBMA0GCSqG
SIb3DQEBCwUAA4IBAQAYQrsPBtYDh5bjP2OBDwmkoWhIDDkic574y04tfzHpn+cJ
odI2D4SseesQ6bDrarZ7C30ddLibZatoKiws3UL9xnELz4ct92vID24FfVbiI1hY
+SW6FoVHkNeWIP0GCbaM4C6uVdF5dTUsMVs/ZbzNnIdCp5Gxmx5ejvEau8otR/Cs
kGN+hr/W5GvT1tMBjgWKZ1i4//emhA1JG1BbPzoLJQvyEotc03lXjTaCzv8mEbep
8RqZ7a2CPsgRbuvTPBwcOMBBmuFeU88+FSBX6+7iP0il8b4Z0QFqIwwMHfs/L6K1
vepuoxtGzi4CZ68zJpiq1UvSqTbFJjtbD4seiMHl
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
      String serverPath = "https://economia.awesomeapi.com.br/json/last/USD-BRL";
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

      String bidStr = myObject["USDBRL"]["bid"];
      float bid = bidStr.toFloat();

      Serial.print("Cotação do Dólar (USD): R$ ");
      Serial.println(bid, 2);
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
