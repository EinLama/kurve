#ifndef KURVE_ENGINE_H
#define KURVE_ENGINE_H

#include <allegro5/allegro.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

#define DIR_RIGHT 0
#define DIR_DOWN_RIGHT 45
#define DIR_DOWN 90
#define DIR_DOWN_LEFT 135
#define DIR_LEFT 180
#define DIR_UP_LEFT 225
#define DIR_UP 270
#define DIR_UP_RIGHT 315

enum KURVE_KEYS {
  KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
  KEY_A, KEY_S
};

class Point {
  public:
    double x, y;
    Point (double _x, double _y) { x = _x; y = _y; }
};

#endif
