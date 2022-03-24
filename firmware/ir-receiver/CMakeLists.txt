add_avr_executable(ir-tiny attiny9
	main.cpp
	board.h)

target_link_libraries(ir-tiny PRIVATE tiny-avrlib)
