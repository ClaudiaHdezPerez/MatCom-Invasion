#include <ncursesw/ncurses.h>
#include <pthread.h>
#include <unistd.h>

typedef struct SpaceShip
{
    int x;
    int y;
    int lifes;
    int game_over;
} SpaceShip;

pthread_mutex_t lock;

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

void draw_lifes(SpaceShip *spaceShip, int max_x, int max_y)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    mvprintw(1, max_x - 17, "%s", "LIFES");
    mvprintw(1, max_x - 10, "%s", "      ");

    for (int i = 0; i < 2 * spaceShip->lifes; i+=2)
        mvprintw(1, max_x - 10 + i, "%s", "♥");

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void die(SpaceShip *spaceShip, int max_x, int max_y)
{
    if (!spaceShip->game_over)
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

            sleep(2);
            spaceShip->game_over = 1;
            return;
        }

        spaceShip->lifes--;
        draw_lifes(spaceShip, max_x, max_y);
    }
}

