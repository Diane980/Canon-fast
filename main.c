#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// --- STRUCTURES ---
typedef struct {
float x, y;
int vieMax, vie;
int actif;
Color beret;
} Bandit;

typedef struct {
float x, y;
int actif;
} Roquette;

typedef struct {
float x, y;
int actif;
} BalleAK;

// --- VARIABLES GLOBALES ---
extern int life;
extern int kills;
extern int vague;
extern int pauseJeu;
extern int healUtilise;
extern float canonX, canonY;
extern Bandit ligne[12];
extern Roquette roquettes[20];
extern BalleAK balles[100];
extern float feuTimer;
extern Rectangle btnHeal, btnPause, btnFire;
extern char msgVague[64];
extern float timerVagueMsg;

// --- FONCTIONS ---
void InitGame(void);
void CreerVague(int num);
void UpdateGame(void);
void DrawGame(void);

#endif