#include "libraries.h"
#include "functions.h"

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
#include "kbhit.h"

static sig_atomic_t end = 0;
static void sighandler(int signo)   { end = 1;}
#endif

void clear_screen() 
{
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    printf("\x1b[%dJ", 2);
#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    system("cls");
#endif
}

int main()
{
    bool exit = false;

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    // Обработчик сигналов для клавиш
    term_setup(sighandler);
#endif

    // Создание и генерация карты
    worldMap.map = create_map();
    clear_map(worldMap.map, 0);
    generate();
    move(STAND);

    while (!exit)
    {
#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
        if(kbhit())
        {
            if(keydown(KEY_ESC))
                exit = true;
            else if (keydown(KEY_D))
                move(RIGHT);
            else if (keydown(KEY_A))
                move(LEFT);
        }
#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    // Windows code
#endif
    }

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
    term_restore();
#endif

    free_all();
    clear_screen();

    return 0;
}
