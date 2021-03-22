#ifndef _TURTLE_INCLUDED_
#define _TURTLE_INCLUDED_

#include <common_def.h>
#include <sprite.h>
#include <vector>

using namespace std;

namespace simplecpp{
class Turtle : public Polygon{

public:
  Turtle(Composite* owner=NULL);
};
}

#endif



