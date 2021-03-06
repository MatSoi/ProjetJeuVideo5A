#ifndef PATH_H
#define PATH_H

/*!
 * \file path.h
 * \brief Contient la classe Path qui gere la creation de path pour les personnages
 * \author SOIGNON Matthieu et PASTOR Mickael
 */

#include <irrlicht.h>

namespace ic = irr::core;

/**
 * @brief Classe Path
 * Gere la creation de differents paths possibles pour les personnages
 */
class Path
{
public:
    /**
     * @brief Constructeur vide
     */
    Path(): currentPathNumber(0), sizePath(0), pathTimer(0.0f), timerLimit(0.8f) {}

    /**
     * @brief Constructeur de chemin
     */
    Path(int _currentPathNumber): currentPathNumber(_currentPathNumber), sizePath(0), pathTimer(0.0f), timerLimit(0.8f) {}

    /**
     * @brief Initialisation d'un chemin rectangulaire centré sur center et de taille 2 * radius
     * @param center : centre du chemin rectangulaire
     * @param radius : "rayon" du chemin
     */
    void initRectangularPath(ic::vector3df center, int radius);

    /**
     * @brief Initialisation d'un chemin linéaire selon x centré sur center et de taille 2 * radius
     * @param center : centre du chemin
     * @param radius : "rayon" du chemin
     */
    void initXLinearPath(ic::vector3df center, int radius);

    /**
     * @brief Initialisation d'un chemin linéaire selon z centré sur center et de taille 2 * radius
     * @param center : centre du chemin
     * @param radius : "rayon" du chemin
     */
    void initZLinearPath(ic::vector3df center, int radius);

    /**
     * @brief Initialisation d'un chemin circulaire centré sur center et de taille radius
     * @param center : centre du chemin
     * @param radius : rayon du chemin
     * @param nbrPos : nombre de points dans le cercle
     */
    void initCircularPath(ic::vector3df center, int radius, int nbrPos);

    /**
     * @brief Initialisation d'un chemin custom désigné par les positions passé en arguments
     * @param customPositions : positions qui constituent le chemin custom
     * @param customTimer : timer d'attente entre les positions pour ce chemin
     */
    void initCustomPath(ic::array<ic::vector3df> customPositions, float customTimer);

    /**
     * @brief Reset du path actuelle a null
     */
    void clearPath();

    ic::array<ic::vector3df> pathPositions; /*!< Positions successives pour le path follow de l'ennemi */
    int currentPathNumber;                  /*!< Numero de la position actuelle du path suivi */
    int sizePath;                           /*!< Nombre de positions dans le path */
    float pathTimer;                        /*!< Timer du chemin pour attentes entre les positions du chemin */
    float timerLimit;                       /*!< Limite du timer pour définir l'attente */
};

#endif // PATH_H
