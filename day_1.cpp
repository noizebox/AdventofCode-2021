#include <iostream>
#include <numeric>
#include <vector>
#include "data.h"

int main()
{
    // Lambda for counting positive values
    auto sum_if_pos = [] (const int& a, const int& b) {return a + (b > 0);};

    // Solve part 1
    auto& inputs = DATA_1_21;
    std::array<int, inputs.size()> diffs;
    std::adjacent_difference(inputs.begin(), inputs.end(), diffs.begin());
    int incs = std::accumulate(diffs.begin(), diffs.end(), -1 , sum_if_pos);
    std::cout << "Depth increases " << incs << " times." << std::endl;

    // Solve part 2, note we can fill diffs with the sliding sums and the reuse it for the adjacent diffs
    diffs.fill(0);
    for (int i = 0; i < inputs.size() - 2; ++i)
    {
        diffs[i] = std::accumulate(inputs.begin() + i, inputs.begin() + i + 3, 0);
    }
    std::adjacent_difference(diffs.begin(), diffs.end(), diffs.begin());
    incs = std::accumulate(diffs.begin(), diffs.end(), -1 , sum_if_pos);
    std::cout << "Sliding windows (3) depth increases " << incs << " times." << std::endl;

    return 0;
}
