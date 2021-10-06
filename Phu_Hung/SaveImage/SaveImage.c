int Image_Save(const char* filename)
{
    SDL_Surface * image = IMG_Load("image.png");
    SDL_SaveBMP(image, filename);
    SDL_FreeSurface(image);
 
    return 0;
}
