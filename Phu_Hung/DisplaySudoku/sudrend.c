#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#if 0
#define WINDOW_WIDTH        512
#endif
#if 0
#define WINDOW_WIDTH        (64 * 9)
#endif
#if 1
#define WINDOW_WIDTH        (32 * 9)
#endif
#define WINDOW_HEIGHT       (WINDOW_WIDTH)
#define GRIDMAX             (WINDOW_WIDTH / 3)

#define SUDBOXMAX           3
#define SUDXMAX             (SUDBOXMAX * 3)
#define SUDYMAX             (SUDBOXMAX * 3)
typedef int board_t[SUDYMAX][SUDXMAX];

typedef unsigned char byte;

typedef struct {
    byte r;
    byte g;
    byte b;
#ifdef PIX32
    byte a;
#endif
} pixel_t;

int brd[9][9];

TTF_Font *font;

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void
get_text_and_rect(SDL_Renderer *renderer,
    int x, int y,
    char *text, TTF_Font *font,
    SDL_Texture **texture, SDL_Rect *rect)
{
    int text_width;
    int text_height;
    SDL_Surface *surface;
#if 0
    SDL_Color textColor = { 255, 255, 255, 0 };
#endif
#if 0
    SDL_Color textColor = { 237, 135, 45, 0 };
#endif
#if 1
    SDL_Color textColor = { 255, 69, 0, 0 };
#endif

    surface = TTF_RenderText_Solid(font,text,textColor);
    *texture = SDL_CreateTextureFromSurface(renderer,surface);

    text_width = surface->w;
    text_height = surface->h;

    SDL_FreeSurface(surface);

    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

// genboard -- generate sudoku board
void
genboard(int grid[9][9])
{
    for (int y = 0; y < SUDYMAX; y++)
    {
	for (int x = 0; x < SUDYMAX; x++)
	{
             brd[y][x] = grid[y][x];
	}
    }
}

// prtboard -- print sudoku board as text
void
prtboard(int brd[9][9])
{

    for (int y = 0;  y < SUDYMAX;  ++y) {
        if ((y % 3) == 0)
            printf("\n");
        for (int x = 0;  x < SUDYMAX;  ++x) {
            if ((x % 3) == 0)
                printf(" ");
            int val = brd[y][x];
            if (val == 0)
                printf(" *");
            else
                printf(" %d",val);
        }
        printf("\n");
    }
}

void
drawgrid(SDL_Renderer *renderer)
{
    int gridmax;
    int boxmax;
    SDL_Rect rect;

#if 0
    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0);
#endif
#if 0
    SDL_SetRenderDrawColor(renderer,143,188,143,0);
#endif
#if 1
    SDL_SetRenderDrawColor(renderer,0,0,139,0);
#endif

    gridmax = GRIDMAX;
    boxmax = SUDYMAX;
    for (int ybox = 0;  ybox < boxmax;  ++ybox) {
        int ybase = ybox * gridmax;
        rect.y = ybase;
        rect.h = gridmax;
        for (int xbox = 0;  xbox < boxmax;  ++xbox) {
            int xbase = xbox * gridmax;
            rect.x = xbase;
            rect.w = gridmax;
            SDL_RenderDrawRect(renderer,&rect);
        }
    }
}

void
drawbox(SDL_Renderer *renderer)
{
    int gridmax;
    int boxmax;
    SDL_Rect rect;

#if 0
    SDL_SetRenderDrawColor(renderer, 255, 216, 0, 0);
#else
    SDL_SetRenderDrawColor(renderer,255,250,205,0);
#endif
    gridmax = GRIDMAX / 3;
    boxmax = SUDYMAX * 3;
    for (int ybox = 0;  ybox < boxmax;  ++ybox) {
        int ybase = ybox * gridmax;
        rect.y = ybase;
        rect.h = gridmax;
        for (int xbox = 0;  xbox < boxmax;  ++xbox) {
            int xbase = xbox * gridmax;
            rect.x = xbase;
            rect.w = gridmax;
            SDL_RenderDrawRect(renderer,&rect);
        }
    }
}

void
drawtext(SDL_Renderer *renderer)
{
    int gridmax;
    int boxmax_y;
    int boxmax_x;
    SDL_Rect grect;
    SDL_Rect trect;
    SDL_Texture *texture;
    char buf[2];

#if 0
    SDL_SetRenderDrawColor(renderer, 255, 216, 0, 0);
#endif
#if 0
    SDL_SetRenderDrawColor(renderer,255,250,205,0);
#endif
#if 1
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
#endif

    gridmax = GRIDMAX / 3;
#if 0
    boxmax_y = SUDYMAX * 3;
    boxmax_x = SUDXMAX * 3;
#else
    boxmax_y = SUDYMAX;
    boxmax_x = SUDXMAX;
#endif
    for (int ybox = 0;  ybox < boxmax_y;  ++ybox) {
        int ybase = ybox * gridmax;
        grect.y = ybase + gridmax / 3;
        grect.h = gridmax / 3;

        for (int xbox = 0;  xbox < boxmax_x;  ++xbox) {
            int chr = brd[ybox][xbox];
            if (chr != 0)
                buf[0] = chr + '0';
            else
                buf[0] = ' ';
            buf[1] = 0;
            get_text_and_rect(renderer, 0, 0, buf, font, &texture, &trect);

            int xbase = xbox * gridmax;
            grect.x = xbase + gridmax / 3;
            grect.w = gridmax / 3;

            SDL_RenderCopy(renderer, texture, &trect, &grect);

            SDL_DestroyTexture(texture);
        }
    }
}

// imgsave -- save image to P6 .ppm file
void
imgsave(SDL_Renderer *renderer,int imgno)
{
    int pitch;
    FILE *xf;
    char file[1000];
    pixel_t *pixmap;

    pitch = sizeof(pixel_t) * WINDOW_WIDTH;
    pixmap = malloc(pitch * WINDOW_HEIGHT);

    // get pixel map from renderer image
#ifdef PIX32
    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_RGBA32,pixmap,pitch);
#else
    SDL_RenderReadPixels(renderer,NULL,SDL_PIXELFORMAT_RGB24,pixmap,pitch);
#endif

    sprintf(file,"img%3.3d.ppm",imgno);
    xf = fopen(file,"w");

    fprintf(xf,"P6\n");
    fprintf(xf,"%d %d",WINDOW_WIDTH,WINDOW_HEIGHT);
    fprintf(xf," %d\n",255);

#ifdef PIX32
    pixel_t *pixcur = &pixmap[0];
    pixel_t *pixlim = &pixmap[WINDOW_WIDTH * WINDOW_HEIGHT];
    for (;  pixcur < pixlim;  ++pixcur) {
        fputc(pixcur->r,xf);
        fputc(pixcur->g,xf);
        fputc(pixcur->b,xf);
    }
#else
    fwrite(pixmap,sizeof(pixel_t),WINDOW_WIDTH * WINDOW_HEIGHT,xf);
#endif

    fclose(xf);

    free(pixmap);
}

void
fontinit(const char *font_tail)
{
    const char *dir;
    char font_path[1000];

    TTF_Init();

    for (int idx = 0;  idx <= 1;  ++idx) {
        switch (idx) {
        case 0:
            dir = NULL;
            break;
        default:  // NOTE: my system needed this
            dir = "/usr/share/fonts/truetype/freefont";
            break;
        }

        if (dir == NULL)
            strcpy(font_path,font_tail);
        else
            sprintf(font_path,"%s/%s",dir,font_tail);

        font = TTF_OpenFont(font_path, 24);
        if (font != NULL)
            break;
    }

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
}

int
main(int argc, char **argv)
{
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    char *font_path;
    int quit;

    switch (argc) {
    case 1:
        font_path = "FreeSans.ttf";
        break;
    case 2:
        font_path = argv[1];
        break;
    default:
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
        break;
    }
    int grid[9][9] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
                       { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
                       { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
                       { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
                       { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };
    genboard(grid);

    prtboard(brd);
    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window,
        &renderer);

    fontinit(font_path);

#if 0
    SDL_Rect rect1, rect2;
    SDL_Texture *texture1, *texture2;
    get_text_and_rect(renderer, 0, 0, "hello", font, &texture1, &rect1);
    get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font,
        &texture2, &rect2);
#endif

    quit = 0;
    while (1) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT)
                quit = 1;
        }
        if (quit)
            break;

        // set background
#if 0
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
#else
        SDL_SetRenderDrawColor(renderer,135,206,250,0);
#endif
        SDL_RenderClear(renderer);

        drawbox(renderer);
        drawgrid(renderer);
        drawtext(renderer);

        /* Use TTF textures. */
#if 0
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
#endif

        imgsave(renderer,0);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
#if 0
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
#endif
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
