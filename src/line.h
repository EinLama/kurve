#ifndef KURVE_LINE_H
#define KURVE_LINE_H

#include <math.h>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "engine.h"

class Line {
  private:
  ALLEGRO_COLOR color;
  std::vector<Point> positions;
  double THICKNESS, speed, angle;
  KURVE_KEYS left_key, right_key;
  bool alive;
  double current_x, current_y;

  public:
  Line (ALLEGRO_COLOR, KURVE_KEYS, KURVE_KEYS);
  void spawn(Point);
  void draw(void);
  void steer(double);
  void update(bool*, ALLEGRO_BITMAP*);
  void remember_position(Point);
  void remember_current_position(void);
};

#endif
