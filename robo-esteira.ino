#include <Wire.h>
#include <L3G4200D.h>
#include <stdio.h>
#include <VirtualWire.h> // Inclui biblioteca necessária para comunicação

L3G4200D gyro;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  vw_set_tx_pin(12);       // Configura o pino Digital utilizado pelo Módulo Transmissor
  vw_setup(2000);          // Bits per segundo
  gyro.enableDefault();
}

void loop() {
  gyro.read();
  char data[80];
  int x = (int)gyro.g.x;
  int y = (int)gyro.g.y;
  int z = (int)gyro.g.z;
  sprintf(data, "X: %d Y: %d Z: %d", x, y, z);
  
  Serial.print("Enviado : ");    // Imprime na porta serial a mensagem em destaque
  Serial.println(data);          // Imprime o valor enviado

  send(data);                    // Envia o valor para o receptor
  
  delay(100);
}

void send (char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();                  // Aguarda o envio de dados
}
