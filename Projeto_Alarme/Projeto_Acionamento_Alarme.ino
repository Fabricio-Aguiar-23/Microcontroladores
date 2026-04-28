#define RED PB12
#define YELLOW PB13
#define GREEN PB14
#define RES PA0
#define BUTTON PB15

int sinal;
bool alarme=false;


void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(RES,INPUT);
  pinMode(BUTTON,INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  sinal = analogRead(RES);
  
  if(!alarme){
    Serial.print("Sinal: ");
    Serial.print(sinal);

    if(sinal>=0 && sinal<1500){
      digitalWrite(GREEN,HIGH);
      digitalWrite(RED,LOW);
      digitalWrite(YELLOW,LOW);
      Serial.println(". Estado Normal");
    }
    else if(sinal>=1500 && sinal<3000){
      digitalWrite(YELLOW,HIGH);
      digitalWrite(RED,LOW);
      digitalWrite(GREEN,LOW);
      Serial.println(". Estado de Atencao");
    }
    else if(sinal>=3000 && sinal<4096){
      digitalWrite(RED,HIGH);
      digitalWrite(YELLOW,LOW);
      digitalWrite(GREEN,LOW);
      alarme=true;
      Serial.println(". Estado de Alarme");
    }
    else{
      Serial.print("Erro de Leitura com valor de ");
      Serial.println(sinal);
    }
  } else if(alarme){
    Serial.print("Alerta: ");
    Serial.println(sinal);

    delay(10);
    static bool estadoAnterior = HIGH;
    bool estadoAtual = digitalRead(BUTTON);

    if(estadoAnterior == HIGH && estadoAtual == LOW){
      delay(2000); // Delay de 2 segundos para ajustar valor no potenciômetro 
      sinal = analogRead(RES);

      if(sinal<3000){
        alarme=false;
      }
      else{
        alarme=true;
      }
    }
    estadoAnterior = estadoAtual;
  }
  delay(10);
}