#include "common.h"
#include "FlightController.hpp"

using namespace Ogre;

FlightController::FlightController() :
        rot_x(0),
        rot_y(0),
        rot_z(0),
        translate(Vector3::ZERO),
        lmb(false),
        rmb(false),
        forward(false),
        back(false),
        left(false),
        right(false),
        current_speed(0),
        last_translate(Vector3::ZERO),
        lastMoveTime(al_get_time()) {
    fprintf(stdout, "Joystick Installed: %s\n", (al_is_joystick_installed()) ? "Yes" : "No");
    fprintf(stdout, "Num Joysticks: %d\n", al_get_num_joysticks());
}

FlightController::~FlightController() {
}

Radian FlightController::getRotX() {
    return Radian(rot_x);
}

Radian FlightController::getRotY() {
    return Radian(rot_y);
}

Radian FlightController::getRotZ() {
    return Radian(rot_y);
}

Vector3 &FlightController::getTranslate() {
    return translate;
}

void FlightController::reset() {
    rot_x = Real(0);
    rot_y = Real(0);
    rot_z = Real(0);
    translate = Vector3::ZERO;
}

bool FlightController::update(ALLEGRO_EVENT event) {
    bool updated = true;

    switch (event.type) {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
            const bool is_down = (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
            if (event.mouse.button == 1)
                lmb = is_down;
            if (event.mouse.button == 2)
                rmb = is_down;
            break;
        }

        case ALLEGRO_EVENT_MOUSE_AXES:
            if (lmb) {
                rot_x = Degree(Real(-event.mouse.dx * 0.13));
                rot_y = Degree(Real(-event.mouse.dy * 0.13));
            }
            if (rmb) {
                translate.x = Real(event.mouse.dx * 0.5);
                translate.y = Real(event.mouse.dy * -0.5);
            }
            break;

        case ALLEGRO_EVENT_JOYSTICK_AXIS: {
            const int stick = event.joystick.stick;
            const int axis = event.joystick.axis;
            const float pos = event.joystick.pos;
//            fprintf(stdout, "Stick: %d Axis: %d Pos: %f\n", stick, axis, pos);

            break;
        }

        case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
        case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP: {
            const bool is_down = (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN);
            const int stick = event.joystick.stick;
            const int button = event.joystick.button;
//            fprintf(stdout, "Stick: %d Button: %d %s\n", stick, button, (is_down) ? "Down" : "Up");
            break;
        }

        case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
            al_reconfigure_joysticks();
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_KEY_UP: {
            const bool is_down = (event.type == ALLEGRO_EVENT_KEY_DOWN);
            if (event.keyboard.keycode == ALLEGRO_KEY_W)
                forward = is_down;
            if (event.keyboard.keycode == ALLEGRO_KEY_S)
                back = is_down;
            if (event.keyboard.keycode == ALLEGRO_KEY_A)
                left = is_down;
            if (event.keyboard.keycode == ALLEGRO_KEY_D)
                right = is_down;
            break;
        }

        default:
            updated = false;
            break;
    }

    if (updated) {
        double timestamp = event.any.timestamp;
        Real time_since_update = Real(timestamp - lastMoveTime);
        Real move_scale = Real(MOVE_SPEED * time_since_update);

        if (forward)
            translate.z = Real(-move_scale);
        if (back)
            translate.z = Real(move_scale);
        if (left)
            translate.x = Real(-move_scale);
        if (right)
            translate.x = Real(move_scale);

        if (translate == Vector3::ZERO) {
            translate = last_translate;
            current_speed -= time_since_update * 0.3;
        } else {
            current_speed += time_since_update;
        }
        if (current_speed > 1.0)
            current_speed = 1.0;
        if (current_speed < 0.0)
            current_speed = 0.0;

        translate *= current_speed;

        last_translate = translate;
        lastMoveTime = timestamp;
    }

    return updated;
}
