#include <Servo.h> 

Servo servo; 
  
const int potPin = A0; 
const int servoPin = 9; 
  
const int trigPin = 6; 
const int echoPin = 7; 
  
const int ledVerde = 2; 
const int ledAmarelo = 3; 
const int ledVermelho = 4; 
  
const int buzzerPin = 5; 
  
const int distanciaCritica = 20; 

void setup() { 
  Serial.begin(9600); 
  
  servo.attach(servoPin); 
  servo.write(90); 
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  pinMode(ledVerde, OUTPUT); 
  pinMode(ledAmarelo, OUTPUT); 
  pinMode(ledVermelho, OUTPUT); 
  
  pinMode(buzzerPin, OUTPUT); 
  
  digitalWrite(ledVerde, LOW); 
  digitalWrite(ledAmarelo, LOW); 
  digitalWrite(ledVermelho, LOW); 
  noTone(buzzerPin); 
} 

void loop() { 
  int valorPot = analogRead(potPin); 
  int angulo = map(valorPot, 0, 1023, 0, 180); 
  servo.write(angulo); 

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 

  long duracao = pulseIn(echoPin, HIGH); 
  int distancia = duracao * 0.034 / 2; 

  if (distancia > 50 || distancia == 0) { 
    digitalWrite(ledVerde, HIGH); 
    digitalWrite(ledAmarelo, LOW); 
    digitalWrite(ledVermelho, LOW); 
    noTone(buzzerPin); 
  } 
  else if (distancia > distanciaCritica) { 
    digitalWrite(ledVerde, LOW); 
    digitalWrite(ledAmarelo, HIGH); 
    digitalWrite(ledVermelho, LOW); 
    noTone(buzzerPin); 
  } 
  else { 
    digitalWrite(ledVerde, LOW); 
    digitalWrite(ledAmarelo, LOW); 
    digitalWrite(ledVermelho, HIGH); 
    
    tone(buzzerPin, 1000); 
    delay(100);
    noTone(buzzerPin);
  } 

  Serial.print("Potenciometro: "); 
  Serial.print(valorPot); 
  Serial.print(" | Servo: "); 
  Serial.print(angulo); 
  Serial.print(" graus | Distancia: "); 
  Serial.print(distancia); 
  Serial.println(" cm"); 

  delay(100); 
}