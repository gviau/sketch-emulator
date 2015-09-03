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
    , m_IsHalted(false)
    , m_AreInterruptsEnabled(true)
    , m_ShouldInterruptsBeDisabled(false)
    , m_ShouldInterruptsBeEnabled(false)
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

    m_Opcodes[0x30] = &CPU::jr_nc_r8;
    m_Opcodes[0x31] = &CPU::ld_sp_d16;
    m_Opcodes[0x32] = &CPU::ld_hl_m_a;
    m_Opcodes[0x33] = &CPU::inc_sp;
    m_Opcodes[0x34] = &CPU::inc_mem_hl;
    m_Opcodes[0x35] = &CPU::dec_mem_hl;
    m_Opcodes[0x36] = &CPU::ld_mem_hl_d8;
    m_Opcodes[0x37] = &CPU::scf;
    m_Opcodes[0x38] = &CPU::jr_c_r8;
    m_Opcodes[0x39] = &CPU::add_hl_sp;
    m_Opcodes[0x3A] = &CPU::ld_a_hl_m;
    m_Opcodes[0x3B] = &CPU::dec_sp;
    m_Opcodes[0x3C] = &CPU::inc_a;
    m_Opcodes[0x3D] = &CPU::dec_a;
    m_Opcodes[0x3E] = &CPU::ld_a_d8;
    m_Opcodes[0x3F] = &CPU::ccf;

    m_Opcodes[0x40] = &CPU::ld_b_b;
    m_Opcodes[0x41] = &CPU::ld_b_c;
    m_Opcodes[0x42] = &CPU::ld_b_d;
    m_Opcodes[0x43] = &CPU::ld_b_e;
    m_Opcodes[0x44] = &CPU::ld_b_h;
    m_Opcodes[0x45] = &CPU::ld_b_l;
    m_Opcodes[0x46] = &CPU::ld_b_mem_hl;
    m_Opcodes[0x47] = &CPU::ld_b_a;
    m_Opcodes[0x48] = &CPU::ld_c_b;
    m_Opcodes[0x49] = &CPU::ld_c_c;
    m_Opcodes[0x4A] = &CPU::ld_c_d;
    m_Opcodes[0x4B] = &CPU::ld_c_e;
    m_Opcodes[0x4C] = &CPU::ld_c_h;
    m_Opcodes[0x4D] = &CPU::ld_c_l;
    m_Opcodes[0x4E] = &CPU::ld_c_mem_hl;
    m_Opcodes[0x4F] = &CPU::ld_c_a;

    m_Opcodes[0x50] = &CPU::ld_d_b;
    m_Opcodes[0x51] = &CPU::ld_d_c;
    m_Opcodes[0x52] = &CPU::ld_d_d;
    m_Opcodes[0x53] = &CPU::ld_d_e;
    m_Opcodes[0x54] = &CPU::ld_d_h;
    m_Opcodes[0x55] = &CPU::ld_d_l;
    m_Opcodes[0x56] = &CPU::ld_d_mem_hl;
    m_Opcodes[0x57] = &CPU::ld_d_a;
    m_Opcodes[0x58] = &CPU::ld_e_b;
    m_Opcodes[0x59] = &CPU::ld_e_c;
    m_Opcodes[0x5A] = &CPU::ld_e_d;
    m_Opcodes[0x5B] = &CPU::ld_e_e;
    m_Opcodes[0x5C] = &CPU::ld_e_h;
    m_Opcodes[0x5D] = &CPU::ld_e_l;
    m_Opcodes[0x5E] = &CPU::ld_e_mem_hl;
    m_Opcodes[0x5F] = &CPU::ld_e_a;

    m_Opcodes[0x60] = &CPU::ld_h_b;
    m_Opcodes[0x61] = &CPU::ld_h_c;
    m_Opcodes[0x62] = &CPU::ld_h_d;
    m_Opcodes[0x63] = &CPU::ld_h_e;
    m_Opcodes[0x64] = &CPU::ld_h_h;
    m_Opcodes[0x65] = &CPU::ld_h_l;
    m_Opcodes[0x66] = &CPU::ld_h_mem_hl;
    m_Opcodes[0x67] = &CPU::ld_h_a;
    m_Opcodes[0x68] = &CPU::ld_l_b;
    m_Opcodes[0x69] = &CPU::ld_l_c;
    m_Opcodes[0x6A] = &CPU::ld_l_d;
    m_Opcodes[0x6B] = &CPU::ld_l_e;
    m_Opcodes[0x6C] = &CPU::ld_l_h;
    m_Opcodes[0x6D] = &CPU::ld_l_l;
    m_Opcodes[0x6E] = &CPU::ld_l_mem_hl;
    m_Opcodes[0x6F] = &CPU::ld_l_a;

    m_Opcodes[0x70] = &CPU::ld_mem_hl_b;
    m_Opcodes[0x71] = &CPU::ld_mem_hl_c;
    m_Opcodes[0x72] = &CPU::ld_mem_hl_d;
    m_Opcodes[0x73] = &CPU::ld_mem_hl_e;
    m_Opcodes[0x74] = &CPU::ld_mem_hl_h;
    m_Opcodes[0x75] = &CPU::ld_mem_hl_l;
    m_Opcodes[0x76] = &CPU::halt;
    m_Opcodes[0x77] = &CPU::ld_mem_hl_a;
    m_Opcodes[0x78] = &CPU::ld_a_b;
    m_Opcodes[0x79] = &CPU::ld_a_c;
    m_Opcodes[0x7A] = &CPU::ld_a_d;
    m_Opcodes[0x7B] = &CPU::ld_a_e;
    m_Opcodes[0x7C] = &CPU::ld_a_h;
    m_Opcodes[0x7D] = &CPU::ld_a_l;
    m_Opcodes[0x7E] = &CPU::ld_a_mem_hl;
    m_Opcodes[0x7F] = &CPU::ld_a_a;

    m_Opcodes[0x80] = &CPU::add_a_b;
    m_Opcodes[0x81] = &CPU::add_a_c;
    m_Opcodes[0x82] = &CPU::add_a_d;
    m_Opcodes[0x83] = &CPU::add_a_e;
    m_Opcodes[0x84] = &CPU::add_a_h;
    m_Opcodes[0x85] = &CPU::add_a_l;
    m_Opcodes[0x86] = &CPU::add_a_mem_hl;
    m_Opcodes[0x87] = &CPU::add_a_a;
    m_Opcodes[0x88] = &CPU::adc_a_b;
    m_Opcodes[0x89] = &CPU::adc_a_c;
    m_Opcodes[0x8A] = &CPU::adc_a_d;
    m_Opcodes[0x8B] = &CPU::adc_a_e;
    m_Opcodes[0x8C] = &CPU::adc_a_h;
    m_Opcodes[0x8D] = &CPU::adc_a_l;
    m_Opcodes[0x8E] = &CPU::adc_a_mem_hl;
    m_Opcodes[0x8F] = &CPU::adc_a_a;

    m_Opcodes[0x90] = &CPU::sub_a_b;
    m_Opcodes[0x91] = &CPU::sub_a_c;
    m_Opcodes[0x92] = &CPU::sub_a_d;
    m_Opcodes[0x93] = &CPU::sub_a_e;
    m_Opcodes[0x94] = &CPU::sub_a_h;
    m_Opcodes[0x95] = &CPU::sub_a_l;
    m_Opcodes[0x96] = &CPU::sub_a_mem_hl;
    m_Opcodes[0x97] = &CPU::sub_a_a;
    m_Opcodes[0x98] = &CPU::sbc_a_b;
    m_Opcodes[0x99] = &CPU::sbc_a_c;
    m_Opcodes[0x9A] = &CPU::sbc_a_d;
    m_Opcodes[0x9B] = &CPU::sbc_a_e;
    m_Opcodes[0x9C] = &CPU::sbc_a_h;
    m_Opcodes[0x9D] = &CPU::sbc_a_l;
    m_Opcodes[0x9E] = &CPU::sbc_a_mem_hl;
    m_Opcodes[0x9F] = &CPU::sbc_a_a;

    m_Opcodes[0xA0] = &CPU::and_b;
    m_Opcodes[0xA1] = &CPU::and_c;
    m_Opcodes[0xA2] = &CPU::and_d;
    m_Opcodes[0xA3] = &CPU::and_e;
    m_Opcodes[0xA4] = &CPU::and_h;
    m_Opcodes[0xA5] = &CPU::and_l;
    m_Opcodes[0xA6] = &CPU::and_mem_hl;
    m_Opcodes[0xA7] = &CPU::and_a;
    m_Opcodes[0xA8] = &CPU::xor_b;
    m_Opcodes[0xA9] = &CPU::xor_c;
    m_Opcodes[0xAA] = &CPU::xor_d;
    m_Opcodes[0xAB] = &CPU::xor_e;
    m_Opcodes[0xAC] = &CPU::xor_h;
    m_Opcodes[0xAD] = &CPU::xor_l;
    m_Opcodes[0xAE] = &CPU::xor_mem_hl;
    m_Opcodes[0xAF] = &CPU::xor_a;

    m_Opcodes[0xB0] = &CPU::or_b;
    m_Opcodes[0xB1] = &CPU::or_c;
    m_Opcodes[0xB2] = &CPU::or_d;
    m_Opcodes[0xB3] = &CPU::or_e;
    m_Opcodes[0xB4] = &CPU::or_h;
    m_Opcodes[0xB5] = &CPU::or_l;
    m_Opcodes[0xB6] = &CPU::or_mem_hl;
    m_Opcodes[0xB7] = &CPU::or_a;
    m_Opcodes[0xB8] = &CPU::cp_b;
    m_Opcodes[0xB9] = &CPU::cp_c;
    m_Opcodes[0xBA] = &CPU::cp_d;
    m_Opcodes[0xBB] = &CPU::cp_e;
    m_Opcodes[0xBC] = &CPU::cp_h;
    m_Opcodes[0xBD] = &CPU::cp_l;
    m_Opcodes[0xBE] = &CPU::cp_mem_hl;
    m_Opcodes[0xBF] = &CPU::cp_a;

    m_Opcodes[0xC0] = &CPU::ret_nz;
    m_Opcodes[0xC1] = &CPU::pop_bc;
    m_Opcodes[0xC2] = &CPU::jp_nz_a16;
    m_Opcodes[0xC3] = &CPU::jp_a16;
    m_Opcodes[0xC4] = &CPU::call_nz_a16;
    m_Opcodes[0xC5] = &CPU::push_bc;
    m_Opcodes[0xC6] = &CPU::add_a_d8;
    m_Opcodes[0xC7] = &CPU::rst_00h;
    m_Opcodes[0xC8] = &CPU::ret_z;
    m_Opcodes[0xC9] = &CPU::ret;
    m_Opcodes[0xCA] = &CPU::jp_z_a16;
    m_Opcodes[0xCC] = &CPU::call_z_a16;
    m_Opcodes[0xCD] = &CPU::call_a16;
    m_Opcodes[0xCE] = &CPU::adc_a_d8;
    m_Opcodes[0xCF] = &CPU::rst_08h;

    m_Opcodes[0xD0] = &CPU::ret_nc;
    m_Opcodes[0xD1] = &CPU::pop_de;
    m_Opcodes[0xD2] = &CPU::jp_nc_a16;
    m_Opcodes[0xD4] = &CPU::call_nc_a16;
    m_Opcodes[0xD5] = &CPU::push_de;
    m_Opcodes[0xD6] = &CPU::sub_d8;
    m_Opcodes[0xD7] = &CPU::rst_10h;
    m_Opcodes[0xD8] = &CPU::ret_c;
    m_Opcodes[0xD9] = &CPU::reti;
    m_Opcodes[0xDA] = &CPU::jp_c_a16;
    m_Opcodes[0xDC] = &CPU::call_c_a16;
    m_Opcodes[0xDE] = &CPU::sbc_a_d8;
    m_Opcodes[0xDF] = &CPU::rst_18h;

    m_Opcodes[0xE0] = &CPU::ldh_mem_a8_a;
    m_Opcodes[0xE1] = &CPU::pop_hl;
    m_Opcodes[0xE2] = &CPU::ld_mem_c_a;
    m_Opcodes[0xE5] = &CPU::push_hl;
    m_Opcodes[0xE6] = &CPU::and_d8;
    m_Opcodes[0xE7] = &CPU::rst_20h;
    m_Opcodes[0xE8] = &CPU::add_sp_r8;
    m_Opcodes[0xE9] = &CPU::jp_mem_hl;
    m_Opcodes[0xEA] = &CPU::ld_mem_a16_a;
    m_Opcodes[0xEE] = &CPU::xor_d8;
    m_Opcodes[0xEF] = &CPU::rst_28h;

    m_Opcodes[0xF0] = &CPU::ldh_a_mem_a8;
    m_Opcodes[0xF1] = &CPU::pop_af;
    m_Opcodes[0xF2] = &CPU::ld_a_mem_c;
    m_Opcodes[0xF3] = &CPU::di;
    m_Opcodes[0xF5] = &CPU::push_af;
    m_Opcodes[0xF6] = &CPU::or_d8;
    m_Opcodes[0xF7] = &CPU::rst_30h;
    m_Opcodes[0xF8] = &CPU::ld_hl_sp_r8;
    m_Opcodes[0xF9] = &CPU::ld_sp_hl;
    m_Opcodes[0xFA] = &CPU::ld_a_mem_a16;
    m_Opcodes[0xFB] = &CPU::ei;
    m_Opcodes[0xFE] = &CPU::cp_d8;
    m_Opcodes[0xFF] = &CPU::rst_38h;
}

void CPU::Initialize()
{
    m_Memory.Initialize();
}

int CPU::Step()
{
    bool disableInterrupts = false;
    if (m_ShouldInterruptsBeDisabled)
    {
        disableInterrupts = true;
    }

    bool enableInterrupts = false;
    if (m_ShouldInterruptsBeEnabled)
    {
        enableInterrupts = true;
    }

    unsigned char opcode = 0;
    m_Memory.Read1ByteFromMem(m_PC, opcode);
    m_PC += 1;

    int cycles = 0;
    if (opcode == 0xCB)
    {
        m_Memory.Read1ByteFromMem(m_PC, opcode);
        m_PC += 1;

        cycles = CALL_MEMBER_FUNCTION(*this, m_OpcodesPrefixCb[opcode])();
    }
    else
    {
        cycles = CALL_MEMBER_FUNCTION(*this, m_Opcodes[opcode])();
    }

    if (disableInterrupts)
    {
        m_AreInterruptsEnabled = false;
        m_ShouldInterruptsBeDisabled = false;
    }

    if (enableInterrupts)
    {
        m_AreInterruptsEnabled = true;
        m_ShouldInterruptsBeEnabled = false;
    }

    return cycles;
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

int CPU::jr_nc_r8()
{
    unsigned char r8 = 0;
    m_Memory.Read1ByteFromMem(m_PC, r8);
    m_PC += 1;

    if (!GetCarryFlag())
    {
        m_PC += r8;
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
        m_PC += r8;
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

int CPU::IncShortRegister(unsigned char& msb, unsigned char& lsb)
{
    unsigned short reg = m_Memory.Convert2BytesToShort(lsb, msb);
    reg += 1;
    m_Memory.ConvertShortTo2Bytes(reg, lsb, msb);
    return 8;
}

int CPU::DecShortRegister(unsigned char& msb, unsigned char& lsb)
{
    unsigned short reg = m_Memory.Convert2BytesToShort(lsb, msb);
    reg -= 1;
    m_Memory.ConvertShortTo2Bytes(reg, lsb, msb);
    return 8;
}

int CPU::AddByteToByte(unsigned char regToAdd, unsigned char& regResult)
{
    unsigned char newValue = regToAdd + regResult;

    SetZeroFlag(newValue == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag( IsCarryForBit(regResult, newValue - regResult, 0x10) );
    SetCarryFlag( IsCarryForBit(regResult, newValue - regResult, 0x80) );

    regResult = newValue;

    return 4;
}

int CPU::AddShortToShort(unsigned char msbToAdd, unsigned char lsbToAdd, unsigned char& msbResult, unsigned char& lsbResult)
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

int CPU::SubByteToByte(unsigned char regToSub, unsigned char& regResult)
{
    unsigned char newValue = regResult - regToSub;

    SetZeroFlag(newValue == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag( IsCarryForBit(regResult, newValue - regResult, 0x10) );
    SetCarryFlag( IsCarryForBit(regResult, newValue - regResult, 0x80) );

    regResult = newValue;

    return 4;
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

int CPU::AndReg(unsigned char reg)
{
    m_A &= reg;

    SetZeroFlag(m_A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);
    SetCarryFlag(false);

    return 4;
}

int CPU::XorReg(unsigned char reg)
{
    m_A ^= reg;

    SetZeroFlag(m_A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);

    return 4;
}

int CPU::OrReg(unsigned char reg)
{
    m_A |= reg;

    SetZeroFlag(m_A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);

    return 4;
}

int CPU::CompareReg(unsigned char reg)
{
    SetZeroFlag(m_A == reg);
    SetSubtractFlag(true);
    SetHalfCarryFlag(false);
    SetCarryFlag(m_A < reg);

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

int CPU::LoadD16InReg(unsigned char& msb, unsigned char& lsb)
{
    m_Memory.Read2BytesFromMem(m_PC, lsb, msb);
    m_PC += 2;
    return 12;
}

int CPU::PushShort(unsigned char msb, unsigned char lsb)
{
    m_SP -= 1;
    m_Memory.WriteByteToAddress(m_SP, msb);
    m_SP -= 1;
    m_Memory.WriteByteToAddress(m_SP, lsb);

    return 16;
}

int CPU::PopShort(unsigned char& msb, unsigned char& lsb)
{
    m_Memory.Read1ByteFromMem(m_SP, lsb);
    m_SP += 1;
    m_Memory.Read1ByteFromMem(m_SP, msb);
    m_SP += 1;

    return 12;
}

int CPU::Rst(int n)
{
    unsigned char P, C;
    m_Memory.ConvertShortTo2Bytes(m_PC, P, C);
    PushShort(P, C);

    m_PC = n;

    return 32;
}

}