#include "CPU.h"

#include <iostream>
#include <string>
using namespace std;

namespace SketchEmulator {

// From https://isocpp.org/wiki/faq/pointers-to-members#macro-for-ptr-to-memfn
#define CALL_MEMBER_FUNCTION(object, ptrToMember) ((object).*(ptrToMember))

string opcodeNames[] = {
	"NOP",
	"LD  BC,(a16)",
	"LD  (BC),A",
	"INC  BC",
	"INC  B",
	"DEC  B",
	"LD  B,n",
	"RLCA",
	"LD  (a16),SP",
	"ADD  HL,BC",
	"LD  A,(BC)",
	"DEC  BC",
	"INC  C",
	"DEC  C",
	"LD  C,n",
	"RRCA",
	"STOP",
	"LD  DE,(a16)",
	"LD  (DE),A",
	"INC  DE",
	"INC  D",
	"DEC  D",
	"LD  D,n",
	"RLA",
	"JR  r8",
	"ADD  HL,DE",
	"LD  A,(DE)",
	"DEC  DE",
	"INC  E",
	"DEC  E",
	"LD E,n",
	"RRA",
	"JR  NZ,r8",
	"LD  HL,d16",
	"LD  (HL+),A",
	"INC  HL",
	"INC  H",
	"DEC  H",
	"LD H,n",
	"DAA",
	"JR  Z,r8",
	"ADD  HL,HL",
	"LD  A,(HL+)",
	"DEC  HL",
	"INC  L",
	"DEC  L",
	"LD L,n",
	"CPL",
	"JR  NC,r8",
	"LD SP, d16",
	"LD  (HL-),A",
	"INC  SP",
	"INC  (HL)",
	"DEC  (HL)",
	"LD  (HL),d8",
	"SCF",
	"JR  C,r8",
	"ADD  HL,SP",
	"LD  A,(HL-)",
	"DEC  SP",
	"INC  A",
	"DEC  A",
	"LD  A,d8",
	"CCF",
	"LD  B,B",
	"LD  B,C",
	"LD  B,D",
	"LD  B,E",
	"LD  B,H",
	"LD  B,L",
	"LD  B,(HL)",
	"LD  B,A",
	"LD  C,B",
	"LD  C,C",
	"LD  C,D",
	"LD  C,E",
	"LD  C,H",
	"LD  C,L",
	"LD  C,(HL)",
	"LD  C,A",
	"LD  D,B",
	"LD  D,C",
	"LD  D,D",
	"LD  D,E",
	"LD  D,H",
	"LD  D,L",
	"LD  D,(HL)",
	"LD  D,A",
	"LD  E,B",
	"LD  E,C",
	"LD  E,D",
	"LD  E,E",
	"LD  E,H",
	"LD  E,L",
	"LD  E,(HL)",
	"LD  E,A",
	"LD  H,B",
	"LD  H,C",
	"LD  H,D",
	"LD  H,E",
	"LD  H,H",
	"LD  H,L",
	"LD  H,(HL)",
	"LD  H,A",
	"LD  L,B",
	"LD  L,C",
	"LD  L,D",
	"LD  L,E",
	"LD  L,H",
	"LD  L,L",
	"LD  L,(HL)",
	"LD  L,A",
	"LD  (HL),B",
	"LD  (HL),C",
	"LD  (HL),D",
	"LD  (HL),E",
	"LD  (HL),H",
	"LD  (HL),L",
	"HALT",
	"LD  (HL),A",
	"LD  A,B",
	"LD  A,C",
	"LD  A,D",
	"LD  A,E",
	"LD  A,H",
	"LD  A,L",
	"LD  A,(HL)",
	"LD  A,A",
	"ADD  A,B",
	"ADD  A,C",
	"ADD  A,D",
	"ADD  A,E",
	"ADD  A,H",
	"ADD  A,L",
	"ADD  A,(HL)",
	"ADD  A,A",
	"ADC  A,B",
	"ADC  A,C",
	"ADC  A,D",
	"ADC  A,E",
	"ADC  A,H",
	"ADC  A,L",
	"ADC  A,(HL)",
	"ADC  A,A",
	"SUB  B",
	"SUB  C",
	"SUB  D",
	"SUB  E",
	"SUB  H",
	"SUB  L",
	"SUB  (HL)",
	"SUB  A",
	"SBC  A,B",
	"SBC  A,C",
	"SBC  A,D",
	"SBC  A,E",
	"SBC  A,H",
	"SBC  A,L",
	"SBC  A,(HL)",
	"SBC  A,A",
	"AND  B",
	"AND  C",
	"AND  D",
	"AND  E",
	"AND  H",
	"AND  L",
	"AND  (HL)",
	"AND  A",
	"XOR  B",
	"XOR  C",
	"XOR  D",
	"XOR  E",
	"XOR  H",
	"XOR  L",
	"XOR  (HL)",
	"XOR  A",
	"OR  B",
	"OR  C",
	"OR  D",
	"OR  E",
	"OR  H",
	"OR  L",
	"OR  (HL)",
	"OR  A",
	"CP  B",
	"CP  C",
	"CP  D",
	"CP  E",
	"CP  H",
	"CP  L",
	"CP  (HL)",
	"CP  A",
	"RET NZ",
	"POP  BC",
	"JP  NZ,a16",
	"JP  a16",
	"CALL  NZ,a16",
	"PUSH BC",
	"ADD  A,d8",
	"RST 0x00",
	"RET Z",
	"RET",
	"JP  Z,a16",
    "",
	"CALL  Z,a16",
	"CALL a16",
	"ADC  A,d8",
	"RST 0x08",
	"RET NC",
	"POP  DE",
	"JP  NC,a16",
    "",
	"CALL  NC,a16",
	"PUSH DE",
	"SUB  d8",
	"RST 0x10",
	"RET C",
	"RETI",
	"JP  C,a16",
    "",
	"CALL  C,a16",
    "",
	"SBC  A,d8",
	"RST  0x18",
	"LDH  (a8),A",
	"POP  HL",
	"LD  (C),A",
    "",
    "",
	"PUSH HL",
	"AND  d8",
	"RST 0x20",
	"ADD  SP,r8",
	"JP  (HL)",
	"LD  (a16),A",
    "",
    "",
    "",
	"XOR  d8",
	"RST 0x28",
	"LDH  A,(a8)",
	"POP  AF",
	"LD  A,(C)",
	"DI",
    "",
	"PUSH AF",
	"OR  d8",
	"RST 0x30",
	"LD  HL,SP+r8",
	"LD  SP,HL",
	"LD  A,(a16)",
	"EI",
    "",
    "",
	"CP  d8",
	"RST 0x38",
};

CPU::CPU(Memory& memory, InterruptsHandler& interruptsHandler)
    : m_Memory(memory)
    , m_InterruptsHandler(interruptsHandler)
    , m_A(0)
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

    m_OpcodesPrefixCb[0x00] = &CPU::rlc_b;
    m_OpcodesPrefixCb[0x01] = &CPU::rlc_c;
    m_OpcodesPrefixCb[0x02] = &CPU::rlc_d;
    m_OpcodesPrefixCb[0x03] = &CPU::rlc_e;
    m_OpcodesPrefixCb[0x04] = &CPU::rlc_h;
    m_OpcodesPrefixCb[0x05] = &CPU::rlc_l;
    m_OpcodesPrefixCb[0x06] = &CPU::rlc_mem_hl;
    m_OpcodesPrefixCb[0x07] = &CPU::rlc_a;
    m_OpcodesPrefixCb[0x08] = &CPU::rrc_b;
    m_OpcodesPrefixCb[0x09] = &CPU::rrc_c;
    m_OpcodesPrefixCb[0x0A] = &CPU::rrc_d;
    m_OpcodesPrefixCb[0x0B] = &CPU::rrc_e;
    m_OpcodesPrefixCb[0x0C] = &CPU::rrc_h;
    m_OpcodesPrefixCb[0x0D] = &CPU::rrc_l;
    m_OpcodesPrefixCb[0x0E] = &CPU::rrc_mem_hl;
    m_OpcodesPrefixCb[0x0F] = &CPU::rrc_a;

    m_OpcodesPrefixCb[0x10] = &CPU::rl_b;
    m_OpcodesPrefixCb[0x11] = &CPU::rl_c;
    m_OpcodesPrefixCb[0x12] = &CPU::rl_d;
    m_OpcodesPrefixCb[0x13] = &CPU::rl_e;
    m_OpcodesPrefixCb[0x14] = &CPU::rl_h;
    m_OpcodesPrefixCb[0x15] = &CPU::rl_l;
    m_OpcodesPrefixCb[0x16] = &CPU::rl_mem_hl;
    m_OpcodesPrefixCb[0x17] = &CPU::rl_a;
    m_OpcodesPrefixCb[0x18] = &CPU::rr_b;
    m_OpcodesPrefixCb[0x19] = &CPU::rr_c;
    m_OpcodesPrefixCb[0x1A] = &CPU::rr_d;
    m_OpcodesPrefixCb[0x1B] = &CPU::rr_e;
    m_OpcodesPrefixCb[0x1C] = &CPU::rr_h;
    m_OpcodesPrefixCb[0x1D] = &CPU::rr_l;
    m_OpcodesPrefixCb[0x1E] = &CPU::rr_mem_hl;
    m_OpcodesPrefixCb[0x1F] = &CPU::rr_a;

    m_OpcodesPrefixCb[0x20] = &CPU::sla_b;
    m_OpcodesPrefixCb[0x21] = &CPU::sla_c;
    m_OpcodesPrefixCb[0x22] = &CPU::sla_d;
    m_OpcodesPrefixCb[0x23] = &CPU::sla_e;
    m_OpcodesPrefixCb[0x24] = &CPU::sla_h;
    m_OpcodesPrefixCb[0x25] = &CPU::sla_l;
    m_OpcodesPrefixCb[0x26] = &CPU::sla_mem_hl;
    m_OpcodesPrefixCb[0x27] = &CPU::sla_a;
    m_OpcodesPrefixCb[0x28] = &CPU::sra_b;
    m_OpcodesPrefixCb[0x29] = &CPU::sra_c;
    m_OpcodesPrefixCb[0x2A] = &CPU::sra_d;
    m_OpcodesPrefixCb[0x2B] = &CPU::sra_e;
    m_OpcodesPrefixCb[0x2C] = &CPU::sra_h;
    m_OpcodesPrefixCb[0x2D] = &CPU::sra_l;
    m_OpcodesPrefixCb[0x2E] = &CPU::sra_mem_hl;
    m_OpcodesPrefixCb[0x2F] = &CPU::sra_a;

    m_OpcodesPrefixCb[0x30] = &CPU::swap_b;
    m_OpcodesPrefixCb[0x31] = &CPU::swap_c;
    m_OpcodesPrefixCb[0x32] = &CPU::swap_d;
    m_OpcodesPrefixCb[0x33] = &CPU::swap_e;
    m_OpcodesPrefixCb[0x34] = &CPU::swap_h;
    m_OpcodesPrefixCb[0x35] = &CPU::swap_l;
    m_OpcodesPrefixCb[0x36] = &CPU::swap_mem_hl;
    m_OpcodesPrefixCb[0x37] = &CPU::swap_a;
    m_OpcodesPrefixCb[0x38] = &CPU::srl_b;
    m_OpcodesPrefixCb[0x39] = &CPU::srl_c;
    m_OpcodesPrefixCb[0x3A] = &CPU::srl_d;
    m_OpcodesPrefixCb[0x3B] = &CPU::srl_e;
    m_OpcodesPrefixCb[0x3C] = &CPU::srl_h;
    m_OpcodesPrefixCb[0x3D] = &CPU::srl_l;
    m_OpcodesPrefixCb[0x3E] = &CPU::srl_mem_hl;
    m_OpcodesPrefixCb[0x3F] = &CPU::srl_a;

    m_OpcodesPrefixCb[0x40] = &CPU::bit_0_b;
    m_OpcodesPrefixCb[0x41] = &CPU::bit_0_c;
    m_OpcodesPrefixCb[0x42] = &CPU::bit_0_d;
    m_OpcodesPrefixCb[0x43] = &CPU::bit_0_e;
    m_OpcodesPrefixCb[0x44] = &CPU::bit_0_h;
    m_OpcodesPrefixCb[0x45] = &CPU::bit_0_l;
    m_OpcodesPrefixCb[0x46] = &CPU::bit_0_mem_hl;
    m_OpcodesPrefixCb[0x47] = &CPU::bit_0_a;
    m_OpcodesPrefixCb[0x48] = &CPU::bit_1_b;
    m_OpcodesPrefixCb[0x49] = &CPU::bit_1_c;
    m_OpcodesPrefixCb[0x4A] = &CPU::bit_1_d;
    m_OpcodesPrefixCb[0x4B] = &CPU::bit_1_e;
    m_OpcodesPrefixCb[0x4C] = &CPU::bit_1_h;
    m_OpcodesPrefixCb[0x4D] = &CPU::bit_1_l;
    m_OpcodesPrefixCb[0x4E] = &CPU::bit_1_mem_hl;
    m_OpcodesPrefixCb[0x4F] = &CPU::bit_1_a;

    m_OpcodesPrefixCb[0x50] = &CPU::bit_2_b;
    m_OpcodesPrefixCb[0x51] = &CPU::bit_2_c;
    m_OpcodesPrefixCb[0x52] = &CPU::bit_2_d;
    m_OpcodesPrefixCb[0x53] = &CPU::bit_2_e;
    m_OpcodesPrefixCb[0x54] = &CPU::bit_2_h;
    m_OpcodesPrefixCb[0x55] = &CPU::bit_2_l;
    m_OpcodesPrefixCb[0x56] = &CPU::bit_2_mem_hl;
    m_OpcodesPrefixCb[0x57] = &CPU::bit_2_a;
    m_OpcodesPrefixCb[0x58] = &CPU::bit_3_b;
    m_OpcodesPrefixCb[0x59] = &CPU::bit_3_c;
    m_OpcodesPrefixCb[0x5A] = &CPU::bit_3_d;
    m_OpcodesPrefixCb[0x5B] = &CPU::bit_3_e;
    m_OpcodesPrefixCb[0x5C] = &CPU::bit_3_h;
    m_OpcodesPrefixCb[0x5D] = &CPU::bit_3_l;
    m_OpcodesPrefixCb[0x5E] = &CPU::bit_3_mem_hl;
    m_OpcodesPrefixCb[0x5F] = &CPU::bit_3_a;

    m_OpcodesPrefixCb[0x60] = &CPU::bit_4_b;
    m_OpcodesPrefixCb[0x61] = &CPU::bit_4_c;
    m_OpcodesPrefixCb[0x62] = &CPU::bit_4_d;
    m_OpcodesPrefixCb[0x63] = &CPU::bit_4_e;
    m_OpcodesPrefixCb[0x64] = &CPU::bit_4_h;
    m_OpcodesPrefixCb[0x65] = &CPU::bit_4_l;
    m_OpcodesPrefixCb[0x66] = &CPU::bit_4_mem_hl;
    m_OpcodesPrefixCb[0x67] = &CPU::bit_4_a;
    m_OpcodesPrefixCb[0x68] = &CPU::bit_5_b;
    m_OpcodesPrefixCb[0x69] = &CPU::bit_5_c;
    m_OpcodesPrefixCb[0x6A] = &CPU::bit_5_d;
    m_OpcodesPrefixCb[0x6B] = &CPU::bit_5_e;
    m_OpcodesPrefixCb[0x6C] = &CPU::bit_5_h;
    m_OpcodesPrefixCb[0x6D] = &CPU::bit_5_l;
    m_OpcodesPrefixCb[0x6E] = &CPU::bit_5_mem_hl;
    m_OpcodesPrefixCb[0x6F] = &CPU::bit_5_a;

    m_OpcodesPrefixCb[0x70] = &CPU::bit_6_b;
    m_OpcodesPrefixCb[0x71] = &CPU::bit_6_c;
    m_OpcodesPrefixCb[0x72] = &CPU::bit_6_d;
    m_OpcodesPrefixCb[0x73] = &CPU::bit_6_e;
    m_OpcodesPrefixCb[0x74] = &CPU::bit_6_h;
    m_OpcodesPrefixCb[0x75] = &CPU::bit_6_l;
    m_OpcodesPrefixCb[0x76] = &CPU::bit_6_mem_hl;
    m_OpcodesPrefixCb[0x77] = &CPU::bit_6_a;
    m_OpcodesPrefixCb[0x78] = &CPU::bit_7_b;
    m_OpcodesPrefixCb[0x79] = &CPU::bit_7_c;
    m_OpcodesPrefixCb[0x7A] = &CPU::bit_7_d;
    m_OpcodesPrefixCb[0x7B] = &CPU::bit_7_e;
    m_OpcodesPrefixCb[0x7C] = &CPU::bit_7_h;
    m_OpcodesPrefixCb[0x7D] = &CPU::bit_7_l;
    m_OpcodesPrefixCb[0x7E] = &CPU::bit_7_mem_hl;
    m_OpcodesPrefixCb[0x7F] = &CPU::bit_7_a;

    m_OpcodesPrefixCb[0x80] = &CPU::res_0_b;
    m_OpcodesPrefixCb[0x81] = &CPU::res_0_c;
    m_OpcodesPrefixCb[0x82] = &CPU::res_0_d;
    m_OpcodesPrefixCb[0x83] = &CPU::res_0_e;
    m_OpcodesPrefixCb[0x84] = &CPU::res_0_h;
    m_OpcodesPrefixCb[0x85] = &CPU::res_0_l;
    m_OpcodesPrefixCb[0x86] = &CPU::res_0_mem_hl;
    m_OpcodesPrefixCb[0x87] = &CPU::res_0_a;
    m_OpcodesPrefixCb[0x88] = &CPU::res_1_b;
    m_OpcodesPrefixCb[0x89] = &CPU::res_1_c;
    m_OpcodesPrefixCb[0x8A] = &CPU::res_1_d;
    m_OpcodesPrefixCb[0x8B] = &CPU::res_1_e;
    m_OpcodesPrefixCb[0x8C] = &CPU::res_1_h;
    m_OpcodesPrefixCb[0x8D] = &CPU::res_1_l;
    m_OpcodesPrefixCb[0x8E] = &CPU::res_1_mem_hl;
    m_OpcodesPrefixCb[0x8F] = &CPU::res_1_a;

    m_OpcodesPrefixCb[0x90] = &CPU::res_2_b;
    m_OpcodesPrefixCb[0x91] = &CPU::res_2_c;
    m_OpcodesPrefixCb[0x92] = &CPU::res_2_d;
    m_OpcodesPrefixCb[0x93] = &CPU::res_2_e;
    m_OpcodesPrefixCb[0x94] = &CPU::res_2_h;
    m_OpcodesPrefixCb[0x95] = &CPU::res_2_l;
    m_OpcodesPrefixCb[0x96] = &CPU::res_2_mem_hl;
    m_OpcodesPrefixCb[0x97] = &CPU::res_2_a;
    m_OpcodesPrefixCb[0x98] = &CPU::res_3_b;
    m_OpcodesPrefixCb[0x99] = &CPU::res_3_c;
    m_OpcodesPrefixCb[0x9A] = &CPU::res_3_d;
    m_OpcodesPrefixCb[0x9B] = &CPU::res_3_e;
    m_OpcodesPrefixCb[0x9C] = &CPU::res_3_h;
    m_OpcodesPrefixCb[0x9D] = &CPU::res_3_l;
    m_OpcodesPrefixCb[0x9E] = &CPU::res_3_mem_hl;
    m_OpcodesPrefixCb[0x9F] = &CPU::res_3_a;

    m_OpcodesPrefixCb[0xA0] = &CPU::res_4_b;
    m_OpcodesPrefixCb[0xA1] = &CPU::res_4_c;
    m_OpcodesPrefixCb[0xA2] = &CPU::res_4_d;
    m_OpcodesPrefixCb[0xA3] = &CPU::res_4_e;
    m_OpcodesPrefixCb[0xA4] = &CPU::res_4_h;
    m_OpcodesPrefixCb[0xA5] = &CPU::res_4_l;
    m_OpcodesPrefixCb[0xA6] = &CPU::res_4_mem_hl;
    m_OpcodesPrefixCb[0xA7] = &CPU::res_4_a;
    m_OpcodesPrefixCb[0xA8] = &CPU::res_5_b;
    m_OpcodesPrefixCb[0xA9] = &CPU::res_5_c;
    m_OpcodesPrefixCb[0xAA] = &CPU::res_5_d;
    m_OpcodesPrefixCb[0xAB] = &CPU::res_5_e;
    m_OpcodesPrefixCb[0xAC] = &CPU::res_5_h;
    m_OpcodesPrefixCb[0xAD] = &CPU::res_5_l;
    m_OpcodesPrefixCb[0xAE] = &CPU::res_5_mem_hl;
    m_OpcodesPrefixCb[0xAF] = &CPU::res_5_a;

    m_OpcodesPrefixCb[0xB0] = &CPU::res_6_b;
    m_OpcodesPrefixCb[0xB1] = &CPU::res_6_c;
    m_OpcodesPrefixCb[0xB2] = &CPU::res_6_d;
    m_OpcodesPrefixCb[0xB3] = &CPU::res_6_e;
    m_OpcodesPrefixCb[0xB4] = &CPU::res_6_h;
    m_OpcodesPrefixCb[0xB5] = &CPU::res_6_l;
    m_OpcodesPrefixCb[0xB6] = &CPU::res_6_mem_hl;
    m_OpcodesPrefixCb[0xB7] = &CPU::res_6_a;
    m_OpcodesPrefixCb[0xB8] = &CPU::res_7_b;
    m_OpcodesPrefixCb[0xB9] = &CPU::res_7_c;
    m_OpcodesPrefixCb[0xBA] = &CPU::res_7_d;
    m_OpcodesPrefixCb[0xBB] = &CPU::res_7_e;
    m_OpcodesPrefixCb[0xBC] = &CPU::res_7_h;
    m_OpcodesPrefixCb[0xBD] = &CPU::res_7_l;
    m_OpcodesPrefixCb[0xBE] = &CPU::res_7_mem_hl;
    m_OpcodesPrefixCb[0xBF] = &CPU::res_7_a;

    m_OpcodesPrefixCb[0xC0] = &CPU::set_0_b;
    m_OpcodesPrefixCb[0xC1] = &CPU::set_0_c;
    m_OpcodesPrefixCb[0xC2] = &CPU::set_0_d;
    m_OpcodesPrefixCb[0xC3] = &CPU::set_0_e;
    m_OpcodesPrefixCb[0xC4] = &CPU::set_0_h;
    m_OpcodesPrefixCb[0xC5] = &CPU::set_0_l;
    m_OpcodesPrefixCb[0xC6] = &CPU::set_0_mem_hl;
    m_OpcodesPrefixCb[0xC7] = &CPU::set_0_a;
    m_OpcodesPrefixCb[0xC8] = &CPU::set_1_b;
    m_OpcodesPrefixCb[0xC9] = &CPU::set_1_c;
    m_OpcodesPrefixCb[0xCA] = &CPU::set_1_d;
    m_OpcodesPrefixCb[0xCB] = &CPU::set_1_e;
    m_OpcodesPrefixCb[0xCC] = &CPU::set_1_h;
    m_OpcodesPrefixCb[0xCD] = &CPU::set_1_l;
    m_OpcodesPrefixCb[0xCE] = &CPU::set_1_mem_hl;
    m_OpcodesPrefixCb[0xCF] = &CPU::set_1_a;

    m_OpcodesPrefixCb[0xD0] = &CPU::set_2_b;
    m_OpcodesPrefixCb[0xD1] = &CPU::set_2_c;
    m_OpcodesPrefixCb[0xD2] = &CPU::set_2_d;
    m_OpcodesPrefixCb[0xD3] = &CPU::set_2_e;
    m_OpcodesPrefixCb[0xD4] = &CPU::set_2_h;
    m_OpcodesPrefixCb[0xD5] = &CPU::set_2_l;
    m_OpcodesPrefixCb[0xD6] = &CPU::set_2_mem_hl;
    m_OpcodesPrefixCb[0xD7] = &CPU::set_2_a;
    m_OpcodesPrefixCb[0xD8] = &CPU::set_3_b;
    m_OpcodesPrefixCb[0xD9] = &CPU::set_3_c;
    m_OpcodesPrefixCb[0xDA] = &CPU::set_3_d;
    m_OpcodesPrefixCb[0xDB] = &CPU::set_3_e;
    m_OpcodesPrefixCb[0xDC] = &CPU::set_3_h;
    m_OpcodesPrefixCb[0xDD] = &CPU::set_3_l;
    m_OpcodesPrefixCb[0xDE] = &CPU::set_3_mem_hl;
    m_OpcodesPrefixCb[0xDF] = &CPU::set_3_a;

    m_OpcodesPrefixCb[0xE0] = &CPU::set_4_b;
    m_OpcodesPrefixCb[0xE1] = &CPU::set_4_c;
    m_OpcodesPrefixCb[0xE2] = &CPU::set_4_d;
    m_OpcodesPrefixCb[0xE3] = &CPU::set_4_e;
    m_OpcodesPrefixCb[0xE4] = &CPU::set_4_h;
    m_OpcodesPrefixCb[0xE5] = &CPU::set_4_l;
    m_OpcodesPrefixCb[0xE6] = &CPU::set_4_mem_hl;
    m_OpcodesPrefixCb[0xE7] = &CPU::set_4_a;
    m_OpcodesPrefixCb[0xE8] = &CPU::set_5_b;
    m_OpcodesPrefixCb[0xE9] = &CPU::set_5_c;
    m_OpcodesPrefixCb[0xEA] = &CPU::set_5_d;
    m_OpcodesPrefixCb[0xEB] = &CPU::set_5_e;
    m_OpcodesPrefixCb[0xEC] = &CPU::set_5_h;
    m_OpcodesPrefixCb[0xED] = &CPU::set_5_l;
    m_OpcodesPrefixCb[0xEE] = &CPU::set_5_mem_hl;
    m_OpcodesPrefixCb[0xEF] = &CPU::set_5_a;

    m_OpcodesPrefixCb[0xF0] = &CPU::set_6_b;
    m_OpcodesPrefixCb[0xF1] = &CPU::set_6_c;
    m_OpcodesPrefixCb[0xF2] = &CPU::set_6_d;
    m_OpcodesPrefixCb[0xF3] = &CPU::set_6_e;
    m_OpcodesPrefixCb[0xF4] = &CPU::set_6_h;
    m_OpcodesPrefixCb[0xF5] = &CPU::set_6_l;
    m_OpcodesPrefixCb[0xF6] = &CPU::set_6_mem_hl;
    m_OpcodesPrefixCb[0xF7] = &CPU::set_6_a;
    m_OpcodesPrefixCb[0xF8] = &CPU::set_7_b;
    m_OpcodesPrefixCb[0xF9] = &CPU::set_7_c;
    m_OpcodesPrefixCb[0xFA] = &CPU::set_7_d;
    m_OpcodesPrefixCb[0xFB] = &CPU::set_7_e;
    m_OpcodesPrefixCb[0xFC] = &CPU::set_7_h;
    m_OpcodesPrefixCb[0xFD] = &CPU::set_7_l;
    m_OpcodesPrefixCb[0xFE] = &CPU::set_7_mem_hl;
    m_OpcodesPrefixCb[0xFF] = &CPU::set_7_a;
}

void CPU::Initialize(const CartridgeInfo_t& cartridgeInfo)
{
    if (cartridgeInfo.m_GameboyColor)
    {
        m_A = 0x11;
    }
    else
    {
        m_A = 0x01;
    }

    m_F = 0xB0;
    m_C = 0x13;
    m_E = 0xD8;
    m_H = 0x01;
    m_L = 0x4D;
    m_SP = 0xFFFE;
    m_PC = 0x100;

    m_Memory.WriteByteToAddress(0xFF05, 0x00);
    m_Memory.WriteByteToAddress(0xFF06, 0x00);
    m_Memory.WriteByteToAddress(0xFF07, 0x00);
    m_Memory.WriteByteToAddress(0xFF10, 0x80);
    m_Memory.WriteByteToAddress(0xFF11, 0xBF);
    m_Memory.WriteByteToAddress(0xFF12, 0xF3);
    m_Memory.WriteByteToAddress(0xFF14, 0xBF);
    m_Memory.WriteByteToAddress(0xFF16, 0x3F);
    m_Memory.WriteByteToAddress(0xFF17, 0x00);
    m_Memory.WriteByteToAddress(0xFF19, 0xBF);
    m_Memory.WriteByteToAddress(0xFF1A, 0x7F);
    m_Memory.WriteByteToAddress(0xFF1B, 0xFF);
    m_Memory.WriteByteToAddress(0xFF1C, 0x9F);
    m_Memory.WriteByteToAddress(0xFF1E, 0xBF);
    m_Memory.WriteByteToAddress(0xFF20, 0xFF);
    m_Memory.WriteByteToAddress(0xFF21, 0x00);
    m_Memory.WriteByteToAddress(0xFF22, 0x00);
    m_Memory.WriteByteToAddress(0xFF23, 0xBF);
    m_Memory.WriteByteToAddress(0xFF24, 0x77);
    m_Memory.WriteByteToAddress(0xFF25, 0xF3);
    m_Memory.WriteByteToAddress(0xFF26, 0xF1);
    m_Memory.WriteByteToAddress(0xFF40, 0x91);
    m_Memory.WriteByteToAddress(0xFF42, 0x00);
    m_Memory.WriteByteToAddress(0xFF43, 0x00);
    m_Memory.WriteByteToAddress(0xFF45, 0x00);
    m_Memory.WriteByteToAddress(0xFF47, 0xFC);
    m_Memory.WriteByteToAddress(0xFF48, 0xFF);
    m_Memory.WriteByteToAddress(0xFF49, 0xFF);
    m_Memory.WriteByteToAddress(0xFF4A, 0x00);
    m_Memory.WriteByteToAddress(0xFF4B, 0x00);
    m_Memory.WriteByteToAddress(0xFFFF, 0x00);
}

int CPU::Step(bool debug)
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

    if (m_AreInterruptsEnabled)
    {
        InterruptType_t type = m_InterruptsHandler.GetInterruptToHandle();
        if (type != InterruptType_t::NO_INTERRUPTS)
        {
            m_InterruptsHandler.HandleInterrupt(type, this);
        }
    }

    if (debug)
    {
        unsigned char opcode = 0;
        m_Memory.Read1ByteFromMem(m_PC, opcode);
        if (opcode == 0xCB)
        {
            m_Memory.Read1ByteFromMem(m_PC, opcode);

            
        }
        else
        {
            cout << "[" << hex << m_PC << dec << "] " << opcodeNames[opcode] << endl;

            string line;
            do {
                cout << ">";
                getline(cin, line);
                if (line == "show regs")
                {
                    int zeroFlag = (GetZeroFlag() ? 1 : 0);
                    int subtractFlag = (GetSubtractFlag() ? 1 : 0);
                    int halfCarryFlag = (GetHalfCarryFlag() ? 1 : 0);
                    int carryFlag = (GetCarryFlag() ? 1 : 0);

                    cout << "A=" << hex << (int)m_A << "\tF=" << hex << (int)m_F << "\t( Z=" << zeroFlag << " S=" << subtractFlag << " H=" << halfCarryFlag << " C=" << carryFlag << " )" << endl;
                    cout << "B=" << hex << (int)m_B << "\tC=" << hex << (int)m_C << endl;
                    cout << "D=" << hex << (int)m_D << "\tE=" << hex << (int)m_E << endl;
                    cout << "H=" << hex << (int)m_H << "\tL=" << hex << (int)m_L << endl;
                    cout << "PC=" << hex << (int)m_PC <<endl;
                    cout << "SP=" << hex << (int)m_SP << endl;
                }
                else if (line.find("0x") != string::npos)
                {
                    unsigned short address = atoi(line.c_str());
                    unsigned char byte;
                    m_Memory.Read1ByteFromMem(address, byte);

                    cout << hex << (int)byte << endl;
                }
                else if (line != "c" && line != "")
                {
                    cout << "Couldn't understand this command!" << endl;
                }

            } while(line != "c" && line != "");
        }
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
    reg |= ((oldBit7 > 0) ? 1 : 0);

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
    reg |= ((oldBit0 > 0) ? 0x80 : 0);

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

int CPU::ShiftByteLeftIntoCarry(unsigned char& reg)
{
    unsigned char oldBit7 = (reg & 0x80);

    reg <<= 1;
    
    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit7 > 0);

    return 8;
}

int CPU::ShiftByteRightIntoCarryMsbNoChange(unsigned char& reg)
{
    unsigned char oldBit0 = (reg & 0x01);
    unsigned char oldBit7 = (reg & 0x80);

    reg >>= 1;
    reg |= oldBit7;
    
    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit0 > 0);

    return 8;
}

int CPU::ShiftByteRightIntoCarry(unsigned char& reg)
{
    unsigned char oldBit0 = (reg & 0x01);

    reg >>= 1;
    
    SetZeroFlag(reg == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(oldBit0 > 0);

    return 8;
}

int CPU::SwapByteNibbles(unsigned char& reg)
{
    unsigned char high = (reg & 0xF0);
    unsigned char low = (reg & 0x0F);
    reg = ((low << 4) | high);

    return 8;
}

int CPU::Bit(unsigned char bit, unsigned char reg)
{
    unsigned char mask = 1;
    if (bit > 0)
    {
        mask <<= 1;
    }

    SetZeroFlag( (reg & mask) == 0 );
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);

    return 8;
}

int CPU::Res(unsigned char bit, unsigned char& reg)
{
    unsigned char mask = 1;
    if (bit > 0)
    {
        mask <<= 1;
    }

    reg &= ~mask;

    return 8;
}

int CPU::Set(unsigned char bit, unsigned char& reg)
{
    unsigned char mask = 1;
    if (bit > 0)
    {
        mask <<= 1;
    }

    reg |= mask;

    return 8;
}

}