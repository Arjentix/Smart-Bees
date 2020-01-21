/**
 * @author: Polyakov Daniil
 * @mail: arjentix@gmail.com
 * @github: Arjentix
 * @date: 04.11.2019
 */

/**
 * Firmware for Smart Lamp based on Arduino Nano
 */

#define OUT_PIN_BLUE 9		// D9
#define OUT_PIN_RED 10		// D10
#define OUT_PIN_GREEN 11	// D11
#define HAPPY_NEW_YEAR_BLINK_TIMEOUT 1500

bool happy_new_year = false;

void turnoff_all() {
	digitalWrite(OUT_PIN_BLUE,  LOW);
	digitalWrite(OUT_PIN_RED, LOW);
	digitalWrite(OUT_PIN_GREEN, LOW);
	digitalWrite(LED_BUILTIN, LOW);
  happy_new_year = false;
}


void setup() {
	Serial.begin(9600);
	pinMode(OUT_PIN_RED, OUTPUT);
	pinMode(OUT_PIN_GREEN, OUTPUT);
	pinMode(OUT_PIN_BLUE, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	// Setting pins to HIGH cause relay works with LOW signal
	digitalWrite(OUT_PIN_RED, LOW);
	digitalWrite(OUT_PIN_GREEN, LOW);
	digitalWrite(OUT_PIN_BLUE, LOW);

  randomSeed(analogRead(0));

	Serial.println("Started");
}

void loop() {
	if (Serial.available()) {
		String command = Serial.readString();
		Serial.print("Readed: ");
		Serial.println(command);

    if (command == "OFF") {
		  turnoff_all();
    }
		if (command == "BLUE") {
      turnoff_all();
 
			digitalWrite(OUT_PIN_BLUE, HIGH);

			digitalWrite(LED_BUILTIN, HIGH);
		}
		if (command == "RED") {
      turnoff_all();

			digitalWrite(OUT_PIN_RED, HIGH);

			digitalWrite(LED_BUILTIN, HIGH);
		}
		if (command == "GREEN") {
      turnoff_all();

			digitalWrite(OUT_PIN_GREEN, HIGH);

			digitalWrite(LED_BUILTIN, HIGH);
		}
    if (command == "WHITE") {
      turnoff_all();

      digitalWrite(OUT_PIN_RED, HIGH);
      digitalWrite(OUT_PIN_GREEN, HIGH);
      digitalWrite(OUT_PIN_BLUE, HIGH);

      digitalWrite(LED_BUILTIN, HIGH);
    }
		if (command == "PURPLE") {
      turnoff_all();

			digitalWrite(OUT_PIN_RED, HIGH);
			digitalWrite(OUT_PIN_BLUE, HIGH);

			digitalWrite(LED_BUILTIN, HIGH);
		}
		if (command == "PINK") {
      turnoff_all();

			analogWrite(OUT_PIN_RED, 255);
			analogWrite(OUT_PIN_GREEN, 0);
			analogWrite(OUT_PIN_BLUE, 147);

			digitalWrite(LED_BUILTIN, HIGH);
		}
    if (command == "RANDOM") {
      turnoff_all();

      analogWrite(OUT_PIN_RED, random(255));
      analogWrite(OUT_PIN_GREEN, random(255));
      analogWrite(OUT_PIN_BLUE, random(255));

      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (command == "HAPPY NEW YEAR") {
      turnoff_all();

      happy_new_year = true;

      digitalWrite(LED_BUILTIN, HIGH);
    }
	}
  else if (happy_new_year) {
    static unsigned long int loop_count = 0;
    if (loop_count == 0) {
      turnoff_all();
      happy_new_year = true;
      
      digitalWrite(OUT_PIN_RED, HIGH);
      
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Red");
    }
    else if (loop_count == HAPPY_NEW_YEAR_BLINK_TIMEOUT) {
      turnoff_all();
      happy_new_year = true;
      
      digitalWrite(OUT_PIN_GREEN, HIGH);
      
      Serial.println("White");
    }
    else if (loop_count == 2 * HAPPY_NEW_YEAR_BLINK_TIMEOUT) {
      loop_count = -1;
    }
    delay(1);
    ++loop_count;
  }
}
