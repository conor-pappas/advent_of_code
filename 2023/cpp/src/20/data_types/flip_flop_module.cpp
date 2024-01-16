//
// Created by Conor Pappas on 1/15/24.
//

#include "flip_flop_module.hpp"

namespace pulse {
    data_types::SignalList data_types::FlipFlopModule::propagate(const Signal& signal) {
        if(signal.pulse == Pulse::High) return {};
        m_state = !m_state;
        const Pulse pulse = m_state ? Pulse::High : Pulse::Low;
        return Base::build_signal(pulse);
    }
}
