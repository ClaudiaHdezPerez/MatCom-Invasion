#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include "SpaceShip.c"

typedef struct Page {
    int start;
    int width;
    int empty;
    int age;
} Page;

typedef struct Enemy {
    int x;
    int y;
    int lifes;
    int color;
    int active;
    int number;
    Page *page;
} Enemy;

typedef struct FreeList {
    int block;
    int space;
    struct FreeList *next;
} FreeList;

typedef struct enemies_thread {
    Enemy *enemy;
    SpaceShip *spaceShip;
} enemies_thread;

#define MAX_ENEMIES 40

int max_x1, max_y1;

Enemy enemies[MAX_ENEMIES];
pthread_mutex_t lock; // Declara el mutex

void DrawEnemy(Enemy enemy)
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
        mvprintw(y, x, "%s", " _ ");
        mvprintw(y + 1, x, "%s", "<0>");
    }

    attroff(COLOR_PAIR(enemy.number));

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void EraseEnemy(Enemy enemy)
{
    pthread_mutex_lock(&lock); // Bloquea el mutex antes de borrar
    
    int y = enemy.y;
    int x = enemy.x;
    
    mvprintw(y, x, "%s", "   ");
    mvprintw(y + 1, x, "%s", "   ");

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void* MoveEnemy(void* arg)
{
    getmaxyx(stdscr, max_y1, max_x1);

    enemies_thread *struct_thread_enemy = (enemies_thread*)arg;
    Enemy *enemy = struct_thread_enemy->enemy;
    SpaceShip *spaceShip = struct_thread_enemy->spaceShip;

    int x = enemy->x;
    while(enemy->active && !spaceShip->game_over) {
        for (int i = 0; i < 3; i++)
        {
            if (spaceShip->game_over)
                break;

            EraseEnemy(*enemy);
            enemy->x++;

            if (i == 2)
            {
                enemy->y++;
                enemy->x = x;
                i = -1;
            } 

            DrawEnemy(*enemy);

            usleep(400000);
            if(enemy->y > max_y1 - 4) {
                enemy->active = 0;
                enemy->page->empty = 1;
                EraseEnemy(*enemy);
                break;
            }
        }
    }
    
    pthread_exit(NULL);
}