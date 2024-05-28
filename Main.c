#include <ncurses.h>
#include <unistd.h>
// #include <locale.h>
#include "SpaceShip.c"

void DrawWindowLimits()
{
    for (int i = 2; i < 78; i++)
    {
        mvprintw(2, i, "%s", "_");
        mvprintw(33, i, "%s", "_");
    }
    
    for (int i = 4; i < 33; i++)
    {
        mvprintw(i, 2, "%s", "|");
        mvprintw(i, 78, "%s", "|");
    }
    
}

int main() {
    // setlocale(LC_ALL, "");
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
    spaceShip.X = 38;
    spaceShip.Y = 31;
    
    DrawWindowLimits();
    DrawSpaceShip(spaceShip);

    // Bucle para capturar la entrada del teclado
    while((ch = getch()) != 27) { // Salir con ESC
        
        MovSpaceShip(&spaceShip, ch);
        
        // Refresca la pantalla para mostrar el cambio de posición 
        refresh();
    }

    // Finaliza el programa de ncurses
    endwin();

    return 0;
}
