#include "Utils.h"

<<<<<<< HEAD
// extern pthread_mutex_t lock;
// extern pthread_mutex_t lock_spaceShip;
=======
pthread_mutex_t lock_spaceShip;

typedef struct SpaceShip
{
    int x;
    int y;
    int lifes;
    int game_over;
} SpaceShip;

pthread_mutex_t lock;
>>>>>>> 34c491be860adbe943c3da35ac855e1a67c41cf0

void draw_spaceShip(SpaceShip *spaceShip)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    char *str1 = "  /\\  ";
    char *str2 = "_//\\\\_";

    mvprintw(spaceShip->y, spaceShip->x, "%s", str1);
    mvprintw(spaceShip->y + 1, spaceShip->x, "%s", str2);
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void erase_spaceShip(SpaceShip *spaceShip)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    char *space1 = "      ";
    char *space2 = "      ";

    mvprintw(spaceShip->y, spaceShip->x, "%s", space1);
    mvprintw(spaceShip->y + 1, spaceShip->x, "%s", space2);
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void* move_spaceShip(void* arg)
{
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

            erase_spaceShip(spaceShip);

            pthread_mutex_lock(&lock_spaceShip);
            spaceShip->x = max_x / 2;
            spaceShip->y = max_y - 3;  
            pthread_mutex_unlock(&lock_spaceShip);

            draw_spaceShip(spaceShip);    
        }

        int y = spaceShip->y;
        int x = spaceShip->x;

        int ch = getch();
        if (ch != ERR) // Solo procesa la entrada si se presionó una tecla
        {  
            pthread_mutex_lock(&lock_spaceShip);
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
                case ' ':
                    fire_bullet(spaceShip, spaceShip->x + 2, spaceShip->y - 1);
                    break;
            }

            pthread_mutex_unlock(&lock_spaceShip);

            erase_spaceShip(spaceShip);

            pthread_mutex_lock(&lock_spaceShip);
            spaceShip->y = y;
            spaceShip->x = x;
            pthread_mutex_unlock(&lock_spaceShip);

            draw_spaceShip(spaceShip);
        }     
    }
    
    pthread_exit(NULL);
}

void draw_lifes(SpaceShip *spaceShip, int max_x)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    mvprintw(1, max_x - 17, "%s", "LIFES");
    mvprintw(1, max_x - 10, "%s", "      ");

    for (int i = 0; i < 2 * spaceShip->lifes; i += 2)
        mvprintw(1, max_x - 10 + i, "%s", "♥");

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void die(SpaceShip *spaceShip, int max_x, int max_y)
{
    int y = spaceShip->y;
    int x = spaceShip->x;           
    
    erase_spaceShip(spaceShip);
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    mvprintw(y, x, "%s", "  **  ");
    mvprintw(y + 1, x, "%s", "** * *");

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar

    usleep(300000);
    
    erase_spaceShip(spaceShip);
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar        
    mvprintw(y, x, "%s", " *   *");
    mvprintw(y + 1, x, "%s", "   * *");

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar

    usleep(300000);

    erase_spaceShip(spaceShip);
    draw_spaceShip(spaceShip);

    if (spaceShip->lifes <= 0)
    {
        pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
        mvprintw(max_y / 2, max_x / 2 - 4, "%s", "GAME OVER");
        mvprintw(max_y / 2 + 1, max_x / 2 - 1, "%s", "*_*");    
        pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar

        usleep(2000000);
        
        spaceShip->game_over = 1;
        return;
    }

    spaceShip->lifes--;
<<<<<<< HEAD
    draw_lifes(spaceShip, max_x);
=======
    draw_lifes(spaceShip, max_x, max_y);
>>>>>>> 34c491be860adbe943c3da35ac855e1a67c41cf0
}