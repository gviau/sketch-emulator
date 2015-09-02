#ifndef SKETCH_EMULATOR_CPU_H
#define SKETCH_EMULATOR_CPU_H

#include "Memory.h"

namespace SketchEmulator {

class CPU
{
public:
    CPU();

    void Initialize();
    int Step();

    bool IsCPUStopped() const { return m_IsCPUStopped; }

private:
    typedef int (CPU::*Instruction) ();

    Memory m_Memory;

    // Registers
    unsigned char m_A;
    unsigned char m_B;
    unsigned char m_C;
    unsigned char m_D;
    unsigned char m_E;
    unsigned char m_F;
    unsigned char m_H;
    unsigned char m_L;
    unsigned short m_SP;
    unsigned short m_PC;

    bool m_IsCPUStopped;

    // Opcodes mapping
    Instruction m_Opcodes[256];
    Instruction m_OpcodesPrefixCb[256];

    // Utility functions
    void SetZeroFlag(bool value);
    void SetSubtractFlag(bool value);
    void SetHalfCarryFlag(bool value);
    void SetCarryFlag(bool value);

    bool GetZeroFlag() const;
    bool GetSubtractFlag() const;
    bool GetHalfCarryFlag() const;
    bool GetCarryFlag() const;

    bool IsCarryForBit(unsigned short oldValue, unsigned short increment, unsigned short bit) const;

    int IncByteRegister(unsigned char& reg);
    int DecByteRegister(unsigned char& reg);
    int IncShortRegister(unsigned char& lsb, unsigned char& msb);
    int DecShortRegister(unsigned char& lsb, unsigned char& msb);
    int AddShortToShort(unsigned char lsbToAdd, unsigned char msbToAdd, unsigned char& lsbResult, unsigned char& msbResult);
    int RotateByteLeftWithCarryFlag(unsigned char& reg);
    int RotateByteRightWithCarryFlag(unsigned char& reg);
    int RotateByteLeftThroughCarryFlag(unsigned char& reg);
    int RotateByteRightThroughCarryFlag(unsigned char& msb);

    int LoadD8InReg(unsigned char& reg);
    int LoadD16InReg(unsigned char& lsb, unsigned char& msb);

    // Instructions. Those comes directly from http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
    // 0x00 - 0x0F
    int nop();
    int ld_bc_d16();
    int ld_mem_bc_a();
    int inc_bc();
    int inc_b();
    int dec_b();
    int ld_b_d8();
    int rlca();
    int ld_mem_a16_sp();
    int add_hl_bc();
    int ld_a_mem_bc();
    int dec_bc();
    int inc_c();
    int dec_c();
    int ld_c_d8();
    int rrca();

    // 0x10 - 0x1F
    int stop();
    int ld_de_d16();
    int ld_mem_de_a();
    int inc_de();
    int inc_d();
    int dec_d();
    int ld_d_d8();
    int rla();
    int jr_r8();
    int add_hl_de();
    int ld_a_mem_de();
    int dec_de();
    int inc_e();
    int dec_e();
    int ld_e_d8();
    int rra();

    // 0x20 - 0x2F
    int jr_nz_r8();
    int ld_hl_d16();
    int ld_hl_p_a();
    int inc_hl();
    int inc_h();
    int dec_h();
    int ld_h_d8();
    int daa();
    int jr_z_r8();
    int add_hl_hl();
    int ld_a_hl_p();
    int dec_hl();
    int inc_l();
    int dec_l();
    int ld_l_d8();
    int cpl();
};

}

#endif