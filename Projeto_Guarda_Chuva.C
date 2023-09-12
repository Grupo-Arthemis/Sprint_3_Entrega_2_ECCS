#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"

char ssid[] = "Galaxy Felipe";
char pass[] = "Felipe100603";
char serverAddress[] = "https://api.tago.io/data";  // TagoIO address
char contentHeader[] = "application/json";
char tokenHeader[]   = "b6649bfb-bca8-4ee5-8cf6-5b8f021f4621"; // TagoIO Token

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_wifi();
}
void init_wifi() {
  Serial.println("Conectando WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado");
  Serial.print("Meu IP eh: ");
  Serial.println(WiFi.localIP());
}

float Chuva = 0; // Variável de chuva
float Umidade = 0; // Variável de umidade

void loop() {
  char anyData[30];
  char chuvaData[300];
  char anyData1[30];
  char umidadeData[300];
  int statusCode = 0;

  // Formatar e enviar dados de chuva
  strcpy(chuvaData, "{\n\t\"variable\": \"Chuva\",\n\t\"value\": ");
  dtostrf(Chuva, 6, 2, anyData);
  strncat(chuvaData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"mm\"\n\t}\n");
  strncat (chuvaData, anyData1, 100);
  Serial.println(chuvaData);
  HTTPClient client; // Iniciar uma nova instância do cliente HTTP
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(chuvaData);
  delay (2000);
  Serial.print("Status code (Chuva): ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");
  Serial.println();

  // Formatar e enviar dados de umidade
  strcpy(umidadeData, "{\n\t\"variable\": \"Umidade\",\n\t\"value\": ");
  dtostrf(Umidade, 6, 2, anyData);
  strncat(umidadeData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"%\"\n\t}\n");
  strncat(umidadeData, anyData1, 100);
  Serial.println(umidadeData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(umidadeData);
  delay(2000);
  Serial.print("Status code (Umidade): ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");

  // Incrementar o valor da chuva (simulação)
  Chuva += 10;

  // Incrementar o valor da umidade (simulação)
  Umidade += 5;

  // Delay para enviar os dados periodicamente
  delay(5000);
}