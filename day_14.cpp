#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>

#include "data/data_day_14.h"

constexpr int ITERATIONS_PT_1 = 10;
constexpr int ITERATIONS_PT_2 = 40;

std::map<std::string, char> parse_map()
{
    std::map<std::string, char> map;
    for (const auto& p : DATA_14_12)
    {
        map.insert({p.first, p.second});
    }
    return map;
}

std::string generate_polymer(std::map<std::string, char>& map, std::string init, int iters)
{
    for (int i = 0; i < iters; ++i)
    {
        auto new_pairs = std::string(init.size(), ' ');
        auto merged = std::string(1, init[0]);

        std::adjacent_difference(init.begin(), init.end(), new_pairs.begin(), [&] (auto a, auto b) {return map[std::string({b, a,})];});
        for (int c = 1; c < init.size(); ++c)
        {
            merged.push_back(new_pairs[c]);
            merged.push_back(init[c]);
        }
        init = merged;
    }
    return init;
}

void count_chars(std::string data)
{
    std::map<char, int> counts;
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        if (int count = std::count(data.begin(), data.end(), c))
            counts[c] = count;
    }
    auto max = std::max_element(counts.begin(), counts.end(), [] (auto a, auto b) {return a.second < b.second;});
    auto min = std::min_element(counts.begin(), counts.end(), [] (auto a, auto b) {return a.second < b.second;});
    std::cout << "Solution " << max->second - min->second << std::endl;
}

void  solve_part_1()
{
    auto map = parse_map();
    count_chars(generate_polymer(map, DATA_14_INIT, ITERATIONS_PT_1));
}

void solve_part_2()
{
    /* The naive version in part 1 cannot be use to solve part 2 since it would run out of memory.
     * As we're only interested in the number of pairs, we can use a similar algorithm as when we
     * counted fish, i.e. only keep track of the number of each type of pair, not their positions
     * in the polymer string */
    auto map = parse_map();
    std::map<std::string, int64_t> pairs;
    std::string_view init(DATA_14_INIT);
    for (int i = 1 ; i < init.size(); ++i)
    {
        pairs[std::string({init[i-1], init[i]})] += 1;
    }

    for (int i = 0; i < ITERATIONS_PT_2; ++i)
    {
        std::map<std::string, int64_t> new_pairs;
        for (const auto& p : pairs)
        {
            char new_char = map[p.first];
            new_pairs[std::string({p.first[0], new_char})] += p.second;
            new_pairs[std::string({new_char, p.first[1]})] += p.second;
        }
        pairs = new_pairs;
    }

    /* Sum over all the pairs to count the number of characters, note that this counts every character
     * twine, except for the first and last character in the string, this needs to be compensated for */
    std::map<char, int64_t> chars;
    chars[init.front()] += 1;
    chars[init.back()] += 1;
    for (const auto& p : pairs)
    {
        chars[p.first[0]] += p.second;
        chars[p.first[1]] += p.second;
    }

    auto max = std::max_element(chars.begin(), chars.end(), [] (auto a, auto b) {return a.second < b.second;});
    auto min = std::min_element(chars.begin(), chars.end(), [] (auto a, auto b) {return a.second < b.second;});
    std::cout << "Solution: " << (max->second / 2 - min->second / 2) << std::endl;
}

int main()
{
    solve_part_1();
    solve_part_2();
    return 0;
}