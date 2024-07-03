# Definir el compilador
CC = gcc

# Definir las banderas del compilador
CFLAGS = -Wall -Wextra -pedantic

# Definir las bibliotecas a enlazar
LIBS = -lncursesw -lpthread -lSDL2 -lSDL2_mixer

# Definir los archivos objeto
OBJS = Main.c SpaceShip.c Score.c Engine.c Enemy.c Colision.c Bullet.c

# Regla por defecto
all: MatcomInvasion

# Regla para el programa
MatcomInvasion: $(OBJS)
	$(CC) $(CFLAGS) -o MatcomInvasion $(OBJS) $(LIBS)

# Regla para limpiar los archivos objeto y el ejecutable
clean:
	rm -f $(OBJS) MatcomInvasion

