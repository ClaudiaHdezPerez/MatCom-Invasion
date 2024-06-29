#include "Move.c"
#include <time.h>
#include <stdlib.h>

pthread_mutex_t lock_lifes; // Declara el mutex

Page *pages;
int total_pages = 0;

int enemies_lifes[MAX_ENEMIES];

int lifes[4];

void divide_screen()
{
    srand(time(NULL)); 
    int max_x = getmaxx(stdscr);
    max_x -= 4;

    int width_page = 7;
    total_pages = max_x / width_page;
    
    int start_page = 2; 

    pages = (Page*)malloc(total_pages * sizeof(Page));
    for (int i = 0; i < total_pages; i++) 
    {
        pages[i].start = start_page;
        pages[i].width = width_page;
        pages[i].age = rand() % 2;
        pages[i].age = (rand() % 5) + 1;

        start_page += width_page;
    }
}

int max(int num1, int num2) 
{
    return (num1 > num2 ) ? num1 : num2;
}

Page* older_page(Enemy *enemy) // LRU
{
    Page *older = NULL;
 
    for (int i = 0; i < total_pages; i++)
    {      
        int older_;  
        if (older != NULL)
            older_ = max(pages[i].age, older->age);

        if (older == NULL && pages[i].capacity <= enemy->lifes)
            older = &pages[i];

        if (older_ == pages[i].age && pages[i].capacity <= enemy->lifes)
            older = &pages[i];

        pages[i].age++;   
    }

    if (older != NULL)
        older->age -= 2;
    
    return older;
}

void* generate_enemies(void* arg)
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
                Page *old = older_page(&enemies[i]);

                if (old != NULL)
                {
                    enemies[i].active = 1;
                    enemies[i].x = old->start + 1;
                    enemies[i].y = 3;

                    pthread_mutex_lock(&lock_lifes); 
                    enemies[i].lifes = enemies_lifes[i];
                    pthread_mutex_unlock(&lock_lifes); 

                    enemies[i].page = old;
                    enemies[i].page->capacity -= enemies[i].lifes;
                    enemies[i].number = i + 1;
                    enemies[i].block = 0;

                    switch (enemies[i].lifes)
                    {
                        case 1:
                            enemies[i].color = COLOR_YELLOW;
                            enemies[i].width = 4;
                            break;

                        case 3:
                            enemies[i].color = COLOR_CYAN;
                            enemies[i].width = 4;
                            break;

                        case 5:
                            enemies[i].color = COLOR_RED;
                            enemies[i].width = 4;
                            break;

                        default:
                            enemies[i].color = COLOR_GREEN;
                            enemies[i].width = 10;
                            break;
                    }

                    pthread_t thread_enemy;
                    enemies_thread struct_thread_enemy;
                    struct_thread_enemy.enemy = &enemies[i];
                    struct_thread_enemy.spaceShip = spaceShip;
                    
                    pthread_create(&thread_enemy, NULL, move_enemy, (void*)&struct_thread_enemy);
                    break;
                }
            }
        }

        usleep(1000000);
    }

    pthread_exit(NULL);
}

void* rr_scheduling(void* arg) 
{
    SpaceShip *spaceShip = (SpaceShip*) arg; 
    lifes[0] = 1;
    lifes[1] = 3;
    lifes[2] = 5;
    lifes[3] = 15;

    int time_slice = 10;
    int time = time_slice;
    int index_enemies_lifes = 0;

    while (!spaceShip->game_over) {
        for (int i = 0; i < 3; i++) {
            if (lifes[i] <= time) 
            {
                if (i != 0)
                    time -= lifes[i];

                else
                    time -= 4;

                pthread_mutex_lock(&lock_lifes); 
                enemies_lifes[index_enemies_lifes] = lifes[i];
                pthread_mutex_unlock(&lock_lifes); 

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