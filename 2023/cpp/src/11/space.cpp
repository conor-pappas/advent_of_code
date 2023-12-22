//
// Created by Conor Pappas on 12/22/23.
//

#include "space.hpp"

#include <iostream>

#include "parse.hpp"

namespace cosmic {
    Space::Space() = default;

    Space::Space(const coord_type& width, const coord_type& height):
        m_width(width),
        m_height(height) {}

    Space::coord_type Space::get_width() const { return m_width; }
    Space::coord_type Space::get_height() const { return m_height; }

    std::pmr::unordered_set<Space::Galaxy>::iterator Space::begin() const { return m_galaxies.begin(); }
    std::pmr::unordered_set<Space::Galaxy>::iterator Space::end() const { return m_galaxies.end(); }

    void Space::set_width(const coord_type& width) { m_width = width; }
    void Space::set_height(const coord_type& height) { m_height = height; }

    void Space::add_galaxy(const Galaxy& galaxy) { m_galaxies.insert(galaxy); }

    struct Space::Private {
        struct dim_index {
            std::unique_ptr<coord_type[]> columns;
            std::unique_ptr<coord_type[]> rows;
        };
        static dim_index count_dimensions(const Space&);
        static void expand_dimensions(Space&, const dim_index&,coord_type);
        static void expand_galaxies(Space&, const dim_index&, coord_type);
    };

    void Space::expand(const coord_type size) {
        const auto index = Private::count_dimensions(*this);
        Private::expand_dimensions(*this, index, size);
        Private::expand_galaxies(*this, index, size);
    }

    Space::Private::dim_index Space::Private::count_dimensions(const Space& space) {
        // TODO: Make wrapper class for run-time sized fixed width array
        std::unique_ptr<coord_type[]> columns(new coord_type[space.m_width]());
        std::unique_ptr<coord_type[]> rows(new coord_type[space.m_height]());
        for(const Galaxy& galaxy: space) {
            columns[galaxy.x()]++;
            rows[galaxy.y()]++;
        }
        return { std::move(columns), std::move(rows) };
    }

    void Space::Private::expand_dimensions(Space& space, const dim_index& index, const coord_type size) {
        coord_type width_adjust = 0;
        coord_type height_adjust = 0;
        for(coord_type x=0; x<space.m_width; ++x) {
            if(index.columns[x] == 0) width_adjust += size;
        }
        for(coord_type y=0; y<space.m_height; ++y) {
            if(index.rows[y] == 0) height_adjust += size;
        }
        space.m_width += width_adjust;
        space.m_height += height_adjust;
    }

    void Space::Private::expand_galaxies(Space& space, const dim_index& index, const coord_type size) {
        galaxy_set new_galaxies;
        for(const auto& galaxy: space) {
            Galaxy shift = {0,0};
            for(coord_type x=0; x<galaxy.x(); ++x) {
                if(index.columns[x] == 0) shift.x() += size;
            }
            for(coord_type y=0; y<galaxy.y(); ++y) {
                if(index.rows[y] == 0) shift.y() += size;
            }
            new_galaxies.insert(galaxy + shift);
        }
        space.m_galaxies = new_galaxies;
    }

    std::ostream& operator<<(std::ostream& os, const Space& space) {
        os << "Space: " << space.get_width() << "x" << space.get_height() << '\n';
        for(Space::coord_type y=0; y<space.get_height(); ++y) {
            for(Space::coord_type x=0; x<space.get_width(); ++x) {
                if(space.m_galaxies.contains(Space::Galaxy{x, y})) {
                    os << static_cast<char>(parse::Symbol::GALAXY);
                } else {
                    os << static_cast<char>(parse::Symbol::SPACE);
                }
            }
            os << '\n';
        }
        return os;
    }
}
