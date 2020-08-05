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
#ifndef __APPFERNANDOK_H__
#define __APPFERNANDOK_H__

#include <Arduino.h>
#include <vector>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

class AppFernandoK {
    private:
        //Variável que vai guardar a função de callback para avisar sobre mensagens que chegarem
        std::function<void(String, IPAddress)> messageCallback;

        //Objeto WiFi Server onde os clients irão se conectar
        WiFiServer wifiServer;

        //Vector com os clients que estão conectados
        std::vector<WiFiClient> clients;

        //Verifica se existe alguém tentando se conectar
        void checkForNewClient();

        //Remove do vector os clients desconectados
        void refreshConnections();

        //Faz a leitura das mensagens que vêm dos clients
        void readFromClients();
    public:
        AppFernandoK();

        //Inicializa o server na porta indicada
        void beginWiFiServer(int port);

        //Função que deverá ser chamada no loop ou em uma task para que a lib possa executar suas operações
        void handleConnections();

        //Envia mensagem para o client que possui o ip indicado
        void sendMessage(String message, IPAddress ip);

        //Envia mensagem para todos os clients conectados
        void sendMessageToAll(String message);

        //Informa qual função de callback deverá ser executada quando uma mensagem chegar de um dos clients conectados
        void onMessage(std::function<void(String, IPAddress)> callback);
};

#endif