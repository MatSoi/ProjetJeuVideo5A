/*!
 * \file path.cpp
 * \brief Implementation des fonctions de la classe Path.
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include "path.h"

//Construit un chemin correspondant à un carré centré sur center et de taille 2 * radius
void Path::initRectangularPath(ic::vector3df center, int radius)
{
    pathPositions.push_back(center + ic::vector3df(-radius, 0, -radius));
    pathPositions.push_back(center + ic::vector3df(-radius, 0, +radius));
    pathPositions.push_back(center + ic::vector3df(+radius, 0, +radius));
    pathPositions.push_back(center + ic::vector3df(+radius, 0, -radius));

    sizePath = pathPositions.size() - 1;
    timerLimit = 0.8f;
}

//Construit un chemin correspondant à une ligne selon x centré sur center et de taille 2 * radius
void Path::initXLinearPath(ic::vector3df center, int radius)
{
    pathPositions.push_back(center + ic::vector3df(-radius, 0, 0));
    pathPositions.push_back(center + ic::vector3df(+radius, 0, 0));

    sizePath = pathPositions.size() - 1;
    timerLimit = 1.f;
}

//Construit un chemin correspondant à une ligne selon x centré sur center et de taille 2 * radius
void Path::initZLinearPath(ic::vector3df center, int radius)
{
    pathPositions.push_back(center + ic::vector3df(0, 0, -radius));
    pathPositions.push_back(center + ic::vector3df(0, 0, +radius));

    sizePath = pathPositions.size() - 1;
    timerLimit = 1.f;
}

//Construit un chemin correspondant à un cercle centré sur center et de rayon radius  avec nbrPos nombres de positions
void Path::initCircularPath(ic::vector3df center, int radius, int nbrPos)
{
    for (float theta = 0; theta < (2.f * M_PI * (1.f - 1.f/nbrPos)); theta += ((2 * M_PI)/nbrPos))
        pathPositions.push_back(center + ic::vector3df(radius * cos(theta), 0, radius * sin(theta)));

    sizePath = pathPositions.size() - 1;
    timerLimit = 0;
}

void Path::initCustomPath(ic::array<ic::vector3df> customPositions, float customTimer)
{
    pathPositions = customPositions;
    sizePath = pathPositions.size() - 1;

    timerLimit = customTimer;
}

//Reset du path à null
void Path::clearPath()
{
    pathPositions.clear();
    sizePath = pathPositions.size() - 1;
    currentPathNumber = 0;
}



