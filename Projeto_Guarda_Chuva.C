#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "DHT.h"

char ssid[] = "Galaxy Felipe";
char pass[] = "Felipe100603";
char serverAddress[] = "https://api.tago.io/data";  // TagoIO address
char contentHeader[] = "application/json";
char tokenHeader[]   = "b6649bfb-bca8-4ee5-8cf6-5b8f021f4621"; // TagoIO Token
HTTPClient client; // Iniciar uma nova instância do cliente HTTP

const char deviceID[] = "01"; //ID do dispositivo

#define DHTPIN 4// Pino onde o sensor DHT11 está conectado
#define DHTTYPE DHT11 // Tipo do sensor DHT (DHT11 no seu caso)
#define REED_SWITCH_PIN  14// Pino onde o reed switch está conectado

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  init_wifi();
  dht.begin(); // Inicializar o sensor DHT11
  pinMode(REED_SWITCH_PIN, INPUT_PULLUP); // Configurar o reed switch como entrada com resistor pull-up
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

float Chuva = digitalRead(REED_SWITCH_PIN); // Variável de chuva
float umidade = dht.readHumidity(); // Variável de umidade
float temperatura = dht.readTemperature(); // Variável de temperatura

void loop() {
  char anyData[30];
  char anyData1[30];
  char chuvaData[300];
  char umidadeData[300];
  char tempData[300];
  int statusCode = 0;

  // Formatar e enviar dados de chuva
  strcpy(chuvaData, "{\n\t\"variable\": \"Chuva\",\n\t\"value\": ");
  dtostrf(Chuva, 6, 2, anyData);
  strncat(chuvaData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"mm\"\n\t}\n");
  strncat (chuvaData, anyData1, 100);
  Serial.println(chuvaData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(chuvaData);
  delay (2000);
  Serial.print("Status code (Chuva): ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");
  Serial.println();

  // Formatar e enviar dados de temperatura
  strcpy(tempData, "{\n\t\"variable\": \"Temperatura\",\n\t\"value\": ");
  dtostrf(temperatura, 6, 2, anyData);
  strncat(tempData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"C\"\n\t}\n");
  strncat (tempData, anyData1, 100);
  Serial.println(tempData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(tempData);
  delay (2000);
  Serial.print("Status code (Temperatura): ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");
  Serial.println();
  delay(5000);

  // Formatar e enviar dados de umidade
  strcpy(umidadeData, "{\n\t\"variable\": \"Umidade\",\n\t\"value\": ");
  dtostrf(umidade, 6, 2, anyData);
  strncat(umidadeData, anyData, 100);
  strcpy(anyData1, ",\n\t\"unit\": \"%\"\n\t}\n");
  strncat(umidadeData, anyData1, 100);
  Serial.println(umidadeData);
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(umidadeData);
   Serial.print("Status code (Umidade): ");
  Serial.println(statusCode);
  Serial.println("End of POST to TagoIO");

  // Delay para enviar os dados periodicamente
  delay(5000);
}
