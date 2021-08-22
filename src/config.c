#include "config.h"

sMap worldMap  = {.iMapWidth = SCREEN_WIDTH,
                  .iMapHeight = SCREEN_HEIGHT,
                  .iMapOldWidth = SCREEN_WIDTH,
                  .iMapOldHeight = SCREEN_HEIGHT,
                  .iMapFrameCount = 1,
                  .iMapSize = SCREEN_HEIGHT * SCREEN_WIDTH,
                  .iMapOfHeightsSize = 0,
                  .iMapOfCloudsSize = 0,
                  .iMapFullSize = SCREEN_HEIGHT * SCREEN_WIDTH,
                  .map = NULL,
                  .mapOfHeights = NULL,
                  .mapOfClouds = NULL
};


iPoint CameraPosition = {
                  .x = 0,
                  .y = 0
};


char* palette_ground = " .:oO";
