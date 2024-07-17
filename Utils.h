#include <pthread.h>
#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#pragma region SpaceShip
extern pthread_mutex_t lock_spaceShip;

typedef struct SpaceShip
{
    int x;
    int y;
    int lifes;
    int game_over;
} SpaceShip;

extern pthread_mutex_t lock;

void draw_spaceShip(SpaceShip *spaceShip);
void draw_spaceShip(SpaceShip *spaceShip);
void erase_spaceShip(SpaceShip *spaceShip);
void draw_lifes(SpaceShip *spaceShip, int max_x);
void die(SpaceShip *spaceShip, int max_x, int max_y);
void* move_spaceShip(void* arg);
#pragma endregion

#pragma region Aliens

#define MAX_ENEMIES 21
#define ENEMY_BULLETS 5
extern int max_x1, max_y1;

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
    int killed;
    Page *page;
} Enemy;

typedef struct Bullet_Enemy {
    int x;
    int y;
    int active;
} Bullet_Enemy;


extern Bullet_Enemy enemy_bullets[ENEMY_BULLETS];

typedef struct bullet_spaceShip {
    Bullet_Enemy *bullet;
    SpaceShip *spaceShip;
} bullet_spaceShip;

typedef struct enemies_thread {
    Enemy *enemy;
    SpaceShip *spaceShip;
} enemies_thread;

void draw_enemy(Enemy enemy);
void erase_enemy(Enemy enemy);
void* move_enemy(void* arg);
void draw_bullet_enemy(Bullet_Enemy bullet);
void erase_bullet_enemy(Bullet_Enemy bullet);
void* fire_bullet_big_enemy(void* arg);
void bullet_big_enemy(Enemy *enemy, SpaceShip *spaceShip);
#pragma endregion

#pragma region Triggers and Colisions
#define MAX_BULLETS 20

extern int score;
extern int big_enemy;
extern int active;

typedef struct Bullet {
    int x;
    int y;
    int active;
} Bullet;

extern Enemy enemies[MAX_ENEMIES];
extern Bullet bullets[MAX_BULLETS];

void draw_bullet(Bullet bullet);
void increrase_score(Enemy enemy);
void erase_bullet(Bullet bullet);
void* bullet_thread(void* arg);
void fire_bullet(SpaceShip *spaceShip, int x, int y);
void colision_bullet_enemy(Bullet *bullet);
void colision_spaceShip_enemy(SpaceShip *spaceShip, Enemy *enemy);
void colision_bullet_spaceShip(Bullet_Enemy *bullet, SpaceShip *spaceShip);
#pragma endregion

#pragma region Engine
extern pthread_mutex_t lock_lifes; // Declara el mutex

extern Page *pages;
extern int total_pages;
extern int enemies_lifes[MAX_ENEMIES];
extern int count;
extern int lifes[4];

void divide_screen();
int max(int num1, int num2);
Page* older_page(SpaceShip *spaceShip); // LRU
void* generate_enemies(void* arg);
int all_inactive();
void* rr_scheduling(void* arg);
#pragma endregion