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

void play(SDL_Surface *screen)
{
    Input in;
    Go go;
    Sprite road,  menu, finish,
    car, car_crushes[9], enemies[TOTAL_CARS], roads[400],
    fuel, timer, game_over, game_win, car_position, logo, ike; // All Sprites
    int start=0, game_on=1,  win=0, lose=0, i=0, j=0, time=GAME_TIME,
    last_road=0, car_state=1, direction=0, current[10], previous[10],
    Advance=0, Explode=0, position_number=0,
    low_speed = LOW_SPEED, initial_speed = 0, first_car = 0, current_car = 0, countdown = 3;
    char src[100], fuel_time[3], delay_time[3], position_text[5], game_win_text[60], game_over_text[60];
    SDL_Event event;
    /*POINTERS*/
    //int *carstate = &car_state, *dir = &direction, *WIN = &win;

    SDL_Color black = {0,0,0}, white = {255,255,255}, red = {187,11,11};
    TTF_Font *electronic = NULL, *electronic200 = NULL, *badabb = NULL, *arial = NULL;
    FSOUND_SAMPLE *start_beep=NULL;
    FSOUND_STREAM *car_sound=NULL;

    settimes(current, 10);
    settimes(previous, 10);


    /* let's load the fonts */
    electronic = TTF_OpenFont("fonts/electronic_highway.ttf", 40);
    electronic200 = TTF_OpenFont("fonts/electronic_highway.ttf", 200);
    badabb = TTF_OpenFont("fonts/badabb.ttf", 60);
    arial = TTF_OpenFont("fonts/arial.ttf", 30);

    sprintf(src, "%d", GAME_TIME);
    timer.source = TTF_RenderText_Blended(electronic, src, red);
    timer.x = 840;
    timer.y = 72;

    car_position.source = TTF_RenderText_Blended(electronic, "6/6", red);
    car_position.x = 840;
    car_position.y = 201;

    fuel.source = TTF_RenderText_Blended(electronic, "0", red);
    fuel.x = 840;
    fuel.y = 329;

    game_over.x = 165;
    game_over.y = 200;

    game_win.x = 165;
    game_win.y = 200;

    // We load background
    road.source        = IMG_Load("images/road/road.jpg");
     // Position the road
    for (i=0; i<TOTAL_ROADS; i++)
    {
        roads[i].y = i*(-1000);
    }

    menu.source        = IMG_Load("images/stats.jpg");
    // Position the menu
    menu.x = 750;
    menu.y = 0;

    finish.source      = IMG_Load("images/finish_line.jpg");
    finish.x = 136;
    finish.y = 50 - (DISTANCE - 1738);

    // Main car
    car.source         = IMG_Load("images/cars/hero.png");
    // position the main car
    car.x = 170;
    car.y = 450;
    car.speed = 0;

    logo.source         = IMG_Load("images/logo.png");
    logo.x = 775;
    logo.y = 450;

    // Enemey cars
    current_car = 170;

    for (i=0; i<TOTAL_CARS; i++)
    {
        sprintf(src, "images/cars/enemy_%d.png", i);
        enemies[i].source = IMG_Load(src);

        // position the enemy cars
        current_car += 70;

        enemies[i].y = 450;
        enemies[i].x = current_car;

        enemies[i].speed = low_speed;
        low_speed += 5;
    }

    car_sound = FSOUND_Stream_Open("sound/drive_ferrari.wav", 0, 0, 0);
    start_beep = FSOUND_Sample_Load(FSOUND_FREE, "sound/start_beep.wav", 0, 0, 0);

    sprintf(src,"%d",4);
    ike.source = TTF_RenderText_Blended(electronic200, src, black);

    memset( &in, 0, sizeof(in) );


    while ( game_on ) {

        updateEvents(&in, &event);
        settimes(current, 10);

        current[7] = SDL_GetTicks();
        if ( !start )
        {
            if ( current[7] - previous[7] >= 1200 )
            {
                //
                if ( countdown <= 0 )
                {
                    start = 1;
                    countdown=0;
                }

                if ( countdown == 0 )
                {
                    sprintf(src,"GO");
                }
                else
                {
                    sprintf(src,"%d",countdown);
                    FSOUND_PlaySound(FSOUND_FREE, start_beep);
                }

                ike.source = TTF_RenderText_Blended(electronic200, src, black);
                countdown--;
                previous[7] = current[7];

            }
        }

        if( (in.key[SDLK_SPACE] || in.button[0]) && time > 0 && start)
        {
            go.up = 1;
            initial_speed = 1;

            if ( car.speed <= HIGH_SPEED )
            {
                if ( current[2] - previous[2] >= 20 )
                {
                    sprintf(fuel_time, "%d", car.speed);

                    fuel.source = TTF_RenderText_Blended(electronic, fuel_time, red);
                    car.speed++;

                    previous[2] = current[2];
                }
                else
                {
                    SDL_Delay(20 - (current[2] - previous[2]));
                }
            }
        }
        else
        {
            go.up = 0;

            if ( car.speed >= 0 )
            {
                if ( current[4] - previous[4] >= 10 )
                {
                    sprintf(fuel_time, "%d", car.speed);
                    fuel.source = TTF_RenderText_Blended(electronic, fuel_time, red);
                    car.speed -= 5;
                    if ( car.speed < 0)
                        car.speed = 0;
                    previous[4] = current[4];
                }
                else
                {
                    SDL_Delay(30 - (current[4] - previous[4]));
                }
            }
        }

        if (in.key[SDLK_LEFT] || in.axis[LEFT] || in.hat[LEFT])
        {
            go.left = 1;
        }
        else
        {
            go.left = 0;
        }

        if (in.key[SDLK_RIGHT] || in.axis[RIGHT] || in.hat[RIGHT])
        {
            go.right = 1;
        }
        else
        {
            go.right = 0;
        }

        if (in.key[SDLK_ESCAPE])
        {
            game_on = 0;
            break;
        }

        if (car.speed > 0) // GO UP
        {
//*
            forward(&car, enemies, roads, &finish, &car_state, &direction, &win);
            if (initial_speed == 0)
            {
                car.speed = 0;
            }
/*
            current[9] = SDL_GetTicks();
            if ( current[9] - previous[9] >= 6500 )
            {
                //FSOUND_Stream_Play(FSOUND_FREE, car_sound);
                previous[9] = current[9];
            }
*/

/*
            if ( car.speed <= HIGH_SPEED )
            {
                if ( current[2] - previous[2] >= 20 )
                {
                    sprintf(fuel_time, "%d", car.speed);

                    fuel.source = TTF_RenderText_Blended(electronic, fuel_time, red);
                    car.speed++;

                    previous[2] = current[2];
                }
                else
                {
                    SDL_Delay(20 - (current[2] - previous[2]));
                }
            }
*/
        }

        if (go.left)
        {
            move(&car, enemies, DIR_LEFT);
        }
        else if (go.right)
        {
            move(&car, enemies, DIR_RIGHT);
        }

        // Let's move the enemy cars automatically
        if ( start )
        {
            if ( current[0] - previous[0] >= 30 )
            {

                if ( car.speed > 0 )
                {

                    if ( car_state == 1 ) {
                        surpass(enemies, &car, 1);
                    } else {

                        // We must explode the car
                        Explode = 1;
                        release_cars(enemies);
                    }

                } else {
    //*
                    release_cars(enemies);
                    go.up = 0;
                }

                if ( !win ) position_number = position(&car, enemies);

                sprintf(position_text, "%d/6", position_number);
                car_position.source = TTF_RenderText_Blended(electronic, position_text, red);

            }
            else
            {
                SDL_Delay(30 - (current[0] - previous[0]));
            }
        }


        if ( Explode )
        {
            j++;

            if ( current[1] - previous[1] >= 50 )
            {

                // Let's check to see if the car is on the left or the right
                if ( direction == RIGHT )
                {
                    if ( car.x < 586 - 90 )
                        car.x += j;
                }
                else if ( direction == LEFT)
                {
                    if ( car.x > 136 )
                        car.x -= j;
                }

                go.up = 0;
                previous[1] = current[1];

                if ( j > 10)
                {
                    j = 0;
                    Explode = 0;
                    go.up = 1;
                }
            }
            else
            {
                SDL_Delay(50 - (current[1] - previous[1]));
            }
        }

        if (win)
        {
            switch(position_number)
            {
                case 1:
                    sprintf(game_win_text, "BRAVO! LE PREMIER!");
                    break;

                case 2:
                    sprintf(game_win_text, "BRAVO! LE 2EME");
                    break;

                case 3:
                    sprintf(game_win_text, "BRAVO! LE 3EME");
                    break;

                default:
                    //sprintf(game_over_text, "Vous avez perdu!");
                    lose = 1;
                    game_over.source = TTF_RenderText_Blended(electronic, "VOUS AVEZ PERDU!", white);
                    break;
            }

            car.y -= car.speed;
            go.up = 0;

            game_win.source = TTF_RenderText_Blended(electronic, game_win_text, white);
//*
            if ( current[6] - previous[6] >= (GAME_TIME - time + 7)*1000 )
            {
                win = 0;
                game_on = 0;
            }
//*/
        }
        else if (current[3] - previous[3] >= 1000)
        {
            sprintf(delay_time, "%d", time);
            timer.source = TTF_RenderText_Blended(electronic, delay_time, red);

            if ( start ) time--;

            if (time <= 0)
            {

                time = 0;
                go.up = 0;
                go.right = 0;
                go.left = 0;

                // You lose
                game_over.source = TTF_RenderText_Blended(electronic, "Vous avez perdu!", white);
//*
                if ( current[5] - previous[5] >= ( ( GAME_TIME+5 )*1000) )
                {

                    game_on = 0;
                    win = 0;
                }
//*/
            }

            previous[3] = current[3];
        }
        else
        {
            //SDL_Delay(10 - (current[3] - previous[3]));
        }

        // We fill the scrren with chosen color
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255,255,255));

        for ( i=0; i<TOTAL_ROADS; i++ )
        {
           Blit( road.source, screen, 0, roads[i].y ); // we blit the road
        }

        Blit(finish.source,screen, finish.x, finish.y); // The finish Line
        Blit(menu.source,screen, menu.x, menu.y); // The menu
        Blit(logo.source,screen, logo.x, logo.y); // The menu

        Blit(car.source,screen, car.x, car.y); // we blit the main car

        // we blit the enemy cars
        for ( i=0; i<TOTAL_CARS; i++ )
        {
           Blit(enemies[i].source, screen, enemies[i].x, enemies[i].y);
        }

        Blit(fuel.source,screen, fuel.x, fuel.y); // we blit the car
        Blit(timer.source,screen, timer.x, timer.y); // we blit the car
        Blit(car_position.source,screen, car_position.x, car_position.y); // we blit the car

        if (win) Blit(game_win.source,screen, game_win.x, game_win.y); // we blit the win
        if (time == 0 || lose) Blit(game_over.source,screen, game_over.x, game_over.y); // we blit the game_over picture
        if ( !start ) Blit(ike.source, screen, 300, 200);
        // we update the screen
        SDL_Flip(screen);

        unsettimes(current, 10);
        //unsettimes(previous, 10);
    }

    FSOUND_Stream_Stop(car_sound);

    /*
    SDL_FreeSurface(car.source);

    for ( i=0; i<TOTAL_CARS; i++ )
    {
       SDL_FreeSurface(enemies[i].source);
    }

    for ( i=0; i<TOTAL_ROADS; i++ )
    {
       SDL_FreeSurface(road.source); // we blit the road
    }
    SDL_FreeSurface(finish.source);


    SDL_FreeSurface(game_win.source);
    SDL_FreeSurface(game_over.source);
    SDL_FreeSurface(fuel.source);
    SDL_FreeSurface(timer.source);
    */


}

Uint32 quit(Uint32 interval, void *param)
{
    int *continuer = param;
    *continuer = 0;
    return interval;
}

