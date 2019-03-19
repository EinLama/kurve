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
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const float FPS = 60;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

bool should_quit_game = false;

enum OBSERVED_KEYS {
  KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN
};

bool pressed_keys[4];

void handle_key_down(ALLEGRO_EVENT evt) {
  int keycode = evt.keyboard.keycode;
  if (keycode == ALLEGRO_KEY_ESCAPE) should_quit_game = true;
  if (keycode == ALLEGRO_KEY_LEFT) pressed_keys[KEY_LEFT] = true;
  if (keycode == ALLEGRO_KEY_DOWN) pressed_keys[KEY_DOWN] = true;
  if (keycode == ALLEGRO_KEY_UP) pressed_keys[KEY_UP] = true;
  if (keycode == ALLEGRO_KEY_RIGHT) pressed_keys[KEY_RIGHT] = true;
}

void handle_key_up(ALLEGRO_EVENT evt) {
  int keycode = evt.keyboard.keycode;
  if (keycode == ALLEGRO_KEY_LEFT) pressed_keys[KEY_LEFT] = false;
  if (keycode == ALLEGRO_KEY_DOWN) pressed_keys[KEY_DOWN] = false;
  if (keycode == ALLEGRO_KEY_UP) pressed_keys[KEY_UP] = false;
  if (keycode == ALLEGRO_KEY_RIGHT) pressed_keys[KEY_RIGHT] = false;
}

class Line {
  ALLEGRO_COLOR color;
  float x;

  public:
    Line (int, int, int);
    void draw(void);
};

Line::Line (int r, int g, int b) {
  color = al_map_rgb(r, g, b);
  x = 0;
}

void Line::draw() {
  al_draw_line(this->x, 0, this->x, SCREEN_HEIGHT, this->color, 1);

  this->x += 10;
  if (this->x > SCREEN_WIDTH) {
    this->x = 0;
  }
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

  // Initialise our line object for drawing
  Line line (255, 255, 255);

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
          fprintf(stderr, "Unsupported event received: %d\n", event.type);
          break;
      }
    }

    // Check if we need to redraw
    if (redraw && al_is_event_queue_empty(event_queue)) {
      // Redraw
      al_clear_to_color(al_map_rgb(0, 0, 0));

      // Drawing a line here!
      line.draw();

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
