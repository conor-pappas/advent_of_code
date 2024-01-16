//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <list>

#include "label.hpp"

namespace pulse::data_types {
    enum class Pulse { High, Low };

    struct Signal {
        label origin {};
        label destination {};
        Pulse pulse {};
    };

    using SignalList = std::list<Signal>;
}
