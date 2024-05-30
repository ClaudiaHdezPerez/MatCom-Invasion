#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <locale.h>
#include "SpaceShip.c"

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
    spaceShip->X = x / 2;
    spaceShip->Y = y - 3;
    spaceShip->Lifes = 3;

    DrawWindowLimits(x, y);
    DrawSpaceShip(*spaceShip); 
    DrawLifes(spaceShip, x, y);
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

    int ch;
    SpaceShip spaceShip;

    // Obtener el tamaño de la ventana de la terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    Initialize(&spaceShip, max_x, max_y);

    // Bucle para capturar la entrada del teclado
    while((ch = getch()) != 27) { // Salir con ESC

        int new_x, new_y;
        getmaxyx(stdscr, new_y, new_x);

        if (new_y != max_y || new_x != max_x)
        {
            // DrawWindowLimits(max_x, max_y);
            // DrawLifes(&spaceShip, max_x, max_y);
            max_x = new_x;
            max_y = new_y;

            clear();
            Initialize(&spaceShip, new_x, new_y);
        }

        MovSpaceShip(&spaceShip, ch, max_x, max_y);
        
        // Refresca la pantalla para mostrar el cambio de posición 
        refresh();
    }

    // Finaliza el programa de ncurses
    endwin();

    return 0;
}
