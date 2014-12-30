#include "common.h"
#include "Game.hpp"

Game::Game(ALLEGRO_DISPLAY *d, ALLEGRO_EVENT_QUEUE *q, ALLEGRO_TIMER *t) :
        display(d),
        queue(q),
        timer(t),
        flightController(),
        renderer(d),
        asteroids(),
        startTime(al_get_time()),
        score(0),
        highScore(0),
        highScoreInitials("???") {
}

Game::~Game() {
}

void Game::start() {
    createObjects();

    setupRenderer();

    mainLoop();
}

void Game::createObjects() {
    asteroids.clear();

    for (int i = 0; i < 20; i++) {
        asteroids.push_back(Asteroid::create(Asteroid::Size::LARGE));
    }
}

void Game::updateObjects(Real now, Real lastMoveTime) {
    for (auto iter = std::begin(asteroids); iter != std::end(asteroids); iter++) {
        (*iter)->move(now - lastMoveTime);
    }
}

void Game::setupRenderer() {
    renderer.setup();
}

void Game::updateRenderer() {
    for (auto iter = std::begin(asteroids); iter != std::end(asteroids); iter++) {
        renderer.update(*iter);
    }

    renderer.updateScore(score);
    renderer.updateHighScore(highScoreInitials, highScore);

    if (asteroids.size() > 0) {
        renderer.moveCamera(asteroids.front());
    } else {
        renderer.moveCamera(flightController.getRotX(), flightController.getRotY(),
                flightController.getRotZ(), flightController.getTranslate());
    }
}

void Game::mainLoop() {
    bool redraw = true;
    Real lastMoveTime = Real(al_get_time());

    al_start_timer(timer);

    for (; ;) {
        ALLEGRO_EVENT event;

        Real now = Real(al_get_time());

        if (al_is_event_queue_empty(queue) && redraw) {
            renderer.nextFrame(now);
            redraw = false;
        }

        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            break;
        }
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        flightController.reset();

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN: {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    std::list<unsigned int> currentAsteroids;
                    for (auto iter = std::begin(asteroids); iter != std::end(asteroids); iter++) {
                        currentAsteroids.push_back((*iter)->getId());
                    }
                    for (auto currIter = std::begin(currentAsteroids); currIter != std::end(currentAsteroids); currIter++) {
                        auto iter = std::begin(asteroids);
                        while (iter != std::end(asteroids)) {
                            if ((*iter)->getId() == *currIter) {
                                Vector3 hitDirection = (*iter)->getPosition();
                                hitDirection.normalise();
                                handleAsteroidHit(*iter, hitDirection);
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case ALLEGRO_EVENT_DISPLAY_RESIZE: {
                al_acknowledge_resize(event.display.source);
                int w = al_get_display_width(display);
                int h = al_get_display_height(display);
                renderer.resize(w, h);
                redraw = true;
                break;
            }

            default:
                flightController.update(event);
                break;
        }

        updateObjects(now, lastMoveTime);

        updateRenderer();

        lastMoveTime = now;
    }

    for (auto iter = std::begin(asteroids); iter != std::end(asteroids); iter++) {
        renderer.destroy(*iter);
    }
}

void Game::handleAsteroidHit(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection) {

    renderer.destroy(aPtr);

    switch (aPtr->getSize()) {
        case Asteroid::Size::LARGE:
            for (int i = 0; i < 2; i++) {
                asteroids.push_back(Asteroid::create(Asteroid::Size::MEDIUM, aPtr, hitDirection));
            }
            updateScore(10);
            break;

        case Asteroid::Size::MEDIUM:
            for (int i = 0; i < 2; i++) {
                asteroids.push_back(Asteroid::create(Asteroid::Size::SMALL, aPtr, hitDirection));
            }
            updateScore(30);
            break;

        case Asteroid::Size::SMALL:
            updateScore(50);
            break;
    }

    for (auto oldIter = asteroids.begin(); oldIter != asteroids.end(); oldIter++) {
        if ((*oldIter)->getId() == aPtr->getId()) {
            // Don't reset oldIter--break out of loop instead
            asteroids.erase(oldIter);
            break;
        }
    }
}

void Game::updateScore(const unsigned int pointsAwarded) {
    score += pointsAwarded;
    if (score > highScore) {
        highScore = score;
        highScoreInitials = "???";
    }
}
