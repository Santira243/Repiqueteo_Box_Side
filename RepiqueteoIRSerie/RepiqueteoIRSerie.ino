
//--
//###Version 24/10
//--

#include <Adafruit_NeoPixel.h>

// Variables de conteo y comunicacion serie
int valorEnviado = 0;
int contDetecciones = 0;
// Valores de detección mínimo y máximo, para determinar
// el comportamiento de la tira de LEDs
#define MIN_SPEED 2
#define MAX_SPEED 4

// Estado guardado de ambos infrarrojos
int IR1_stateNow = 0;
int IR2_stateNow = 0;
// Estado previo de ambos infrarrojos
int IR1_statePrev = 0;
int IR2_statePrev = 0;
// Variables temporales
long actualTime = 0;
long interruptTime = 0;
const long INTERVAL_TIME = 500;
int   tramo_r, tramo_g , tramo_b;
// Constantes de la tira de LEDs
#define pinRGB 4    // Pin que va a la tira de LEDs RGB
#define NUMPIXELS 112 // Para controlar las luces
#define DELAYVAL 0    // Retardo de actualizacion de pixeles
// Nuevo objeto 'pixels', para controlar la tira RGB
Adafruit_NeoPixel pixels(NUMPIXELS, pinRGB, NEO_GRB + NEO_KHZ800);

// Pines de entrada
# define PIN_1 2
# define PIN_2 3

//-------------------------------------------------------------------------------------------

// Funcion llamada para procesar los conteos de cada periodo
void procesarDatos()
{
  // Calculos para el avance o retroceso de la barra
  if (contDetecciones > MAX_SPEED){
    valorEnviado+=5;}
  else if (contDetecciones < MIN_SPEED){
    valorEnviado-=5;}

  // Limitacion de valores minimo y maximo
  if (valorEnviado > NUMPIXELS){
    valorEnviado = NUMPIXELS;}
  else if (valorEnviado < 0){
    valorEnviado = 0;}

  // Enviamos los valores al Serial o a la tira de LEDs
  Serial.write(valorEnviado);
  //Serial.println(valorEnviado);
  prenderTira(valorEnviado);
}

//-------------------------------------------------------------------------------------------

void setup()
{
  // Salida del infrarrojo numero 1
  pinMode(PIN_1, INPUT);
  // Salida del infrarrojo numero 2
  pinMode(PIN_2, INPUT);
  // Inicializamos el objeto de NeoPixels
  pixels.begin(); 
  
  // Inicio de aspectos temporales
  actualTime = millis();
  interruptTime = actualTime+INTERVAL_TIME;
  
  Serial.begin(115200);
  }

//-------------------------------------------------------------------------------------------

void loop()
{

  // Este retardo va a modo de antirrebote
  delay(20);

  // Leemos ambos IR
  IR1_statePrev = IR1_stateNow;
  IR2_statePrev = IR2_stateNow;
  IR1_stateNow = digitalRead(PIN_1);
  IR2_stateNow = digitalRead(PIN_2);

  // Comprobacion para el IR 1
  if(IR1_statePrev != IR1_stateNow){    // Detecto un cambio de estado
    if (IR1_stateNow == LOW){              // El nivel bajo significa que hay un obstaculo
      contDetecciones++;
    }
  }
  // Comprobacion para el IR 2
  if(IR2_statePrev != IR2_stateNow){    
    if (IR2_stateNow == LOW){              
      contDetecciones++;
    }
  }

  // Chequeamos que haya pasado el intervalo esperado
  actualTime = millis();
  if (actualTime > interruptTime){
    procesarDatos();
    pixels.show();   // Actualizamos los valores para la tira de RGB
    contDetecciones = 0;
    interruptTime = actualTime+INTERVAL_TIME;
  }
}

//-------------------------------------------------------------------------------------------

// Funcion para encender la tira de NeoPixels
void prenderTira (int cantLEDs){
    // Limpiamos el buffer de la tira de LEDs
    pixels.clear();
    // Encendemos la cantidad de LEDs calculados, actualizando el buffer con un bucle
    for(int i=0; i<cantLEDs; i++) {
      tramo_r = map(i, 1,NUMPIXELS/3,0,255);
      tramo_g = map(i, (NUMPIXELS/3)-10,(2*NUMPIXELS/3)+10,1,255);
      tramo_b = map(i,(2*NUMPIXELS/3)-10,(NUMPIXELS+10),1,255);
      if(tramo_r > 255)
      { tramo_r  = 0; }
      if((tramo_g > 255) || (tramo_g < 0))
      { tramo_g  = 0; }
      
      if((tramo_b > 255) || (tramo_b < 0))
      { tramo_b  = 0; }
      
      pixels.setPixelColor(i, pixels.Color(tramo_r,tramo_g,tramo_b));
   
      //Serial.println("Funcion"+String(tramo_r)+"g:"+String(tramo_g)+"b:"+String(tramo_b));
    }
    pixels.show();   // Actualizamos los valores para la tira de RGB
    
}
