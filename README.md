# ESP32 - Requisições HTTPS com a AwesomeAPI

Este tutorial apresenta um exemplo prático de como conectar o ESP32 a uma rede Wi-Fi e realizar **requisições HTTPS GET** para obter dados de uma API de cotações de moedas. Utilizaremos a **AwesomeAPI - Economia**, que fornece valores atualizados para dólar, euro e bitcoin.

---

## Objetivo

- Conectar o ESP32 a uma rede Wi-Fi
- Realizar uma requisição HTTPS GET
- Interpretar a resposta JSON recebida da API
- Exibir as cotações de USD, EUR e BTC em relação ao real brasileiro (BRL)

---

## Requisitos

### Hardware

- Placa **ESP32 DOIT DEVKIT V1**
- Cabo micro-USB
- Acesso à rede Wi-Fi

### Software

- IDE Arduino (1.8 ou superior)
- Bibliotecas:
  - WiFi (inclusa)
  - HTTPClient (inclusa)
  - WiFiClientSecure (inclusa)
  - Arduino_JSON (instalar via gerenciador de bibliotecas)

---

## Instalação da biblioteca Arduino_JSON

1. Abra a IDE Arduino
2. Vá em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas...**
3. Procure por **Arduino_JSON**
4. Clique em **Instalar**

---

## Código-fonte

O código está disponível no arquivo `main.ino`. Antes de carregar o código na placa, atualize suas credenciais de Wi-Fi:

```cpp
const char* SECRET_SSID = "SUA REDE WIFI";
const char* SECRET_PW = "SUA SENHA WIFI";
```

---

## Funcionamento

1. O ESP32 conecta-se à rede Wi-Fi utilizando os dados fornecidos.
2. A cada 10 segundos, ele realiza uma requisição HTTPS GET ao seguinte endpoint:

```
https://economia.awesomeapi.com.br/last/USD-BRL,EUR-BRL,BTC-BRL
```

3. A resposta é um JSON contendo as informações das cotações.
4. O código realiza o parse do JSON e exibe, no monitor serial, o valor de compra (`bid`) para:

- Dólar (USD)
- Euro (EUR)
- Bitcoin (BTC)

---

## Exemplo de saída no monitor serial

```
========================================================================
JSON recebido = {"USDBRL":{"bid":"5.2730",...},"EURBRL":{"bid":"5.7290",...},"BTCBRL":{"bid":"362000.00",...}}
========================================================================
USD/BRL: R$ 5.2730
EUR/BRL: R$ 5.7290
BTC/BRL: R$ 362000.00
========================================================================
```

---

## Observações

- Este exemplo utiliza `client.setInsecure()` para desabilitar a verificação do certificado SSL. Este método é aceitável apenas para testes locais. Para ambientes de produção, recomenda-se utilizar a verificação correta do certificado.
- Este projeto não utiliza tratamento de erro avançado para o JSON. É possível expandi-lo para mostrar variações, máximas e mínimas, por exemplo.

---

## Estrutura do projeto

```
esp32-https-awesomeapi/
├── main.ino
├── README.md
└── LICENSE
```

---

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
