
//
// Created by is4grewa on 12/31/2019.
//

#ifndef EMULATIONPROJECT_DRIVER_H
#define EMULATIONPROJECT_DRIVER_H
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
typedef unsigned char BYTE;
typedef unsigned short int WORD;


class driver {
public:

private:
    BYTE m_fgame_mem[0xFFF]; //full mem of the game
    BYTE m_registers[16]; //creating 16 1 byte registers
    WORD m_Addy;
    WORD m_PC; //program counter
    std::vector<WORD> m_stack;
    void cpuReset();
    WORD GetNextOpcode();
    void DecodeOpcode();
    void Opcode1NNN(WORD opcode);
    void Opcode00E0(WORD opcode);
    void Opcode00EE(WORD opcode);
    void Opcode2NNN(WORD opcode);
    void Opcode3XNN(WORD opcode);
    void Opcode4XNN(WORD opcode);
    void Opcode5XY0(WORD opcode);
    void Opcode6XNN(WORD opcode);
    void Opcode7XNN(WORD opcode);
    void Opcode8XY0(WORD opcode);
    void Opcode8XY1(WORD opcode);
    void Opcode8XY2(WORD opcode);
    void Opcode8XY3(WORD opcode);
    void Opcode8XY4(WORD opcode);
    void Opcode8XY5(WORD opcode);
    void Opcode8XY6(WORD opcode);
    void Opcode8XY7(WORD opcode);
    void Opcode8XYE(WORD opcode);

};

void driver::cpuReset() {
    m_Addy = 0;
    m_PC = 0x200; //Setting PC to point where chip8 spec game is loaded in at this location
    FILE *in;
    in = fopen("c:/Invaders", "rb");
    fread(&m_fgame_mem[0x200], 0xfff, 1, in);
    fclose(in);
}

WORD driver::GetNextOpcode() {
    WORD res = 0;
    res = m_fgame_mem[m_PC];
    res <<= 8; //opcode is 1 word long so we shift the first half of the instruction by 8 bits (bc each m_fgame_mem is 1 byte)
    m_PC++;
    res = res | m_fgame_mem[m_PC];
    m_PC++;
    return res;
}
void driver::DecodeOpcode() {
    WORD opcode = GetNextOpcode();
    switch (opcode & 0xF000) {
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x000E:
                    Opcode00EE(opcode);
                    break;
                case 0x0000:
                    Opcode00E0(opcode);
                    break;
            }

        case 0x1000:
            Opcode1NNN(opcode);
            break;
        case 0x2000:
            Opcode2NNN(opcode);
            break;
        case 0x3000:
            Opcode3XNN(opcode);
            break;
        case 0x4000:
            Opcode4XNN(opcode);
            break;
        case 0x5000:
            Opcode5XY0(opcode);
            break;
        case 0x6000:
            Opcode6XNN(opcode);
            break;
        case 0x7000:
            Opcode7XNN(opcode);
            break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000:
                    Opcode8XY0(opcode);
                    break;
                case 0x0001:
                    Opcode8XY1(opcode);
                    break;
                case 0x0002:
                    Opcode8XY2(opcode);
                    break;
                case 0x0003:
                    Opcode8XY3(opcode);
                    break;
                case 0x0004:
                    Opcode8XY4(opcode);
                    break;
                case 0x0005:
                    Opcode8XY5(opcode);
                    break;
                case 0x0006:
                    Opcode8XY6(opcode);
                    break;
                case 0x0007:
                    Opcode8XY7(opcode);
                    break;
                case 0x000E:
                    Opcode8XYE(opcode);
                    break;


            }
            break;
    }


}

void driver::Opcode00EE(WORD opcode) {
    m_PC = m_stack.front();
    m_stack.erase(m_stack.begin());
}

void driver::Opcode00E0(WORD opcode){
//TODO
}

void driver::Opcode1NNN(WORD opcode) {
    m_PC = opcode & 0x0FFF;
}

void driver::Opcode2NNN(WORD opcode) {
    m_stack.push_back(m_PC);
    m_PC = opcode & 0x0FFF;
}

void driver::Opcode3XNN(WORD opcode) {
    int xVal = opcode & 0x0F00;
    xVal >>= 8;
    int check = opcode & 0x00FF;
    if(m_registers[xVal] == check) {
        m_PC +=2;
    }
}

void driver::Opcode4XNN(WORD opcode) {
    int xVal = opcode & 0x0F00;
    xVal >>= 8;
    int check = opcode & 0x00FF;
    if(m_registers[xVal] != check) {
        m_PC +=2;
    }
}

void driver::Opcode5XY0(WORD opcode) {
    int xVal = opcode & 0x0F00;
    int yVal = opcode & 0x00F0;
    xVal >>= 8;
    yVal >>= 4;
    if(m_registers[xVal] == m_registers[yVal]) {
        m_PC += 2;
    }
}

void driver::Opcode6XNN(WORD opcode) {
    int xVal = opcode & 0x0F00;
    xVal >>= 8;
    int check = opcode & 0x00FF;
    m_registers[xVal] = check;
}

void driver::Opcode7XNN(WORD opcode) {
    int xVal = opcode & 0x0F00;
    xVal >>= 8;
    int check = opcode & 0x00FF;
    m_registers[xVal] += check;
}

void driver::Opcode8XY0(WORD opcode) {
    int xVal = opcode & 0x0F00;
    int yVal = opcode & 0x00F0;
    xVal >>= 8;
    yVal >>= 4;
    m_registers[xVal] = m_registers[yVal];
}

void driver::Opcode8XY1(WORD opcode) {
    int xVal = opcode & 0x0F00;
    int yVal = opcode & 0x00F0;
    xVal >>= 8;
    yVal >>= 4;
    m_registers[xVal] = m_registers[yVal] | m_registers[xVal];
}

void driver::Opcode8XY2(WORD opcode) {
    int xVal = opcode & 0x0F00;
    int yVal = opcode & 0x00F0;
    xVal >>= 8;
    yVal >>= 4;
    m_registers[xVal] = m_registers[yVal] & m_registers[xVal];
}

void driver::Opcode8XY3(WORD opcode) {
    int xVal = opcode & 0x0F00;
    int yVal = opcode & 0x00F0;
    xVal >>= 8;
    yVal >>= 4;
    m_registers[xVal] = m_registers[yVal] & m_registers[xVal];
}
#endif //EMULATIONPROJECT_DRIVER_H
