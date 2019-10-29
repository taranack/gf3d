#include "gf3d_map.h"
#include <stdlib.h>
int manhattanDist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);

}
