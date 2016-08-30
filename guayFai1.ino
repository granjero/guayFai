#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

// Inicializa la libreria de cliente WiFi
WiFiClient cliente;

// Variables para la gestión de datos vía serial.
char input_buffer[512];
char input_buffer_index  = 0;
char buffering           = 0;
bool bandera             = false;

void setup()
{
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
  recibeSerie();
  if (bandera)
  {
    //Serial.println("test");
    //Serial.println(input_buffer);
    bandera = false;
    web(input_buffer);
    reset_input_buffer();
  }



}
//recibe lo que entra por serial y lo carga en inputbuffer si está entre <>
void recibeSerie()
{
  while (Serial.available())
  {
    // Obtengo un byte
    char input_char = (char)Serial.read();

    if ( input_char == '>')     // cerrador de paquete
    {
      buffering = 0;
      bandera = true;
    }

    if ( buffering > 0 )        // paquete valido ya iniciado
    {
      input_buffer[ input_buffer_index ] = input_char;
      input_buffer_index++;
    }

    if ( input_char == '<' )    // inicio de paquete
    {
      reset_input_buffer();
      buffering = 1;
    }
  }
}

//resetea el buffer de entrada serial
void reset_input_buffer()
{
  //borro el buffer y reinicio el índice.
  input_buffer_index   = 0;
  buffering            = 0;
  memset( input_buffer , 0 , sizeof(input_buffer) );
}


void web(char * direccion)
{
  if (cliente.connect(direccion, 80))
  {
    Serial.print(F ( "Conectado a: " ) );
    Serial.println(direccion);
    cliente.print("GET ");
    cliente.print("/robots.txt");
    cliente.print("");
    cliente.print(" HTTP/1.1\r\nHost: indoormatic.com.ar\r\n\r\n");
    cliente.println("");
  }
  else
  {
    Serial.println("no conecta con el servidor");
  }
  Serial.print("Respuesta web: ");
  while (cliente.available())
  {
    //Serial.print("cliente available ");
    Serial.print( (char)cliente.read() );
  }
}
/*
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  //needed for library <ESP8266WiFi.h>
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

  // Inicializa la libreria de cliente WiFi
  WiFiClient cliente;

  String inputString0 = "";         // a string to hold incoming data
  //String inputString1 = "";         // a string to hold incoming data
  char incomingByte = 0;   // for incoming serial data
  String respuestaWeb = "";
  int i = 1;
  bool bandera = false;
  bool correcto0 = false;
  bool correcto1 = false;

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
  if (bandera)
  {
    bandera = false;
    Serial.print("bandera TRUE String recibibo: ");
    Serial.println(inputString0);
    Serial.print("Iteracion: ");
    Serial.println(i);
    recibeWeb(inputString0);
    Serial.print("Respuesta WEB: ");
    Serial.println(respuestaWeb);
    inputString0 = "";
  //    inputString1 = "";
    respuestaWeb = "";
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
      correcto0 = true;
    }

    if (incomingByte == '[')
    {
      correcto1 = true;
    }

    if (correcto0)
    {
      inputString0 += incomingByte;
      if (incomingByte == '>')
      {
        correcto0 = false;
        bandera = true;
      }
    }

    if (correcto1)
    {
      inputString1 += incomingByte;
      if (incomingByte == ']')
      {
        correcto1 = false;
        bandera = true;
      }
    }


  }
  }

  void recibeWeb(String direccion)
  {
  char addres[128];
  parseaComando(inputString0).toCharArray(addres, 128);
  Serial.print("Direccion que trata de conectar");
  Serial.println(addres);
  if (cliente.connect(addres, 80))
  {
    Serial.println("Conectado");
    cliente.print("GET ");
    cliente.print(inputString0);
    cliente.print("");
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
  //bandera = true;
  }

  String parseaComando (String comando)
  {
  String resultado = comando.substring(1, sizeof(comando));
  Serial.println(resultado);
  return resultado;
  }
*/
