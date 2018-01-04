#ifndef ID_LIST_H
#define ID_LIST_H

/*!
 * \file ID_list.h
 * \brief Liste des ID des elements presents dans la scene
 * \author SOIGNON Matthieu et PASTOR Mickael
 */


/**
 * @brief ID_List enum
 * Enumeration des ID des elements de la scene
 */
enum ID_List
{
    ID_MAP = 1 << 0,
    ID_PLAYER = 1 << 1,
    ID_ENEMY_1 = 1 << 2,
    ID_ENEMY_2 = 1 << 3,
    ID_ENEMY_3 = 1 << 4,
    ID_ENEMY_4 = 1 << 5,
    ID_ENEMY_5 = 1 << 6,
    ID_ENEMY_6 = 1 << 7,
    ID_ENEMY_7 = 1 << 8
};

#endif
