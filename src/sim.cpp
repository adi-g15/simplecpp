#include "sim.h"

namespace simplecpp{

double sim::time = 0;
priority_queue< sim::ETpair, vector<sim::ETpair>, sim::compareETpair> sim::pq;
}
