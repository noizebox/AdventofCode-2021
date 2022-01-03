#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <memory>

#include "data/data_day_11.h"

constexpr int MAX_ENERGY = 9;
constexpr int GRID_SIZE = 10;
constexpr int ITERATIONS = 100;

int char_to_int(char c)
{
    return c - '0';
}

class Octopus
{
public:
    Octopus(int start_energy) : _energy(start_energy), _flashed(false) {}

    void add_neighbour(Octopus* neighbour)
    {
        _neighbours.push_back(neighbour);
    }

    void tick()
    {
        if (!_flashed && ++_energy > MAX_ENERGY)
            _flash();
    }

    bool has_flashed(bool reset)
    {
        bool flashed = _flashed;
        if (reset)
            _flashed = false;
        return flashed;
    }

private:
    void _flash()
    {
        _flashed = true;
        _energy = 0;
        std::for_each(_neighbours.begin(), _neighbours.end(), [] (auto& a) {a->tick();});
    }

    int  _energy;
    bool _flashed;
    std::vector<Octopus*> _neighbours;
};

void add_neighbours(std::vector<std::unique_ptr<Octopus>>& octas)
{
    for (int y = 0; y < GRID_SIZE; ++y)
    {
        for (int x = 0; x < GRID_SIZE; ++x)
        {
            // Add neighbouring octopuses clock-wise, starting at 12 o clock.
            Octopus* octopus = octas.at(x + GRID_SIZE * y).get();
            if (y > 0)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y - 1) + x).get());
            if (x < GRID_SIZE - 1 && y > 0)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y - 1) + x + 1).get());
            if (x < GRID_SIZE - 1)
                octopus->add_neighbour(octas.at(GRID_SIZE * y + x + 1).get());
            if (x < GRID_SIZE - 1 && y < GRID_SIZE - 1)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y + 1) + x + 1).get());
            if (y < GRID_SIZE - 1)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y + 1) + x).get());
            if (x > 0 && y < GRID_SIZE - 1)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y + 1) + x - 1).get());
            if (x > 0)
                octopus->add_neighbour(octas.at(GRID_SIZE * y + x - 1).get());
            if (x > 0 && y > 0)
                octopus->add_neighbour(octas.at(GRID_SIZE * (y - 1) + x - 1).get());
        }
    }
}

void solve_day_11()
{
    std::vector<std::unique_ptr<Octopus>> octas;
    for (auto c : std::string_view(DATA_11_12))
    {
        octas.push_back(std::make_unique<Octopus>(char_to_int(c)));
    }
    add_neighbours(octas);

    int total_flashes = 0;
    int iters = 0;

    while(++iters)
    {
        std::for_each(octas.begin(), octas.end(), [] (auto& a) {a->tick();});
        int flashes = std::accumulate(octas.begin(), octas.end(), 0, [] (auto a, auto& b) {return a + (b->has_flashed(true)? 1 : 0);});
        total_flashes += flashes;

        if (iters == ITERATIONS)
        {
            std::cout << "Total flashes after " << iters << " iterations: " << total_flashes << std::endl;
        }

        if (flashes == octas.size())
        {
            break;
        }
    }

    std::cout << "All octopuses flashed at iteration: " << iters << std::endl;
}

int main()
{
    solve_day_11();
    return 0;
}