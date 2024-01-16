//
// Created by Conor Pappas on 1/15/24.
//

#include "module.hpp"

#include <cassert>
#include <utility>

namespace pulse::data_types {
    Module::Module(label label): m_label(std::move(label)) { }

    Module::~Module() = default;

    const label& Module::get_label() const {
        return m_label;
    }

    void Module::register_input(const label& label) {
        m_input_labels.push_back(label);
    }

    void Module::register_output(const label& label) {
        m_output_labels.push_back(label);
    }

    SignalList Module::propagate(const Signal&) {
        return {};
    }

    SignalList Module::build_signal(const Pulse& pulse) const {
        SignalList result;
        for(const auto& destination : m_output_labels) {
            result.push_back({ m_label, destination, pulse });
        }
        return result;
    }

    void connect(Module& origin, Module& destination) {
        origin.register_output(destination.get_label());
        destination.register_input(origin.get_label());
    }
}
