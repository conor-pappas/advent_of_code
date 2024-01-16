//
// Created by Conor Pappas on 1/15/24.
//

#include "broadcast_module.hpp"

namespace pulse::data_types {
    SignalList BroadcastModule::propagate(const Signal& signal) {
        return Base::build_signal(signal.pulse);
    }
}
