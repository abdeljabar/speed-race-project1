#define WIDTH           1000
#define HEIGHT          700

#define TOTAL_ROADS     50
#define DISTANCE        TOTAL_ROADS * 1000
#define LAST_ROAD       TOTAL_ROADS - 1

#define TOTAL_CARS      5 // enemies
#define CAR_WIDTH       43
#define CAR_HEIGHT      96
#define HIGH_SPEED      120
#define LOW_SPEED       80

#define GAME_TIME       30

#define ENTRER_LARGEUR  250
#define ENTRER_HAUTEUR  161

enum {
  DIR_LEFT=0,
  DIR_RIGHT=1
}; // for handling car directions

typedef struct
{
    SDL_Surface *source;
    int x;
    int y;
    int speed;
} Sprite; // Sprites/objects sub-variables

typedef struct
{
    int left;
    int right;
    int up;
    int down;
} Go;
