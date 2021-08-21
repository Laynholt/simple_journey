#ifndef _MYCONFIG_H_
#define _MYCONFIG_H_

#include "libraries.h"

#define SCREEN_WIDTH  149           // Ширина экрана
#define SCREEN_HEIGHT 32            // Высота экрана

#define SEED 0

#define TREES_ON 1
#define TREES_NUM 7

#define CLOUDS_ON 1
#define CLOUDS_NUM 20

#define SPEED 2

typedef struct
{
    uint32_t iMapWidth;              // Ширина карты
    uint16_t iMapHeight;             // Высота карты

    uint32_t iMapOldWidth;           // Необходимы для переноса данных при ресайзе
    uint16_t iMapOldHeight;

    uint16_t iMapFrameCount;         // Количество созданных кадров (сцен)

    uint32_t iMapSize;

    char** map;
    uint16_t* iMapOfHeights;        // Карта высот

}sMap;

typedef struct
{
    uint32_t x;
    uint16_t y;

}iPoint;

enum eDirection
{
    STAND = 0,
    RIGHT = 1,
    LEFT  = 2,
    UP    = 3,
    DOWN  = 4,
};


extern sMap worldMap;
extern iPoint CameraPosition;
extern char* palette_ground; 

#endif
