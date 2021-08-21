#include "config.h"

sMap worldMap  = {.iMapWidth = SCREEN_WIDTH,
                  .iMapHeight = SCREEN_HEIGHT,
                  .iMapOldWidth = SCREEN_WIDTH,
                  .iMapOldHeight = SCREEN_HEIGHT,
                  .iMapFrameCount = 1,
                  .iMapSize = SCREEN_HEIGHT * SCREEN_WIDTH,
                  .map = NULL,
                  .iMapOfHeights = NULL
};


iPoint CameraPosition = {
                  .x = 0,
                  .y = 0
};


char* palette_ground = " .:oO";
