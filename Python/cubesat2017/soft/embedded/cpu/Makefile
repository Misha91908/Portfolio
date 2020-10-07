all: build

flash:
	echo "DFU flash..."
	cd micropython/ports/stm32 && make BOARD=CUBESATCPU deploy

clean:
	rm -rf micropython/ports/stm32/boards/CUBESATCPU
	rm -rf micropython/ports/stm32/modules/*.py
	rm -rf micropython/ports/stm32/build-CUBESATCPU

copyfiles:
	mkdir micropython/ports/stm32/boards/CUBESATCPU
	cp -r bsp/*.* micropython/ports/stm32/boards/CUBESATCPU
	cp -r frozen_modules/*.* micropython/ports/stm32/modules 

build: clean copyfiles
	cd micropython && git submodule update --init
	cd micropython/ports/stm32 && make BOARD=CUBESATCPU -j4

upload:
	@echo "Uploading files to CPU module ..."
	@echo "--------- !!! Сделать загрузку файлов на плату через ампи тут !!! ---------------"
