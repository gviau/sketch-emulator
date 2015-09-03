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
    bool AreInterruptsEnabled() const { return m_AreInterruptsEnabled; }

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
    bool m_AreInterruptsEnabled;
    bool m_ShouldInterruptsBeDisabled;
    bool m_ShouldInterruptsBeEnabled;

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
    int AndReg(unsigned char reg);
    int XorReg(unsigned char reg);
    int OrReg(unsigned char reg);
    int CompareReg(unsigned char reg);

    int LoadD8InReg(unsigned char& reg);
    int LoadD16InReg(unsigned char& msb, unsigned char& lsb);

    int PushShort(unsigned char msb, unsigned char lsb);
    int PopShort(unsigned char& msb, unsigned char& lsb);
    int Rst(int n);

    int ShiftByteLeftIntoCarry(unsigned char& reg);
    int ShiftByteRightIntoCarryMsbNoChange(unsigned char& reg);
    int ShiftByteRightIntoCarry(unsigned char& reg);
    int SwapByteNibbles(unsigned char& reg);

    int Bit(unsigned char bit, unsigned char reg);
    int Res(unsigned char bit, unsigned char& reg);
    int Set(unsigned char bit, unsigned char& reg);

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

    // 0xA0 - 0xAF
    int and_b();
    int and_c();
    int and_d();
    int and_e();
    int and_h();
    int and_l();
    int and_mem_hl();
    int and_a();
    int xor_b();
    int xor_c();
    int xor_d();
    int xor_e();
    int xor_h();
    int xor_l();
    int xor_mem_hl();
    int xor_a();

    // 0xB0 - 0xBF
    int or_b();
    int or_c();
    int or_d();
    int or_e();
    int or_h();
    int or_l();
    int or_mem_hl();
    int or_a();
    int cp_b();
    int cp_c();
    int cp_d();
    int cp_e();
    int cp_h();
    int cp_l();
    int cp_mem_hl();
    int cp_a();

    // 0xC0 - 0xCF
    int ret_nz();
    int pop_bc();
    int jp_nz_a16();
    int jp_a16();
    int call_nz_a16();
    int push_bc();
    int add_a_d8();
    int rst_00h();
    int ret_z();
    int ret();
    int jp_z_a16();
    int call_z_a16();
    int call_a16();
    int adc_a_d8();
    int rst_08h();

    // 0xD0 - 0xDF
    int ret_nc();
    int pop_de();
    int jp_nc_a16();
    int call_nc_a16();
    int push_de();
    int sub_d8();
    int rst_10h();
    int ret_c();
    int reti();
    int jp_c_a16();
    int call_c_a16();
    int sbc_a_d8();
    int rst_18h();

    // 0xE0 - 0xEF
    int ldh_mem_a8_a();
    int pop_hl();
    int ld_mem_c_a();
    int push_hl();
    int and_d8();
    int rst_20h();
    int add_sp_r8();
    int jp_mem_hl();
    int ld_mem_a16_a();
    int xor_d8();
    int rst_28h();

    // 0xF0 - 0xFF
    int ldh_a_mem_a8();
    int pop_af();
    int ld_a_mem_c();
    int di();
    int push_af();
    int or_d8();
    int rst_30h();
    int ld_hl_sp_r8();
    int ld_sp_hl();
    int ld_a_mem_a16();
    int ei();
    int cp_d8();
    int rst_38h();

    // Prefix CB opcodes
    // 0x00 - 0x0F
    int rlc_b();
    int rlc_c();
    int rlc_d();
    int rlc_e();
    int rlc_h();
    int rlc_l();
    int rlc_mem_hl();
    int rlc_a();
    int rrc_b();
    int rrc_c();
    int rrc_d();
    int rrc_e();
    int rrc_h();
    int rrc_l();
    int rrc_mem_hl();
    int rrc_a();

    // 0x10 - 0x1F
    int rl_b();
    int rl_c();
    int rl_d();
    int rl_e();
    int rl_h();
    int rl_l();
    int rl_mem_hl();
    int rl_a();
    int rr_b();
    int rr_c();
    int rr_d();
    int rr_e();
    int rr_h();
    int rr_l();
    int rr_mem_hl();
    int rr_a();

    // 0x20 - 0x2F
    int sla_b();
    int sla_c();
    int sla_d();
    int sla_e();
    int sla_h();
    int sla_l();
    int sla_mem_hl();
    int sla_a();
    int sra_b();
    int sra_c();
    int sra_d();
    int sra_e();
    int sra_h();
    int sra_l();
    int sra_mem_hl();
    int sra_a();

    // 0x30 - 0x3F
    int swap_b();
    int swap_c();
    int swap_d();
    int swap_e();
    int swap_h();
    int swap_l();
    int swap_mem_hl();
    int swap_a();
    int srl_b();
    int srl_c();
    int srl_d();
    int srl_e();
    int srl_h();
    int srl_l();
    int srl_mem_hl();
    int srl_a();

    // 0x40 - 0x4F
    int bit_0_b();
    int bit_0_c();
    int bit_0_d();
    int bit_0_e();
    int bit_0_h();
    int bit_0_l();
    int bit_0_mem_hl();
    int bit_0_a();
    int bit_1_b();
    int bit_1_c();
    int bit_1_d();
    int bit_1_e();
    int bit_1_h();
    int bit_1_l();
    int bit_1_mem_hl();
    int bit_1_a();

    // 0x50 - 0x5F
    int bit_2_b();
    int bit_2_c();
    int bit_2_d();
    int bit_2_e();
    int bit_2_h();
    int bit_2_l();
    int bit_2_mem_hl();
    int bit_2_a();
    int bit_3_b();
    int bit_3_c();
    int bit_3_d();
    int bit_3_e();
    int bit_3_h();
    int bit_3_l();
    int bit_3_mem_hl();
    int bit_3_a();

    // 0x60 - 0x6F
    int bit_4_b();
    int bit_4_c();
    int bit_4_d();
    int bit_4_e();
    int bit_4_h();
    int bit_4_l();
    int bit_4_mem_hl();
    int bit_4_a();
    int bit_5_b();
    int bit_5_c();
    int bit_5_d();
    int bit_5_e();
    int bit_5_h();
    int bit_5_l();
    int bit_5_mem_hl();
    int bit_5_a();

    // 0x70 - 0x7F
    int bit_6_b();
    int bit_6_c();
    int bit_6_d();
    int bit_6_e();
    int bit_6_h();
    int bit_6_l();
    int bit_6_mem_hl();
    int bit_6_a();
    int bit_7_b();
    int bit_7_c();
    int bit_7_d();
    int bit_7_e();
    int bit_7_h();
    int bit_7_l();
    int bit_7_mem_hl();
    int bit_7_a();

    // 0x80 - 0x8F
    int res_0_b();
    int res_0_c();
    int res_0_d();
    int res_0_e();
    int res_0_h();
    int res_0_l();
    int res_0_mem_hl();
    int res_0_a();
    int res_1_b();
    int res_1_c();
    int res_1_d();
    int res_1_e();
    int res_1_h();
    int res_1_l();
    int res_1_mem_hl();
    int res_1_a();

    // 0x90 - 0x9F
    int res_2_b();
    int res_2_c();
    int res_2_d();
    int res_2_e();
    int res_2_h();
    int res_2_l();
    int res_2_mem_hl();
    int res_2_a();
    int res_3_b();
    int res_3_c();
    int res_3_d();
    int res_3_e();
    int res_3_h();
    int res_3_l();
    int res_3_mem_hl();
    int res_3_a();

    // 0xA0 - 0xAF
    int res_4_b();
    int res_4_c();
    int res_4_d();
    int res_4_e();
    int res_4_h();
    int res_4_l();
    int res_4_mem_hl();
    int res_4_a();
    int res_5_b();
    int res_5_c();
    int res_5_d();
    int res_5_e();
    int res_5_h();
    int res_5_l();
    int res_5_mem_hl();
    int res_5_a();

    // 0xB0 - 0xBF
    int res_6_b();
    int res_6_c();
    int res_6_d();
    int res_6_e();
    int res_6_h();
    int res_6_l();
    int res_6_mem_hl();
    int res_6_a();
    int res_7_b();
    int res_7_c();
    int res_7_d();
    int res_7_e();
    int res_7_h();
    int res_7_l();
    int res_7_mem_hl();
    int res_7_a();

    // 0xC0 - 0xCF
    int set_0_b();
    int set_0_c();
    int set_0_d();
    int set_0_e();
    int set_0_h();
    int set_0_l();
    int set_0_mem_hl();
    int set_0_a();
    int set_1_b();
    int set_1_c();
    int set_1_d();
    int set_1_e();
    int set_1_h();
    int set_1_l();
    int set_1_mem_hl();
    int set_1_a();

    // 0xD0 - 0xDF
    int set_2_b();
    int set_2_c();
    int set_2_d();
    int set_2_e();
    int set_2_h();
    int set_2_l();
    int set_2_mem_hl();
    int set_2_a();
    int set_3_b();
    int set_3_c();
    int set_3_d();
    int set_3_e();
    int set_3_h();
    int set_3_l();
    int set_3_mem_hl();
    int set_3_a();

    // 0xE0 - 0xEF
    int set_4_b();
    int set_4_c();
    int set_4_d();
    int set_4_e();
    int set_4_h();
    int set_4_l();
    int set_4_mem_hl();
    int set_4_a();
    int set_5_b();
    int set_5_c();
    int set_5_d();
    int set_5_e();
    int set_5_h();
    int set_5_l();
    int set_5_mem_hl();
    int set_5_a();

    // 0xF0 - 0xFF
    int set_6_b();
    int set_6_c();
    int set_6_d();
    int set_6_e();
    int set_6_h();
    int set_6_l();
    int set_6_mem_hl();
    int set_6_a();
    int set_7_b();
    int set_7_c();
    int set_7_d();
    int set_7_e();
    int set_7_h();
    int set_7_l();
    int set_7_mem_hl();
    int set_7_a();
};

}

#endif