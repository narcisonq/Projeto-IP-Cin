#include "raylib.h"

typedef struct bola
{
    float posx;
    float posy;
    float raio;
} bola;



void desenhar()
{
    for (int i = 0; i < 100; i++)
    {
        DrawLine(10 * 3 * i, 750, 10 * i * 3, 790, WHITE);
    }

    for (int i = 0; i < 39; i++)
    {
        DrawLine(5, 10 * 3 * i, 15, 10 * i * 3, WHITE);
    }
}

void moverBola(bola *minhaBola)
{
    // mexendo a vola usando a desreferencia do ponteiro, dá pra usar o restrict nesse argumento
    //pra nao usar -> eu prefiro usar a desreferencia
    if (IsKeyDown(KEY_RIGHT)) (*minhaBola).posx += 5.0f;
    if (IsKeyDown(KEY_LEFT)) (*minhaBola).posx -= 5.0f;
    if (IsKeyDown(KEY_UP)) (*minhaBola).posy -= 5.0f;
    if (IsKeyDown(KEY_DOWN)) (*minhaBola).posy += 5.0f;
}

int main(){
    InitWindow(1400, 800, "raylib [core] example - basic window"); //tela

    bola minhaBola = {10, 770, 10}; // posição inicial da bola
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // desenhando o circulo usando as coordenadas da bola
        DrawCircle(minhaBola.posx, minhaBola.posy, minhaBola.raio, WHITE);
        
        //plano cartesiano pra referencia  
        DrawLine(10, -100, 10, 10000, WHITE);
        DrawLine(-100, 770, 10000, 770, WHITE);

        desenhar(); //linhas de referencia
        moverBola(&minhaBola); // Call the function to handle circle movement

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
