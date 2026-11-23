#include "sound.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>

static Sound sonBoun;
static Sound sonTac;
static short *dataBoun = NULL;
static short *dataTac = NULL;

void InitSound(void) {
// BOUN - Grave, 0.15s
Wave w = {0};
w.sampleCount = 6615; w.sampleRate = 44100; w.sampleSize = 16; w.channels = 1;
dataBoun = (short*)malloc(sizeof(short) * w.sampleCount);
for (int i = 0; i < w.sampleCount; i++) {
float t = (float)i / 44100.0f;
float freq = 150 - (t * 500); if (freq < 40) freq = 40;
float env = 1.0f - (t / 0.15f);
dataBoun[i] = (short)(sinf(2 * PI * freq * t) * 30000 * env);
}
w.data = dataBoun;
sonBoun = LoadSoundFromWave(w);
SetSoundVolume(sonBoun, 0.9f);

// TAC - Aigu, 0.05s
Wave w2 = {0};
w2.sampleCount = 2205; w2.sampleRate = 44100; w2.sampleSize = 16; w2.channels = 1;
dataTac = (short*)malloc(sizeof(short) * w2.sampleCount);
for (int i = 0; i < w2.sampleCount; i++) {
float t = (float)i / 44100.0f;
float env = 1.0f - (t / 0.05f);
dataTac[i] = (short)(sinf(2 * PI * 800 * t) * 15000 * env);
}
w2.data = dataTac;
sonTac = LoadSoundFromWave(w2);
SetSoundVolume(sonTac, 0.4f);
}

void PlayBoun(void) { PlaySound(sonBoun); }
void PlayTac(void) { PlaySound(sonTac); }

void UnloadSoundAll(void) {
UnloadSound(sonBoun);
UnloadSound(sonTac);
free(dataBoun);
free(dataTac);
}