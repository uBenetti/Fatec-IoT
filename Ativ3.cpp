//Link do wokiw: https://wokwi.com/projects/473014752084068353

#define VermCar 1
#define AmarCar 2
#define VerdCar 3

#define VermPed 4
#define VerdPed 5

#define Buzzer 6

void setup() {

  pinMode(VermCar, OUTPUT);
  pinMode(AmarCar, OUTPUT);
  pinMode(VerdCar, OUTPUT);
  pinMode(VerdPed, OUTPUT);
  pinMode(VermPed, OUTPUT);

  pinMode(Buzzer, OUTPUT);
}

void loop() {

  digitalWrite(VermCar, LOW);
  digitalWrite(AmarCar, LOW);
  digitalWrite(VerdCar, HIGH);

  digitalWrite(VerdPed, LOW);
  digitalWrite(VermPed, HIGH);
  digitalWrite(Buzzer, HIGH);

  delay(4000);

  digitalWrite(VerdCar, LOW);
  digitalWrite(AmarCar, HIGH);

  digitalWrite(VermPed, HIGH);
  digitalWrite(Buzzer, HIGH);

  delay(2000);

  digitalWrite(AmarCar, LOW);
  digitalWrite(VermCar, HIGH);

  digitalWrite(VermPed, LOW);
  digitalWrite(Buzzer, LOW);

  digitalWrite(VerdPed, HIGH);

  delay(5000);

  digitalWrite(VerdPed, LOW);

  for (int i = 0; i < 5; i++) {

    digitalWrite(VermPed, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(200);

    digitalWrite(VermPed, LOW);
    digitalWrite(Buzzer, LOW);
    delay(200);
  }

}