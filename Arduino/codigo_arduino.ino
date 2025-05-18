#include <SoftwareSerial.h>

// Pines del BTS7960
#define RPWM 10 //blanoco
#define LPWM 11 //marron
#define R_EN 12  //naranja
#define L_EN 9  //amarillo

// Bluetooth (por SoftwareSerial)
SoftwareSerial BT(0, 1);  // RX, TX del Bluetooth
String  c;
String estado;
bool conectado = false;
String value;
bool  encendido;

void setup() {
    Serial.begin(9600);  // Monitor Serial (PC)
    BT.begin(9600);      // Bluetooth

    // Configurar pines del motor
    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);

    Serial.println("Esperando conexión Bluetooth...");
}

void loop() {
    // Si hay datos desde Bluetooth
    if (BT.available() and !conectado) {
      
            conectado = true;
            Serial.println(" ✅ Dispositivo Bluetooth conectado.");
    }
    if (conectado) {
         static int velocidad = 0;
        value= BT.readString();
        //Serial.println(value);
        
        if(value=="A"){
          Serial.println(" Comando: ENCENDER ");
          moveMotor( 150 ); //valor por defecto para arracnque
          encendido= true;   
         
        }else{

        }
        if(value=="B"){
          Serial.println(" Comando: APAGAR ");
          stopMotor();
          encendido=false;
        }

        if(encendido){
            String vel= BT.readString();
            estado  += vel;
            if(estado.length()>0){
                moveMotor(estado.toInt());
                estado="";
            }
         
        }
     
    }
delay(100);
    
}





void moveMotor(int speed) {
    Serial.println("en la funcion motorspeed");
    if (speed >= 0 && speed <= 255) {
        analogWrite(RPWM, speed);
        analogWrite(LPWM, 0);
        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
        Serial.print("▶️ Motor derecha. Velocidad: ");
        Serial.println(speed);
    } else if (speed >= -255 && speed < 0) {
        analogWrite(RPWM, 0);
        analogWrite(LPWM, -speed);
        digitalWrite(R_EN, HIGH);
        digitalWrite(L_EN, HIGH);
        Serial.print("◀️ Motor izquierda. Velocidad: ");
        Serial.println(speed);
    }
}

void stopMotor() {
    analogWrite(RPWM, 0);
    analogWrite(LPWM, 0);
    digitalWrite(R_EN, LOW);
    digitalWrite(L_EN, LOW);
    Serial.println("✅ Motor detenido");
}
