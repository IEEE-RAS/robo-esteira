#define X 0
#define Y 1
#define Z 2

int angAnt[] = {0, 0, 0};
int angAtual[] = {0, 0, 0};
int bufferIMU[] = {0, 0, 0};

int variacaoInicial = 50;

void setup() {
  
}

void loop() {
  lerSensor();
}

void lerSensor(){
  int bufferTEMP[] = {0,0,0};
  while(true){ //<-- Enquanto no tiver uma variação desejada, fica parado no lopp
    //Aqui vai o codigo da leitura do sensor
    // getBufferIMU(bufferTEMP);
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



