//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include "module.hpp"

namespace pulse::data_types {

    class FlipFlopModule final : public Module {
    public:
        using Module::Module;
        using Base = Module;

        SignalList propagate(const Signal&) override;
    private:
         bool m_state { false };
    };
}
