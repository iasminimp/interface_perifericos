#include <Servo.h>  // Inclui a Biblioteca

Servo Horizontal;   // Inicia o Servo da Horizontal
Servo Vertical;     // Inicia o Servo da Vertical

int ServoHorizontal = 90;  // Estabelece valor fixo à ServoHorizontal
int ServoVertical = 90;    // Estabelece valor fixo à ServoVertical

int LimiteServoHorizontalMax = 180;  // Estabele os limites de rotação
int LimiteServoHorizontalMin = 65;   // Estabele os limites de rotação

int LimiteServoVerticalMax = 120;    // Estabele os limites de rotação
int LimiteServoVerticalMin = 15;     // Estabele os limites de rotação

int LDRDC = A0;  // Inicia LDRDC no pino A0
int LDRDB = A1;  // Inicia LDRDB no pino A1
int LDREC = A2;  // Inicia LDREC no pino A2
int LDREB = A3;  // Inicia LDREB no pino A3

bool manualMode = false; // Flag para saber se está no modo manual
unsigned long lastSerialTime = 0; // Tempo da última mensagem recebida

void setup() {
  Serial.begin(9600);      // Inicia a comunicação serial
  Horizontal.attach(3);    // Inicia servo Horizontal na porta D3
  Vertical.attach(5);      // Inicia servo Vertical na porta D5

  Horizontal.write(ServoHorizontal);   // Inicia servo Horizontal na posição 90
  Vertical.write(ServoVertical);       // Inicia servo Vertical na posição 90
}

void loop() {
  
  if (Serial.available()) {
    String serialData = Serial.readStringUntil('\n'); // Lê os dados enviados pela interface web
    int commaIndex = serialData.indexOf(',');

    if (commaIndex > 0) {
      int h = serialData.substring(0, commaIndex).toInt();
      int v = serialData.substring(commaIndex + 1).toInt();

      if (h >= LimiteServoHorizontalMin && h <= LimiteServoHorizontalMax && 
          v >= LimiteServoVerticalMin && v <= LimiteServoVerticalMax) {
        ServoHorizontal = h;
        ServoVertical = v;
        Horizontal.write(ServoHorizontal);
        Vertical.write(ServoVertical);

        manualMode = true;
        lastSerialTime = millis();
      }
    }
  }

  if (manualMode) {
    if (millis() - lastSerialTime > 5000) { // Volta ao modo automático após 5 segundos sem dados manuais
      manualMode = false;
    }
  }

  if (!manualMode) {
    // Modo automático: leituras dos LDRs e movimentação dos servos
    int LDC = analogRead(LDRDC);      // Leitura Analógica do LDR Direita Cima
    int LEC = analogRead(LDREC);      // Leitura Analógica do LDR Esquerda Cima
    int LDB = analogRead(LDRDB);      // Leitura Analógica do LDR Direita Baixo
    int LEB = analogRead(LDREB);      // Leitura Analógica do LDR Esquerda Baixo

    int tol = 50;

    int ValorSup = (LDC + LEC) / 2;   // Média da leitura dos LDR superior
    int ValorInf = (LDB + LEB) / 2;   // Média da leitura dos LDR inferior
    int ValorDir = (LDC + LDB) / 2;   // Média da leitura dos LDR da direita
    int ValorEsq = (LEC + LEB) / 2;   // Média da leitura dos LDR da esquerda

    int DifSupInf = ValorSup - ValorInf;      // Diferença entre LDR superior e inferior
    int DifDirEsq = ValorDir - ValorEsq;      // Diferença entre LDR direita e esquerda

    // Realiza a leitura e executa os movimentos referente ao Servo Vertical
    if (abs(DifSupInf) > tol) {
      if (ValorSup > ValorInf) {
        ServoVertical++;
        if (ServoVertical > LimiteServoVerticalMax) {
          ServoVertical = LimiteServoVerticalMax;
        }
      } else if (ValorSup < ValorInf) {
        ServoVertical--;
        if (ServoVertical < LimiteServoVerticalMin) {
          ServoVertical = LimiteServoVerticalMin;
        }
      }
     
    }

    // Realiza a leitura e executa os movimentos referente ao Servo Horizontal
    if (abs(DifDirEsq) > tol) {
      if (ValorDir > ValorEsq) {
        ServoHorizontal--;
        if (ServoHorizontal < LimiteServoHorizontalMin) {
          ServoHorizontal = LimiteServoHorizontalMin;
        }
      } else if (ValorDir < ValorEsq) {
        ServoHorizontal++;
        if (ServoHorizontal > LimiteServoHorizontalMax) {
          ServoHorizontal = LimiteServoHorizontalMax;
        }
      }
      
    }
    
   Serial.print(LDC);
   Serial.print(",");
   Serial.print(LEC);
   Serial.print(",");
   Serial.print(LDB);
   Serial.print(",");
   Serial.print(LEB);
   Serial.print("\n");

  // Serial.print(DifSupInf);
  // Serial.print(",");
  // Serial.print(DifDirEsq);
  // Serial.print(",");
  // Serial.print("\n");

    // Envia os ângulos dos servos pela porta serial
   
  }


   

  Vertical.write(ServoVertical);
  Horizontal.write(ServoHorizontal);
  Serial.print(ServoHorizontal);
  Serial.print(",");
  Serial.print(ServoVertical);
  Serial.print("\n");

  delay(100);   // Aguarda 1 segundo
}