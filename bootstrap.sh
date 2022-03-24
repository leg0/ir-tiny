cmake -S . -B _build -DCMAKE_CXX_COMPILER=avr-gcc -DCMAKE_C_COMPILER=avr-gcc -DCMAKE_TOOLCHAIN_FILE=cmake/avr-gcc.toolchain.cmake -DCMAKE_VERBOSE_MAKEFILE=false
