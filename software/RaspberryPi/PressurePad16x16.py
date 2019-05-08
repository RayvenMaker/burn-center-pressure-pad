import time

import board
from board import *
import busio
import digitalio

import adafruit_mcp230xx
import adafruit_mcp3xxx.mcp3008 as ADC
from adafruit_mcp3xxx.analog_in import AnalogIn

GRID_SIZE = 16

mux_pin_numbers = [D4, D17, D27, D22]

#i2c = busio.I2C(board.SCL, board.SDA)
spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
cs = digitalio.DigitalInOut(board.D5)

#port_expander = adafruit_mcp230xx.MCP23017(i2c)
adc = ADC.MCP3008(spi, cs)

mux_pins = [None] * 4

for i in range(4):
    new_pin = digitalio.DigitalInOut(mux_pin_numbers[i]) 
    new_pin.direction = digitalio.Direction.OUTPUT
    mux_pins[i] = new_pin

values = [([0] * GRID_SIZE)] * GRID_SIZE
print(values)
