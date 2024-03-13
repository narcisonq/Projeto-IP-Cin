#include "raylib.h"

#define MAX_BACKGROUNDS 3 // Defina o número máximo de imagens de fundo

// Função para trocar o plano de fundo
int mudarfundo(Texture2D *fundoAtual, Texture2D fundos[], int indiceAtual, int direction)
{
    indiceAtual += direction;

    // Verifica se o índice está dentro dos limites do vetor
    if (indiceAtual < 0)
    {
        indiceAtual = MAX_BACKGROUNDS - 1;
    }
    else if (indiceAtual >= MAX_BACKGROUNDS)
    {
        indiceAtual = 0;
    }

    *fundoAtual = fundos[indiceAtual];

    return indiceAtual;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Inicialize a janela
    InitWindow(screenWidth, screenHeight, "Imagens e Teclas");

    // Carregue suas imagens de fundo em um vetor
    Texture2D fundos[MAX_BACKGROUNDS];
    fundos[0] = LoadTexture("Mapa1.png");
    fundos[1] = LoadTexture("mapa2.png");
    fundos[2] = LoadTexture("mapa3.png");

    Texture2D fundoAtual = fundos[0];
    int indiceAtual = 0;

    while (!WindowShouldClose())
    {
        // Verifique as teclas pressionadas
        if (IsKeyPressed(KEY_RIGHT))
        {
            indiceAtual = mudarfundo(&fundoAtual, fundos, indiceAtual, 1);
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            indiceAtual = mudarfundo(&fundoAtual, fundos, indiceAtual, -1);
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Desenhe a imagem atual
        DrawTexture(fundoAtual, 0, 0, WHITE);

        EndDrawing();
    }

    // Libere as texturas e feche a janela
    for (int i = 0; i < MAX_BACKGROUNDS; i++)
    {
        UnloadTexture(fundos[i]);
    }

    CloseWindow();

    return 0;
}


