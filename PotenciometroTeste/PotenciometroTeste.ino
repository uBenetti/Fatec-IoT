//Wokiw: https://wokwi.com/projects/474259929800425473

#define LED 11  
#define POT A0  

unsigned int brilho;

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  brilho = analogRead(POT);
  brilho = map(brilho, 0, 1023, 0, 255);
  Serial.println(brilho);
  analogWrite(LED, brilho);
  delay(100);
}