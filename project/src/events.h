// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class EventReceiver : public IEventReceiver
{
private:
    irr::gui::IGUIEnvironment *gui;
    irr::scene::IAnimatedMeshSceneNode *node;
    bool button_pressed;
    int  old_x, old_y;
    std::vector<irr::video::ITexture*> textures;
    int current_texture;


    bool mouse_handler(const irr::SEvent &event);
    bool gui_handler(const irr::SEvent &event);

    void updateKeyState ();

    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    virtual bool IsKeyReleased(EKEY_CODE keyCode) const
    {
        return (KeyEvent[keyCode] && !KeyIsDown[keyCode]);
    }

    virtual bool IsKeyTriggered(EKEY_CODE keyCode) const
    {
        return (KeyEvent[keyCode] && KeyIsDown[keyCode]);
    }

    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
    bool KeyEvent[KEY_KEY_CODES_COUNT];

public:
    EventReceiver();
    bool OnEvent(const irr::SEvent &event);
    bool keyboard_handler(const f32 frameDeltaTime);
    void set_gui(irr::gui::IGUIEnvironment *gui);
    void set_node(irr::scene::IAnimatedMeshSceneNode *node);
    void set_textures(const std::vector<irr::video::ITexture *> &tex){textures=tex;}
};

#endif
