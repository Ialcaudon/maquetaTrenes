
//de version cuatro ala cinco se implementa multifuncion virtual para evitar comando delay en intermitentes Paso a nivel
 
#include <Wire.h>                           //inserta libreria del Bus I2C para manejar dos tarjetas de servos//
#include <Adafruit_PWMServoDriver.h>        //Inserta libreria tarjeta control servos//

Adafruit_PWMServoDriver servos0 = Adafruit_PWMServoDriver(0x40);//establece direccion en bus I2C de tarjeta servos0
Adafruit_PWMServoDriver servos1 = Adafruit_PWMServoDriver(0x41);//establece direccion en bus I2C de tarjeta servos1
unsigned int pos0 = 172;                    //Ancho de pulso en cuentas para pocicion 0°de los servos
unsigned int pos180 = 565;                  //Ancho de pulso en cuentas para la pocicion 180°de los servos
unsigned int pos110 = 400;                  //Ancho de pulso en cuentas para la pocicion 110°de los servos

#define LED_PN_1_PIN 50                      //cambio nombre pin 50 a LED_PN_1 para señales paso a nivel
#define LED_PN_2_PIN 52 //cambio nombre pin 52 a LED_PN_2 para señales paso a nivel
unsigned long previousTimeLEDPN1 = millis(); //unidades tiempo encendido LED PN 1 En milisegundos
long timeIntervalLEDPN1 = 1000;
int ledStatel = LOW;

                                      //implemento valores iniciales para tarjeta Mux Multiplexora de interruptores
int nivel = 0;
int muxValue = 0;
const int muxSIG = A0;//pin que devuelve el estado del interruptor
const int muxS0 = 23;//pines que dicen a que dierccion preguntar. es la direccion del interruptor en binario
const int muxS1 = 25;
const int muxS2 = 27;
const int muxS3 = 29;


void setup() {
 
                                       //Defino el puerto serie 
  Serial.begin(9600);
  Serial.print(" desvios_dos_tarjetas_mas_PN_1.5       22_11_2023");
                                       //Ecribo en puerto serie nombre programa y version y fecha
  delay(10000);                        //Muestra el Texto duranta 10 segundos en el monitor
                                       // Para paso a nivel defino los pines 50 y 52 ya renombrados como salida
  pinMode(LED_PN_1_PIN, OUTPUT);       //Defino salidas led para intermitente paso a nivel
  pinMode(LED_PN_2_PIN, OUTPUT);       
                                       //Termino definiciones paso a nivel

                                       
                                       //Empiezo asisgnacion de pines a tarjeta multiplexora interruptores
  pinMode(muxS0, OUTPUT);              //estos cuatro son los punteros de entrada en tarjeta Mux
  pinMode(muxS1, OUTPUT);              // los defino como salida expresa  que entrada vamos a leer en binario 
  pinMode(muxS2, OUTPUT);
  pinMode(muxS3, OUTPUT);
  pinMode(muxSIG, INPUT);              //este informa cual es el valor del interruptor que señala el puntero lo defino como entrada
  
  servos0.begin();
  servos0.setPWMFreq(60);              //Frecuecia PWM de 60Hz o T=16,66ms para tarjeta servos 0
  
  servos1.begin();
  servos1.setPWMFreq(60);              //Frecuecia PWM de 60Hz o T=16,66ms para tarjeta servos 1 
}
void setServo(uint8_t n_servo, int angulo) 
{
  int duty;
  duty = map(angulo,0,180,pos0,pos180);//configuracion en grados de los servos 
  servos0.setPWM(n_servo, 0, duty);
  servos1.setPWM(n_servo, 0, duty);
}
                                      //todo lo escrito anteriormente se ejecuta una sola vez en el arranque
                                      
                                       
void loop() {                         //Desde este punto se ejecuta en bucle constante
                                       
unsigned long currentTime = millis();  //Se define la unidad de tiempo para el paso a nivel
  if(currentTime - previousTimeLEDPN1 > timeIntervalLEDPN1) { 
    previousTimeLEDPN1 = currentTime;
  if (ledStatel == HIGH){
    ledStatel = LOW;
  }
                                       //Si el tiempo Actual - el tiempo anterior es mayor que el tiempo definido en timeIntervalLEDPN1 y el led esta en estado alto
                                       
    else {ledStatel = HIGH;            //El led pasa a estado apagado en caso contrario esta encendido
    }
    digitalWrite(LED_PN_1_PIN, ledStatel);
                                       //Aqui se define que si el segundo led tiene estado contrario al primero 
     if (ledStatel == HIGH){
    digitalWrite(LED_PN_2_PIN, LOW); 
    }
     if (ledStatel == LOW){
    digitalWrite(LED_PN_2_PIN, HIGH);
    }
  }
  
                                       //Termina intermitente paso a nivel //
 

  //preparando monitor serie
  Serial.print("Entrada interruptor 0:");
  
  //Empieza movimientos servos
  
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 0  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(nivel);

  if (nivel == HIGH) {
  servos0.setPWM(0, 0 ,pos110);        //si la variable nivel esta en valor HIGH el servo va a la posicion 180 grados//
  }
  else {  
  servos0.setPWM(0, 0 ,pos180);        //si la variable nivel esta en valor, no HIGH,bajo, el servo va a la posicion 0 grados//
  }
  delay(5);

  //Para consultar funcion servos0.setPWM ver https://learn.adafruit.com/16-channel-pwm-servo-driver?view=all
  // Hasta este punto se repite otras 15 vezez cambiando el numero de entrada y de salida a servo//
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 1  //


  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 1:");
Serial.print(nivel);

  if (nivel == HIGH) {
    servos0.setPWM(1, 0 ,pos180);      //si la variable nivel esta en valor HIGH el servo va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(1, 0 ,pos0);        //si la variable nivel esta en valor, no HIGH,bajo, el servo va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 2  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 2:");
Serial.print(nivel);
  if (nivel == HIGH) {
    servos1.setPWM(2 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 2A va a la posicion 0 grados//
    servos1.setPWM(8 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 2B va a la posicion 0 grados//
  }
  else {
    servos1.setPWM(2 ,0 ,pos180);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 2A va a la posicion 180 grados//
    servos1.setPWM(8 ,0 ,pos180);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 2B va a la posicion 180 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 3  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 3:");
Serial.print(nivel);
  if (nivel == HIGH) {
    servos1.setPWM(3 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 3A va a la posicion 0 grados//
  }
  else {
    servos1.setPWM(3 ,0 ,pos180);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 3A va a la posicion 180 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 4  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 4:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos1.setPWM(4 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 4A va a la posicion 0 grados//
  }
  else {
    servos1.setPWM(4 , 0 ,pos180);     //si la variable nivel esta en valor, no HIGH,bajo, el servo 4a va a la posicion 180 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 5  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 5:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos1.setPWM(5, 0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 5A va a la posicion 0 grados//
  }
  else {
    servos1.setPWM(5, 0 ,pos180);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 5A va a la posicion 180 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 6  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 6:");
Serial.print(nivel);
  if (nivel == HIGH) {
    servos1.setPWM(0  ,0 ,pos0);       //si la variable nivel esta en valor HIGH el servo 6A va a la posicion 0 grados//
  }
  else {
    servos1.setPWM(0 , 0 ,pos180);     //si la variable nivel esta en valor, no HIGH,bajo, el servo 6A va a la posicion 180 grados//
  }
  delay(5);

                                       
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , LOW);           //puntero Vete a entrada Multiplexora 7  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //
Serial.print(" 7:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos0.setPWM(7  ,0 ,pos0);       //si la variable nivel esta en valor HIGH el servo 7A va a la posicion 0 grados//
  }
  else {
    servos0.setPWM(7 , 0 ,pos180);     //si la variable nivel esta en valor, no HIGH,bajo, el servo 7A va a la posicion 180 grados//
  }
  delay(5);

  
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 8  // 

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 8:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos0.setPWM(8 ,0 ,pos180);      //si la variable nivel esta en valor HIGH el servo 8A va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(8 , 0 ,pos0);       //si la variable nivel esta en valor, no HIGH,bajo, el servo 8A va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 9  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

 if (nivel == HIGH) {
    servos0.setPWM(1 ,0 ,180);         //si la variable nivel esta en valor HIGH el servo va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(1 , 0 ,0);          //si la variable nivel esta en valor, no HIGH,bajo, el servo va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 10  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 10:");
Serial.print(nivel);
  if (nivel == HIGH) {
    servos0.setPWM(10 ,0 ,pos180);     //si la variable nivel esta en valor HIGH el servo 10A va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(10 , 0 ,pos0);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 10A va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , LOW);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 11  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 11:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos0.setPWM(11 ,0 ,pos0);       //si la variable nivel esta en valor HIGH el servo 11A va a la posicion 0 grados//
  }
  else {
    servos0.setPWM(11 , 0 ,pos180);    //si la variable nivel esta en valor, no HIGH,bajo, el servo 11A va a la posicion 180 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 12  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 12:");
Serial.print(nivel);
 if (nivel == HIGH) {
    servos0.setPWM(2 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 12B va a la posicion 180 grados//
    servos0.setPWM(12 ,0 ,pos180);     //si la variable nivel esta en valor HIGH el servo 12A va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(2 ,0 ,pos180);      //si la variable nivel esta en valor, no HIGH,bajo, el servo 12B va a la posicion 0 grados//
    servos0.setPWM(12 ,0 ,pos0);       //si la variable nivel esta en valor, no HIGH,bajo, el servo 12A va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , LOW);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , HIGH);          //puntero Vete a entrada Multiplexora 13  //

  nivel = digitalRead(muxSIG);         //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 13:");
Serial.print(nivel);
  if (nivel == HIGH) {
    servos0.setPWM(3 ,0 ,pos0);        //si la variable nivel esta en valor HIGH el servo 13B va a la posicion 180 grados//
    servos0.setPWM(13 ,0 ,pos180);   //si la variable nivel esta en valor HIGH el servo 13A va a la posicion 180 grados//
  }
  else {
    
    servos0.setPWM(3 , 0 ,pos180);   //si la variable nivel esta en valor, no HIGH,bajo, el servo 13B va a la posicion 0 grados//
    servos0.setPWM(13 , 0 ,pos0);    //si la variable nivel esta en valor, no HIGH,bajo, el servo 13A va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , LOW);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , HIGH);        //puntero Vete a entrada Multiplexora 14  //

  nivel = digitalRead(muxSIG);       //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 14:");
Serial.print(nivel);

  if (nivel == HIGH) {
    servos0.setPWM(4 ,0 ,pos0);      //si la variable nivel esta en valor HIGH el servo 14B va a la posicion 180 grados//
    servos0.setPWM(14 ,0 ,pos180);   //si la variable nivel esta en valor HIGH el servo 14A va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(4 ,0 ,pos180);     //si la variable nivel esta en valor HIGH el servo 14B va a la posicion 180 grados//
    servos0.setPWM(14 , 0 ,pos0);     //si la variable nivel esta en valor, no HIGH,bajo, el servo 14A va a la posicion 0 grados//
  }
  delay(5);
  digitalWrite(muxS0 , HIGH);
  digitalWrite(muxS1 , HIGH);
  digitalWrite(muxS2 , HIGH);
  digitalWrite(muxS3 , HIGH);        //puntero Vete a entrada Multiplexora 15  //

  nivel = digitalRead(muxSIG);       //leer valor en entrada A0 que es la salida SIG del multiplexor //

Serial.print(" 15:");
Serial.println(nivel);
  if (nivel == HIGH) {
    servos0.setPWM(15 ,0 ,pos180);   //si la variable nivel esta en valor HIGH el servo 15A va a la posicion 180 grados//
  }
  else {
    servos0.setPWM(15 , 0 ,pos0);    //si la variable nivel esta en valor, no HIGH,bajo, el servo 15A va a la posicion 0 grados//
  
  }  
  }
  
 
