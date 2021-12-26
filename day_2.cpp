#include <iostream>
#include <numeric>
#include <algorithm>
#include <string_view>

#include "data/data_day_2.h"

class Movement
{
public:
    Movement() : _distance(0), _depth(0) {}

    explicit Movement(const char* str)
    {
        auto view = std::string_view(str);
        auto split = view.find(" ");
        int num = atoi(view.substr(split).data());
        auto cmd_str = view.substr(0, split);
        if (cmd_str == "forward")
        {
            _distance = num;
            _depth = 0;
        }
        if (cmd_str == "down")
        {
            _distance = 0;
            _depth = num;
        }
        if (cmd_str == "up")
        {
            _distance = 0;
            _depth = -num;
        }
    }

    int distance() const {return _distance;}
    int depth() const {return _depth;};
    int code() const {return _depth * _distance;}

protected:
    int _distance;
    int _depth;

    friend Movement operator+(const Movement& a, const Movement& b)
    {
        Movement m;
        m._distance = a._distance + b._distance;
        m._depth = a._depth + b._depth;
        return m;
    }
};

struct MovementExt : public Movement
{
public:
    MovementExt() : Movement(), _aim(0) {}
    int aim() const {return _aim;}

private:
    int _aim;

    friend MovementExt operator+(const MovementExt& a, const Movement& b)
    {
        MovementExt m;
        if (b.depth() != 0)
        {
            m._aim = a._aim + b.depth();
            m._distance = a._distance;
            m._depth = a._depth;
        }
        else if (b.distance() != 0)
        {
            m._distance = a._distance + b.distance();
            m._depth = a._depth + a._aim * b.distance();
            m._aim = a._aim;
        }
        return m;
    }
};


void solve_day_2()
{
    // Part 1
    auto& inputs = DATA_2_12;
    Movement final = std::accumulate(inputs.begin(), inputs.end(), Movement(), [] (const Movement& a, const char* b) {return a + Movement(b);});
    std::cout << "Final pos is dist: " << final.distance() << ", depth: " << final.depth() << ", code " << final.code() << std::endl;

    // Part 2
    MovementExt final_2 = std::accumulate(inputs.begin(), inputs.end(), MovementExt(), [] (const MovementExt& a, const char* b) {return a + Movement(b);});
    std::cout << "Final pos (pt 2) is dist: " << final_2.distance() << ", depth: " << final_2.depth() << ", aim: " <<
              final_2.aim() << ", code " << final_2.code() << std::endl;

}

int main()
{
    solve_day_2();
    return 0;
}
