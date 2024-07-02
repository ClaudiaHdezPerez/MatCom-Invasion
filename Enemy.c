#include <stdio.h>
#include "SpaceShip.c"

#define MAX_ENEMIES 20

typedef struct Page {
    int start;
    int width;
    int age;
} Page;

typedef struct Enemy {
    int x;
    int y;
    int width;
    int lifes;
    int color;
    int active;
    int number;
    int block;
    Page *page;
} Enemy;

typedef struct enemies_thread {
    Enemy *enemy;
    SpaceShip *spaceShip;
} enemies_thread;

int max_x1, max_y1;

void draw_enemy(Enemy enemy)
{
    pthread_mutex_lock(&lock); // Bloquea el mutex antes de dibujar

    int y = enemy.y;
    int x = enemy.x;

    start_color();

    use_default_colors();

    init_pair(enemy.number, enemy.color, -1);

    attron(COLOR_PAIR(enemy.number));

    if (enemy.color == COLOR_GREEN)
    {
        mvprintw(y, x, "%s", "    \\ /   ");
        mvprintw(y + 1, x, "%s", "  \\(-_-)/ ");
        mvprintw(y + 2, x, "%s", "   (___)  ");
        mvprintw(y + 3, x, "%s", "   /   \\ ");
    }

    else
    {
        char str[4];
        sprintf(str, "<%d>", enemy.lifes); 
        mvprintw(y, x, "%s", " _ ");
        mvprintw(y + 1, x, "%s", str);
    }

    attroff(COLOR_PAIR(enemy.number));

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void erase_enemy(Enemy enemy)
{
    pthread_mutex_lock(&lock); // Bloquea el mutex antes de borrar
    
    int y = enemy.y;
    int x = enemy.x;
    
    if (enemy.color == COLOR_GREEN)
    {
        mvprintw(y, x, "%s", "          ");
        mvprintw(y + 1, x, "%s", "          ");
        mvprintw(y + 2, x, "%s", "          "); 
        mvprintw(y + 3, x, "%s", "         ");
    }

    else
    {
        mvprintw(y, x, "%s", "      ");
        mvprintw(y + 1, x, "%s", "      ");
    }

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}