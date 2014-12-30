#include "common.h"
#include "Renderer.hpp"
#include "FlightController.hpp"
#include "Game.hpp"

const int WIDTH = 1280;
const int HEIGHT = 1024;

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

void init(void) {
    srand(time(NULL));

    if (!al_init()) {
        abort_example("Could not init Allegro.\n");
    }
    if (!al_install_keyboard()) {
        abort_example("Could not init Allegro keyboard.\n");
    }
    if (!al_install_mouse()) {
        abort_example("Could not init Allegro mouse.\n");
    }
    if (!al_install_joystick()) {
        abort_example("Could not init Allegro joystick.\n");
    }
    al_reconfigure_joysticks();

    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_RESIZABLE);
    display = al_create_display(WIDTH, HEIGHT);
    if (!display) {
        abort_example("Error creating display\n");
    }
    al_set_window_title(display, "Asteroids 3D");

    const double BPS = 60.0;
    timer = al_create_timer(1.0 / BPS);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_joystick_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    init();

    {
        Game game(display, queue, timer);
        game.start();
    }

    al_uninstall_system();

    return 0;
}
