#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string_view>
#include <cmath>

#include "data/data_day_3.h"

// Compute the binary number from an integer representation
int binary_count(std::vector<int> bits)
{
    int sum = 0;
    for (int i = 0; i < bits.size(); ++i)
    {
        sum += bits[i] * static_cast<int>(std::round(std::pow(2, i)));
    }
    return sum;
}

// Convert string formatted binary number to a vector of integers (lsb first)
std::vector<int> to_int_bits(std::string_view view)
{
    std::vector<int> v;
    for (auto c : view)
    {
        v.push_back(c - '0');
    }
    std::reverse(v.begin(), v.end());
    return v;
}

// Filter and remove values until only 1 element remains and return that element converted to an integer
int filter_values(std::vector<std::string_view> in, bool bit_criteria)
{
    int idx = 0;
    while (in.size() > 1)
    {
        int common_bit = std::accumulate(in.begin(), in.end(), 0, [&] (auto& a, auto& b) {return a + (b[idx] - '0');}) * 2 / in.size();
        if (bit_criteria == false)
            common_bit = common_bit == 1? 0 : 1;
        in.erase(std::remove_if(in.begin(), in.end(), [&] (auto v) {return (v[idx] - '0') == common_bit;}), in.end());
        idx++;
    }

    return binary_count(to_int_bits(in.front()));
}

void solve_day_3()
{
    // Part 1
    auto& inputs = DATA_3_12;
    int n_bits = std::string_view(inputs[0]).size();
    std::vector<int> bits;
    for (int i = 0; i < n_bits; ++i)
    {
        bits.push_back(std::accumulate(inputs.begin(), inputs.end(), 0, [&] (auto& a, auto& b) {return a + (b[i] - '0');}) * 2 / inputs.size() );
    }
    // Note lsb first.
    std::reverse(bits.begin(), bits.end());
    int gamma_rate = binary_count(bits);

    std::transform(bits.begin(), bits.end(), bits.begin(), [] (auto v) {return v == 1 ? 0 : 1;});
    int epsilon_rate = binary_count(bits);

    std::cout << "gamma rate: " << gamma_rate << ", epsilon rate: " << epsilon_rate << ", prod: "
              << gamma_rate * epsilon_rate << std::endl;

    // Part 2
    int oxygen_rate = filter_values(std::vector<std::string_view>(inputs.begin(), inputs.end()), true);
    int co2_scrub_rate = filter_values(std::vector<std::string_view>(inputs.begin(), inputs.end()), false);

    std::cout << "Oxygen generator rating: " << oxygen_rate << ", co2 scrubbing rating: " << co2_scrub_rate << ", prod: "
              << oxygen_rate * co2_scrub_rate << std::endl;

}


int main()
{
    solve_day_3();
    return 0;
}