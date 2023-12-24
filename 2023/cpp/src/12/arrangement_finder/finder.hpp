//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

#include <list>
#include <optional>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "data_types/record.hpp"
#include "data_types/pattern.hpp"
#include "range_concepts.hpp"

#include "request.hpp"
#include "result.hpp"

namespace spring::arrangement_finder {
    using namespace data_types;

    class Finder {
    public:
        using result_count = size_t;
        Result find(const Request&);

        template<support::random_access_range_of<Condition> Range>
        Result find(const Range&, Pattern);

        template<support::random_access_range_of<Condition> Range>
        result_count find_count(const Range&, Pattern);

        template<support::random_access_range_of<Condition> Range>
        Result match_empty_pattern(const Range&);

        template<support::random_access_range_of<Condition> Range>
        result_count match_empty_pattern_count(const Range&);

        template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Itr>
        Result match_damaged(const Range&, const Itr&, Pattern);

        template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Itr>
        result_count match_damaged_count(const Range&, const Itr&, Pattern);

        template<support::random_access_range_of<Condition> Range>
        static std::optional<Record> ensure_and_replace(const Range&, Condition);

        template<support::random_access_range_of<Condition> Range>
        static bool ensure_all(const Range&, Condition);

    private:
        std::unordered_map<Request, Result, Request::Hasher> m_request_cache;
        std::unordered_map<Request, result_count, Request::Hasher> m_request_count_cache;

        const Result& cache_result(const Request&, const Result&);
        template<support::random_access_range_of<Condition> Range>
        const Result& cache_result(const Range&, const Pattern&, const Result&);
        std::optional<const Result> cache_lookup(const Request&);
        template<support::random_access_range_of<Condition> Range>
        std::optional<const Result> cache_lookup(const Range&, const Pattern&);

        result_count cache_result_count(const Request&, result_count);
        template<support::random_access_range_of<Condition> Range>
        result_count cache_result_count(const Range&, const Pattern&, result_count);
        std::optional<result_count> cache_lookup_count(const Request&);
        template<support::random_access_range_of<Condition> Range>
        std::optional<result_count> cache_lookup_count(const Range&, const Pattern&);

        template<support::random_access_range_of<Condition> Range>
        static Request to_request(const Range&, Pattern);

    };

    // TODO:: Add memoizer to support lib
    template<support::random_access_range_of<Condition> Range>
    Result Finder::find(const Range& range, Pattern pattern) {
        if(auto cached = cache_lookup(range, pattern)) return *cached;
        auto memo = [&](const Result& result) { return cache_result(range, pattern, result); };

        if(std::ranges::empty(pattern)) return memo(match_empty_pattern(range));
        if(pattern.peak() > std::ranges::size(range)) return memo({});

        Result arrangements;
        for (auto itr=std::ranges::begin(range); itr + pattern.peak() <= std::ranges::end(range); ++itr) {
            arrangements.splice(arrangements.end(), match_damaged(range, itr, pattern));
        }
        return memo(arrangements);
    }

    template<support::random_access_range_of<Condition> Range>
    Finder::result_count Finder::find_count(const Range& range, Pattern pattern) {
        if(auto cached = cache_lookup_count(range, pattern)) return *cached;
        auto memo = [&](result_count result) { return cache_result_count(range, pattern, result); };

        if(std::ranges::empty(pattern)) return memo(match_empty_pattern_count(range));
        if(pattern.peak() > std::ranges::size(range)) return memo(0);

        result_count count = 0;
        for (auto itr=std::ranges::begin(range); itr + pattern.peak() <= std::ranges::end(range); ++itr) {
            count += match_damaged_count(range, itr, pattern);
        }
        return memo(count);
    }

    template<support::random_access_range_of<Condition> Range>
    Result Finder::match_empty_pattern(const Range& range) {
        auto replaced = ensure_and_replace(range, Condition::OPERATIONAL);
        if(replaced.has_value()) return {*replaced};
        return {};
    }

    template<support::random_access_range_of<Condition> Range>
    Finder::result_count Finder::match_empty_pattern_count(const Range& range) {
        if(ensure_and_replace(range, Condition::OPERATIONAL)) return 1;
        return 0;
    }

    template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Itr>
    Result Finder::match_damaged(const Range& range, const Itr& itr, Pattern pattern) {
        using namespace std::ranges;
        auto run_size = pattern.peak();
        pattern.pop();
        auto before = ensure_and_replace(Record(begin(range), itr), Condition::OPERATIONAL);
        auto match_range = ensure_and_replace(Record(itr, itr+run_size), Condition::DAMAGED);
        if(!before || !match_range) return {};
        if(itr+run_size == end(range) && empty(pattern)) return {*before + *match_range};
        auto gap = ensure_and_replace(subrange(itr+run_size, itr+run_size+1), Condition::OPERATIONAL);
        if(!gap) return {};

        Record prefix = Record::join({*before, *match_range, *gap});
        auto sub_records = find(subrange(itr+run_size+1, end(range)), pattern);

        Result arrangements;
        for(auto& sub_record : sub_records) {
            auto arrangement = prefix + sub_record;
            assert(size(arrangement) == size(range));
            assert(arrangement.is_concrete());
            arrangements.push_back(arrangement);
        }
        return arrangements;
    }

    template<support::random_access_range_of<Condition> Range, support::input_iterator_of<Condition> Itr>
    Finder::result_count Finder::match_damaged_count(const Range& range, const Itr& itr, Pattern pattern) {
        using namespace std::ranges;
        auto run_size = pattern.peak();
        pattern.pop();
        const bool before = ensure_all(Record(begin(range), itr), Condition::OPERATIONAL);
        const bool match_range = ensure_all(Record(itr, itr+run_size), Condition::DAMAGED);
        if(!before || !match_range) return 0;
        if(itr+run_size == end(range) && empty(pattern)) return 1;
        const bool gap = ensure_all(subrange(itr+run_size, itr+run_size+1), Condition::OPERATIONAL);
        if(!gap) return 0;

        return find_count(subrange(itr+run_size+1, end(range)), pattern);
    }

    template<support::random_access_range_of<Condition> Range>
    std::optional<Record> Finder::ensure_and_replace(const Range& range, const Condition target_condition) {
        Record record(std::ranges::begin(range), std::ranges::end(range));
        for (auto& condition: record) {
            if(condition == Condition::UNKNOWN) condition = target_condition;
            else if(condition != target_condition) return {};
        }
        return record;
    }

    template<support::random_access_range_of<Condition> Range>
    bool Finder::ensure_all(const Range& range, Condition target_condition) {
        for (auto& condition: range) {
            if(condition == Condition::UNKNOWN) continue;
            if(condition == target_condition) continue;
            return false;
        }
        return true;
    }

    template<support::random_access_range_of<Condition> Range>
    const Result& Finder::cache_result(const Range& range, const Pattern& pattern, const Result& result) {
        return cache_result(to_request(range, pattern), result);
    }

    template<support::random_access_range_of<Condition> Range>
    std::optional<const Result> Finder::cache_lookup(const Range& range, const Pattern& pattern) {
        return cache_lookup(to_request(range, pattern));
    }

    template<support::random_access_range_of<Condition> Range>
    Finder::result_count Finder::cache_result_count(const Range& range, const Pattern& pattern, const result_count result) {
        return cache_result_count(to_request(range, pattern), result);
    }

    template<support::random_access_range_of<Condition> Range>
    std::optional<Finder::result_count> Finder::cache_lookup_count(const Range& range, const Pattern& pattern) {
        return cache_lookup_count(to_request(range, pattern));
    }

    template<support::random_access_range_of<Condition> Range>
    Request Finder::to_request(const Range& range, Pattern pattern) {
        const Record record(std::ranges::begin(range), std::ranges::end(range));
        return Request{record,std::move(pattern)};
    }
}
