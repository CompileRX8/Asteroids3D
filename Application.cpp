#include "common.h"
#include "Application.hpp"

void Application::setup(ALLEGRO_DISPLAY *display) {
    createRoot();
    defineResources();
    setupRenderSystem();
    createRenderWindow(display);
    initializeResourceGroups();
    chooseSceneManager();
    createCamera();
    createViewports();
    createScene();
}

Application::Application() {
    mRoot = NULL;
}

Application::~Application() {
    delete mRoot;
}

void Application::createRoot() {
    mRoot = new Root();
}

void Application::defineResources() {
    String secName, typeName, archName;
    ConfigFile cf;
    cf.load("resources.cfg");
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName,
                    typeName, secName);
        }
    }
}

void Application::setupRenderSystem() {
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog()) {
        throw Exception(52, "User cancelled the config dialog!",
                "Application::setupRenderSystem()");
    }
}

void Application::createRenderWindow(ALLEGRO_DISPLAY *display) {
    int w = al_get_display_width(display);
    int h = al_get_display_height(display);

    // Initialise Ogre without creating a window.
    mRoot->initialise(false);

    Ogre::NameValuePairList misc;
#ifdef ALLEGRO_WINDOWS
    unsigned long winHandle      = reinterpret_cast<size_t>(al_get_win_window_handle(display));
    unsigned long winGlContext   = reinterpret_cast<size_t>(wglGetCurrentContext());
    misc["externalWindowHandle"] = StringConverter::toString(winHandle);
    misc["externalGLContext"]    = StringConverter::toString(winGlContext);
    misc["externalGLControl"]    = String("True");
#else
    misc["currentGLContext"] = String("True");
#endif

    mWindow = mRoot->createRenderWindow("MainRenderWindow", w, h, false,
            &misc);
}

void Application::initializeResourceGroups() {
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Application::chooseSceneManager() {
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "Default SceneManager");
}

void Application::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Vector3(-300, 300, -300));
    mCamera->lookAt(Vector3(0, 0, 0));
    mCamera->setNearClipDistance(5);
}

void Application::createViewports() {
    // Create one viewport, entire window.
    Viewport *vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0, 0.25, 0.5));

    // Alter the camera aspect ratio to match the viewport.
    mCamera->setAspectRatio(
            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void Application::render() {
    const bool swap_buffers = false;
    mWindow->update(swap_buffers);
    mRoot->renderOneFrame();
    al_flip_display();
}
