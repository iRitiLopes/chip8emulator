#include <iostream>
#include <chrono>
#include "Chip8.hpp"

int main() {
	Chip8 chip8;

	chip8.load("./tetris.ch8");

	auto lastCycleTime = std::chrono::high_resolution_clock::now();

	for (;;) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		chip8.processInput();
		if (dt > 1) {
			lastCycleTime = currentTime;
			chip8.cycle();
		}
	}

	chip8.print();
	return 0;
}