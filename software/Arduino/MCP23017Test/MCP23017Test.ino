/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

int analogPins[] = {A3, A2, A1, A0};
int mcpPins[] = {8, 9, 10, 11};

void setup() {
	Serial.begin(2000000);

	for (int i = 2; i <= 5; i++) {
		pinMode(i, OUTPUT);
	}

	mcp.begin();

	for (int i = 0; i < 16; i++) {
		mcp.pinMode(i, OUTPUT);
	}
	/*for (int i = 0; i < 4; i++) {
		mcp.digitalWrite(mcpPins[i], HIGH);
	}*/

	for (int i = 2; i <= 5; i++) {
		digitalWrite(i, HIGH);
	}
}

void loop() {
	uint16_t values[] = {0, 0, 0, 0};
	for (int i = 0; i < 4; i++) {
		mcp.digitalWrite(mcpPins[i], HIGH);
		values[i] = analogRead(analogPins[i]);
		mcp.digitalWrite(mcpPins[i], LOW);
		delayMicroseconds(650);
	}

	Serial.println("--------------------------------------------------");
	Serial.print("{ ");
	for (int i = 0; i < 4; i++) {
		Serial.print(values[i]);
		Serial.print(", ");
	}
	Serial.println("}");

	delay(2000);
}
