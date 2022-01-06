#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string_view>
#include <map>

#include "data/data_day_12.h"

using Path = std::vector<std::string>;

/* Build a map of all caves and their adjacent caves */
std::map<std::string, Path> build_map()
{
    std::map<std::string, Path> caves;
    for (std::string_view pair : DATA_12_12)
    {
        int split_point = pair.find('-');
        std::string id_1(pair.substr(0, split_point));
        std::string id_2(pair.substr(split_point + 1));
        auto& cave_1 = caves[id_1];
        cave_1.push_back(id_2);
        auto& cave_2 = caves[id_2];
        cave_2.push_back(id_1);
    }
    return caves;
}

/* Returns the number of valid paths, starting from 'current' */
int recursive_path_search(std::map<std::string, Path>& map,
                           const std::string& current,
                           int small_cave_double_visits,
                           Path& path)
{
    if (current == "end")
    {
        return 1;
    }
    if (islower(current[0]))
    {
        if (std::count(path.begin(), path.end(), current) > 0)
        {
            if (--small_cave_double_visits < 0 || current == "start")
                return 0;
        }
    }
    path.push_back(current);
    int sum = 0;
    for (const auto& next : map[current])
    {
        sum += recursive_path_search(map, next, small_cave_double_visits, path);
    }
    path.pop_back();
    return sum;
}

void solve_part_1(std::map<std::string, Path>& caves)
{
    Path path;
    std::cout << "Part 1, found " << recursive_path_search(caves, "start", 0, path) << " paths" << std::endl;
}

void solve_part_2(std::map<std::string, Path>& caves)
{
    Path path;
    std::cout << "Part 2, found " << recursive_path_search(caves, "start", 1, path) << " paths" << std::endl;
}

int main()
{
    auto map = build_map();
    solve_part_1(map);
    solve_part_2(map);
    return 0;
}