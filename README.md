# Consulta de Cotação do Dólar com ESP32 via HTTPS (sem certificado)

Este projeto exemplifica como conectar o ESP32 a uma rede Wi-Fi e realizar uma requisição HTTPS para consultar a cotação do dólar (USD-BRL) utilizando a API da AwesomeAPI.

Neste exemplo, utilizamos a biblioteca WiFiClientSecure com o método `setInsecure()`, que ignora a verificação do certificado SSL do servidor. Essa prática é útil para fins educacionais e protótipos, mas não é recomendada para ambientes de produção.

---

## Requisitos

- Placa ESP32 (DOIT DevKit V1 ou equivalente)
- Arduino IDE com suporte ao ESP32
- Biblioteca Arduino_JSON instalada
- Conexão com a internet via Wi-Fi

---

## Funcionamento

- O ESP32 se conecta à rede Wi-Fi.
- A cada 10 segundos, é feita uma requisição HTTPS para a URL:
  
  `https://economia.awesomeapi.com.br/json/last/USD-BRL`

- A resposta JSON é processada para extrair o campo `bid`, que representa a cotação de compra do dólar em reais.
- O valor é convertido para float e exibido com duas casas decimais no Monitor Serial.

---

## Exemplo de saída no Monitor Serial

```
========================================================================
JSON object = {"USDBRL":{"code":"USD", ... }}
========================================================================
Cotação do Dólar (USD): R$ 5.17
========================================================================
```

---

## Observações

- A utilização de `setInsecure()` dispensa o uso de certificados, mas torna a conexão suscetível a ataques de interceptação. Utilize essa abordagem apenas para testes controlados.

---

## Referências

- https://docs.awesomeapi.com.br/api-de-moedas
- https://randomnerdtutorials.com/esp32-http-get-open-weather-map-json-arduino/
- https://github.com/espressif/arduino-esp32

---

## Licença

Este projeto está licenciado sob a licença MIT.
