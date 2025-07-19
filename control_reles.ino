#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Configuración Wi-Fi
const char* ssid = "";
const char* password = "";

// Configuración Telegram
#define BOT_TOKEN ""
#define CHAT_ID ""
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

unsigned long tiempoConsulta = 0;

// Estados de los relés
bool estadoRele[4] = {false, false, false, false};

// Envía el comando serial al módulo relé (según protocolo)
void enviarComandoRele(int rele, bool encender) {
  uint8_t comando[4];
  comando[0] = 0xA0;                    // cabecera
  comando[1] = rele;                    // 1–4
  comando[2] = encender ? 0x01 : 0x00;  // encender/apagar
  comando[3] = comando[0] + comando[1] + comando[2];  // checksum

  Serial.write(comando, 4);
}

// Envía mensaje de bienvenida por Telegram
void obtenerHora() {
  configTime(0, 0, "pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    delay(100);
    now = time(nullptr);
  }

  bot.sendMessage(CHAT_ID,
    "🚀 *ESP8266 iniciado*\n"
    "📡 Conectado a Wi-Fi\n\n"
    "🔌 *Control de relés*\n"
    "Para activar o desactivar un relé, escribe:\n\n"
    "/rele1 → activa o desactiva el relé 1\n"
    "/rele2 → activa o desactiva el relé 2\n"
    "/rele3 → activa o desactiva el relé 3\n"
    "/rele4 → activa o desactiva el relé 4\n\n"
    "💡 *Nota:* Todos los relés están *desactivados* al inicio.",
    "Markdown");
}

// Maneja comandos de Telegram
void manejarMensajes(int nuevosMensajes) {
  for (int i = 0; i < nuevosMensajes; i++) {
    String texto = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (chat_id != CHAT_ID) continue;

    for (int j = 0; j < 4; j++) {
      if (texto == "/rele" + String(j + 1)) {
        estadoRele[j] = !estadoRele[j];
        enviarComandoRele(j + 1, estadoRele[j]);
        bot.sendMessage(chat_id, 
          "Relé " + String(j + 1) + 
          (estadoRele[j] ? " encendido ✅" : " apagado ❌"), "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);  // Comunicación UART hacia módulo de relés
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.begin(ssid, password);
  secured_client.setInsecure();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  obtenerHora();
}

void loop() {
  if (millis() - tiempoConsulta > 60000) {
    obtenerHora();
    tiempoConsulta = millis();
  }

  int mensajes = bot.getUpdates(bot.last_message_received + 1);
  if (mensajes > 0) {
    manejarMensajes(mensajes);
  }

  delay(1000);
}