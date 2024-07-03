#include "Utils.h"

// extern int max_x1, max_y1;
// extern pthread_mutex_t lock;

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
        char str[12];
        sprintf(str, "     %d    ", enemy.lifes); 
        mvprintw(y, x, "%s", "    \\ /   ");
        mvprintw(y + 1, x, "%s", "  \\(-_-)/ ");
        mvprintw(y + 2, x, "%s", "   (___)  ");
        mvprintw(y + 3, x, "%s", "   /   \\ ");
        mvprintw(y + 4, x, "%s", str);
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
        mvprintw(y + 4, x, "%s", "         ");
    }

    else
    {
        mvprintw(y, x, "%s", "      ");
        mvprintw(y + 1, x, "%s", "      ");
    }

    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void* move_enemy(void* arg)
{
    getmaxyx(stdscr, max_y1, max_x1);

    enemies_thread *struct_thread_enemy = (enemies_thread*)arg;
    Enemy *enemy = struct_thread_enemy->enemy;
    SpaceShip *spaceShip = struct_thread_enemy->spaceShip;

    int x = enemy->x;
    int use_enemy = 1;

    while(use_enemy && !spaceShip->game_over) {
        if (enemy->color == COLOR_GREEN)
        {
            int count = 0;
            for (int i = 2; i < (max_y1 - 1); i++) {
                srand(time(NULL)); 

                erase_enemy(*enemy);
                int width_screen = max_x1 - 15;
                enemy->x = (rand() % width_screen) + 3;

                if (enemy->x + i + 11 >= max_x1 - 1)
                    break;

                count++;
                if (count == 2)
                {
                    count = 0;
                    enemy->y++;
                }

                draw_enemy(*enemy);

                usleep(400000);

                for (int i = 0; i < 11; i++)
                {
                    if (enemy->x + i + 11 >= max_x1 - 1)
                        break;

                    if (spaceShip->game_over)
                        break;

                    erase_enemy(*enemy);

                    enemy->x++;

                    if (i % 5 == 0)
                        bullet_big_enemy(enemy, spaceShip);

                    draw_enemy(*enemy);

                    usleep(300000);
                }

                colision_spaceShip_enemy(spaceShip, enemy);

                if(enemy->lifes <= 0) {
                    enemy->active = 0;
                    erase_enemy(*enemy);
                    use_enemy = 0;
                    big_enemy = 0;
                    active = 0;
                    break;
                }

                if (enemy->y > max_y1 - 12)
                {
                    spaceShip->lifes = 0;
                    die(spaceShip, max_x1, max_y1);
                    break;
                }
            }
        }

        else
        {
            for (int i = 0; i < 3; i++)
            {
                if (spaceShip->game_over)
                    break;

                erase_enemy(*enemy);
                enemy->x++;

                if (i == 2)
                {
                    enemy->y++;
                    enemy->x = x;
                } 

                draw_enemy(*enemy);

                usleep(200000);

                colision_spaceShip_enemy(spaceShip, enemy);

                if(enemy->y > max_y1 - 4 || !enemy->active || enemy->lifes <= 0) {
                    enemy->active = 0;
                    enemy->page->age++;
                    erase_enemy(*enemy);
                    use_enemy = 0;
                    break;
                }
            }
        }
    }
    
    pthread_exit(NULL);
}