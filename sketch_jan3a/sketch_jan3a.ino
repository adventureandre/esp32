#include <WiFi.h>
#include <PubSubClient.h>
#define LED_PIN 2  // Define o pino do LED (GPIO 2)

// Definição das credenciais do Wi-Fi e do servidor MQTT
const char* ssid = "Milena";            
const char* password = "auroraemilly";       
const char* mqtt_server = "192.168.50.118";  

WiFiClient espClient;                  
PubSubClient client(espClient);        
long lastMsg = 0;                      

// Função de callback para processar mensagens recebidas
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");

  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i]; // Converte o payload para string
  }
  Serial.println(msg);

  // Verifica a mensagem recebida e liga/desliga o LED
  if (msg == "ligar") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED LIGADO");
  } else if (msg == "desligar") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED DESLIGADO");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);  // Configura o pino como saída
  digitalWrite(LED_PIN, LOW); // Garante que o LED comece desligado
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);          

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexão Wi-Fi estabelecida");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconectar() {
  while (!client.connected()) {
    Serial.println("Aguardando conexão");

    if (client.connect("ESP32client")) {
      Serial.println("Conectado ao servidor MQTT");
      client.subscribe("server/data");  // Assina o tópico "esp32/send" para receber mensagens

    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconectar();
  }
  client.loop();


  // Publica a mensagem apenas a cada 5 segundos
      // long now = millis();
      // if (now - lastMsg > 10000) {
      //   lastMsg = now;
      //   client.publish("esp32/data", "esp32 original"); // envia a msn
      //   Serial.println("Mensagem publicada: esp32 original ativo");
      // }
}