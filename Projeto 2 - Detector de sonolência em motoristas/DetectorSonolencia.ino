#include <Wire.h>
#include <math.h>

const int MPU = 0x68;
const int BUZZER = 8;

const float ANGULO_SONOLENCIA = 25.0;

const float ANGULO_NORMAL = 10.0;

const float VELOCIDADE_MAXIMA = 30.0;

const unsigned long TEMPO_DETECCAO = 1000;

int16_t AcX, AcY, AcZ;
int16_t GyX, GyY, GyZ;

float Ax, Ay, Az;
float Gx, Gy, Gz;

float magnitude;
float inclinacao;

bool sonolencia = false;

unsigned long inicioInclinacao = 0;

void setup() {

  Serial.begin(9600);

  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);

  Wire.begin();

  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  Serial.println();
  Serial.println("======================================");
  Serial.println("      DETECTOR DE SONOLENCIA");
  Serial.println("======================================");
  Serial.println("MPU6050 iniciado!");
  Serial.println("Sistema pronto.");
  Serial.println();
}

void lerMPU6050() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

  Wire.read();
  Wire.read();

  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  Ax = (AcX / 16384.0) * 9.80665;
  Ay = (AcY / 16384.0) * 9.80665;
  Az = (AcZ / 16384.0) * 9.80665;

  Gx = GyX / 131.0;
  Gy = GyY / 131.0;
  Gz = GyZ / 131.0;
}

void loop() {

  lerMPU6050();

  magnitude = sqrt(
    (Ax * Ax) +
    (Ay * Ay) +
    (Az * Az)
  );

  inclinacao = atan2(
    Ax,
    sqrt((Ay * Ay) + (Az * Az))
  );

  inclinacao = inclinacao * 180.0 / PI;

  float velocidadeAngular = sqrt(
    (Gx * Gx) +
    (Gy * Gy) +
    (Gz * Gz)
  );

  bool cabecaInclinada =
    abs(inclinacao) >= ANGULO_SONOLENCIA;

  bool cabecaEstavel =
    velocidadeAngular <= VELOCIDADE_MAXIMA;

  if (!sonolencia && cabecaInclinada && cabecaEstavel) {

    if (inicioInclinacao == 0) {
      inicioInclinacao = millis();
    }

    if (millis() - inicioInclinacao >= TEMPO_DETECCAO) {

      sonolencia = true;

      tone(BUZZER, 1000);

      Serial.println();
      Serial.println("**************************************");
      Serial.println("     ALERTA DE SONOLENCIA!");
      Serial.println("     BUZZER ATIVADO!");
      Serial.println("**************************************");
    }

  } else {
    if (!sonolencia) {
      inicioInclinacao = 0;
    }
  }

  if (sonolencia && abs(inclinacao) <= ANGULO_NORMAL) {

    sonolencia = false;
    inicioInclinacao = 0;

    noTone(BUZZER);

    Serial.println();
    Serial.println("--------------------------------------");
    Serial.println("     CABECA VOLTOU AO NORMAL");
    Serial.println("     BUZZER DESLIGADO");
    Serial.println("--------------------------------------");
  }

  Serial.print("Ax: ");
  Serial.print(Ax, 2);

  Serial.print(" | Ay: ");
  Serial.print(Ay, 2);

  Serial.print(" | Az: ");
  Serial.print(Az, 2);

  Serial.print(" | Magnitude: ");
  Serial.print(magnitude, 2);

  Serial.print(" | Inclinacao: ");
  Serial.print(inclinacao, 2);

  Serial.print(" graus");

  Serial.print(" | Gx: ");
  Serial.print(Gx, 1);

  Serial.print(" | Gy: ");
  Serial.print(Gy, 1);

  Serial.print(" | Gz: ");
  Serial.print(Gz, 1);

  Serial.print(" | Vel.Angular: ");
  Serial.print(velocidadeAngular, 1);

  Serial.print(" | STATUS: ");

  if (sonolencia) {
    Serial.println("SONOLENCIA");
  } else {
    Serial.println("NORMAL");
  }

  delay(300);
}