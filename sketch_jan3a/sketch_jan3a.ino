#include <WiFi.h>

#include <PubSubClient.h>



// Definição das credenciais do Wi-Fi e do servidor MQTT

const char* ssid = "Milena";            // Nome da rede Wi-Fi

const char* password = "auroraemilly";       // Senha da rede Wi-Fi

const char* mqtt_server = "192.168.50.118";  // Endereço do servidor MQTT



WiFiClient espClient;                  // Cria um cliente Wi-Fi

PubSubClient client(espClient);        // Cria um cliente MQTT usando o cliente Wi-Fi

long lastMsg = 0;                      // Variável para controlar o tempo das mensagens



void setup() {

  Serial.begin(115200);                // Inicializa a comunicação serial a 115200 bps

  setup_wifi();                        // Chama a função para configurar a conexão Wi-Fi

  client.setServer(mqtt_server, 1883); // Configura o servidor MQTT



  // Inicialização do hardware, configuração de pinos, etc., pode ser adicionada aqui

}



void setup_wifi() {

  delay(10);

  Serial.println();

  Serial.print("Conectando a ");

  Serial.println(ssid);



  WiFi.begin(ssid, password);          // Conecta-se à rede Wi-Fi com as credenciais fornecidas



  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }



  Serial.println("");

  Serial.println("Conexão Wi-Fi estabelecida");

  Serial.println("Endereço IP: ");

  Serial.println(WiFi.localIP());



  // Outras configurações de Wi-Fi, como definições de DHCP, podem ser adicionadas aqui

}



void reconectar() {

  while (!client.connected()) {

    Serial.println("Aguardando conexão");



    if (client.connect("ESP32client")) {

      Serial.println("Conectado ao servidor MQTT");

      client.subscribe("esp32/data"); // Assina o tópico "esp32/data" para receber mensagens

    } else {

      Serial.print("Falha na conexão, rc=");

      Serial.print(client.state());

      Serial.println(" Tentando novamente em 5 segundos");

      delay(5000); // Espera 5 segundos antes de tentar novamente a conexão

    }

  }



  // Outras operações após a reconexão bem-sucedida podem ser adicionadas aqui

}



void loop() {

  if (!client.connected()) {

    reconectar(); // Se o cliente MQTT não estiver conectado, tenta reconectar

  }

  client.loop(); // Permite ao cliente MQTT lidar com eventos e mensagens



  long now = millis();



  if (now - lastMsg > 5000) {

    lastMsg = now;

    client.publish("esp32/data", "esp32"); // Publica a mensagem "esp32" no tópico "esp32/data"

  }



  // Outras tarefas e lógica do programa podem ser adicionadas ao loop

}

