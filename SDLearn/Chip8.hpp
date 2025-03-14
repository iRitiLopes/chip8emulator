#ifndef CHIP8_H

#define CHIP8_H

#include "Platform.h"
#include <cstdint>
#include <random>
#include <iostream>

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define VIDEO_SCALE 10

class Chip8 {
	uint8_t registers[16]{};
	uint8_t memory[4096]{};
	uint16_t index{};
	uint16_t pc{};
	uint16_t stack[16]{};
	uint8_t sp{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint8_t keypad[16]{};
	uint32_t video[64 * 32]{};
	uint16_t opcode;
	bool debug = false;

	Platform platform{ "chip8", VIDEO_WIDTH * VIDEO_SCALE, VIDEO_HEIGHT * VIDEO_SCALE, VIDEO_WIDTH, VIDEO_HEIGHT };

public:
	Chip8();
	void load(const char* path);
	void cycle();
	void processInput();
	void print();

private:
	void handle8();
	void handleE();
	void handleF();
	void initialize();
	void drawSprite();

	// instructions

	std::default_random_engine randGen;
	std::uniform_int_distribution<unsigned short> randByte;
	

	void OP_00e0() {
		memset(video, 0, sizeof(video));
	}

	void OP_00ee() {
		--sp;
		pc = stack[sp];
	}

	void OP_1nnn() {
		auto address = opcode & 0x0fffU;
		pc = address;
	}

	void OP_2nnn() {
		uint16_t address = opcode & 0x0FFFu;

		stack[sp] = pc;
		++sp;
		pc = address;
	}

	void OP_3xnn() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto vx = registers[x];
		auto nn = (opcode & 0x00FF);

		if (vx == nn) {
			pc += 2;
		}
	}

	void OP_4xnn() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto vx = registers[x];
		auto nn = (opcode & 0x00FF);

		if (vx != nn) {
			pc += 2;
		}
	}

	void OP_5xy0() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto y = (opcode & 0x00F0) >> 4u;

		auto vx = registers[x];
		auto vy = registers[y];

		if (vx == vy) {
			pc += 2;
		}
	}

	void OP_6xnn() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto nn = opcode & 0x00FF;

		registers[x] = nn;
	}

	void OP_7xnn() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto nn = opcode & 0x00FF;

		registers[x] += nn;
	}

	void OP_8xy0() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto y = (opcode & 0x00F0) >> 4u;

		auto vy = registers[y];
		registers[x] = vy;
	}

	void OP_8xy1() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto y = (opcode & 0x00F0) >> 4u;

		auto vy = registers[y];
		registers[x] |= vy;
	}

	void OP_8xy2() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] &= registers[Vy];
	}

	void OP_8xy3() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		registers[Vx] ^= registers[Vy];
	}

	void OP_8xy4() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		uint16_t sum = registers[Vx] + registers[Vy];

		if (sum > 255U)
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] = sum & 0xFFu;
	}

	void OP_8xy5() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vx] > registers[Vy])
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] -= registers[Vy];
	}

	void OP_8xy6() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		// Save LSB in VF
		registers[0xF] = (registers[Vx] & 0x1u);

		registers[Vx] >>= 1;
	}

	void OP_8xy7() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;

		if (registers[Vy] > registers[Vx])
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[Vx] = registers[Vy] - registers[Vx];
	}


	void OP_8xyE() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		// Save MSB in VF
		registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

		registers[Vx] <<= 1;
	}

	void OP_9xy0() {
		auto x = (opcode & 0x0F00) >> 8u;
		auto y = (opcode & 0x00F0) >> 4u;

		auto vx = registers[x];
		auto vy = registers[y];

		if (vx != vy) {
			pc += 2;
		}
	}

	void OP_Annn() {
		auto address = opcode & 0x0fff;
		index = address;
	};

	void OP_Bnnn() {
		auto address = opcode & 0x0fff;

		pc = registers[0] + address;
	}

	void OP_Cxnn() {
		auto vx = (opcode & 0x0F00u) >> 8u;
		uint8_t BYTE = opcode & 0x00FFu;

		registers[vx] = randByte(randGen) & BYTE;
	}

	void OP_Dxyn() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;
		uint8_t height = opcode & 0x000Fu;

		// Wrap if going beyond screen boundaries
		uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
		uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

		registers[0xF] = 0;

		for (unsigned int row = 0; row < height; ++row)
		{
			uint8_t spriteByte = memory[index + row];

			for (unsigned int col = 0; col < 8; ++col)
			{
				uint8_t spritePixel = spriteByte & (0x80u >> col);
				uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

				// Sprite pixel is on
				if (spritePixel)
				{
					// Screen pixel also on - collision
					if (*screenPixel == 0xFFFFFFFF)
					{
						registers[0xF] = 1;
					}

					// Effectively XOR with the sprite pixel
					*screenPixel ^= 0xFFFFFFFF;
				}
			}
		}
	}

	void OP_Ex9E() {
		auto vx = (opcode & 0x0F00) >> 8u;
		auto key = registers[vx];

		if (keypad[key]) {
			pc += 2;
		}
	}

	void OP_ExA1() {
		auto vx = (opcode & 0x0F00) >> 8u;
		auto key = registers[vx];

		if (!keypad[key]) {
			pc += 2;
		}
	}

	void OP_Fx07() {
		auto vx = (opcode & 0x0f00) >> 8u;
		registers[vx] = delayTimer;
	}

	void OP_Fx0A() {
		uint8_t vx = (opcode & 0x0F00u) >> 8u;

		bool keyPressed = false;
		for (int i = 0; i < 16; ++i) {
			if (keypad[i]) {
				std::cout << "Pressed key: " << i << std::endl;
				registers[vx] = i;
				keyPressed = true;
				return;
			}
		}

		if (!keyPressed) {
			pc -= 2;
		}
	}

	void OP_Fx15() {
		auto vx = (opcode & 0x0F00) >> 8u;
		delayTimer = registers[vx];
	}

	void OP_Fx18() {
		auto vx = (opcode & 0x0F00) >> 8u;
		soundTimer = registers[vx];
	}

	void OP_Fx1E() {
		auto vx = (opcode & 0x0F00) >> 8u;
		index += registers[vx];
	}

	void OP_Fx29() {
		auto vx = (opcode & 0x0F00u) >> 8u;
		auto digit = registers[vx];

		index = 0x50 + (5 * digit);
	}

	void OP_Fx33() {
		auto vx = (opcode & 0x0F00) >> 8u;
		auto value = registers[vx];

		// first
		memory[index + 2] = value % 10;
		value /= 10;

		memory[index + 1] = value % 10;
		value /= 10;
		
		memory[index] = value % 10;
	}

	void OP_Fx55() {
		auto vx = (opcode & 0x0f00) >> 8u;

		for (auto i = 0; i <= vx; i++) {
			memory[index + i] = registers[i];
		}
	}

	void OP_Fx65() {
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;

		for (uint8_t i = 0; i <= Vx; ++i) {
			registers[i] = memory[index + i];
		}
	}
};
	

#endif