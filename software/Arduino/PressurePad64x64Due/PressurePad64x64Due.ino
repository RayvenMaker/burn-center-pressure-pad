/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP3008.h"

Adafruit_MCP3008 adc;

// digital out switch pins: 2-5
PROGMEM byte controlPins[][4] = {
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{1, 1, 0, 0},
	{0, 0, 1, 0},
	{1, 0, 1, 0},
	{0, 1, 1, 0},
	{1, 1, 1, 0},
	{0, 0, 0, 1},
	{1, 0, 0, 1},
	{0, 1, 0, 1},
	{1, 1, 0, 1},
	{0, 0, 1, 1},
	{1, 0, 1, 1},
	{0, 1, 1, 1},
	{1, 1, 1, 1},
};

uint16_t values[64][64];

Adafruit_MCP23017 expander0;
Adafruit_MCP23017 expander1;
Adafruit_MCP23017 expander2;
Adafruit_MCP23017 expander3;

Adafruit_MCP23017 expanders[4] = {expander0, expander1, expander2, expander3};

void setPin(uint8_t outputPin) {
	digitalWrite(2, controlPins[outputPin][0]);
	digitalWrite(3, controlPins[outputPin][1]);
	digitalWrite(4, controlPins[outputPin][2]);
	digitalWrite(5, controlPins[outputPin][3]);
}

void readData() {
	// Cycle through MCP pins
	for (int i = 0; i < 64; i++) {
		// Cycle through each pin on the 4 MCP23017 I2C port expanders
		expanders[i / 16].digitalWrite((i+16) % 16, HIGH);

		// Cycle through MUX pins
		for (int j = 0; j < 16; j++) {
			setPin(i);
			
			values[i][j] = adc.readADC(0);
			values[i][j+16] = adc.readADC(1);
			values[i][j+32] = adc.readADC(2);
			values[i][j+48] = adc.readADC(3);

			delayMicroseconds(10);
		}

		expanders[i / 16].digitalWrite((i+16) % 16, LOW);
	}
}

void setup() {
	expanders[0].begin(0x20);
	expanders[1].begin(0x21);
	expanders[2].begin(0x22);
	expanders[3].begin(0x23);

	Serial.begin(2000000);

	adc.begin();

	for (int i = 2; i <= 5; i++) {
		pinMode(i, OUTPUT);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			expanders[i].pinMode(j, OUTPUT);
		}
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
