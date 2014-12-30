#include "common.h"
#include "Application.hpp"
#include "Asteroid.hpp"
#include <memory>

#ifndef __asteroids3d_renderer_hpp
#define __asteroids3d_renderer_hpp

class Renderer : public Application {
private:
    ALLEGRO_DISPLAY *display;
    Real lastRenderTime;
    Real startTime;
    std::list<Ogre::ParticleSystem *> explosions;

public:
    Renderer(ALLEGRO_DISPLAY *display);

    ~Renderer();

    void setup();

    void moveCamera(const Ogre::Radian &rot_x, const Ogre::Radian &rot_y, const Ogre::Radian &rot_z,
            const Ogre::Vector3 &translate);

    void moveCamera(const std::unique_ptr<Asteroid> &aPtr);

    void update(const std::unique_ptr<Asteroid> &aPtr);

    void destroy(const std::unique_ptr<Asteroid> &aPtr);

    void nextFrame(const Real now);

    void resize(unsigned int w, unsigned int h);

    void updateScore(const unsigned int score);

    void updateHighScore(const std::string initials, const unsigned int score);

private:
    void createScene();

    void animate(const Real now);

    Ogre::SceneNode *getSceneNode(const std::unique_ptr<Asteroid> &aPtr) const;

    Ogre::Entity *getEntity(const std::unique_ptr<Asteroid> &aPtr) const;

    const std::string getEntityName(const std::unique_ptr<Asteroid> &aPtr) const;

    const std::string getNodeName(const std::unique_ptr<Asteroid> &aPtr) const;

    void add(const std::unique_ptr<Asteroid> &aPtr);

    std::string formatScore(const unsigned int score) const;
};

#endif