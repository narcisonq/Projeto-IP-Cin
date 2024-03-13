#include "raylib.h"
#include <stdlib.h> // For the rand() function

#define TRANSITION_SPEED 3 // Adjust transition speed as necessary
typedef enum EstadosDeTela {Carregamento,Titulo,Gameplay,Solo,Personagem,Mapa,Controles,Opcoes,Final}EstadosDeTela;

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

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Imagens e Teclas");

    // Carregue suas imagens de fundo em um vetor
    Texture2D vetor[3];
    vetor[0] = LoadTexture("Mapa1.png");
    vetor[1] = LoadTexture("mapa2.png");
    vetor[2] = LoadTexture("mapa3.png");

    int currentTextureIndex = 0;
    int selectedMapIndex = 0; // Store the index of the selected map
    int transitionOffset = 0;
    int sentido = 0;

    EstadosDeTela estadoTela = Mapa;

    while (!WindowShouldClose())
    {
        // Chame a função para realizar a transição
        
        switch(estadoTela) {
            case Mapa:{
                PerformTransition(vetor, &currentTextureIndex, &transitionOffset, &sentido, screenWidth);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    // Store the index of the selected map
                    selectedMapIndex = currentTextureIndex;
                    estadoTela = Gameplay; // Change state to Gameplay when the left mouse button is pressed
                }
            }break;

            case Gameplay:{
               // ClearBackground(RAYWHITE);
                // Draw the selected map during gameplay
                //DrawTexture(vetor[selectedMapIndex], 0, 0, WHITE);
            } break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(estadoTela) {
            // Desenhe a textura atual
            case Mapa:{
                DrawTexturePro(vetor[currentTextureIndex], (Rectangle){0, 0, 1280, 620}, (Rectangle){0, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);

                // **Transition Animation:**
                if (transitionOffset != 0) {
                    
                    float progress = (float)abs(transitionOffset) / screenWidth; // Calculate transition progress
                    if(sentido == 1){
                    // Draw the previous image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){screenWidth * progress * sentido, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);

                    // Draw the next image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex + 2) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){-screenWidth * (1 - progress) * sentido, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);
                    }
                    else if(sentido == -1){
                     DrawTexturePro(vetor[(currentTextureIndex) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){screenWidth * progress * sentido, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);

                    // Draw the next image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex + 1) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){-screenWidth * (1 - progress) * sentido, 0, 1280, 720}, (Vector2){0, 0}, 0, WHITE);
                    }
                }

            } break;

            case Gameplay:{
                ClearBackground(RAYWHITE);
                // Draw the selected map during gameplay
                DrawTexture(vetor[selectedMapIndex], 0, 0, WHITE);
            } break;
        }
        EndDrawing();
    }

    // Libere as texturas e feche a janela
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(vetor[i]);
    }

    CloseWindow();

    return 0;
}