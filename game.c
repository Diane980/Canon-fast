#include "game.h"
#include "sound.h"
#include <math.h>
#include <stdlib.h>

// --- ETAT DU JEU ---
int life = 1000;
int kills = 0;
int vague = 1;
int pauseJeu = 0;
int healUtilise = 0;
float canonX = 500;
float canonY = 1650;
Bandit ligne[12];
Roquette roquettes[20] = {0};
BalleAK balles[100] = {0};
float feuTimer = 0;
float timerTirAK = 0;
float timerVagueMsg = 0;
char msgVague[64] = "VAGUE 1 - ROUGES !";
Rectangle btnHeal = {30, 1760, 280, 110};
Rectangle btnPause = {360, 1760, 280, 110};
Rectangle btnFire = {690, 1760, 280, 110};

void InitGame(void) {
life = 1000; kills = 0; vague = 1;
pauseJeu = 0; healUtilise = 0;
canonX = 500; canonY = 1650;
CreerVague(1);
}

void CreerVague(int num) {
Color c = RED; int v = 1;
if (num == 2) { c = YELLOW; v = 2; }
if (num == 3) { c = GREEN; v = 3; }
for (int i = 0; i < 12; i++) {
ligne[i].x = 45 + i * 78;
ligne[i].y = 280;
ligne[i].actif = 1;
ligne[i].beret = c;
ligne[i].vieMax = v; ligne[i].vie = v;
}
if (num == 1) TextCopy(msgVague, "VAGUE 1 - ROUGES !");
if (num == 2) TextCopy(msgVague, "VAGUE 2 - JAUNES !");
if (num == 3) TextCopy(msgVague, "VAGUE 3 - VERTS !");
}

static void Tirer(void) {
for (int i = 0; i < 20; i++) if (!roquettes[i].actif) {
roquettes[i].x = canonX; roquettes[i].y = canonY - 120; roquettes[i].actif = 1;
PlayBoun(); // BOUN !
feuTimer = 0.08f; // Petit feu
break;
}
}

void UpdateGame(void) {
float dt = GetFrameTime();
Vector2 souris = GetMousePosition();
int clic = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

// --- CANON BOUGE ---
if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) canonX -= 8;
if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) canonX += 8;
if (IsKeyDown(KEY_SPACE) && !pauseJeu && life > 0) Tirer();
// Sur telephone : glisser le doigt
if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMousePosition().y > 1500 && GetMousePosition().y < 1640) {
canonX = GetMousePosition().x;
}
if (canonX < 80) canonX = 80;
if (canonX > 920) canonX = 920;

// --- BOUTONS ---
if (clic) {
if (CheckCollisionPointRec(souris, btnFire) && !pauseJeu && life > 0) Tirer();
if (CheckCollisionPointRec(souris, btnHeal) && healUtilise == 0 && !pauseJeu) {
life += 200; if (life > 1000) life = 1000; healUtilise = 1;
}
if (CheckCollisionPointRec(souris, btnPause)) pauseJeu = !pauseJeu;
}

if (!pauseJeu && life > 0) {
// Roquettes montent
for (int i = 0; i < 20; i++) if (roquettes[i].actif) {
roquettes[i].y -= 12;
if (roquettes[i].y < 0) roquettes[i].actif = 0;
for (int b = 0; b < 12; b++) if (ligne[b].actif) {
if (fabsf(roquettes[i].x - ligne[b].x) < 35 && fabsf(roquettes[i].y - ligne[b].y) < 35) {
ligne[b].vie--; roquettes[i].actif = 0;
if (ligne[b].vie <= 0) { ligne[b].actif = 0; kills++; }
break;
}
}
}
// AK-47 : TAC TAC
timerTirAK += dt;
if (timerTirAK > 0.5f) {
timerTirAK = 0;
for (int b = 0; b < 12; b++) if (ligne[b].actif) {
for (int j = 0; j < 100; j++) if (!balles[j].actif) {
balles[j].x = ligne[b].x; balles[j].y = ligne[b].y + 30; balles[j].actif = 1;
PlayTac(); break;
}
}
}
for (int j = 0; j < 100; j++) if (balles[j].actif) {
balles[j].y += 9;
if (balles[j].y > 1900) balles[j].actif = 0;
if (fabsf(balles[j].x - canonX) < 60 && fabsf(balles[j].y - canonY) < 80) {
life -= 10; balles[j].actif = 0; if (life < 0) life = 0;
}
}
if (feuTimer > 0) feuTimer -= dt;
// Vague finie ?
int restants = 0;
for (int b = 0; b < 12; b++) if (ligne[b].actif) restants++;
if (restants == 0) {
vague++; if (vague > 3) vague = 1;
CreerVague(vague);
timerVagueMsg = 2.0f;
}
if (timerVagueMsg > 0) timerVagueMsg -= dt;
}
}

void DrawGame(void) {
BeginDrawing();
ClearBackground((Color){90, 200, 80, 255});

// LIFE à gauche
DrawRectangle(20, 20, 420, 90, (Color){20, 60, 80, 220});
DrawText(TextFormat("LIFE: %i", life), 50, 30, 45, WHITE);
DrawRectangle(50, 85, 360, 12, DARKGRAY);
DrawRectangle(50, 85, (int)(360 * life / 1000.0f), 12, GREEN);
// KILLS à droite
DrawRectangle(620, 20, 360, 90, (Color){20, 30, 40, 220});
DrawText(TextFormat("KILLS: %i", kills), 660, 30, 45, WHITE);

// Bandits : 1 seule ligne
for (int b = 0; b < 12; b++) if (ligne[b].actif) {
float x = ligne[b].x, y = ligne[b].y;
DrawRectangle((int)(x - 18), (int)(y - 5), 36, 45, BLACK);
DrawRectangle((int)(x - 18), (int)(y + 38), 14, 18, DARKGRAY);
DrawRectangle((int)(x + 4), (int)(y + 38), 14, 18, DARKGRAY);
DrawCircle((int)x, (int)(y - 15), 14, (Color){255, 200, 160, 255});
DrawCircle((int)x, (int)(y - 22), 15, ligne[b].beret);
DrawRectangle((int)(x - 15), (int)(y - 22), 30, 6, ligne[b].beret);
DrawRectangle((int)(x - 20), (int)(y + 10), 45, 5, (Color){30, 30, 30, 255});
DrawRectangle((int)(x - 20), (int)(y - 45), 40, 6, DARKGRAY);
DrawRectangle((int)(x - 20), (int)(y - 45), (int)(40 * ligne[b].vie / (float)ligne[b].vieMax), 6, RED);
}

if (timerVagueMsg > 0) {
DrawRectangle(250, 450, 500, 80, (Color){0, 0, 0, 180});
DrawText(msgVague, 280, 475, 38, YELLOW);
}

// Roquettes + petite trainée
for (int i = 0; i < 20; i++) if (roquettes[i].actif) {
DrawCircle((int)roquettes[i].x, (int)(roquettes[i].y + 18), 4, ORANGE);
DrawCircle((int)roquettes[i].x, (int)(roquettes[i].y + 26), 3, YELLOW);
DrawCircle((int)roquettes[i].x, (int)(roquettes[i].y + 34), 2, RED);
DrawRectangle((int)(roquettes[i].x - 5), (int)(roquettes[i].y - 15), 10, 30, LIGHTGRAY);
DrawTriangle((Vector2){roquettes[i].x - 5, roquettes[i].y - 15},
(Vector2){roquettes[i].x + 5, roquettes[i].y - 15},
(Vector2){roquettes[i].x, roquettes[i].y - 28}, RED);
}
for (int j = 0; j < 100; j++) if (balles[j].actif) DrawCircle((int)balles[j].x, (int)balles[j].y, 3, YELLOW);

// Rails
for (int i = 0; i < 14; i++) DrawRectangle(20 + i * 70, 1640, 45, 90, (Color){150, 80, 60, 255});
DrawRectangle(0, 1655, 1000, 10, DARKGRAY);
DrawRectangle(0, 1705, 1000, 10, DARKGRAY);

// Canon
DrawRectangle((int)(canonX - 55), (int)(canonY - 60), 110, 110, (Color){30, 140, 40, 255});
DrawRectangle((int)(canonX - 70), (int)(canonY - 20), 140, 40, (Color){20, 110, 30, 255});
DrawRectangle((int)(canonX - 10), (int)(canonY - 160), 20, 110, DARKGRAY);
DrawCircle((int)canonX, (int)(canonY - 165), 14, BLACK);
DrawRectangle((int)(canonX - 80), (int)(canonY - 210), 160, 35, GREEN);
DrawText("INFINITE", (int)(canonX - 70), (int)(canonY - 202), 22, BLACK);
DrawText("oo", (int)(canonX + 55), (int)(canonY - 205), 22, BLACK);

// Petit feu
if (feuTimer > 0) {
DrawCircle((int)canonX, (int)(canonY - 180), 12, ORANGE);
DrawCircle((int)canonX, (int)(canonY - 180), 8, YELLOW);
DrawCircle((int)canonX, (int)(canonY - 180), 4, WHITE);
}

// Boutons
DrawRectangleRec(btnHeal, healUtilise ? DARKGRAY : GREEN);
DrawText(healUtilise ? "+ HEAL 0x" : "+ HEAL 1x", (int)btnHeal.x + 25, (int)btnHeal.y + 35, 40, WHITE);
DrawRectangleRec(btnPause, pauseJeu ? RED : GOLD);
DrawText(pauseJeu ? "PLAY" : "PAUSE", (int)btnPause.x + 60, (int)btnPause.y + 35, 40, BLACK);
DrawRectangleRec(btnFire, RED);
DrawText("FIRE", (int)btnFire.x + 85, (int)btnFire.y + 35, 50, WHITE);
DrawText("<<< >>>", (int)btnPause.x + 60, 1885, 30, DARKGREEN);

if (life <= 0) {
DrawRectangle(0, 0, 1000, 1900, (Color){0, 0, 0, 150});
DrawText("GAME OVER", 280, 800, 80, RED);
DrawText(TextFormat("KILLS: %i", kills), 350, 900, 50, WHITE);
}
if (pauseJeu && life > 0) DrawText("PAUSE", 420, 800, 60, WHITE);

EndDrawing();
}