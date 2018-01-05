/*!
 * \file screenManager.cpp
 * \brief Implementation des fonctions de la classe ScreenManager.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "screenManager.h"

ScreenManager::ScreenManager(irr::IrrlichtDevice *device, iv::IVideoDriver *driver, float _screen_width, float _screen_height, State_List *_game_state, int _enemyLeft)
    :screen_width(_screen_width), screen_height(_screen_height), game_state(_game_state), enemyLeft(_enemyLeft)
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
    // Image de victoire
    init_victory(driver);
    // Image de la barre de vie
    init_life(driver);
    // Image d ennemis restants
    init_enemyLeftImage(driver);
    // Textures des chiffres
    init_numberText(driver);
    // Images des nombres
    init_numberImage();

    // Choix de la police de caractères
    skin = g->getSkin();
    font = g->getFont("data/fontlucida.png");
    skin->setFont(font);

    // Creation du bouton de newGame
    init_newGame_button(driver);
    // Creation du bouton de restartGame
    init_restartGame_button(driver);
    // Creation du bouton de quitGame
    init_quitGame_button(driver);
}

void ScreenManager::updateState(float width, float height, int life, int _enemyLeft)
{
    enemyLeft = _enemyLeft;

    switch(*game_state)
    {
    case(RUNNING_GAME):
        hideInfo(titleImage);
        hideInfo(gameOverImage);
        hideInfo(victoryImage);
        hideInfo(newGame_button);
        hideInfo(restartGame_button);
        hideInfo(quitGame_button);
        displayInfo(enemyLeftImage);
        displayVectImage(LifeImage);
        displayVectImage(numberImage);
        break;
    case(VICTORY):
        displayInfo(victoryImage);
        displayInfo(restartGame_button);
        restartGame_button->setEnabled(true);
        displayInfo(quitGame_button);
        break;
    case(GAME_OVER):
        displayInfo(gameOverImage);
        displayInfo(restartGame_button);
        restartGame_button->setEnabled(true);
        displayInfo(quitGame_button);

        life = 0;   // on affiche une barre de vie vide en cas de game over
        break;
    default:
        break;
    }


    if(screen_width != width || screen_height != height)
    {
        screen_width = width;
        screen_height = height;
    }

    updateLife(life);
    updateNumber();
}

void ScreenManager::displayPain(bool state)
{
    painImage->setVisible(state);
}

bool ScreenManager::isVisiblePain() const
{
    return painImage->isVisible();
}

void ScreenManager::init_newGame_button(iv::IVideoDriver *driver)
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
    newGame_button->setVisible(true);
    newGame_button->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER);
    newGame->drop();
}

void ScreenManager::init_restartGame_button(iv::IVideoDriver *driver)
{
    restartGame_button = g->addButton(ic::rect<irr::s32>(screen_width/2 - screen_width/4, screen_height/2 - screen_height/12,
                                                         screen_width/2 + screen_width/4, screen_height/2 + screen_height/12),
                                      0,
                                      RESTART_GAME_BUTTON);

    iv::ITexture* restartGame = driver->getTexture("data/RestartUnpressed.png");
    restartGame_button->setScaleImage(true);
    restartGame_button->setImage(restartGame);

    restartGame = driver->getTexture("data/RestartPressed.png");
    restartGame_button->setPressedImage(restartGame);
    restartGame_button->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER);
    restartGame_button->setVisible(false);
    restartGame_button->setEnabled(false);
    restartGame->drop();
}

void ScreenManager::init_quitGame_button(iv::IVideoDriver *driver)
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
    quitGame_button->setVisible(true);
    quitGame_button->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_CENTER);
    quitGame->drop();
}

void ScreenManager::init_title(iv::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/Title.png");
    titleImage = g->addImage(ic::rect<irr::s32>(0, 0,
                                                screen_width, screen_height/2 - screen_height/50));
    titleImage->setScaleImage(true);
    titleImage->setImage(basicTexture);
    titleImage->setVisible(true);
    titleImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT, ig::EGUIA_UPPERLEFT, ig::EGUIA_CENTER);
}

void ScreenManager::init_gameOver(iv::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/gameOver.png");
    gameOverImage = g->addImage(ic::rect<irr::s32>(screen_width/8, 0,
                                                   7 * screen_width/8, screen_height/2 - screen_height/12));
    gameOverImage->setScaleImage(true);
    gameOverImage->setImage(basicTexture);
    gameOverImage->setVisible(false);
    gameOverImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT, ig::EGUIA_UPPERLEFT, ig::EGUIA_CENTER);
}

void ScreenManager::init_victory(iv::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/Victory.png");
    victoryImage = g->addImage(ic::rect<irr::s32>(screen_width/8, 0,
                                                  7 * screen_width/8, screen_height/2 - screen_height/12));
    victoryImage->setScaleImage(true);
    victoryImage->setImage(basicTexture);
    victoryImage->setVisible(false);
    victoryImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT, ig::EGUIA_UPPERLEFT, ig::EGUIA_CENTER);
}

void ScreenManager::init_enemyLeftImage(iv::IVideoDriver *driver)
{
    iv::ITexture * basicTexture = driver->getTexture("data/ennemisRestants.png");
    enemyLeftImage = g->addImage(ic::rect<irr::s32>(0, 4*screen_height/5,
                                                    screen_width/4, screen_height));
    enemyLeftImage->setScaleImage(true);
    enemyLeftImage->setImage(basicTexture);
    enemyLeftImage->setVisible(false);
    enemyLeftImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_CENTER, ig::EGUIA_LOWERRIGHT, ig::EGUIA_LOWERRIGHT);
}

void ScreenManager::init_numberImage()
{
    for(int i = 0; i < 2; ++i)
    {
        numberImage.push_back(g->addImage(ic::rect<irr::s32>(screen_width/4 + i * screen_width/12, 4*screen_height/5,
                                                        screen_width/4 + (i + 1)*screen_width/12, screen_height)));
        numberImage[i]->setScaleImage(true);
        numberImage[i]->setVisible(false);
        numberImage[i]->setAlignment(ig::EGUIA_CENTER, ig::EGUIA_CENTER, ig::EGUIA_LOWERRIGHT, ig::EGUIA_LOWERRIGHT);
    }
}

void ScreenManager::init_numberText(iv::IVideoDriver *driver)
{
    std::string spath;
    irr::io::path path;
    for(int i = 0; i < 10; ++i)
    {
        spath = "data/" + std::to_string(i) + ".png";
        path =  spath.c_str();
        numberText.push_back(driver->getTexture(path));
    }

}

void ScreenManager::init_pain(iv::IVideoDriver *driver)
{
    iv::ITexture *basicTexture = driver->getTexture("data/basicImage.pcx");
    painImage = g->addImage(ic::rect<irr::s32>(0, 0, screen_width, screen_height));
    painImage->setScaleImage(true);
    painImage->setImage(basicTexture);
    painImage->setColor(iv::SColor(50, 255, 0, 0));
    painImage->setVisible(false);
    painImage->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT, ig::EGUIA_UPPERLEFT, ig::EGUIA_LOWERRIGHT);
}

void ScreenManager::init_life(iv::IVideoDriver *driver)
{
    float horizontalOffset = 0.0f;
    fullHeartTexture = driver->getTexture("data/fullHeart.png");
    emptyHeartTexture = driver->getTexture("data/emptyHeart.png");

    for (int index = 0; index < MAX_LIFE; ++index)
    {
        LifeImage.push_back(g->addImage(ic::rect<irr::s32>(5 + horizontalOffset, 20, 5 + screen_width/18 + horizontalOffset, 20 + screen_height/14)));
        LifeImage[index]->setScaleImage(true);
        LifeImage[index]->setImage(fullHeartTexture);
        LifeImage[index]->setVisible(true);
        LifeImage[index]->setAlignment(ig::EGUIA_UPPERLEFT, ig::EGUIA_UPPERLEFT, ig::EGUIA_UPPERLEFT, ig::EGUIA_UPPERLEFT);
        LifeImage[index]->setVisible(false);
        horizontalOffset += screen_width/14;
    }
}

void ScreenManager::updateLife(unsigned int life)
{
    for(unsigned int index = 0; index < LifeImage.size(); ++index)
    {
        if(index < life)
            LifeImage[index]->setImage(fullHeartTexture);
        else
            LifeImage[index]->setImage(emptyHeartTexture);
    }
}

void ScreenManager::updateNumber()
{
    int decimal = enemyLeft/10;
    numberImage[0]->setImage(numberText[decimal]);
    numberImage[1]->setImage(numberText[enemyLeft - decimal*10]);
}
