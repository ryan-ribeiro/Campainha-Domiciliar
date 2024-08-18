#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "WiFi network name";
const char* password = "password";

const char* mainURL = "https://api.telegram.org/bot{{botToken}}/sendmessage?chat_id={{chat_id}}&parse_mode=HTML&text=Alguém%20está%20na%20porta!";

unsigned long timerDelay = 5000;
unsigned long lastTime = 0;

const int data = 5; // pino analogico conectado ao sensor de som  
int db = 0;
const int buzzerPin = 9;  // Pino digital conectado ao buzzer

void tocarCampainha() {
  // Frequências das notas musicais em Hz
  const int Csharp6 = 1109; // C#6 (aproximadamente 1108.73 Hz)
  const int A4 = 440;       // A4 (440 Hz)

  // Duração das notas em milissegundos (baseado no tempo de batida)
  const int tempoBase = 500;  // Tempo base de 1 batida em milissegundos
  const int duracaoCsharp6 = tempoBase / 2;  // Duração de 1/2 batida
  const int duracaoA4 = tempoBase;           // Duração de 1 batida

  // Toca a nota C#6 por 1/2 batida
  tone(buzzerPin, Csharp6, duracaoCsharp6);
  delay(duracaoCsharp6 + 50);  // Pequena pausa entre as notas

  // Toca a nota A4 por 1 batida
  tone(buzzerPin, A4, duracaoA4);
  delay(duracaoA4 + 50);  // Pequena pausa após a nota
}

void setup() {
  Serial.begin(115200);
  pinMode(data, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Conectado à rede WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // RequisicaoHttpGET();
  // delay(5000);
  Serial.println(db);
  if (WiFi.status() == WL_CONNECTED) {
      db = analogRead(data);
  } else {
      Serial.println("WiFi desconectado");
      setup();
  }

  if (db >= 250 && ((millis() - lastTime) > timerDelay)) {
      RequisicaoHttpGET();
      tocarCampainha();
      lastTime = millis();
  }
}

void RequisicaoHttpGET() {
  WiFiClientSecure client;
  client.setInsecure();  // Ignora a verificação do certificado SSL

  HTTPClient http;
  http.begin(client, mainURL);

  int httpResponseCode = http.GET();

  Serial.print("Código de resposta HTTP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("Resposta do servidor:");
      Serial.println(payload);
  } else {
      Serial.print("Erro na solicitação HTTP: ");
      Serial.println(httpResponseCode);
  }

  http.end();
}
