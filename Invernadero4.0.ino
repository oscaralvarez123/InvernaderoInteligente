#include <ESP32Servo.h>
#include <DHT.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define DHT_PIN 4
#define HUMEDAD_TIERRA_PIN 34
#define VENTILADOR_PIN 26
#define BOMBA_RIEGO_PIN 25
#define LED_PIN 22
#define FOTORESISTENCIA_PIN 35
#define servo_pin 19
#define LED_BUILTIN  
#define LedStatus

// Wifi network station credentials
#define WIFI_SSID "TP-LINK_541C"
#define WIFI_PASSWORD "07940804"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5831378817:AAEuyYH7L0NhwDyMkaFWNmug02zTTFkUfMQ"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

const int TEMPERATURA_MAXIMA=35;
const int HUMEDAD_TIERRA_MINIMA=3500;
const int HUMEDAD_AIRE_MINIMA=30;
const int LUMINOSIDAD_MINIMA=3500;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

DHT dht(DHT_PIN, DHT11);
Servo servo;

const int ledPin = LED_BUILTIN
ledStatus = 0;
void handleNewMessages(int numNewMessages)
{
Serial.print("handleNewMessages ");
Serial.println(numNewMessages);
for (int i = 0; i < numNewMessages; i++)
{
String chat_id = bot.messages[i].chat_id;
String text = bot.messages[i].text;
String from_name = bot.messages[i].from_name;
if (from_name == "")
from_name = "Guest";

if (text == "/ledon")
{
digitalWrite(ledPin, LOW); // turn the LED on (HIGH is the voltage level)
ledStatus = 1;
bot.sendMessage(chat_id, "Led is ON", "");
}
if (text == "/ledoff")
{
ledStatus = 0;
digitalWrite(ledPin, HIGH); // turn the LED off (LOW is the voltage level)
bot.sendMessage(chat_id, "Led is OFF", "");
}
if (text == "/status")
{
if (ledStatus)
{
bot.sendMessage(chat_id, "Led is ON", "");
}
else
{
bot.sendMessage(chat_id, "Led is OFF", "");
}
}
if (text == "/start")
{
String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
welcome += "This is Flash Led Bot example.\n\n";
welcome += "/ledon : to switch the Led ON\n";
welcome += "/ledoff : to switch the Led OFF\n";
welcome += "/status : Returns current status of LED\n";
bot.sendMessage(chat_id, welcome, "Markdown");
}
}
}

void setup() {
  Serial.begin(115200);
  pinMode(HUMEDAD_TIERRA_PIN, INPUT);
  pinMode(VENTILADOR_PIN, OUTPUT);
  pinMode(BOMBA_RIEGO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FOTORESISTENCIA_PIN, INPUT);

  dht.begin();
  servo.attach(servo_pin);


Serial.println();
pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
delay(10);
digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)
// attempt to connect to Wifi network:
Serial.print("Connecting to Wifi SSID ");
Serial.print(WIFI_SSID);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.print("\nWiFi connected. IP address: ");
Serial.println(WiFi.localIP());
Serial.print("Retrieving time: ");
configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
time_t now = time(nullptr);
while (now < 24 * 3600)
{
Serial.print(".");
delay(100);
now = time(nullptr);
}
Serial.println(now);
}

void loop() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  int humedad_tierra = analogRead(HUMEDAD_TIERRA_PIN);
  int luminosidad = analogRead(FOTORESISTENCIA_PIN);

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print("°C - Humedad: ");
    Serial.print(humedad);
    Serial.print("% - Humedad de tierra: ");
    Serial.print(humedad_tierra);
    Serial.print("% - Luminosidad: ");
    Serial.println(luminosidad);

    if (temperatura > TEMPERATURA_MAXIMA) {
      // Activar el ventilador y la bomba de riego
      digitalWrite(VENTILADOR_PIN, HIGH);
      digitalWrite(BOMBA_RIEGO_PIN, HIGH);

      // Encender el LED si la temperatura supera los 150 grados centígrados
      if (temperatura > 150) {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      digitalWrite(VENTILADOR_PIN, LOW);
      digitalWrite(BOMBA_RIEGO_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
    }

    if (humedad_tierra > HUMEDAD_TIERRA_MINIMA) {
      // Activar la bomba de riego y el servo
      digitalWrite(BOMBA_RIEGO_PIN, HIGH);
      
      servo.write(135);

    } else {
      digitalWrite(BOMBA_RIEGO_PIN, LOW); 
    }

    if (humedad < HUMEDAD_AIRE_MINIMA) {
      // Activar el ventilador
      digitalWrite(VENTILADOR_PIN, HIGH);
    } else {
      digitalWrite(VENTILADOR_PIN, LOW);
    }

    if (luminosidad > LUMINOSIDAD_MINIMA) {
      // Encender las luces LED
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  
  delay(1000);
  servo.write(45);
  delay(1000);


if (millis() - bot_lasttime > BOT_MTBS)
{
int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
while (numNewMessages)
{
Serial.println("got response");
handleNewMessages(numNewMessages);
numNewMessages = bot.getUpdates(bot.last_message_received + 1);
}
bot_lasttime = millis();
}
}
