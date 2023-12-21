//
// Created by Conor Pappas on 12/18/23.
//

#pragma once

#include <vector>

namespace oasis {
    class History {
    public:
        using value_type = long;

        History();
        explicit History(const std::vector<value_type>& values);

        void add(const value_type& value);
        [[nodiscard]] const std::vector<value_type>& get_values() const;
        [[nodiscard]] std::size_t size() const;

        friend std::ostream& operator<<(std::ostream& os, const History& history);

    private:
        std::vector<value_type> m_values {};
    };
};
