#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include "Engine.c"

void DrawWindowLimits(int max_x, int max_y)
{
    // Dibujar el marco

    // Lados
    for (int i = 2; i < (max_x - 1); i++) {
        mvaddch(2, i, ACS_HLINE); // Superior
        mvaddch(max_y - 1, i, ACS_HLINE); // Inferior
    }

    for (int i = 2; i < (max_y - 1); i++) {
        mvaddch(i, 2, ACS_VLINE); // Izquierdo
        mvaddch(i, max_x - 1, ACS_VLINE); // Derecho
    }

    // Esquinas
    mvaddch(2, 2, ACS_ULCORNER);
    mvaddch(2, max_x - 1, ACS_URCORNER);
    mvaddch(max_y - 1, 2, ACS_LLCORNER);
    mvaddch(max_y - 1, max_x - 1, ACS_LRCORNER);

    //Presentacion
    mvprintw(1, max_x / 2 - 5, "%s", "MATCOM INVASION");
}

void Initialize(SpaceShip *spaceShip, int x, int y) 
{
    DrawWindowLimits(x, y);
    DrawLifes(spaceShip, x, y);

    refresh();
}

int main() {  
    setlocale(LC_ALL, "");  
    // Inicializa la pantalla de ncurses
    initscr();
    // Permite la captura de teclas especiales como las flechas del teclado
    keypad(stdscr, TRUE);
    // No muestra los caracteres ingresados por el usuario
    noecho();
    // Oculta el cursor de la pantalla
    curs_set(0);

    pthread_mutex_init(&lock, NULL); // Inicializa el mutex

    SpaceShip spaceShip;
    spaceShip.Lifes = 3;
    spaceShip.game_over = 0;

    pthread_t move;
    pthread_t enemies;
    pthread_t lifes;

    // Obtener el tamaño de la ventana de la terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    spaceShip.X = max_x / 2;
    spaceShip.Y = max_y - 3;

    Initialize(&spaceShip, max_x, max_y);
    DrawSpaceShip(&spaceShip); 

    DivideScreen();
    // GenerateEnemies();

    pthread_create(&enemies, NULL, &GenerateEnemies, (void*) &spaceShip);
    pthread_create(&move, NULL, &MovSpaceShip, (void*) &spaceShip);
    pthread_create(&lifes, NULL, &rr_scheduling, (void*) &spaceShip);

    // Bucle para capturar la entrada del teclado
    while(!spaceShip.game_over) { // Salir con ESC

        int new_x, new_y;
        getmaxyx(stdscr, new_y, new_x);

        if (new_y != max_y || new_x != max_x)
        {
            DivideScreen();
            max_x = new_x;
            max_y = new_y;

            clear();
            Initialize(&spaceShip, new_x, new_y);         
        }

        refresh();
        usleep(30000);
    }

    // Finaliza el programa de ncurses
    endwin();
    
    free(pages);
    pthread_join(move, NULL);
    pthread_mutex_destroy(&lock); // Destruye el mutex al final del programa
    pthread_mutex_destroy(&lock_lifes); // Destruye el mutex al final del programa

    return 0;
}
