#include <NewPing.h>

// Definição dos pinos dos motores
#define MOTOR1A 9
#define MOTOR1B 8
#define MOTOR2A 12
#define MOTOR2B 13
#define VELOCIDADE1 10
#define VELOCIDADE2 11

// Definição dos pinos dos sensores infravermelhos
#define INFRA 6
#define INFRA2 7

// Definição dos pinos dos sensores ultrassônicos
#define TRIG_PIN1 5
#define ECHO_PIN1 4
#define TRIG_PIN2 3
#define ECHO_PIN2 2

#define MAX_DISTANCE 30  // Alterado para 60 cm

// Inicializa os sensores ultrassônicos usando a biblioteca NewPing
NewPing sonar1(TRIG_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIG_PIN2, ECHO_PIN2, MAX_DISTANCE);

bool autonomiaAtiva = false;

void verificar_zonas(int dist1, int dist2) {
  String zona = "";

  if(dist1 > 0 && dist1 <= MAX_DISTANCE) {
    if(dist1 <= 8) {
      zona = "Muito na Direita";
    } else if(dist1 <= 16) {
      zona = "Direita";
    } else {
      zona = "Meio";
    }
    Serial.print("Zona Direita: ");
    Serial.println(zona);
  } else {
    Serial.println("Nenhum obstáculo detectado na Direita");
  }

  if(dist2 > 0 && dist2 <= MAX_DISTANCE) {
    if(dist2 <= 8) {
      zona = "Muito na Esquerda";
    } else if(dist2 <= 16) {
      zona = "Esquerda";
    } else {
      zona = "Meio";
    }
    Serial.print("Zona Esquerda: ");
    Serial.println(zona);
  } else {
    Serial.println("Nenhum obstáculo detectado na Esquerda");
  }
}

void acelerar() {
  digitalWrite(MOTOR1A, HIGH); 
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, HIGH);   
  digitalWrite(MOTOR2B, LOW);
  analogWrite(VELOCIDADE1, 180); 
  analogWrite(VELOCIDADE2, 255); 
}

void re() {
  digitalWrite(MOTOR1A, LOW);   
  digitalWrite(MOTOR1B, HIGH);
  digitalWrite(MOTOR2A, LOW);   
  digitalWrite(MOTOR2B, HIGH);
  analogWrite(VELOCIDADE1, 255);
  analogWrite(VELOCIDADE2, 255); 
}

void direita() {
  digitalWrite(MOTOR1A, LOW);   
  digitalWrite(MOTOR1B, HIGH);
  digitalWrite(MOTOR2A, HIGH);   
  digitalWrite(MOTOR2B, LOW);
  analogWrite(VELOCIDADE1, 150); 
  analogWrite(VELOCIDADE2, 255); 
}

void esquerda() {
  digitalWrite(MOTOR1A, HIGH);   
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, LOW);   
  digitalWrite(MOTOR2B, HIGH);
  analogWrite(VELOCIDADE1, 255); 
  analogWrite(VELOCIDADE2, 150); 
}

void parar() {
  digitalWrite(MOTOR1A, LOW);   
  digitalWrite(MOTOR1B, LOW);
  digitalWrite(MOTOR2A, LOW);   
  digitalWrite(MOTOR2B, LOW);
}

void autonomia() {
  int distancia1 = sonar1.ping_cm();
  int distancia2 = sonar2.ping_cm();

  verificar_zonas(distancia1, distancia2);

  if ((distancia1 > 0 && distancia1 <= MAX_DISTANCE) || (distancia2 > 0 && distancia2 <= MAX_DISTANCE)) {
    acelerar();
    Serial.println("Atacando o obstáculo!");
  } else {
    esquerda();
    Serial.println("Nenhum obstáculo detectado. Robô girando à esquerda.");
  }

  delay(500);
}

void combo()
{
  re();
  delay(500);
  direita();
  delay(2800);
  acelerar();
  delay(500);
}

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR1A, OUTPUT);
  pinMode(MOTOR1B, OUTPUT);
  pinMode(MOTOR2A, OUTPUT);
  pinMode(MOTOR2B, OUTPUT);
  pinMode(VELOCIDADE1, OUTPUT);
  pinMode(VELOCIDADE2, OUTPUT);
  pinMode(INFRA, INPUT);
  pinMode(INFRA2, INPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  delay(500);
}

void loop() {
  if (Serial.available()) {
    int leitura = Serial.read();
    Serial.println(leitura);

    switch(leitura) {
      case 70:  // 'F' para ativar o modo autonomia
        autonomiaAtiva = true;
        Serial.println("Modo Autonomia Ativado");
        break;
      case 101:  // 'e' para desativar o modo autonomia
        autonomiaAtiva = false;
        parar();  // Parar o robô ao desativar a autonomia
        Serial.println("Modo Autonomia Desativado");
        break;
      case 66:  // 'B' para acelerar
        acelerar();
        break;
      case 67:  // 'C' para direita
        direita();
        break;
      case 68:  // 'D' para re
        re();
        break;
      case 65:  // 'A' para atacar
        acelerar();
        break;
      case 69:  // 'E' para girar à esquerda
        esquerda();  
        break;
      case 71: //G
        combo();
        break;
      default:
        break;
    }
  }

  if (autonomiaAtiva) {
    autonomia();
  }
}
