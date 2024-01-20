//Importação das bibliotecas
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

//Definindo as dimensões da tela(Resolução HD 1280x720)
#define WIDTHSCREEN 1280
#define HEIGHTSCREEN 720

//Definindo os estados de transição de tela
typedef enum EstadosDeTela{Carregamento, Titulo, Gameplay, Opcoes, Final}EstadosDeTela;

//Struct de opções do menu, elas vão facilitar o manejo das opções
typedef struct {
    Rectangle rect;
    const char* text;
    Color textColor;
    Color rectColor;
} MenuItem;

int main(void){

    //Inicializando a tela com as dimensões pre-processadas
    InitWindow(WIDTHSCREEN,HEIGHTSCREEN,"Bloody War");

    //Posições dos textos que aparecem no menu do jogo
    Vector2 textPosition1 = {785.f, 130.f};

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
    Image myImage = LoadImage("OIG_resized.png");
    Texture2D texture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);

    //Segunda Imagem
    Image myImage2 = LoadImage("Espadas_Duplas2.png");
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

        //Definindo algumas opções de menu manualmente
        int numItemsMenu = 3;
        int numItemsOptions = 2;
        MenuItem itemsMenu[] = {
            {{793, 300, 400, 40}, "Press Enter to battle!", WHITE, RED},
            {{940, 470, 200, 40}, "Options", WHITE, RED},
            {{930, 530, 200, 40}, "Controls", WHITE, RED},
        };

        MenuItem itemsOptions[] = {
            {{940, 470, 200, 40}, "Sound", WHITE, RED},
            {{930, 530, 200, 40}, "Back", WHITE, RED}

        };

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
                
                //Implementando interacao do mouse no menu
                for(int i = 0; i < numItemsMenu; i++) {
                    if(CheckCollisionPointRec(GetMousePosition(), itemsMenu[i].rect)) {
                        itemsMenu[i].rectColor = MAROON;
                        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            if(i == 0) estadoTela = Gameplay;
                            else if(i == 1) estadoTela = Opcoes;
                            //else if(i == x) continuar para as outras opções... 
                        }
                    }
                }
            }break;

            //Fecha o jogo e vai para a tela final se o usuario digitar P
            case Gameplay:
            {
                if(IsKeyPressed(KEY_P))
                    estadoTela = Final;
            }break;

            //Volta para o menu se o usuario digitar Enter
            case Opcoes:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Titulo;
                int temSom = 1; //Flag para saber se o som está ligado ou não
                for(int i = 0; i < numItemsOptions; i++) {
                        if(CheckCollisionPointRec(GetMousePosition(), itemsOptions[i].rect)) {
                            itemsOptions[i].rectColor = MAROON;
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                if(i == 0) { //Opção para o som
                                    if(temSom) {
                                        SetMasterVolume(0.0);
                                        temSom = 0;
                                    }
                                    else {
                                        SetMasterVolume(1.0);
                                        temSom = 1;
                                    }
                                }
                                else if(i == 1) estadoTela = Titulo; 
                            }
                        }
                    }
            }break;
            
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
                    DrawText("Loading Screen", 20, 20, 50, RED);
                    DrawText("This a loading screen. Wait for 2 seconds!",100,HEIGHTSCREEN/2,50, WHITE);

                }break;
                //Tela de titulo
                case Titulo:
                {

                    //Introduzindo as duas imagens(texturas)
                    DrawTexture(texture,0,0, WHITE);
                    DrawTexture(texture2,800,345, WHITE);

                    //Introduzindo os retangulos do titulo
                    DrawRectangle(760,40,490,240,RED);
                    DrawRectangle(780,65,450,190,MAROON);

                    //Titulo
                    DrawTextEx(font,"Bloody War",textPosition1,100,2,BLACK);
                    //Opções
                    for(int i = 0; i < numItemsMenu; i ++) { 
                        DrawRectangleRec(itemsMenu[i].rect, itemsMenu[i].rectColor);
                        DrawText(itemsMenu[i].text, (int)(itemsMenu[i].rect.x + itemsMenu[i].rect.width / 2 - MeasureText(itemsMenu[i].text, 20) / 2), (int)(itemsMenu[i].rect.y + itemsMenu[i].rect.height / 2 - 10), 20, itemsMenu[i].textColor);
                    }

                }break;
                case Gameplay:
                {
                    //Logica do jogo aqui dentro
                    //O que acontece no jogo sera introduzido aqui, que é a tela pos menu
                    //Aqui a parte grafica do jogo acontece
                    DrawText("Gameplay Screen is Here",100,HEIGHTSCREEN/2,60,WHITE);
                }break;

                case Opcoes:
                {
                    for(int i = 0; i < numItemsOptions; i ++) { 
                        DrawRectangleRec(itemsOptions[i].rect, itemsOptions[i].rectColor);
                        DrawText(itemsOptions[i].text, (int)(itemsOptions[i].rect.x + itemsOptions[i].rect.width / 2 - MeasureText(itemsOptions[i].text, 20) / 2), (int)(itemsOptions[i].rect.y + itemsOptions[i].rect.height / 2 - 10), 20, itemsOptions[i].textColor);
                    }
                    DrawText("Press ENTER to go to Menu.",100,HEIGHTSCREEN/2,60,WHITE);
                }break;
                case Final:
                {
                    //Tela final aqui dentro
                    DrawText("Ending Screen is Here",100,HEIGHTSCREEN/2,60,WHITE);

                }break;
                default: break;
            }
    
        EndDrawing();
        //Fim da parte grafica(Desenho)
    }
    //Fechar janela
    CloseWindow();

return 0;
}
