#include "common.h"

#ifndef __asteroids3d_application_hpp
#define __asteroids3d_application_hpp

using namespace Ogre;

class Application {
protected:
    Root *mRoot;
    RenderWindow *mWindow;
    SceneManager *mSceneMgr;
    Camera *mCamera;

public:
    void setup(ALLEGRO_DISPLAY *display);

    Application();

    ~Application();

    void render();

private:
    void createRoot();

    void defineResources();

    void setupRenderSystem();

    void createRenderWindow(ALLEGRO_DISPLAY *display);

    void initializeResourceGroups();

    virtual void chooseSceneManager();

    virtual void createCamera();

    virtual void createViewports();

    virtual void createScene() = 0;
};

#endif
