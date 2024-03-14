#ifndef MYLIBRARY_H
#define MYLIBRARY_H

#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTHSCREEN 1280
#define HEIGHTSCREEN 720
#define TRANSITION_SPEED 40 // Adjust transition speed as necessary

typedef enum EstadosDeTela { Carregamento, Titulo, Gameplay, Solo, Personagem, Mapa, Controles, Opcoes, Final } EstadosDeTela;

// Estrutura do personagem
typedef struct {
    Rectangle posRect;
    Rectangle SourceRect;
    char name[20];
    int health;
    int attackDamage;
    int jumpForce;
    int velocity;
    int jumpSpeed;
    int isJumping;
    bool isAttacking;
} Character;

// Estrutura do projetil
typedef struct {
    Character player;
    Rectangle Source;
    Rectangle Position;
    Vector2 speed;
    bool active;
} Projectile;

// Estrutura para os itens do menu
typedef struct {
    Rectangle rect;
    const char* text;
    Color textColor;
    Color rectColor;
} MenuItem;

extern bool audioLigado;
extern Sound musicaMenu;
extern Sound musicaLuta;
extern Sound musicaFinal;
extern Sound fxSoco;
extern Sound fxChute;
extern Sound fxHadouken;
extern Sound fxVidaExtra;
extern Sound fxPulo;
extern Sound fxCombate;
extern Sound fxSelecao;

// Function prototypes
void initAudios();
void tocaAudio(int num);
void atualizarPersonagem(Character* character, int sentido, int screenWidth);
void desenharCharacter(Character* character, Texture2D texture);
void dispararProjetil(Projectile* projectile);
void aplicarGravidade(Character* character, int* countJump, int num);
void PerformTransition(Texture2D* vector, int* currentTextureIndex, int* transitionOffset, int* direction, int screenWidth);

#endif
