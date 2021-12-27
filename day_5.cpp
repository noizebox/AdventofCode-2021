#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <memory>

#include "data/data_day_5.h"

constexpr int WORLD_SIZE = 1000;

struct Point
{
    int x;
    int y;
};

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point& a, const Point& b)
{
    return !(a == b);
}

class Line
{
public:
    Line(const std::array<int, 4>& points) : _point_1{points[0], points[1]},
                                             _point_2{points[2], points[3]} {}

    Line() : _point_1{0 , 0}, _point_2{0, 0} {}

    bool is_vertical() const {return _point_1.y == _point_2.y;}

    bool is_horisontal() const {return _point_1.x == _point_2.x;}

    const Point& start() const {return _point_1;}

    const Point& end() const {return _point_2;}

private:
    Point _point_1;
    Point _point_2;
};

template<int size>
class World
{
public:
    World()
    {
        for (auto& row : _pixels)
            row.fill(0);
    }

    void draw_line(const Line& line)
    {
        Point start = line.start();
        Point end = line.end();
        while (start != end)
        {
            _pixels[start.x][start.y]++;

            if (start.x < end.x)
                start.x++;

            else if (start.x > end.x)
                start.x--;

            if (start.y < end.y)
                start.y++;

            else if (start.y > end.y)
                start.y--;
        }
        // Draw end pixel
        _pixels[start.x][start.y]++;
    }

    int count_pixels_greater_than(int limit)
    {
        // Kinda ugly to assume that arrays are laid out in memory without padding bits.
        return std::count_if(_pixels.front().begin(), _pixels.back().end(), [&] (auto v) {return v > limit;});
    }

private:
    std::array<std::array<int, size>, size> _pixels;
};

void solve_day_5()
{
    auto world = std::make_unique<World<WORLD_SIZE>>();
    std::vector<Line> lines(DATA_5_12.begin(), DATA_5_12.end());
    for (const auto line : lines)
    {
        if (line.is_horisontal() || line.is_vertical())
        {
            world->draw_line(line);
        }
    }
    std::cout << "Total pixels overlapping: " << world->count_pixels_greater_than(1) << std::endl;

    // Part 2 - now just draw the diagonal lines and we're done
    for (const auto line : lines)
    {
        if (!line.is_horisontal() && !line.is_vertical())
        {
            world->draw_line(line);
        }
    }
    std::cout << "Total pixels overlapping (including diagonal lines): " << world->count_pixels_greater_than(1) << std::endl;
}

int main()
{
    solve_day_5();
    return 0;
}