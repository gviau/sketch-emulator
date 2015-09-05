#include "Memory.h"

namespace SketchEmulator {

Memory::Memory()
    : m_CartridgeType(ROM_ONLY)
    , m_NumberOfRomBanks(1)
    , m_NumberOfRamBanks(1)
    , m_RomBank(nullptr)
    , m_SwitchableRomBank(nullptr)
    , m_SwitchableRam(nullptr)
    , m_InternalRam(nullptr)
    , m_SmallInternalRam(nullptr)
    , m_IOPorts(nullptr)
    , m_InterruptEnableRegister(0)
{
}

Memory::~Memory()
{
    delete[] m_SwitchableRam;
    delete[] m_InternalRam;
    delete[] m_SmallInternalRam;
    delete[] m_IOPorts;
}

bool Memory::Initialize(const string& cartridge, const CartridgeInfo_t& cartridgeInfo)
{
    if (cartridge.size() == 0)
    {
        return false;
    }

    m_Cartridge = cartridge;

    m_CartridgeType = cartridgeInfo.m_CartridgeType;
    m_NumberOfRomBanks = cartridgeInfo.m_NumberOfRomBanks;
    m_NumberOfRamBanks = cartridgeInfo.m_NumberOfRamBanks;

    m_RomBank = (unsigned char*)&m_Cartridge[0];
    m_SwitchableRam = (unsigned char*)&m_Cartridge[0x4000];

    m_SwitchableRam = new unsigned char[8192];
    m_InternalRam = new unsigned char[8192];
    m_IOPorts = new unsigned char[0x4C];
    m_SmallInternalRam = new unsigned char[0x7F];

    memset(m_SwitchableRam, 0, 8192);
    memset(m_InternalRam, 0, 8192);
    memset(m_IOPorts, 0, 0x4C);
    memset(m_SmallInternalRam, 0, 0x7F);

    return true;
}

void Memory::WriteByteToAddress(unsigned short address, unsigned char value)
{
    if (address >= 0x2000 && address < 0x4000)
    {
        unsigned char romBank = (value & 0x0F);
        unsigned char* romBankStart = (unsigned char*)&m_Cartridge[((m_Cartridge.size() / m_NumberOfRomBanks) * romBank)];
        return;
    }

    unsigned short relativeAddress = 0;
    unsigned char* memoryToWriteTo = GetMemBankFromAddress(address, relativeAddress);

    if (memoryToWriteTo != nullptr)
    {
        memoryToWriteTo[relativeAddress] = value;
    }
}

void Memory::WriteShortToAddress(unsigned short address, unsigned short value)
{
    unsigned short relativeAddress = 0;
    unsigned char* memoryToWriteTo = GetMemBankFromAddress(address, relativeAddress);

    if (memoryToWriteTo != nullptr)
    {
        unsigned char lsb = 0;
        unsigned char msb = 0;
        ConvertShortTo2Bytes(value, lsb, msb);

        memoryToWriteTo[relativeAddress    ] = lsb;
        memoryToWriteTo[relativeAddress + 1] = msb;
    }
}

void Memory::Read1ByteFromMem(unsigned short address, unsigned char& byte) const
{
    unsigned short relativeAddress = 0;
    unsigned char* memoryToReadFrom = GetMemBankFromAddress(address, relativeAddress);

    byte = memoryToReadFrom[relativeAddress];
}

void Memory::Read2BytesFromMem(unsigned short address, unsigned char& msb, unsigned char& lsb) const
{
    unsigned short relativeAddress = 0;
    unsigned char* memoryToReadFrom = GetMemBankFromAddress(address, relativeAddress);

    lsb = memoryToReadFrom[relativeAddress    ];
    msb = memoryToReadFrom[relativeAddress + 1];
}

unsigned short Memory::Convert2BytesToShort(unsigned char msb, unsigned char lsb) const
{
    unsigned short value = ( (unsigned short)lsb | ((unsigned short)msb << 8) );
    return value;
}

void Memory::ConvertShortTo2Bytes(unsigned short value, unsigned char& msb, unsigned char& lsb) const
{
    lsb = (unsigned char)(value & 0xFF);
    msb = (unsigned char)( (value & 0xFF00) >> 8);
}

unsigned char* Memory::GetMemBankFromAddress(unsigned short address, unsigned short& relativeAddress) const
{
    if (address < 0x4000)
    {
        relativeAddress = address;
        return m_RomBank;
    }
    else if (address < 0x8000)
    {
        relativeAddress = address - 0x4000;
        return m_SwitchableRomBank;
    }
    else if (address >= 0xA000 && address < 0xC000)
    {
        relativeAddress = address - 0xA000;
        return m_SwitchableRam;
    }
    else if (address >= 0xC000 && address < 0xFE00)
    {
        if (address > 0xE000)
        {
            relativeAddress = address - 0xE000;
        }
        else
        {
            relativeAddress = address - 0xC000;
        }
        return m_InternalRam;
    }
    else if (address >= 0xFF00 && address < 0xFF4C)
    {
        relativeAddress = address - 0xFF00;
        return m_IOPorts;
    }
    else if (address >= 0xFF80 && address < 0xFFFF)
    {
        relativeAddress = address - 0xFF80;
        return m_SmallInternalRam;
    }
    else if (address == 0xffff)
    {
        relativeAddress = 0;
        return (unsigned char*)&m_InterruptEnableRegister;
    }

    return nullptr;
}

}