#pragma once

#include <vector>
#include <utility>

using Display = std::vector<int>;
using XPoint = std::pair<int,int>;

constexpr auto GXcopy = 1;

using Window = std::vector<int>;
using Colormap	  = std::vector<int>;
using GC		  = std::vector<int>;
using XGCValues	  = std::vector<int>;
using XFontStruct = std::vector<int>;
using Drawable = std::vector<int>;
using Pixmap = std::vector<int>;
