#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

#include "data/data_day_6.h"

constexpr int FISH_START_TIME = 8;

int growth_function(const int& age)
{
    return age > 0? age - 1 : 6;
}

// Recursively count the number of fish 1 fish spawns after n days
int64_t fish_after(int days)
{
    int64_t fish = 1;
    days -= 9;
    while (days >= 0)
    {
        fish += fish_after(days);
        days -= 7;
    }
    return fish;
}

/* Naive solver that works well for part 1, but runs out of memory
 * when solving part 2 */
void solve_day_6(int days)
{
    std::vector<int> fish(DATA_6_12.begin(), DATA_6_12.end());

    for (int i = 0; i < days; ++i)
    {
        int spawn = std::count(fish.begin(), fish.end(), 0);
        std::transform(fish.begin(), fish.end(), fish.begin(), growth_function);
        fish.insert(fish.end(), spawn, FISH_START_TIME);
    }

    std::cout << "Done, total fish after " << days << " days: " << fish.size() << std::endl;
}

/* Solver that recursively counts the number of fish after n iterations.
 * Doesn't run out of memory but still takes a significant amount of time to solve
 * for 256 days.
 *
 * Optimised by not doing the recursion more than once per starting value.
 */
void solve_day_6_opt(int days)
{
    int64_t fish = 0;

    for (int i = 0; i <= FISH_START_TIME; ++i)
    {
        int count = std::count(DATA_6_12.begin(), DATA_6_12.end(), i);
        if (count > 0)
        {
            fish += count * fish_after(days + FISH_START_TIME - i);
        }
        std::cout << "sum: " << fish << std::endl;

    }

    std::cout << "Done, total fish after " << days << " days: " << fish << std::endl;
}

/* Second attempt at optimising.
 *
 * The idea is to keep track of the number of fish in each stage and model this
 * using a circular buffer. Iterating is then as simple as rotating the buffer
 * one step to the left, and the number of newly spawned fish is simply the
 * number of fish that we moved from the leftmost position to the rightmost, i.e.
 * the number in buffer[0] before rotating.
 *
 * As newly spawned fish start at 8 and not 6, we need a special buffer to
 * keep track of them before they can be moved to the main buffer.
 *
 * This is basically O(n) and solves for 256 days in no time.
 */
void solve_day_6_opt_v2(int days)
{
    std::array<int64_t, 7> stages;
    std::array<int64_t, 2> pre_stages;

    std::fill(pre_stages.begin(), pre_stages.end(), 0l);
    int i = 0;
    std::generate(stages.begin(), stages.end(), [&] () {return std::count(DATA_6_12.begin(), DATA_6_12.end(), i++);});

    for (int d = 0; d < days; ++d)
    {
        int64_t new_fish = stages[0];
        std::rotate(stages.begin(), stages.begin() + 1, stages.end());
        std::rotate(pre_stages.begin(), pre_stages.begin() + 1, pre_stages.end());
        stages[6] += pre_stages[1];
        pre_stages[1] = new_fish;
    }
    int64_t total_fish = std::accumulate(stages.begin(), stages.end(), 0l) + std::accumulate(pre_stages.begin(), pre_stages.end(), 0l);

    std::cout << "Done, total fish after " << days << " days: " << total_fish  << std::endl;
}

int main()
{
    // Part 1
    solve_day_6(80);
    // Part 2
    solve_day_6_opt_v2(256);
    return 0;
}