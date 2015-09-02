#include "CPU.h"

namespace SketchEmulator {

// From https://isocpp.org/wiki/faq/pointers-to-members#macro-for-ptr-to-memfn
#define CALL_MEMBER_FUNCTION(object, ptrToMember) ((object).*(ptrToMember))

CPU::CPU()
    : m_A(0)
    , m_B(0)
    , m_C(0)
    , m_D(0)
    , m_E(0)
    , m_F(0)
    , m_H(0)
    , m_L(0)
    , m_SP(0)
    , m_PC(0)
    , m_IsCPUStopped(false)
{
    m_Opcodes[0x00] = &CPU::nop;
    m_Opcodes[0x01] = &CPU::ld_bc_d16;
    m_Opcodes[0x02] = &CPU::ld_mem_bc_a;
    m_Opcodes[0x03] = &CPU::inc_bc;
    m_Opcodes[0x04] = &CPU::inc_b;
    m_Opcodes[0x05] = &CPU::dec_b;
    m_Opcodes[0x06] = &CPU::ld_b_d8;
    m_Opcodes[0x07] = &CPU::rlca;
    m_Opcodes[0x08] = &CPU::ld_mem_a16_sp;
    m_Opcodes[0x09] = &CPU::add_hl_bc;
    m_Opcodes[0x0A] = &CPU::ld_a_mem_bc;
    m_Opcodes[0x0B] = &CPU::dec_bc;
    m_Opcodes[0x0C] = &CPU::inc_c;
    m_Opcodes[0x0D] = &CPU::dec_c;
    m_Opcodes[0x0E] = &CPU::ld_c_d8;
    m_Opcodes[0x0F] = &CPU::rrca;

    m_Opcodes[0x10] = &CPU::stop;
    m_Opcodes[0x11] = &CPU::ld_de_d16;
    m_Opcodes[0x12] = &CPU::ld_mem_de_a;
    m_Opcodes[0x13] = &CPU::inc_de;
    m_Opcodes[0x14] = &CPU::inc_d;
    m_Opcodes[0x15] = &CPU::dec_d;
    m_Opcodes[0x16] = &CPU::ld_d_d8;
    m_Opcodes[0x17] = &CPU::rla;
    m_Opcodes[0x18] = &CPU::jr_r8;
    m_Opcodes[0x19] = &CPU::add_hl_de;
    m_Opcodes[0x1A] = &CPU::ld_a_mem_de;
    m_Opcodes[0x1B] = &CPU::dec_de;
    m_Opcodes[0x1C] = &CPU::inc_e;
    m_Opcodes[0x1D] = &CPU::dec_e;
    m_Opcodes[0x1E] = &CPU::ld_e_d8;
    m_Opcodes[0x1F] = &CPU::rra;

    m_Opcodes[0x20] = &CPU::jr_nz_r8;
    m_Opcodes[0x21] = &CPU::ld_hl_d16;
    m_Opcodes[0x22] = &CPU::ld_hl_p_a;
    m_Opcodes[0x23] = &CPU::inc_hl;
    m_Opcodes[0x24] = &CPU::inc_h;
    m_Opcodes[0x25] = &CPU::dec_h;
    m_Opcodes[0x26] = &CPU::ld_h_d8;
    m_Opcodes[0x27] = &CPU::daa;
    m_Opcodes[0x28] = &CPU::jr_z_r8;
    m_Opcodes[0x29] = &CPU::add_hl_hl;
    m_Opcodes[0x2A] = &CPU::ld_a_hl_p;
    m_Opcodes[0x2B] = &CPU::dec_hl;
    m_Opcodes[0x2C] = &CPU::inc_l;
    m_Opcodes[0x2D] = &CPU::dec_l;
    m_Opcodes[0x2E] = &CPU::ld_l_d8;
    m_Opcodes[0x2F] = &CPU::cpl;
}

void CPU::Initialize()
{
    m_Memory.Initialize();
}

int CPU::Step()
{
    unsigned char opcode = 0;
    m_Memory.Read1ByteFromMem(m_PC, opcode);
    m_PC += 1;

    if (opcode == 0xCB)
    {
        m_Memory.Read1ByteFromMem(m_PC, opcode);
        m_PC += 1;

        return CALL_MEMBER_FUNCTION(*this, m_OpcodesPrefixCb[opcode])();
    }

    return CALL_MEMBER_FUNCTION(*this, m_Opcodes[opcode])();
}

int CPU::nop()
{
    return 4;
}

int CPU::ld_bc_d16()
{
    return LoadD16InReg(m_B, m_C);
}

int CPU::ld_mem_bc_a()
{
    unsigned short BC = m_Memory.Convert2BytesToShort(m_B, m_C);
    m_Memory.WriteByteToAddress(BC, m_A);
    return 8;
}

int CPU::inc_bc()
{
    return IncShortRegister(m_B, m_C);
}

int CPU::inc_b()
{
    return IncByteRegister(m_B);
}

int CPU::dec_b()
{
    return DecByteRegister(m_B);
}

int CPU::ld_b_d8()
{
    return LoadD8InReg(m_B);
}

int CPU::rlca()
{
    return RotateByteLeftWithCarryFlag(m_A);
}

int CPU::ld_mem_a16_sp()
{
    unsigned char lsb, msb;
    m_Memory.Read2BytesFromMem(m_PC, lsb, msb);
    m_PC += 2;

    unsigned address = m_Memory.Convert2BytesToShort(lsb, msb);
    m_Memory.WriteShortToAddress(address, m_SP);

    return 20;
}

int CPU::add_hl_bc()
{
    return AddShortToShort(m_B, m_C, m_H, m_L);
}

int CPU::ld_a_mem_bc()
{
    unsigned short BC = m_Memory.Convert2BytesToShort(m_B, m_C);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(BC, value);

    m_A = value;

    return 8;
}

int CPU::dec_bc()
{
    return DecShortRegister(m_B, m_C);
}

int CPU::inc_c()
{
    return IncByteRegister(m_C);
}

int CPU::dec_c()
{
    return DecByteRegister(m_C);
}

int CPU::ld_c_d8()
{
    return LoadD8InReg(m_C);
}

int CPU::rrca()
{
    return RotateByteRightWithCarryFlag(m_A);
}

int CPU::stop()
{
    m_IsCPUStopped = true;
    return 4;
}

int CPU::ld_de_d16()
{
    return LoadD16InReg(m_D, m_E);
}

int CPU::ld_mem_de_a()
{
    unsigned short DE = m_Memory.Convert2BytesToShort(m_D, m_E);
    m_Memory.WriteByteToAddress(DE, m_A);
    return 8;
}

int CPU::inc_de()
{
    return IncShortRegister(m_D, m_E);
}

int CPU::inc_d()
{
    return IncByteRegister(m_D);
}

int CPU::dec_d()
{
    return DecByteRegister(m_D);
}

int CPU::ld_d_d8()
{
    return LoadD8InReg(m_D);
}

int CPU::rla()
{
    return RotateByteLeftThroughCarryFlag(m_A);
}

int CPU::jr_r8()
{
    unsigned char r8;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;
    m_PC += r8;

    return 8;
}

int CPU::add_hl_de()
{
    return AddShortToShort(m_D, m_E, m_H, m_L);
}

int CPU::ld_a_mem_de()
{
    unsigned short address = m_Memory.Convert2BytesToShort(m_D, m_E);
    m_Memory.Read1ByteFromMem(address, m_A);
    return 8;
}

int CPU::dec_de()
{
    return DecShortRegister(m_D, m_E);
}

int CPU::inc_e()
{
    return IncByteRegister(m_E);
}

int CPU::dec_e()
{
    return DecByteRegister(m_E);
}

int CPU::ld_e_d8()
{
    return LoadD8InReg(m_E);
}

int CPU::rra()
{
    return RotateByteRightThroughCarryFlag(m_A);
}

int CPU::jr_nz_r8()
{
    unsigned char r8;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    if (!GetZeroFlag())
    {
        m_PC += r8;
    }

    return 8;
}

int CPU::ld_hl_d16()
{
    return LoadD16InReg(m_H, m_L);
}

int CPU::ld_hl_p_a()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.WriteByteToAddress(HL, m_A);
    HL += 1;
    m_Memory.ConvertShortTo2Bytes(HL, m_H, m_L);

    return 8;
}

int CPU::inc_hl()
{
    return IncShortRegister(m_H, m_L);
}

int CPU::inc_h()
{
    return IncByteRegister(m_H);
}

int CPU::dec_h()
{
    return DecByteRegister(m_H);
}

int CPU::ld_h_d8()
{
    return LoadD8InReg(m_H);
}

int CPU::daa()
{
    return 4;
}

int CPU::jr_z_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    if (GetZeroFlag())
    {
        m_PC += r8;
    }

    return 8;
}

int CPU::add_hl_hl()
{
    unsigned char h = m_H;
    unsigned char l = m_L;
    return AddShortToShort(h, l, m_H, m_L);
}

int CPU::ld_a_hl_p()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    m_A = value;
    HL += 1;

    m_Memory.ConvertShortTo2Bytes(HL, m_H, m_L);

    return 8;
}

int CPU::dec_hl()
{
    return DecShortRegister(m_H, m_L);
}

int CPU::inc_l()
{
    return IncByteRegister(m_L);
}

int CPU::dec_l()
{
    return DecByteRegister(m_L);
}

int CPU::ld_l_d8()
{
    return LoadD8InReg(m_L);
}

int CPU::cpl()
{
    m_A = ~m_A;

    SetSubtractFlag(true);
    SetHalfCarryFlag(true);

    return 4;
}

void CPU::SetZeroFlag(bool value)
{
    if (value)
    {
        m_F |= 0x80;
    }
    else
    {
        m_F &= (~0x80);
    }
}

void CPU::SetSubtractFlag(bool value)
{
    if (value)
    {
        m_F |= 0x40;
    }
    else
    {
        m_F &= (~0x40);
    }
}

void CPU::SetHalfCarryFlag(bool value)
{
    if (value)
    {
        m_F |= 0x20;
    }
    else
    {
        m_F &= (~0x20);
    }
}

void CPU::SetCarryFlag(bool value)
{
    if (value)
    {
        m_F |= 0x10;
    }
    else
    {
        m_F &= (~0x10);
    }
}

bool CPU::GetZeroFlag() const
{
    return (m_F & 0x80) > 0;
}

bool CPU::GetSubtractFlag() const
{
    return (m_F & 0x40) > 0;
}

bool CPU::GetHalfCarryFlag() const
{
    return (m_F & 0x20) > 0;
}

bool CPU::GetCarryFlag() const
{
    return (m_F & 0x10) > 0;
}

bool CPU::IsCarryForBit(unsigned short oldValue, unsigned short increment, unsigned short bit) const
{
    return ( ((oldValue & 0xFF) + (increment & 0xFF)) & bit ) > 0;
}

int CPU::IncByteRegister(unsigned char& reg)
{
    unsigned char newValue = reg + 1;

    SetZeroFlag(newValue == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(IsCarryForBit(reg, 1, 0x10));
    
    reg = newValue;

    return 4;
}

int CPU::DecByteRegister(unsigned char& reg)
{
    unsigned char newValue = reg - 1;

    SetZeroFlag( newValue == 0 );
    SetSubtractFlag(true);
    SetHalfCarryFlag( IsCarryForBit(reg, -1, 0x10) );

    reg = newValue;

    return 4;
}

int CPU::IncShortRegister(unsigned char& lsb, unsigned char& msb)
{
    unsigned short reg = m_Memory.Convert2BytesToShort(lsb, msb);
    reg += 1;
    m_Memory.ConvertShortTo2Bytes(reg, lsb, msb);
    return 8;
}

int CPU::DecShortRegister(unsigned char& lsb, unsigned char& msb)
{
    unsigned short reg = m_Memory.Convert2BytesToShort(lsb, msb);
    reg -= 1;
    m_Memory.ConvertShortTo2Bytes(reg, lsb, msb);
    return 8;
}

int CPU::AddShortToShort(unsigned char lsbToAdd, unsigned char msbToAdd, unsigned char& lsbResult, unsigned char& msbResult)
{
    unsigned short regToAdd = m_Memory.Convert2BytesToShort(lsbToAdd, msbToAdd);
    unsigned short resultReg = m_Memory.Convert2BytesToShort(lsbResult, msbResult);
    unsigned short newValue = regToAdd + resultReg;

    SetSubtractFlag(false);
    SetCarryFlag( IsCarryForBit(resultReg, newValue - resultReg, 0x8000) );
    SetHalfCarryFlag( IsCarryForBit(resultReg, newValue - resultReg, 0x1000) );

    m_Memory.ConvertShortTo2Bytes(newValue, lsbResult, msbResult);

    return 8;
}

int CPU::RotateByteLeftWithCarryFlag(unsigned char& reg)
{
    unsigned char oldBit7 = (reg & 0x80);

    reg <<= 1;

    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit7 > 0);

    return 4;
}

int CPU::RotateByteRightWithCarryFlag(unsigned char& reg)
{
    unsigned char oldBit0 = (reg & 0x01);

    reg >>= 1;

    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit0 > 0);

    return 4;
}

int CPU::RotateByteLeftThroughCarryFlag(unsigned char& reg)
{
    unsigned char oldBit7 = (reg & 0x80);

    reg <<= 1;
    reg |= (GetCarryFlag() ? 1 : 0);

    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit7 > 0);

    return 4;
}

int CPU::RotateByteRightThroughCarryFlag(unsigned char& reg)
{
    unsigned char oldBit0 = (reg & 0x01);

    reg >>= 1;
    reg |= (GetCarryFlag() ? 0x80 : 0);

    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit0 > 0);

    return 4;
}

int CPU::LoadD8InReg(unsigned char& reg)
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    reg = d8;

    return 8;
}

int CPU::LoadD16InReg(unsigned char& lsb, unsigned char& msb)
{
    m_Memory.Read2BytesFromMem(m_PC, lsb, msb);
    m_PC += 2;
    return 12;
}

}