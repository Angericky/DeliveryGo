// #pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <chrono>
#include <set>
#include <queue>
#include <sstream>

struct point_t
{
    int x, y;
    point_t operator+(const point_t rhs) const
    {
        return { x + rhs.x, y + rhs.y };
    }
    point_t operator-(const point_t rhs) const
    {
        return { x - rhs.x, y - rhs.y };
    }
    bool operator!=(const point_t rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }
    bool operator<(const point_t rhs) const
    {
        return std::make_pair(x, y) < std::make_pair(rhs.x, rhs.y);
    }
};

struct state_t
{
    int time, from;
    point_t pos;
    bool operator<(const state_t rhs) const
    {
        return time > rhs.time;
    }
};
std::string  deliveryGo(char* dataPath);//,std::string res_path);


