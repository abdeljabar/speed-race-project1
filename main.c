#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "fmod.h"

#include "constants.h"
#include "functions.h"
#include "events.h"
#include "play.h"

int main ( int argc, char *argv[] )
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0)
        return EXIT_FAILURE;

    TTF_Init();
    FSOUND_Init(44100, 32, 0);

    SDL_Joystick *joystick = NULL;
    SDL_Event event;
    SDL_Surface *screen = NULL, *entree = NULL, *commencer = NULL;

    SDL_Rect position_commencer, position_entree;
    TTF_Font *badabb = NULL;

    Input in;
    int run = 1, i = 0;

    //*
    SDL_WM_SetCaption("Speed Race", NULL);
    SDL_WM_SetIcon(IMG_Load("images/icon.png"), NULL);

    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    joystick = SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);

    entree = IMG_Load("images/speed_entree.jpg");
    position_entree.x = 0;
    position_entree.y = 0;

    commencer = IMG_Load("images/commencer.png");
    position_commencer.x = 720;
    position_commencer.y = 515;

    memset( &in, 0, sizeof(in) );

    while (run)
    {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                run = 0;
                break;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        run = 0;
                        break;

                    default:
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.x > 720
                   && event.button.x <= 720+ENTRER_LARGEUR
                   && event.button.y > 515
                   && event.button.y <= 515+ENTRER_HAUTEUR)
                   {
                       play(screen);
                   }

                   break;

            case SDL_JOYBUTTONDOWN:
            //case SDL_JOYAXISMOTION:

                if(event.jbutton.button == 2)
                {
                    play(screen);
                }
                break;

            default:
                break;
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,255,255));
        SDL_BlitSurface(entree, NULL, screen, &position_entree);
        SDL_BlitSurface(commencer, NULL, screen, &position_commencer);
        SDL_Flip(screen);

    }
    //*/
    SDL_JoystickClose(joystick);
    SDL_Quit();
    return EXIT_SUCCESS;
}
