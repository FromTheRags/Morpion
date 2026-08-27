#include <cstdlib>
#include <cstdio>
#include <ctime>

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


void pause(SDL_Surface *ecran);
void affrond(SDL_Surface *rond, SDL_Rect posirond,SDL_Surface *ecran, int pcase);
void affcroix(SDL_Surface *croix, SDL_Rect posicroix,SDL_Surface *ecran, int pcase);
int coordonnees(SDL_Rect posiclic);
int verifie(int a, int b, int c, int d, int e, int f, int g, int h, int i );
int verifie2(int a, int b, int c, int d, int e, int f, int g, int h, int i );
int vict(int a, int b, int c, int d, int e, int f, int g, int h, int i );
int plein(int a, int b, int c, int d, int e, int f, int g, int h, int i );
void grille(SDL_Surface *ecran);
int choixniveau(SDL_Rect posiclic);
void nice(SDL_Surface *ecran);


int main ( int argc, char* argv[] )
{
    TTF_Init();//démarrage de la bibliothèque texte
    if (SDL_Init(SDL_INIT_VIDEO) == -1) // Démarrage de la SDL. Si
//erreur alors...
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n",SDL_GetError()); // Ecriture de l'erreur

        exit(EXIT_FAILURE); // On quitte le programme
    }

    SDL_WM_SetCaption("Morpion !", NULL);//sert à renommer la fenêtre,second parametre icone
      srand(time(NULL));

    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Color couleurindigo = {10, 206, 212};
    SDL_Color couleurverte = {128,255,23};
    SDL_Color couleurneutre = {190,150,120};
    SDL_Color couleurrouge = {255,0,0};
    SDL_Color couleurblanche = {255,255,255};
    SDL_Color couleurviolette = {165,32,175};
    SDL_Color couleurorange = {255,123,0};
    SDL_Color couleurrose = {255,81,255};
    SDL_Color couleurjaune = {255,233,0};
    SDL_Color couleurvertepomme = {177,255,23};
    SDL_Color couleurbleumarine = {0,0,255};
    SDL_Color couleurbeige = {240,213,185};
    //SDL_Color couleurbeige = {250,183,147};
    SDL_Surface *ecran = NULL;
    SDL_Surface *imagedefond = NULL;
    SDL_Surface *rond = NULL;
    SDL_Surface *croix = NULL;
    SDL_Surface *clic = NULL;
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;

    int a = 0,b = 0, c = 0,d = 0,e = 0,f = 0,g = 0,h = 0,i = 0,joue = 0,rejouer = 1;
    int lasse = 0;
    int booleen = 1;

    SDL_Rect image,posirond,posicroix,posiclic, positexte;
      image.x = 0;
      image.y = 0;
      posirond.x = 40;//en haut à gauche
      posirond.y = 48;
      posicroix.x = 40;
      posicroix.y = 48;
      posiclic.x = 0;
      posiclic.y = 0;
      positexte.x = 80;
      positexte.y = 275;
    SDL_WM_SetIcon(SDL_LoadBMP("sdl_icone.bmp"), NULL);
    police = TTF_OpenFont("PatrickHand-Regular.ttf", 62);
    ecran = SDL_SetVideoMode(760, 757, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);//on tente d'ouvrir une fenêtre

       SDL_Event event;

    if (ecran == NULL) // Si l'ouverture a échoué, on écrit l'erreur
        //et on arrête
    {
        fprintf(stderr, "Impossible de charger le mode vidéo :%s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Chargement d'une image Bitmap dans une surface */
      imagedefond = SDL_LoadBMP("grille.bmp");
     /* On blitte par-dessus l'écran */
      SDL_BlitSurface(imagedefond , NULL, ecran, &image );
      SDL_Flip(ecran); /* Mise à jour de l'écran */


       int continuer = 1,coord = 0,vvict= 0,pplein =0,nombremys = 0, min = 1, max = 4,niveau = 0,lignes = 0;
while(rejouer == 1 && lasse == 0)
{

    nice(ecran);
    positexte.x = 10;
    positexte.y = 275;
    texte = TTF_RenderUTF8_Blended(police, "Contre quel niveau d'I.A veux-tu jouer?",couleurblanche);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =120;
    positexte.y = 350;
    texte = TTF_RenderUTF8_Shaded(police, "Vraiment Facile",couleurNoire,couleurverte);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =450;
    positexte.y = 350;
    texte = TTF_RenderUTF8_Shaded(police, "   Normal    ",couleurNoire,couleurneutre);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =120;
    positexte.y = 450;
    texte = TTF_RenderUTF8_Shaded(police, "   Difficile    ",couleurNoire,couleurrouge);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =450;
    positexte.y = 450;
    texte = TTF_RenderUTF8_Shaded(police, "  Impossible ",couleurblanche,couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    SDL_Flip(ecran);




       continuer = 1;
    while(continuer)
      {
          SDL_WaitEvent(&event);
      switch(event.type)
      {
      case SDL_QUIT:
      continuer = 0;
      lasse = 1;
       break;
       case SDL_MOUSEBUTTONUP: /* Clic de la souris */
       posiclic.x = event.button.x; /* change les coordonnées de posiclic*/
       posiclic.y = event.button.y;
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
 grille(ecran);
       do
{
    continuer = 1;
    while(continuer)
      {
          SDL_WaitEvent(&event);
      switch(event.type)
      {
      case SDL_QUIT:
      continuer = 0;
      lasse = 1;
       break;
       case SDL_MOUSEBUTTONUP: /* Clic de la souris */
       posiclic.x = event.button.x; /* change les coordonnées de posiclic*/
       posiclic.y = event.button.y;
       continuer = 0;

       break;
      }
      }

      coord = 0;
      coord = coordonnees(posiclic);


      if(coord ==9 )
      {i = 1;lignes = 0;}
      else if(coord == 8)
      {h= 1;lignes = 0;}
      else if(coord == 7 )
      {g= 1;lignes = 0;}
      else if(coord == 6)
      {f= 1;lignes =0;}
      else if(coord == 5)
      {e= 1;lignes = 0;}
      else if(coord == 4)
      {d= 1;lignes = 0;}
      else if(coord == 3)
      {c= 1;lignes = 0;}
      else if(coord == 2)
      {b= 1;lignes = 0;}
      else if(coord == 1)
      {a= 1;
      lignes = 0;}
      else if(coord == 0)
      lignes = 1;

}while(lignes == 1 && lasse == 0);
      if(lasse == 0)
      affrond(rond, posirond, ecran, coord);
}
      if(e == 0 && lasse == 0 && niveau > 2)
      {
          affcroix(croix, posicroix, ecran, 5);
          e = 2;
      }
      else if(lasse == 0 && e == 1 && niveau > 2)
     {
         min = 1;
         max = 4;
//ces deux lignes servent a faire choisir a l'ordinateur 1 nombre aléatoire(avec srand time au début)
      nombremys = (rand() % (max - min + 1)) + min;

      if(nombremys == 1)
      {affcroix(croix, posicroix, ecran, 1);
      a=2;
      }
      if(nombremys == 2)
      {affcroix(croix, posicroix, ecran, 3);
      c = 2;
      }
      if(nombremys == 3)
      {affcroix(croix, posicroix, ecran, 7);
      g = 2;
      }
      if(nombremys == 4)
      {affcroix(croix, posicroix, ecran, 9);
      i = 2;
      }
     }
     if(lasse == 0 && niveau == 2)//niveau normal
     {
         min = 1;
         max = 9;
         while(a<2 && b<2 && c<2 && d<2 && e<2 && f<2 && g<2 && h<2 && i<2)
{
       nombremys = (rand() % (max - min + 1)) + min;

         if(nombremys == 1 && a==0)
      {affcroix(croix, posicroix, ecran, 1);
      a=2;
      }
      if(nombremys == 2 && b==0)
      {affcroix(croix, posicroix, ecran, 2);
      b = 2;
      }
      if(nombremys == 3 && c==0)
      {affcroix(croix, posicroix, ecran, 3);
      c = 2;
      }
      if(nombremys == 4 && d==0)
      {affcroix(croix, posicroix, ecran, 4);
      d = 2;
      }
      if(nombremys == 5 && e==0)
      {affcroix(croix, posicroix, ecran, 5);
      e = 2;
      }
      if(nombremys == 6 && f==0)
      {affcroix(croix, posicroix, ecran, 6);
      f = 2;
      }
      if(nombremys == 7 && g==0)
      {affcroix(croix, posicroix, ecran, 7);
      g = 2;
      }
      if(nombremys == 8 && h==0)
      {affcroix(croix, posicroix, ecran, 8);
      h = 2;
      }
      if(nombremys == 9 && i==0)
      {affcroix(croix, posicroix, ecran, 9);
      i = 2;
      }

     }
     }
     if(lasse == 0 && niveau == 1)//niveau vraiment facile
     {
         while(a<2 && b<2 && c<2 && d<2 && e<2 && f<2 && g<2 && h<2 && i<2)
         {
            min = 1;
            max = 4;
            nombremys = (rand() % (max - min + 1)) + min;

      if(nombremys == 1 && b == 0)//application d'un correctif pour empêcher l'I.A
      {affcroix(croix, posicroix, ecran, 2);//de jouer sur l'emplacement choisi par le joueur
      b=2;
      }
      if(nombremys == 2 && d == 0)
      {affcroix(croix, posicroix, ecran, 4);
      d = 2;
      }
      if(nombremys == 3 && f == 0)
      {affcroix(croix, posicroix, ecran, 6);
      f = 2;
      }
      if(nombremys == 4 && h ==0)
      {affcroix(croix, posicroix, ecran, 8);
      h = 2;
      }

         }
     }


//début du while
while(vvict == 0 && pplein == 0 && lasse ==0)
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
      switch(event.type)
      {
      case SDL_QUIT:
      continuer = 0;
      lasse = 1;
       break;
       case SDL_MOUSEBUTTONUP: /* Clic de la souris */
       posiclic.x = event.button.x; /* change les coordonnées de posiclic*/
       posiclic.y = event.button.y;
       continuer = 0;

       break;
      }
      }

if(lasse == 0 && vvict == 0 && pplein == 0)
 {

      coord = coordonnees(posiclic);



      if(coord ==9 && i == 0)
      {i = 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 8 && h == 0)
      {h= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 7 && g == 0)
      {g= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 6 && f == 0)
      {f= 1;lignes =0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 5 && e==0)
      {e= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 4 && d == 0)
      {d= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 3 && c==0)
      {c= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 2 && b ==0)
      {b= 1;lignes = 0;affrond(rond, posirond, ecran, coord);}
      else if(coord == 1 && a==0)
      {a= 1;
      affrond(rond, posirond, ecran, coord);
      lignes = 0;}
      else
      lignes = 1;


      vvict = vict(a,b,c,d,e,f,g,h,i);
      pplein = plein(a,b,c,d,e,f,g,h,i);
}

}while(lignes == 1 && lasse == 0);
if(lasse == 0 && vvict == 0 && pplein == 0)
{
if(niveau == 1)
   {
       joue = verifie2(a,b,c,d,e,f,g,h,i);
   }
   else
   {
         joue = verifie(a,b,c,d,e,f,g,h,i);
   }

    affcroix(croix, posicroix, ecran, joue);
    if(joue == 9)
      i = 2;
      else if(joue == 8)
      h= 2;
      else if(joue == 7)
      g= 2;
      else if(joue == 6)
      f= 2;
      else if(joue == 5)
      e= 2;
      else if(joue == 4)
      d= 2;
      else if(joue == 3)
      c= 2;
      else if(joue == 2)
      b= 2;
      else if(joue == 1)
      a= 2;

}
if(joue == 0 && vvict == 0 && pplein == 0)
{
    //si personne peut gagner: stratégie du diago diago contrer par 8 et 5+le faire jouer quelque part
if(niveau == 4)
{
    if(a==1 && i==1 && h==0)
    {
    affcroix(croix, posicroix, ecran, 8);
    h=2;
    booleen = 0;
    }
    else if(g==1 && c==1 && h==0)
    {
        affcroix(croix, posicroix, ecran, 8);
    h=2;
    booleen = 0;
    }
    else if(a==1 && h==1 && g==0)
    {
        affcroix(croix, posicroix, ecran, 7);
    g=2;
    booleen = 0;
    }
    else if(a==1 && f==1 && c==0)
    {
        affcroix(croix, posicroix, ecran, 3);
    c=2;
    booleen = 0;
    }
    else if(g==1 && b==1 && a==0)
    {
        affcroix(croix, posicroix, ecran, 1);
    a=2;
    booleen = 0;
    }
    else if(g==1 && f==1 && i==0)
    {
        affcroix(croix, posicroix, ecran, 9);
    i=2;
    booleen = 0;
    }
    else if(c==1 && d==1 && a==0)
    {
        affcroix(croix, posicroix, ecran, 1);
    a=2;
    booleen = 0;
    }
    else if(c==1 && h==1 && i==0)
    {
        affcroix(croix, posicroix, ecran, 9);
    i=2;
    booleen = 0;
    }
    else if(i==1 && b==1 && c==0)
    {
        affcroix(croix, posicroix, ecran, 3);
    c=2;
    booleen = 0;
    }
    else if(i==1 && d==1 && g==0)
    {
        affcroix(croix, posicroix, ecran, 7);
    g=2;
    booleen = 0;
    }
    else if(h == 1 && f == 1 && i==0)
    {
        affcroix(croix, posicroix, ecran, 9);
        i = 2;
        booleen = 0;
    }
    else if(h == 1 && d==1 && g == 0)
    {
        affcroix(croix, posicroix, ecran, 7);
        g = 2;
        booleen = 0;
    }
    else if(b == 1 && d == 1 && a == 0)
    {
        affcroix(croix, posicroix, ecran, 1);
        a = 2;
        booleen = 0;
    }
    else if(b == 1 && f == 1 && c == 0)
    {
        affcroix(croix, posicroix, ecran, 3);
        c = 2;
        booleen = 0;
    }
}
if(booleen)
{
    if(a==0)//rupture de la stratégie :on joue pù on peut de la façon la +stratégique à la - stratégique
    {
        affcroix(croix, posicroix, ecran, 1);
    a=2;
    }
    else if(c==0)
    {
        affcroix(croix, posicroix, ecran, 3);
    c=2;
    }
    else if(g==0)
    {
        affcroix(croix, posicroix, ecran, 7);
    g=2;
    }
    else if(i==0)
    {
        affcroix(croix, posicroix, ecran, 9);
    i=2;
    }
    else if(b==0)
    {
        affcroix(croix, posicroix, ecran, 2);
    b=2;
    }
    else if(f==0)
    {
        affcroix(croix, posicroix, ecran, 6);
    f=2;
    }
    else if(d==0)
    {
        affcroix(croix, posicroix, ecran, 4);
    d=2;
    }
    else if(h==0)
    {
        affcroix(croix, posicroix, ecran, 8);
    h=2;
    }
}
}
vvict = vict(a,b,c,d,e,f,g,h,i);
pplein = plein(a,b,c,d,e,f,g,h,i);
}
//fin du while
if(lasse == 0)
  {
    TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    police = TTF_OpenFont("Graduate-Regular.ttf", 200);

    if(vvict == 1)
    {

        positexte.x = 0;
    positexte.y = 80;
    texte = TTF_RenderUTF8_Blended(police, "V",couleurrouge);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 140;
    texte = TTF_RenderUTF8_Blended(police, "I",couleurrose);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 190;
    texte = TTF_RenderUTF8_Blended(police, "C",couleurorange);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 290;
    texte = TTF_RenderUTF8_Blended(police, "T",couleurjaune);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 390;
    texte = TTF_RenderUTF8_Blended(police, "O",couleurvertepomme);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 490;
    texte = TTF_RenderUTF8_Blended(police, "I",couleurindigo);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 540;
    texte = TTF_RenderUTF8_Blended(police, "R",couleurbleumarine);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 650;
    texte = TTF_RenderUTF8_Blended(police, "E",couleurviolette);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    SDL_Flip(ecran);
    }
    else if(vvict == 2)
    {

        positexte.x = 10;
    positexte.y = 80;
    texte = TTF_RenderUTF8_Blended(police, "D",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 120;
    texte = TTF_RenderUTF8_Blended(police, "E",couleurbeige);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 230;
    texte = TTF_RenderUTF8_Blended(police, "F",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 320;
    texte = TTF_RenderUTF8_Blended(police, "A",couleurbeige);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 450;
    texte = TTF_RenderUTF8_Blended(police, "I",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 510;
    texte = TTF_RenderUTF8_Blended(police, "T",couleurbeige);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 635;
    texte = TTF_RenderUTF8_Blended(police, "E",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    SDL_Flip(ecran);
    }
    else if(pplein == 1)
    {

        positexte.x = 80;
    positexte.y = 80;
    texte = TTF_RenderUTF8_Blended(police, "MATCH",couleurviolette);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.y = 500;
    positexte.x = 180;
    texte = TTF_RenderUTF8_Blended(police, "NUL",couleurviolette);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    SDL_Flip(ecran);
    }
    TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    police = TTF_OpenFont("PatrickHand-Regular.ttf", 62);
    positexte.x = 80;
    positexte.y = 275;
    texte = TTF_RenderUTF8_Blended(police, "Veux-tu rejouer contre L'I.A?",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =200;
    positexte.y = 350;
    texte = TTF_RenderUTF8_Shaded(police, "OUI",couleurNoire,couleurindigo);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */

    positexte.x =500;
    positexte.y = 350;
    texte = TTF_RenderUTF8_Shaded(police, "NON",couleurNoire,couleurindigo);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */


    SDL_Flip(ecran);

    continuer = 1;
      while(continuer)
      {
          SDL_WaitEvent(&event);
      switch(event.type)
      {
      case SDL_QUIT:
      continuer = 0;
      lasse = 1;
       break;
       case SDL_MOUSEBUTTONUP: //0 Clic de la souris
       posiclic.x = event.button.x; // change les coordonnées de posiclic
       posiclic.y = event.button.y;
       if (posiclic.x >= 199 && posiclic.x <= 285 && posiclic.y >= 350 && posiclic.y <= 420) // grâce aux  coordonnées de posiclic
       {
           rejouer = 1;
           vvict = 0;
           pplein = 0;
           a = 0;b = 0; c = 0;d = 0;e = 0;f = 0;g = 0;h = 0;i = 0;
           continuer = 0;
       }
       else if(posiclic.x >= 500 && posiclic.x <= 605 && posiclic.y >= 350 && posiclic.y <=420)
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
    pause(ecran);
     int z =0;
    SDL_FreeSurface(imagedefond);//on libère la surface
    while(z <10)
    {SDL_FreeSurface(rond);
    SDL_FreeSurface(croix);
    z++;
    }
    SDL_FreeSurface(clic);
    TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    TTF_Quit();//quitte la biblio texte
    SDL_Quit();//quitte la SDL

    return EXIT_SUCCESS;
}

void pause(SDL_Surface *ecran)
{
    int continuer = 1;
    SDL_Event event;
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Rect positexte;
    positexte.x = 80;
    positexte.y = 200;

    SDL_Color couleurNoire = {0, 0, 0};
    police = TTF_OpenFont("PatrickHand-Regular.ttf", 62);
    grille(ecran);//recharger la grille impératif!!!!!!!

    texte = TTF_RenderUTF8_Blended(police, "Ce programme a été créé par :",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    positexte.x = 225;
    positexte.y = 300;
    texte = TTF_RenderUTF8_Blended(police, "FromTheRags",couleurNoire);
    SDL_BlitSurface(texte, NULL, ecran, &positexte); /* Blit du texte par-dessus */
    SDL_Flip(ecran);
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
        }
    }
}
void affrond(SDL_Surface *rond, SDL_Rect posirond,SDL_Surface *ecran, int pcase)
{
    rond = SDL_LoadBMP("rond.bmp");
    if(pcase == 9)
    {posirond.x = 498;
    posirond.y = 48;
    }
    if(pcase == 8)
    {posirond.x = 269;
    posirond.y = 48;
    }
    if(pcase == 7)
    {posirond.x = 40;
    posirond.y = 48;
    }
    if(pcase ==6)
    {posirond.x = 498;
    posirond.y = 275;
    }
    if(pcase == 5)
    {posirond.x = 269;
    posirond.y = 275;
    }
    if(pcase == 4)
    {posirond.x = 40;
    posirond.y = 275;
    }
    if(pcase == 3)
    {posirond.x = 498;
    posirond.y = 502;
    }
    if(pcase == 2)
    {posirond.x = 269;
    posirond.y = 502;
    }
    if(pcase == 1)
    {posirond.x = 40;
    posirond.y = 502;
    }

    if(pcase>0 && rond != NULL)
    {
     SDL_SetColorKey(rond, SDL_SRCCOLORKEY, SDL_MapRGB(rond->format, 255, 255, 255));
     SDL_BlitSurface(rond, NULL, ecran, &posirond);
     SDL_Flip(ecran);
    }

}
void affcroix(SDL_Surface *croix, SDL_Rect posicroix,SDL_Surface *ecran, int pcase)
{

    croix = SDL_LoadBMP("croix.bmp");
    if(pcase == 9)
    {posicroix.x = 498;
    posicroix.y = 48;
    }
    if(pcase == 8)
    {posicroix.x = 269;
    posicroix.y = 48;
    }
    if(pcase == 7)
    {posicroix.x = 40;
    posicroix.y = 48;
    }
    if(pcase ==6)
    {posicroix.x = 498;
    posicroix.y = 275;
    }
    if(pcase == 5)
    {posicroix.x = 269;
    posicroix.y = 275;
    }
    if(pcase == 4)
    {posicroix.x = 40;
    posicroix.y = 275;
    }
    if(pcase == 3)
    {posicroix.x = 498;
    posicroix.y = 502;
    }
    if(pcase == 2)
    {posicroix.x = 269;
    posicroix.y = 502;
    }
    if(pcase == 1)
    {posicroix.x = 40;
    posicroix.y = 502;
    }

    if(pcase>0 && croix != NULL)
    {
        //SDL_Delay(1000);sert à arrêter le programme(temps en ms)
     SDL_SetColorKey(croix, SDL_SRCCOLORKEY, SDL_MapRGB(croix->format, 255, 255, 255));
     SDL_BlitSurface(croix, NULL, ecran, &posicroix);
     SDL_Flip(ecran);
    }


}
int coordonnees(SDL_Rect posiclic)
{
    if(posiclic.x >= 498 && posiclic.x <=710 && posiclic.y >=48 && posiclic.y <= 260)
    return 9;
    if(posiclic.x >= 269 && posiclic.x <=481 && posiclic.y >=48 && posiclic.y <= 260)
    return 8;
    if(posiclic.x >= 40 && posiclic.x <=252 && posiclic.y >=48 && posiclic.y <= 260)
    return 7;
    if(posiclic.x >= 498 && posiclic.x <=710 && posiclic.y >=275 && posiclic.y <= 487)
    return 6;
    if(posiclic.x >= 269 && posiclic.x <=481 && posiclic.y >=275 && posiclic.y <= 487)
    return 5;
    if(posiclic.x >= 40 && posiclic.x <=252 && posiclic.y >=275 && posiclic.y <= 487)
    return 4;
    if(posiclic.x >= 498 && posiclic.x <=710 && posiclic.y >=502 && posiclic.y <= 714)
    return 3;
    if(posiclic.x >= 269 && posiclic.x <=481 && posiclic.y >=502 && posiclic.y <= 714)
    return 2;
    if(posiclic.x >= 40 && posiclic.x <=252 && posiclic.y >=502 && posiclic.y <= 714)
    {
        return 1;
    }

        return 0;




}

int verifie(int a, int b, int c, int d, int e, int f, int g, int h, int i )
{

        if(a==2 && d == 2 && g ==0)
        return 7;
        else if(b==2 && e==2 && h== 0)
        return 8;
        else if(c==2 && f==2 && i==0)
        return 9;
        else if(d==2 && g==2 && a==0)
        return 1;
        else if(e==2 && h== 2 && b==0)
        return 2;
        else if(f==2 && i ==2 && c==0)
        return 3;
         else if(a == 2 && b==2 && c == 0)
        return 3;
         else if(d==2 && e==2 && f==0)
        return 6;
         else if(g == 2 && h==2 && i==0)
        return 9;
         else if(b==2 && c==2 && a==0)
        return 1;
         else if(e == 2 && f == 2 && d==0)
        return 4;
         else if(i == 2 && h==2 && g==0)
        return 7;
         else if(a==2 && e==2 && i==0)
        return 9;
         else if(e==2 && i==2 && a==0)
        return 1;
         else if(c==2 && e==2 && g==0)
        return 7;
         else if(g==2 && e==2 && c==0)
        return 3;
         else if(a==2 && i==2 && e==0)
        return 5;
         else if(g==2 && c==2 && e==0)
        return 5;
         else if(a==2 && g==2 && d==0)
        return 4;
         else if(h==2 && b==2 && e==0)
        return 5;
         else if(c==2 && i==2 && f==0)
        return 6;
         else if(a==2 && c==2 && b==0)
        return 2;
         else if(d==2 && f==2 && e==0)
        return 5;
         else if(g==2 && i==2 && h==0)
        {
            return 8;
        }


         else if(a==1 && d == 1 && g ==0)
        return 7;
         else if(b==1 && e==1 && h== 0)
        return 8;
         else if(c==1 && f==1 && i==0)
        return 9;
         else if(d==1 && g==1 && a==0)
        return 1;
         else if(e==1 && h== 1 && b==0)
        return 2;
         else if(f==1 && i ==1 && c==0)
        return 3;
         else if(a == 1 && b==1 && c == 0)
        return 3;
         else if(d==1 && e==1 && f==0)
        return 6;
         else if(g == 1 && h==1 && i==0)
        return 9;
         else if(b==1 && c==1 && a==0)
        return 1;
         else if(e==1 && f==1 && d==0)
        return 4;
         else if(i == 1 && h==1 && g==0)
        return 7;
         else if(a==1 && e==1 && i==0)
        return 9;
         else if(e==1 && i==1 && a==0)
        return 1;
         else if(c==1 && e==1 && g==0)
        return 7;
         else if(g==1 && e==1 && c==0)
        return 3;
         else if(a==1 && i==1 && e==0)
        return 5;
         else if(g==1 && c==1 && e==0)
        return 5;
         else if(a==1 && g==1 && d==0)
        return 4;
         else if(h==1 && b==1 && e==0)
        return 5;
         else if(c==1 && i==1 && f==0)
        return 6;
         else if(a==1 && c==1 && b==0)
        return 2;
         else if(d==1 && f==1 && e==0)
        return 5;
         else if(g==1 && i==1 && h==0)
        return 8;


    return 0;


}
int vict(int a, int b, int c, int d, int e, int f, int g, int h, int i )
{
    if(a==1 && b==1 && c==1)
    return 1;
    if(d==1 && e==1 && f==1)
    return 1;
    if(g==1 && h==1 && i==1)
    return 1;
    if(a==1 && d==1 && g==1)
    return 1;
    if(b==1 && e==1 && h==1)
    return 1;
    if(c==1 && f==1 && i==1)
    return 1;
    if(a==1 && e==1 && i==1)
    return 1;
    if(c == 1 && e==1 && g==1)
    return 1;

    if(a==2 && b==2 && c==2)
    return 2;
    if(d==2 && e==2 && f==2)
    return 2;
    if(g==2 && h==2 && i==2)
    return 2;
    if(a==2 && d==2 && g==2)
    return 2;
    if(b==2 && e==2 && h==2)
    return 2;
    if(c==2 && f==2 && i==2)
    return 2;
    if(a==2 && e==2 && i==2)
    return 2;
    if(c == 2 && e==2 && g==2)
    return 2;


    return 0;

}

int plein(int a, int b, int c, int d, int e, int f, int g, int h, int i )
{
    if(a!= 0 && b!=0 && c!=0 && d!=0 && e!=0 && f!=0 && g!=0 && h!=0 && i!=0)
    {
        return 1;
    }
    return 0;
}

void grille(SDL_Surface *ecran)
{


        SDL_Surface *imagedefond = NULL;
        SDL_Rect image;
        image.x = 0;
        image.y = 0;

     /* Chargement d'une image Bitmap dans une surface */
     imagedefond = SDL_LoadBMP("grille.bmp");
     /* On blitte par-dessus l'écran */
      SDL_BlitSurface(imagedefond , NULL, ecran, &image );
      SDL_Flip(ecran); /* Mise à jour de l'écran */

}

int choixniveau(SDL_Rect posiclic)
{

    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 350 && posiclic.y <= 417)
    return 1;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 350 && posiclic.y <= 417)
    return 2;
    if(posiclic.x >= 120 && posiclic.x <= 418 && posiclic.y >= 451 && posiclic.y <=518 )
    return 3;
    if(posiclic.x >= 454 && posiclic.x <= 718 && posiclic.y >= 451 && posiclic.y <= 518)
    return 4;

    return 0;


}
void nice(SDL_Surface *ecran)
{
    SDL_Surface *imagedefond = NULL;
        SDL_Rect image;
        image.x = 0;
        image.y = 0;

     /* Chargement d'une image Bitmap dans une surface */
     imagedefond = SDL_LoadBMP("nice.bmp");
     /* On blitte par-dessus l'écran */
      SDL_BlitSurface(imagedefond , NULL, ecran, &image );
      SDL_Flip(ecran); /* Mise à jour de l'écran */
}
int verifie2(int a, int b, int c, int d, int e, int f, int g, int h, int i )
{
    if(a==2 && d == 2 && g ==0)
        return 7;
        else if(b==2 && e==2 && h== 0)
        return 8;
        else if(c==2 && f==2 && i==0)
        return 9;
        else if(d==2 && g==2 && a==0)
        return 1;
        else if(e==2 && h== 2 && b==0)
        return 2;
        else if(f==2 && i ==2 && c==0)
        return 3;
         else if(a == 2 && b==2 && c == 0)
        return 3;
         else if(d==2 && e==2 && f==0)
        return 6;
         else if(g == 2 && h==2 && i==0)
        return 9;
         else if(b==2 && c==2 && a==0)
        return 1;
         else if(e == 2 && f == 2 && d==0)
        return 4;
         else if(i == 2 && h==2 && g==0)
        return 7;
         else if(a==2 && e==2 && i==0)
        return 9;
         else if(e==2 && i==2 && a==0)
        return 1;
         else if(c==2 && e==2 && g==0)
        return 7;
         else if(g==2 && e==2 && c==0)
        return 3;


         else if(a==1 && d == 1 && g ==0)
        return 7;
         else if(b==1 && e==1 && h== 0)
        return 8;
         else if(c==1 && f==1 && i==0)
        return 9;
         else if(d==1 && g==1 && a==0)
        return 1;
         else if(e==1 && h== 1 && b==0)
        return 2;
         else if(f==1 && i ==1 && c==0)
        return 3;
         else if(a == 1 && b==1 && c == 0)
        return 3;
         else if(d==1 && e==1 && f==0)
        return 6;
         else if(g == 1 && h==1 && i==0)
        return 9;
         else if(b==1 && c==1 && a==0)
        return 1;
         else if(e==1 && f==1 && d==0)
        return 4;
         else if(i == 1 && h==1 && g==0)
        return 7;
         else if(a==1 && e==1 && i==0)
        return 9;
         else if(e==1 && i==1 && a==0)
        return 1;
         else if(c==1 && e==1 && g==0)
        return 7;
         else if(g==1 && e==1 && c==0)
        return 3;


        return 0;
}






