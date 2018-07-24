// Código de funcionamento Transmissor RF
// Projeto de comunicação sem fio Arduino
// Fonte: http://blog.usinainfo.com.br/comunicacao-sem-fio-arduino-transferindo-dados-com-transmissor-receptor-rf-433mhz/
// Download da biblioteca Virtual Wire: http://blog.usinainfo.com.br/wp-content/uploads/2018/02/VirtualWire.zip

#include <VirtualWire.h> // Inclui biblioteca necessária para comunicação

void setup() {
  Serial.begin(9600);      // Configura a comunicação serial em 9600

  vw_set_tx_pin(12);       // Configura o pino Digital utilizado pelo Módulo Transmissor
  vw_setup(2000);          // Bits per segundo

  Serial.println("Digite o texto que deseja Enviar ..."); //Imprime na Porta Serial
}

void loop() {
  char data[40];
  int num;

  if (Serial.available() > 0)  {
    num = Serial.readBytesUntil (13, data, 40); // Retorna o número de caracteres lidos no buffer. Um 0 significa que nenhum dado válido foi encontrado.
    data[num] = 0;
    Serial.print("Enviado : ");    // Imprime na porta serial a mensagem em destaque
    Serial.println(data);          // Imprime o valor enviado

    send(data);                    // Envia o valor para o receptor
  }
}

void send (char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();                  // Aguarda o envio de dados
}
