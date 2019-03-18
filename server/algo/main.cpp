
#include "main.h"
using namespace std;
// int fileID = 1;

point_t step[4] = { { 0,1 },{ 1,0 },{ 0,-1 },{ -1,0 } };
point_t no_pos = { -1,-1 };



const int N = 3003;
const int K = 65;

int light[N][N][3];
point_t target[K];
std::set<point_t> targets;
int n, m; //map size
int k; // number of targets

int best;
std::vector<point_t> route, best_route;
std::vector<std::vector<point_t>> best_route_vec;

FILE f;


int pass_time(point_t p, int cur_time)
{
    if (cur_time == 0) {
        return 30;
    }
    int tg = light[p.x][p.y][0];
    int tr = light[p.x][p.y][1];
    int tp = light[p.x][p.y][2];
    int valid_time = tg - cur_time % (tg + tr);
    if (valid_time >= tp) {
        return cur_time + tp + 30;
    }
    else {
        return (cur_time / (tg + tr) + 1) * (tg + tr) + tp + 30;
    }
}

point_t dijkstra(priority_queue<state_t> & q, int dist[][N], int pre[][N], set<point_t> & targets)
{
    while (!q.empty()) {
        auto state = q.top();
        q.pop();
        auto p = state.pos;
        auto& d = dist[p.x][p.y];
        if (d >= 0) {
            continue;
        }
        d = state.time;
        pre[p.x][p.y] = state.from;
        int next_time = pass_time(state.pos, state.time);
        for (int i = 0; i < 4; ++i) {
            auto next_pos = state.pos + step[i];
            if (next_pos.x <= 0 || next_pos.x > n
                || next_pos.y <= 0 || next_pos.y > m
                || dist[next_pos.x][next_pos.y] >= 0) {
                continue;
            }
            q.push({ next_time, i, next_pos });
        }
        if (targets.find(state.pos) != targets.end()) {
            return state.pos;
        }
    }
    return no_pos;
}

void append_route(point_t end, int pre[][N])
{
    pair<int, int> a, b;
    stack<point_t> reverse;
    while (pre[end.x][end.y] != -1) {
        reverse.push(end);
        end = end - step[pre[end.x][end.y]];
    }
    while (!reverse.empty()) {
        route.push_back(reverse.top());
        reverse.pop();
    }
}

void erase_route(point_t start)
{
    while (!route.empty() && route.back() != start) {
        route.pop_back();
    }
}

void back_to_start(state_t cur)
{
    auto dist = new int[N][N];
    auto pre = new int[N][N];
    memset(dist, -1, sizeof(int) * N * N);
    priority_queue<state_t> q;
    q.push(cur);
    
    targets.insert(target[0]);
    point_t next = dijkstra(q, dist, pre, targets);
    append_route(target[0], pre);
    cur.time = dist[target[0].x][target[0].y];
    if (cur.time < best) {
        best = cur.time;
        best_route = route;
        cout << best << ' ' << clock() << endl;
        
        best_route_vec.clear();
        best_route_vec.push_back(best_route);
        
    }else if (cur.time == best) {
        best = cur.time;
        best_route = route;
        cout << best << ' ' << clock() << endl;
        
        best_route_vec.push_back(best_route);
    }
    erase_route(cur.pos);
    targets.erase(target[0]);
    delete[] dist;
    delete[] pre;
}

void dfs(state_t cur, set<point_t> & targets)
{
    if (targets.empty()) {
        back_to_start(cur);
        return;
    }
    int min_dist = INT_MAX;
    for (auto p : targets) {
        for (auto q : targets) {
            min_dist = min(min_dist, 30 * (abs(p.x - q.x) + abs(p.y - q.y)));
        }
    }
    if (cur.time + targets.size() * min_dist > best) {
        return;
    }
    auto dist = new int[N][N];
    auto pre = new int[N][N];
    memset(dist, -1, sizeof(int) * N * N);
    priority_queue<state_t> q;
    q.push(cur);
    point_t next;
    while ((next = dijkstra(q, dist, pre, targets)) != no_pos) {
        targets.erase(next);
        append_route(next, pre);
        dfs({ dist[next.x][next.y], -1, next }, targets);
        erase_route(cur.pos);
        targets.insert(next);
    }
    delete[] dist;
    delete[] pre;
}

void read(char* path)
{
    auto file = fopen(path, "r");
    fscanf(file, "%d%d", &n, &m);
    fscanf(file, "%d%d", &target[0].x, &target[0].y);
    fscanf(file, "%d", &k);
    for (int i = 1; i <= k; ++i) {
        fscanf(file, "%d%d", &target[i].x, &target[i].y);
        targets.insert(target[i]);
    }
    for (int i = 0; i < n; ++i) {
        for (int i = 0; i < m; ++i) {
            int x, y;
            fscanf(file, "%d%d", &x, &y);
            for (auto& t : light[x][y]) {
                fscanf(file, "%d", &t);
            }
        }
    }
}

string deliveryGo(char* dataPath) {
    std::stringstream ss;
    char res_path[100] = "res.txt";
    read(dataPath);
    best = INT_MAX;
    route.push_back(target[0]);
    cout << clock() << endl;
    dfs({ 0,-1,target[0] }, targets);
    int cur_time = 0;
    vector<int> vec_time;
    route = best_route_vec.back();
    vec_time.push_back(0);
    for (int i = 0; i < route.size(); ++i) {
        if (i == route.size() - 1) {
            break;
        }
        cur_time = pass_time(route[i], cur_time);
        vec_time.push_back(cur_time);
    }
  
    for (auto best_route : best_route_vec) {
        int i = 0;
        ss<<best<<"\n";
        for (auto p : best_route) {
            ss<< p.x << " "<<p.y <<" "<< vec_time[i++]<<"\n";
        }
    }
    
    return ss.str();
}




int main(int argc, char* argv[])
{

    char dataPath[200] = "/Users/cheriewang/git/hackthon_sever/app/algo/datafile.txt";

    read(dataPath);
    best = INT_MAX;
    string res= deliveryGo(dataPath);
    cout<<"result"<<endl;
    cout<<res;
    route.push_back(target[0]);

    dfs({ 0,-1,target[0] }, targets);
    

    
    
    
}