//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <list>
#include <optional>
#include <cassert>

#include "record.hpp"
#include "pattern.hpp"
#include "range_concepts.hpp"

namespace spring {
    struct ArrangementFinder {
        template<support::random_access_range_of<Condition> Range>
        static std::list<Record> find(const Range&, Pattern);

        template<support::random_access_range_of<Condition> Range>
        static std::list<Record> match_empty_pattern(const Range&);

        template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Ittr>
        static std::list<Record> match_damaged(const Range&, const Ittr&, Pattern);

        template<support::random_access_range_of<Condition> Range>
        static std::optional<Record> ensure_and_replace(const Range&, Condition);
    };

    template<support::random_access_range_of<Condition> Range>
    std::list<Record> ArrangementFinder::find(const Range& range, Pattern pattern) {
        if(pattern.empty()) return match_empty_pattern(range);
        if(pattern.front() > std::ranges::size(range)) return {};

        std::list<Record> arrangements;
        for (auto itr=std::ranges::begin(range); itr + pattern.front() <= std::ranges::end(range); ++itr) {
            auto sub_arrangements = match_damaged(range, itr, pattern);
            arrangements.splice(arrangements.end(), sub_arrangements);
        }
        return arrangements;
    }

    template<support::random_access_range_of<Condition> Range>
    std::list<Record> ArrangementFinder::match_empty_pattern(const Range& range) {
        auto replaced = ensure_and_replace(range, Condition::OPERATIONAL);
        if(replaced.has_value()) return {*replaced};
        return {};
    }

    template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Ittr>
    std::list<Record> ArrangementFinder::match_damaged(const Range& range, const Ittr& itr, Pattern pattern) {
        using namespace std::ranges;
        auto match_size = pattern.front();
        pattern.pop_front();
        auto before = ensure_and_replace(subrange(begin(range), itr), Condition::OPERATIONAL);
        auto match_range = ensure_and_replace(subrange(itr, itr+match_size, match_size), Condition::DAMAGED);
        if(!before || !match_range) return {};
        if(itr+match_size == end(range) && empty(pattern)) return {*before + *match_range};
        auto gap = ensure_and_replace(subrange(itr+match_size, itr+match_size+1), Condition::OPERATIONAL);
        if(!gap) return {};

        Record prefix = Record::join({*before, *match_range, *gap});
        auto sub_records = find(subrange(itr+match_size+1, end(range)), pattern);

        std::list<Record> arrangements;
        for(auto& sub_record : sub_records) {
            auto arrangement = prefix + sub_record;
            assert(size(arrangement) == size(range));
            arrangements.push_back(arrangement);
        }
        return arrangements;
    }

    template<support::random_access_range_of<Condition> Range>
    std::optional<Record> ArrangementFinder::ensure_and_replace(const Range& range, const Condition target_condition) {
        Record record(std::ranges::begin(range), std::ranges::end(range));
        for (auto& condition: record) {
            if(condition == Condition::UNKNOWN) condition = target_condition;
            else if(condition != target_condition) return {};
        }
        return record;
    }
}
