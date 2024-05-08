//Prototipo 
#if defined(ESP8266)
#pragma message "Codigo para Arduino ESP8266!"
#include <ESP8266WiFi.h>
#include <time.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
const int LED = 2;
const int trigPinj = 13;  // Pi
const int echoPinj = 12;  // Pin de recepción (ECHO) del jabon
const int trigPins = 14;  // Pin de activtt
const int echoPins = 4;  // Pin de rejdjdjdjdjdjdjdjjdjdjdvddddddddddddddddddddddd
  

  
#elif defined(ESP32)
#pragma message "Codigo para Arduino ESP32!"
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
const int LED = 2;

#elif defined(ARDUINO)
#pragma message "Codigo para Arduino basado AVR (UNO, MEGA, etc)!"
#else
#pragma message "No es ni ESP32 o ES8266 o AVR, que es?"
#endif

const char* BOTtoken = "7084956399:AAGucmXr2ngyWCxKiYL-nsq7_XaZAsiYrCs";
WiFiClientSecure client;

#include <UniversalTelegramBot.h>
UniversalTelegramBot bot(BOTtoken, client);
#include <ArduinoJson.h>

// Sleep********************************************************************

struct tm timeinfo;
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // Desplazamiento horario para Montevideo, Uruguay (en segundos)
 int daylightOffset_sec = 0; // No 
 int horaDespertar = 8; // Hora de despertar (8:00 AM)
 int horaDormirSemana = 18; // Hora de dormir durante la semana (6:00 PM)
 int horaDormirFinSemana = 13; // Hora de dormir los sábados y domingos (10:00 PM)
// HORAS PARA AJUSTAR DORMIR

long durations, distanciaS;
long durationj, distanciaJ;
#define BOTtoken "7084956399:AAGucmXr2ngyWCxKiYL-nsq7_XaZAsiYrCs"
//const char* CHAT_IDS[] = {"18276128906", "5742761451", "7108801749", "6429638448","7172541840","6998190922","7156904590", "chat_id_3"}; // Agrega más IDs de chat según sea necesario
const char* CHAT_IDS[] = { "574276145"};
X509List cert(TELEGRAM_CERTIFICATE_ROOT);



int botRequestDelay = 100;
unsigned long lastTimeBotRan;
unsigned long lastNotificationTime = 0;
unsigned long  notificationInterval = 0; // Intervalo de notificación: 1 hora
unsigned long  desactivar=0;
// Handle what happens when you receive new messages


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Laveya", "equind234");
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
  if (wifiMulti.run() == WL_CONNECTED);
  delay(100);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // Imprimir la hora actual
  

    // Configurar pines de los sensores de ultrasonido
  pinMode(trigPinj, OUTPUT);
  pinMode(echoPinj, INPUT);
  pinMode(trigPins, OUTPUT);
  pinMode(echoPins, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);


   #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert);
  #endif

 
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);

  
 
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  
   for (const auto& id : CHAT_IDS) {
      bot.sendMessage(id, "¡Atención! El dispositivo se ha encendido RAM815.Profundidad: " );
    }
  

}
void conexion_wifi() {
  if (wifiMulti.run() == WL_CONNECTED) {
    digitalWrite(LED, LOW);
    delay(1000); 
  } else {
    Serial.println("WiFi not connected! Reconnecting...");
    digitalWrite(LED, HIGH);
    delay(1000); // Espera 1 segundo antes de intentar reconectar
    WiFi.reconnect();
  }
}

void handleNewMessages(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    bool authorized = false;
    // Check if the incoming chat ID is authorized
    for (const auto& id : CHAT_IDS) {
      if (chat_id.equals(id)) {
        authorized = true;
        break;
      }
    }
    if (!authorized) {
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    if (text == "/laveya") {
      String welcome = "¡Bienvenido a Rambla 815!\n";
      welcome += "Utiliza /ram815 para verificar el nivel de productos\n";  
      bot.sendMessage(chat_id, welcome, "");
    }
     String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "¡Bienvenido a Rambla 815!, " + from_name + ".\n";
      welcome += "Utiliza /ram815 para verificar el nivel de productos.\n";  
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/ram815") {
      String welcome = "¡Verificar productos en Rambla 815!\n";
      welcome += "Utiliza /skip para verificar el nivel de jabón.\n";
      welcome += "Utiliza /confort para verificar el nivel de suavizante.\n";
      bot.sendMessage(chat_id, welcome, "");
    }
    
   
    // Allow users to check soap level with "/check_soap" command
    if (text == "/skip") {
      if (distanciaJ <= 28){
        bot.sendMessage(chat_id, "Hay jabón disponible RAM815. Profundidad: " + String(distanciaJ) + " cm");
      }
       else{
        bot.sendMessage(chat_id, "El jabón se ha agotado RAM815. Profundidad: "+ String(distanciaJ) + " cm");
      }
    }

    // Allow users to check softener level with "/check_softener" command
    if (text == "/confort") {
      if (distanciaS <=28){
        bot.sendMessage(chat_id, "Hay suavizante disponible RAM815. Profundidad: " + String(distanciaS) + " cm");
      }
       else{
        bot.sendMessage(chat_id, "El suavizante se ha agotado RAM815. Profundidad: " + String(distanciaS) + " cm");
      }
    }
  }
}

void lectura(){
  
  digitalWrite(trigPinj, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinj, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinj, LOW);
  durationj = pulseIn(echoPinj, HIGH);
  distanciaJ = (durationj * 0.0343) / 2;  // Calcular la distancia en centímetros
  delay(100);

  // Lectura del segundo sensor de ultrasonido
  
  digitalWrite(trigPins, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPins, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins, LOW);
  durations = pulseIn(echoPins, HIGH);
  distanciaS = (durations * 0.0343) / 2;  // Calcular la distancia en centímetros

  // Imprimir las distancias medidas
  Serial.print("Profundidad Suavizante: ");
  Serial.print(distanciaS);
  Serial.println(" cm");
  Serial.print("Profundidad Jabon: ");
  Serial.print(distanciaJ);
  Serial.println(" cm");
}

void printLocalTime() {
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return;
  }

  // Verificar si es fin de semana (sábado o domingo)
  bool esFinSemana = (timeinfo.tm_wday == 0 || timeinfo.tm_wday == 6);

  // Calcular la hora de dormir según el día de la semana
  int horaDormir;
  if (esFinSemana) {
    horaDormir = horaDormirFinSemana;
  } else {
    horaDormir = horaDormirSemana;
  }

  // Calcular el tiempo restante hasta la hora de despertar
  int horaActual = timeinfo.tm_hour -3;
  int minutosActual = timeinfo.tm_min;
  int segundosActual = timeinfo.tm_sec;
  int segundosHastaDespertar = 0;

  if (horaActual < horaDespertar) {
    segundosHastaDespertar = ((horaDespertar - horaActual) * 3600) + ((59 - minutosActual) * 60) + (59 - segundosActual);
  } else {
    segundosHastaDespertar = (((24 - horaActual) + horaDespertar) * 3600) + ((59 - minutosActual) * 60) + (59 - segundosActual);
  }
int segundosHastaDormir;

int segundosHastaEvento = min(segundosHastaDespertar, segundosHastaDormir);
  // Calcular el tiempo restante hasta la hora de dormir
  
  if (horaActual < horaDormir) {
    segundosHastaDormir = ((horaDormir - horaActual) * 3600) + ((59 - minutosActual) * 60) + (59 - segundosActual);
    Serial.println("Despierto!");
  } else {
    segundosHastaDormir = (((24 - horaActual) + horaDormir) * 3600) + ((59 - minutosActual) * 60) + (59 - segundosActual);
    Serial.println("Durmiendo...");
    ESP.deepSleep(segundosHastaEvento * 1000000); // Convertir segundos a microsegundos
  }

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return;
  }

  Serial.print("Hora actual: ");
  Serial.print(horaActual);
  Serial.print(":");
  Serial.print(timeinfo.tm_min);
  Serial.print(":");
  Serial.println(timeinfo.tm_sec);
}

void loop() {
  conexion_wifi(); // Llamada a la función para manejar la conexión Wi-Fi
  lectura();
  printLocalTime();

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
    
  }

  if (distanciaJ >= 28 && millis() - lastNotificationTime > notificationInterval) {
    // Soap has run out, send notification
    for (const auto& id : CHAT_IDS) {
      bot.sendMessage(id, "¡Atención! El jabón se ha agotado RAM815. Profundidad: " + String(distanciaJ) + " cm");
    }
    lastNotificationTime = millis();
    notificationInterval = 60 * 60 * 1000; // Intervalo de notificación: 1 hora
  }

  if (distanciaS >= 28 && millis() - lastNotificationTime > notificationInterval) {
    // Softener has run out, send notification
    for (const auto& id : CHAT_IDS) {
      bot.sendMessage(id, "¡Atención! El suavizante se ha agotado RAM815.Profundidad: " + String(distanciaS) + " cm");
    }
    lastNotificationTime = millis();
    notificationInterval = 60 * 60 * 1000; // Intervalo de notificación: 1 hora
  }

  }

  