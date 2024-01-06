//
// Created by Conor Pappas on 1/5/24.
//

#include "platform.hpp"

#include <unordered_map>
#include <utility>

#include "bool_change_set.hpp"

namespace dish::data_types {
    struct Platform::Private {
        using ChangeSet = BoolChangeSet<Point, Point::Hasher>;

        static ChangeSet build_change_set(const Platform&, Direction);
        static Point resting_location(const Platform&, Direction, const Point&);
        static bool in_grid(const Platform&, const Point&);
    };

    Platform::Platform() = default;

    Platform::Platform(container entities):
        m_entities(std::move(entities)) {}

    size_t Platform::width() const {
        return m_entities[0].size();
    }

    size_t Platform::height() const {
        return m_entities.size();
    }

    Platform::container& Platform::entities() {
        return m_entities;
    }

    const Platform::container& Platform::entities() const {
        return m_entities;
    }

    void Platform::roll(const Direction dir) {
        const auto change_set = Private::build_change_set(*this, dir);
        for(const auto& [point, entity] : change_set.changes()) {
            const Entity final = entity ? Entity::ROUND : Entity::EMPTY;
            m_entities[point.y()][point.x()] = final;
        }
    };

    Platform::Private::ChangeSet Platform::Private::build_change_set(const Platform& platform, const Direction dir) {
        ChangeSet change_set;
        const auto& entities = platform.entities();
        for(size_t y = 0; y < entities.size(); ++y) {
            for(size_t x = 0; x < entities[y].size(); ++x) {
                if(entities[y][x] == data_types::Entity::ROUND) {
                    const auto resting_loation = resting_location(platform, dir, {x, y});
                    change_set.move({x,y}, resting_loation);
                }
            }
        }
        return change_set;
    }

    Point Platform::Private::resting_location(const Platform& platform, const Direction dir, const Point& start) {
        point_size_t blocking_rocks = 0;
        Point pos = start;
        const Point direction = to_point(dir);
        for(; in_grid(platform, pos); pos += direction) {
            const auto entity = platform.entities()[pos.y()][pos.x()];
            if(entity == Entity::ROUND) ++blocking_rocks;
            if(entity == Entity::CUBE) break;
        }
        return pos - direction * blocking_rocks;
    }

    bool Platform::Private::in_grid(const Platform& platform, const Point& point) {
        return point.x() < platform.width() && point.y() < platform.height();
    }

    bool operator==(const Platform& lhs, const Platform& rhs) {
        return lhs.m_entities == rhs.m_entities;
    }
}
