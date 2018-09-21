
all: main.elf

main.elf: main.c
	avr-gcc -mmcu=attiny10 -DF_CPU=1000000 -g -Os -Wall -Werror main.c -o main.elf
	# avr-gcc -mmcu=attiny10 -DF_CPU=4000000 -g -Os -Wall -Werror main.c -S
	avr-objdump -ldrwC -Mintel -S main.elf > main.s
	avr-size --mcu=attiny10 -C main.elf

flash: main.elf
	avrdude -p attiny10 -c dasaftdi -P /dev/ttyUSB0 -U flash:w:main.elf -v
	@# avrdude -p t10 -c dasaftdi -P /dev/ttyUSB0 -U flash:w:main.elf -b 128 -vvvv
	@# avrdude -c usbasp-clone -p t10 -U flash:w:main.elf -v

clean:
	rm -f main.elf

# FTDI
#
# TXD (Orange) --///--+
#                       |   +----------------------+
# CTS (Brown)  ---------+---| TPI DATA (1)   RESET | ------+
# GND (Black)  -------------| GND              VCC | ---+  |
# RTS (Green)  -------------| TPI CLK           NC |    |  |
#                           +----------------------+    |  |
#                                                       |  |
# VCC (Red)    -----------------------------------------+  |
#                                                          |
# GND (Black)  --------------------------------------------+
#
#
# USB ASP
#                           +----------------------+
# MOSI         -------------| TPI DATA (1)   RESET | ------+
# GND          -------------| GND              VCC | ---+  |
# SCK          -------------| TPI CLK           NC |    |  |
#                           +----------------------+    |  |
#                                                       |  |
# VCC (Red)    -----------------------------------------+  |
#                                                          |
# RST          --------------------------------------------+
#
