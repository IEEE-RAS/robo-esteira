#include "MPU9250.h"
#include <VirtualWire.h> // Inclui biblioteca necessária para comunicação
#include <ArduinoJson.h>

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;

//Usado para calcular o angulo - Variaveis do acelerometro
double accXangle;
double accYangle;
double accZangle;
//Usado para calcular o angulo - Variaveis do giroscopio
double gyroXangle = 0;
double gyroYangle = 0;
double gyroZangle = 0;

uint32_t timer;

typedef struct {
  double gyroXangle;
  double gyroYangle;
  double gyroZangle; 
  
} Values;


void setup() {
// serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  Serial.println("Serial started");

  vw_set_tx_pin(11);       // Configura o pino Digital utilizado pelo Módulo Transmissor
  vw_setup(2000);          // Bits per segundo

  // start communication with IMU 
  status = IMU.begin();
  status = IMU.setGyroRange(MPU9250::GYRO_RANGE_1000DPS);
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  timer = micros();
}


void loop() {

  //Read the sensor
  IMU.readSensor();
  Values r = getGyro();
  sendJson(r.gyroXangle, r.gyroYangle, r.gyroZangle, 0);
}

Values getGyro(){
  int16_t ax = IMU.getAccelX_mss(), ay = IMU.getAccelY_mss(), az = IMU.getAccelZ_mss();
  int16_t gx = IMU.getGyroX_rads(), gy = IMU.getGyroY_rads(), gz = IMU.getGyroZ_rads();
  
  // Calcular os angulos com base nos sensores do acelerometro
  accXangle = (atan2(ax,az) + PI) * RAD_TO_DEG;
  accYangle = (atan2(ay,az) + PI) * RAD_TO_DEG;
  accZangle = (atan2(ax,ay) + PI) * RAD_TO_DEG;  

  double gyroXrate = ((double)gx / 131.0);
  double gyroYrate = -((double)gy / 131.0);
  double gyroZrate = -((double)gz / 131.0);
  
  //###################### Calcular o ângulo de giro sem qualquer filtro ######################### 
  gyroXangle += gyroXrate*((double)(micros()-timer)/100); 
  gyroYangle += gyroYrate*((double)(micros()-timer)/100);
  gyroZangle += gyroZrate*((double)(micros()-timer)/100);

  timer = micros();
  Values r;
  r.gyroXangle = gyroXangle;
  r.gyroYangle = gyroYangle;
  r.gyroZangle = gyroZangle;
//  delay(1);
  return r;
}

void send (char *message) {
  Serial.println(message);
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();                  // Aguarda o envio de dados
}

void sendJson(double x, double y, double z, double p) {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["x"] = (int)x;
    root["y"] = (int)y;
    root["z"] = (int)z;
    root["p"] = p;
    root["time"] = micros();
    
    char json[100];

    root.printTo(json);

//    sprintf(json, "{\"x\":%f,\"y\":%f,\"z\":%f, \"p\":%f, \"timer\":%d}", f, y, z, p, micros());
    Serial.println(json);
    send(json); //Envia via radio
}
