#define LED 13
#define LDR 15
#define BUTTON 4

int luminosidade;
bool modo_aut = true;

// Duplo clique
unsigned long tempoClique = 0;
unsigned long intervaloDuplo = 300;
bool esperandoSegundoClique = false;

// Inatividade
unsigned long ultimoClique = 0;
unsigned long tempoInatividade = 60000; // 1 minuto

// Amostragem de ambiente
unsigned long ultimoCheck = 0;
unsigned long intervaloAmostragem = 300000; // 5 minutos

void alteraLED(){
  if(digitalRead(LED)==1){
    digitalWrite(LED,LOW); 
  }
  else{
    digitalWrite(LED,HIGH);
  }
  modo_aut=false;
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {

  // ---------------- MODO AUTOMÁTICO ----------------
  if(modo_aut){

    // Verifica se é hora de fazer amostragem 
    if (millis() - ultimoCheck > intervaloAmostragem) {

      ultimoCheck = millis();

      Serial.println("Amostrando luz ambiente real...");

      digitalWrite(LED, LOW);

      delay(50); 

      luminosidade = analogRead(LDR);

      Serial.print("Luminosidade: ");
      Serial.println(luminosidade);

      if(luminosidade>=2700 && luminosidade<4096){
        digitalWrite(LED,HIGH); 
      }
      else if(luminosidade<2700 && luminosidade>0){
        digitalWrite(LED,LOW); 
      }
      else{
        Serial.println("Erro de Leitura");
        modo_aut = false;
      }
    }
  }

  // Verifica acionamento do botão
  static bool estadoAnterior = HIGH;
  bool estadoAtual = digitalRead(BUTTON);

  if (estadoAnterior == HIGH && estadoAtual == LOW) {

    ultimoClique = millis();

    if (!esperandoSegundoClique) {
      esperandoSegundoClique = true;
      tempoClique = millis();
    } else {
      if (millis() - tempoClique <= intervaloDuplo) {
        Serial.println("Duplo clique detectado");
        modo_aut = true;
        esperandoSegundoClique = false;
      }
    }
  }

  if (esperandoSegundoClique && (millis() - tempoClique > intervaloDuplo)) {
    Serial.println("Clique simples");
    esperandoSegundoClique = false;
    alteraLED();
  }

  estadoAnterior = estadoAtual;

  // Inatividade
  if (!modo_aut && (millis() - ultimoClique > tempoInatividade)) {
    Serial.println("Retornando para modo automático");
    modo_aut = true;
  }

  delay(50);
}