#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WIDTHSCREEN 1280
#define HEIGHTSCREEN 720
#define FATOR_REDUCAO 0.15
#define NUM_PERSONAGENS 2

//Definição dos estados de transição de tela do jogo
typedef enum EstadosDeTela {Carregamento, Titulo, Gameplay, Personagem, Mapa, Controles, Opcoes, Final}EstadosDeTela;

//Estrutura para os personagens com suas respectivas caracteristicas
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

//Estrutura para os projéteis com suas caracteristicas
typedef struct
{
    Character player;
    Rectangle Source;
    Rectangle Position;
    Vector2 speed;
    bool active;

} Projectile;

//Estrutura para os itens do menu
typedef struct {
  
    Rectangle rect;
    const char* text;
    Color textColor;
    Color rectColor;
  
} MenuItem;
// Função para atualizar a posição do personagem e mantê-lo dentro dos limites da tela
void atualizarPersonagem(Character *character, int sentido, int screenWidth){
    //Atualiza a posição do personagem
    character->posRect.x += sentido * 5;

    //Controle de movimentação do personagem dentro dos limites da tela
    if (character->posRect.x < 0)
        character->posRect.x = 0;
    if (character->posRect.x + character->posRect.width > WIDTHSCREEN)
        character->posRect.x = screenWidth - character->posRect.width;
}

// Função para desenhar um personagem
void desenharCharacter(Character *character, Texture2D texture) {
    DrawTexturePro(texture, character->SourceRect, character->posRect,(Vector2){0, 0}, 0, WHITE);
}
// Função para disparar um projétil
void dispararProjetil(Projectile *projetcile){

    //Verifica se o projetil não esta ativo antes de dispara-lo
    if (!projetcile->active) {
        //projetcile->Source.x = projetcile->Source.x
        //projetcile->Source.y = projetcile->Source.y;
        projetcile->active = true;
    }
}

void aplicarGravidade(Character *Character, int *countJump) {
    // Se o jogador estiver pulando atualize suas posições
    if (Character->isJumping) {
        Character->posRect.y -= Character->jumpSpeed;
        Character->jumpForce -= Character->jumpSpeed/1.7;
    
        // Verifica se o pulo terminou
        if (Character->jumpForce <= 0) {
            Character->isJumping = false;
        }
    } else {
        // Aplica a gravidade ao jogador
        if((Character->posRect.y + Character->posRect.height) < 650) {
            Character->posRect.y += 14;
        }
        else
            (*countJump) = 0;
    }
}

int main(void) {
    

    InitWindow(WIDTHSCREEN, HEIGHTSCREEN, "Bloody War");

    int controlHeight = 0;

    int TimeProjectile = 0;
    int TimeProjectile2 = 0;

    //Tempo de ataque
    double timeAttack = 0;
    double timeAttack2 = 0;

    //Contador para os sprites dos personagens
    int countSprite = 0;
    int countSprite2 = 0;

    //Variáveis para controlar o cronometro
    int counterFps = 0;
    int count = 120;
    int fpsAtual;

    // Definição da quantidade de items por menu
    int numItemsMenu = 3;
    int numItemsOptions = 2;

    //Variaveis para controlar os pulos dos personagens
    int countJump = 0;
    int countJump2 = 0;
    //variáveis de tempo de cooldown para cada personagem
    int cooldownPlayer = 0;
    int cooldownEnemy = 0;

    int cooldownProjectilePlayer = 0;
    int cooldownProjectileEnemy = 0;

    Character player = {{200, 300, 350, 350},{50 * countSprite + 24,88,50, 100},"Bloodthirsty",100, 3, 150, 0,20,0,false};
    Character enemy = {{900,300,350,350},{100 * countSprite2,130,100,136},"Warrior",100,2,150,0,20,0,false};

    Projectile projetcilePlayer = {player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
    Projectile projetcileEnemy = {enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};

    Rectangle DefaultSourcePlayer1 = player.SourceRect;
    Rectangle DefaultSourcePlayer2 = enemy.SourceRect;
    Rectangle DefaultPositionPlayer2 = enemy.posRect;

    Rectangle posRect2;

    //Posições dos textos do menu
    Vector2 textPosition1 = {785.f, 130.f};
    Vector2 textPosition2 = {100, HEIGHTSCREEN / 3};
    Vector2 textPosition3 = {400,610};

    // Instalação da fonte
    Font font = LoadFont("leadcoat.ttf");
    if (font.texture.id == 0) {
        TraceLog(LOG_WARNING, "Font could not be loaded! Exiting...");
        CloseWindow();
        return -1;
    }

    //Carregamento das imagens
    Image myImage = LoadImage("OIG_resized.png");
    Texture2D texture = LoadTextureFromImage(myImage);
    UnloadImage(myImage);

    Image myImage2 = LoadImage("Espadas_Duplas2.png");
    Texture2D texture2 = LoadTextureFromImage(myImage2);
    UnloadImage(myImage2);

    Image myImage3 = LoadImage("Mapa1.png");
    Texture2D texture3 = LoadTextureFromImage(myImage3);
    UnloadImage(myImage3);

    Image myImage4 = LoadImage("escudo.png");
    Texture2D texture4 = LoadTextureFromImage(myImage4);
    UnloadImage(myImage4);

    Image myImage5 = LoadImage("hero3.png");
    Texture2D texture5 = LoadTextureFromImage(myImage5);
    UnloadImage(myImage5);

    Image myImage6 = LoadImage("hero1.png");
    Texture2D texture6 = LoadTextureFromImage(myImage6);
    UnloadImage(myImage6);

    Image myImage7 = LoadImage("ballblue.png");
    Texture2D texture7 = LoadTextureFromImage(myImage7);
    UnloadImage(myImage7);

    Image myImage8 = LoadImage("ballred.png");
    Texture2D texture8 = LoadTextureFromImage(myImage8);
    UnloadImage(myImage8);

    Image myImage9 = LoadImage("fundoEscolha.png");
    Texture2D texture9 = LoadTextureFromImage(myImage9);
    UnloadImage(myImage9);

    Image myImage10 = LoadImage("gameOver.png");
    Texture2D texture10 = LoadTextureFromImage(myImage10);
    UnloadImage(myImage10);

    Image myImage11 = LoadImage("escolhaMapa.png");
    Texture2D texture11 = LoadTextureFromImage(myImage11);
    UnloadImage(myImage11);

    Image myImage12 = LoadImage("mapa2.png");
    Texture2D texture12 = LoadTextureFromImage(myImage12);
    UnloadImage(myImage12);

    Image myImage13 = LoadImage("mapa3.png");
    Texture2D texture13 = LoadTextureFromImage(myImage13);
    UnloadImage(myImage13);

    Texture2D texture14;

    // Estado inicial da tela
    EstadosDeTela estadoTela = Carregamento;
  
    //Definindo os botões do menu
    MenuItem itemsMenu[] = {
        {{793, 300, 400, 40}, "Click Here to battle!", WHITE, RED},
        {{900, 470, 200, 40}, "Options", WHITE, RED},
        {{900, 530, 200, 40}, "Controls", WHITE, RED},
    };

    //Definindo os botões dentro do sub-menu "Options"
    MenuItem itemsOptions[] = {
        {{560, 300, 200, 40}, "Sound", WHITE, RED},
        {{560, 360, 200, 40}, "Back", WHITE, RED}
    };

    while (!IsWindowReady()){}

    //Definindo 60 FPS
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        posRect2 = (Rectangle){player.posRect.x,player.posRect.y,100,100};
                
        switch(estadoTela) {
            case Carregamento:
            {
                //Tela de carregamento por 2 segundos
                counterFps++;
                if (counterFps > 120) {
                    estadoTela = Titulo;
                    counterFps = 0;
                } 
            } break;
            case Titulo:
            {
                //Para todo item do menu 
                for (int i = 0; i < numItemsMenu; i++){
                    //Verifica se o usuario selecionou alguma opção
                    if (CheckCollisionPointRec(GetMousePosition(), itemsMenu[i].rect)) {
                        
                        //Checa se o mouse esta encima do botão, se verdadeiro altera a cor para indicação
                        if(GetMouseX() == itemsMenu[i].rect.x || GetMouseY() == itemsMenu[i].rect.y)
                            itemsMenu[i].rectColor = MAROON;

                        //Verifica se alguma opção da janela de titulo foi selecionada
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            if (i == 0) estadoTela = Personagem;
                            else if (i == 1) estadoTela = Opcoes;
                            else if(i == 2) estadoTela = Controles;
                        }
                    }
                }
            } break;
            case Personagem:
            {
                if (IsKeyPressed(KEY_P))
                    estadoTela = Mapa;
            

            } break;
            case Mapa:
            {
                if (IsKeyPressed(KEY_P))
                    estadoTela = Gameplay;

                if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){150,400,250,250})){
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                       estadoTela = Gameplay;
                       texture14 = texture3;
                    }
                }
                if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){525,400,250,250})){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        estadoTela = Gameplay;
                        texture14 = texture12;
                    }
                }
                if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){925,400,250,250})){
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        estadoTela = Gameplay;
                        texture14 = texture13;
                    }
                }

            }break;
            case Gameplay:
            {
                //Se apertar P fecha o jogo
                if (IsKeyPressed(KEY_P))
                    estadoTela = Final;

                //Controle do cronometro
                counterFps++;
                if (counterFps == 120 || counterFps == fpsAtual + 60) {
                    fpsAtual = counterFps;
                    count--;
                }    
                if (counterFps == 7200)
                    estadoTela = Final;

                
                //Movimentação do personagem de numero 1
                if (IsKeyDown(KEY_D)){
                    atualizarPersonagem(&player, 1, WIDTHSCREEN);
                    countSprite = ((int)GetTime())%3;
                    player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                }else
                    player.SourceRect = (Rectangle){24,88, 50, 110};
                
                if (IsKeyDown(KEY_A)){
                    atualizarPersonagem(&player, -1, WIDTHSCREEN);
                    countSprite = ((int)GetTime())%3;
                    player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                }

                if (IsKeyPressed(KEY_C) && player.isAttacking == false && cooldownPlayer < GetTime()) {
                player.isAttacking = true;
                timeAttack = GetTime() + 0.2; 
                cooldownPlayer = GetTime() + 1;
                }
                if (player.isAttacking && GetTime() < timeAttack){
                // Desenhe a animação de ataque
                player.SourceRect = (Rectangle){325, 180, 70, 90};
                if (CheckCollisionRecs(enemy.posRect, player.posRect)) {
                    enemy.health -= 1;
                    
                }

                }
                else if(player.isAttacking && GetTime() >= timeAttack) {
                // Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                player.SourceRect = DefaultSourcePlayer1;
                player.isAttacking = false;
                }
                
                //Atualização da posição do projetil
                if (IsKeyPressed(KEY_X) && cooldownProjectilePlayer < GetTime()) {
                    dispararProjetil(&projetcilePlayer);
                    TimeProjectile = GetTime() + 2.0;
                    cooldownProjectilePlayer = GetTime() + 3;
                }

                //Se o projetil estiver ativo, tera uma taxa de atualização da posição X
                if (projetcilePlayer.active) {
                    projetcilePlayer.Source.x += projetcilePlayer.speed.x;

                    if(GetTime() < TimeProjectile)
                        player.SourceRect = (Rectangle){390,570,82,80};
                    else    
                        player.SourceRect = DefaultSourcePlayer1;

                    //Se o projeto ultrapassar os limites da tela, ele deixa de ser ativo
                    if (projetcilePlayer.Source.x > WIDTHSCREEN){
                        projetcilePlayer.active = false;
                        projetcilePlayer = (Projectile){player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
                    }
                    
                    if(CheckCollisionRecs(projetcilePlayer.Source,enemy.posRect)){
                        enemy.health -= player.attackDamage;
                        projetcilePlayer.active = false;
                        projetcilePlayer = (Projectile){player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
                    }
                    // Empurra o jogador para trás
                    if (enemy.posRect.x < player.posRect.x) {
                    player.posRect.x += 3; 
                     } else {
                    player.posRect.x -= 3;  
                    }
                    }
                if (IsKeyPressed(KEY_W) && !player.isJumping && countJump < 2) {
                    countJump += 1;
                    player.isJumping = true;
                    player.jumpForce = 150;
                }
                aplicarGravidade(&player, &countJump);


                //Controle do segundo personagem
                if (IsKeyDown(KEY_RIGHT)){
                    atualizarPersonagem(&enemy, 1, WIDTHSCREEN);
                    countSprite2 = ((int)GetTime())%3;
                    enemy.SourceRect = (Rectangle){100 * countSprite2,130,96,136};
                }else
                    enemy.SourceRect = (Rectangle){0,130,100,136};
                
                if (IsKeyDown(KEY_LEFT)){
                    atualizarPersonagem(&enemy, -1, WIDTHSCREEN);
                    countSprite2 = ((int)GetTime())%3;
                    enemy.SourceRect = (Rectangle){100 * countSprite2,130,96,136};
                }

                if (IsKeyPressed(KEY_M) && enemy.isAttacking == false && cooldownEnemy < GetTime()) {
                enemy.isAttacking = true;
                timeAttack2 = GetTime() + 0.5; 
                cooldownEnemy = GetTime() + 2;
                }
                if (enemy.isAttacking && GetTime() < timeAttack2) {
                // Desenhe a animação de ataque
                enemy.SourceRect = (Rectangle){925, 670, 95, 150};
                if(controlHeight < 1){
                    enemy.posRect.y = enemy.posRect.y - 179;
                    controlHeight++;
                }
                if (CheckCollisionRecs(enemy.posRect, player.posRect)) {
                    player.health -= 1;
                }
                }else if(enemy.isAttacking && GetTime() >= timeAttack2) {
                // Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                enemy.SourceRect = DefaultSourcePlayer2;
                enemy.posRect.y = DefaultPositionPlayer2.y;
                controlHeight = 0;
                enemy.isAttacking = false;
                }
                
                //Atualização da posição do projetil
                if (IsKeyPressed(KEY_N) && cooldownProjectileEnemy < GetTime()) {
                    dispararProjetil(&projetcileEnemy);
                    TimeProjectile2 = GetTime() + 2.0;
                    cooldownProjectileEnemy = GetTime() + 3;
                }
                //Se o projetil estiver ativo, tera uma taxa de atualização da posição X
                if (projetcileEnemy.active){
                    projetcileEnemy.Source.x -= projetcileEnemy.speed.x;

                    if(GetTime() < TimeProjectile2)
                        enemy.SourceRect = (Rectangle){820,286,140,110};
                    else    
                        enemy.SourceRect = DefaultSourcePlayer2;

                    //Se o projeto ultrapassar os limites da tela, ele deixa de ser ativo
                    if (projetcileEnemy.Source.x < 0){
                        projetcileEnemy.active = false;
                        projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                   }
                    
                    if(CheckCollisionRecs(projetcileEnemy.Source,posRect2)){
                        player.health -= enemy.attackDamage;
                        projetcileEnemy.active = false;
                        projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                    }
                }


                if (IsKeyPressed(KEY_UP) && !enemy.isJumping && countJump2 < 2) {
                    countJump2 += 1;
                    enemy.isJumping = true;
                    enemy.jumpForce = 150;
                }
                 if (CheckCollisionRecs(player.posRect, enemy.posRect)) {
        
                    player.velocity *= FATOR_REDUCAO;
                    enemy.velocity *= FATOR_REDUCAO;

                    // Ajustar as posições dos personagens para evitar sobreposição
                    if (player.posRect.x < enemy.posRect.x) {
                        player.posRect.x -= 1;
                        enemy.posRect.x += 1;
                    } else {
                        player.posRect.x += 1;
                        enemy.posRect.x -= 1;
                    }
                    if (player.health <= 0 || enemy.health <= 0) {
                        estadoTela = Final;
                    }
                 }
                 aplicarGravidade(&enemy, &countJump2);
                
            } break;  
            case Opcoes:
            {
                int temSom = 1;
                ///Percorrendo as sub-opções do options
                for (int i = 0; i < numItemsOptions; i++){
                    
                    if (CheckCollisionPointRec(GetMousePosition(), itemsOptions[i].rect)) {

                        //Checa se o mouse esta encima do botão, se verdadeiro altera a cor para indicação
                        if(GetMouseX() == itemsOptions[i].rect.x || GetMouseY() == itemsOptions[i].rect.y)
                            itemsOptions[i].rectColor = MAROON;

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            //Configuração do som
                            if (i == 0) {
                                if (temSom) {
                                    SetMasterVolume(0.0);
                                    temSom = 0;
                                } else {
                                    SetMasterVolume(1.0);
                                    temSom = 1;
                                }
                            } else if (i == 1) {
                                estadoTela = Titulo; 
                            }
                        }
                    }
                }
            } break;
            case Controles:
            {
                //P volta a tela de titulo
                if (IsKeyPressed(KEY_P))
                    estadoTela = Titulo;

            }break;
            case Final:
            {
                //Volta para o inicio quando se apertar enter na tela final
                if (IsKeyPressed(KEY_ENTER))
                    estadoTela = Titulo;
            } break;
            default: break;
              
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

        switch(estadoTela) {
            case Carregamento:
            {
                //Tela de esperapor 2 segundos
                DrawTexture(texture4, 500, 300, WHITE);
                DrawTextEx(font, "This a loading screen. Wait for 2 seconds", textPosition2, 70, 2, WHITE);
            } break;
            case Titulo:
            {
                //Desennhando as texturas
                DrawTexture(texture, 0, 0, WHITE);
                DrawTexture(texture2, 800, 345, WHITE);

                //Retangulos do titulo
                DrawRectangle(760, 40, 490, 240, RED);
                DrawRectangle(780, 65, 450, 190, MAROON);

                //Desenhando o titulo
                DrawTextEx(font, "Bloody War", textPosition1, 100, 2, BLACK);

                for (int i = 0; i < numItemsMenu; i++) { 
                    DrawRectangleRec(itemsMenu[i].rect, itemsMenu[i].rectColor);
                    DrawText(itemsMenu[i].text, (int)(itemsMenu[i].rect.x + itemsMenu[i].rect.width / 2 - MeasureText(itemsMenu[i].text, 20) / 2), (int)(itemsMenu[i].rect.y + itemsMenu[i].rect.height / 2 - 10), 20, itemsMenu[i].textColor);
                }
            }break;
            case Personagem:
            {
                
                DrawTexture(texture9, 0, -200, WHITE);
                DrawTextEx(font, "Escolha seu personagem", (Vector2){350, 100}, 60, 2, WHITE);

                DrawText("Bloodthirsty",150,300,40,WHITE);
                DrawText("Warrior",500,300,40,WHITE);
                DrawText("Fighter",850,300,40,WHITE);

                DrawTexturePro(texture5,(Rectangle){50 * 0 + 24,88,50, 100},(Rectangle){100, 400, 350, 350},(Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(texture6,(Rectangle){100 * 0,130,100,136},(Rectangle){450,400,350,350},(Vector2){0, 0}, 0, WHITE);

            }break;
            case Mapa:
            {
                DrawTexture(texture11, 0, 0, WHITE);
                DrawTextEx(font, "Escolha seu Mapa", (Vector2){420, 100}, 64, 2, WHITE);

                DrawTexturePro(texture3,(Rectangle){0,0,1280,633},(Rectangle){150,400,250,250},(Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(texture12,(Rectangle){0,0,1280,633},(Rectangle){525,400,250,250},(Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(texture13,(Rectangle){0,0,1284,619},(Rectangle){925,400,250,250},(Vector2){0, 0}, 0, WHITE);

            }break;
            case Gameplay:
            {
                
                //Textura de fundo
                DrawTexture(texture14, 0, 0, WHITE);
                desenharCharacter(&player,texture5);
                desenharCharacter(&enemy,texture6);
                
                //Desenhando a base(piso) dos personagems
                DrawRectangle(0, 600, WIDTHSCREEN, 120, DARKBROWN);

                //Desenhando o titulo
                DrawTextEx(font, "Bloody War", textPosition3, 100, 2, RED);

                //Barra de vida dos personagens
                DrawRectangle(10, 10, player.health * 2, 20, WHITE);
                DrawRectangle(1080, 10, enemy.health * 2, 20, RED);

                //Cronometro
                char numeroString[20];
                sprintf(numeroString, "%d", count);
                DrawText(numeroString, WIDTHSCREEN / 2, 10, 20, BLACK);

                //Desenha os nomes dos personagens 
                DrawText(player.name, 20, 70, 40, MAROON);
                DrawText(enemy.name, 1020, 70, 40, WHITE);

                //Desenhando o projetil caso esteja ativo
                if (projetcilePlayer.active == true) {
                   DrawTexturePro(texture7,projetcilePlayer.Position,projetcilePlayer.Source,(Vector2){0, 0}, 0, WHITE);
                }
                if (projetcileEnemy.active == true) {
                   DrawTexturePro(texture8,projetcileEnemy.Position,projetcileEnemy.Source,(Vector2){0, 0}, 0, WHITE);
                }
                if (player.health <= 0 || enemy.health <= 0) {
                    estadoTela = Final;
                }

            } break;
            case Opcoes:
            {
                for (int i = 0; i < numItemsOptions; i++){ 
                    DrawRectangleRec(itemsOptions[i].rect, itemsOptions[i].rectColor);
                    DrawText(itemsOptions[i].text, (int)(itemsOptions[i].rect.x + itemsOptions[i].rect.width / 2 - MeasureText(itemsOptions[i].text, 20) / 2), (int)(itemsOptions[i].rect.y + itemsOptions[i].rect.height / 2 - 10), 20, itemsOptions[i].textColor);
                }
            } break;
            case Controles:
            {
                ClearBackground(BLACK);

                //Descrevendo os controles basicos
                DrawText("Player 1 Controls:",100,100,30,WHITE);
                DrawText("D -> move to the right | A -> move to the left",100,150,30,WHITE);
                DrawText("W -> to jump",100,200,30,WHITE);
                DrawText("C -> hand-to-hand attack | X -> shoot projectile ",100,250,30,WHITE);

                DrawText("Player 2 Controls:",100,350,30,GRAY);
                DrawText("Right arrow -> move to the right | Left arrow -> move to the left",100,400,30,GRAY);
                DrawText("Up arrow -> to jump",100,450,30,GRAY);
                DrawText("M -> hand-to-hand attack | N -> shoot projectile ",100,500,30,GRAY);

            }break;
            case Final:
            {
                ClearBackground(BLACK);
                DrawTexture(texture10,300,0, WHITE);
                DrawText("Digite Enter To Back Menu",250,500,50,WHITE);
                if (IsKeyPressed(KEY_ENTER))
                estadoTela = Titulo;
    
            }break;
            default: break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
} 