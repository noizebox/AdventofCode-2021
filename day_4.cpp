#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>

#include "data/data_day_4.h"

constexpr int BOARD_WIDTH = 5;
constexpr int BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;

class Board
{
public:
    explicit Board(const std::array<int, BOARD_SIZE>& numbers) : _bingo(false)
    {
        _numbers.reserve(BOARD_SIZE);
        for (int i = 0; i < numbers.size(); ++i)
        {
            _numbers.push_back({numbers[i], {i % BOARD_WIDTH, i / BOARD_WIDTH}});
        }
        _h_count.fill(0);
        _v_count.fill(0);
    }

    // Returns true if bingo
    bool mark(int number)
    {
        bool bingo = false;
        /* Yes, this is a linear search, though this is surprisingly faster than storing the
         * numbers and their coordinates in a std::map or std::unordered_map. Most likely due
         * to the fewer memory allocations and cache-friendliness of storing the bingo board
         * in one single memory allocation.
         *
         * The linear search is actually faster (though not by much) than implementing a binary
         * search with std::lower_bound. A binary search requires sorting the numbers beforehand
         * which comes at a cost that not fully recouped in cheaper lookups */

        auto node = std::find_if(_numbers.begin(), _numbers.end(), [&] (const auto& i) {return i.first == number;});
        if (node != _numbers.end())
        {
            auto& coord = node->second;
            if (++_v_count[coord.first] == BOARD_WIDTH || ++_h_count[coord.second] == BOARD_WIDTH)
            {
                _bingo = true;
                bingo = true;
            }
            // If marked numbers are removed, score becomes dead easy to count afterwards.
            _numbers.erase(node);
        }
        return bingo;
    }

    // Sum of unmarked (remaining) numbers
    int calc_score() const
    {
        return std::accumulate(_numbers.begin(), _numbers.end(), 0, [] (const auto& a, const auto& b) {return a + b.first;});
    }

    bool has_bingo() const {return _bingo;}

private:
    // Store coordinates for every value instead of the entire board
    std::vector<std::pair<int, std::pair<int, int>>> _numbers;

    // Store count of marked numbers in every row and column for quick bingo identification
    std::array<int, BOARD_WIDTH> _h_count;
    std::array<int, BOARD_WIDTH> _v_count;

    bool _bingo;
};


void solve_part_1()
{
    std::vector<Board> boards(BINGO_BOARDS.begin(), BINGO_BOARDS.end());

    for (auto number : BINGO_NUMBERS)
    {
        for (auto& board : boards)
        {
            if (board.mark(number))
            {
                std::cout << "Bingo! Score: " << board.calc_score() * number << std::endl;
                return;
            }
        }
    }
}

void solve_part_2()
{
    std::vector<Board> boards(BINGO_BOARDS.begin(), BINGO_BOARDS.end());

    for (auto number : BINGO_NUMBERS)
    {
        for (auto& board : boards)
        {
            board.mark(number);
        }
        if (boards.size() > 1)
        {
            boards.erase(std::remove_if(boards.begin(), boards.end(), [] (const auto& b) {return b.has_bingo();}), boards.end());
        }
        else if (boards.front().has_bingo())
        {
            std::cout << "1 board remaining with score: " << boards.front().calc_score() * number << std::endl;
            return;
        }
    }
}

int main()
{
    solve_part_1();
    solve_part_2();
    return 0;
}