#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

//Definindo as dimensões da tela(Praticamente FULL HD 1920x1020)
#define WIDTHSCREEN 1920
#define HEIGHTSCREEN 1020

//Definindo os estados de transição de tela
typedef enum EstadosDeTela{Carregamento, Titulo, Gameplay, Final}EstadosDeTela;

int main(void){

    //Inicializando a tela com as dimensões pre-processadas
    InitWindow(WIDTHSCREEN,HEIGHTSCREEN,"Bloody War");

    //Posições dos textos que aparecem no menu do jogo
    Vector2 textPosition1 = {1160.f, 140.f};
    Vector2 textPosition2 = {1110.f, 600.f};
    Vector2 textPosition3 = {1340.f, 690.f};
    Vector2 textPosition4 = {1320.f, 780.f};

    //Instalação da fonte do tipo ttf
    Font font = LoadFont("leadcoat.ttf");

    //Verificando se não ocorreu nenhum problema na instalação da fonte
    if (font.texture.id == 0) {
        TraceLog(LOG_WARNING, "Font could not be loaded! Exiting...");
        CloseWindow();
        return -1;
    }

    //Processo para carregar as imagens do menu do jogo
    //Primeira Imagem
    Image myImage = LoadImage("OIG.png");
    Texture2D texture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);

    //Segunda Imagem
    Image myImage2 = LoadImage("espadas.png");
    Texture2D texture2 = LoadTextureFromImage(myImage2);
    UnloadImage(myImage2);

    //Variavel que armazena o estado de tela atual
    EstadosDeTela estadoTela = Carregamento;

    //Loop enquanto a tela não estiver pronta
    while(!IsWindowReady()){

    }
    //Variavel para contar a quantidade de quadros
    int counterFps = 0;

    //Cravando 60 FPS
    SetTargetFPS(60);

    //Loop principal onde o jogo vai rodar
    while(!WindowShouldClose()){

        switch(estadoTela)
        {
            //Fica na tela de carregamento por dois segundo, ou seja, 120 Frames
            case Carregamento:
            {
                counterFps++;
                if(counterFps > 120)
                    estadoTela = Titulo;

            }break;
            //Entra no jogo se o usuario digitar Enter
            case Titulo:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Gameplay;

            }break;
            //Fecha o jogo se o usuario digitar ESC
            case Gameplay:
            {
                if(IsKeyPressed(KEY_ESCAPE))
                    estadoTela = Final;
            }break;
            //Volta para o menu se o usuario digitar Enter
            case Final:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Titulo;
            }break;

            default:break;

        }
        //Inicio da parte grafica(Desenho)
        BeginDrawing();
        //Definindo o fundo de cor preta
        ClearBackground(BLACK);

            switch(estadoTela)
            {
                //Tela de carregamento
                case Carregamento:
                {
                    DrawText("Loading Screen", 20, 20, 100, RED);
                    DrawText("This a loading screen. Wait for 2 seconds!",100,HEIGHTSCREEN/2,80, WHITE);

                }break;
                //Tela de titulo
                case Titulo:
                {
                    //Introduzindo as duas imagens(texturas)
                    DrawTexture(texture,0,0, WHITE);
                    DrawTexture(texture2,1050,345, WHITE);

                    //Introduzindo os retangulos do titulo
                    DrawRectangle(1100,40,780,320,RED);
                    DrawRectangle(1150,70,680,270,MAROON);

                    //Titulo e etc...
                    DrawTextEx(font,"Bloody War",textPosition1,150,2,BLACK);
                    DrawTextEx(font,"Press Enter for continue!",textPosition2,75,2,RAYWHITE);
                    DrawTextEx(font,"Options",textPosition3,75,2,RAYWHITE);
                    DrawTextEx(font,"Controls",textPosition4,75,2,RAYWHITE);
 
                }break;
                case Gameplay:
                {
                    //Logica do jogo aqui dentro
                    DrawText("This is a Gameplay Screen",100,HEIGHTSCREEN/2,80,WHITE);

    
                }break;
                case Final:
                {
                    //Tela final aqui dentro
                    DrawText("This is a Ending Screen",100,HEIGHTSCREEN/2,80,WHITE);

                }break;
                default: break;
            }
    
        EndDrawing();
        //Fim da parte grafica(Desenho)
    }
    CloseWindow();

return 0;
}
