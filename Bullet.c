#include "Utils.h"

// pthread_mutex_t lock;

void draw_bullet(Bullet bullet)
{
    pthread_mutex_lock(&lock);

    int y = bullet.y;
    int x = bullet.x;

    mvprintw(y, x, "%s", "*");
    refresh();
    pthread_mutex_unlock(&lock);
}

void increrase_score(Enemy enemy)
{
    switch (enemy.color)
    {
        case COLOR_YELLOW:
            score++;
            break;
        
        case COLOR_CYAN:
            score += 3;
            break;

        case COLOR_RED:
            score += 5;
            break;

        default:
            score += 15;
            break;
    }
}

void erase_bullet(Bullet bullet)
{
    pthread_mutex_lock(&lock);
    
    int y = bullet.y;
    int x = bullet.x;
    
    mvprintw(y, x, "%s", " ");
    refresh();
    pthread_mutex_unlock(&lock); 
}

void* bullet_thread(void* arg) 
{
    Bullet* bullet = (Bullet*)arg;
    while(bullet->active) {
        erase_bullet(*bullet);
        bullet->y--;
        colision_bullet_enemy(bullet);
        draw_bullet(*bullet);

        if(bullet->y <= 3 || !bullet->active) {
            bullet->active = 0;
            erase_bullet(*bullet);
        }

        usleep(50000);
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