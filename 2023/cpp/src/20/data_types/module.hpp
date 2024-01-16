//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

#include <list>

#include "label.hpp"
#include "signal.hpp"

namespace pulse::data_types {
    class Module {
    public:
        explicit Module(label label);

        virtual ~Module();

        [[nodiscard]] const label& get_label() const;

        virtual void register_input(const label&);
        virtual void register_output(const label&);

        [[nodiscard]] virtual bool is_nand() const;

        virtual SignalList propagate(const Signal&);
    private:
        label m_label;
        std::list<label> m_output_labels;
        std::list<label> m_input_labels;

    protected:
        [[nodiscard]] SignalList build_signal(const Pulse&) const;
    };

    void connect(Module& origin, Module& destination);
}
