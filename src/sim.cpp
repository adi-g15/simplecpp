#include <queue>

#include "sim.h"

namespace simplecpp {

double sim::time = 0;
std::priority_queue<sim::ETpair, vector<sim::ETpair>, sim::compareETpair>
    sim::pq;
} // namespace simplecpp
