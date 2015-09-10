#include "InterruptsHandler.h"

#include "CPU.h"
#include "Memory.h"

namespace SketchEmulator {
InterruptsHandler::InterruptsHandler()
    : m_RaisedInterrupts(nullptr)
    , m_AskedInterrupts(nullptr)
{
}

void InterruptsHandler::Initialize(Memory& memory)
{
    m_RaisedInterrupts = memory.GetRaisedInterruptsRegister();
    m_AskedInterrupts = memory.GetInterruptEnableRegister();
}

void InterruptsHandler::Update()
{
}

InterruptType_t InterruptsHandler::GetInterruptToHandle()
{
    unsigned char raisedInterrupts = *m_RaisedInterrupts;
    unsigned char askedInterrupts = *m_AskedInterrupts;

    for (int i = 0; i < 5; i++)
    {
        InterruptType_t type = InterruptType_t::V_BLANK;
        if (i > 0)
        {
            type = (InterruptType_t)(1 << i);
        }

        if ( (raisedInterrupts & type) > 0 && (askedInterrupts & type) > 0 )
        {
            return type;
        }
    }
    
    return InterruptType_t::NO_INTERRUPTS;
}

void InterruptsHandler::HandleInterrupt(InterruptType_t type, CPU* cpu)
{
    cpu->m_AreInterruptsEnabled = false;
    unsigned char P, C;
    cpu->m_Memory.ConvertShortTo2Bytes(cpu->m_PC, P, C);
    cpu->PushShort(P, C);

    switch (type)
    {
    case InterruptType_t::V_BLANK:                      cpu->m_PC = cpu->m_Memory.Convert2BytesToShort(0x00, 0x40); break;
    case InterruptType_t::LCDC_STATUS:                  cpu->m_PC = cpu->m_Memory.Convert2BytesToShort(0x00, 0x48); break;
    case InterruptType_t::TIMER_OVERFLOW:               cpu->m_PC = cpu->m_Memory.Convert2BytesToShort(0x00, 0x50); break;
    case InterruptType_t::SERIAL_TRANSFER_COMPLETION:   cpu->m_PC = cpu->m_Memory.Convert2BytesToShort(0x00, 0x58); break;
    case InterruptType_t::HIGH_TO_LOW_P10_P13:          cpu->m_PC = cpu->m_Memory.Convert2BytesToShort(0x00, 0x60); break;
    }

    *m_RaisedInterrupts &= ~((int)type);
}

}