// Código de funcionamento Receptor RF
// Projeto de comunicação sem fio Arduino
// Fonte: http://blog.usinainfo.com.br/comunicacao-sem-fio-arduino-transferindo-dados-com-transmissor-receptor-rf-433mhz/
// Download da biblioteca Virtual Wire: http://blog.usinainfo.com.br/wp-content/uploads/2018/02/VirtualWire.zip

#include <VirtualWire.h>          // Inclui biblioteca necessária para comunicação
#define DATA_PIN 13               

byte message[VW_MAX_MESSAGE_LEN];    // Variável para armazenagem de mensagens

byte messageLength = VW_MAX_MESSAGE_LEN;    // Tamanho da mensagem

void setup() {

  Serial.begin(9600);         // Configura a comunicação serial em 9600

  Serial.println("Dispositivo pronto para receber dados!"); //Imprime na Porta Serial

  vw_set_rx_pin(11);         // Configura o pino Digital utilizado pelo Módulo Receptor
  vw_setup(2000);            // Bits por segundo
  vw_rx_start();             // Inicializa o receptor
}

void loop() {
  if (vw_get_message(message, &messageLength)) {     // Elimina o bloqueio

    Serial.print("Recebido: ");      // Imprime na porta serial a mensagem em destaque
    Serial.write(message[0]);

    if (message[0] == '0') {           // Se receber o valor 0
      digitalWrite(DATA_PIN, LOW);             // Põem o pino 13 em estado LOW
    }

    else if (message[0] == '1') {      // Se receber o valor 1
      digitalWrite(DATA_PIN, HIGH);            // Põem o pino 13 em estado HIGH
    }

    Serial.println();
  }
}
