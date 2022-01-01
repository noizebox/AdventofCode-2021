#include <vector>
#include <algorithm>
#include <iostream>

#include "data/data_day_9.h"

using Area = std::vector<std::vector<int>>;

std::pair<bool, int> calc_low_point(const Area& area, int x, int y)
{
    int v = area[y][x];
    int up = y > 0 ?                        area[y - 1][x] : 10;
    int down = y < (area.size() - 1) ?      area[y + 1][x] : 10;
    int left = x > 0 ?                      area[y][x - 1] : 10;
    int right = x < (area[0].size() - 1) ?  area[y][x + 1] : 10;

    return {v < up && v < down && v < left && v < right, v + 1};
}

int char_to_int(char c)
{
    return c - '0';
}

Area decode_data()
{
    std::string_view str_data = DATA_9_12;
    Area area;
    int end_of_line = 0;

    while (end_of_line != std::string_view::npos)
    {
        end_of_line = str_data.find('\n');
        auto line = str_data.substr(0, end_of_line);
        auto& d = area.emplace_back(line.size());
        std::transform(line.begin(), line.end(), d.begin(), [](auto a) {return char_to_int(a);});

        str_data = str_data.substr(end_of_line + 1);
    }
    return area;
}

void solve_part_1(const Area& area)
{
    int sum = 0;
    for (int y = 0; y < area.size(); ++y)
    {
        for (int x = 0; x < area[0].size(); ++x)
        {
            auto [low_point, height] = calc_low_point(area, x, y);
            if (low_point)
                sum += height;
        }
    }
    std::cout << "Total height: " << sum << std::endl;
}

int main()
{
    auto data = decode_data();
    solve_part_1(data);
    return 0;
}