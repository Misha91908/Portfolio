all:
	platformio run
	sudo avrdude -c jtag3isp -P usb -B 500kHz -p m2560 -e -U flash:w:.pioenvs/megaatmega2560/firmware.hex:i

provision:
	sudo add-apt-repository -y ppa:ubuntuhandbook1/apps
	sudo apt-get update
	sudo apt-get install -y avrdude avrdude-doc
	echo "Patch avrdude conf file... "
	sudo cp avrdude.conf /etc/avrdude.conf
