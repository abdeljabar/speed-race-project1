#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "constants.h"
#include "functions.h"
#include "events.h"
#include "play.h"

void updateEvents(Input* in, SDL_Event *event)
{

    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            SDL_QUIT:
                in->quit = 1;
                break;

            // Dealing with the keyboard
            case SDL_KEYDOWN:
                in->key[event->key.keysym.sym]=1;
                break;

            case SDL_KEYUP:
                in->key[event->key.keysym.sym]=0;
                break;

            // Dealing with joystick:buttons
            case SDL_JOYBUTTONDOWN:
                in->button[event->jbutton.button]=1;
                break;

            case SDL_JOYBUTTONUP:
                in->button[event->jbutton.button]=0;
                break;

            // Dealing with joystick:axis
            case SDL_JOYAXISMOTION:
                if ( event->jaxis.axis == 0 && event->jaxis.value < -2000 ) // left
                    in->axis[LEFT] = 1;
                else if (event->jaxis.axis == 0 && event->jaxis.value > 2000) // right
                    in->axis[RIGHT] = 1;
                else if ( event->jaxis.axis == 1 && event->jaxis.value < -2000 ) // up
                    in->axis[UP] = 1;
                else if ( event->jaxis.axis == 1 && event->jaxis.value > 2000 ) // down
                    in->axis[DOWN] = 1;
                else
                {
                    in->axis[UP] = 0;
                    in->axis[RIGHT] = 0;
                    in->axis[DOWN] = 0;
                    in->axis[LEFT] = 0;
                }

			// Dealing with joystick:hatch
            case SDL_JOYHATMOTION:
                switch(event->jhat.value)
                {
                    case SDL_HAT_LEFT:
                        in->hat[LEFT] = 1;
                        break;

                    case SDL_HAT_LEFTUP:
                        in->hat[LEFTUP] = 1;
                        break;

                    case SDL_HAT_LEFTDOWN:
                        in->hat[LEFTDOWN] = 1;
                        break;

                    case SDL_HAT_RIGHT:
                        in->hat[RIGHT] = 1;
                        break;

                    case SDL_HAT_RIGHTUP:
                        in->hat[RIGHTUP] = 1;
                        break;

                    case SDL_HAT_RIGHTDOWN:
                        in->hat[RIGHTDOWN] = 1;
                        break;

                    case SDL_HAT_UP:
                        in->hat[UP] = 1;
                        break;

                    case SDL_HAT_DOWN:
                        in->hat[DOWN] = 1;
                        break;

                    default:
                        in->hat[UP] = 0;
                        in->hat[RIGHT] = 0;
                        in->hat[DOWN] = 0;
                        in->hat[LEFT] = 0;
                        in->hat[LEFTUP] = 0;
                        in->hat[LEFTDOWN] = 0;
                        in->hat[RIGHTUP] = 0;
                        in->hat[RIGHTDOWN] = 0;
                        break;
                }
                break;

			// Dealing with mouse onclick
            case SDL_MOUSEBUTTONDOWN:

                in->mousedown[CLICK] = 1;
                in->mousedown[x] = event->button.x;
                in->mousedown[y] = event->button.y;

                switch(event->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        in->mousedown[LEFT] = 1;
                        break;

                    case SDL_BUTTON_RIGHT:
                        in->mousedown[RIGHT] = 1;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        in->mousedown[MIDDLE] = 1;
                        break;

                    default:
                        in->mousedown[LEFT] = 1;
                        in->mousedown[MIDDLE] = 1;
                        in->mousedown[RIGHT] = 1;
                        break;
                }
                break;

			// Dealing with mouse onrelease
            case SDL_MOUSEBUTTONUP:

                in->mouseup[RELEASE] = 1;
                in->mouseup[x] = event->button.x;
                in->mouseup[y] = event->button.y;

                switch(event->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        in->mouseup[LEFT] = 1;
                        break;

                    case SDL_BUTTON_RIGHT:
                        in->mouseup[RIGHT] = 1;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        in->mouseup[MIDDLE] = 1;
                        break;

                    default:
                        in->mouseup[LEFT] = 1;
                        in->mouseup[MIDDLE] = 1;
                        in->mouseup[RIGHT] = 1;
                        break;
                }
                break;

			// Dealing with mouse motion
            case SDL_MOUSEMOTION:
                in->mousemotion[MOTION] = 1;
                in->mousemotion[x] = event->button.x;
                in->mousemotion[y] = event->button.y;

            default:
                break;
        }
    }
}
