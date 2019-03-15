#define MUX_IN A0

// digital out switch pins: 2-5
byte controlPins[] = {
	B00000000,
	B00100000,
	B00010000,
	B00110000,
	B00001000,
	B00101000,
	B00011000,
	B00111000,
	B00000100,
	B00100100,
	B00010100,
	B00110100,
	B00001100,
	B00101100,
	B00011100,
	B00111100,
};

uint8_t values[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setPin(uint8_t outputPin) {
	PORTD = controlPins[outputPin];
}

void setup() {
	Serial.begin(2000000);
	DDRD = DDRD | B00111100;
}

void loop() {
	for (int i = 0; i < 16; i++) {
		setPin(i);
		values[i] = analogRead(MUX_IN);
		delayMicroseconds(10000);
	}

	Serial.print("{");
	for (int i = 0; i < 16; i++) {
		Serial.print(values[i], DEC);
		Serial.print(", ");
	}
	Serial.println("}");

	delay(1000);
}

