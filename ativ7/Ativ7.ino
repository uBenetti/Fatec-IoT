#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESPping.h>

const char* redeWiFi = "Wokwi-GUEST";
const char* senhaWiFi = "";

WiFiUDP conexaoUDP;
NTPClient relogioNTP(conexaoUDP, "pool.ntp.org", -10800, 60000);

LiquidCrystal_I2C display(0x27, 16, 2);

String hostSelecionado = "Aguardando host";
String bufferSerial = "";

bool ntpSincronizado = false;
bool pingExecutando = false;

String ajustarTextoLCD(String mensagem) {
  mensagem = mensagem.substring(0, 16);

  while (mensagem.length() < 16) {
    mensagem += ' ';
  }

  return mensagem;
}

void mostrarInformacoes() {
  display.setCursor(0, 0);
  display.print(ajustarTextoLCD(hostSelecionado));

  display.setCursor(0, 1);

  if (ntpSincronizado) {
    display.print(ajustarTextoLCD(relogioNTP.getFormattedTime()));
  } else {
    display.print(ajustarTextoLCD("Aguardando NTP"));
  }
}

void tarefaDePing(void* dados) {
  char* endereco = (char*)dados;

  Serial.println();
  Serial.print("Testando host: ");
  Serial.println(endereco);

  bool sucesso = Ping.ping(endereco, 4);
  float mediaMs = Ping.averageTime();

  Serial.println("----- Resultado -----");

  if (sucesso) {
    Serial.println("Ping realizado com sucesso!");
    Serial.print("Tempo medio: ");
    Serial.print(mediaMs);
    Serial.println(" ms");
  } else {
    Serial.println("O host nao respondeu.");
    Serial.println("Verifique o endereco ou a rede.");
  }

  Serial.println("---------------------");
  Serial.println("Digite outro host e pressione Enter.");

  delete[] endereco;

  pingExecutando = false;
  vTaskDelete(NULL);
}

void solicitarPing(String enderecoDigitado) {
  enderecoDigitado.trim();

  if (enderecoDigitado.length() == 0) {
    return;
  }

  if (pingExecutando) {
    Serial.println("Aguarde o ping atual terminar.");
    return;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado.");
    return;
  }

  if (enderecoDigitado.length() > 127) {
    Serial.println("Endereco muito longo.");
    return;
  }

  hostSelecionado = enderecoDigitado;
  mostrarInformacoes();

  char* enderecoCopia = new char[128];
  enderecoDigitado.toCharArray(enderecoCopia, 128);

  pingExecutando = true;

  BaseType_t resultadoTarefa = xTaskCreate(
    tarefaDePing,
    "VerificarPing",
    8192,
    enderecoCopia,
    1,
    NULL
  );

  if (resultadoTarefa != pdPASS) {
    delete[] enderecoCopia;
    pingExecutando = false;
    Serial.println("Erro ao criar tarefa de ping.");
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  display.init();
  display.backlight();

  display.setCursor(0, 0);
  display.print("Conectando WiFi");
  display.setCursor(0, 1);
  display.print("Aguarde...");

  Serial.println();
  Serial.println("ESP32 - Ping via Serial");
  Serial.println("Conectando ao Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(redeWiFi, senhaWiFi);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());

  relogioNTP.begin();

  Serial.println("Sincronizando horario NTP...");

  for (int tentativa = 0; tentativa < 10; tentativa++) {
    if (relogioNTP.forceUpdate()) {
      ntpSincronizado = true;
      break;
    }

    delay(500);
  }

  if (ntpSincronizado) {
    Serial.print("Horario atual: ");
    Serial.println(relogioNTP.getFormattedTime());
  } else {
    Serial.println("Nao foi possivel obter o horario NTP.");
  }

  hostSelecionado = "Digite um host";
  mostrarInformacoes();

  Serial.println();
  Serial.println("Digite o endereco para testar.");
  Serial.println("Exemplo: google.com");
  Serial.println("Pressione Enter para executar o ping.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && relogioNTP.update()) {
    ntpSincronizado = true;
  }

  while (Serial.available() > 0) {
    char recebido = Serial.read();

    if (recebido == '\n' || recebido == '\r') {
      if (bufferSerial.length() > 0) {
        String enderecoInformado = bufferSerial;
        bufferSerial = "";

        solicitarPing(enderecoInformado);
      }
    } else if (recebido >= 32 && recebido <= 126) {
      if (bufferSerial.length() < 127) {
        bufferSerial += recebido;
      }
    }
  }

  mostrarInformacoes();
  delay(250);
}