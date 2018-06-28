#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "fmod.h"

#include "constants.h"
#include "functions.h"
#include "events.h"
#include "play.h"

void Blit(SDL_Surface* source,SDL_Surface* dest,int x,int y)
{
    SDL_Rect R;
    R.x = x;
    R.y = y;
    SDL_BlitSurface(source,NULL,dest,&R);
}

void move(Sprite *car, Sprite *enemies, int direction )
{
    int i=0, right=0, left=0;

    if ( direction == 1 )
    {
        // Let's check if there is any car nearby on the RIGHT
        for (i=0; i<TOTAL_CARS; i++ )
        {
            if (  (enemies[i].x - car->x <= CAR_WIDTH && enemies[i].x - car->x > 0)
                && ( ( enemies[i].y - car->y <= CAR_HEIGHT && enemies[i].y - car->y > 0 )
                || ( car->y - enemies[i].y <= CAR_HEIGHT && car->y - enemies[i].y > 0) )
                || car->x >= 586 - CAR_WIDTH)
            {
                right++;
            }
        }

        if ( right == 0 )
            car->x+=3;
    }
    else
    {
        // Let's check if there is any car nearby on the LEFT
        for ( i=0; i<TOTAL_CARS; i++ )
        {
            if ( (car->x - enemies[i].x <= CAR_WIDTH && car->x - enemies[i].x > 0)
                && ( ( enemies[i].y - car->y <= CAR_HEIGHT && enemies[i].y - car->y > 0 )
                || ( car->y - enemies[i].y <= CAR_HEIGHT && car->y - enemies[i].y > 0) )
                || car->x <= 136 )
            {
                left++;
            }
        }

        if ( left == 0 )
            car->x -= 3;
    }

}

void forward( Sprite *car,  Sprite *enemies, Sprite *roads, Sprite *finish, int *carState, int *direction, int *win)
//void forward(int *direction)
{

    //*
    int i=0, STOP=0;

    if ( roads[LAST_ROAD].y <= (- car->speed ) )
    {

        STOP = check_collision(enemies, car, direction);

        if ( STOP )
        {
            *carState = 0;
        }
        else
        {
            for (i=0; i<TOTAL_ROADS; i++)
            {
                roads[i].y += car->speed;
            }

            finish->y += car->speed;
            *carState = 1;
        }

    }
    else
    {
        // YOU WIN
        *win = 1;
    }
    //*/
}

void surpass(Sprite *enemies, Sprite *car, int state)
{

    int i=0;

    if ( state )
    {
        for (i=0; i<TOTAL_CARS; i++)
        {
            enemies[i].y += ( car->speed - enemies[i].speed );
        }
    }

}

void release_cars(Sprite *enemies)
{
    int i=0;

    for (i=0; i<TOTAL_CARS; i++)
    {
        enemies[i].y -= enemies[i].speed;
    }
}

int check_collision(Sprite *enemies, Sprite *car, int *direction)
{
    int i=0, exists = 0;

    for ( i=0; i<TOTAL_CARS; i++)
    {
        if ( car->y - enemies[i].y <= CAR_HEIGHT
            && car->y - enemies[i].y > 0 )
        {
            if (car->x - enemies[i].x < CAR_WIDTH
                && car->x - enemies[i].x >= 0)// right
            {
                exists++;
                *direction = RIGHT;
            }
            else if (enemies[i].x - car->x < CAR_WIDTH
                     && enemies[i].x - car->x >= 0) // left
            {
                exists++;
                *direction = LEFT;
            }
        }
    }

    if ( exists > 0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

int position(Sprite *car, Sprite *enemies )
{
    int position = 1, i = 0;
    // Let's check if there is any car nearby on the RIGHT

    for (i=0; i<TOTAL_CARS; i++ )
    {
        if (enemies[i].y < car->y)
        {
            position++;
        }
    }

    return position;
}

void settimes(int *time, int total)
{
    int i=0;

    for (i=0; i<total; i++)
    {
        time[i] = SDL_GetTicks();
    }
}

void unsettimes(int *time, int total)
{
    int i=0;

    for (i=0; i<total; i++)
    {
        time[i] = 0;
    }
}
