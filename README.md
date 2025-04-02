## Chip 8 Emulator Implementation in C++

This project is a Chip 8 emulator written in C++. The Chip 8 is a simple, interpreted programming language used on early computers and gaming systems. This emulator replicates the behavior of the Chip 8 virtual machine, allowing you to run Chip 8 programs and games.

### Features

- **Instruction Set Implementation**: Full support for the Chip 8 instruction set, including opcodes for graphics, input, and timers.
- **Graphics Rendering**: Emulates the 64x32 monochrome display using modern rendering techniques.
- **Input Handling**: Maps the Chip 8's hexadecimal keypad to modern keyboard inputs.
- **Timers**: Implements the delay and sound timers as specified in the Chip 8 specification.
- **Debugging Tools**: Includes optional logging and debugging features to trace program execution.

### Implementation Details

- **CPU**: The emulator's CPU processes Chip 8 instructions in a cycle-based manner, fetching, decoding, and executing opcodes.
- **Memory**: Emulates the Chip 8's 4KB memory, including reserved areas for the interpreter, font set, and program data.
- **Graphics**: Uses a simple framebuffer to emulate the Chip 8's pixel-based display.
- **Sound**: Implements a basic sound system to emulate the Chip 8's buzzer.
- **Input**: Maps the Chip 8's 16-key keypad to modern keyboard keys for user interaction.

### Build

To build the project, ensure you have a C++ compiler and CMake installed. Then, follow these steps:

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd SDLearn
   ```
2. Create a build directory and navigate to it:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   cmake --build .
   ```

### Usage

1. Run the emulator executable:
   ```bash
   ./chip8_emulator <path-to-chip8-rom>
   ```
2. Use the mapped keys to interact with the program. Refer to the documentation for key mappings.

### Allowed Keyboard

The Chip 8 uses a 16-key hexadecimal keypad, which is mapped to modern keyboard keys as follows:

| Chip 8 Key | Keyboard Key |
|------------|--------------|
| 1          | 1            |
| 2          | 2            |
| 3          | 3            |
| C          | 4            |
| 4          | Q            |
| 5          | W            |
| 6          | E            |
| D          | R            |
| 7          | A            |
| 8          | S            |
| 9          | D            |
| E          | F            |
| A          | Z            |
| 0          | X            |
| B          | C            |
| F          | V            |


Special character ESC close the emulator

### Future Improvements

- Add support for Super Chip 8 instructions.
- Improve sound emulation for better accuracy.
- Add a graphical user interface for easier ROM selection and configuration.

### License

This project is licensed under the MIT License. See the `LICENSE` file for details.

### Acknowledgments

- The Chip 8 instruction set and architecture are well-documented thanks to the efforts of the retrocomputing community.
- Special thanks to online resources and tutorials that provided guidance on emulator development.




