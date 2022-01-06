#include <vector>
#include <algorithm>
#include <iostream>
#include <string_view>

#include "data/data_day_13.h"

template <typename T>
class Vector2D
{
public:
    Vector2D(int x, int y) : _x_size(x), _y_size(y), _data(x * y, 0) {}

    int x_size() const {return _x_size;}
    int y_size() const {return _y_size;}

    std::vector<T>& data() {return _data;}
    T& at(int x, int y) {return _data.at(x + _x_size * y);}
    const T& at(int x, int y) const {return _data.at(x + _x_size * y);}
    auto begin() const {return _data.begin();};
    auto begin() {return _data.begin();};
    auto end() const {return _data.end();}
    auto end() {return _data.end();}

private:
    int _x_size;
    int _y_size;
    std::vector<T> _data;
};

Vector2D<int> parse_data()
{
    int x_max = std::max_element(DATA_13_12.begin(), DATA_13_12.end(), [] (auto& a, auto& b) {return a.first < b.first;})->first;
    int y_max = std::max_element(DATA_13_12.begin(), DATA_13_12.end(), [] (auto& a, auto& b) {return a.second < b.second;})->second;

    Vector2D<int> data(x_max + 1, y_max + 1);
    for (const auto& point : DATA_13_12)
    {
        data.at(point.first, point.second) = 1;
    }
    return data;
}

Vector2D<int> fold(Vector2D<int> data, char direction, int distance)
{
    if (direction == 'y')
    {
        Vector2D<int> folded(data.x_size(), distance);
        std::copy(data.begin(), data.begin() + (folded.x_size() * distance), folded.begin());
        for (int y = 1; y < data.y_size() - distance; ++y)
        {
            for (int x = 0; x < data.x_size(); ++x)
            {
                if (data.at(x, distance + y) == 1)
                    folded.at(x, distance - y) = 1;
            }
        }
        return folded;
    }
    else
    {
        Vector2D<int> folded(distance, data.y_size());
        for (int y = 0; y < data.y_size() ; ++y)
        {
            std::copy(&data.at(0, y), &data.at(0, y) + distance, &folded.at(0,y));
            for (int x = 1; x < data.x_size() - distance; ++x)
            {
                if (data.at(distance + x, y) == 1)
                    folded.at(distance - x, y) = 1;
            }
        }
        return folded;
    }
}

void solve_part_1(const Vector2D<int>& data)
{
    auto folded = fold(data, 'x', 655);
    std::cout << "Part 1: " << std::count(folded.begin(), folded.end(), 1) << " dots" << std::endl;
}

void solve_part_2(Vector2D<int> data)
{
    for (const auto& p : FOLD_13_12)
    {
        data = fold(data, p.first, p.second);
    }
    std::string code((data.x_size() + 1) * data.y_size(), '\n');
    for (int y = 0; y < data.y_size(); ++y)
    {
        std::transform(&data.at(0,y), &data.at(0,y) + data.x_size(), code.begin() + y * (data.x_size() + 1), [] (auto a) {return a == 0 ? ' ' : a == 1 ? '#' : a;});
    }
    std::cout << code;
}

int main()
{
    auto data = parse_data();
    solve_part_1(data);
    solve_part_2(data);
    return 0;
}

