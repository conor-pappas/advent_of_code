//
// Created by Conor Pappas on 1/5/24.
//

#pragma once

#include <unordered_map>

namespace dish::data_types {
    template<typename Object, typename Hash = std::hash<Object>>
    class BoolChangeSet {
    public:
        using container = std::unordered_map<Object, bool, Hash>;

        void change(const Object&, bool);
        void add(const Object&);
        void remove(const Object&);
        void move(const Object& start, const Object& end);

        [[nodiscard]] const container& changes() const;

    private:
        container m_changes {};
    };

    template<typename Object, typename Hash>
    void BoolChangeSet<Object,Hash>::change(const Object& key, bool value) {
        if (!m_changes.contains(key)) {
            m_changes[key] = value;
        } else if(m_changes[key] != value) {
            m_changes.erase(key);
        }
    }

    template<typename Object, typename Hash>
    void BoolChangeSet<Object, Hash>::add(const Object& key) {
        return change(key, true);
    }

    template<typename Object, typename Hash>
    void BoolChangeSet<Object, Hash>::remove(const Object& key) {
        return change(key, false);
    }

    template<typename Object, typename Hash>
    void BoolChangeSet<Object, Hash>::move(const Object& start, const Object& end) {
        remove(start);
        add(end);
    }

    template<typename Object, typename Hash>
    const typename BoolChangeSet<Object, Hash>::container& BoolChangeSet<Object, Hash>::changes() const {
        return m_changes;
    }
}
