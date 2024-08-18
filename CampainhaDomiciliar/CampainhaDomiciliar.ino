#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "WiFi network names";
const char* password = "password";

// Seu nome de domínio com caminho de URL ou endereço IP com caminho
const char* serverName = "http://192.168.1.15:8080/campainha/tocar";

unsigned long timerDelay = 5000; // Timer set to 5 seconds (5000)
unsigned long lastTime = 0;

int data = 5; // pin de leitura do sensor de som
int db = 0; // db é altura do ruído de som, variando de 0 a 90

String local = "Caixa de correios";
String arduino = "Atmega2560";
String dados = "Alguém está na porta!";

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

  Serial.println("Timer configurado para 5 segundos (variável timerDelay), levará 5 segundos antes de publicar a primeira leitura.");
}

void loop() {
  Serial.println(db);
  if (WiFi.status() == WL_CONNECTED) {
      db = analogRead(data); // db é altura do ruído de som, variando de 0 a 90
  } else {
      Serial.println("WiFi desconectado");
      setup();
  }

  if (db >= 250 && ((millis() - lastTime) > timerDelay)) {
      RequisicaoHttpPOST();
      lastTime = millis();
  }
}

void RequisicaoHttpPOST() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  // Se você precisar de autenticação Node-RED/servidor, insira usuário e senha abaixo
  // http.setAuthorization("SEU_USUÁRIO", "SUA_SENHA");

  http.addHeader("Content-Type", "application/json");
  // String httpRequestData = "{\"api_key\":\"tPmAT5Ab3j7F9\",\"nome\":\"" + nome + "\",\"arduino\":\"" + arduino + 
  // "\",\"id\":\"" + id + "\",\"mensagem\":\"" + mensagem + "\",\"data\":\"" + data + "\,\"hora\":\"" + hora + "\"}";
  String httpRequestData = "{\"api_key\":\"tPmAT5Ab3j7F9\",\"local\":\"" + local + "\",\"arduino\":\"" + arduino + 
  "\",\"dados\":\"" + dados + "\"}";

  Serial.println(httpRequestData);

  int httpResponseCode = http.POST(httpRequestData);

  // Se você precisar de uma solicitação HTTP com um tipo de conteúdo: text/plain
  // http.addHeader("Content-Type", "text/plain");
  // int httpResponseCode = http.POST("Olá, Mundo!");
  Serial.print("Código de resposta HTTP: ");
  Serial.println(httpResponseCode);

  http.end();
}
