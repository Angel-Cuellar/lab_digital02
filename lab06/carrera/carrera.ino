/*
 * LABORATORIO #6: CARRERA EN TIVA C
 * Angel Arnoldo Cuellar Bendfeldt 
 * Electronica Digital 2 
 * Ingenieria mecatronica 
 */

/////////// variables /////////////

int cont1 = 0; //el contador inicia en 0
int cont2 = 0; //el contador inicia en 0
int bloqueo = 1; // bloqueo de botones de jugadores activado

void setup() {
  // put your setup code here, to run once:

  pinMode(2, INPUT);  // push1
  pinMode(19, INPUT); // push2
  pinMode(18, INPUT); // star
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(33, OUTPUT);

  pinMode(30, OUTPUT);   // RGB rojo
  pinMode(40, OUTPUT);   // RGB azul
  pinMode(39, OUTPUT);   // RGB verde

}

void loop() {

  if ((digitalRead(18) == HIGH) and (bloqueo == 1)) {

    cont1 = 0;
    cont2 = 0;  // inicializo los contadores en 0

    digitalWrite(RED_LED, HIGH); // rojo
    digitalWrite(BLUE_LED, LOW);  // azul    luz roja
    digitalWrite(GREEN_LED, LOW);  // verde
    delay(500);
    digitalWrite(RED_LED, HIGH); // rojo
    digitalWrite(BLUE_LED, HIGH); // azul    luz amarilla
    digitalWrite(GREEN_LED, LOW);  // verde
    delay(500);
    digitalWrite(RED_LED, LOW); // rojo
    digitalWrite(BLUE_LED, LOW);  // azul    luz verde
    digitalWrite(GREEN_LED, HIGH);  // verde

    bloqueo = 0;  // dejo de bloquear el botones de los jugadores
  }

  if ((digitalRead(2) == HIGH) and (bloqueo == 0)) {
    while (digitalRead(2) == HIGH) {}   // antirebote de boton de jugador 1
    cont1++;   // incremento contador de jugador 1
  }
  else if ((digitalRead(19) == HIGH) and (bloqueo == 0)) {
    while (digitalRead(19) == HIGH) {}  // antirebote de boton de jugador 2
    cont2++;   // incremento contador de jugador 2
  }

  // este sirve para encender los leds en forma de decadas segun la cantidad de botonazos dados por jugador 1

  switch (cont1) {
    case (1):
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case (2):
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case (3):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case (4):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
      /*
    case (5):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case (6):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
    case (7):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      break;
    case (8):
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      break;
      */
    default:
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      break;
  }

  // este sirve para encender los leds en forma de decadas segun la cantidad de botonazos dados por jugador 2

  switch (cont2) {
    case (1):
      digitalWrite(23, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
    case (2):
      digitalWrite(23, LOW);
      digitalWrite(24, HIGH);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
    case (3):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, HIGH);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
    case (4):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, HIGH);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
      /*
    case (5):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, HIGH);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
    case (6):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, HIGH);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
    case (7):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, HIGH);
      digitalWrite(33, LOW);
      break;
    case (8):
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, HIGH);
      break;
      */
    default:
      digitalWrite(23, LOW);
      digitalWrite(24, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      digitalWrite(28, LOW);
      digitalWrite(29, LOW);
      digitalWrite(33, LOW);
      break;
  }

  if ((cont1 == 4) or (cont2 == 4)) {
    bloqueo = 1;    // activo el bloqueo de botones de ambos jugadores 
    digitalWrite(GREEN_LED, LOW);  // apago el RGB verde 
    
    if (cont1 == 4) {
      digitalWrite(RED_LED, HIGH); // rojo gana player 1
    }
    else if (cont2 == 4) {
      digitalWrite(BLUE_LED, HIGH); // azul gana palyer 2
    }
  }

}
