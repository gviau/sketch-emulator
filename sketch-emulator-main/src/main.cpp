#include "CPU.h"
#include "Memory.h"
using namespace SketchEmulator;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void ReadRomInformation(unsigned char* startOfInfo, CartridgeInfo_t& cartridgeInfo);

int main(int argc, char** argv)
{
    /*
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " rom" << endl;
        return 1;
    }
    */

    //string romName(argv[1]);
    string romName("C:\\Users\\Gauthier\\Desktop\\Pokemon_red.gb");
    ifstream romFile(romName, ios::binary);
    if (!romFile.is_open())
    {
        cerr << "Couldn't open rom " << romName << endl;
        return 1;
    }

    string romContent;
    romFile.seekg(0, std::ios::end);
    romContent.reserve((size_t) romFile.tellg());
    romContent.resize((size_t) romFile.tellg());
    romFile.seekg(0, std::ios::beg);

    romFile.read(&romContent[0], romContent.length());

    CartridgeInfo_t cartridgeInfo;
    ReadRomInformation((unsigned char*)&romContent[0x0134], cartridgeInfo);

    Memory memory;
    memory.Initialize(romContent, cartridgeInfo);

    CPU cpu(memory);
    cpu.Initialize(cartridgeInfo);

    while (true)
    {
        cpu.Step(true);
    }

    return 0;
}

void ReadRomInformation(unsigned char* startOfInfo, CartridgeInfo_t& cartridgeInfo)
{
    for (unsigned char i = 0; i < 16; i++)
    {
        cartridgeInfo.m_GameName[i] = (char)*startOfInfo++;
    }

    if (*startOfInfo++ == 0x80)
    {
        cartridgeInfo.m_GameboyColor = true;
    }
    else
    {
        cartridgeInfo.m_GameboyColor = false;
    }

    startOfInfo += 2;
    if (*startOfInfo++ == 0x00)
    {
        cartridgeInfo.m_SuperGameboy = false;
    }
    else
    {
        cartridgeInfo.m_SuperGameboy = true;
    }

    cartridgeInfo.m_CartridgeType = (CartridgeType_t)(*startOfInfo++);

    switch (*startOfInfo++)
    {
        case 0:     cartridgeInfo.m_NumberOfRomBanks = 2; break;
        case 1:     cartridgeInfo.m_NumberOfRomBanks = 4; break;
        case 2:     cartridgeInfo.m_NumberOfRomBanks = 8; break;
        case 3:     cartridgeInfo.m_NumberOfRomBanks = 16; break;
        case 4:     cartridgeInfo.m_NumberOfRomBanks = 32; break;
        case 5:     cartridgeInfo.m_NumberOfRomBanks = 64; break;
        case 6:     cartridgeInfo.m_NumberOfRomBanks = 128; break;
        case 0x52:  cartridgeInfo.m_NumberOfRomBanks = 72; break;
        case 0x53:  cartridgeInfo.m_NumberOfRomBanks = 80; break;
        case 0x54:  cartridgeInfo.m_NumberOfRomBanks = 96; break;
    }

    switch (*startOfInfo++)
    {
        case 0: cartridgeInfo.m_NumberOfRamBanks = 0; break;
        case 1: cartridgeInfo.m_NumberOfRamBanks = 1; break;
        case 2: cartridgeInfo.m_NumberOfRamBanks = 1; break;
        case 3: cartridgeInfo.m_NumberOfRamBanks = 4; break;
        case 4: cartridgeInfo.m_NumberOfRamBanks = 16; break;
    }
}