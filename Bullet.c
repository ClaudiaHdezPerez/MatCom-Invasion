#include "Utils.h"

<<<<<<< HEAD
// pthread_mutex_t lock;
=======
int score = 0;
int big_enemy;
int active = 0;

typedef struct Bullet {
    int x;
    int y;
    int active;
} Bullet;

Enemy enemies[MAX_ENEMIES];
pthread_mutex_t lock; 
>>>>>>> 34c491be860adbe943c3da35ac855e1a67c41cf0

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

<<<<<<< HEAD
=======
void colision(Bullet *bullet)
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

            int y = enemies[i].y;
            int x = enemies[i].x;

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

>>>>>>> 34c491be860adbe943c3da35ac855e1a67c41cf0
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