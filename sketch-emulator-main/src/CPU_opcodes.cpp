#include "CPU.h"

namespace SketchEmulator {
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

    if ( (r8 & 0x80) > 0)
    {
        r8 &= ~0x80;
        m_PC -= r8;
    }
    else
    {
        m_PC += r8;
    }

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
        if ( (r8 & 0x80) > 0)
        {
            r8 &= ~0x80;
            m_PC -= r8;
        }
        else
        {
            m_PC += r8;
        }
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
        if ( (r8 & 0x80) > 0)
        {
            r8 &= ~0x80;
            m_PC -= r8;
        }
        else
        {
            m_PC += r8;
        }
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

int CPU::jr_nc_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    if (!GetCarryFlag())
    {
        if ( (r8 & 0x80) > 0)
        {
            r8 &= ~0x80;
            m_PC -= r8;
        }
        else
        {
            m_PC += r8;
        }
    }

    return 8;
}

int CPU::ld_sp_d16()
{
    unsigned char S, P;
    int cycles = LoadD16InReg(S, P);
    m_SP = m_Memory.Convert2BytesToShort(S, P);
    return cycles;
}

int CPU::ld_hl_m_a()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.WriteByteToAddress(HL, m_A);
    HL -= 1;

    m_Memory.ConvertShortTo2Bytes(HL, m_H, m_L);

    return 8;
}

int CPU::inc_sp()
{
    unsigned char S, P;
    m_Memory.ConvertShortTo2Bytes(m_SP, S, P);

    int cycles = IncShortRegister(S, P);
    m_SP = m_Memory.Convert2BytesToShort(S, P);

    return cycles;
}

int CPU::inc_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    IncByteRegister(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 12;
}

int CPU::dec_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    DecByteRegister(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 12;
}

int CPU::ld_mem_hl_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.WriteByteToAddress(HL, d8);

    return 12;
}

int CPU::scf()
{
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(true);

    return 4;
}

int CPU::jr_c_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    if (GetCarryFlag())
    {
        if ( (r8 & 0x80) > 0)
        {
            r8 &= ~0x80;
            m_PC -= r8;
        }
        else
        {
            m_PC += r8;
        }
    }

    return 8;
}

int CPU::add_hl_sp()
{
    unsigned char S, P;
    m_Memory.ConvertShortTo2Bytes(m_SP, S, P);
    return AddShortToShort(S, P, m_H, m_L);
}

int CPU::ld_a_hl_m()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    m_A = value;

    HL -= 1;

    m_Memory.ConvertShortTo2Bytes(HL, m_H, m_L);
    return 8;
}

int CPU::dec_sp()
{
    unsigned char S, P;
    m_Memory.ConvertShortTo2Bytes(m_SP, S, P);

    int cycles = DecShortRegister(S, P);

    m_SP = m_Memory.Convert2BytesToShort(S, P);

    return cycles;
}

int CPU::inc_a()
{
    return IncByteRegister(m_A);
}

int CPU::dec_a()
{
    return DecByteRegister(m_A);
}

int CPU::ld_a_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    m_A = d8;

    return 8;
}

int CPU::ccf()
{
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(!GetCarryFlag());

    return 4;
}

// I don't want to write all this...
#define LD_B_REG(reg, m) int CPU::ld_b_##reg() { m_B = m_##m; return 4; }
#define LD_C_REG(reg, m) int CPU::ld_c_##reg() { m_C = m_##m; return 4; }
#define LD_D_REG(reg, m) int CPU::ld_d_##reg() { m_D = m_##m; return 4; }
#define LD_E_REG(reg, m) int CPU::ld_e_##reg() { m_E = m_##m; return 4; }
#define LD_H_REG(reg, m) int CPU::ld_h_##reg() { m_H = m_##m; return 4; }
#define LD_L_REG(reg, m) int CPU::ld_l_##reg() { m_L = m_##m; return 4; }
#define LD_A_REG(reg, m) int CPU::ld_a_##reg() { m_A = m_##m; return 4; }
#define LD_MEM_HL_REG(reg, m) int CPU::ld_mem_hl_##reg() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); m_Memory.WriteByteToAddress(HL, m_##m); return 8; }

LD_B_REG(b, B)
LD_B_REG(c, C)
LD_B_REG(d, D)
LD_B_REG(e, E)
LD_B_REG(h, H)
LD_B_REG(l, L)
LD_B_REG(a, A)

LD_C_REG(b, B)
LD_C_REG(c, C)
LD_C_REG(d, D)
LD_C_REG(e, E)
LD_C_REG(h, H)
LD_C_REG(l, L)
LD_C_REG(a, A)

LD_D_REG(b, B)
LD_D_REG(c, C)
LD_D_REG(d, D)
LD_D_REG(e, E)
LD_D_REG(h, H)
LD_D_REG(l, L)
LD_D_REG(a, A)

LD_E_REG(b, B)
LD_E_REG(c, C)
LD_E_REG(d, D)
LD_E_REG(e, E)
LD_E_REG(h, H)
LD_E_REG(l, L)
LD_E_REG(a, A)

LD_H_REG(b, B)
LD_H_REG(c, C)
LD_H_REG(d, D)
LD_H_REG(e, E)
LD_H_REG(h, H)
LD_H_REG(l, L)
LD_H_REG(a, A)

LD_L_REG(b, B)
LD_L_REG(c, C)
LD_L_REG(d, D)
LD_L_REG(e, E)
LD_L_REG(h, H)
LD_L_REG(l, L)
LD_L_REG(a, A)

LD_A_REG(b, B)
LD_A_REG(c, C)
LD_A_REG(d, D)
LD_A_REG(e, E)
LD_A_REG(h, H)
LD_A_REG(l, L)
LD_A_REG(a, A)

LD_MEM_HL_REG(b, B)
LD_MEM_HL_REG(c, C)
LD_MEM_HL_REG(d, D)
LD_MEM_HL_REG(e, E)
LD_MEM_HL_REG(h, H)
LD_MEM_HL_REG(l, L)
LD_MEM_HL_REG(a, A)

int CPU::ld_b_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_B);
    return 8;
}

int CPU::ld_c_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_C);
    return 8;
}

int CPU::ld_d_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_D);
    return 8;
}

int CPU::ld_e_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_E);
    return 8;
}

int CPU::ld_h_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_H);
    return 8;
}

int CPU::ld_l_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_L);
    return 8;
}

int CPU::ld_a_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    m_Memory.Read1ByteFromMem(HL, m_A);
    return 8;
}

int CPU::halt()
{
    m_IsHalted = true;
    return 4;
}

// Still don't want to write all this
#define ADD_A_REG(reg, m) int CPU::add_a_##reg() { return AddByteToByte(m_##m, m_A); }
#define ADC_A_REG(reg, m) int CPU::adc_a_##reg() { return AddByteToByte(m_##m + (GetCarryFlag() ? 1 : 0), m_A); }
#define SUB_A_REG(reg, m) int CPU::sub_a_##reg() { return SubByteToByte(m_##m, m_A); }
#define SBC_A_REG(reg, m) int CPU::sbc_a_##reg() { return SubByteToByte(m_##m + (GetCarryFlag() ? 1 : 0), m_A); }

ADD_A_REG(b, B)
ADD_A_REG(c, C)
ADD_A_REG(d, D)
ADD_A_REG(e, E)
ADD_A_REG(h, H)
ADD_A_REG(l, L)
ADD_A_REG(a, A)

int CPU::add_a_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    AddByteToByte(value, m_A);

    return 8;
}

ADC_A_REG(b, B)
ADC_A_REG(c, C)
ADC_A_REG(d, D)
ADC_A_REG(e, E)
ADC_A_REG(h, H)
ADC_A_REG(l, L)
ADC_A_REG(a, A)

int CPU::adc_a_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    AddByteToByte(value + (GetCarryFlag() ? 1 : 0), m_A);

    return 8;
}

SUB_A_REG(b, B)
SUB_A_REG(c, C)
SUB_A_REG(d, D)
SUB_A_REG(e, E)
SUB_A_REG(h, H)
SUB_A_REG(l, L)
SUB_A_REG(a, A)

int CPU::sub_a_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    SubByteToByte(value, m_A);

    return 8;
}

SBC_A_REG(b, B)
SBC_A_REG(c, C)
SBC_A_REG(d, D)
SBC_A_REG(e, E)
SBC_A_REG(h, H)
SBC_A_REG(l, L)
SBC_A_REG(a, A)

int CPU::sbc_a_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    SubByteToByte(value + (GetCarryFlag() ? 1 : 0), m_A);

    return 8;
}

// Yup
#define AND_REG(reg, m) int CPU::and_##reg() { return AndReg(m_##m); }
#define XOR_REG(reg, m) int CPU::xor_##reg() { return XorReg(m_##m); }
#define OR_REG(reg, m) int CPU::or_##reg() { return OrReg(m_##m); }
#define CP_REG(reg, m) int CPU::cp_##reg() { return CompareReg(m_##m); }

AND_REG(b, B)
AND_REG(c, C)
AND_REG(d, D)
AND_REG(e, E)
AND_REG(h, H)
AND_REG(l, L)
AND_REG(a, A)

XOR_REG(b, B)
XOR_REG(c, C)
XOR_REG(d, D)
XOR_REG(e, E)
XOR_REG(h, H)
XOR_REG(l, L)
XOR_REG(a, A)

OR_REG(b, B)
OR_REG(c, C)
OR_REG(d, D)
OR_REG(e, E)
OR_REG(h, H)
OR_REG(l, L)
OR_REG(a, A)

CP_REG(b, B)
CP_REG(c, C)
CP_REG(d, D)
CP_REG(e, E)
CP_REG(h, H)
CP_REG(l, L)
CP_REG(a, A)

int CPU::and_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    return AndReg(value) + 4;
}

int CPU::xor_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    return XorReg(value) + 4;
}

int CPU::or_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    return OrReg(value) + 4;
}

int CPU::cp_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value = 0;
    m_Memory.Read1ByteFromMem(HL, value);

    return CompareReg(value) + 4;
}

int CPU::ret_nz()
{
    if (!GetZeroFlag())
    {
        unsigned char P, C;
        PopShort(P, C);
        m_PC = m_Memory.Convert2BytesToShort(P, C);
    }

    return 8;
}

int CPU::pop_bc()
{
    PopShort(m_B, m_C);
    return 12;
}

int CPU::jp_nz_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (!GetZeroFlag())
    {
        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::jp_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    m_PC = m_Memory.Convert2BytesToShort(msb, lsb);

    return 12;
}

int CPU::call_nz_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (!GetZeroFlag())
    {
        unsigned char P, C;
        m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
        PushShort(P, C);

        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::push_bc()
{
    PushShort(m_B, m_C);
    return 16;
}

int CPU::add_a_d8()
{
    unsigned char d8;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    AddByteToByte(d8, m_A);

    return 8;
}

int CPU::rst_00h()
{
    return Rst(0x00);
}

int CPU::ret_z()
{
    if (GetZeroFlag())
    {
        unsigned char P, C;
        PopShort(P, C);
        m_PC = m_Memory.Convert2BytesToShort(P, C);
    }

    return 8;
}

int CPU::ret()
{
    unsigned char P, C;
    PopShort(P, C);
    m_PC = m_Memory.Convert2BytesToShort(P, C);

    return 8;
}

int CPU::jp_z_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (GetZeroFlag())
    {
        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::call_z_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (GetZeroFlag())
    {
        unsigned char P, C;
        m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
        PushShort(P, C);

        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::call_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    unsigned char P, C;
    m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
    PushShort(P, C);

    m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    
    return 12;
}

int CPU::adc_a_d8()
{
    unsigned char d8;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    AddByteToByte(d8 + (GetCarryFlag() ? 1 : 0), m_A);

    return 8;
}

int CPU::rst_08h()
{
    return Rst(0x08);
}

int CPU::ret_nc()
{
    if (!GetCarryFlag())
    {
        unsigned char P, C;
        PopShort(P, C);
        m_PC = m_Memory.Convert2BytesToShort(P, C);
    }

    return 8;
}

int CPU::pop_de()
{
    return PopShort(m_D, m_E);
}

int CPU::jp_nc_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (!GetCarryFlag())
    {
        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::call_nc_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (!GetCarryFlag())
    {
        unsigned char P, C;
        m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
        PushShort(P, C);

        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::push_de()
{
    return PushShort(m_D, m_E);
}

int CPU::sub_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return SubByteToByte(d8, m_A) + 4;
}

int CPU::rst_10h()
{
    return Rst(0x10);
}

int CPU::ret_c()
{
    if (GetCarryFlag())
    {
        unsigned char P, C;
        PopShort(P, C);
        m_PC = m_Memory.Convert2BytesToShort(P, C);
    }

    return 8;
}

int CPU::reti()
{
    unsigned char P, C;
    PopShort(P, C);
    m_PC = m_Memory.Convert2BytesToShort(P, C);

    m_AreInterruptsEnabled = true;

    return 8;
}

int CPU::jp_c_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (GetCarryFlag())
    {
        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::call_c_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    if (GetCarryFlag())
    {
        unsigned char P, C;
        m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
        PushShort(P, C);

        m_PC = m_Memory.Convert2BytesToShort(msb, lsb);
    }

    return 12;
}

int CPU::sbc_a_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return SubByteToByte(d8 + (GetCarryFlag() ? 1 : 0), m_A) + 4;
}

int CPU::rst_18h()
{
    return Rst(0x18);
}

int CPU::ldh_mem_a8_a()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    m_Memory.WriteByteToAddress(0xFF00 + d8, m_A);

    return 12;
}

int CPU::pop_hl()
{
    return PopShort(m_H, m_L);
}

int CPU::ld_mem_c_a()
{
    m_Memory.WriteByteToAddress(0xFF00 + m_C, m_A);
    return 8;
}

int CPU::push_hl()
{
    return PushShort(m_H, m_L);
}

int CPU::and_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return AndReg(d8) + 4;
}

int CPU::rst_20h()
{
    return Rst(0x20);
}

int CPU::add_sp_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);

    unsigned short newValue = m_SP + (unsigned short)r8;

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag( IsCarryForBit(m_SP, r8, 0x1000) );
    SetHalfCarryFlag( IsCarryForBit(m_SP, r8, 0x8000) );

    m_SP = newValue;

    return 16;
}

int CPU::jp_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char P, C;
    m_Memory.Read1ByteFromMem(HL, C);
    P = 0;

    m_PC = m_Memory.Convert2BytesToShort(P, C);

    return 4;
}

int CPU::ld_mem_a16_a()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    unsigned short a16 = m_Memory.Convert2BytesToShort(msb, lsb);

    m_Memory.WriteByteToAddress(a16, m_A);

    return 16;
}

int CPU::xor_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return XorReg(d8) + 4;
}

int CPU::rst_28h()
{
    return Rst(0x28);
}

int CPU::ldh_a_mem_a8()
{
    unsigned char a8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, a8);
    m_PC += 1;

    m_Memory.Read1ByteFromMem(0xFF00 + a8, m_A);

    return 12;
}

int CPU::pop_af()
{
    return PopShort(m_A, m_F);
}

int CPU::ld_a_mem_c()
{
    m_Memory.Read1ByteFromMem(0xFF00 + m_C, m_A);
    return 8;
}

int CPU::di()
{
    m_ShouldInterruptsBeDisabled = true;
    return 4;
}

int CPU::push_af()
{
    return PushShort(m_A, m_F);
}

int CPU::or_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return OrReg(d8) + 4;
}

int CPU::rst_30h()
{
    return Rst(0x30);
}

int CPU::ld_hl_sp_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    m_Memory.ConvertShortTo2Bytes(m_SP + (unsigned char)r8, m_H, m_L);

    SetZeroFlag(false);
    SetSubtractFlag(false);

    return 12;    
}

int CPU::ld_sp_hl()
{
    m_SP = m_Memory.Convert2BytesToShort(m_H, m_L);
    return 8;
}

int CPU::ld_a_mem_a16()
{
    unsigned char msb, lsb;
    m_Memory.Read2BytesFromMem(m_PC, msb, lsb);
    m_PC += 2;

    unsigned short a16 = m_Memory.Convert2BytesToShort(msb, lsb);
    m_Memory.Read1ByteFromMem(a16, m_A);

    return 16;
}

int CPU::ei()
{
    m_ShouldInterruptsBeEnabled = true;
    return 4;
}

int CPU::cp_d8()
{
    unsigned char d8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, d8);
    m_PC += 1;

    return CompareReg(d8) + 4;
}

int CPU::rst_38h()
{
    return Rst(0x38);
}

// Again, no chance that I'm typying all that
#define RLC_REG(reg, m) int CPU::rlc_##reg() { return RotateByteLeftWithCarryFlag(m_##m) + 4; }
#define RRC_REG(reg, m) int CPU::rrc_##reg() { return RotateByteRightWithCarryFlag(m_##m) + 4; }
#define RL_REG(reg, m) int CPU::rl_##reg() { return RotateByteLeftThroughCarryFlag(m_##m) + 4; }
#define RR_REG(reg, m) int CPU::rr_##reg() { return RotateByteRightThroughCarryFlag(m_##m) + 4; }
#define SLA_REG(reg, m) int CPU::sla_##reg() { return ShiftByteLeftIntoCarry(m_##m); }
#define SRA_REG(reg, m) int CPU::sra_##reg() { return ShiftByteRightIntoCarryMsbNoChange(m_##m); }
#define SWAP_REG(reg, m) int CPU::swap_##reg() { return SwapByteNibbles(m_##m); }
#define SRL_REG(reg, m) int CPU::srl_##reg() { return ShiftByteRightIntoCarry(m_##m); }

RLC_REG(b, B)
RLC_REG(c, C)
RLC_REG(d, D)
RLC_REG(e, E)
RLC_REG(h, H)
RLC_REG(l, L)
RLC_REG(a, A)

RRC_REG(b, B)
RRC_REG(c, C)
RRC_REG(d, D)
RRC_REG(e, E)
RRC_REG(h, H)
RRC_REG(l, L)
RRC_REG(a, A)

RL_REG(b, B)
RL_REG(c, C)
RL_REG(d, D)
RL_REG(e, E)
RL_REG(h, H)
RL_REG(l, L)
RL_REG(a, A)

RR_REG(b, B)
RR_REG(c, C)
RR_REG(d, D)
RR_REG(e, E)
RR_REG(h, H)
RR_REG(l, L)
RR_REG(a, A)

SLA_REG(b, B)
SLA_REG(c, C)
SLA_REG(d, D)
SLA_REG(e, E)
SLA_REG(h, H)
SLA_REG(l, L)
SLA_REG(a, A)

SRA_REG(b, B)
SRA_REG(c, C)
SRA_REG(d, D)
SRA_REG(e, E)
SRA_REG(h, H)
SRA_REG(l, L)
SRA_REG(a, A)

SWAP_REG(b, B)
SWAP_REG(c, C)
SWAP_REG(d, D)
SWAP_REG(e, E)
SWAP_REG(h, H)
SWAP_REG(l, L)
SWAP_REG(a, A)

SRL_REG(b, B)
SRL_REG(c, C)
SRL_REG(d, D)
SRL_REG(e, E)
SRL_REG(h, H)
SRL_REG(l, L)
SRL_REG(a, A)

int CPU::rlc_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    RotateByteLeftWithCarryFlag(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::rrc_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    RotateByteRightWithCarryFlag(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::rl_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    RotateByteLeftThroughCarryFlag(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::rr_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    RotateByteRightThroughCarryFlag(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::sla_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    ShiftByteLeftIntoCarry(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::sra_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    ShiftByteRightIntoCarryMsbNoChange(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::swap_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    SwapByteNibbles(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

int CPU::srl_mem_hl()
{
    unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L);
    unsigned char value;
    m_Memory.Read1ByteFromMem(HL, value);

    ShiftByteRightIntoCarry(value);

    m_Memory.WriteByteToAddress(HL, value);

    return 16;
}

// Saves typying
#define BIT_0(reg, m) int CPU::bit_0_##reg() { return Bit(0, m_##m); }
#define BIT_1(reg, m) int CPU::bit_1_##reg() { return Bit(1, m_##m); }
#define BIT_2(reg, m) int CPU::bit_2_##reg() { return Bit(2, m_##m); }
#define BIT_3(reg, m) int CPU::bit_3_##reg() { return Bit(3, m_##m); }
#define BIT_4(reg, m) int CPU::bit_4_##reg() { return Bit(4, m_##m); }
#define BIT_5(reg, m) int CPU::bit_5_##reg() { return Bit(5, m_##m); }
#define BIT_6(reg, m) int CPU::bit_6_##reg() { return Bit(6, m_##m); }
#define BIT_7(reg, m) int CPU::bit_7_##reg() { return Bit(7, m_##m); }

#define RES_0(reg, m) int CPU::res_0_##reg() { return Res(0, m_##m); }
#define RES_1(reg, m) int CPU::res_1_##reg() { return Res(1, m_##m); }
#define RES_2(reg, m) int CPU::res_2_##reg() { return Res(2, m_##m); }
#define RES_3(reg, m) int CPU::res_3_##reg() { return Res(3, m_##m); }
#define RES_4(reg, m) int CPU::res_4_##reg() { return Res(4, m_##m); }
#define RES_5(reg, m) int CPU::res_5_##reg() { return Res(5, m_##m); }
#define RES_6(reg, m) int CPU::res_6_##reg() { return Res(6, m_##m); }
#define RES_7(reg, m) int CPU::res_7_##reg() { return Res(7, m_##m); }

#define SET_0(reg, m) int CPU::set_0_##reg() { return Set(0, m_##m); }
#define SET_1(reg, m) int CPU::set_1_##reg() { return Set(1, m_##m); }
#define SET_2(reg, m) int CPU::set_2_##reg() { return Set(2, m_##m); }
#define SET_3(reg, m) int CPU::set_3_##reg() { return Set(3, m_##m); }
#define SET_4(reg, m) int CPU::set_4_##reg() { return Set(4, m_##m); }
#define SET_5(reg, m) int CPU::set_5_##reg() { return Set(5, m_##m); }
#define SET_6(reg, m) int CPU::set_6_##reg() { return Set(6, m_##m); }
#define SET_7(reg, m) int CPU::set_7_##reg() { return Set(7, m_##m); }

BIT_0(b, B) 
BIT_0(c, C)
BIT_0(d, D)
BIT_0(e, E)
BIT_0(h, H)
BIT_0(l, L)
BIT_0(a, A)

BIT_1(b, B)
BIT_1(c, C)
BIT_1(d, D)
BIT_1(e, E)
BIT_1(h, H)
BIT_1(l, L)
BIT_1(a, A)

BIT_2(b, B)
BIT_2(c, C)
BIT_2(d, D)
BIT_2(e, E)
BIT_2(h, H)
BIT_2(l, L)
BIT_2(a, A)

BIT_3(b, B)
BIT_3(c, C)
BIT_3(d, D)
BIT_3(e, E)
BIT_3(h, H)
BIT_3(l, L)
BIT_3(a, A)

BIT_4(b, B)
BIT_4(c, C)
BIT_4(d, D)
BIT_4(e, E)
BIT_4(h, H)
BIT_4(l, L)
BIT_4(a, A)

BIT_5(b, B)
BIT_5(c, C)
BIT_5(d, D)
BIT_5(e, E)
BIT_5(h, H)
BIT_5(l, L)
BIT_5(a, A)

BIT_6(b, B)
BIT_6(c, C)
BIT_6(d, D)
BIT_6(e, E)
BIT_6(h, H)
BIT_6(l, L)
BIT_6(a, A)

BIT_7(b, B)
BIT_7(c, C)
BIT_7(d, D)
BIT_7(e, E)
BIT_7(h, H)
BIT_7(l, L)
BIT_7(a, A)

RES_0(b, B) 
RES_0(c, C)
RES_0(d, D)
RES_0(e, E)
RES_0(h, H)
RES_0(l, L)
RES_0(a, A)

RES_1(b, B)
RES_1(c, C)
RES_1(d, D)
RES_1(e, E)
RES_1(h, H)
RES_1(l, L)
RES_1(a, A)

RES_2(b, B)
RES_2(c, C)
RES_2(d, D)
RES_2(e, E)
RES_2(h, H)
RES_2(l, L)
RES_2(a, A)

RES_3(b, B)
RES_3(c, C)
RES_3(d, D)
RES_3(e, E)
RES_3(h, H)
RES_3(l, L)
RES_3(a, A)

RES_4(b, B)
RES_4(c, C)
RES_4(d, D)
RES_4(e, E)
RES_4(h, H)
RES_4(l, L)
RES_4(a, A)

RES_5(b, B)
RES_5(c, C)
RES_5(d, D)
RES_5(e, E)
RES_5(h, H)
RES_5(l, L)
RES_5(a, A)

RES_6(b, B)
RES_6(c, C)
RES_6(d, D)
RES_6(e, E)
RES_6(h, H)
RES_6(l, L)
RES_6(a, A)

RES_7(b, B)
RES_7(c, C)
RES_7(d, D)
RES_7(e, E)
RES_7(h, H)
RES_7(l, L)
RES_7(a, A)

SET_0(b, B) 
SET_0(c, C)
SET_0(d, D)
SET_0(e, E)
SET_0(h, H)
SET_0(l, L)
SET_0(a, A)

SET_1(b, B)
SET_1(c, C)
SET_1(d, D)
SET_1(e, E)
SET_1(h, H)
SET_1(l, L)
SET_1(a, A)

SET_2(b, B)
SET_2(c, C)
SET_2(d, D)
SET_2(e, E)
SET_2(h, H)
SET_2(l, L)
SET_2(a, A)

SET_3(b, B)
SET_3(c, C)
SET_3(d, D)
SET_3(e, E)
SET_3(h, H)
SET_3(l, L)
SET_3(a, A)

SET_4(b, B)
SET_4(c, C)
SET_4(d, D)
SET_4(e, E)
SET_4(h, H)
SET_4(l, L)
SET_4(a, A)

SET_5(b, B)
SET_5(c, C)
SET_5(d, D)
SET_5(e, E)
SET_5(h, H)
SET_5(l, L)
SET_5(a, A)

SET_6(b, B)
SET_6(c, C)
SET_6(d, D)
SET_6(e, E)
SET_6(h, H)
SET_6(l, L)
SET_6(a, A)

SET_7(b, B)
SET_7(c, C)
SET_7(d, D)
SET_7(e, E)
SET_7(h, H)
SET_7(l, L)
SET_7(a, A)

// Finally, the mem opcode variants for the last ones
int CPU::bit_0_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(0, value); return 16; }
int CPU::bit_1_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(1, value); return 16; }
int CPU::bit_2_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(2, value); return 16; }
int CPU::bit_3_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(3, value); return 16; }
int CPU::bit_4_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(4, value); return 16; }
int CPU::bit_5_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(5, value); return 16; }
int CPU::bit_6_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(6, value); return 16; }
int CPU::bit_7_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Bit(7, value); return 16; }

int CPU::res_0_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(0, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_1_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(1, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_2_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(2, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_3_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(3, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_4_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(4, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_5_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(5, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_6_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(6, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::res_7_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Res(7, value); m_Memory.WriteByteToAddress(HL, value); return 16; }

int CPU::set_0_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(0, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_1_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(1, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_2_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(2, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_3_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(3, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_4_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(4, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_5_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(5, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_6_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(6, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
int CPU::set_7_mem_hl() { unsigned short HL = m_Memory.Convert2BytesToShort(m_H, m_L); unsigned char value; m_Memory.Read1ByteFromMem(HL, value); Set(7, value); m_Memory.WriteByteToAddress(HL, value); return 16; }
}