//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <unordered_map>

#include "module.hpp"

namespace pulse::data_types {

    class ConjunctionModule final : public Module {
    public:
        using Module::Module;
        using Base = Module;

        void register_input(const label&) override;

        [[nodiscard]] SignalList propagate(const Signal&) override;
    private:
        std::unordered_map<label, Pulse> m_memory;
    };
}
