/*
   LABORATORIO #7: MANEJO DE MEMORIA MICRO SD
   Angel Arnoldo Cuellar Bendfeldt
   Electronica Digital 2
   Ingenieria mecatronica
*/

// INCLUSION DE LIBRERIAS Y DEFINICION DE VARIABLES GLOBALES

#include <SPI.h>
#include <SD.h>

char opcion ; // variable para almacenar la informacion resivida en la comunicacion serial 

// variables para uso y manejo de la libreria SD 
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = PA_3; //cs PIN

File myFile;

void setup() {
  // habilitando la comunicacion serial a 9600 baudios 
  Serial.begin(9600);

  SPI.setModule(0);   // seleccionando el modulo SPI0 de la tiva 

  Serial.println("Inicializando memoria SD...");

  pinMode(PA_3, OUTPUT);     // definiendo como un pin de salida 

// inicializando la memoria SD para poder acceder a su contenido. 
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("Inicializacion fallida, por favor verifica lo siguiente:");
    Serial.println("* insertastes la memoria SD ?");
    Serial.println("* cableaste de manera adecuada tu modulo ?");
    Serial.println("* seleccionaste los pines correctos para tu dispositivo ?");
    return;
  } else {
    Serial.println("La conexion fue exitosa");
  }

  // Impresion de tipo de memoria reconocida por el microcontrolador. 
  Serial.print("\ntipo de memoria: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Desconocida");
  }

  // verificando el tipo de particion de la memoria SD si es FAT32 O FAT16
  if (!volume.init(card)) {
    Serial.println(" Ninguno de los formatos es adecuado, por favor formatea tu memoria par uno de esos tipos ");
    return;
  }

  // imprimiendo el tipo de particion que posee la memoria SD 
  uint32_t volumesize;
  Serial.print("\nEl tipo de particion es ");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  // incializando comunicacion ISP con la memoria para poder utilizar su contenido 
  
if (!SD.begin(chipSelect)) {
    Serial.println("Conexion fallida o memoria no insertada ");
    // don't do anything more:
    //return;
  }
  Serial.println(" memoria inicializada ");
}

void loop() {

  // mensaje de bienvenida y desplegue de archivos para visualizacion del usuario 

  Serial.println("Los archivos contenidos en la MICRO SD son: ");

  root.openRoot(volume);

  // aqui se desplegara el listado de archivos por nombre, tipo y fecha de creacion 
  root.ls(LS_R | LS_DATE | LS_SIZE);

  // aqui se le solucitara al usuario que ingrese el numero segun el orden en que fueron desplegados el archivo que desea ver 

  Serial.println(" "); 
  Serial.println(" Seleccion con un numero segun la posicion del archivo TXT que desea seleccionar ");

  while (!Serial.available()) {   // espero hasta que el usuario me envie su opcion 
    delay(10);
  }
  
  if (!SD.begin(chipSelect)) {   // inicializo nuevamente la comunicacion ISP con la memoria para poder manejar su contenido 
  }
  
  Serial.println("tarjeta incializada para usar");
  
  opcion = Serial.read();  // almaceno el dato enviado por el usuario 

  // en este caso segun el valor enviado asi mismo sera el archivo que se desplegara en el monitor 

  switch (opcion) {
    case ('1'):
      // abriendo el archivo SALUDO.TXT en forma de lectura 
      myFile = SD.open("saludo.txt");
      if (myFile) {
        Serial.println("saludo.txt:");

        // aqui imprimimos constantemente cada caracter contenido en el archivo hasta que ya no haya nada mas que enviarse 
        while (myFile.available()) {
          Serial.write(myFile.read()); 
        }
        // cerramos el acceso al archivo en forma de lectura 
        myFile.close();
        Serial.println(" ");
      } else {
        // aqui enviamos un mensaje en dado caso suceda algun error en la lectura del archivo 
        Serial.println("error de lectura en el archivo saludo.txt");
      }
      break;
    case ('2'):
      // abriendo el archivo ESPADA.TXT en forma de lectura
      myFile = SD.open("espada.txt");
      if (myFile) {
        Serial.println("espada.txt:");

        // aqui imprimimos constantemente cada caracter contenido en el archivo hasta que ya no haya nada mas que enviarse
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // cerramos el acceso al archivo en forma de lectura
        myFile.close();
        Serial.println(" ");
      } else {
         // aqui enviamos un mensaje en dado caso suceda algun error en la lectura del archivo 
        Serial.println("error opening espada.txt");
      }
      break;
    case ('3'):
      // abriendo el archivo JINJAN.TXT en forma de lectura
      myFile = SD.open("jinjan.txt");
      if (myFile) {
        Serial.println("jinjan.txt:");

        // aqui imprimimos constantemente cada caracter contenido en el archivo hasta que ya no haya nada mas que enviarse
        while (myFile.available()) {
          Serial.write(myFile.read());
        }
        // cerramos el acceso al archivo en forma de lectura
        myFile.close();
        Serial.println(" ");
      } else {
         // aqui enviamos un mensaje en dado caso suceda algun error en la lectura del archivo 
        Serial.println("error opening jinjan.txt");
      }
      break;
     default:
        Serial.println("opción inválida "); // si la opcion no corresponde enviamos este mensaje 
  }

  opcion = Serial.read(); // resetemos el valor de la variable a cero. 

}
