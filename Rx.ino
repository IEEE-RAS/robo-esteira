// Código de funcionamento Receptor RF
// Projeto de comunicação sem fio Arduino
// Fonte: http://blog.usinainfo.com.br/comunicacao-sem-fio-arduino-transferindo-dados-com-transmissor-receptor-rf-433mhz/
// Download da biblioteca Virtual Wire: http://blog.usinainfo.com.br/wp-content/uploads/2018/02/VirtualWire.zip

#include <VirtualWire.h>          // Inclui biblioteca necessária para comunicação
#define DATA_PIN 13
#define TEMPO 750


//vetor com angulos
int angulos [6] = {90, -90, 0, 45, -45, 30};
char movimento;

byte message[VW_MAX_MESSAGE_LEN];    // Variável para armazenagem de mensagens

byte messageLength  = 80;    // Tamanho da mensagem

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(9600);         // Configura a comunicação serial em 9600

  Serial.println("Dispositivo pronto para receber dados!"); //Imprime na Porta Serial

  vw_set_rx_pin(11);         // Configura o pino Digital utilizado pelo Módulo Receptor
  vw_setup(2000);            // Bits por segundo
  vw_rx_start();             // Inicializa o receptor
  digitalWrite(DATA_PIN, LOW);
}

void loop() {
  clearBuffer(message, 80);

  if (vw_get_message(message, &messageLength)) {     // Elimina o bloqueio
    //Serial.println(messageLength);
    Serial.print("Recebido: ");      // Imprime na porta serial a mensagem em destaque
    int i;
    for (i = 0; i < 80; i++) {
      Serial.write(message[i]);
    }

    char convertida[messageLength];
    convesorByteChar(message, messageLength, convertida);
    atualizarVetor(convertida, messageLength);

    Serial.print("\nVetor: ");
    for (i = 0; i < 6; i++) {
      Serial.print(angulos[i]);
      Serial.print(" ; ");
    }
    Serial.println(movimento);

  }
}

int atualizarVetor(char *msg, int t){
   int i, j=0, k=0;
   char v[4];

   for(i=0; i<t && msg[i]!='\0'; i++){ //percorre o vetor de mensagem
      if(msg[i]==','){ //verifica se acabou o numero
         v[k] = '\0'; //delimita o fim da string temporaria
         angulos[j] = atoi(v); //converte de string para nÃºmero
         j++; //muda a posicao do vetor angulo
         k = 0;   //zera a string temporaria
      }else{ //acomula os digitos do nÃºmero
         v[k] = msg[i]; 
         k++;      
      }   
   
   }
   movimento = msg[i-1];      
}

void clearBuffer(byte *v, int t) {
  int j;
  for (j = 0; j < t; j++) {
    v[j] = '\0';
  }
}

/*parte de conversão de ângulos*/


//Função para enviar comando apenas para um motor.
void moverUmMotor (int servo, int pos, int Time) {
  Serial.print("#");
  Serial.print(servo);
  Serial.print(" P");
  Serial.print(pos, 0);
  Serial.print(" T");
  Serial.println(Time);
  delay(Time);
}

//Função para enviar comando para todos os seis motores.
void moverTudo (int angulos []) {
  int i;
  for (i = 0; i < 6; i++) {
    Serial.print("#");
    Serial.print(i);
    Serial.print(" P");
    Serial.print(conversorGraus (angulos[i]), 0);
    Serial.print(" ");
  }
  Serial.print("T");
  Serial.println (TEMPO);
}


//Função que converte graus para ms.
int conversorGraus (int graus) {
  return 1500 + ((1000 * graus) / 90);
}

void convesorByteChar(byte a[], int t, char b[]){
  int i;
  for (i=0; i < t; i++){
    b[i] = (char)a[i]; 
  }
}

