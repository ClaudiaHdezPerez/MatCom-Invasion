#include <ncursesw/ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "Bullet.c"

typedef struct SpaceShip
{
    int X;
    int Y;
    int Lifes;
    int game_over;
} SpaceShip;


void DrawSpaceShip(SpaceShip *spaceShip)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    char *str1 = "  /\\  ";
    char *str2 = "_//\\\\_";

    mvprintw(spaceShip->Y, spaceShip->X, "%s", str1);
    mvprintw(spaceShip->Y + 1, spaceShip->X, "%s", str2);
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void EraseSpaceShip(SpaceShip *spaceShip)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    char *space1 = "      ";
    char *space2 = "      ";

    mvprintw(spaceShip->Y, spaceShip->X, "%s", space1);
    mvprintw(spaceShip->Y + 1, spaceShip->X, "%s", space2);
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void DrawLifes(SpaceShip *spaceShip, int max_x, int max_y)
{
    mvprintw(1, max_x - 17, "%s", "LIFES");
    mvprintw(1, max_x - 10, "%s", "      ");

    for (int i = 0; i < 2 * spaceShip->Lifes; i+=2)
        mvprintw(1, max_x - 10 + i, "%s", "♥");
}

void Die(SpaceShip *spaceShip, int max_x, int max_y)
{
    int y = spaceShip->Y;
    int x = spaceShip->X;
    if (spaceShip->Lifes > 0)
        spaceShip->Lifes--;

    else 
    {
        mvprintw(max_y / 2, max_x / 2 - 4, "%s", "GAME OVER");
        mvprintw(max_y / 2 + 1, max_x / 2 - 1, "%s", "*_*");
        spaceShip->game_over = 1;
    }

    EraseSpaceShip(spaceShip);
    mvprintw(y, x, "%s", "  **  ");
    mvprintw(y + 1, x, "%s", "** * *");

    refresh();
    usleep(300000);
    
    EraseSpaceShip(spaceShip);
    mvprintw(y, x, "%s", " *   *");
    mvprintw(y + 1, x, "%s", "   * *");

    refresh();
    usleep(300000);
    EraseSpaceShip(spaceShip);
    DrawSpaceShip(spaceShip);
    DrawLifes(spaceShip, max_x, max_y);
}

void* MovSpaceShip(void* arg)
{
    // Thread *thread = ;
    SpaceShip *spaceShip = (SpaceShip*) arg;  

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    while (!spaceShip->game_over)
    {
        int new_x, new_y;
        getmaxyx(stdscr, new_y, new_x);

        if (new_y != max_y || new_x != max_x)
        {
            max_x = new_x;
            max_y = new_y;   

            EraseSpaceShip(spaceShip);

            spaceShip->X = max_x / 2;
            spaceShip->Y = max_y - 3;  

            DrawSpaceShip(spaceShip);    
        }

        // pthread_mutex_lock(&mutex);
        int y = spaceShip->Y;
        int x = spaceShip->X;

        int ch = getch();
        if (ch != ERR) // Solo procesa la entrada si se presionó una tecla
        {  
            switch(ch) 
            {
                case KEY_UP:
                    y = y > (max_y / 2) + 5 ? y - 1 : y;
                    break;
                case KEY_DOWN:
                    y = y < (max_y - 3) ? y + 1 : y;
                    break;
                case KEY_LEFT:
                    x = x > 3 ? x - 1 : x;
                    break;
                case KEY_RIGHT:
                    x = x < (max_x - 7) ? x + 1 : x;
                    break;
                case 27:
                    spaceShip->game_over = 1;
                    break;
                case 97:
                    Die(spaceShip, max_x, max_y);
                    break;
                case ' ':
                    fire_bullet(spaceShip->X + 2, spaceShip->Y - 1);
                    break;
            }

            EraseSpaceShip(spaceShip);

            spaceShip->Y = y;
            spaceShip->X = x;

            DrawSpaceShip(spaceShip);
        }     
    }
    
    sleep(2);
    pthread_exit(NULL);
}