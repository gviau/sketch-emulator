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
    bool IsHalted() const { return m_IsHalted; }

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
    bool m_IsHalted;

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
    int IncShortRegister(unsigned char& msb, unsigned char& lsb);
    int DecShortRegister(unsigned char& msb, unsigned char& lsb);
    int AddByteToByte(unsigned char regToAdd, unsigned char& regResult);
    int AddShortToShort(unsigned char msbToAdd, unsigned char lsbToAdd, unsigned char& msbResult, unsigned char& lsbResult);
    int SubByteToByte(unsigned char regToSub, unsigned char& regResult);
    int RotateByteLeftWithCarryFlag(unsigned char& reg);
    int RotateByteRightWithCarryFlag(unsigned char& reg);
    int RotateByteLeftThroughCarryFlag(unsigned char& reg);
    int RotateByteRightThroughCarryFlag(unsigned char& msb);

    int LoadD8InReg(unsigned char& reg);
    int LoadD16InReg(unsigned char& msb, unsigned char& lsb);

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

    // 0x30 - 0x3F
    int jr_nc_r8();
    int ld_sp_d16();
    int ld_hl_m_a();
    int inc_sp();
    int inc_mem_hl();
    int dec_mem_hl();
    int ld_mem_hl_d8();
    int scf();
    int jr_c_r8();
    int add_hl_sp();
    int ld_a_hl_m();
    int dec_sp();
    int inc_a();
    int dec_a();
    int ld_a_d8();
    int ccf();

    // 0x40 - 0x4F
    int ld_b_b();
    int ld_b_c();
    int ld_b_d();
    int ld_b_e();
    int ld_b_h();
    int ld_b_l();
    int ld_b_mem_hl();
    int ld_b_a();
    int ld_c_b();
    int ld_c_c();
    int ld_c_d();
    int ld_c_e();
    int ld_c_h();
    int ld_c_l();
    int ld_c_mem_hl();
    int ld_c_a();

    // 0x50 - 0x5F
    int ld_d_b();
    int ld_d_c();
    int ld_d_d();
    int ld_d_e();
    int ld_d_h();
    int ld_d_l();
    int ld_d_mem_hl();
    int ld_d_a();
    int ld_e_b();
    int ld_e_c();
    int ld_e_d();
    int ld_e_e();
    int ld_e_h();
    int ld_e_l();
    int ld_e_mem_hl();
    int ld_e_a();

    // 0x60 - 0x6F
    int ld_h_b();
    int ld_h_c();
    int ld_h_d();
    int ld_h_e();
    int ld_h_h();
    int ld_h_l();
    int ld_h_mem_hl();
    int ld_h_a();
    int ld_l_b();
    int ld_l_c();
    int ld_l_d();
    int ld_l_e();
    int ld_l_h();
    int ld_l_l();
    int ld_l_mem_hl();
    int ld_l_a();

    // 0x70 - 7F
    int ld_mem_hl_b();
    int ld_mem_hl_c();
    int ld_mem_hl_d();
    int ld_mem_hl_e();
    int ld_mem_hl_h();
    int ld_mem_hl_l();
    int halt();
    int ld_mem_hl_a();
    int ld_a_b();
    int ld_a_c();
    int ld_a_d();
    int ld_a_e();
    int ld_a_h();
    int ld_a_l();
    int ld_a_mem_hl();
    int ld_a_a();

    // 0x80 - 0x8F
    int add_a_b();
    int add_a_c();
    int add_a_d();
    int add_a_e();
    int add_a_h();
    int add_a_l();
    int add_a_mem_hl();
    int add_a_a();
    int adc_a_b();
    int adc_a_c();
    int adc_a_d();
    int adc_a_e();
    int adc_a_h();
    int adc_a_l();
    int adc_a_mem_hl();
    int adc_a_a();

    // 0x90 - 0x9F
    int sub_a_b();
    int sub_a_c();
    int sub_a_d();
    int sub_a_e();
    int sub_a_h();
    int sub_a_l();
    int sub_a_mem_hl();
    int sub_a_a();
    int sbc_a_b();
    int sbc_a_c();
    int sbc_a_d();
    int sbc_a_e();
    int sbc_a_h();
    int sbc_a_l();
    int sbc_a_mem_hl();
    int sbc_a_a();
};

}

#endif