/*!
 * \file screenManager.cpp
 * \brief Implementation des fonctions de la classe ScreenManager.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "screenManager.h"

ScreenManager::ScreenManager(irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver, float _screen_width, float _screen_height, State_List *_game_state)
    :screen_width(_screen_width), screen_height(_screen_height), game_state(_game_state)
{
    g = device->getGUIEnvironment();
    cursor = device->getCursorControl();
    cursor->setActiveIcon(ig::ECURSOR_ICON(1));

    // La barre de menu
    menu = new Menu();
    menu->create_menu(g);
    // Une fenêtre pour différents réglages
    menu->create_window(g);

    // Image de prise de degats
    init_pain(driver);

    // Image du titre
    init_title(driver);

    // Image du game over
    init_gameOver(driver);

    // Choix de la police de caractères
    skin = g->getSkin();
    font = g->getFont("data/fontlucida.png");
    skin->setFont(font);

    // Creation du bouton de newGame
    init_newGame_button(driver);
    // Creation du bouton de quitGame
    init_quitGame_button(driver);

}

void ScreenManager::updateState(float width, float height)
{
    if(*game_state == START_SCREEN)
    {
        titleImage->setVisible(true);
        newGame_button->setVisible(true);
        quitGame_button->setVisible(true);
    }
    else if(*game_state == RUNNING_GAME)
    {
        titleImage->setVisible(false);
        gameOverImage->setVisible(false);
        newGame_button->setVisible(false);
        quitGame_button->setVisible(false);
    }
    else if(*game_state == GAME_OVER)
    {
        gameOverImage->setVisible(true);
        newGame_button->setVisible(true);
        quitGame_button->setVisible(true);
    }

    if(screen_width != width || screen_height != height)
    {
        screen_width = width;
        screen_height = height;
    }
}

void ScreenManager::displayPain(bool state)
{
    painImage->setVisible(state);
}

bool ScreenManager::isVisiblePain() const
{
    return painImage->isVisible();
}

void ScreenManager::init_newGame_button(irr::video::IVideoDriver *driver)
{
    newGame_button = g->addButton(ic::rect<irr::s32>(screen_width/2 - screen_width/4, screen_height/2 - screen_height/12,
                                                     screen_width/2 + screen_width/4, screen_height/2 + screen_height/12),
                                  0,
                                  NEW_GAME_BUTTON);

    iv::ITexture* newGame = driver->getTexture("data/NewGameUnpressed.png");
    newGame_button->setScaleImage(true);
    newGame_button->setImage(newGame);

    newGame = driver->getTexture("data/NewGamePressed.png");
    newGame_button->setPressedImage(newGame);
    newGame_button->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER);
    newGame->drop();
}

void ScreenManager::init_quitGame_button(irr::video::IVideoDriver *driver)
{
    quitGame_button = g->addButton(ic::rect<irr::s32>(screen_width/2 - screen_width/4, 3*screen_height/4 - screen_height/12,
                                                      screen_width/2 + screen_width/4, 3*screen_height/4 + screen_height/12),
                                   0,
                                   QUIT_GAME_BUTTON);

    iv::ITexture* quitGame = driver->getTexture("data/QuitGameUnpressed.png");
    quitGame_button->setScaleImage(true);
    quitGame_button->setImage(quitGame);

    quitGame = driver->getTexture("data/QuitGamePressed.png");
    quitGame_button->setPressedImage(quitGame);
    quitGame_button->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER);
    quitGame->drop();
}

void ScreenManager::init_title(irr::video::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/Title.png");
    titleImage = g->addImage(ic::rect<irr::s32>(screen_width/2 - screen_width/3, screen_height/4 - screen_height/12,
                                                screen_width/2 + screen_width/3, screen_height/4 + screen_height/12));
    titleImage->setScaleImage(false);
    titleImage->setImage(basicTexture);
    titleImage->setVisible(false);
    titleImage->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT);
}

void ScreenManager::init_gameOver(irr::video::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/gameOver.png");
    gameOverImage = g->addImage(ic::rect<irr::s32>(screen_width/2 - screen_width/3, screen_height/4 - screen_height/12,
                                                screen_width/2 + screen_width/3, screen_height/4 + screen_height/12));
    gameOverImage->setScaleImage(false);
    gameOverImage->setImage(basicTexture);
    gameOverImage->setVisible(false);
    gameOverImage->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT);
}

void ScreenManager::init_pain(irr::video::IVideoDriver *driver)
{
    iv::ITexture *basicTexture = driver->getTexture("data/basicImage.pcx");
    painImage = g->addImage(ic::rect<irr::s32>(0, 0, screen_width, screen_height));
    painImage->setScaleImage(true);
    painImage->setImage(basicTexture);
    painImage->setColor(iv::SColor(50, 255, 0, 0));
    painImage->setVisible(false);
    painImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT, ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT);
}
