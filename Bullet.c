#include <ncursesw/ncurses.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Bullet {
    int x;
    int y;
    int active;
    pthread_t thread_id;
} Bullet;

#define MAX_BULLETS 20

pthread_mutex_t lock; // Declara el mutex

void DrawBullet(Bullet bullet)
{
    pthread_mutex_lock(&lock); // Bloquea el mutex antes de dibujar

    int y = bullet.y;
    int x = bullet.x;

    mvprintw(y, x, "%s", "*");
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void EraseBullet(Bullet bullet)
{
    pthread_mutex_lock(&lock); // Bloquea el mutex antes de borrar
    
    int y = bullet.y;
    int x = bullet.x;
    
    mvprintw(y, x, "%s", " ");
    refresh();
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

Bullet bullets[MAX_BULLETS];

void* bullet_thread(void* arg) 
{
    Bullet* bullet = (Bullet*)arg;
    while(bullet->active) {
        EraseBullet(*bullet);
        bullet->y--;
        DrawBullet(*bullet);

        usleep(50000);
        if(bullet->y <= 3) {
            bullet->active = 0;
            EraseBullet(*bullet);
        }
    }
    
    pthread_exit(NULL);
}

void fire_bullet(int x, int y) 
{
    for(int i = 0; i < MAX_BULLETS; i++) 
    {
        if(!bullets[i].active) 
        {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].active = 1;
            pthread_t thread_id;
            bullets[i].thread_id = thread_id;
            pthread_create(&thread_id, NULL, bullet_thread, &bullets[i]);
            break;
        }
    }
}