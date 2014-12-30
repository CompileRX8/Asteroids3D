#include "common.h"

#ifndef __asteroids3d_asteroid_hpp
#define __asteroids3d_asteroid_hpp

using namespace Ogre;

class Asteroid {
public:

    enum class Size {
        LARGE,
        MEDIUM,
        SMALL
    };

private:

    static unsigned int nextId;

    unsigned int id;
    std::string name;

    Size size;

    Vector3 position;
    Vector3 direction;
    Real moveSpeed;

    Quaternion orientation;
    Quaternion rotationQuaternion;
    Quaternion rotationSource;
    Quaternion rotationTarget;
    Real rotationProgress;

    void initLarge();

    void initMedium(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection);

    void initSmall(const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection);

    Asteroid(const Size sz, const std::unique_ptr<Asteroid> &aPtr, const Vector3 &hitDirection);

public:

    static std::unique_ptr<Asteroid> create(const Size sz, const std::unique_ptr<Asteroid> &aPtr = nullptr, const Vector3 &hitDirection = Vector3::UNIT_X);

    ~Asteroid();

    const Asteroid::Size getSize() const;

    const unsigned int getId() const;

    const std::string getName() const;

    const Vector3 getPosition() const;

    const Quaternion getOrientation() const;

    void move(Real time);
};

#endif
