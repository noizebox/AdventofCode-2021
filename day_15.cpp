#include <iostream>
#include <algorithm>
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

struct Node2
{
    Node* node;
    int distance;
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

// Pretty efficient Dijkstra impl. Solves part 2 in ~40s on a 6 year old intel laptop.
template <size_t size>
void solve_opt(Array2D<Node, size>& map)
{
    map[0][0].distance = 0;

    std::vector<Node2> unvisited;
    unvisited.reserve(map.size() * map.size());
    std::for_each(map.begin(), map.end(), [&] (auto& i) {std::for_each(i.begin(), i.end(), [&] (auto& node) {unvisited.push_back({&node, node.distance});});});

    auto unvisited_front = unvisited.begin();

    while (unvisited_front != unvisited.end())
    {
        // Sorting the entire unvisited set is a waste. We just need the single closest node in every iteration.
        auto nearest = std::min_element(unvisited_front, unvisited.end(), [](const auto& a, const auto& b) {return a.distance < b.distance;});
        Node2& node = *nearest;
        node.node->visited = true;

        for (auto n : node.node->neighbours)
        {
            if (!n->visited && node.node->distance + n->cost < n->distance)
            {
                n->distance = node.node->distance + n->cost;
                auto base_node = std::find_if(unvisited_front, unvisited.end(), [&](Node2& i) {return i.node == n;});
                if (base_node != unvisited.end())
                    base_node->distance = n->distance;
            }
        }

        // Don't remove visited nodes, just move them to the front and work on the rest.
        if (nearest != unvisited_front)
        {
            std::iter_swap(nearest, unvisited_front);
        }
        unvisited_front++;

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
    solve_opt(*map_2);

    return 0;
}