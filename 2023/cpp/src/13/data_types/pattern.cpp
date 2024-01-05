//
// Created by Conor Pappas on 1/4/24.
//

#include "pattern.hpp"

#include <ostream>

namespace incidence::data_types {
    Pattern::Pattern() = default;

    Pattern::Pattern(const support::Grid<Entity,2>& entities):
        m_entities(entities) {}

    Entity Pattern::get(const size_t x, const size_t y) const {
        return m_entities[y][x];
    }

    size_t Pattern::width() const {
        return m_entities[0].size();
    }

    size_t Pattern::height() const {
        return m_entities.size();
    }

    std::ostream& operator<<(std::ostream& os, const Pattern& pattern) {
        for (size_t i = 0; i < pattern.height(); ++i) {
            for (size_t j = 0; j < pattern.width(); ++j) {
                os << static_cast<char>(pattern.get(j, i));
            }
            os << '\n';
        }
        return os;
    }

    support::Grid<Entity,2>::iterator Pattern::begin() {
        return m_entities.begin();
    }

    support::Grid<Entity,2>::iterator Pattern::end() {
        return m_entities.end();
    }

    support::Grid<Entity,2>::const_iterator Pattern::begin() const {
        return m_entities.begin();
    }

    support::Grid<Entity,2>::const_iterator Pattern::end() const {
        return m_entities.end();
    }

    Pattern Pattern::transpose() const {
        return Pattern(m_entities.transpose());
    }
}

