#include <VirtualWire.h> // Inclui biblioteca necessária para comunicação
#include <Wire.h>
#include <String.h>
#include <L3G4200D.h>

#define X 0
#define Y 1
#define Z 2

int angAnt[] = {0, 0, 0};
int angAtual[] = {0, 0, 0};
int bufferIMU[] = {0, 0, 0};
L3G4200D gyro;

int variacaoInicial = 50;

void setup() {
  Serial.begin(9600);      // Configura a comunicação serial em 9600

  vw_set_tx_pin(12);       // Configura o pino Digital utilizado pelo Módulo Transmissor
  vw_setup(2000);          // Bits per segundo

  Wire.begin();
  gyro.enableDefault();
  Serial.println("Digite o texto que deseja Enviar ..."); //Imprime na Porta Serial
}

void loop() {
 // lerSensor();
  char data[40];
  int num;
  angAtual[X] = "20";
  angAtual[Y] = "50";
  angAtual[Z] = "90";

  char string[50]; //= (char)angAtual[X] +" "+ (char)angAtual[Y] +" "+ (char)angAtual[Z];
  strcat(string, angAtual[X]);
  strcat(string, ",");
  strcat(string, angAtual[Y]);
  strcat(string, ",");
  strcat(string, angAtual[Z]);
  strcat(string, ";");
  Serial.println(string);
  send(string);                    // Envia o valor para o receptor
 strcpy( string, "" );
}

void lerSensor(){
  int bufferTEMP[] = {0,0,0};
  while(true){ //<-- Enquanto no tiver uma variação desejada, fica parado no lopp
    //Aqui vai o codigo da leitura do sensor
    getBufferIMU(bufferTEMP);
    if(bufferIMU[X] - bufferTEMP[X] > variacaoInicial || bufferIMU[Y] - bufferTEMP[Y] < variacaoInicial || bufferIMU[Z] - bufferTEMP[Z] < variacaoInicial){
      angAtual[X] = bufferTEMP[X]; angAtual[Y] = bufferTEMP[Y]; angAtual[Z] = bufferTEMP[Z];
      break;
    }
    bufferIMU[X] = bufferTEMP[X]; bufferIMU[Y] = bufferTEMP[Y];bufferIMU[Z] = bufferTEMP[Z];
  }
  angAtual[X] += angAnt[X]; // Coordenada X
  angAtual[Y] += angAnt[Y]; // Coordenada Y
  angAtual[Z] += angAnt[Z]; // Coordenada Z
}

void send (char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();                  // Aguarda o envio de dados
}

void getBufferIMU(int* bufferTEMP){
  gyro.read();
  bufferTEMP[X] = (int)gyro.g.x;
  bufferTEMP[Y] = (int)gyro.g.y;
  bufferTEMP[Z] = (int)gyro.g.z;
  delay(100);
  }

