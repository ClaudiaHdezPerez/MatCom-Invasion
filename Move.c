#include "Bullet.c"

#define MAX_BULLETS 20

Bullet bullets[MAX_BULLETS];

void colision_spaceShip_enemy(SpaceShip *spaceShip, Enemy *enemy)
{
    if (!spaceShip->game_over && enemy->active && 
    (enemy->x <= spaceShip->x + 7 && enemy->x >= spaceShip->x
    && enemy->y >= spaceShip->y - 2  ||
    enemy->x + 1 <= spaceShip->x + 7 && enemy->x + 1 >= spaceShip->x
    && enemy->y >= spaceShip->y - 2  ||
    enemy->x + 2 <= spaceShip->x + 7 && enemy->x + 2 >= spaceShip->x
    && enemy->y >= spaceShip->y - 2  ||
    enemy->x + 3 <= spaceShip->x + 7 && enemy->x + 3 >= spaceShip->x
    && enemy->y >= spaceShip->y - 2))
    {
        enemy->active = 0;
        erase_enemy(*enemy);
        die(spaceShip, max_x1, max_y1);
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

            if(enemy->y > max_y1 - 4 || !enemy->active) {
                enemy->active = 0;
                enemy->page->age++;
                enemy->page->capacity += enemy->lifes;
                erase_enemy(*enemy);
                use_enemy = 0;
                break;
            }
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

            spaceShip->x = max_x / 2;
            spaceShip->y = max_y - 3;  

            draw_spaceShip(spaceShip);    
        }

        int y = spaceShip->y;
        int x = spaceShip->x;

        int ch = getch();
        if (ch != ERR) // Solo procesa la entrada si se presionó una tecla
        {  
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
                    fire_bullet(spaceShip->x + 2, spaceShip->y - 1);
                    break;
            }

            erase_spaceShip(spaceShip);

            spaceShip->y = y;
            spaceShip->x = x;

            draw_spaceShip(spaceShip);
        }     
    }
    
    sleep(2);
    pthread_exit(NULL);
}