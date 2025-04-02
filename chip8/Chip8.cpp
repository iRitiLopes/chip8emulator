#include "Chip8.hpp"
#include <iostream>
#include <fstream>
#include <xstring>
#include <chrono>
#include <random>

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_PROGRAM_ADDRESS = 0x200;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(): randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
	initialize();
}

void Chip8::print() {
	for (unsigned int i = 0; i < sizeof(registers); i++) {
		std::cout << "V" << i << ": " << (int)registers[i] << std::endl;
	}

	std::cout << "opcode: " << std::hex << opcode << std::endl;
	std::cout << "pc: " << std::hex << pc << std::endl;
}

void Chip8::initialize() {
	pc = START_PROGRAM_ADDRESS;
	opcode = 0;
	index = 0;
	sp = 0;

	for (int i = 0; i < 16; i++) {
		stack[i] = 0;
		registers[i] = 0;
	}

	memset(memory, 0, sizeof(memory));
	memset(video, 0, sizeof(video));
	memset(keypad, 0, sizeof(keypad));

	for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	std::cout << "initializing random" << std::endl;
	randByte = std::uniform_int_distribution<unsigned short>(0, 255);
}

void Chip8::load(const char* path) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		auto size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; i++) {
			memory[START_PROGRAM_ADDRESS + i] = buffer[i];
		}


		delete[] buffer;
	}
}

void Chip8::processInput() {
	platform.processInput(keypad, &close);
	if (close) {
		exit(0);
	}
}

int counter = 0;
void Chip8::cycle() {
	// Instruction is a 16bit 
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	pc += 2;

	if (debug) {
		printf("opcode: 0x%X \n", opcode);
	}


	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode & 0xFFFF) {
			// 0x0???
		case 0x00E0:
			OP_00e0();
			break;
		case 0x00EE:
			OP_00ee();
			break;
		}
		break;
	case 0x1000: // 0x1NNN
		OP_1nnn();
		break;
	case 0x2000:
		OP_2nnn();
		break;
	case 0x3000:
		OP_3xnn();
		break;
	case 0x4000:
		OP_4xnn();
		break;
	case 0x5000:
		OP_5xy0();
		break;
	case 0x6000:
		OP_6xnn();
		break;
	case 0x7000:
		OP_7xnn();
		break;
	case 0x8000:
		handle8();
		break;
	case 0x9000:
		OP_9xy0();
		break;
	case 0xA000:
		OP_Annn();
		break;
	case 0xB000:
		OP_Bnnn();
		break;
	case 0xC000:
		OP_Cxnn();
		break;
	case 0xD000:
		OP_Dxyn();
		break;
	case 0xE000:
		handleE();
		break;
	case 0xF000:
		handleF();
		break;
	default:
		printf("Unknown opcode: 0x%X \n", opcode);
		exit(1);
		break;
	}

	if (delayTimer > 0) {
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0) {
		--soundTimer;
	}

	platform.update(video, sizeof(video[0]) * VIDEO_WIDTH);
}

void Chip8::handle8() {
	switch (opcode & 0xF00F) {
	case 0x8000:
		OP_8xy0();
		break;
	case 0x8001:
		OP_8xy1();
		break;
	case 0x8002:
		OP_8xy2();
		break;
	case 0x8003:
		OP_8xy3();
		break;
	case 0x8004:
		OP_8xy4();
		break;
	case 0x8005:
		OP_8xy5();
		break;
	case 0x8006:
		OP_8xy6();
		break;
	case 0x8007:
		OP_8xy7();
		break;
	case 0x800E:
		OP_8xyE();
		break;
	default:
		printf("Unknown opcode: 0x%X \n", opcode);
		exit(1);
		break;
	}
}

void Chip8::handleE() {
	switch (opcode & 0xF0FF) {
	case 0xE09E:
		OP_Ex9E();
		break;
	case 0xE0A1:
		OP_ExA1();
		break;
	}
}

void Chip8::handleF() {
	switch (opcode & 0xF0FF) {
	case 0xF007:
		OP_Fx07();
		break;
	case 0xF00A:
		OP_Fx0A();
		break;
	case 0XF015:
		OP_Fx15();
		break;
	case 0xF018:
		OP_Fx18();
		break;
	case 0xF01E:
		OP_Fx1E();
		break;
	case 0xF029:
		OP_Fx29();
		break;
	case 0xF055:
		OP_Fx55();
		break;
	case 0xF033:
		OP_Fx33();
		break;
	case 0xF065:
		OP_Fx65();
		break;
	default:
		printf("Unknown opcode: 0x%X \n", opcode);
		exit(1);
		break;
	}
}

