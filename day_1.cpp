#include <iostream>
#include <numeric>
#include <vector>
#include "data.h"

int main()
{
    auto& inputs = DATA_1_21;
    std::array<int, inputs.size()> diffs;
    std::adjacent_difference(inputs.begin(), inputs.end(), diffs.begin());
    int incs = std::accumulate(diffs.begin(), diffs.end(), -1 ,[] (const auto& a, const auto& b) {return a + (b > 0);});
    std::cout << "Depth increases " << incs << " times." << std::endl;

    return 0;
}
