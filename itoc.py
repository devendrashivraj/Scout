import smbus
import time
device_bus = 1
ard_addr = 0x2a
sen_addr = 0x70

data = []

bus = smbus.SMBus(device_bus)

def send_command(com):
    bus.write_byte(ard_addr,com)
    

