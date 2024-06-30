#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Engine.c"
#include "Score.c"

int record;

void draw_window_limits(int max_x, int max_y)
{
    // Dibujar el marco

    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar

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
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void load_score_and_record(int x, int y)
{
    pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
    mvprintw(1, 3, "%s", "SCORE: 0");
    mvprintw(1, 18, "%s", "RECORD:");

    record = get_record();
    mvprintw(1, 26, "%d", record);
    pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
}

void initialize(SpaceShip *spaceShip, int x, int y) 
{
    draw_window_limits(x, y);
    load_score_and_record(x, y);
    draw_lifes(spaceShip, x, y);

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
    spaceShip.lifes = 3;
    spaceShip.game_over = 0;

    pthread_t move;
    pthread_t enemies;
    pthread_t lifes;

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Inicializar SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error al inicializar SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }

    // Cargar el efecto de sonido
    Mix_Chunk* efecto = Mix_LoadWAV("planckx27s-constant-la-constante-de-planck-127554.wav");
    if (efecto == NULL) {
        printf("Error al cargar el efecto de sonido: %s\n", Mix_GetError());
        return 1;
    }

    // Reproducir el efecto de sonido
    Mix_PlayChannel(-1, efecto, 0);

    // Esperar para que el sonido tenga tiempo de reproducirse
    SDL_Delay(2000);

    // Obtener el tamaño de la ventana de la terminal
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    spaceShip.x = max_x / 2;
    spaceShip.y = max_y - 3;

    initialize(&spaceShip, max_x, max_y);
    draw_spaceShip(&spaceShip); 

    divide_screen();
    
    pthread_create(&enemies, NULL, &generate_enemies, (void*) &spaceShip);
    pthread_create(&move, NULL, &move_spaceShip, (void*) &spaceShip);
    pthread_create(&lifes, NULL, &rr_scheduling, (void*) &spaceShip);

    // Bucle para capturar la entrada del teclado
    while(!spaceShip.game_over) 
    { // Salir con ESC

        int new_x, new_y;
        getmaxyx(stdscr, new_y, new_x);
        
        pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
        mvprintw(1, 10, "%d", score);
        pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar

        if (score > record)
        {
            pthread_mutex_lock(&lock); // Desbloquea el mutex después de dibujar
            mvprintw(1, 26, "%d", score);
            pthread_mutex_unlock(&lock); // Desbloquea el mutex después de dibujar
        }

        if (new_y != max_y || new_x != max_x)
        {
            divide_screen();
            max_x = new_x;
            max_y = new_y;

            clear();
            initialize(&spaceShip, new_x, new_y);         
        }

        refresh();
        usleep(30000);
    }

    // Finaliza el programa de ncurses
    endwin();

    // Limpiar sonido
    Mix_FreeChunk(efecto);
    Mix_CloseAudio();
    SDL_Quit();

    set_record(score, record);
    
    free(pages);
    pthread_join(move, NULL);
    pthread_mutex_destroy(&lock); // Destruye el mutex al final del programa
    pthread_mutex_destroy(&lock_lifes); // Destruye el mutex al final del programa

    return 0;
}
