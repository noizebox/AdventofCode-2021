#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>
#include <queue>

#include "data/data_day_15.h"

constexpr size_t DATA_SIZE = 100;

template <typename T, size_t size>
using Array2D = std::array<std::array<T, size>, size>;
struct Node
{
    int cost{};
    int distance{std::numeric_limits<int>::max()};
    bool visited{false};
    std::vector<Node*> neighbours;
};

template <size_t size>
void parse_map(Array2D<Node, size>& map, const std::array<const char*, DATA_SIZE>& data)
{
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map.size(); ++j)
        {
            Node& node = map[i][j];
            node.cost = ((data[i % data.size()][j % data.size()] - '0') + i / data.size() + j / data.size());
            while (node.cost > 9)
                node.cost -= 9;
            if (i < map.size() - 1)
                node.neighbours.push_back(&map[i + 1][j]);
            if (j < map.size() - 1)
                node.neighbours.push_back(&map[i][j + 1]);
            if (i > 0)
                node.neighbours.push_back(&map[i - 1][j]);
            if (j > 0)
                node.neighbours.push_back(&map[i][j - 1]);
        }
    }
}


/* Solves part 1 very fast, but takes some shortcuts that makes it unable to
 * solve part 2 correctly */
template <size_t size>
void solve(Array2D<Node, size>& map)
{
    map[0][0].distance = 0;

    // Lambdas nested 3 layers deep ;)
    std::for_each(map.begin(), map.end(), [] (auto& i)
        {std::for_each(i.begin(), i.end(), [] (auto& node)
            {std::for_each(node.neighbours.begin(), node.neighbours.end(), [&] (auto& n)
                {if (!n->visited)
                    {
                        n->distance = std::min(n->distance, node.distance + n->cost);
                    };
                node.visited = true;
                });
            });
        });

    std::cout << "Shortest total distance: " << map.back().back().distance << std::endl;
}

struct Node2
{
    Node* node;
    int distance;
};

template <size_t size>
void solve_opt2(Array2D<Node, size>& map)
{
    map[0][0].distance = 0;

    // By keeping distance in the temporary node, memory access is much more cache friendly when sorting
    std::vector<Node2> unvisited;
    unvisited.reserve(map.size() * map.size());
    std::for_each(map.begin(), map.end(), [&] (auto& i) {std::for_each(i.begin(), i.end(), [&] (auto& node) {unvisited.push_back({&node, node.distance});});});

    std::reverse(unvisited.begin(), unvisited.end());

    while (!unvisited.empty())
    {
        auto node = unvisited.back();
        unvisited.pop_back();

        if (unvisited.size() % 10000 == 0)
        {
            std::cout << unvisited.size() <<std::endl;
        }

        for (auto n : node.node->neighbours)
        {
            if (!n->visited)
            {
                if (node.node->distance + n->cost < n->distance)
                {
                    n->distance = node.node->distance + n->cost;
                    auto base_node = std::find_if(unvisited.begin(), unvisited.end(), [&](Node2& i) {return i.node == n;});
                    if (base_node != unvisited.end())
                        base_node->distance = n->distance;
                }
            }
        }
        node.node->visited = true;
        std::sort(unvisited.begin(), unvisited.end(), [] (const auto& a, const auto& b) {return (a.distance > b.distance);});
    }
    std::cout << "Shortest total distance: " << map.back().back().distance << std::endl;
}


int main()
{
    auto map = std::make_unique<Array2D<Node, 100>>();
    parse_map(*map, DATA_15_12);
    solve(*map);

    std::cout << "Part 2: " << std::endl;
    auto map_2 = std::make_unique<Array2D<Node, 500>>();
    parse_map(*map_2, DATA_15_12);
    solve_opt2(*map_2);

    return 0;
}