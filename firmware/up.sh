make clean
rm -rf bin obj
make all
fuser -k /dev/ttyACM1
avrdude -c arduino -P /dev/ttyACM1 -p m328p -D -U flash:w:bin/firmware.elf
figlet "MIC17"
