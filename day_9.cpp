#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

#include "data/data_day_9.h"

using Area = std::vector<std::vector<int>>;

constexpr int MAX_HEIGHT = 9;

// Returns {true, height} if this is a low point
std::pair<bool, int> calc_low_point(const Area& area, int x, int y)
{
    int v = area[y][x];
    int up = y > 0 ?                        area[y - 1][x] : 10;
    int down = y < (area.size() - 1) ?      area[y + 1][x] : 10;
    int left = x > 0 ?                      area[y][x - 1] : 10;
    int right = x < (area[0].size() - 1) ?  area[y][x + 1] : 10;

    return {v < up && v < down && v < left && v < right, v + 1};
}

// Fills and returns the number of pixels filled
int recursive_flood_fill(Area& area, int value, int x, int y)
{
    if (x < 0 || x >= area.size() || y < 0 || y >= area[0].size())
        return 0;

    int& current_pixel = area[y][x];

    if (current_pixel == value || current_pixel >= MAX_HEIGHT)
        return 0;

    current_pixel = value;

    return 1 + recursive_flood_fill(area, value, x + 1, y) +
               recursive_flood_fill(area, value, x, y + 1) +
               recursive_flood_fill(area, value, x - 1, y) +
               recursive_flood_fill(area, value, x, y - 1);
}

int char_to_int(char c)
{
    return c - '0';
}

// Decode the string data into a nested std::vector of ints
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

struct Point
{
    int x;
    int y;
    int size;
};
bool operator<(const Point& lhs, const Point& rhs) {return lhs.size < rhs.size;}
bool operator>(const Point& lhs, const Point& rhs) {return rhs < lhs;}

void solve_part_2(Area& area)
{
    std::vector<Point> low_points;
    /* First find all low_points */
    for (int y = 0; y < area.size(); ++y)
    {
        for (int x = 0; x < area[0].size(); ++x)
        {
            auto [low_point, height] = calc_low_point(area, x, y);
            if (low_point)
                low_points.push_back({x, y, 0});
        }
    }
    /* Do a fill around each low point and store the results */
    for (auto& p : low_points)
    {
        p.size = recursive_flood_fill(area, -1, p.x, p.y);
    }
    /* Sort in descending order */
    std::sort(low_points.begin(), low_points.end(), std::greater<>());

    int total = std::accumulate(low_points.begin(), low_points.begin() + 3, 1, [] (const auto& a, const auto& b) {return a * b.size;});
    std::cout << "Product of the 3 largest basins: " << total << std::endl;
}

int main()
{
    auto data = decode_data();
    solve_part_1(data);
    solve_part_2(data);

    return 0;
}