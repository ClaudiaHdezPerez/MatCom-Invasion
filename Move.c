#include <stdlib.h>
#include "Bullet.c"

#define MAX_BULLETS 20
#define ENEMY_BULLETS 5

typedef struct Bullet_Enemy {
    int x;
    int y;
    int active;
} Bullet_Enemy;

Bullet bullets[MAX_BULLETS];
Bullet_Enemy enemy_bullets[ENEMY_BULLETS];

typedef struct bullet_spaceShip {
    Bullet_Enemy *bullet;
    SpaceShip *spaceShip;
} bullet_spaceShip;

void draw_bullet_enemy(Bullet_Enemy bullet)
{
    pthread_mutex_lock(&lock);

    int y = bullet.y;
    int x = bullet.x;

    start_color();

    use_default_colors();

    init_pair(22, COLOR_GREEN, -1);

    attron(COLOR_PAIR(22));

    mvprintw(y, x, "%s", "*");

    attroff(COLOR_PAIR(22));

    refresh();
    pthread_mutex_unlock(&lock);
}

void erase_bullet_enemy(Bullet_Enemy bullet)
{
    pthread_mutex_lock(&lock);
    
    int y = bullet.y;
    int x = bullet.x;
    
    mvprintw(y, x, "%s", " ");
    refresh();
    pthread_mutex_unlock(&lock); 
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

void* fire_bullet_big_enemy(void* arg)
{
    bullet_spaceShip* args = (bullet_spaceShip*)arg;
    Bullet_Enemy *bullet = args->bullet;
    SpaceShip *spaceShip = args->spaceShip;

    while(bullet->active) {
        erase_bullet_enemy(*bullet);
        bullet->y++;
        colision_bullet_spaceShip(bullet, spaceShip);
        draw_bullet_enemy(*bullet);

        if(bullet->y > max_y1 - 4 || !bullet->active) {
            bullet->active = 0;
            erase_bullet_enemy(*bullet);
        }

        usleep(70000);
    }
    
    free(arg);
    pthread_exit(NULL);
}

void bullet_big_enemy(Enemy *enemy, SpaceShip *spaceShip)
{
    for(int i = 0; i < ENEMY_BULLETS; i++) 
    {
        if (spaceShip->game_over)
            break;

        if(!enemy_bullets[i].active) 
        {
            enemy_bullets[i].x = enemy->x + 7;
            enemy_bullets[i].y = enemy->y + 5;
            enemy_bullets[i].active = 1;
            pthread_t thread_id;
            bullet_spaceShip *args = (bullet_spaceShip*)malloc(sizeof(bullet_spaceShip));
            args->bullet = &enemy_bullets[i];
            args->spaceShip = spaceShip;
            pthread_create(&thread_id, NULL, fire_bullet_big_enemy, args);
            break;
        }
    }
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

void fire_bullet(SpaceShip *spaceShip, int x, int y) 
{
    for(int i = 0; i < MAX_BULLETS; i++) 
    {
        if (spaceShip->game_over)
            break;

        if(!bullets[i].active) 
        {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].active = 1;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, bullet_thread, &bullets[i]);
            break;
        }
    }
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