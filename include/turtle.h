#ifndef _TURTLE_INCLUDED_
#define _TURTLE_INCLUDED_

#include "polygon.h"

using namespace std;

namespace simplecpp{
class Turtle : public Polygon{

public:
  Turtle(Composite* owner=nullptr);
};
}

#endif



