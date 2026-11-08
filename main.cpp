#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>

#if defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #undef min
    #undef max
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <unistd.h>
#else
    #include <unistd.h>
#endif

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

// Base virtual screen dimensions (original 2013 game resolution)
const int VIRTUAL_W = 760;
const int VIRTUAL_H = 757;

// Global screen and renderer pointers
static SDL_Window *g_window = NULL;
static SDL_Renderer *g_renderer = NULL;
static SDL_Texture *g_canvas = NULL;   // 760x757 persistent target texture
static bool g_fullscreen = false;

struct SDL_Point2D {
    int x;
    int y;
};

static std::string get_executable_dir()
{
#if defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
    {
        std::string path(buffer, len);
        size_t pos = path.find_last_of("\\/");
        if (pos != std::string::npos)
        {
            return path.substr(0, pos + 1);
        }
    }
#elif defined(__APPLE__)
    char buffer[1024];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0)
    {
        std::string path(buffer);
        size_t pos = path.find_last_of('/');
        if (pos != std::string::npos)
        {
            return path.substr(0, pos + 1);
        }
    }
#else
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';
        std::string path(buffer);
        size_t pos = path.find_last_of('/');
        if (pos != std::string::npos)
        {
            return path.substr(0, pos + 1);
        }
    }
#endif
    return "";
}

static std::string get_asset_path(const char *name)
{
    // 1. Check relative to current working directory: assets/<name>
    std::string candidate = std::string("assets/") + name;
    FILE *f = fopen(candidate.c_str(), "rb");
    if (f)
    {
        fclose(f);
        return candidate;
    }

    // 2. Check next to executable: <exe_dir>/assets/<name>
    std::string exe_dir = get_executable_dir();
    if (!exe_dir.empty())
    {
        candidate = exe_dir + "assets/" + name;
        f = fopen(candidate.c_str(), "rb");
        if (f)
        {
            fclose(f);
            return candidate;
        }
    }

    // 3. Check parent directory: ../assets/<name> (useful when launched from a build directory)
    candidate = std::string("../assets/") + name;
    f = fopen(candidate.c_str(), "rb");
    if (f)
    {
        fclose(f);
        return candidate;
    }

    return std::string("assets/") + name;
}

// Function prototypes
void present_screen();
void toggle_fullscreen();
void handle_window_events(SDL_Event *event);
SDL_Point2D get_logical_mouse_coords(SDL_Event *event);

void pause_screen();
void affrond(int pcase);
void affcroix(int pcase);
int coordonnees(SDL_Point2D posiclic);
int verifie(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int verifie2(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int vict(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int plein(int a, int b, int c, int d, int e, int f, int g, int h, int i);
void grille();
int choixniveau(SDL_Point2D posiclic);
void nice();

void draw_text_centered(TTF_Font *font, const char *str, SDL_Color color, int center_x, int top_y);
void draw_button_text(TTF_Font *font, const char *str, SDL_Color fg, SDL_Color bg, SDL_FRect btn_rect);
void draw_victory_banner(TTF_Font *font, int top_y);
void draw_defeat_banner(TTF_Font *font, int top_y);
void draw_draw_banner(TTF_Font *font);

void present_screen()
{
    if (!g_renderer || !g_canvas) return;

    // Switch render target to the main window
    SDL_SetRenderTarget(g_renderer, NULL);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // Draw the 760x757 canvas (automatically scaled and letterboxed by SDL 3 logical presentation)
    SDL_RenderTexture(g_renderer, g_canvas, NULL, NULL);
    SDL_RenderPresent(g_renderer);

    // Switch target back to g_canvas for subsequent game drawing
    SDL_SetRenderTarget(g_renderer, g_canvas);
}

void toggle_fullscreen()
{
    if (!g_window) return;
    g_fullscreen = !g_fullscreen;
    SDL_SetWindowFullscreen(g_window, g_fullscreen);
    present_screen();
}

void handle_window_events(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_F11 ||
            (event->key.key == SDLK_RETURN && (event->key.mod & SDL_KMOD_ALT)) ||
            event->key.key == SDLK_F)
        {
            toggle_fullscreen();
        }
    }
    else if (event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED ||
             event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        present_screen();
    }
}

SDL_Point2D get_logical_mouse_coords(SDL_Event *event)
{
    SDL_Point2D pt = { -1, -1 };
    if (!event || !g_renderer) return pt;

    // Convert screen coordinates to logical render coordinates
    SDL_ConvertEventToRenderCoordinates(g_renderer, event);
    if (event->button.x >= 0.0f && event->button.x <= (float)VIRTUAL_W &&
        event->button.y >= 0.0f && event->button.y <= (float)VIRTUAL_H)
    {
        pt.x = (int)event->button.x;
        pt.y = (int)event->button.y;
    }
    return pt;
}

void draw_text_centered(TTF_Font *font, const char *str, SDL_Color color, int center_x, int top_y)
{
    if (!font || !str || !g_renderer || !g_canvas) return;
    SDL_Surface *txt_surf = TTF_RenderText_Blended(font, str, 0, color);
    if (txt_surf)
    {
        SDL_Texture *txt_tex = SDL_CreateTextureFromSurface(g_renderer, txt_surf);
        if (txt_tex)
        {
            SDL_FRect r = { (float)(center_x - txt_surf->w / 2), (float)top_y, (float)txt_surf->w, (float)txt_surf->h };
            SDL_RenderTexture(g_renderer, txt_tex, NULL, &r);
            SDL_DestroyTexture(txt_tex);
        }
        SDL_DestroySurface(txt_surf);
    }
}

void draw_button_text(TTF_Font *font, const char *str, SDL_Color fg, SDL_Color bg, SDL_FRect btn_rect)
{
    if (!font || !str || !g_renderer || !g_canvas) return;
    SDL_Surface *txt_surf = TTF_RenderText_Shaded(font, str, 0, fg, bg);
    if (txt_surf)
    {
        SDL_Texture *txt_tex = SDL_CreateTextureFromSurface(g_renderer, txt_surf);
        if (txt_tex)
        {
            SDL_FRect r = {
                btn_rect.x + (btn_rect.w - (float)txt_surf->w) / 2.0f,
                btn_rect.y + (btn_rect.h - (float)txt_surf->h) / 2.0f,
                (float)txt_surf->w,
                (float)txt_surf->h
            };
            SDL_RenderTexture(g_renderer, txt_tex, NULL, &r);
            SDL_DestroyTexture(txt_tex);
        }
        SDL_DestroySurface(txt_surf);
    }
}

void draw_victory_banner(TTF_Font *font, int top_y)
{
    if (!font || !g_renderer || !g_canvas) return;
    const char *letters[] = {"V", "I", "C", "T", "O", "I", "R", "E"};
    SDL_Color colors[] = {
        {255, 0, 0, 255},     // Rouge
        {255, 81, 255, 255},  // Rose
        {255, 123, 0, 255},   // Orange
        {255, 233, 0, 255},   // Jaune
        {177, 255, 23, 255},  // Verte pomme
        {10, 206, 212, 255},  // Indigo
        {0, 0, 255, 255},     // Bleu marine
        {165, 32, 175, 255}   // Violette
    };
    int num_letters = 8;
    int letter_widths[8];
    int total_width = 0;
    int spacing = 10;

    for (int k = 0; k < num_letters; ++k)
    {
        int w = 0, h = 0;
        TTF_GetStringSize(font, letters[k], 0, &w, &h);
        letter_widths[k] = w;
        total_width += w;
        if (k > 0) total_width += spacing;
    }

    int start_x = (VIRTUAL_W - total_width) / 2;
    int curr_x = start_x;

    for (int k = 0; k < num_letters; ++k)
    {
        SDL_Surface *txt = TTF_RenderText_Blended(font, letters[k], 0, colors[k]);
        if (txt)
        {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, txt);
            if (tex)
            {
                SDL_FRect r = { (float)curr_x, (float)top_y, (float)txt->w, (float)txt->h };
                SDL_RenderTexture(g_renderer, tex, NULL, &r);
                SDL_DestroyTexture(tex);
            }
            SDL_DestroySurface(txt);
        }
        curr_x += letter_widths[k] + spacing;
    }
}

void draw_defeat_banner(TTF_Font *font, int top_y)
{
    if (!font || !g_renderer || !g_canvas) return;
    const char *letters[] = {"D", "E", "F", "A", "I", "T", "E"};
    SDL_Color couleurNoire = {0, 0, 0, 255};
    SDL_Color couleurbeige = {240, 213, 185, 255};
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
        TTF_GetStringSize(font, letters[k], 0, &w, &h);
        letter_widths[k] = w;
        total_width += w;
        if (k > 0) total_width += spacing;
    }

    int start_x = (VIRTUAL_W - total_width) / 2;
    int curr_x = start_x;

    for (int k = 0; k < num_letters; ++k)
    {
        SDL_Surface *txt = TTF_RenderText_Blended(font, letters[k], 0, colors[k]);
        if (txt)
        {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, txt);
            if (tex)
            {
                SDL_FRect r = { (float)curr_x, (float)top_y, (float)txt->w, (float)txt->h };
                SDL_RenderTexture(g_renderer, tex, NULL, &r);
                SDL_DestroyTexture(tex);
            }
            SDL_DestroySurface(txt);
        }
        curr_x += letter_widths[k] + spacing;
    }
}

void draw_draw_banner(TTF_Font *font)
{
    SDL_Color couleurviolette = {165, 32, 175, 255};
    draw_text_centered(font, "MATCH", couleurviolette, VIRTUAL_W / 2, 60);
    draw_text_centered(font, "NUL", couleurviolette, VIRTUAL_W / 2, 170);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (!TTF_Init())
    {
        fprintf(stderr, "Erreur TTF_Init : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    if (!SDL_CreateWindowAndRenderer("Morpion ! (F11: Plein écran)", VIRTUAL_W, VIRTUAL_H, SDL_WINDOW_RESIZABLE, &g_window, &g_renderer))
    {
        fprintf(stderr, "Impossible de créer la fenêtre et le renderer : %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Set logical presentation with aspect ratio preservation (letterbox)
    SDL_SetRenderLogicalPresentation(g_renderer, VIRTUAL_W, VIRTUAL_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Set window icon
    SDL_Surface *icon = SDL_LoadBMP(get_asset_path("sdl_icone.bmp").c_str());
    if (icon)
    {
        SDL_SetWindowIcon(g_window, icon);
        SDL_DestroySurface(icon);
    }

    // Create 760x757 persistent canvas texture
    g_canvas = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, VIRTUAL_W, VIRTUAL_H);
    if (!g_canvas)
    {
        fprintf(stderr, "Impossible de créer la texture canvas : %s\n", SDL_GetError());
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetRenderTarget(g_renderer, g_canvas);

    SDL_Color couleurNoire = {0, 0, 0, 255};
    SDL_Color couleurindigo = {10, 206, 212, 255};
    SDL_Color couleurverte = {128, 255, 23, 255};
    SDL_Color couleurneutre = {190, 150, 120, 255};
    SDL_Color couleurrouge = {255, 0, 0, 255};
    SDL_Color couleurblanche = {255, 255, 255, 255};

    TTF_Font *police = NULL;

    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, joue = 0, rejouer = 1;
    int lasse = 0;
    int booleen = 1;

    SDL_Point2D posiclic = {0, 0};

    grille();

    SDL_Event event;
    int continuer = 1, coord = 0, vvict = 0, pplein = 0, nombremys = 0, min = 1, max = 4, niveau = 0, lignes = 0;

    while(rejouer == 1 && lasse == 0)
    {
        nice();
        police = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 40.0f);

        // Menu title cleanly centered
        draw_text_centered(police, "Contre quel niveau d'I.A veux-tu jouer ?", couleurblanche, VIRTUAL_W / 2, 275);

        // Menu buttons with centered text inside their clickable boxes
        SDL_FRect btn_facile = {120.0f, 350.0f, 298.0f, 67.0f};
        draw_button_text(police, "  Vraiment Facile  ", couleurNoire, couleurverte, btn_facile);

        SDL_FRect btn_normal = {454.0f, 350.0f, 264.0f, 67.0f};
        draw_button_text(police, "   Normal   ", couleurNoire, couleurneutre, btn_normal);

        SDL_FRect btn_difficile = {120.0f, 451.0f, 298.0f, 67.0f};
        draw_button_text(police, "   Difficile   ", couleurNoire, couleurrouge, btn_difficile);

        SDL_FRect btn_impossible = {454.0f, 451.0f, 264.0f, 67.0f};
        draw_button_text(police, "  Impossible  ", couleurblanche, couleurNoire, btn_impossible);

        present_screen();

        if (police) { TTF_CloseFont(police); police = NULL; }

        continuer = 1;
        while(continuer)
        {
            if (!SDL_WaitEvent(&event)) break;
            handle_window_events(&event);

            switch(event.type)
            {
            case SDL_EVENT_QUIT:
                continuer = 0;
                lasse = 1;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    posiclic = get_logical_mouse_coords(&event);
                    niveau = choixniveau(posiclic);
                    if(niveau != 0)
                        continuer = 0;
                    else
                        continuer = 1;
                }
                break;
            default:
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
                    if (!SDL_WaitEvent(&event)) break;
                    handle_window_events(&event);

                    switch(event.type)
                    {
                    case SDL_EVENT_QUIT:
                        continuer = 0;
                        lasse = 1;
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            posiclic = get_logical_mouse_coords(&event);
                            continuer = 0;
                        }
                        break;
                    default:
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
                    if (!SDL_WaitEvent(&event)) break;
                    handle_window_events(&event);

                    switch(event.type)
                    {
                    case SDL_EVENT_QUIT:
                        continuer = 0;
                        lasse = 1;
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            posiclic = get_logical_mouse_coords(&event);
                            continuer = 0;
                        }
                        break;
                    default:
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
            police = TTF_OpenFont(get_asset_path("Graduate-Regular.ttf").c_str(), 95.0f);

            if(vvict == 1)
            {
                draw_victory_banner(police, 100);
                present_screen();
            }
            else if(vvict == 2)
            {
                draw_defeat_banner(police, 100);
                present_screen();
            }
            else if(pplein == 1)
            {
                draw_draw_banner(police);
                present_screen();
            }

            if (police) { TTF_CloseFont(police); police = NULL; }

            police = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 42.0f);

            // Replay question cleanly centered
            draw_text_centered(police, "Veux-tu rejouer contre l'I.A ?", couleurNoire, VIRTUAL_W / 2, 275);

            // OUI / NON buttons centered in their clickable zones
            SDL_FRect btn_oui = {199.0f, 350.0f, 86.0f, 70.0f};
            draw_button_text(police, " OUI ", couleurNoire, couleurindigo, btn_oui);

            SDL_FRect btn_non = {500.0f, 350.0f, 105.0f, 70.0f};
            draw_button_text(police, " NON ", couleurNoire, couleurindigo, btn_non);

            present_screen();

            if (police) { TTF_CloseFont(police); police = NULL; }

            continuer = 1;
            while(continuer)
            {
                if (!SDL_WaitEvent(&event)) break;
                handle_window_events(&event);

                switch(event.type)
                {
                case SDL_EVENT_QUIT:
                    continuer = 0;
                    lasse = 1;
                    rejouer = 0;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        posiclic = get_logical_mouse_coords(&event);
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
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Affichage des crédits de fin
    pause_screen();

    // Clean resource deallocation
    if (g_canvas)
    {
        SDL_DestroyTexture(g_canvas);
        g_canvas = NULL;
    }
    if (g_renderer)
    {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = NULL;
    }
    if (g_window)
    {
        SDL_DestroyWindow(g_window);
        g_window = NULL;
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}

void pause_screen()
{
    int continuer = 1;
    SDL_Event event;

    grille();

    TTF_Font *p1 = TTF_OpenFont(get_asset_path("PatrickHand-Regular.ttf").c_str(), 44.0f);
    if (p1)
    {
        SDL_Color couleurNoire = {0, 0, 0, 255};
        draw_text_centered(p1, "Ce programme a été créé par :", couleurNoire, VIRTUAL_W / 2, 220);
        TTF_CloseFont(p1);
    }

    TTF_Font *p2 = TTF_OpenFont(get_asset_path("Graduate-Regular.ttf").c_str(), 60.0f);
    if (p2)
    {
        SDL_Color couleurNoire = {0, 0, 0, 255};
        draw_text_centered(p2, "FromTheRags", couleurNoire, VIRTUAL_W / 2, 310);
        TTF_CloseFont(p2);
    }

    present_screen();

    // Flush any pending clicks from previous screen so credits stay visible
    SDL_Delay(250);
    while (SDL_PollEvent(&event)) {}

    while (continuer)
    {
        if (!SDL_WaitEvent(&event)) break;
        handle_window_events(&event);
        switch(event.type)
        {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            continuer = 0;
            break;
        default:
            break;
        }
    }
}

void affrond(int pcase)
{
    if (pcase <= 0 || !g_renderer || !g_canvas) return;
    SDL_Surface *rond = SDL_LoadBMP(get_asset_path("rond.bmp").c_str());
    if (!rond) return;

    SDL_SetSurfaceColorKey(rond, true, SDL_MapSurfaceRGB(rond, 255, 255, 255));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, rond);
    if (tex)
    {
        SDL_FRect posirond = {0.0f, 0.0f, (float)rond->w, (float)rond->h};
        if(pcase == 9) { posirond.x = 498.0f; posirond.y = 48.0f; }
        if(pcase == 8) { posirond.x = 269.0f; posirond.y = 48.0f; }
        if(pcase == 7) { posirond.x = 40.0f;  posirond.y = 48.0f; }
        if(pcase == 6) { posirond.x = 498.0f; posirond.y = 275.0f; }
        if(pcase == 5) { posirond.x = 269.0f; posirond.y = 275.0f; }
        if(pcase == 4) { posirond.x = 40.0f;  posirond.y = 275.0f; }
        if(pcase == 3) { posirond.x = 498.0f; posirond.y = 502.0f; }
        if(pcase == 2) { posirond.x = 269.0f; posirond.y = 502.0f; }
        if(pcase == 1) { posirond.x = 40.0f;  posirond.y = 502.0f; }

        SDL_RenderTexture(g_renderer, tex, NULL, &posirond);
        SDL_DestroyTexture(tex);
    }
    SDL_DestroySurface(rond);
    present_screen();
}

void affcroix(int pcase)
{
    if (pcase <= 0 || !g_renderer || !g_canvas) return;
    SDL_Surface *croix = SDL_LoadBMP(get_asset_path("croix.bmp").c_str());
    if (!croix) return;

    SDL_SetSurfaceColorKey(croix, true, SDL_MapSurfaceRGB(croix, 255, 255, 255));
    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, croix);
    if (tex)
    {
        SDL_FRect posicroix = {0.0f, 0.0f, (float)croix->w, (float)croix->h};
        if(pcase == 9) { posicroix.x = 498.0f; posicroix.y = 48.0f; }
        if(pcase == 8) { posicroix.x = 269.0f; posicroix.y = 48.0f; }
        if(pcase == 7) { posicroix.x = 40.0f;  posicroix.y = 48.0f; }
        if(pcase == 6) { posicroix.x = 498.0f; posicroix.y = 275.0f; }
        if(pcase == 5) { posicroix.x = 269.0f; posicroix.y = 275.0f; }
        if(pcase == 4) { posicroix.x = 40.0f;  posicroix.y = 275.0f; }
        if(pcase == 3) { posicroix.x = 498.0f; posicroix.y = 502.0f; }
        if(pcase == 2) { posicroix.x = 269.0f; posicroix.y = 502.0f; }
        if(pcase == 1) { posicroix.x = 40.0f;  posicroix.y = 502.0f; }

        SDL_RenderTexture(g_renderer, tex, NULL, &posicroix);
        SDL_DestroyTexture(tex);
    }
    SDL_DestroySurface(croix);
    present_screen();
}

int coordonnees(SDL_Point2D posiclic)
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
    if (!g_renderer || !g_canvas) return;
    SDL_Surface *imagedefond = SDL_LoadBMP(get_asset_path("grille.bmp").c_str());
    if (imagedefond)
    {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, imagedefond);
        if (tex)
        {
            SDL_RenderTexture(g_renderer, tex, NULL, NULL);
            SDL_DestroyTexture(tex);
        }
        SDL_DestroySurface(imagedefond);
    }
    present_screen();
}

int choixniveau(SDL_Point2D posiclic)
{
    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 350 && posiclic.y <= 417) return 1;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 350 && posiclic.y <= 417) return 2;
    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 451 && posiclic.y <= 518) return 3;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 451 && posiclic.y <= 518) return 4;

    return 0;
}

void nice()
{
    if (!g_renderer || !g_canvas) return;
    SDL_Surface *imagedefond = SDL_LoadBMP(get_asset_path("nice.bmp").c_str());
    if (imagedefond)
    {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, imagedefond);
        if (tex)
        {
            SDL_RenderTexture(g_renderer, tex, NULL, NULL);
            SDL_DestroyTexture(tex);
        }
        SDL_DestroySurface(imagedefond);
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
