//Importação das bibliotecas
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

//Definindo as dimensões da tela(Resolução HD 1280x720)
#define WIDTHSCREEN 1280
#define HEIGHTSCREEN 720

//Definindo os estagios de transição de tela
//Posteriormente sera necesssario mais estados de tela
typedef enum EstadosDeTela{Carregamento, Titulo, Gameplay, Final}EstadosDeTela;

//Criação de um personagem com atributos de vida e de dano
//Posteriormente sera necessario mais structs para diversificar os personagens
typedef struct{
    Rectangle rect;
    int health;
    int attackDamage;

}Character;

//Função feita para atualizar personagem e manter ele dentros dos limites da tela
//Recebe o personagem, o sentido do movimento e a largura da tela
void atualizarPersonagem(Character *character, int sentido, int screenWidth){

    //Basicamente atualiza a posição do personagem
    //5 é a constante que define a velocidade, um numero maior,mais velocidade
    character->rect.x += sentido * 5;

    //Isso impede que o personagem ultrapasse os limites da tela
    if(character->rect.x < 0)
        character->rect.x = 0;
    if(character->rect.x + character->rect.width > WIDTHSCREEN)
        character->rect.x = screenWidth - character->rect.width;
}
void desenharCharacter(Character character, Color color) {
    // Desenha o personagem na tela
    DrawRectangleRec(character.rect, color);
}

int main(void){

    //Inicializando a tela com as dimensões pre-processadas
    InitWindow(WIDTHSCREEN,HEIGHTSCREEN,"Bloody War");

    //Variaveis utilizadas para simular um cronometro de tempo
    int counterFps = 0;
    int count = 120;
    int fpsAtual;

    //Criação dos dois personagems iniciais
    Character player = {{200, 350, 100, 250}, 100, 10};
    Character enemy = {{1000, 350, 100, 250}, 100, 10};

    //Posições dos textos que aparecem no menu do jogo
    //Totalizando cinco textos
    Vector2 textPosition1 = {785.f, 130.f};
    Vector2 textPosition2 = {793.f, 300.f};
    Vector2 textPosition3 = {940.f, 470.f};
    Vector2 textPosition4 = {930.f, 530.f};
    Vector2 textPosition5 = {100,HEIGHTSCREEN/3};

    //Instalação da fonte do tipo ttf
    Font font = LoadFont("leadcoat.ttf");

    //Verificando se não ocorreu nenhum problema na instalação da fonte
    if (font.texture.id == 0) {
        TraceLog(LOG_WARNING, "Font could not be loaded! Exiting...");
        CloseWindow();
        return -1;
    }

    //Processo para carregar as imagens do menu do jogo
    Image myImage = LoadImage("OIG_resized.png");
    Texture2D texture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);

    Image myImage2 = LoadImage("Espadas_Duplas2.png");
    Texture2D texture2 = LoadTextureFromImage(myImage2);
    UnloadImage(myImage2);

    Image myImage3 = LoadImage("arena.png");
    Texture2D texture3 = LoadTextureFromImage(myImage3);
    UnloadImage(myImage3);

    Image myImage4 = LoadImage("escudo.png");
    Texture2D texture4 = LoadTextureFromImage(myImage4);
    UnloadImage(myImage4);
    
    //Variavel que armazena o estado de tela atual
    EstadosDeTela estadoTela = Carregamento;

    //Loop enquanto a tela não estiver pronta
    while(!IsWindowReady()){

    }

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
                if(counterFps > 120){
                    estadoTela = Titulo;
                    counterFps = 0;
                } 
            }break;
            //Entra no jogo se o usuario digitar Enter
            case Titulo:
            {
                if(IsKeyPressed(KEY_ENTER))
                    estadoTela = Gameplay;

            }break;
            //Fecha o jogo e vai para a tela final se o usuario digitar P
            //Cronometro de 120 segundos
            case Gameplay:
            {
                counterFps++;
                if(counterFps == 120 || counterFps == fpsAtual + 60){
                    fpsAtual = counterFps;
                    count--;
                }    
                if(counterFps == 7200)
                    estadoTela = Final;

                if(IsKeyPressed(KEY_P))
                    estadoTela = Final;

                //Atualização do personagem
                if(IsKeyDown(KEY_D))
                    atualizarPersonagem(&player,1,WIDTHSCREEN);
                if(IsKeyDown(KEY_A))
                    atualizarPersonagem(&player,-1,WIDTHSCREEN);

                if(IsKeyPressed(KEY_SPACE)){

                    if(CheckCollisionRecs(player.rect, enemy.rect)){
                        enemy.health -= player.attackDamage;
                    } 
                }
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
                    DrawTexture(texture4,500,300,WHITE);
                    DrawTextEx(font,"This a loading screen. Wait for 2 seconds",textPosition5,70,2,WHITE);

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

                    //Titulo e etc...
                    DrawTextEx(font,"Bloody War",textPosition1,100,2,BLACK);
                    DrawTextEx(font,"Press Enter for continue!",textPosition2,40,2,WHITE);
                    DrawTextEx(font,"Options",textPosition3,40,2,RAYWHITE);
                    DrawTextEx(font,"Controls",textPosition4,40,2,RAYWHITE);

                }break;
                case Gameplay:
                {
                    //Cenario de fundo
                    DrawTexture(texture3,0,0,WHITE);

                    //Players
                    desenharCharacter(player,BLUE);
                    desenharCharacter(enemy,GREEN);

                    //Piso
                    DrawRectangle(0,600,WIDTHSCREEN,120,BROWN);
                    
                    //barra de vida
                    DrawRectangle(10,10,player.health * 2,20,WHITE);
                    DrawRectangle(1080,10,enemy.health * 2,20,RED);

                    //Cronometro
                    char numeroString[20];
                    sprintf(numeroString, "%d", count);
                    DrawText(numeroString, WIDTHSCREEN/2, 10, 20, BLACK);


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