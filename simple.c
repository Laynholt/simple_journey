#include "libraries.h"
#include "functions.h"
#include "kbhit.h"

static sig_atomic_t end = 0;
static void sighandler(int signo)   { end = 1;}

void clear_screen() 
{
    printf("\x1b[%dJ", 2);
}

int main()
{
    bool exit = false;

    // Обработчик сигналов для клавиш
    term_setup(sighandler);

    // Создание и генерация карты
    worldMap.map = create_map();
    clear_map(worldMap.map, 0);
    generate();
    move(STAND);

    while (!exit)
    {
        if(kbhit())
        {
            if(keydown(KEY_ESC))
                exit = true;
            else if (keydown(KEY_D))
                move(RIGHT);
            else if (keydown(KEY_A))
                move(LEFT);
        }
    }

    term_restore();
    free_all();

    clear_screen();
    return 0;
}
