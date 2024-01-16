//
// Created by Conor Pappas on 1/15/24.
//

#include "conjunction_module.hpp"

namespace pulse::data_types {
    void ConjunctionModule::register_input(const label& label) {
        Module::register_input(label);
        m_memory[label] = Pulse::Low;
    }

    SignalList ConjunctionModule::propagate(const Signal& signal) {
        m_memory[signal.origin] = signal.pulse;
        auto output_pulse = Pulse::Low;
        for(const auto& [label, pulse] : m_memory) {
            if(pulse == Pulse::Low) output_pulse = Pulse::High;
        }
        return Base::build_signal(output_pulse);
    }
}
