#include "Utils.h"

void colision_bullet_enemy(Bullet *bullet)
{
    int heigth = 2;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (big_enemy && enemies[MAX_ENEMIES - 1].active)
        {
            i = MAX_ENEMIES - 1;
            heigth = 5;
        }

        if (bullet->x <= enemies[i].x + enemies[i].width && bullet->x >= enemies[i].x
            && bullet->y <= enemies[i].y + heigth && enemies[i].active)
        {
            enemies[i].lifes--;

            start_color();

            use_default_colors();

            init_pair(enemies[i].number, enemies[i].color, -1);

            attron(COLOR_PAIR(enemies[i].number));

            erase_enemy(enemies[i]);
            pthread_mutex_lock(&lock); 
            mvprintw(enemies[i].y, enemies[i].x, "%s", "** ");
            pthread_mutex_unlock(&lock); 

            refresh();
            attroff(COLOR_PAIR(enemies[i].number));

            usleep(200000);
            
            attron(COLOR_PAIR(enemies[i].number));
            
            erase_enemy(enemies[i]);
            pthread_mutex_lock(&lock); 
            mvprintw(enemies[i].y, enemies[i].x, "%s", " * ");
            pthread_mutex_unlock(&lock); 

            refresh();
            attroff(COLOR_PAIR(enemies[i].number));
            usleep(200000);
            erase_enemy(enemies[i]);
            draw_enemy(enemies[i]);
            
            refresh();

            if (enemies[i].lifes <= 0)
            {
                erase_enemy(enemies[i]);
                enemies[i].active = 0;

                if (!enemies[i].killed)
                {
                    increrase_score(enemies[i]);
                    enemies[i].killed = 1;
                }
            }

            bullet->active = 0;
            return;
        }
    }
}

void colision_spaceShip_enemy(SpaceShip *spaceShip, Enemy *enemy)
{
    int x = spaceShip->x;
    int y = spaceShip->y;

    for (int i = 0; i <= enemy->width; i++)
    {
        if (!spaceShip->game_over && enemy->active && 
            enemy->x + i <= x + 7 && enemy->x + i >= x && enemy->y >= y - 2)
        {
            enemy->lifes = 0;
            erase_enemy(*enemy);
            die(spaceShip, max_x1, max_y1);
            break;
        }
    }
}

void colision_bullet_spaceShip(Bullet_Enemy *bullet, SpaceShip *spaceShip)
{
    int x = spaceShip->x;
    int y = spaceShip->y;

    if (!spaceShip->game_over && bullet->active && 
    bullet->x <= x + 7 && bullet->x >= x && bullet->y >= y - 2)
    {
        bullet->active = 0;
        erase_bullet_enemy(*bullet);
        die(spaceShip, max_x1, max_y1);
    } 
}