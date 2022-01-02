#include <string_view>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "data/data_day_10.h"

// Map delimiters to score
auto SCORE_MAP = std::map<char, int>({{')',3}, {']', 57}, {'}', 1197}, {'>', 25137}});
auto COMPLETION_SCORE = std::map<char, int>({{'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}});
// Map closing brace to opening brace
auto MATCH_BRACE = std::map<char, char>({{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}});

constexpr std::array OPENERS = {'(', '[', '{', '<'};

bool is_opener(char c)
{
    return std::count(OPENERS.begin(), OPENERS.end(), c) > 0;
}

enum class State
{
    OK,
    INCOMPLETE,
    CORRUPTED
};

// Parse a line and return whether it's valid, and if the line is corrupted or incomplete, calculate its score
std::pair<State, int64_t> parse_line(std::string_view line)
{
    std::vector<char> stack;
    for (auto c : line)
    {
        if (is_opener(c))
            stack.push_back(c);

        else if (MATCH_BRACE[c] != stack.back())
            return {State::CORRUPTED, SCORE_MAP[c]};

        else
            stack.pop_back();
    }
    if (stack.size() > 0)
    {
        return {State::INCOMPLETE, std::accumulate(stack.rbegin(), stack.rend(), 0l, [] (auto a, auto b) {return a * 5 + COMPLETION_SCORE[b];})};
    }
    return {State::OK, 0};
}

void solve_day_10()
{
    int corrupt_score = 0;
    std::vector<int64_t> completion_score;

    for (std::string_view line : DATA_10_12)
    {
        auto [status, score] = parse_line(line);
        if (status == State::CORRUPTED)
            corrupt_score += score;

        if (status == State::INCOMPLETE)
            completion_score.push_back(score);
    }

    std::sort(completion_score.begin(), completion_score.end());
    std::cout << "Total corrupt score: " << corrupt_score << std::endl;
    std::cout << "Median completion score: " << completion_score[completion_score.size() / 2] << std::endl;
}

int main()
{
    solve_day_10();
    return 0;
}

