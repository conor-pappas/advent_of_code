//
// Created by Conor Pappas on 1/16/24.
//

#include "network.hpp"

#include <deque>

#include "data_types/module.hpp"
#include "data_types/broadcast_module.hpp"
#include "data_types/conjunction_module.hpp"
#include "data_types/flip_flop_module.hpp"

namespace pulse::data_types {
    std::unique_ptr<Module> make_module(const label& label, ModuleType module_type) {
        switch(module_type) {
            case ModuleType::Broadcast:
                return std::make_unique<BroadcastModule>(label);
            case ModuleType::FlipFlop:
                return std::make_unique<FlipFlopModule>(label);
            case ModuleType::Conjunction:
                return std::make_unique<ConjunctionModule>(label);
            case ModuleType::Test:
                return std::make_unique<Module>(label);
            default:
                throw std::runtime_error("Invalid module type");
        }
    }

    void register_module(Network& network, const label& label, ModuleType module_type) {
        network[label] = make_module(label, module_type);
    }

    PulseCount& operator+=(PulseCount& signal_count, const Pulse& pulse) {
        if(pulse == Pulse::High) signal_count.high++;
        else signal_count.low++;
        return signal_count;
    }

    PulseCount operator+(const PulseCount& lhs, const PulseCount& rhs) {
        return { lhs.high + rhs.high, lhs.low + rhs.low };
    }

    PulseCount press_button(Network& network) {
        std::deque<Signal> signals {{BUTTON, BROADCASTER, Pulse::Low } };
        PulseCount result;
        while(!signals.empty()) {
            auto signal = signals.front();
            signals.pop_front();
            result += signal.pulse;
            const auto module = network[signal.destination].get();
            auto output_signals = module->propagate(signal);
            signals.insert(signals.end(), output_signals.begin(), output_signals.end());
        }
        return result;
    }
}
