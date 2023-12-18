//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

#include <vector>

namespace oasis {
    class History {
        using value_type = long;

    public:
        History();
        History(const std::vector<value_type>& values);

        void add(const value_type& value);
        const std::vector<value_type>& get_values() const;

    private:
        std::vector<value_type> values;
    };
};
