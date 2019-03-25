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
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "engine.h"
#include "line.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

const double FPS = 60;
bool should_quit_game = false;
bool pressed_keys[6] = {}; // set all to false

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

  ALLEGRO_COLOR bg_color = al_map_rgb(50, 50, 50);
  ALLEGRO_BITMAP *previous_buffer = NULL;

  // Display a black screen
  al_clear_to_color(bg_color);
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

    if (previous_buffer != NULL) {
      al_lock_bitmap(previous_buffer, ALLEGRO_LOCK_READONLY, al_get_bitmap_format(previous_buffer));
    }
    for (auto player = players.begin(); player != players.end(); ++player) {
      player->update(pressed_keys, previous_buffer);
    }
    if (previous_buffer != NULL) {
      al_unlock_bitmap(previous_buffer);
    }

    // Check if we need to redraw
    if (redraw && al_is_event_queue_empty(event_queue)) {
      // Redraw
      al_clear_to_color(bg_color);

      for (auto player = players.begin(); player != players.end(); ++player) {
        player->draw();
      }

      al_flip_display();
      redraw = false;

      if (previous_buffer != NULL) {
        al_destroy_bitmap(previous_buffer);
      }
      previous_buffer = al_clone_bitmap(al_get_backbuffer(display));
    }
  }

  // Clean up
  if (timer) al_destroy_timer(timer);
  if (display) al_destroy_display(display);
  if (event_queue) al_destroy_event_queue(event_queue);

  al_uninstall_system();

  return 0;
}
