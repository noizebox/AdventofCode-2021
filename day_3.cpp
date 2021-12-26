#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string_view>
#include <cmath>
#include "data/data_day_3.h"

int binary_count(std::vector<int> bits)
{
    int sum = 0;
    for (int i = 0; i < bits.size(); ++i)
    {
        sum += bits[i] * static_cast<int>(std::round(std::pow(2, i)));
    }
    return sum;
}

void solve_day_2()
{
    auto& inputs = DATA_3_12;
    int n_bits = std::string_view(inputs[0]).size();
    std::vector<int> bits;
    for (int i = 0; i < n_bits; ++i)
    {
        bits.push_back(std::accumulate(inputs.begin(), inputs.end(), 0, [&] (auto& a, auto& b) {return a + (b[i] - '0');}) * 2 / inputs.size() );
    }
    std::reverse(bits.begin(), bits.end());
    int gamma_rate = binary_count(bits);

    std::transform(bits.begin(), bits.end(), bits.begin(), [] (auto v) {return v == 1 ? 0 : 1;});
    int epsilon_rate = binary_count(bits);

    std::cout << "gamma rate: " << gamma_rate << ", epsilon rate: " << epsilon_rate << ", prod: " << gamma_rate * epsilon_rate << std::endl;
}




int main()
{
    solve_day_2();
    return 0;
}