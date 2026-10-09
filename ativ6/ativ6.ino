#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ctype.h>
#include <string.h>

#define COLUNAS 20
#define LINHAS 4

#define SDA_LCD 21
#define SCL_LCD 22

#define TAMANHO_LINHA 20

LiquidCrystal_I2C lcd(0x27, COLUNAS, LINHAS);

char textoOriginal[TAMANHO_LINHA + 1];
char textoMaiusculo[TAMANHO_LINHA + 1];

int quantidade = 0;

bool novaMensagem = true;

void limparTextos() {
  memset(textoOriginal, ' ', TAMANHO_LINHA);
  memset(textoMaiusculo, ' ', TAMANHO_LINHA);

  textoOriginal[TAMANHO_LINHA] = '\0';
  textoMaiusculo[TAMANHO_LINHA] = '\0';

  quantidade = 0;
}
void atualizarLCD() {
  lcd.setCursor(0, 0);
  lcd.print(textoOriginal);

  lcd.setCursor(0, 1);
  lcd.print(textoMaiusculo);
}

void processarCaractere(char caractere) {
  if (caractere == '\r' || caractere == '\n') {
    novaMensagem = true;
    return;
  }

  if (novaMensagem) {
    limparTextos();
    lcd.clear();
    novaMensagem = false;
  }

  if (caractere == '\b' || caractere == 127) {
    if (quantidade > 0) {
      quantidade--;
      textoOriginal[quantidade] = ' ';
      textoMaiusculo[quantidade] = ' ';
      atualizarLCD();
    }
    return;
  }

  if (quantidade >= TAMANHO_LINHA) {
    memmove(textoOriginal, textoOriginal + 1, TAMANHO_LINHA - 1);
    memmove(textoMaiusculo, textoMaiusculo + 1, TAMANHO_LINHA - 1);

    quantidade = TAMANHO_LINHA - 1;
    textoOriginal[quantidade] = ' ';
    textoMaiusculo[quantidade] = ' ';
  }

  textoOriginal[quantidade] = caractere;
  textoMaiusculo[quantidade] =
      (char)toupper((unsigned char)caractere);

  quantidade++;

  textoOriginal[TAMANHO_LINHA] = '\0';
  textoMaiusculo[TAMANHO_LINHA] = '\0';

  atualizarLCD();
}


void setup() {
  Serial.begin(9600);

  Wire.begin(SDA_LCD, SCL_LCD);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  limparTextos();

  atualizarLCD();

  Serial.println("Sistema LCD 20x4 iniciado.");
  Serial.println("Digite um texto no Monitor Serial.");
}

void loop() {
  while (Serial.available() > 0) {

    char caractere = (char)Serial.read();

    processarCaractere(caractere);
  }
}