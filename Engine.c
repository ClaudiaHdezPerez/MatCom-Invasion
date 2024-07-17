#include "Utils.h"

void divide_screen()
{
    srand(time(NULL)); 
    int max_x2 = getmaxx(stdscr);
    max_x2 -= 4;

    int width_page = 7;
    total_pages = max_x2 / width_page;
    
    int start_page = 2; 

    pages = (Page*)malloc(total_pages * sizeof(Page));
    for (int i = 0; i < total_pages; i++) 
    {
        pages[i].start = start_page;
        pages[i].width = width_page;
        pages[i].age = (rand() % 5) + 1;

        start_page += width_page;
    }
}

int max(int num1, int num2) 
{
    return (num1 > num2 ) ? num1 : num2;
}

Page* older_page(SpaceShip *spaceShip) // LRU
{
    Page *older = NULL;
 
    for (int i = 0; i < total_pages; i++)
    {      
        if (spaceShip->game_over)
        {
            older = NULL;
            break;
        }

        int older_age;  
        if (older == NULL)
            older = &pages[i];
        else
            older_age = max(pages[i].age, older->age);

        if (older_age == pages[i].age)
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

    big_enemy = 0;
    count = 0;

    while (!spaceShip->game_over)
    {
        usleep(1000000);

        if (!big_enemy)
        {
            for (int i = 0; i < MAX_ENEMIES - 1; i++)
            {
                if (count >= 20)
                {
                    big_enemy = 1;
                    count = 0;
                    break;
                }

                if (spaceShip->game_over)
                    break;

                if (big_enemy)
                    break;
                    
                if(!enemies[i].active) 
                {
                    Page *page = older_page(spaceShip);

                    if (page != NULL)
                    {
                        enemies[i].active = 1;
                        enemies[i].x = page->start + 1;
                        enemies[i].y = 3;

                        pthread_mutex_lock(&lock_lifes); 
                        enemies[i].lifes = enemies_lifes[i];
                        pthread_mutex_unlock(&lock_lifes); 

                        enemies[i].page = page;
                        enemies[i].number = i + 1;
                        enemies[i].killed = 0;

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
                        
                        count++;
                        break;
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
}

int all_inactive()
{
    for (int i = 0; i < MAX_ENEMIES - 1; i++)
    {
        if (enemies[i].active)
            return 0;
    }

    return 1;
}

void* rr_scheduling(void* arg) 
{
    SpaceShip *spaceShip = (SpaceShip*) arg; 
    lifes[0] = 1;
    lifes[1] = 3;
    lifes[2] = 5;
    lifes[3] = 15;

    enemies[MAX_ENEMIES - 1].active = 0;
    big_enemy = 0;
    int time_slice = (rand() % 5) + 8;
    int index_enemies_lifes = 0;

    while (!spaceShip->game_over) 
    {
        if (!big_enemy)
        {
            for (int i = 0; i < 3; i++)
            {
                int burst_time = (rand() % lifes[i]) + 1;
                if (burst_time <= time_slice) 
                {
                    if (spaceShip->game_over)
                        break;

                    time_slice -= burst_time;

                    pthread_mutex_lock(&lock_lifes); 
                    enemies_lifes[index_enemies_lifes] = lifes[i];
                    pthread_mutex_unlock(&lock_lifes); 

                    index_enemies_lifes++;
                    i--;

                    if (index_enemies_lifes == MAX_ENEMIES - 1)
                        index_enemies_lifes = 0;
                } 
                
                else 
                    time_slice = (rand() % 5) + 8;  
            } 
        }

        else 
        {
            if (!active && big_enemy && all_inactive())
            {
                enemies[MAX_ENEMIES - 1].x = max_x1 / 2 - 6;
                enemies[MAX_ENEMIES - 1].y = 3;
                enemies[MAX_ENEMIES - 1].width = 11;
                enemies[MAX_ENEMIES - 1].lifes = lifes[3];
                enemies[MAX_ENEMIES - 1].color = COLOR_GREEN;
                enemies[MAX_ENEMIES - 1].active = 1;
                enemies[MAX_ENEMIES - 1].number = 22;
                enemies[MAX_ENEMIES - 1].active = 1;
                enemies[MAX_ENEMIES - 1].killed = 0;

                enemies_thread big_t;
                big_t.enemy = &enemies[MAX_ENEMIES - 1];
                big_t.spaceShip = spaceShip;

                pthread_t thread_enemy;
                pthread_create(&thread_enemy, NULL, move_enemy, (void*)&big_t);
                active = 1;
            }  
        }
    }

    pthread_exit(NULL);
}