#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#include <cstdio>
extern "C" {
    FILE * __cdecl __iob_func(void)
    {
        return __acrt_iob_func(0);
    }
}
#endif

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>

#if defined(__APPLE__)
    #include <SDL/SDL.h>
    #include <SDL/SDL_ttf.h>
#elif defined(__linux__) || defined(__unix__)
    #if __has_include(<SDL/SDL.h>)
        #include <SDL/SDL.h>
    #else
        #include <SDL.h>
    #endif
    #if __has_include(<SDL/SDL_ttf.h>)
        #include <SDL/SDL_ttf.h>
    #else
        #include <SDL_ttf.h>
    #endif
#else
    #if __has_include(<SDL/SDL.h>)
        #include <SDL/SDL.h>
    #else
        #include <SDL.h>
    #endif
    #if __has_include(<SDL/SDL_ttf.h>)
        #include <SDL/SDL_ttf.h>
    #else
        #include <SDL_ttf.h>
    #endif
#endif

// Base virtual screen dimensions (original 2013 game resolution)
const int VIRTUAL_W = 760;
const int VIRTUAL_H = 757;

// Global screen pointers
static SDL_Surface *g_ecran = NULL;           // Actual window / display surface
static SDL_Surface *g_virtual_screen = NULL;   // 760x757 render buffer
static bool g_fullscreen = false;

static std::string get_asset_path(const char *name)
{
    std::string candidate = std::string("assets/") + name;
    FILE *f = fopen(candidate.c_str(), "rb");
    if (f)
    {
        fclose(f);
        return candidate;
    }
    return std::string(name);
}

// Function prototypes
void present_screen();
void handle_window_events(SDL_Event *event);
SDL_Rect transform_mouse_coords(int mx, int my);

void pause_screen();
void affrond(int pcase);
void affcroix(int pcase);
int coordonnees(SDL_Rect posiclic);
int verifie(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int verifie2(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int vict(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int plein(int a, int b, int c, int d, int e, int f, int g, int h, int i);
void grille();
int choixniveau(SDL_Rect posiclic);
void nice();

void draw_text_centered(SDL_Surface *dst, TTF_Font *font, const char *str, SDL_Color color, int center_x, int top_y);
void draw_button_text(SDL_Surface *dst, TTF_Font *font, const char *str, SDL_Color fg, SDL_Color bg, SDL_Rect btn_rect);
void draw_victory_banner(SDL_Surface *dst, TTF_Font *font, int top_y);
void draw_defeat_banner(SDL_Surface *dst, TTF_Font *font, int top_y);
void draw_draw_banner(SDL_Surface *dst, TTF_Font *font);

// Pixel-format-aware scaling helper for fullscreen / resizing with aspect ratio preservation
void blit_scaled(SDL_Surface *src, SDL_Surface *dst, const SDL_Rect &dst_rect)
{
    if (!src || !dst) return;
    if (SDL_MUSTLOCK(src)) SDL_LockSurface(src);
    if (SDL_MUSTLOCK(dst)) SDL_LockSurface(dst);

    SDL_PixelFormat *sfmt = src->format;
    SDL_PixelFormat *dfmt = dst->format;

    bool same_format = (sfmt->BitsPerPixel == 32 && dfmt->BitsPerPixel == 32 &&
                        sfmt->Rmask == dfmt->Rmask &&
                        sfmt->Gmask == dfmt->Gmask &&
                        sfmt->Bmask == dfmt->Bmask);

    if (same_format)
    {
        Uint32 *src_pixels = (Uint32*)src->pixels;
        Uint32 *dst_pixels = (Uint32*)dst->pixels;
        int src_pitch = src->pitch / 4;
        int dst_pitch = dst->pitch / 4;

        for (int y = 0; y < dst_rect.h; ++y)
        {
            int src_y = (y * src->h) / dst_rect.h;
            if (src_y >= src->h) src_y = src->h - 1;
            Uint32 *dst_row = dst_pixels + (dst_rect.y + y) * dst_pitch + dst_rect.x;
            Uint32 *src_row = src_pixels + src_y * src_pitch;

            for (int x = 0; x < dst_rect.w; ++x)
            {
                int src_x = (x * src->w) / dst_rect.w;
                if (src_x >= src->w) src_x = src->w - 1;
                dst_row[x] = src_row[src_x];
            }
        }
    }
    else
    {
        Uint32 *src_pixels = (Uint32*)src->pixels;
        int src_pitch = src->pitch / 4;

        for (int y = 0; y < dst_rect.h; ++y)
        {
            int src_y = (y * src->h) / dst_rect.h;
            if (src_y >= src->h) src_y = src->h - 1;
            Uint32 *src_row = src_pixels + src_y * src_pitch;

            for (int x = 0; x < dst_rect.w; ++x)
            {
                int src_x = (x * src->w) / dst_rect.w;
                if (src_x >= src->w) src_x = src->w - 1;
                Uint32 spix = src_row[src_x];

                Uint8 r = (Uint8)(((spix & sfmt->Rmask) >> sfmt->Rshift) << sfmt->Rloss);
                Uint8 g = (Uint8)(((spix & sfmt->Gmask) >> sfmt->Gshift) << sfmt->Gloss);
                Uint8 b = (Uint8)(((spix & sfmt->Bmask) >> sfmt->Bshift) << sfmt->Bloss);

                Uint32 dpix = ((r >> dfmt->Rloss) << dfmt->Rshift) |
                              ((g >> dfmt->Gloss) << dfmt->Gshift) |
                              ((b >> dfmt->Bloss) << dfmt->Bshift);

                if (dfmt->BitsPerPixel == 32)
                {
                    Uint32 *dst_pixels = (Uint32*)dst->pixels;
                    int dst_pitch = dst->pitch / 4;
                    dst_pixels[(dst_rect.y + y) * dst_pitch + (dst_rect.x + x)] = dpix;
                }
                else if (dfmt->BitsPerPixel == 16)
                {
                    Uint16 *dst_pixels = (Uint16*)dst->pixels;
                    int dst_pitch = dst->pitch / 2;
                    dst_pixels[(dst_rect.y + y) * dst_pitch + (dst_rect.x + x)] = (Uint16)dpix;
                }
            }
        }
    }

    if (SDL_MUSTLOCK(src)) SDL_UnlockSurface(src);
    if (SDL_MUSTLOCK(dst)) SDL_UnlockSurface(dst);
}

void present_screen()
{
    if (!g_virtual_screen || !g_ecran) return;

    if (g_ecran->w == g_virtual_screen->w && g_ecran->h == g_virtual_screen->h &&
        g_ecran->format->Rmask == g_virtual_screen->format->Rmask &&
        g_ecran->format->Gmask == g_virtual_screen->format->Gmask &&
        g_ecran->format->Bmask == g_virtual_screen->format->Bmask &&
        g_ecran->format->BitsPerPixel == g_virtual_screen->format->BitsPerPixel)
    {
        SDL_BlitSurface(g_virtual_screen, NULL, g_ecran, NULL);
    }
    else
    {
        float scale = ((float)g_ecran->w / VIRTUAL_W < (float)g_ecran->h / VIRTUAL_H)
                    ? ((float)g_ecran->w / VIRTUAL_W)
                    : ((float)g_ecran->h / VIRTUAL_H);
        SDL_Rect dst;
        dst.w = (Uint16)(VIRTUAL_W * scale);
        dst.h = (Uint16)(VIRTUAL_H * scale);
        dst.x = (Sint16)((g_ecran->w - dst.w) / 2);
        dst.y = (Sint16)((g_ecran->h - dst.h) / 2);

        SDL_FillRect(g_ecran, NULL, 0x000000);
        blit_scaled(g_virtual_screen, g_ecran, dst);
    }
    SDL_Flip(g_ecran);
}

void toggle_fullscreen()
{
    g_fullscreen = !g_fullscreen;
    Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF | (g_fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE);
    int w = g_fullscreen ? 0 : VIRTUAL_W;
    int h = g_fullscreen ? 0 : VIRTUAL_H;
    g_ecran = SDL_SetVideoMode(w, h, 32, flags);
    if (!g_ecran)
    {
        g_fullscreen = false;
        g_ecran = SDL_SetVideoMode(VIRTUAL_W, VIRTUAL_H, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
    }
    present_screen();
}

void handle_window_events(SDL_Event *event)
{
    if (event->type == SDL_VIDEORESIZE)
    {
        g_ecran = SDL_SetVideoMode(event->resize.w, event->resize.h, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
        present_screen();
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_F11 ||
            (event->key.keysym.sym == SDLK_RETURN && (event->key.keysym.mod & KMOD_ALT)) ||
            event->key.keysym.sym == SDLK_f)
        {
            toggle_fullscreen();
        }
    }
}

SDL_Rect transform_mouse_coords(int mx, int my)
{
    SDL_Rect res = {0, 0, 0, 0};
    if (!g_ecran) return res;

    float scale = ((float)g_ecran->w / VIRTUAL_W < (float)g_ecran->h / VIRTUAL_H)
                ? ((float)g_ecran->w / VIRTUAL_W)
                : ((float)g_ecran->h / VIRTUAL_H);
    int dst_w = (int)(VIRTUAL_W * scale);
    int dst_h = (int)(VIRTUAL_H * scale);
    int dst_x = (g_ecran->w - dst_w) / 2;
    int dst_y = (g_ecran->h - dst_h) / 2;

    if (mx >= dst_x && mx < dst_x + dst_w && my >= dst_y && my < dst_y + dst_h)
    {
        res.x = (Sint16)((mx - dst_x) * (float)VIRTUAL_W / dst_w);
        res.y = (Sint16)((my - dst_y) * (float)VIRTUAL_H / dst_h);
    }
    else
    {
        res.x = -1;
        res.y = -1;
    }
    return res;
}

void draw_text_centered(SDL_Surface *dst, TTF_Font *font, const char *str, SDL_Color color, int center_x, int top_y)
{
    if (!font || !str || !dst) return;
    SDL_Surface *txt = TTF_RenderUTF8_Blended(font, str, color);
    if (txt)
    {
        SDL_Rect r = { (Sint16)(center_x - txt->w / 2), (Sint16)top_y, (Uint16)txt->w, (Uint16)txt->h };
        SDL_BlitSurface(txt, NULL, dst, &r);
        SDL_FreeSurface(txt);
    }
}

void draw_button_text(SDL_Surface *dst, TTF_Font *font, const char *str, SDL_Color fg, SDL_Color bg, SDL_Rect btn_rect)
{
    if (!font || !str || !dst) return;
    SDL_Surface *txt = TTF_RenderUTF8_Shaded(font, str, fg, bg);
    if (txt)
    {
        SDL_Rect r = { (Sint16)(btn_rect.x + (btn_rect.w - txt->w) / 2), (Sint16)(btn_rect.y + (btn_rect.h - txt->h) / 2), (Uint16)txt->w, (Uint16)txt->h };
        SDL_BlitSurface(txt, NULL, dst, &r);
        SDL_FreeSurface(txt);
    }
}

void draw_victory_banner(SDL_Surface *dst, TTF_Font *font, int top_y)
{
    const char *letters[] = {"V", "I", "C", "T", "O", "I", "R", "E"};
    SDL_Color colors[] = {
        {255, 0, 0},     // Rouge
        {255, 81, 255},  // Rose
        {255, 123, 0},   // Orange
        {255, 233, 0},   // Jaune
        {177, 255, 23},  // Verte pomme
        {10, 206, 212},  // Indigo
        {0, 0, 255},     // Bleu marine
        {165, 32, 175}   // Violette
    };
    int num_letters = 8;
    int letter_widths[8];
    int total_width = 0;
    int spacing = 10;

    for (int k = 0; k < num_letters; ++k)
    {
        int w = 0, h = 0;
        TTF_SizeUTF8(font, letters[k], &w, &h);
        letter_widths[k] = w;
        total_width += w;
        if (k > 0) total_width += spacing;
    }

    int start_x = (VIRTUAL_W - total_width) / 2;
    int curr_x = start_x;

    for (int k = 0; k < num_letters; ++k)
    {
        SDL_Surface *txt = TTF_RenderUTF8_Blended(font, letters[k], colors[k]);
        if (txt)
        {
            SDL_Rect r = { (Sint16)curr_x, (Sint16)top_y, (Uint16)txt->w, (Uint16)txt->h };
            SDL_BlitSurface(txt, NULL, dst, &r);
            SDL_FreeSurface(txt);
        }
        curr_x += letter_widths[k] + spacing;
    }
}

void draw_defeat_banner(SDL_Surface *dst, TTF_Font *font, int top_y)
{
    const char *letters[] = {"D", "E", "F", "A", "I", "T", "E"};
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Color couleurbeige = {240, 213, 185};
    SDL_Color colors[] = {
        couleurNoire, couleurbeige, couleurNoire, couleurbeige,
        couleurNoire, couleurbeige, couleurNoire
    };
    int num_letters = 7;
    int letter_widths[7];
    int total_width = 0;
    int spacing = 10;

    for (int k = 0; k < num_letters; ++k)
    {
        int w = 0, h = 0;
        TTF_SizeUTF8(font, letters[k], &w, &h);
        letter_widths[k] = w;
        total_width += w;
        if (k > 0) total_width += spacing;
    }

    int start_x = (VIRTUAL_W - total_width) / 2;
    int curr_x = start_x;

    for (int k = 0; k < num_letters; ++k)
    {
        SDL_Surface *txt = TTF_RenderUTF8_Blended(font, letters[k], colors[k]);
        if (txt)
        {
            SDL_Rect r = { (Sint16)curr_x, (Sint16)top_y, (Uint16)txt->w, (Uint16)txt->h };
            SDL_BlitSurface(txt, NULL, dst, &r);
            SDL_FreeSurface(txt);
        }
        curr_x += letter_widths[k] + spacing;
    }
}

void draw_draw_banner(SDL_Surface *dst, TTF_Font *font)
{
    SDL_Color couleurviolette = {165, 32, 175};
    draw_text_centered(dst, font, "MATCH", couleurviolette, VIRTUAL_W / 2, 60);
    draw_text_centered(dst, font, "NUL", couleurviolette, VIRTUAL_W / 2, 170);
}

int main ( int argc, char* argv[] )
{
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) == -1)
    {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Morpion ! (F11: Plein écran)", NULL);
    srand((unsigned int)time(NULL));

    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Color couleurindigo = {10, 206, 212};
    SDL_Color couleurverte = {128, 255, 23};
    SDL_Color couleurneutre = {190, 150, 120};
    SDL_Color couleurrouge = {255, 0, 0};
    SDL_Color couleurblanche = {255, 255, 255};

    TTF_Font *police = NULL;

    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, joue = 0, rejouer = 1;
    int lasse = 0;
    int booleen = 1;

    SDL_Rect posiclic = {0, 0, 0, 0};

    // Set window icon safely
    SDL_Surface *icon = SDL_LoadBMP(get_asset_path("sdl_icone.bmp").c_str());
    if (icon)
    {
        SDL_WM_SetIcon(icon, NULL);
    }

    // Set initial window video mode
    g_ecran = SDL_SetVideoMode(VIRTUAL_W, VIRTUAL_H, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
    if (g_ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create virtual rendering surface matching display format
    g_virtual_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, VIRTUAL_W, VIRTUAL_H, 32,
        g_ecran->format->Rmask, g_ecran->format->Gmask, g_ecran->format->Bmask, g_ecran->format->Amask);

    if (g_virtual_screen == NULL)
    {
        fprintf(stderr, "Impossible de créer la surface virtuelle\n");
        exit(EXIT_FAILURE);
    }

    grille();

    SDL_Event event;
    int continuer = 1, coord = 0, vvict = 0, pplein = 0, nombremys = 0, min = 1, max = 4, niveau = 0, lignes = 0;

    while(rejouer == 1 && lasse == 0)
    {
        nice();
        police = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 40);

        // Menu title cleanly centered
        draw_text_centered(g_virtual_screen, police, "Contre quel niveau d'I.A veux-tu jouer ?", couleurblanche, VIRTUAL_W / 2, 275);

        // Menu buttons with centered text inside their clickable boxes
        SDL_Rect btn_facile = {120, 350, 298, 67};
        draw_button_text(g_virtual_screen, police, "  Vraiment Facile  ", couleurNoire, couleurverte, btn_facile);

        SDL_Rect btn_normal = {454, 350, 264, 67};
        draw_button_text(g_virtual_screen, police, "   Normal   ", couleurNoire, couleurneutre, btn_normal);

        SDL_Rect btn_difficile = {120, 451, 298, 67};
        draw_button_text(g_virtual_screen, police, "   Difficile   ", couleurNoire, couleurrouge, btn_difficile);

        SDL_Rect btn_impossible = {454, 451, 264, 67};
        draw_button_text(g_virtual_screen, police, "  Impossible  ", couleurblanche, couleurNoire, btn_impossible);

        present_screen();

        if (police) { TTF_CloseFont(police); police = NULL; }

        continuer = 1;
        while(continuer)
        {
            SDL_WaitEvent(&event);
            handle_window_events(&event);

            switch(event.type)
            {
            case SDL_QUIT:
                continuer = 0;
                lasse = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                posiclic = transform_mouse_coords(event.button.x, event.button.y);
                niveau = choixniveau(posiclic);
                if(niveau != 0)
                    continuer = 0;
                else
                    continuer = 1;
                break;
            }
        }
        niveau = choixniveau(posiclic);

        if(lasse == 0)
        {
            grille();
            do
            {
                continuer = 1;
                while(continuer)
                {
                    SDL_WaitEvent(&event);
                    handle_window_events(&event);

                    switch(event.type)
                    {
                    case SDL_QUIT:
                        continuer = 0;
                        lasse = 1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        posiclic = transform_mouse_coords(event.button.x, event.button.y);
                        continuer = 0;
                        break;
                    }
                }

                coord = 0;
                coord = coordonnees(posiclic);

                if(coord == 9) { i = 1; lignes = 0; }
                else if(coord == 8) { h = 1; lignes = 0; }
                else if(coord == 7) { g = 1; lignes = 0; }
                else if(coord == 6) { f = 1; lignes = 0; }
                else if(coord == 5) { e = 1; lignes = 0; }
                else if(coord == 4) { d = 1; lignes = 0; }
                else if(coord == 3) { c = 1; lignes = 0; }
                else if(coord == 2) { b = 1; lignes = 0; }
                else if(coord == 1) { a = 1; lignes = 0; }
                else if(coord == 0) lignes = 1;

            } while(lignes == 1 && lasse == 0);

            if(lasse == 0)
                affrond(coord);
        }

        if(e == 0 && lasse == 0 && niveau > 2)
        {
            affcroix(5);
            e = 2;
        }
        else if(lasse == 0 && e == 1 && niveau > 2)
        {
            min = 1;
            max = 4;
            nombremys = (rand() % (max - min + 1)) + min;

            if(nombremys == 1) { affcroix(1); a = 2; }
            if(nombremys == 2) { affcroix(3); c = 2; }
            if(nombremys == 3) { affcroix(7); g = 2; }
            if(nombremys == 4) { affcroix(9); i = 2; }
        }

        if(lasse == 0 && niveau == 2) // Niveau normal
        {
            min = 1;
            max = 9;
            while(a<2 && b<2 && c<2 && d<2 && e<2 && f<2 && g<2 && h<2 && i<2)
            {
                nombremys = (rand() % (max - min + 1)) + min;
                if(nombremys == 1 && a==0) { affcroix(1); a = 2; }
                if(nombremys == 2 && b==0) { affcroix(2); b = 2; }
                if(nombremys == 3 && c==0) { affcroix(3); c = 2; }
                if(nombremys == 4 && d==0) { affcroix(4); d = 2; }
                if(nombremys == 5 && e==0) { affcroix(5); e = 2; }
                if(nombremys == 6 && f==0) { affcroix(6); f = 2; }
                if(nombremys == 7 && g==0) { affcroix(7); g = 2; }
                if(nombremys == 8 && h==0) { affcroix(8); h = 2; }
                if(nombremys == 9 && i==0) { affcroix(9); i = 2; }
            }
        }

        if(lasse == 0 && niveau == 1) // Niveau vraiment facile
        {
            while(a<2 && b<2 && c<2 && d<2 && e<2 && f<2 && g<2 && h<2 && i<2)
            {
                min = 1;
                max = 4;
                nombremys = (rand() % (max - min + 1)) + min;

                if(nombremys == 1 && b == 0) { affcroix(2); b = 2; }
                if(nombremys == 2 && d == 0) { affcroix(4); d = 2; }
                if(nombremys == 3 && f == 0) { affcroix(6); f = 2; }
                if(nombremys == 4 && h == 0) { affcroix(8); h = 2; }
            }
        }

        // Début du while
        while(vvict == 0 && pplein == 0 && lasse == 0)
        {
            do
            {
                coord = 0;
                joue = 0;
                booleen = 1;

                continuer = 1;
                while(continuer)
                {
                    SDL_WaitEvent(&event);
                    handle_window_events(&event);

                    switch(event.type)
                    {
                    case SDL_QUIT:
                        continuer = 0;
                        lasse = 1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        posiclic = transform_mouse_coords(event.button.x, event.button.y);
                        continuer = 0;
                        break;
                    }
                }

                if(lasse == 0 && vvict == 0 && pplein == 0)
                {
                    coord = coordonnees(posiclic);

                    if(coord == 9 && i == 0) { i = 1; lignes = 0; affrond(coord); }
                    else if(coord == 8 && h == 0) { h = 1; lignes = 0; affrond(coord); }
                    else if(coord == 7 && g == 0) { g = 1; lignes = 0; affrond(coord); }
                    else if(coord == 6 && f == 0) { f = 1; lignes = 0; affrond(coord); }
                    else if(coord == 5 && e == 0) { e = 1; lignes = 0; affrond(coord); }
                    else if(coord == 4 && d == 0) { d = 1; lignes = 0; affrond(coord); }
                    else if(coord == 3 && c == 0) { c = 1; lignes = 0; affrond(coord); }
                    else if(coord == 2 && b == 0) { b = 1; lignes = 0; affrond(coord); }
                    else if(coord == 1 && a == 0) { a = 1; affrond(coord); lignes = 0; }
                    else lignes = 1;

                    vvict = vict(a, b, c, d, e, f, g, h, i);
                    pplein = plein(a, b, c, d, e, f, g, h, i);
                }

            } while(lignes == 1 && lasse == 0);

            if(lasse == 0 && vvict == 0 && pplein == 0)
            {
                if(niveau == 1)
                    joue = verifie2(a, b, c, d, e, f, g, h, i);
                else
                    joue = verifie(a, b, c, d, e, f, g, h, i);

                affcroix(joue);
                if(joue == 9) i = 2;
                else if(joue == 8) h = 2;
                else if(joue == 7) g = 2;
                else if(joue == 6) f = 2;
                else if(joue == 5) e = 2;
                else if(joue == 4) d = 2;
                else if(joue == 3) c = 2;
                else if(joue == 2) b = 2;
                else if(joue == 1) a = 2;
            }

            if(joue == 0 && vvict == 0 && pplein == 0)
            {
                if(niveau == 4)
                {
                    if(a==1 && i==1 && h==0) { affcroix(8); h=2; booleen = 0; }
                    else if(g==1 && c==1 && h==0) { affcroix(8); h=2; booleen = 0; }
                    else if(a==1 && h==1 && g==0) { affcroix(7); g=2; booleen = 0; }
                    else if(a==1 && f==1 && c==0) { affcroix(3); c=2; booleen = 0; }
                    else if(g==1 && b==1 && a==0) { affcroix(1); a=2; booleen = 0; }
                    else if(g==1 && f==1 && i==0) { affcroix(9); i=2; booleen = 0; }
                    else if(c==1 && d==1 && a==0) { affcroix(1); a=2; booleen = 0; }
                    else if(c==1 && h==1 && i==0) { affcroix(9); i=2; booleen = 0; }
                    else if(i==1 && b==1 && c==0) { affcroix(3); c=2; booleen = 0; }
                    else if(i==1 && d==1 && g==0) { affcroix(7); g=2; booleen = 0; }
                    else if(h == 1 && f == 1 && i==0) { affcroix(9); i = 2; booleen = 0; }
                    else if(h == 1 && d==1 && g == 0) { affcroix(7); g = 2; booleen = 0; }
                    else if(b == 1 && d == 1 && a == 0) { affcroix(1); a = 2; booleen = 0; }
                    else if(b == 1 && f == 1 && c == 0) { affcroix(3); c = 2; booleen = 0; }
                }

                if(booleen)
                {
                    if(a==0) { affcroix(1); a=2; }
                    else if(c==0) { affcroix(3); c=2; }
                    else if(g==0) { affcroix(7); g=2; }
                    else if(i==0) { affcroix(9); i=2; }
                    else if(b==0) { affcroix(2); b=2; }
                    else if(f==0) { affcroix(6); f=2; }
                    else if(d==0) { affcroix(4); d=2; }
                    else if(h==0) { affcroix(8); h=2; }
                }
            }

            vvict = vict(a,b,c,d,e,f,g,h,i);
            pplein = plein(a,b,c,d,e,f,g,h,i);
        }

        // Fin du round / affichage résultat
        if(lasse == 0)
        {
            police = TTF_OpenFont(get_asset_path("Graduate-Regular.ttf").c_str(), 95);

            if(vvict == 1)
            {
                draw_victory_banner(g_virtual_screen, police, 100);
                present_screen();
            }
            else if(vvict == 2)
            {
                draw_defeat_banner(g_virtual_screen, police, 100);
                present_screen();
            }
            else if(pplein == 1)
            {
                draw_draw_banner(g_virtual_screen, police);
                present_screen();
            }

            if (police) { TTF_CloseFont(police); police = NULL; }

            police = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 42);

            // Replay question cleanly centered
            draw_text_centered(g_virtual_screen, police, "Veux-tu rejouer contre l'I.A ?", couleurNoire, VIRTUAL_W / 2, 275);

            // OUI / NON buttons centered in their clickable zones
            SDL_Rect btn_oui = {199, 350, 86, 70};
            draw_button_text(g_virtual_screen, police, " OUI ", couleurNoire, couleurindigo, btn_oui);

            SDL_Rect btn_non = {500, 350, 105, 70};
            draw_button_text(g_virtual_screen, police, " NON ", couleurNoire, couleurindigo, btn_non);

            present_screen();

            if (police) { TTF_CloseFont(police); police = NULL; }

            continuer = 1;
            while(continuer)
            {
                SDL_WaitEvent(&event);
                handle_window_events(&event);

                switch(event.type)
                {
                case SDL_QUIT:
                    continuer = 0;
                    lasse = 1;
                    rejouer = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
                    posiclic = transform_mouse_coords(event.button.x, event.button.y);
                    if (posiclic.x >= 199 && posiclic.x <= 285 && posiclic.y >= 350 && posiclic.y <= 420)
                    {
                        rejouer = 1;
                        vvict = 0;
                        pplein = 0;
                        a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 0; h = 0; i = 0;
                        continuer = 0;
                    }
                    else if(posiclic.x >= 500 && posiclic.x <= 605 && posiclic.y >= 350 && posiclic.y <= 420)
                    {
                        rejouer = 0;
                        continuer = 0;
                    }
                    else
                        continuer = 1;
                    break;
                }
            }
        }
    }

    // Affichage des crédits.
    // SDL_QUIT (clic sur la croix rouge) est seulement un signal logique : la fenêtre SDL reste
    // physiquement valide jusqu'à ce que SDL_Quit() soit appelé explicitement.
    // On peut donc toujours dessiner les crédits dans les deux cas (fermeture normale ou via la croix).
    pause_screen();

    // Sur Windows x64, SDL_Quit(), TTF_Quit() et même le simple "return 0" de main() déclenchent
    // une chaîne de handlers atexit et de destructeurs C++ enregistrés par SDL 1.2 qui accèdent à
    // des pointeurs de fonctions internes 32-bit invalides sous un processus 64-bit, provoquant
    // une violation d'accès 0xC0000005.
    //
    // _exit(0) court-circuite TOUTE cette chaîne (pas de destructeurs, pas d'atexit, pas de
    // SDL_Quit()) et demande directement au noyau Windows de terminer le processus.
    //
    //
    // Sur Linux / macOS, SDL 1.2 fonctionne correctement en 64-bit : on fait un return normal.
#if defined(_WIN32)
    _exit(0);
#else
    if (icon) SDL_FreeSurface(icon);
    if (g_virtual_screen) SDL_FreeSurface(g_virtual_screen);
    TTF_Quit();
    SDL_Quit();
    return 0;
#endif
}

void pause_screen()
{
    int continuer = 1;
    SDL_Event event;

    grille();

    TTF_Font *p1 = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 44);
    if (p1)
    {
        SDL_Color couleurNoire = {0, 0, 0};
        draw_text_centered(g_virtual_screen, p1, "Ce programme a été créé par :", couleurNoire, VIRTUAL_W / 2, 220);
        TTF_CloseFont(p1);
    }

    TTF_Font *p2 = TTF_OpenFont(get_asset_path("Graduate-Regular.ttf").c_str(), 60);
    if (p2)
    {
        SDL_Color couleurNoire = {0, 0, 0};
        draw_text_centered(g_virtual_screen, p2, "FromTheRags", couleurNoire, VIRTUAL_W / 2, 310);
        TTF_CloseFont(p2);
    }

    present_screen();

    // Flush any pending clicks from previous screen so credits stay visible
    SDL_Delay(250);
    SDL_Event dummy;
    while (SDL_PollEvent(&dummy)) {}

    while (continuer)
    {
        if (SDL_WaitEvent(&event) == 0) break;
        handle_window_events(&event);
        switch(event.type)
        {
        case SDL_QUIT:
        case SDL_KEYDOWN:
        case SDL_MOUSEBUTTONUP:
            continuer = 0;
            break;
        }
    }
}

void affrond(int pcase)
{
    if (pcase <= 0 || !g_virtual_screen) return;
    SDL_Surface *rond = SDL_LoadBMP(get_asset_path("rond.bmp").c_str());
    if (!rond) return;

    SDL_Rect posirond = {0, 0, (Uint16)rond->w, (Uint16)rond->h};
    if(pcase == 9) { posirond.x = 498; posirond.y = 48; }
    if(pcase == 8) { posirond.x = 269; posirond.y = 48; }
    if(pcase == 7) { posirond.x = 40;  posirond.y = 48; }
    if(pcase == 6) { posirond.x = 498; posirond.y = 275; }
    if(pcase == 5) { posirond.x = 269; posirond.y = 275; }
    if(pcase == 4) { posirond.x = 40;  posirond.y = 275; }
    if(pcase == 3) { posirond.x = 498; posirond.y = 502; }
    if(pcase == 2) { posirond.x = 269; posirond.y = 502; }
    if(pcase == 1) { posirond.x = 40;  posirond.y = 502; }

    SDL_SetColorKey(rond, SDL_SRCCOLORKEY, SDL_MapRGB(rond->format, 255, 255, 255));
    SDL_BlitSurface(rond, NULL, g_virtual_screen, &posirond);
    present_screen();
    SDL_FreeSurface(rond);
}

void affcroix(int pcase)
{
    if (pcase <= 0 || !g_virtual_screen) return;
    SDL_Surface *croix = SDL_LoadBMP(get_asset_path("croix.bmp").c_str());
    if (!croix) return;

    SDL_Rect posicroix = {0, 0, (Uint16)croix->w, (Uint16)croix->h};
    if(pcase == 9) { posicroix.x = 498; posicroix.y = 48; }
    if(pcase == 8) { posicroix.x = 269; posicroix.y = 48; }
    if(pcase == 7) { posicroix.x = 40;  posicroix.y = 48; }
    if(pcase == 6) { posicroix.x = 498; posicroix.y = 275; }
    if(pcase == 5) { posicroix.x = 269; posicroix.y = 275; }
    if(pcase == 4) { posicroix.x = 40;  posicroix.y = 275; }
    if(pcase == 3) { posicroix.x = 498; posicroix.y = 502; }
    if(pcase == 2) { posicroix.x = 269; posicroix.y = 502; }
    if(pcase == 1) { posicroix.x = 40;  posicroix.y = 502; }

    SDL_SetColorKey(croix, SDL_SRCCOLORKEY, SDL_MapRGB(croix->format, 255, 255, 255));
    SDL_BlitSurface(croix, NULL, g_virtual_screen, &posicroix);
    present_screen();
    SDL_FreeSurface(croix);
}

int coordonnees(SDL_Rect posiclic)
{
    if(posiclic.x >= 498 && posiclic.x <= 710 && posiclic.y >= 48 && posiclic.y <= 260) return 9;
    if(posiclic.x >= 269 && posiclic.x <= 481 && posiclic.y >= 48 && posiclic.y <= 260) return 8;
    if(posiclic.x >= 40 && posiclic.x <= 252 && posiclic.y >= 48 && posiclic.y <= 260) return 7;
    if(posiclic.x >= 498 && posiclic.x <= 710 && posiclic.y >= 275 && posiclic.y <= 487) return 6;
    if(posiclic.x >= 269 && posiclic.x <= 481 && posiclic.y >= 275 && posiclic.y <= 487) return 5;
    if(posiclic.x >= 40 && posiclic.x <= 252 && posiclic.y >= 275 && posiclic.y <= 487) return 4;
    if(posiclic.x >= 498 && posiclic.x <= 710 && posiclic.y >= 502 && posiclic.y <= 714) return 3;
    if(posiclic.x >= 269 && posiclic.x <= 481 && posiclic.y >= 502 && posiclic.y <= 714) return 2;
    if(posiclic.x >= 40 && posiclic.x <= 252 && posiclic.y >= 502 && posiclic.y <= 714) return 1;

    return 0;
}

int verifie(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    if(a==2 && d == 2 && g == 0) return 7;
    else if(b==2 && e==2 && h== 0) return 8;
    else if(c==2 && f==2 && i==0) return 9;
    else if(d==2 && g==2 && a==0) return 1;
    else if(e==2 && h== 2 && b==0) return 2;
    else if(f==2 && i ==2 && c==0) return 3;
    else if(a == 2 && b==2 && c == 0) return 3;
    else if(d==2 && e==2 && f==0) return 6;
    else if(g == 2 && h==2 && i==0) return 9;
    else if(b==2 && c==2 && a==0) return 1;
    else if(e == 2 && f == 2 && d==0) return 4;
    else if(i == 2 && h==2 && g==0) return 7;
    else if(a==2 && e==2 && i==0) return 9;
    else if(e==2 && i==2 && a==0) return 1;
    else if(c==2 && e==2 && g==0) return 7;
    else if(g==2 && e==2 && c==0) return 3;
    else if(a==2 && i==2 && e==0) return 5;
    else if(g==2 && c==2 && e==0) return 5;
    else if(a==2 && g==2 && d==0) return 4;
    else if(h==2 && b==2 && e==0) return 5;
    else if(c==2 && i==2 && f==0) return 6;
    else if(a==2 && c==2 && b==0) return 2;
    else if(d==2 && f==2 && e==0) return 5;
    else if(g==2 && i==2 && h==0) return 8;

    else if(a==1 && d == 1 && g == 0) return 7;
    else if(b==1 && e==1 && h== 0) return 8;
    else if(c==1 && f==1 && i==0) return 9;
    else if(d==1 && g==1 && a==0) return 1;
    else if(e==1 && h== 1 && b==0) return 2;
    else if(f==1 && i ==1 && c==0) return 3;
    else if(a == 1 && b==1 && c == 0) return 3;
    else if(d==1 && e==1 && f==0) return 6;
    else if(g == 1 && h==1 && i==0) return 9;
    else if(b==1 && c==1 && a==0) return 1;
    else if(e==1 && f==1 && d==0) return 4;
    else if(i == 1 && h==1 && g==0) return 7;
    else if(a==1 && e==1 && i==0) return 9;
    else if(e==1 && i==1 && a==0) return 1;
    else if(c==1 && e==1 && g==0) return 7;
    else if(g==1 && e==1 && c==0) return 3;
    else if(a==1 && i==1 && e==0) return 5;
    else if(g==1 && c==1 && e==0) return 5;
    else if(a==1 && g==1 && d==0) return 4;
    else if(h==1 && b==1 && e==0) return 5;
    else if(c==1 && i==1 && f==0) return 6;
    else if(a==1 && c==1 && b==0) return 2;
    else if(d==1 && f==1 && e==0) return 5;
    else if(g==1 && i==1 && h==0) return 8;

    return 0;
}

int vict(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    if(a==1 && b==1 && c==1) return 1;
    if(d==1 && e==1 && f==1) return 1;
    if(g==1 && h==1 && i==1) return 1;
    if(a==1 && d==1 && g==1) return 1;
    if(b==1 && e==1 && h==1) return 1;
    if(c==1 && f==1 && i==1) return 1;
    if(a==1 && e==1 && i==1) return 1;
    if(c==1 && e==1 && g==1) return 1;

    if(a==2 && b==2 && c==2) return 2;
    if(d==2 && e==2 && f==2) return 2;
    if(g==2 && h==2 && i==2) return 2;
    if(a==2 && d==2 && g==2) return 2;
    if(b==2 && e==2 && h==2) return 2;
    if(c==2 && f==2 && i==2) return 2;
    if(a==2 && e==2 && i==2) return 2;
    if(c==2 && e==2 && g==2) return 2;

    return 0;
}

int plein(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    if(a!=0 && b!=0 && c!=0 && d!=0 && e!=0 && f!=0 && g!=0 && h!=0 && i!=0)
    {
        return 1;
    }
    return 0;
}

void grille()
{
    SDL_Surface *imagedefond = SDL_LoadBMP(get_asset_path("grille.bmp").c_str());
    if (imagedefond && g_virtual_screen)
    {
        SDL_Rect image = {0, 0, (Uint16)imagedefond->w, (Uint16)imagedefond->h};
        SDL_BlitSurface(imagedefond, NULL, g_virtual_screen, &image);
        SDL_FreeSurface(imagedefond);
    }
    present_screen();
}

int choixniveau(SDL_Rect posiclic)
{
    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 350 && posiclic.y <= 417) return 1;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 350 && posiclic.y <= 417) return 2;
    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 451 && posiclic.y <= 518) return 3;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 451 && posiclic.y <= 518) return 4;

    return 0;
}

void nice()
{
    SDL_Surface *imagedefond = SDL_LoadBMP(get_asset_path("nice.bmp").c_str());
    if (imagedefond && g_virtual_screen)
    {
        SDL_Rect image = {0, 0, (Uint16)imagedefond->w, (Uint16)imagedefond->h};
        SDL_BlitSurface(imagedefond, NULL, g_virtual_screen, &image);
        SDL_FreeSurface(imagedefond);
    }
    present_screen();
}

int verifie2(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    if(a==2 && d == 2 && g == 0) return 7;
    else if(b==2 && e==2 && h== 0) return 8;
    else if(c==2 && f==2 && i==0) return 9;
    else if(d==2 && g==2 && a==0) return 1;
    else if(e==2 && h== 2 && b==0) return 2;
    else if(f==2 && i ==2 && c==0) return 3;
    else if(a == 2 && b==2 && c == 0) return 3;
    else if(d==2 && e==2 && f==0) return 6;
    else if(g == 2 && h==2 && i==0) return 9;
    else if(b==2 && c==2 && a==0) return 1;
    else if(e == 2 && f == 2 && d==0) return 4;
    else if(i == 2 && h==2 && g==0) return 7;
    else if(a==2 && e==2 && i==0) return 9;
    else if(e==2 && i==2 && a==0) return 1;
    else if(c==2 && e==2 && g==0) return 7;
    else if(g==2 && e==2 && c==0) return 3;

    else if(a==1 && d == 1 && g == 0) return 7;
    else if(b==1 && e==1 && h== 0) return 8;
    else if(c==1 && f==1 && i==0) return 9;
    else if(d==1 && g==1 && a==0) return 1;
    else if(e==1 && h== 1 && b==0) return 2;
    else if(f==1 && i ==1 && c==0) return 3;
    else if(a == 1 && b==1 && c == 0) return 3;
    else if(d==1 && e==1 && f==0) return 6;
    else if(g == 1 && h==1 && i==0) return 9;
    else if(b==1 && c==1 && a==0) return 1;
    else if(e==1 && f==1 && d==0) return 4;
    else if(i == 1 && h==1 && g==0) return 7;
    else if(a==1 && e==1 && i==0) return 9;
    else if(e==1 && i==1 && a==0) return 1;
    else if(c==1 && e==1 && g==0) return 7;
    else if(g==1 && e==1 && c==0) return 3;

    return 0;
}
