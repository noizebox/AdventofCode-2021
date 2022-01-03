#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string_view>
#include <map>

#include "data/data_day_12.h"


using Path = std::vector<std::string>;
using PathList = std::vector<Path>;

std::map<std::string, Path> build_map()
{
    std::map<std::string, Path> caves;
    for (std::string_view node : DATA_12_12)
    {
        int split_point = node.find('-');
        std::string id_1(node.substr(0, split_point));
        std::string id_2(node.substr(split_point + 1, std::string_view::npos));
        auto& cave_1 = caves[id_1];
        cave_1.push_back(id_2);
        auto& cave_2 = caves[id_2];
        cave_2.push_back(id_1);
    }
    return caves;
}

void recursive_path_search(std::map<std::string, Path>& map,
                           const std::string& current,
                           PathList& paths,
                           int small_cave_double_visits,
                           Path path = Path())
{
    path.push_back(current);
    if (current == "end")
    {
        paths.push_back(path);
        return;
    }
    if (islower(current[0]))
    {
        if (std::count(path.begin(), path.end(), current) > 1)
        {
            if (small_cave_double_visits-- <= 0 || current == "start")
                return;
        }
    }
    for (const auto& next : map[current])
    {
        recursive_path_search(map, next, paths, small_cave_double_visits, path);
    }
}

void solve_part_1(std::map<std::string, Path>& caves)
{
    PathList paths;
    recursive_path_search(caves, "start", paths, 0);
    std::cout << "Part 1, found " << paths.size() << " paths" << std::endl;
}

void solve_part_2(std::map<std::string, Path>& caves)
{
    PathList paths;
    recursive_path_search(caves, "start", paths, 1);
    std::cout << "Part 2, found " << paths.size() << " paths" << std::endl;
}

int main()
{
    auto map = build_map();
    solve_part_1(map);
    solve_part_2(map);
    return 0;
}