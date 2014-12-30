#include "common.h"
#include "FlightController.hpp"
#include "Renderer.hpp"

#ifndef __asteroids3d_game_hpp
#define __asteroids3d_game_hpp

class Game {
private:
    double startTime;
    FlightController flightController;
    Renderer renderer;
    std::list<std::unique_ptr<Asteroid>> asteroids;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;

    unsigned int score;
    unsigned int highScore;
    std::string highScoreInitials;

    void createObjects();

    void updateObjects(const Real now, const Real lastMoveTime);

    void setupRenderer();

    void updateRenderer();

    void mainLoop();

    void handleAsteroidHit(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection);

    void updateScore(const unsigned int pointsAwarded);

public:
    Game(ALLEGRO_DISPLAY *d, ALLEGRO_EVENT_QUEUE *q, ALLEGRO_TIMER *t);

    ~Game();

    void start();
};

#endif