#include <Servo.h>
#include <DHT.h>

#define DHT_PIN 4
#define HUMEDAD_TIERRA_PIN A0
#define HUMEDAD_AIRE_PIN A2
#define VENTILADOR_PIN 13
#define BOMBA_RIEGO_PIN 12
#define LED_PIN 7
#define FOTORESISTENCIA_PIN A1

#define TEMPERATURA_MAXIMA 35
#define HUMEDAD_TIERRA_MINIMA 500
#define HUMEDAD_AIRE_MINIMA 30
#define LUMINOSIDAD_MINIMA 300

DHT dht(DHT_PIN, DHT11);
Servo servo;

void setup() {
  pinMode(HUMEDAD_TIERRA_PIN, INPUT);
  pinMode(HUMEDAD_AIRE_PIN, INPUT);
  pinMode(VENTILADOR_PIN, OUTPUT);
  pinMode(BOMBA_RIEGO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FOTORESISTENCIA_PIN, INPUT);

  dht.begin();
  servo.attach(9);
}

void loop() {
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  int humedad_tierra = analogRead(HUMEDAD_TIERRA_PIN);
  int humedad_aire = analogRead(HUMEDAD_AIRE_PIN);
  int luminosidad = analogRead(FOTORESISTENCIA_PIN);

  if (temperatura >= 0 && humedad >= 0) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print("°C - Humedad: ");
    Serial.print(humedad);
    Serial.print("% - Humedad de tierra: ");
    Serial.print(humedad_tierra);
    Serial.print(" - Humedad en el aire: ");
    Serial.print(humedad_aire);
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

    if (humedad_tierra < HUMEDAD_TIERRA_MINIMA) {
      // Activar la bomba de riego y el servo
      digitalWrite(BOMBA_RIEGO_PIN, HIGH);
      servo.write(180);
    } else {
      digitalWrite(BOMBA_RIEGO_PIN, LOW);
      servo.write(0);
    }

    if (humedad_aire < HUMEDAD_AIRE_MINIMA) {
      // Activar el ventilador
      digitalWrite(VENTILADOR_PIN, HIGH);
    } else {
      digitalWrite(VENTILADOR_PIN, LOW);
    }

    if (luminosidad < LUMINOSIDAD_MINIMA) {
      // Encender las luces LED
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
  delay(2000
