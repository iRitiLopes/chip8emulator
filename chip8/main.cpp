#include <iostream>
#include <chrono>
#include "Chip8.hpp"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <path_to_rom>" << std::endl;
		return 1;
	}

	auto romPath = argv[1];
	std::cout << "Loading ROM: " << romPath << std::endl;

	Chip8 chip8;

	chip8.load(romPath);

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