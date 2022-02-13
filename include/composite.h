#ifndef _COMPOSITE_INCLUDED_
#define _COMPOSITE_INCLUDED_
#include <GL/gl.h>
#include <vector>

#include "pose.h"
#include "sprite.h"

namespace simplecpp {
class Composite : public Sprite {
    vector<Sprite *> parts;

  public:
    Composite(double x, double y, Composite *owner) : Sprite(x, y, owner) {}
    void addPart(Sprite *p) { parts.push_back(p); }

    void paint(Pose *p) {
        if (visible) {
            for (size_t i = 0; i < parts.size(); i++) {
                if (p) {
                    Pose r = Pose(*p, pose);
                    parts[i]->paint(&r);
                } else {
                    parts[i]->paint(&pose);
                }
            }

            glFlush();
            glutMainLoopEvent();
        }
    }
};
} // namespace simplecpp
#endif
