#include "nossalib.h"

bool audioLigado;
Sound musicaMenu;
Sound musicaLuta;
Sound musicaFinal;
Sound fxSoco;
Sound fxChute;
Sound fxHadouken;
Sound fxVidaExtra;
Sound fxPulo;
Sound fxCombate;
Sound fxSelecao;


void initAudios() {
    //Carregamento dos audios
    SetMasterVolume(1.0f);
    musicaMenu = LoadSound("musicaMenu.wav");

    musicaLuta = LoadSound("musicaLuta.wav");

    musicaFinal = LoadSound("musicaFinal.wav");

    fxSoco = LoadSound("soco.wav");

    fxChute = LoadSound("chute.wav");

    fxHadouken = LoadSound("hadouken.wav");

    fxVidaExtra = LoadSound("vidaExtra.wav");

    fxPulo = LoadSound("pulo.wav");

    fxCombate = LoadSound("combate.wav");

    fxSelecao = LoadSound("selecao.wav");
}

//Função que toca as músicas
void tocaAudio(int num) {
    if(num == 1) {
        StopSound(musicaFinal);
        StopSound(musicaLuta);
        PlaySound(musicaMenu);
    }
    else if(num == 2) {
        StopSound(musicaFinal);
        StopSound(musicaMenu);
        PlaySound(musicaLuta);
    }
    else if(num == 3) {
        StopSound(musicaLuta);
        StopSound(musicaMenu);
        PlaySound(musicaFinal);
    }

}
// Função para atualizar a posição do personagem e mantê-lo dentro dos limites da tela
void atualizarPersonagem(Character *character, int sentido, int screenWidth){
    character->posRect.x += sentido * character->velocity;

    //Controle de movimentação do personagem dentro dos limites da tela
    if (character->posRect.x < 0)
        character->posRect.x = 0;
    if (character->posRect.x + character->posRect.width > WIDTHSCREEN)
        character->posRect.x = screenWidth - character->posRect.width;
}

void desenharCharacter(Character *character, Texture2D texture) {
    DrawTexturePro(texture, character->SourceRect, character->posRect,(Vector2){0, 0}, 0, WHITE);
}

void dispararProjetil(Projectile *projetcile){
    if (!projetcile->active){
        projetcile->active = true;
    }
}

void aplicarGravidade(Character *Character, int *countJump, int num) {
    if (Character->isJumping) {
        Character->posRect.y -= Character->jumpSpeed;
        Character->jumpForce -= Character->jumpSpeed/1.7;

        // Verifica se o pulo terminou
        if (Character->jumpForce <= 0) {
            Character->isJumping = false;
        }
        }else{
            if(num == 0){
                if((Character->posRect.y + Character->posRect.height) < 650) {
                    Character->posRect.y += 14;
                }
                else
                    (*countJump) = 0;
            }
            else
                if((Character->posRect.y + Character->posRect.height) < 565) {
                    Character->posRect.y += 14;
                }
                else
                    (*countJump) = 0;
        }
}

void PerformTransition(Texture2D *vector, int *currentTextureIndex, int *transitionOffset, int *direction, int screenWidth){
    if (IsKeyPressed(KEY_RIGHT))
    {
        (*currentTextureIndex)++;
        if (*currentTextureIndex >= 3) // Change 3 to the total number of textures
            *currentTextureIndex = 0; // Return to the first texture
        *transitionOffset = screenWidth; // Set initial screen-width offset to the right
        *direction = 1;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        (*currentTextureIndex)--;
        if (*currentTextureIndex < 0)
            *currentTextureIndex = 2; // Return to the last texture
        *transitionOffset = -screenWidth; // Set initial screen-width offset to the left
        *direction = -1;
    }

    if (*transitionOffset != 0)
    {
        *transitionOffset -= TRANSITION_SPEED * (*direction); // Gradually decrease offset to 0
        if (abs(*transitionOffset) < TRANSITION_SPEED)
            *transitionOffset = 0;
    }
}