//Importação das bibliotecas utilizadas no Projeto
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

//Definindo as dimensões da tela(Resolução HD 1280x720)
//Estamos utilizando HD para maior compatibilidade com telas
#define WIDTHSCREEN 1280
#define HEIGHTSCREEN 720

//Definindo os estagios de transição de tela
//Posteriormente sera necesssario mais estados de tela
//Por exemplo, sera futuramente sera adicionado uma tela de escolha de personagem
typedef enum EstadosDeTela{Carregamento, Titulo, Gameplay, Opcoes, Final}EstadosDeTela;

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

//Struct de opções do menu, vira a ser mais conveniente manipular utilizando como structs
typedef struct {
  
    Rectangle rect;
    const char* text;
    Color textColor;
    Color rectColor;
  
} MenuItem;

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
    //Totalizando ate momento atual, dois textos
    Vector2 textPosition1 = {785.f, 130.f};
    Vector2 textPosition2 = {100,HEIGHTSCREEN/3};

    //Instalação da fonte do tipo ttf
    Font font = LoadFont("leadcoat.ttf");

    //Verificando se não ocorreu nenhum problema na instalação da fonte
    //Verificação semelhante a realizada em alocação dinamica
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
  
    //Definindo a quantidade de items do menu
    int numItemsMenu = 3;
  
    //Dentro do menu esta localiza a opção options
    //No options temos mais duas funcionalidades(por enquanto)
    int numItemsOptions = 2;
    MenuItem itemsMenu[] = {
            //Aqui a logica é a seguinte cada opção na tela é tratado como um retangulo
            //Assim é possivel manipular o click do usuario
            {{793, 300, 400, 40}, "Press Enter to battle!", WHITE, RED},
            {{930, 470, 200, 40}, "Options", WHITE, RED},
            {{930, 530, 200, 40}, "Controls", WHITE, RED},
    };

    MenuItem itemsOptions[] = {
        //Mesma logica, so que as funcionalidades do options
        {{560, 300, 200, 40}, "Sound", WHITE, RED},
        {{560, 360, 200, 40}, "Back", WHITE, RED}

    };

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
                //Estamos fazendo um loop para percorrer por todos os itens do Menu
                for(int i = 0; i < numItemsMenu; i++)
                {
                    //Note que para cada item do menu esta sendo verificado por meio do if se o Mouse esta Encima do botão
                    //Quando isso acontecer, o botão posteriormente mudara de coloração
                    //Percebemos isso utilizando a função CheckColissionPointRec
                    //Que basicamente verifica se um ponto esta sobre um retangulo(Nesse caso o ponto seria o mouse)
                    if(CheckCollisionPointRec(GetMousePosition(), itemsMenu[i].rect)) {
                        itemsMenu[i].rectColor = MAROON;
                      
                        //Se for verificado um clique, então ele mudara o seu estado de tela
                        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                          
                            if(i == 0) estadoTela = Gameplay;
                            else if(i == 1) estadoTela = Opcoes;
                          
                            //else if(i == x) continuar para as outras opções futuras... 
                        }
                    }
                }
            }break;
            //Fecha o jogo e vai para a tela final se o usuario digitar P
            //Cronometro de 120 segundos
            case Gameplay:
            {
                //Essa parte de baixo simula um cronometro
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
            case Opcoes:
            {
             
                int temSom = 1; //Flag utilizado para verificar a situação atual do som
              
                //itera sobre a quantidade de botões de options
                for(int i = 0; i < numItemsOptions; i++){
                  
                        //Utiliza a mesma logica para verificar a colisão do mouse com opção
                        //Pegando a coordenada do mouse com GetMousePosition()
                        if(CheckCollisionPointRec(GetMousePosition(), itemsOptions[i].rect)){
                            //Novamente a alteração de cor quando mouse esta encima
                            itemsOptions[i].rectColor = MAROON;
                         
                            //Verifica o click
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                              //Se a opção for a de numero 0, então é o botão de som
                                if(i == 0){ //Opção para o som
                                  
                                    //Se estiver ligado, desliga
                                    if(temSom) {
                                        SetMasterVolume(0.0);
                                        temSom = 0;
                                    }
                                    //Se estiver desligado, liga
                                    else {
                                        SetMasterVolume(1.0);
                                        temSom = 1;
                                    }
                                }
                                //Se a opção for 1, então é back, volta para o menu
                                else if(i == 1) estadoTela = Titulo; 
                            }
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
                    DrawTextEx(font,"This a loading screen. Wait for 2 seconds",textPosition2,70,2,WHITE);

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

                    //Titulo do jogo
                    DrawTextEx(font,"Bloody War",textPosition1,100,2,BLACK);
                  
                    //Opções
                    for(int i = 0; i < numItemsMenu; i ++) { 
                        DrawRectangleRec(itemsMenu[i].rect, itemsMenu[i].rectColor);
                        DrawText(itemsMenu[i].text, (int)(itemsMenu[i].rect.x + itemsMenu[i].rect.width / 2 - MeasureText(itemsMenu[i].text, 20) / 2), (int)(itemsMenu[i].rect.y + itemsMenu[i].rect.height / 2 - 10), 20, itemsMenu[i].textColor);
                    }

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
                case Opcoes:
                {
                    for(int i = 0; i < numItemsOptions; i ++) { 
                        DrawRectangleRec(itemsOptions[i].rect, itemsOptions[i].rectColor);
                        DrawText(itemsOptions[i].text, (int)(itemsOptions[i].rect.x + itemsOptions[i].rect.width / 2 - MeasureText(itemsOptions[i].text, 20) / 2), (int)(itemsOptions[i].rect.y + itemsOptions[i].rect.height / 2 - 10), 20, itemsOptions[i].textColor);
                    }
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