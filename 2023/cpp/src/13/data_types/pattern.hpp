//
// Created by Conor Pappas on 12/23/23.
//

#pragma once

#include <vector>

#include "grid.hpp"

#include "entity.hpp"

namespace incidence::data_types {
    class Pattern {
    public:
        Pattern();
        explicit Pattern(const support::Grid<Entity,2>&);

        [[nodiscard]] Entity get(size_t x,size_t y) const;
        [[nodiscard]] size_t width() const;
        [[nodiscard]] size_t height() const;

        [[nodiscard]] Pattern transpose() const;

        [[nodiscard]] support::Grid<Entity,2>::iterator begin();
        [[nodiscard]] support::Grid<Entity,2>::iterator end();
        [[nodiscard]] support::Grid<Entity,2>::const_iterator begin() const;
        [[nodiscard]] support::Grid<Entity,2>::const_iterator end() const;

        friend std::ostream& operator<<(std::ostream&, const Pattern&);
    private:
        support::Grid<Entity,2> m_entities {};
    };
}
