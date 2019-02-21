// Código de funcionamento Receptor RF
// Projeto de comunicação sem fio Arduino
// Fonte: http://blog.usinainfo.com.br/comunicacao-sem-fio-arduino-transferindo-dados-com-transmissor-receptor-rf-433mhz/
// Download da biblioteca Virtual Wire: http://blog.usinainfo.com.br/wp-content/uploads/2018/02/VirtualWire.zip
// Download biblioteca ArduinoJson.h: https://github.com/bblanchon/ArduinoJson

#include <VirtualWire.h>          // Inclui biblioteca necessária para comunicação
#define DATA_PIN 11
#define TEMPO 750
#include <ArduinoJson.h>        //biblioteca para protocolo de comunicação json


//vetor com angulos
int angulos [4] = {0, 0, 0, 0}; //x, y, z, p -> estado da garra (0 para aberto, 1 para fechado)
char movimento;
byte messageLength  = 80;    // Tamanho da mensagem

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  Serial.begin(115200 );         // Configura a comunicação serial em 9600

  Serial.println("Dispositivo pronto para receber dados!"); //Imprime na Porta Serial

  vw_set_rx_pin(DATA_PIN);         // Configura o pino Digital utilizado pelo Módulo Receptor
  vw_setup(2000);            // Bits por segundo
  vw_rx_start();             // Inicializa o receptor
  digitalWrite(DATA_PIN, LOW);

  pinMode(12, OUTPUT); // VCC para recptor de radio
  digitalWrite(12, HIGH);

  pinMode(10, INPUT); // Necessario para o radio funcionar nesta conexão.
  
  pinMode(9, OUTPUT); //GND para receptor de radio
  digitalWrite(9, LOW);
}

void getMessage(byte message[]){
  clearBuffer(message, 80);
  if (vw_get_message(message, &messageLength)) {     // Elimina o bloqueio
    int i;
    char converted[messageLength];
    convertByteChar(message, messageLength, converted);
    receivedJson(converted);
  }
}

void loop() {
  byte message[VW_MAX_MESSAGE_LEN];    // Variável para armazenagem de mensagens
  getMessage(message);
  moveAll(); 
}

void clearBuffer(byte *v, int t) {
  int j;
  for (j = 0; j < t; j++) {
    v[j] = '\0';
  }
}

/*parte de conversão de ângulos*/


//Função para enviar comando apenas para um motor.
void moveOne (int servo, int pos, int Time) {
  Serial.print("#");
  Serial.print(servo);
  Serial.print(" P");
  Serial.print(pos, 0);
  Serial.print(" T");
  Serial.println(Time);
  delay(Time);
}

//Função para enviar comando para todos os motores.
void moveAll () {
  int i;
  for (i = 0; i < 4; i++) {
    Serial.print("#");
    Serial.print(i);
    Serial.print(" P");
    Serial.print(convertDegrees (angulos[i]));
    Serial.print(" ");
  }
  Serial.print("T");
  Serial.println (TEMPO);
}


//Função que converte graus para ms.
int convertDegrees (int graus) {
  return map(graus, -90, 90, 500, 1500);
}

void convertByteChar(byte a[], int t, char *b) {
  int i;
  for (i = 0; i < t; i++) {
    b[i] = (char)a[i];
  }
}

void receivedJson(char *message) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  Serial.println(message);
  angulos[0] = root["x"];
  angulos[1] = root["y"];
  angulos[2] = root["z"];
  angulos[3] = root["p"]; 
}
