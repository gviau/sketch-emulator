#ifndef SKETCH_EMULATOR_MEMORY_H
#define SKETCH_EMULATOR_MEMORY_H

#include <vector>
using namespace std;

namespace SketchEmulator {

// The values are set to correspond to the one we find in the cartridge
enum CartridgeType_t
{
    ROM_ONLY,
    ROM_MBC1,
    ROM_MBC1_RAM,
    ROM_MBC1_RAM_BATT,
    ROM_MBC2 = 5,
    ROM_MBC2_BATT,
    ROM_RAM = 8,
    ROM_RAM_BATT,
    ROM_MMMD1 = 0x0B,
    ROM_MMMD1_SRAM,
    ROM_MMMD1_SRAM_BATT,
    ROM_MBC3_TIMER_BATT = 0x0F,
    ROM_MBC3_TIMER_RAM_BATT,
    ROM_MBC3,
    ROM_MBC3_RAM,
    ROM_MBC3_RAM_BATT,
    ROM_MBC5 = 0x19,
    ROM_MBC5_RAM,
    ROM_MBC5_RAM_BATT,
    ROM_MBC5_RUMBLE,
    ROM_MBC5_RUMBLE_SRAM,
    ROM_MBC5_RUMBLE_SRAM_BATT,
    POCKET_CAMERA,
    BANDAI_TAMA5 = 0xFD,
    HUDSON_HUC3 = 0xFE,
    HUDSON_HUC1,

    ROM_MAX
};

struct CartridgeInfo_t
{
    char m_GameName[16];
    bool m_GameboyColor;
    bool m_SuperGameboy;
    CartridgeType_t m_CartridgeType;
    unsigned char m_NumberOfRomBanks;
    unsigned char m_NumberOfRamBanks;
};

class Memory
{
public:
    Memory();
    ~Memory();

    bool Initialize(const string& cartridge, const CartridgeInfo_t& cartridgeInfo);

    void WriteByteToAddress(unsigned short address, unsigned char value);
    void WriteShortToAddress(unsigned short address, unsigned short value);

    void Read1ByteFromMem(unsigned short address, unsigned char& byte) const;
    void Read2BytesFromMem(unsigned short address, unsigned char& msb, unsigned char& lsb) const;

    unsigned short Convert2BytesToShort(unsigned char msb, unsigned char lsb) const;
    void ConvertShortTo2Bytes(unsigned short value, unsigned char& msb, unsigned char& lsb) const;

private:
    CartridgeType_t m_CartridgeType;
    unsigned char   m_NumberOfRomBanks;
    unsigned char   m_NumberOfRamBanks;

    unsigned char*  m_RomBank;
    unsigned char*  m_SwitchableRomBank;

    unsigned char*  m_SwitchableRam;
    unsigned char*  m_InternalRam;
    unsigned char*  m_SmallInternalRam;

    unsigned char*  m_IOPorts;

    string          m_Cartridge;
    unsigned char   m_InterruptEnableRegister;

    unsigned char* GetMemBankFromAddress(unsigned short address, unsigned short& relativeAddress) const;
};

}

#endif