//Link do wokiw: https://wokwi.com/projects/473012545403901953

#define Verm 10
#define Amar 8

void setup() {
  pinMode(Verm, OUTPUT);
  pinMode(Amar, OUTPUT);
}

void loop() {
  digitalWrite(Verm, HIGH);
  digitalWrite(Amar, LOW);  
  delay(500);

  digitalWrite(Verm, LOW);
  digitalWrite(Amar, HIGH);  
  delay(500);
}
