mkdir -p build
#gcc -Wall -o build/blink blink.cpp -lwiringPi
#gcc -Wall -o build/pipwm pipwm.cpp -lpigpio
#gcc -Wall -o build/wppwm wppwm.cpp -lwiringPi
gcc -Wall -o build/spwm spwm.cpp -lwiringPi

