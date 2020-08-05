//Este exemplo cria um server na porta 80 para receber conexões de clients
//que podem enviar mensagens "relay on" para ligar o relê ou "relay off" para
//desligar o relê
#include <AppFernandoK.h>

//Pino onde está o relê
#define RELAY_PIN 2

//SSID e senha da rede WiFi. Modifique de acordo com a sua rede.
#define SSID "YOUR_SSID"
#define PASSWORD "YOUR_PASSWORD"

//Objeto que irá gerenciar as conexões com os clients
AppFernandoK app;

void setup()
{
    Serial.begin(115200);

    //Colocamos o pino do relê como saída
    pinMode(RELAY_PIN, OUTPUT);

    //Desligamos o relê. Na configuração do nosso relê: HIGH desliga e LOW liga
    digitalWrite(RELAY_PIN, HIGH);

    //Conectamos à WiFi
    setupWiFi();

    //Inicializa um server para que os clients se conectem na porta 80
    app.beginWiFiServer(80);

    //Informamos qual função deverá ser executada quando algo chegar de algum client
    app.onMessage(onMessage);
}

//Conecta à rede WiFi
void setupWiFi() 
{
    Serial.print("Connecting to WiFi");

    //Mandamos conectar à rede WiFi
    WiFi.begin(SSID, PASSWORD);

    //Enquanto não estiver conectado
    while (WiFi.status() != WL_CONNECTED)
    {
        //Espera 500ms
        Serial.print(".");
        delay(500);
    }

    //Se chegou aqui está conectado
    Serial.println("Connected!");

    //Mostra o IP para utilizarmos na conexão
    Serial.println(WiFi.localIP().toString());
}

//Será executada toda fez que chegar algo dos clients
void onMessage(String message, IPAddress ip)
{
    //Exibe a mensagem e o IP de quem enviou
    Serial.print("Received message: ");
    Serial.print(message);
    Serial.print(" from ");
    Serial.println(ip.toString());

    //Remove eventuais espaços em branco
    message.trim();

    //Coloca a string toda em maiúsculo
    message.toUpperCase();

    //Se a mensagem que chegou for "RELAY ON"
    if(message == "RELAY ON")
    {
        //Liga o relê
        digitalWrite(RELAY_PIN, LOW);
        //Avisa para quem enviou a mensagem que tudo ocorreu bem
        app.sendMessage(message + " OK", ip);
        //Alternativamente se quiser enviar para todos os clients conectados:
        //app.sendMessageToAll(message + " OK");
    }
    else if(message == "RELAY OFF")
    {
        //Desliga o relê
        digitalWrite(RELAY_PIN, HIGH);
        //Avisa para quem enviou a mensagem que tudo ocorreu bem
        app.sendMessage(message + " OK", ip);
    }
    else
    {   
        //Se chegou aqui significa que a mensagem não era conhecida
        //Então avisamos quem enviou que a mensagem não foi reconhecida
        app.sendMessage("Unknown command: " + message, ip);
    }
}

void loop()
{
    //Chamamos a função da lib que irá fazer as verificações necessárias (nova conexão, desconexão, nova mensagem)
    app.handleConnections();
}