//
// Created by Conor Pappas on 1/16/24.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <cassert>
#include <functional>

#include <range_concepts.hpp>

#include "label.hpp"
#include "module.hpp"

namespace pulse::data_types {
    enum class ModuleType {
        Broadcast, FlipFlop, Conjunction, Test
    };
    struct PulseCount {
        size_t high {};
        size_t low {};
    };

    using Network = std::unordered_map<label, std::unique_ptr<pulse::data_types::Module>>;

    std::unique_ptr<Module> make_module(const label&, ModuleType);
    void register_module(Network&, const label&, ModuleType);

    template<support::range_of<label> Range>
    void link_modules(Network&, const label& origin, const Range& destinations);

    PulseCount& operator+=(PulseCount&, const Pulse&);
    PulseCount operator+(const PulseCount&, const PulseCount&);
    PulseCount press_button(Network&, const std::function<void(const Signal&)>&visitor = {});

    template<support::range_of<label> Range>
    void link_modules(Network& network, const label& origin, const Range& destinations) {
        assert(network.contains(origin));
        for(const auto& destination : destinations) {
            if(!network.contains(destination)) {
                register_module(network, destination, ModuleType::Test);
            }
            connect(*network[origin], *network[destination]);
        }
    }
}
