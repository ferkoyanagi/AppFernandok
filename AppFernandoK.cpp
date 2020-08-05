/*
 * The MIT License (MIT)
 * Copyright (c) 2020 Fernando K
 * 
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <AppFernandoK.h>

AppFernandoK::AppFernandoK():wifiServer(80) {
}

//Inicializa o server na porta informada para que este espere por novas conexões
void AppFernandoK::beginWiFiServer(int port) {
    wifiServer.begin(port);
}

//Esta função deve ser chamada no loop ou em uma task para que a lib execute as operações necessárias
void AppFernandoK::handleConnections() {
    //Verifica se tem alguém tentando se conectar
    checkForNewClient();

    //Remove do vector eventuais conexões encerradas
    refreshConnections();

    //Verifica se alguém está enviando alguma mensagem, faz a leitura e informa via callback
    readFromClients();
}


//Verifica se existe alguém tentando se conectar e, se sim, coloca no vector
void AppFernandoK::checkForNewClient() {
    //Colocamos na variável um eventual client
    WiFiClient newClient = wifiServer.available();
    
    //Se o client for diferente de nulo significa que temos uma nova conexão
    if(newClient) {  
        // Inserimos no vector
        clients.push_back(newClient);
    }
}

// Função que verifica se um ou mais clients se desconectaram do server e, se sim, estes clients serão retirados do vector
void AppFernandoK::refreshConnections(){
  // Flag que indica se pelo menos um client se desconectou
  bool flag = false;
  
  // Objeto que receberá apenas os clients conectados
  std::vector<WiFiClient> connectedClients;

  // Percorremos o vector
  for(int i=0; i<clients.size(); i++) {
    // Verificamos se o client está desconectado
    if(!clients[i].connected()) {
      // Finalizamos a conexão com o client
      clients[i].stop();
      // Setamos a flag como true indicando que o vector foi alterado
      flag = true;  
    }
    else {
      connectedClients.push_back(clients[i]); // Se o client está conectado, adicionamos no connectedClients
    }
  }

  // Se pelo menos um client se desconectou, atribuimos ao vector "clients" os clients de "connectedClients"
  if(flag) clients = connectedClients;
}

//Verifica um a um se os clients estão enviando algo e, se sim, faz a leitura e avisa via callback
void AppFernandoK::readFromClients() {
    // Percorremos o vector
    for(int i=0; i<clients.size(); i++) {
        WiFiClient client = clients[i];
        // Se existir dados a serem lidos
        if(client.available()) {
            // Recebemos a String até o '\n'
            String message = client.readStringUntil('\n');
            // Verificamos se existe um callback informado e se existe executa o callback
            if(this->messageCallback) {
                this->messageCallback(message, client.remoteIP());
            }
        }
    }
}

 //Envia mensagem para o client que possui o ip indicado
void AppFernandoK::sendMessage(String message, IPAddress ip) {
    for(int i=0; i<clients.size(); i++) {
        WiFiClient client = clients[i];
        if(client.remoteIP() == ip) {
            client.println(message);
        }
    }
}

//Envia mensagem para todos os clients conectados
void AppFernandoK::sendMessageToAll(String message) {
    for(int i=0; i<clients.size(); i++) {
        clients[i].println(message);
    }
}

//Informa qual função de callback deverá ser executada quando uma mensagem chegar de um dos clients conectados
void AppFernandoK::onMessage(std::function<void(String, IPAddress)> callback) {
    this->messageCallback = callback;
}