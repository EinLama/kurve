/*
 * This program uses the Allegro game library to display a blank window.
 *
 * It initializes the display and starts up the main game loop. The
 * game loop only checks for two events: timer (determined by the FPS)
 * and display close (when the user tries to close the window).
 *
 * http://www.damienradtke.org/building-a-mario-clone-with-allegro
 */

#include <stdio.h>
#include <math.h>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

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

const float FPS = 60;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

bool should_quit_game = false;

enum KURVE_KEYS {
  KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
  KEY_A, KEY_S
};

bool pressed_keys[6];

void handle_key_down(ALLEGRO_EVENT evt) {
  int keycode = evt.keyboard.keycode;
  if (keycode == ALLEGRO_KEY_ESCAPE) should_quit_game = true;

  if (keycode == ALLEGRO_KEY_LEFT) pressed_keys[KEY_LEFT] = true;
  if (keycode == ALLEGRO_KEY_DOWN) pressed_keys[KEY_DOWN] = true;
  if (keycode == ALLEGRO_KEY_UP) pressed_keys[KEY_UP] = true;
  if (keycode == ALLEGRO_KEY_RIGHT) pressed_keys[KEY_RIGHT] = true;

  if (keycode == ALLEGRO_KEY_A) pressed_keys[KEY_A] = true;
  if (keycode == ALLEGRO_KEY_S) pressed_keys[KEY_S] = true;
}

void handle_key_up(ALLEGRO_EVENT evt) {
  int keycode = evt.keyboard.keycode;
  if (keycode == ALLEGRO_KEY_LEFT) pressed_keys[KEY_LEFT] = false;
  if (keycode == ALLEGRO_KEY_DOWN) pressed_keys[KEY_DOWN] = false;
  if (keycode == ALLEGRO_KEY_UP) pressed_keys[KEY_UP] = false;
  if (keycode == ALLEGRO_KEY_RIGHT) pressed_keys[KEY_RIGHT] = false;

  if (keycode == ALLEGRO_KEY_A) pressed_keys[KEY_A] = false;
  if (keycode == ALLEGRO_KEY_S) pressed_keys[KEY_S] = false;
}

class Point {
  public:
    float x, y;
    Point (float _x, float _y) { x = _x; y = _y; }
};

class Line {
  ALLEGRO_COLOR color;
  std::vector<Point> positions;
  float THICKNESS, speed, angle;
  KURVE_KEYS left_key, right_key;

  public:
  float current_x, current_y;
  Line (ALLEGRO_COLOR, KURVE_KEYS, KURVE_KEYS);
  void spawn(Point);
  void draw(void);
  void steer(float);
  void update(void);
  void remember_position(Point);
  void remember_current_position(void);
};

Line::Line (ALLEGRO_COLOR c, KURVE_KEYS left, KURVE_KEYS right) {
  color = c;
  THICKNESS = 4;
  speed = 3;
  angle = DIR_DOWN_RIGHT;

  left_key = left;
  right_key = right;
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

void Line::update() {
  // Move
  float delta_x = cos(this->angle * M_PI / 180) * this->speed;
  float delta_y = sin(this->angle * M_PI / 180) * this->speed;

  this->current_x += delta_x;
  this->current_y += delta_y;

  // Be controlled:
  if (pressed_keys[this->left_key]) {
    this->steer(-1);
    this->remember_current_position();
  } else if (pressed_keys[this->right_key]) {
    this->steer(1);
    this->remember_current_position();
  }
}

void Line::steer(float direction) {
  const float MAX_ANGLE_CHANGE = 2;

  this->angle += MAX_ANGLE_CHANGE * direction;
  this->angle = fmod(this->angle, 360);

  if (this->angle < 0) { this->angle += 360; }
}

void Line::draw() {
  float prev_x = 0, prev_y = 0, x = 0, y = 0;
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

int main(int argc, char *argv[])
{
  bool redraw = true;

  // Initialize allegro
  if (!al_init()) {
    fprintf(stderr, "Failed to initialize allegro.\n");
    return 1;
  }

  al_install_keyboard();

  // Initialize the timer
  timer = al_create_timer(1.0 / FPS);
  if (!timer) {
    fprintf(stderr, "Failed to create timer.\n");
    return 1;
  }

  // Create the display
  display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!display) {
    fprintf(stderr, "Failed to create display.\n");
    return 1;
  }

  // Create the event queue
  event_queue = al_create_event_queue();
  if (!event_queue) {
    fprintf(stderr, "Failed to create event queue.");
    return 1;
  }

  // Register event sources
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  // Display a black screen
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  // Start the timer
  al_start_timer(timer);

  // All of our players!
  std::vector<Line> players;

  // Initialise our line object for drawing
  Line line (al_map_rgb(255, 100, 100), KEY_LEFT, KEY_RIGHT);
  Line line2 (al_map_rgb(100, 255, 100), KEY_A, KEY_S);

  line.spawn(Point(100, 200));
  line2.spawn(Point(300, 400));

  players.push_back(line);
  players.push_back(line2);

  // Game loop
  while (!should_quit_game) {
    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;

    // Initialize timeout
    al_init_timeout(&timeout, 0.06);

    // Fetch the event (if one exists)
    bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

    // Handle the event
    if (get_event) {
      switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
          redraw = true;
          break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
          should_quit_game = true;
          break;
        case ALLEGRO_EVENT_KEY_DOWN:
          handle_key_down(event);
          break;
        case ALLEGRO_EVENT_KEY_UP:
          handle_key_up(event);
          break;
        default:
          //fprintf(stderr, "Unsupported event received: %d\n", event.type);
          break;
      }
    }

    for (auto player = players.begin(); player != players.end(); ++player) {
      player->update();
    }

    // Check if we need to redraw
    if (redraw && al_is_event_queue_empty(event_queue)) {
      // Redraw
      al_clear_to_color(al_map_rgb(50, 50, 50));

      for (auto player = players.begin(); player != players.end(); ++player) {
        player->draw();
      }

      al_flip_display();
      redraw = false;
    }
  } 

  // Clean up
  if (timer) al_destroy_timer(timer);
  if (display) al_destroy_display(display);
  if (event_queue) al_destroy_event_queue(event_queue);

  return 0;
}
