#include "path.h"


void Path::initRectangularPath(ic::vector3df center, int radius)//Construit un chemin correspondant à un carré centré sur center et de taille 2 * radius
{
    pathPositions.push_back(center + ic::vector3df(-radius, 0, -radius));
    pathPositions.push_back(center + ic::vector3df(-radius, 0, +radius));
    pathPositions.push_back(center + ic::vector3df(+radius, 0, +radius));
    pathPositions.push_back(center + ic::vector3df(+radius, 0, -radius));

    sizePath = pathPositions.size() - 1;
}



