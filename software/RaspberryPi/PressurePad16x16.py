# Imports
import time

import board
from board import *
import busio
import digitalio

import adafruit_mcp230xx
import adafruit_mcp3xxx.mcp3008 as ADC
from adafruit_mcp3xxx.analog_in import AnalogIn

# Size of grid
GRID_SIZE = 16

# Array to hold grid values
values = [([0] * GRID_SIZE)] * GRID_SIZE

# Array to hold MUX controller pins
mux_pins = [None] * 4

# Digital pin numbers used to control analog multiplexer
mux_pin_numbers = [D4, D17, D27, D22]

# I2C port expander (MCP23017)
port_expander = None

# ADC (MCP3008)
adc = None

# Read grid data and store it in values
def readData():
    for i in range(16):
        port_expander.get_pin(i).value = True

        # TODO: cycle MUX pins and read analog inputs

        port_expander.get_pin(i).value = False

# Setup all I/O
def setup():
    # Setup I2C and SPI
    i2c = busio.I2C(board.SCL, board.SDA)
    spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
    cs = digitalio.DigitalInOut(board.D5)

    # Setup I2C port expander (MCP23017) and ADC (MCP3008)
    port_expander = adafruit_mcp230xx.MCP23017(i2c)
    adc = ADC.MCP3008(spi, cs)

    # Initialize MUX controller pins
    for i in range(4):
        new_pin = digitalio.DigitalInOut(mux_pin_numbers[i]) 
        new_pin.direction = digitalio.Direction.OUTPUT
        mux_pins[i] = new_pin

    # Set all pins on I2C port expander to outputs
    for i in range(16):
        port_expander.get_pin(i).switch_to_output()

# Main loop
def main():
    while True:
        # TODO: call readData() and print data
        pass

if __name__ == "__main__":
    setup()
    main()
