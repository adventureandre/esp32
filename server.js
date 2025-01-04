const express = require('express');
const aedes = require('aedes')(); // Broker MQTT
const ws = require('websocket-stream'); // WebSocket para MQTT
const http = require('http');

// Configurações
const PORT_EXPRESS = 3000; // Porta do servidor HTTP/Express
const PORT_MQTT = 1883;    // Porta do broker MQTT

const app = express();

// Rota básica
app.get('/', (req, res) => {
  res.send('Servidor MQTT com Express está funcionando!');
});

// Inicia o servidor HTTP para Express
const httpServer = http.createServer(app);
httpServer.listen(PORT_EXPRESS, () => {
  console.log(`Servidor HTTP rodando em http://localhost:${PORT_EXPRESS}`);
});

// Inicia o broker MQTT na porta especificada
const mqttServer = http.createServer();
ws.createServer({ server: mqttServer }, aedes.handle);
mqttServer.listen(PORT_MQTT, () => {
  console.log(`Broker MQTT rodando na porta ${PORT_MQTT}`);
});

// Eventos do Broker MQTT
aedes.on('client', (client) => {
  console.log(`Cliente conectado: ${client.id}`);
});

aedes.on('clientDisconnect', (client) => {
  console.log(`Cliente desconectado: ${client.id}`);
});

aedes.on('publish', (packet, client) => {
    console.log(`Mensagem publicada no tópico "${packet.topic}": ${packet.payload.toString()}`);
  });
  
