#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// Inicializa la libreria de cliente WiFi
WiFiClient cliente;

String inputString = "";         // a string to hold incoming data
char incomingByte = 0;   // for incoming serial data
String respuestaWeb = "";
int i = 1;
bool bandera = false;
bool correcto = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.autoConnect("IndoorMatic");

  //if you get here you have connected to the WiFi
  Serial.println("<conectado>");
}

void loop()
{
  recibeSerial();
  recibeWeb();
  if (bandera)
  {
    bandera = false;
    Serial.println(inputString);
    Serial.println(respuestaWeb);
    inputString = "";
    respuestaWeb = "";
    Serial.println(i);
    i++;
  }
}


// FUNCIONES
void recibeSerial()
{
  while (Serial.available() > 0)
  {
    incomingByte = (char)Serial.read();

    if (incomingByte == '<')
    {
      correcto = true;
    }

    if (correcto)
    {
      inputString += incomingByte;
      if (incomingByte == '>')
      {
        correcto = false;
        bandera = true;
      }
    }
  }
}

void recibeWeb()
{
      if (cliente.connect("www.indoormatic.com.ar", 80))
    {
      Serial.println("Conectado");
      cliente.print("GET ");
      cliente.print("/misc/contenido.txt");
//      cliente.print();
      cliente.println(" HTTP/1.1\r\nHost: indoormatic.com.ar\r\n\r\n");
      cliente.println();
    }
    else
    {
      Serial.println("no conecta con el servidor");
    }

    while (cliente.available())
    {
      incomingByte = (char)cliente.read();

      respuestaWeb += incomingByte;
      //Serial.print(respuesta);
    }
    bandera = true;
}

