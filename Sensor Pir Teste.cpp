//https://wokwi.com/projects/474264382064123905

#define LED_DEBUG 13
#define RELE      8
#define PIR       9

unsigned long int tempo_atual;   
unsigned long int tempo_500ms;

void setup()
{
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(PIR, INPUT);

  tempo_500ms = millis();
}

void loop()
{
  tempo_atual = millis();

  if((tempo_atual - tempo_500ms) >= 500) 
  {
    tempo_500ms = millis();

    digitalWrite(LED_DEBUG, !digitalRead(LED_DEBUG)); 
  }

  digitalWrite(RELE, digitalRead(PIR));
}