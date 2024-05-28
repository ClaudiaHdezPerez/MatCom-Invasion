#include <stdio.h>
#include <ncurses.h>

typedef struct SpaceShip
{
    int X;
    int Y;
} SpaceShip;

void DrawSpaceShip(SpaceShip spaceShip)
{
    char *str1 = "  /\\";
    char *str2 = "_//\\\\_";

    // Muestra un "*" en la posición inicial
    mvprintw(spaceShip.Y, spaceShip.X, "%s", str1);
    mvprintw(spaceShip.Y + 1, spaceShip.X, "%s", str2);
}

void EraseSpaceShip(SpaceShip spaceShip)
{
    char *space1 = "    ";
    char *space2 ="      ";

    // Muestra un "*" en la posición inicial
    mvprintw(spaceShip.Y, spaceShip.X, "%s", space1);
    mvprintw(spaceShip.Y + 1, spaceShip.X, "%s", space2);
}

void MovSpaceShip(SpaceShip *spaceShip, int ch)
{
    EraseSpaceShip(*spaceShip);

    int y = spaceShip->Y;
    int x = spaceShip->X;

    switch(ch) {
        case KEY_UP:
            y = y > 20 ? y - 1 : y;
            break;
        case KEY_DOWN:
            y = y < 31 ? y + 1 : y;
            break;
        case KEY_LEFT:
            x = x > 3 ? x - 1 : x;
            break;
        case KEY_RIGHT:
            x = x < 72 ? x + 1 : x;
            break;
    }

    spaceShip->Y = y;
    spaceShip->X = x;

    DrawSpaceShip(*spaceShip);
}
