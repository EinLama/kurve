#include "line.h"

Line::Line (ALLEGRO_COLOR c, KURVE_KEYS left, KURVE_KEYS right) :
  color(c), positions(), THICKNESS(4), speed(3), angle(DIR_DOWN_RIGHT),
  left_key(left), right_key(right),
  alive(true), current_x(0), current_y(0) {

}

void Line::spawn(Point position) {
  this->current_x = position.x;
  this->current_y = position.y;

  this->remember_position(position);
}

void Line::remember_position(Point new_pos) {
  this->positions.push_back(new_pos);
}

void Line::remember_current_position() {
  this->remember_position(Point(this->current_x, this->current_y));
}

void Line::update(bool *pressed_keys) {
  if (!this->alive) { return; }

  // Move
  double delta_x = cos(this->angle * M_PI / 180) * this->speed;
  double delta_y = sin(this->angle * M_PI / 180) * this->speed;

  this->current_x += delta_x;
  this->current_y += delta_y;

  // TODO: replace pseudo test borders with real ones
  if (this->current_x > SCREEN_WIDTH - 50 || this->current_x < 50) {
    this->alive = false;
  }

  // Be controlled:
  if (pressed_keys[this->left_key]) {
    this->steer(-1);
    this->remember_current_position();
  } else if (pressed_keys[this->right_key]) {
    this->steer(1);
    this->remember_current_position();
  }
}

void Line::steer(double direction) {
  const double MAX_ANGLE_CHANGE = 2;

  this->angle += MAX_ANGLE_CHANGE * direction;
  this->angle = fmod(this->angle, 360);

  if (this->angle < 0) { this->angle += 360; }
}

void Line::draw() {
  double prev_x = 0, prev_y = 0, x = 0, y = 0;
  bool first_run = true;

  for (auto iter = this->positions.begin();
      iter != this->positions.end();
      ++iter) {

    if (first_run) {
      prev_x = (*iter).x;
      prev_y = (*iter).y;

      first_run = false;
    } else {
      x = (*iter).x;
      y = (*iter).y;

      al_draw_line(prev_x, prev_y, x, y, this->color, this->THICKNESS);

      prev_x = x;
      prev_y = y;
    }
  }

  al_draw_line(prev_x, prev_y, this->current_x, this->current_y, this->color, this->THICKNESS);
}
