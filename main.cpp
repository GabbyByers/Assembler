#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    std::ifstream inputFile("SourceCode/Snake.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    for (const auto& str : lines) {
        std::cout << str << std::endl;
    }

    std::ofstream binaryFile("ROM.bin", std::ios::binary);

    if (!binaryFile) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return 1;
    }

    int numBytesWritten = 0;

    for (std::string line : lines) {

        // Write Pointer Skip
        if (line.substr(0, 3) == "SKP") {
            std::string memoryAddressHighString = "0x" + line.substr(6, 2);
            std::string memoryAddressLowString = "0x" + line.substr(8, 2);

            unsigned char memoryAddressHighValue = static_cast<unsigned char>(std::stoi(memoryAddressHighString, nullptr, 16));
            unsigned char memoryAddressLowValue = static_cast<unsigned char>(std::stoi(memoryAddressLowString, nullptr, 16));

            int pointerValue = memoryAddressLowValue + (256 * memoryAddressHighValue);
            binaryFile.seekp(pointerValue, std::ios::beg);
        }

        // Regular Instruction Encoding
        unsigned char data = 0;
        bool validInstruction = false;
        bool includeMemoryAddress = false;
        bool includeImmediateValue = false;
        if (line.substr(0, 3) == "LIA") { data = 0x00; validInstruction = true; includeImmediateValue = true; }  // Load Immediate A
        if (line.substr(0, 3) == "LIB") { data = 0x01; validInstruction = true; includeImmediateValue = true; }  // Load Immediate B
        if (line.substr(0, 3) == "LDA") { data = 0x02; validInstruction = true; includeMemoryAddress = true; }   // Load A
        if (line.substr(0, 3) == "LDB") { data = 0x03; validInstruction = true; includeMemoryAddress = true; }   // Load B
        if (line.substr(0, 3) == "STA") { data = 0x04; validInstruction = true; includeMemoryAddress = true; }   // Store A
        if (line.substr(0, 3) == "STB") { data = 0x05; validInstruction = true; includeMemoryAddress = true; }   // Store B
        if (line.substr(0, 3) == "LAX") { data = 0x06; validInstruction = true; includeMemoryAddress = true; }   // Load A, X
        if (line.substr(0, 3) == "LAY") { data = 0x07; validInstruction = true; includeMemoryAddress = true; }   // Load A, Y
        if (line.substr(0, 3) == "LBX") { data = 0x08; validInstruction = true; includeMemoryAddress = true; }   // Load B, X
        if (line.substr(0, 3) == "LBY") { data = 0x09; validInstruction = true; includeMemoryAddress = true; }   // Load B, Y
        if (line.substr(0, 3) == "SAX") { data = 0x0A; validInstruction = true; includeMemoryAddress = true; }   // Store A, X
        if (line.substr(0, 3) == "SAY") { data = 0x0B; validInstruction = true; includeMemoryAddress = true; }   // Store A, Y
        if (line.substr(0, 3) == "SBX") { data = 0x0C; validInstruction = true; includeMemoryAddress = true; }   // Store B, X
        if (line.substr(0, 3) == "SBY") { data = 0x0D; validInstruction = true; includeMemoryAddress = true; }   // Store B, Y
        if (line.substr(0, 3) == "INX") { data = 0x0E; validInstruction = true; }                                // Increment X
        if (line.substr(0, 3) == "INY") { data = 0x0F; validInstruction = true; }                                // Increment Y
        if (line.substr(0, 3) == "DEX") { data = 0x10; validInstruction = true; }                                // Decrement X
        if (line.substr(0, 3) == "DEY") { data = 0x11; validInstruction = true; }                                // Decrement Y
        if (line.substr(0, 3) == "LDX") { data = 0x12; validInstruction = true; includeMemoryAddress = true; }   // Load X
        if (line.substr(0, 3) == "LDY") { data = 0x13; validInstruction = true; includeMemoryAddress = true; }   // Load Y
        if (line.substr(0, 3) == "LIX") { data = 0x14; validInstruction = true; includeImmediateValue = true; }  // Load Immediate X
        if (line.substr(0, 3) == "LIY") { data = 0x15; validInstruction = true; includeImmediateValue = true; }  // Load Immediate Y
        if (line.substr(0, 3) == "ADD") { data = 0x16; validInstruction = true; }                                // Addition
        if (line.substr(0, 3) == "SUB") { data = 0x17; validInstruction = true; }                                // Subtraction
        if (line.substr(0, 3) == "AND") { data = 0x18; validInstruction = true; }                                // Bitwise AND
        if (line.substr(0, 2) == "OR" ) { data = 0x19; validInstruction = true; }                                // Bitwise OR
        if (line.substr(0, 3) == "XOR") { data = 0x1A; validInstruction = true; }                                // Bitwise XOR
        if (line.substr(0, 3) == "JMP") { data = 0x1B; validInstruction = true; includeMemoryAddress = true; }   // Jump
        if (line.substr(0, 3) == "JIZ") { data = 0x1C; validInstruction = true; includeMemoryAddress = true; }   // Jump if Zero
        if (line.substr(0, 3) == "JIC") { data = 0x1D; validInstruction = true; includeMemoryAddress = true; }   // Jump if Carry
        if (line.substr(0, 3) == "JSR") { data = 0x1E; validInstruction = true; includeMemoryAddress = true; }   // Jump to Subroutine
        if (line.substr(0, 3) == "RTS") { data = 0x1F; validInstruction = true; }                                // Return from Subroutine
        if (line.substr(0, 3) == "INP") { data = 0x20; validInstruction = true; includeMemoryAddress = true; }   // Input
        if (line.substr(0, 3) == "DRW") { data = 0x21; validInstruction = true; }                                // Draw
        if (line.substr(0, 3) == "HLT") { data = 0x22; validInstruction = true; }                                // Halt
        if (validInstruction) {
            binaryFile.write(reinterpret_cast<char*>(&data), sizeof(char));
            numBytesWritten++;
        }

        // Include Memory Address Operand
        if (includeMemoryAddress) {
            std::string memoryAddressHighString = "0x" + line.substr(6, 2);
            std::string memoryAddressLowString = "0x" + line.substr(8, 2);

            unsigned char memoryAddressHighValue = static_cast<unsigned char>(std::stoi(memoryAddressHighString, nullptr, 16));
            unsigned char memoryAddressLowValue = static_cast<unsigned char>(std::stoi(memoryAddressLowString, nullptr, 16));

            binaryFile.write(reinterpret_cast<char*>(&memoryAddressHighValue), sizeof(char));
            binaryFile.write(reinterpret_cast<char*>(&memoryAddressLowValue), sizeof(char));
            numBytesWritten++;
            numBytesWritten++;
            continue;
        }

        // Include Immediate Operand
        if (includeImmediateValue) {
            std::string immediateValueString = "0x" + line.substr(6, 2);
            unsigned char immediateValue = static_cast<unsigned char>(std::stoi(immediateValueString, nullptr, 16));

            binaryFile.write(reinterpret_cast<char*>(&immediateValue), sizeof(char));
            numBytesWritten++;
            continue;
        }
    }

    // Variables
    for (std::string line : lines) {
        if (line.substr(0, 3) != "VAR") {
            continue;
        }
        std::string memoryAddressHighString = "0x" + line.substr(6, 2);
        std::string memoryAddressLowString = "0x" + line.substr(8, 2);
        std::string immediateValueString = "0x" + line.substr(13, 2);

        unsigned char memoryAddressHighValue = static_cast<unsigned char>(std::stoi(memoryAddressHighString, nullptr, 16));
        unsigned char memoryAddressLowValue = static_cast<unsigned char>(std::stoi(memoryAddressLowString, nullptr, 16));
        unsigned char immediateValue = static_cast<unsigned char>(std::stoi(immediateValueString, nullptr, 16));

        int pointerValue = memoryAddressLowValue + (256 * memoryAddressHighValue);

        binaryFile.seekp(pointerValue, std::ios::beg);

        if (!binaryFile) {
            std::cerr << "Error seeking position in file!" << std::endl;
            return 1;
        }

        binaryFile.write(reinterpret_cast<char*>(&immediateValue), sizeof(char));
    }

    binaryFile.close(); 

    if (!binaryFile.good()) {
        std::cerr << "Error: Writing to the file failed!" << std::endl;
        return 1;
    }

    std::cout << "File written successfully!" << std::endl;

    return 0;
}

