#include <ncurses.h>
#include <string.h>

int main() {
    // Inicializa la pantalla de ncurses
    initscr();
    // Permite la captura de teclas especiales como las flechas del teclado
    keypad(stdscr, TRUE);
    // No muestra los caracteres ingresados por el usuario
    noecho();
    // Oculta el cursor de la pantalla
    curs_set(0);

    int x = 10, y = 10;
    int ch;
    char *str1 = "  /\\";
    char *str2 = "_//\\\\_";
    char *space1 = "    ";
    char *space2 ="      ";
    // Muestra un "*" en la posición inicial
    mvprintw(y, x, "%s", str1);
    mvprintw(y + 1, x, "%s", str2);
    // Refresca la pantalla para mostrar el "*"
    refresh();

    // Bucle para capturar la entrada del teclado
    while((ch = getch()) != 27) { // Salir con ESC
        // Borra el anterior
        mvprintw(y, x, "%s", space1);
        mvprintw(y + 1, x, "%s", space2);
        switch(ch) {
            case KEY_UP:
                y = y > 0 ? y - 1 : y;
                break;
            case KEY_DOWN:
                y++;
                break;
            case KEY_LEFT:
                x = x > 0 ? x - 1 : x;
                break;
            case KEY_RIGHT:
                x++;
                break;
        }

        mvprintw(y, x, "%s", str1);
        mvprintw(y + 1, x, "%s", str2);
        // Refresca la pantalla para mostrar el cambio de posición 
        refresh();
    }

    // Finaliza el programa de ncurses
    endwin();

    return 0;
}
