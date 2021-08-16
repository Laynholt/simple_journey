#include "functions.h"

char** create_map()
{
    char** ptrOnTheMap = (char**)malloc(worldMap.iMapHeight * sizeof(char*) + worldMap.iMapSize * sizeof(char));
    char* start = (char*)ptrOnTheMap + worldMap.iMapHeight * sizeof(char*);

    for(uint16_t i = 0; i < worldMap.iMapHeight; i++)
    {
        ptrOnTheMap[i] = start + i * worldMap.iMapWidth;
    }

    // Создаем или перевыделяем память для массива высот
    create_map_of_heights();

    return ptrOnTheMap;
}

void create_map_of_heights()
{
    uint16_t* ptr = (uint16_t*)malloc(worldMap.iMapWidth * sizeof(uint16_t));
    
    // Если это первое выделение, то просто присваиваем, иначе еще копируем старые данные
    if (worldMap.iMapOfHeights == NULL)
        worldMap.iMapOfHeights = ptr;
    else
    {
        memcpy(ptr, worldMap.iMapOfHeights, sizeof(uint16_t) * worldMap.iMapOldWidth);
        free(worldMap.iMapOfHeights);
        worldMap.iMapOfHeights = ptr;
    }
}

void resize_map()
{
    worldMap.iMapSize = worldMap.iMapWidth * worldMap.iMapHeight;

    char** tempPtrOnTheMap = create_map();                        // Создание новой карты
    clear_map(tempPtrOnTheMap, 0);                                // Отчистка новой карты

    // Перенос данных из старой в новую
    for(uint16_t i = 0; i < worldMap.iMapOldHeight; i++)
    {
        memcpy(tempPtrOnTheMap[i], worldMap.map[i], worldMap.iMapOldWidth);
    }

    destruct_map();
    worldMap.map = tempPtrOnTheMap;

    // Вызываем функции генерации окружения
    generate();

    worldMap.iMapOldHeight = worldMap.iMapHeight;
    worldMap.iMapOldWidth = worldMap.iMapWidth;
}

void destruct_map()
{
    free(worldMap.map);
}

void free_all()
{
    free(worldMap.map);
    free(worldMap.iMapOfHeights);
}

void clear_map(char** map, int16_t numOfFrame)
{
    memset(map[numOfFrame], ' ', worldMap.iMapSize);
//    memset(map[numOfFrame], '.', worldMap.iMapSize / 2);
    memset(map[worldMap.iMapHeight / 2], '=', worldMap.iMapWidth);
}

void show_map()
{
    printf("\n");
    for (uint16_t i = CameraPosition.y; i < SCREEN_HEIGHT + CameraPosition.y; i++)
    {
        for(uint16_t j = CameraPosition.x; j < SCREEN_WIDTH + CameraPosition.x; j++)
        {
            putchar(worldMap.map[i][j]);
        }
    }
}

void show_info()
{
    static int16_t clicks = 0;

    printf("Current size of map: [%d] | Current row/column pointers: [%d/%d] | Frames: [%d] | Clicks: [%d]\n",
            worldMap.iMapSize, CameraPosition.y, CameraPosition.x, worldMap.iMapFrameCount, clicks+= 1);
}

void move(int8_t direction)
{
    // Сохраняем позицию курсора
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    printf("\x1b%d", 7);        // Сохраняем позицию курсора

#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    COORD pos;
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
        pos = cbsi.dwCursorPosition;
    else
    {
        pos.X = 0; pos.Y = 0;
    }
#endif

    

    if (direction == RIGHT) 
    {
        if (CameraPosition.x + SCREEN_WIDTH + SPEED >= worldMap.iMapWidth)
        {
            worldMap.iMapWidth += SCREEN_WIDTH;
            resize_map();
            worldMap.iMapFrameCount++;
        }

        CameraPosition.x += SPEED;
    }

    else if (direction == LEFT)
    {
        if ((int32_t)CameraPosition.x - SPEED >= 0)
            CameraPosition.x -= SPEED;
        else if (CameraPosition.x != 0)
            CameraPosition.x--;
    }

    show_map();
    show_info();

    // Получаем сохраненную позицию курсора
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    usleep(15000);
    // https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/
    printf("\x1b%d", 8);        // Получаем сохраненную позицию курсора
#elif defined(WIN32)|| defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    SetConsoleCursorPosition(hConsoleOutput, pos);
#endif
}

void generate()
{
    static int16_t frame = 0;

    if (frame == 0)
    {
        srand(SEED);
        // ландшафт
        generate_sun_and_clouds(0, SCREEN_WIDTH);
        generate_landscape(0, SCREEN_HEIGHT / 3, SCREEN_WIDTH, SCREEN_HEIGHT);
        generate_trees(0, SCREEN_WIDTH);
    }

    else
    { 
        generate_sun_and_clouds(worldMap.iMapOldWidth, worldMap.iMapWidth);
        generate_landscape(worldMap.iMapOldWidth, SCREEN_HEIGHT / 3, worldMap.iMapWidth, SCREEN_HEIGHT);
        generate_trees(worldMap.iMapOldWidth, worldMap.iMapWidth);
    }

    frame++;
}

void generate_landscape(uint32_t min_w, uint16_t min_h, uint32_t max_w, uint16_t max_h)
{
   uint16_t start_y = SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT / 4) + rand() % 3) - 1;
   uint16_t end_y = SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT / 4) + rand() % 5) - 1;

   // Отмечаем границы отрезков
   worldMap.map[start_y][min_w] = 'O';
   worldMap.map[end_y][max_w - 1] = 'O';

   // Заносим их в массив высот
   worldMap.iMapOfHeights[min_w] = start_y;
   worldMap.iMapOfHeights[max_w - 1] = end_y;
 
   midpoint_displacement(min_w, start_y, max_w - 1, end_y, 0.3f);
}

void midpoint_displacement(uint32_t leftX, uint16_t leftY, uint32_t rightX, uint16_t rightY, float roughness)
{
    uint32_t length = rightX - leftX;
    if (length == 1) return;
    
    uint16_t randomVal = (rand() % (int32_t)(roughness * (length) * 2)) - length * roughness;
    uint16_t h = (leftY + rightY) / 2 + randomVal;

    // Генерируем случайную высоту
    while(h >= worldMap.iMapHeight || h < 0)
    {
        randomVal = (rand() % (int32_t)(roughness * (length) * 2)) - length * roughness;
        h = (leftY + rightY) / 2 + randomVal;
    }
    worldMap.iMapOfHeights[leftX + length / 2] = h;

    char palette_ground[6] = " .:oO";
    worldMap.map[h][leftX + length / 2] = palette_ground[4];
  
    // Плавно зарисовываем грунт ландшафта
    for(uint16_t y = h; y < worldMap.iMapHeight; y++)
    {
        uint16_t deltaY = worldMap.iMapHeight - h;
        char color;

        if (y > h && y <= h + deltaY / 4) color = palette_ground[3];
        else if (y > h + deltaY / 4 && y <= h + deltaY / 2) color = palette_ground[2];
        else if (y > h + deltaY / 2 && y <= h + 3 * deltaY / 4) color = palette_ground[1];
        else color = palette_ground[0];

        worldMap.map[y][leftX + length / 2] = color;
    }

    // Вызываем для левого и правого отрезков по рекурсии
    midpoint_displacement(leftX, leftY, leftX + length / 2, h, roughness);
    midpoint_displacement(leftX + length / 2, h, rightX, rightY, roughness);
}

 void generate_trees(uint32_t min_w, uint32_t max_w)
{
    char* trees[] = {
        " ^ /|\\/|\\",
        " # ### | ",
        "~#_#x# | "
    };


    uint32_t x, y;
    int16_t randomTree;

    x = y = 0;

    if (TREES_ON)
    {
        for(int16_t k = 0; k < TREES_NUM; k++)
        {
            // Генерируем случайный Х и получаем соответствующую ему высоту из массива высот
            x = min_w + rand() % (max_w - min_w - 15) + 5;
            y = worldMap.iMapOfHeights[x] - 2;

            randomTree = rand() % 3;

            // Деревья имеют размер 3х3
            for(int16_t i = 0; i < 3; i++)
            {
                for(int16_t j = 0; j < 3; j++)
                {
                    if (y + i > 0 && y + i < worldMap.iMapHeight && x + j > min_w && x + j < max_w)
                        worldMap.map[y + i][x + j] = trees[randomTree][i * 3 + j]; 
                }
            }
        }
    }
}

void generate_sun_and_clouds(uint32_t min_w, uint32_t max_w)
{
    char* clouds[] = {
        "   .--.   .-(    ). (__.__)__)",
        "    .-.      (   ).   (___(__)",
        " _-.__     ( __ )._ (___(__)_)"
    };

    uint32_t x, y;
    int16_t randomCloud = 0;

    if (CLOUDS_ON)
    {
        for(int16_t k = 0; k < CLOUDS_NUM; k++)
        {
            // Первые 4 облака рисуем у границы, чтобы убрать эффект рваности
            if (k < 4)  x = min_w + rand() % 2;
            else        x = min_w + rand() % (max_w - min_w - k) + k;
            y = rand() % 5;

            randomCloud = rand() % 3;

            for(int16_t i = 0; i < 3; i++)
            {
                for(int16_t j = 0; j < 10; j++)
                {
                    if (x + j < worldMap.iMapWidth)
                        worldMap.map[CameraPosition.y + i + y][x + j] = clouds[randomCloud][i * 10 + j];
                }
            }
        }
    }
}
