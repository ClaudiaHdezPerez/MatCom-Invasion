#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern FILE *file;

int get_record();
void set_record(int score, int record);