//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <unordered_map>
#include <unordered_set>

#include "point.hpp"

namespace cosmic {
    class Space {
    public:
        using coord_type = int;
        using Galaxy = support::Point<coord_type, 2>;
    private:
        using galaxy_set = std::pmr::unordered_set<Galaxy, Galaxy::Hasher>;
    public:
        using iterator = galaxy_set::iterator;

        Space();
        Space(const coord_type& width, const coord_type& height);

        [[nodiscard]] coord_type get_width() const;
        [[nodiscard]] coord_type get_height() const;
        [[nodiscard]] iterator begin() const;
        [[nodiscard]] iterator end() const;

        void set_width(const coord_type&);
        void set_height(const coord_type&);
        void add_galaxy(const Galaxy&);

        void expand();

        friend std::ostream& operator<<(std::ostream& os, const Space& space);

    private:
        coord_type m_width {};
        coord_type m_height {};
        galaxy_set m_galaxies {};

        struct Private;
    };
}
