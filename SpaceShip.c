#include <ncursesw/ncurses.h>

typedef struct SpaceShip
{
    int X;
    int Y;
    int Lifes;
} SpaceShip;

void DrawSpaceShip(SpaceShip spaceShip)
{
    char *str1 = "  /\\";
    char *str2 = "_//\\\\_";
    // char *str1 = " /\\ ";
    // char *str2 = "|==|";
    // char *str3 = "/__\\";

    mvprintw(spaceShip.Y, spaceShip.X, "%s", str1);
    mvprintw(spaceShip.Y + 1, spaceShip.X, "%s", str2);
    // mvprintw(spaceShip.Y + 2, spaceShip.X, "%s", str3);
}

void EraseSpaceShip(SpaceShip spaceShip)
{
    char *space1 = "    ";
    char *space2 = "      ";
    // char *space3 = "    ";

    mvprintw(spaceShip.Y, spaceShip.X, "%s", space1);
    mvprintw(spaceShip.Y + 1, spaceShip.X, "%s", space2);
    // mvprintw(spaceShip.Y + 2, spaceShip.X, "%s", space3);
}

void MovSpaceShip(SpaceShip *spaceShip, int ch, int max_x, int max_y)
{
    EraseSpaceShip(*spaceShip);

    int y = spaceShip->Y;
    int x = spaceShip->X;

    switch(ch) {
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
    }

    spaceShip->Y = y;
    spaceShip->X = x;

    DrawSpaceShip(*spaceShip);
}

void DrawLifes(SpaceShip *spaceShip, int max_x, int max_y)
{
    mvprintw(1, max_x - 17, "%s", "LIFES");
    mvprintw(1, max_x - 10, "%s", "      ");

    for (int i = 0; i < 2 * spaceShip->Lifes; i+=2)
    {    
        // mvprintw(1, 70 + i, "%s", "0");
        // waddch(ACS_HEART);
        mvprintw(1, max_x - 10 + i, "%s", "♥");
    }

}