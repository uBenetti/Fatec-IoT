const int PINO_LDR = A0;
const int PINO_PIR = 2;
const int PINO_RELE = 8;

const int LIMIAR_ESCURIDAO = 600;

void setup() {
  pinMode(PINO_PIR, INPUT);

  pinMode(PINO_RELE, OUTPUT);

  digitalWrite(PINO_RELE, LOW);

  Serial.begin(9600);

  Serial.println("=================================");
  Serial.println(" SISTEMA DE SEGURANCA NOTURNO");
  Serial.println("=================================");
}

void loop() {

  int valorLDR = analogRead(PINO_LDR);
  int movimento = digitalRead(PINO_PIR);

  bool ambienteEscuro = valorLDR >= LIMIAR_ESCURIDAO;

  if (ambienteEscuro) {

    if (movimento == HIGH) {
      digitalWrite(PINO_RELE, HIGH);

      Serial.println("ESCURO + MOVIMENTO -> RELE LIGADO");

    } else {
      digitalWrite(PINO_RELE, LOW);

      Serial.println("ESCURO + SEM MOVIMENTO -> RELE DESLIGADO");
    }

  } else {
    digitalWrite(PINO_RELE, LOW);

    Serial.println("CLARO -> SISTEMA DESATIVADO");
  }

  Serial.print("Valor LDR: ");
  Serial.print(valorLDR);

  Serial.print(" | Movimento: ");
  Serial.print(movimento == HIGH ? "SIM" : "NAO");

  Serial.print(" | Ambiente: ");
  Serial.println(ambienteEscuro ? "ESCURO" : "CLARO");

  Serial.println("---------------------------------");

  delay(500);
}