#include <ESP32Servo.h>
#include <DHT.h>

#define DHT_PIN 4
#define HUMEDAD_TIERRA_PIN 34
#define VENTILADOR_PIN 26
#define BOMBA_RIEGO_PIN 25
#define LED_PIN 22
#define FOTORESISTENCIA_PIN 35
#define servo_pin 19

const int TEMPERATURA_MAXIMA=35;
const int HUMEDAD_TIERRA_MINIMA=3500;
const int HUMEDAD_AIRE_MINIMA=30;
const int LUMINOSIDAD_MINIMA=3500;

DHT dht(DHT_PIN, DHT11);
Servo servo;

void setup() {
  Serial.begin(115200);
  pinMode(HUMEDAD_TIERRA_PIN, INPUT);
  pinMode(VENTILADOR_PIN, OUTPUT);
  pinMode(BOMBA_RIEGO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(FOTORESISTENCIA_PIN, INPUT);

  dht.begin();
  servo.attach(servo_pin);
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
}
