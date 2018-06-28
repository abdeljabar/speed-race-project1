void Blit(SDL_Surface* source, SDL_Surface* dest, int x, int y);
void move(Sprite *car, Sprite *enemies, int direction );
void forward( Sprite *car,  Sprite *enemies, Sprite *roads, Sprite *finish, int *carState, int *direction, int *win);
void surpass(Sprite *enemies, Sprite *car, int state);
void release_cars(Sprite *enemies);
int check_collision(Sprite *enemies, Sprite *car, int *direction);
int position(Sprite *car, Sprite *enemies);
void settimes(int *time, int total);
void unsettimes(int *time, int total);
