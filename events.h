enum {
    LEFT=0, RIGHT=1, UP=2, DOWN=3,
    LEFTUP=4, RIGHTUP=5, LEFTDOWN=6, RIGHTDOWN=7,
    MIDDLE=8, CLICK=9, RELEASE=10,
    MOTION=11, x=12, y=13
}; // for keyboard, mouse and joystick handling

typedef struct
{
    char quit;
    char key[SDLK_LAST];
    char button[12];
    char axis[4];
    char hat[9];
    char mousedown[4];
    char mouseup[4];
    char mousemotion[14];
} Input; // Events sub-variables

void updateEvents(Input* in, SDL_Event *event);
