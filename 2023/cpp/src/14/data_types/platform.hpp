//
// Created by Conor Pappas on 1/4/24.
//

#pragma once

#include <grid.hpp>

#include "entity.hpp"
#include "direction.hpp"

namespace dish::data_types {
    class Platform {
    public:
        using container = support::Grid<Entity,2>;

        Platform();
        explicit Platform(container);

        [[nodiscard]] size_t width() const;
        [[nodiscard]] size_t height() const;

        container& entities();
        [[nodiscard]] const container& entities() const;

        void roll(Direction);

        friend bool operator==(const Platform&, const Platform&);
    private:
        container m_entities {};
        struct Private;
    };
}
