#ifndef SKETCH_EMULATOR_INTERRUPTS_HANDLER_H
#define SKETCH_EMULATOR_INTERRUPTS_HANDLER_H

namespace SketchEmulator {

// Forward class declarations
class CPU;
class Memory;

enum InterruptType_t {
    V_BLANK = 0x00,
    LCDC_STATUS = 0x01,
    TIMER_OVERFLOW = 0x02,
    SERIAL_TRANSFER_COMPLETION = 0x04,
    HIGH_TO_LOW_P10_P13 = 0x8,

    NO_INTERRUPTS = 0xFF
};

class InterruptsHandler {
    public:
                        InterruptsHandler();

        void            Initialize(Memory& memory);
        void            Update();
        InterruptType_t GetInterruptToHandle();
        void            HandleInterrupt(InterruptType_t type, CPU* cpu);

    private:
        unsigned char*  m_RaisedInterrupts;
        unsigned char*  m_AskedInterrupts;
};

}

#endif