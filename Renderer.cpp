#include "common.h"
#include "Renderer.hpp"
#include "Asteroid.hpp"

using namespace Ogre;

Renderer::Renderer(ALLEGRO_DISPLAY *display) :
        display(display),
        startTime(Real(al_get_time())),
        lastRenderTime(startTime),
        explosions() {
}

Renderer::~Renderer() {
}

void Renderer::setup() {
    Application::setup(display);
}

void Renderer::resize(unsigned int w, unsigned int h) {
    mWindow->resize(w, h);
    mCamera->setAspectRatio(Real(w) / Real(h));
}

void Renderer::createScene() {
    mCamera->setPosition(Vector3::ZERO);

    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

    // Enable shadows.
    mSceneMgr->setAmbientLight(ColourValue(0.25, 1.0, 0.25));
    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);

    SceneNode *shipNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ShipNode", Vector3(30.0, 30.0, 30.0));

    Entity *ship = mSceneMgr->createEntity("Ship", "AngularShipBody.mesh");
    ship->setCastShadows(true);
    ship->setMaterialName("Asteroids3D/Asteroid");
    shipNode->attachObject(ship);
    shipNode->setScale(1.5f, 1.5f, 1.5f);

    Overlay *scoreBarOverlay = OverlayManager::getSingleton().getByName("ScoreBarOverlay");
    if (scoreBarOverlay) {
        scoreBarOverlay->show();
    }
}

void Renderer::moveCamera(const Radian &rot_x, const Radian &rot_y, const Radian &rot_z, const Vector3 &translate) {
    mCamera->yaw(rot_x);
    mCamera->pitch(rot_y);
//    mCamera->roll(rot_z);
//    mCamera->moveRelative(translate);
}

void Renderer::moveCamera(const std::unique_ptr<Asteroid> &aPtr) {
    mCamera->lookAt(aPtr->getPosition());
}

std::string Renderer::formatScore(const unsigned int score) const {
    return StringConverter::toString(score, 8, '0');
}

void Renderer::updateScore(const unsigned int score) {
    OverlayElement *scoreElement = OverlayManager::getSingleton().getOverlayElement("ScoreBarOverlay/ScoreElement");
    if (scoreElement) {
        scoreElement->setCaption(formatScore(score));
    }
}

void Renderer::updateHighScore(const std::string initials, const unsigned int score) {
    OverlayElement *highScoreElement = OverlayManager::getSingleton().getOverlayElement("ScoreBarOverlay/HighScoreElement");
    if (highScoreElement) {
        highScoreElement->setCaption(initials + " " + formatScore(score));
    }
}

const std::string Renderer::getEntityName(const std::unique_ptr<Asteroid> &aPtr) const {
    return aPtr->getName();
}

const std::string Renderer::getNodeName(const std::unique_ptr<Asteroid> &aPtr) const {
    return aPtr->getName() + "Node";
}

Entity *Renderer::getEntity(const std::unique_ptr<Asteroid> &aPtr) const {
    return mSceneMgr->getEntity(getEntityName(aPtr));
}

SceneNode *Renderer::getSceneNode(const std::unique_ptr<Asteroid> &aPtr) const {
    return getEntity(aPtr)->getParentSceneNode();
}

void Renderer::add(const std::unique_ptr<Asteroid> &aPtr) {
    std::string entityMesh, materialName;

    Real scale = 1.0f;
    switch (aPtr->getSize()) {
        case Asteroid::Size::LARGE:
            scale = 0.3f;
            entityMesh = "knot.mesh";
            materialName = "Asteroids3D/Asteroid";
            break;
        case Asteroid::Size::MEDIUM:
            scale = 0.2f;
            entityMesh = "cube.mesh";
            materialName = "Asteroids3D/Asteroid";
            break;
        case Asteroid::Size::SMALL:
            scale = 0.1f;
            entityMesh = "sphere.mesh";
            materialName = "Asteroids3D/Asteroid";
            break;
    }
    Entity *entity = mSceneMgr->createEntity(getEntityName(aPtr), entityMesh);
    entity->setCastShadows(true);
    entity->setMaterialName(materialName);

    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(getNodeName(aPtr));
    node->attachObject(entity);
    node->setScale(scale, scale, scale);
}

void Renderer::update(const std::unique_ptr<Asteroid> &aPtr) {
    if (!mSceneMgr->hasEntity(getEntityName(aPtr))) {
        add(aPtr);
    }

    SceneNode *node = getSceneNode(aPtr);

    node->setPosition(aPtr->getPosition());
    node->setOrientation(aPtr->getOrientation());
}

void Renderer::destroy(const std::unique_ptr<Asteroid> &aPtr) {
    Entity *entity = getEntity(aPtr);
    SceneNode *node = getSceneNode(aPtr);
    node->detachObject(entity);

    ParticleSystem *explosionSystem = mSceneMgr->createParticleSystem(getEntityName(aPtr), "Asteroids3D/AsteroidHitExplosion");
    node->attachObject(explosionSystem);

    explosions.push_back(explosionSystem);
}

void Renderer::animate(const Real now) {
    const Real timeSinceStart = now - startTime;
    const Real timeSinceLastRender = now - lastRenderTime;

    for (auto iter = std::begin(explosions); iter != std::end(explosions);) {
        ParticleSystem *explosionSystem = *iter;
        if (!explosionSystem->getEmitting()) {
            // This explosion is done--erase it from the collection
            iter = explosions.erase(iter);

            // Clean up the node and entity
            SceneNode *node = explosionSystem->getParentSceneNode();
            node->detachObject(explosionSystem);
            node->getParent()->removeChild(node);
        } else {
            iter++;
        }
    }
}

void Renderer::nextFrame(const Real now) {
    animate(now);
    render();
    lastRenderTime = now;
}
