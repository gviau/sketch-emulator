#ifndef SKETCH_EMULATOR_MEMORY_H
#define SKETCH_EMULATOR_MEMORY_H

namespace SketchEmulator {

class Memory
{
public:
    Memory();
    ~Memory();

    bool Initialize();

    void WriteByteToAddress(unsigned short address, unsigned char value);
    void WriteShortToAddress(unsigned short address, unsigned short value);

    void Read1ByteFromMem(unsigned short address, unsigned char& byte) const;
    void Read2BytesFromMem(unsigned short address, unsigned char& msb, unsigned char& lsb) const;

    unsigned short Convert2BytesToShort(unsigned char msb, unsigned char lsb) const;
    void ConvertShortTo2Bytes(unsigned short value, unsigned char& msb, unsigned char& lsb) const;

private:
    unsigned char* m_RomBank;
    unsigned char* m_SwitchableRomBank;

    unsigned char* m_SwitchableRam;
    unsigned char* m_InternalRam;

    unsigned char* GetMemBankFromAddress(unsigned short address, unsigned short& relativeAddress) const;
};

}

#endif