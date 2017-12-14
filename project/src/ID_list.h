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
    ID_ENEMY = 1 << 2
};

#endif
