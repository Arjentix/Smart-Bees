/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 04.11.2019
 */

/**
 * Firmware for Smart Lamp based on Arduino Nano
 */

#define OUT_PIN 2 // D2

void setup() {
  Serial.begin(9600);
  pinMode(OUT_PIN, OUTPUT);
  Serial.println("Started");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readString();
    Serial.print("Readed: ");
    Serial.println(command);
    if (command == "ON") {
      digitalWrite(OUT_PIN, HIGH);
    }
    else if (command == "OFF") {
      digitalWrite(OUT_PIN, LOW);
    }
  }
}