/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 *
 * Compile with: arduino-cli compile --fqbn arduino:avr:<board>
 * Run with: arduino-cli ...
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP3008.h"

Adafruit_MCP3008 adc;

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

uint16_t values[64][64];

Adafruit_MCP23017 mcp0;
Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;

Adafruit_MCP23017 mcps[4] = {mcp0, mcp1, mcp2, mcp3};

void setPin(uint8_t outputPin) {
	PORTD = controlPins[outputPin];
}

void readData() {
	// Cycle through MCP pins
	for (int i = 0; i < 64; i++) {
		// TODO: cycle through pins on the different MCP chips
		mcp0.digitalWrite(i, HIGH);

		// Cycle through MUX pins
		for (int j = 0; j < 16; j++) {
			setPin(i);
			values[i][j] = adc.readADC(0);
			values[i][j+16] = adc.readADC(1);
			values[i][j+32] = adc.readADC(2);
			values[i][j+48] = adc.readADC(3);
			delayMicroseconds(10);
		}

		mcp0.digitalWrite(i, LOW);
	}
}

void setup() {
	mcps[0].begin(0x20);
	mcps[1].begin(0x21);
	mcps[2].begin(0x22);
	mcps[3].begin(0x23);

	Serial.begin(2000000);

	adc.begin();

	DDRD = DDRD | B00111100;

	mcp0.begin();

	for (int i = 0; i < 16; i++) {
		mcp0.pinMode(i, OUTPUT);
	}
}

void loop() {
	Serial.println("----------------------------------------------------------------------------------------------------");
	readData();

	for (int i = 0; i < 64; i++) {
		Serial.print("{");

		for (int j = 0; j < 64; j++) {
			Serial.print(values[i][j], DEC);
			Serial.print(", ");
		}

		Serial.println("}");
	}

	delay(3000);
}
