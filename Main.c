#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include "SpaceShip.c"

void DrawWindowLimits()
{
    for (int i = 0; i < 80; i++)
    {
        mvaddch(2, i, ACS_HLINE);
        mvaddch(33, i, ACS_HLINE);
    }
    
    for (int i = 2; i < 34; i++)
    {
        mvaddch(i, 1, ACS_VLINE);
        mvaddch(i, 78, ACS_VLINE);
    } 
}

void Initialize(SpaceShip *spaceShip) 
{
    spaceShip->X = 38;
    spaceShip->Y = 31;
    spaceShip->Lifes = 3;
    
    DrawWindowLimits();
    DrawSpaceShip(*spaceShip); 
    DrawLifes(spaceShip);
}

int main() {    
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
    Initialize(&spaceShip);

    // Bucle para capturar la entrada del teclado
    while((ch = getch()) != 27) { // Salir con ESC
        
        MovSpaceShip(&spaceShip, ch);
        // DrawLifes(&spaceShip);
        
        // Refresca la pantalla para mostrar el cambio de posición 
        refresh();
    }

    // Finaliza el programa de ncurses
    endwin();

    return 0;
}
