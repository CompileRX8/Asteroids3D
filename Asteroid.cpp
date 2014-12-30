#include "common.h"
#include "Asteroid.hpp"

using namespace Ogre;

unsigned int Asteroid::nextId = 0;

std::unique_ptr<Asteroid> Asteroid::create(const Size sz, const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection) {
    if (sz != Size::LARGE && aPtr == nullptr) {
        throw std::invalid_argument("A valid Asteroid pointer is required for creating Asteroids in sizes other than LARGE");
    }

    return std::unique_ptr<Asteroid>(new Asteroid(sz, aPtr, hitDirection));
}

Asteroid::Asteroid(const Size sz, const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection) :
        size(sz),
        id(nextId++) {
    switch (size) {
        case Size::LARGE:
            initLarge();
            break;
        case Size::MEDIUM:
            initMedium(aPtr, hitDirection);
            break;
        case Size::SMALL:
            initSmall(aPtr, hitDirection);
            break;
    }
    direction.normalise();
    rotationQuaternion.normalise();

    rotationProgress = 0.0;

    name = "Asteroid" + StringConverter::toString(id);
}

Asteroid::~Asteroid() {
}

void Asteroid::initLarge() {
    moveSpeed = 35.0;

    position = Vector3(Math::SymmetricRandom(), Math::SymmetricRandom(), Math::SymmetricRandom()) * 500.0;
    direction = Vector3(Math::SymmetricRandom(), Math::SymmetricRandom(), Math::SymmetricRandom());

    Vector3 rotationAxis(Math::SymmetricRandom(), Math::SymmetricRandom(), Math::SymmetricRandom());
    rotationAxis.normalise();
    rotationQuaternion = Quaternion(Degree(45.0), rotationAxis);

    orientation = rotationSource = Quaternion::IDENTITY;
    rotationTarget = rotationQuaternion;
}

void Asteroid::initMedium(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection) {
    moveSpeed = 50.0;

    position = aPtr->getPosition();
    direction = aPtr->direction.randomDeviant(Degree(60.0), hitDirection);

    rotationQuaternion = aPtr->orientation * (direction.getRotationTo(aPtr->direction.perpendicular()));

    orientation = rotationSource = aPtr->orientation;
    rotationTarget = rotationQuaternion;
}

void Asteroid::initSmall(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection) {
    initMedium(aPtr, hitDirection);
    moveSpeed = 75.0;
    direction = aPtr->direction.randomDeviant(Degree(80.0), hitDirection);
}

const unsigned int Asteroid::getId() const {
    return id;
}

const std::string Asteroid::getName() const {
    return name;
}

const Asteroid::Size Asteroid::getSize() const {
    return size;
}

const Vector3 Asteroid::getPosition() const {
    return position;
}

const Quaternion Asteroid::getOrientation() const {
    return orientation;
}

void Asteroid::move(Real time) {
    Real distance = moveSpeed * time;
    position += (direction * distance);

    if (position.squaredLength() > 3000000.0) {
        position = -position;
        position += (direction * distance);
    }

    if (rotationProgress >= 1.0) {
        rotationProgress = 0.0;
        rotationSource = orientation;
        rotationTarget = rotationQuaternion * orientation;
    }

    orientation = Quaternion::Slerp(rotationProgress, rotationSource, rotationTarget);
    rotationProgress += time;
}
