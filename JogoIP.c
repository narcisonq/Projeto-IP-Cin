#include "raylib.h"

//Definindo as dimensões da tela(FULL HD)
#define WIDTHSCREEN 1920
#define HEIGHTSCREEN 1020

//Definindo os estados de tela
typedef enum EstadosDeTela{Carregamento, Titulo, Gameplay, Final}EstadosDeTela;

int main(void){

    //Inicializando a tela com as dimensões pre-processadas
    InitWindow(WIDTHSCREEN,HEIGHTSCREEN,"Game with raylib");

    Image myImage = LoadImage("OIG.png");
    Texture2D texture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);

    EstadosDeTela estadoTela = Carregamento;

    //Ficar nesse loop enquanto a janela não tiver pronta
    while(!IsWindowReady()){

    }
    int counterFps = 0;

    //Cravando 60 FPS
    SetTargetFPS(60);

    //Loop principal onde o jogo vai rodar
    while(!WindowShouldClose()){

        switch(estadoTela)
        {
            case Carregamento:
            {
                counterFps++;
                if(counterFps > 120)
                    estadoTela = Titulo;

            }break;

            case Titulo:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Gameplay;

            }break;

            case Gameplay:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Final;
            }break;

            case Final:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Titulo;
            }break;

            default: break;

        }

        ClearBackground(RAYWHITE);
        //Inicio da parte grafica(Desenho)
        BeginDrawing();

            switch(estadoTela)
            {
                case Carregamento:
                {
                    DrawText("Loading Screen", 20, 20, 100, GREEN);
                    DrawText("This a loading screen. Wait for 2 seconds!",100,HEIGHTSCREEN/2,80, BLACK);

                }break;
                case Titulo:
                {
                    ClearBackground(BLACK);
                    DrawTexture(texture,0,0, WHITE);
                    DrawRectangle(1100,70,780,350,RED);
                    DrawRectangle(1150,100,680,290,MAROON);
                    
                    DrawText("Bloody War",1180,200,110,BLACK);
                    DrawText("Press Enter for continue!",1120,600,50,RAYWHITE);
                    DrawText("Options", 1350,670,50,RAYWHITE);
                    DrawText("Controls",1340,740,50,RAYWHITE );
                    
                }break;
                case Gameplay:
                {

    
                }break;
                case Final:
                {


                }break;
                default: break;
            }
    
        EndDrawing();
        //Fim da parte grafica(Desenho)
    }
    CloseWindow();

return 0;
}
