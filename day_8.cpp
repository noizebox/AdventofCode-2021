#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <string>
#include <string_view>
#include <map>

#include "data/data_day_8.h"

/* These are the chars that can be uniquely identified by their size */
constexpr std::array<std::pair<char, int>, 4> FIXED_SIZE_CHARS = {{{'1', 2}, {'4', 4}, {'7',3}, {'8', 7}}};

struct Data
{
    std::vector<std::string> codes;
    std::vector<std::string> input;
    std::string              output;
};

std::string decode_separated_string(std::string_view& data, char delimiter)
{
    std::string s;
    int index = data.find(delimiter);
    if (index != std::string_view::npos)
    {
        s = std::string(data.begin(), data.begin() + index);
        data = data.substr(index + 1);
    }
    else
    {
        s = std::string(data);
    }
    /* Sort the string so that we can compare strings later */
    std::sort(s.begin(), s.end());
    return s;
}

std::vector<Data> decode_data()
{
    std::string_view str_data = DATA_8_12;
    std::vector<Data> data;
    int end_of_line = 0;

    while (end_of_line != std::string_view::npos)
    {
        end_of_line = str_data.find('\n');

        auto line = str_data.substr(0, end_of_line);
        auto& d = data.emplace_back();
        for (int i = 0; i < 10 ; ++i)
        {
            d.codes.push_back(decode_separated_string(line, ' '));
        }
        // throw away the '|' and decode the rest space-separated words as input
        decode_separated_string(line, ' ');
        for (int i = 0; i < 4 ; ++i)
        {
            d.input.push_back(decode_separated_string(line, ' '));
        }

        str_data = str_data.substr(end_of_line + 1);
    }
    return data;
}

void solve_part_1(std::vector<Data> data)
{
    for (auto& d : data)
    {
        /* Build solver */
        std::map<std::string, char> solver;
        for (const auto& s : d.codes)
        {
            for (const auto keys : FIXED_SIZE_CHARS)
            {
                if (s.size() == keys.second)
                    solver[s] = keys.first;
            }
        }
        /* Use solver */
        for (const auto& s : d.input)
        {
            auto i = solver.find(s);
            if (i != solver.end())
                d.output.push_back(i->second);
        }
    }

    int sum = std::accumulate(data.begin(), data.end(), 0, [] (auto& a, auto& b) {return a + b.output.size();});
    std::cout << "Total fixed size chars: " << sum << std::endl;
}

void solve_part_2(std::vector<Data> data)
{
    for (auto& d : data)
    {
        /* Build solver, first add the easy ones with fixed lengths */
        std::map<std::string, char> solver;
        std::map<char, std::string> reverse_map;
        for (const auto& s : d.codes)
        {
            for (const auto keys : FIXED_SIZE_CHARS)
            {
                if (s.size() == keys.second)
                {
                    solver[s] = keys.first;
                    reverse_map[keys.first] = s;
                }
            }
        }
        std::string buf(10, ' ');
        /* Then deduce the rest by comparing how many segments differ with the fixed length codes already decoded. */
        for (const auto& s : d.codes)
        {
            int diff_7 = std::set_difference(s.begin(), s.end(), reverse_map['7'].begin(), reverse_map['7'].end(), buf.begin()) - buf.begin();
            int diff_4 = std::set_difference(s.begin(), s.end(), reverse_map['4'].begin(), reverse_map['4'].end(), buf.begin()) - buf.begin();

            switch (s.size())
            {
                case 5:
                    /* There are 3 digits with 5 segments: 2, 3, and 5. 3 differ from 7 with 2 segments, while
                     * 2 and 5 differ with 3 segments. And 5 differs from 4 with 2 segments while 2 differs by 3 */
                    solver[s] = diff_7 == 2 ? '3' : diff_4 == 2 ? '5' : '2';
                    break;

                case 6:
                    /* The 6 segment digits are 0, 6, and 9. */
                    solver[s] = diff_7 == 4 ? '6' : diff_4 == 2 ? '9' : '0';
                    break;
            }
        }
        /* Run the solver */
        for (const auto& s : d.input)
        {
            auto i = solver.find(s);
            if (i != solver.end())
                d.output.push_back(i->second);
        }

    }
    int sum = std::accumulate(data.begin(), data.end(), 0, [] (auto& a, auto& b) {return a + atoi(b.output.c_str());});
    std::cout << "Total of all decoded numbers: " << sum << std::endl;
}

int main()
{
    auto data = decode_data();
    solve_part_1(data);
    solve_part_2(data);
    return 0;
}