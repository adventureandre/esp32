const express = require("express");
const bodyParser = require("body-parser");
const aedes = require("aedes")();
const cors = require("cors");

const port = 5000;

const app = express();
app.use(cors());
app.use(bodyParser.json());

const mqttServer = require("net").createServer(aedes.handle);
const mqttPort = 1883;

mqttServer.listen(mqttPort, () => {
  console.log("Mqtt Server is running on port " + mqttPort);
});

aedes.on("client", (client) => {
  console.log("Client connected: ", client.id);
});

aedes.on("clientDisconnect", (client) => {
  console.log("Client disconnected: ", client.id);
});

aedes.on("publish", (packet, client) => {

  if(client){

    console.log(
      `Messagem recebida do cliente ${client.id} - Topico: ${packet.topic} -> Mensagem : ${packet.payload.toString()}`
    );
  }
});




app.get('/',(req,res)=>{
res.send({message:"API MQTT RODANDO"});
});

app.post('/echo',(req,res)=>{
  const {email, senha} = req.body;
  console.log(email);
  console.log(senha);
  res.send(req.body);
})

app.post('/send',(req,res)=>{
try{
  const mensagem = req.body.mensagem;
  console.log(mensagem);
  res.status(200).send({message:"Messagem publicada"});
}catch(e){
  throw new Error("falha o publicar mensagem")
}
})


app.listen(port, () => {
  console.log(`API MQTT rodando na porta ${port}`);
});



