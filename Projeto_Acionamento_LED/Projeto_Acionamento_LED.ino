#define LED 13
#define LDR 15
#define BUTTON 4

// Leitura de dados e Modo Automático
int luminosidade;
bool modo_aut=true;

// Lógica para duplo clique
unsigned long tempoClique = 0;
unsigned long intervaloDuplo = 300;
bool esperandoSegundoClique = false;

// Inatividade de interação manual
unsigned long ultimoClique = 0;
unsigned long tempoInatividade = 60000; // 1 minuto

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
  pinMode(LED,OUTPUT);
  pinMode(LDR,INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.begin(115200);
}


void loop() {
  // Modo Automático
  // Verifica se está no modo automático
  /*
      Caso baixa luminosidade: acende
      Caso luminosidade normal: apaga
      Caso o sensor retorne valores errados: entra modo manual mas deixa a lâmpada acesa
  */
  if(modo_aut==true){
    luminosidade=analogRead(LDR);
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


  // Acionamento para retorno ao modo automático
  /*
    Verifica se houve acionamento por descida (antes era alto e depois ficou baixo)
      Se houve clique, atualizo o tempo do último clique
      Verifica se estou não esperando próximo clique
  */
  static bool estadoAnterior = HIGH;
  bool estadoAtual = digitalRead(BUTTON);

  // Detecta clique
  if (estadoAnterior == HIGH && estadoAtual == LOW) {
    
    ultimoClique = millis(); // Atualiza o tempo em que ocorreu o último clique para verificar inatividade

    if (!esperandoSegundoClique) {
      // Primeiro clique
      esperandoSegundoClique = true;
      tempoClique = millis();
    } else {
      // Segundo clique dentro do tempo --> Modo Automático
      if (millis() - tempoClique <= intervaloDuplo) {
        Serial.println("Duplo clique detectado");
        modo_aut=true;
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

  // Retorno automático após inatividade
  if (!modo_aut && (millis() - ultimoClique > tempoInatividade)) {
    Serial.println("Retornando para modo automático");
    modo_aut = true;
  }

  delay(50);
}
