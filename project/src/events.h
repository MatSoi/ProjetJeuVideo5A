// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>
#include "player.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

// We'll create a struct to record info on the mouse state
struct SMouseState
{
    ic::position2di Position;
    bool LeftButtonDown;
    SMouseState() : LeftButtonDown(false) { }
};

class EventReceiver : public IEventReceiver
{
public:
    EventReceiver();

    bool OnEvent(const SEvent &event);

    bool event_handler(const f32 frameDeltaTime, float width, float height);

    void set_gui(irr::gui::IGUIEnvironment *gui);

    void set_player(Player* _player);

    void set_camera(is::ICameraSceneNode* _camera, gui::ICursorControl *_cursor, float width, float height);

    void set_textures(const std::vector<iv::ITexture *> &tex){textures=tex;}

private:
    bool keyboard_handler(const f32 frameDeltaTime);

    bool mouse_handler(const f32 frameDeltaTime);

    void camera_handler();

    bool mouse_event(const SEvent &event);

    const SMouseState & GetMouseState(void) const;

    bool gui_handler(const SEvent &event);

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

    ig::IGUIEnvironment *gui;
    Player* player;
    bool button_pressed;
    int  old_x, old_y;
    std::vector<iv::ITexture*> textures;
    int current_texture;

    SMouseState MouseState;
    is::ICameraSceneNode* camera;
    gui::ICursorControl *cursor;
    float angle_camera;
    float screen_width, screen_height;
    bool focus_mouse = true;
};

#endif
