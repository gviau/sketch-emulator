#include "Memory.h"

namespace SketchEmulator {

Memory::Memory()
    : m_RomBank(nullptr)
    , m_SwitchableRomBank(nullptr)
    , m_SwitchableRam(nullptr)
    , m_InternalRam(nullptr)
{
}

Memory::~Memory()
{
    delete[] m_RomBank;
    delete[] m_InternalRam;
}

bool Memory::Initialize()
{
    m_RomBank = new unsigned char[16384];
    m_InternalRam = new unsigned char[8192];

    return true;
}

void Memory::WriteByteToAddress(unsigned short address, unsigned char value)
{
    unsigned short relativeAddress = 0;
    unsigned char* memoryToWriteTo = GetMemBankFromAddress(address, relativeAddress);

    memoryToWriteTo[relativeAddress] = value;
}

void Memory::WriteShortToAddress(unsigned short address, unsigned short value)
{
    unsigned short relativeAddress = 0;
    unsigned char* memoryToWriteTo = GetMemBankFromAddress(address, relativeAddress);

    unsigned char lsb = 0;
    unsigned char msb = 0;
    ConvertShortTo2Bytes(value, lsb, msb);

    memoryToWriteTo[relativeAddress    ] = lsb;
    memoryToWriteTo[relativeAddress + 1] = msb;
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

    return nullptr;
}

}