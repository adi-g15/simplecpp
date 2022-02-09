#ifndef _SIM_INCLUDED_
#define _SIM_INCLUDED_

#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>
#include <queue>
#include <vector>

using std::cout;
using std::vector;

namespace simplecpp {

class sim {
    using Event = std::function<void()>;
    using ETpair = std::pair<Event, double>;

    struct compareETpair {
        bool operator()(ETpair p, ETpair q) { return p.second > q.second; }
    };
    static double time;
    static std::priority_queue<ETpair, vector<ETpair>, compareETpair> pq;

  public:
    static void post(double dT, Event e) { pq.push(make_pair(e, time + dT)); }
    static double getTime() { return time; }
    static void processAll(double tmax = HUGE_VAL) {
        while (!pq.empty() && time < tmax) {
            ETpair ETp = pq.top();
            time = ETp.second;
            pq.pop();
            ETp.first();
        }
    }
    static std::ostream &log() {
        cout << time << ") ";
        return cout;
    }
    //  friend class Resource;
};

class Resource {
    typedef std::function<void()> Event;
    //  typedef  sim::Event Event;
    std::queue<Event> q;
    bool inUse;

  public:
    Resource() { inUse = false; };
    int size() { return q.size(); }
    bool reserve() {
        if (inUse)
            return false;
        else {
            inUse = true;
            return true;
        }
    }
    void acquire(Event e) {
        if (!inUse) {
            inUse = true;
            sim::post(0, e);
        } else
            q.push(e);
    }
    void release() {
        if (!q.empty()) {
            inUse = true;
            Event e = q.front();
            q.pop();
            sim::post(0, e);
        } else
            inUse = false;
    }
};
} // namespace simplecpp

#endif
