#include <stdlib.h>
#include <unistd.h>
#include "Enemy.c"

#define MAX_COLISION 20

pthread_mutex_t lock_lifes; // Declara el mutex

Page *pages;
int total_pages = 0;

int enemies_lifes[MAX_ENEMIES];

int lifes[3];

FreeList* CreateNode(int value) 
{
    FreeList* newNode = (FreeList*)malloc(sizeof(FreeList));
    newNode->space = value;
    newNode->next = NULL;
    return newNode;
}

void DivideScreen()
{
    int max_x = getmaxx(stdscr);
    max_x -= 4;

    int width_page = 5;
    total_pages = max_x / 5;
    
    int start_page = 2; 

    pages = (Page*)malloc(total_pages * sizeof(Page));
    for (int i = 0; i < total_pages; i++) 
    {
        pages[i].start = start_page;
        pages[i].width = 5;
        pages[i].age = 0;

        start_page += 5;
    }
}

int max(int num1, int num2) {
    return (num1 > num2 ) ? num1 : num2;
}

Page* OlderPage()
{
    Page *older = &pages[0];

    for (int i = 0; i < total_pages; i++)
    {        
        int older_ = max(pages[i].age, older->age);
        if (older_ == pages[i].age)
            older = &pages[i];

        pages[i].age++;    
    }

    older->age--;
    return older;
}

void* GenerateEnemies(void* arg)
{
    SpaceShip *spaceShip = (SpaceShip*) arg; 
    usleep(100000);

    while (!spaceShip->game_over)
    {
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (spaceShip->game_over)
                break;
                
            if(!enemies[i].active) 
            {
                Page *old = OlderPage();
                enemies[i].x = old->start + 1;
                enemies[i].y = 3;

                pthread_mutex_lock(&lock_lifes); // Bloquea el mutex antes de dibujar
                enemies[i].lifes = enemies_lifes[i];
                pthread_mutex_unlock(&lock_lifes); // Bloquea el mutex antes de dibujar

                enemies[i].active = 1;
                enemies[i].page = old;
                enemies[i].number = i + 1;

                switch (enemies[i].lifes)
                {
                    case 1:
                        enemies[i].color = COLOR_YELLOW;
                        break;

                    case 3:
                        enemies[i].color = COLOR_CYAN;
                        break;

                    case 5:
                        enemies[i].color = COLOR_RED;
                        break;

                    default:
                        enemies[i].color = COLOR_GREEN;
                        break;
                }

                pthread_t thread_enemy;
                enemies_thread struct_thread_enemy;
                struct_thread_enemy.enemy = &enemies[i];
                struct_thread_enemy.spaceShip = spaceShip;
                
                pthread_create(&thread_enemy, NULL, MoveEnemy, (void*)&struct_thread_enemy);
                break;
            }
        }

        usleep(200000);
    }

    pthread_exit(NULL);
}

void* rr_scheduling(void* arg) 
{
    SpaceShip *spaceShip = (SpaceShip*) arg; 
    lifes[0] = 1;
    lifes[1] = 3;
    lifes[2] = 5;

    int time_slice = 10;
    int time = time_slice;
    int index_enemies_lifes = 0;

    while (!spaceShip->game_over) {
        for (int i = 0; i < 3; i++) {
            if (lifes[i] <= time) 
            {
                time -= lifes[i];

                pthread_mutex_lock(&lock_lifes); // Bloquea el mutex antes de dibujar
                enemies_lifes[index_enemies_lifes] = lifes[i];
                pthread_mutex_unlock(&lock_lifes); // Bloquea el mutex antes de dibujar
                
                index_enemies_lifes++;
                i--;

                if (index_enemies_lifes == MAX_ENEMIES)
                    index_enemies_lifes = 0;
            } 

            else 
                time = time_slice;
        }
    }

    pthread_exit(NULL);
}