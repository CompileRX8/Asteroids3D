#include "common.h"

#ifndef __asteroids3d_flight_controller_hpp
#define __asteroids3d_flight_controller_hpp

class FlightController {
private:
    const double MOVE_SPEED = 1500.0;

    Ogre::Radian rot_x;
    Ogre::Radian rot_y;
    Ogre::Radian rot_z;
    Ogre::Vector3 translate;
    bool lmb;
    bool rmb;
    bool forward;
    bool back;
    bool left;
    bool right;
    float pitch_input;
    float roll_input;
    float yaw_input;
    float current_speed;
    Ogre::Vector3 last_translate;
    double lastMoveTime;

public:
    FlightController();

    ~FlightController();

    void reset();

    bool update(ALLEGRO_EVENT event);

    Ogre::Radian getRotX();

    Ogre::Radian getRotY();

    Ogre::Radian getRotZ();

    Ogre::Vector3 &getTranslate();
};

#endif