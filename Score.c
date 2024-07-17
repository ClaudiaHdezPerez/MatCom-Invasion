#include "Score.h"

FILE *file;

int get_record()
{
    // Abrir el archivo en modo de lectura (r)
    file = fopen("Data.txt", "r");

    // Leer del archivo
    char buffer[100];
    fscanf(file, "%s", buffer);

    // Cerrar el archivo después de la lectura
    fclose(file);

    int record = atoi(buffer);

    return record;
}

void set_record(int score, int record)
{
    if (score > record)
    {
        // Abrir el archivo en modo de escritura (w)
        file = fopen("Data.txt", "w");

        // Escribir en el archivo
        fprintf(file, "%d\n", score);
        
        // Cerrar el archivo después de la escritura
        fclose(file);
    }
}
