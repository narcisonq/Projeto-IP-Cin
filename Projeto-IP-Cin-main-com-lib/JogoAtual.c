#include "nossalib.h"

int main(void) {
    
    InitWindow(WIDTHSCREEN, HEIGHTSCREEN, "Bloody War");
    InitAudioDevice();
    initAudios();

    //Variavel para os ganhos aleatorios de bonus de vida
    int luckyLifePlayer = 0;
    int luckyLifePlayer2 = 0;
    int luckyLifeEnemy = 0;

    //Variavel pra determinar quem venceu
    int whoWins=0;

    //Variaveis de controle para o shield do segundo personagem
    bool Shield = false;
    int timeShield = 0;
    int cooldownShield = 0;
    int vidaInicial = 0;

    //Variaveis referente a escolha de personagem
    int player1Active = 0;
    int player2Active = 0;

    //Variavel de controle de altura
    int controlHeight = 0;

    //Variavel para simular um cooldown do projetil
    int TimeProjectile = 0;
    int TimeProjectileEnemy = 0;

    //Tempo de ataque dos personagens
    double timeAttack = 0;
    double timeAttackEnemy1 = 0;
    double timeAttackPlayer2 = 0;

    //Contador para os sprites dos personagens
    int countSprite = 0;
    int countSpriteEnemy = 0;
    int countSpritePlayer2 = 0;

    //Variáveis para controlar o cronometro
    int counterFps = 0;
    int count = 120;
    int fpsAtual;

    // Definição da quantidade de items por menu
    int numItemsMenu = 4;
    int numItemsOptions = 2;

    //Variaveis para controlar os pulos dos personagens
    int countJump = 0;
    int countJumpEnemy = 0;
    int countJumpPlayer2 = 0;

    //variáveis de tempo de cooldown para cada personagem
    int cooldownPlayer = 0;
    int cooldownEnemy = 0;
    int cooldownPlayer2 = 0;

    //variaveis de cooldown dos personagens
    int cooldownProjectilePlayer = 0;
    int cooldownProjectileEnemy = 0;

    Texture2D vetor[3];
    vetor[0] = LoadTexture("Mapa1.png");
    vetor[1] = LoadTexture("mapa2.png");
    vetor[2] = LoadTexture("mapa3.png");

    int currentTextureIndex = 0;
    int selectedMapIndex = 0; // Store the index of the selected map
    int transitionOffset = 0;
    int sentido = 0;

    //Personagens
    Character player = {{200, 300, 350, 350},{50 * countSprite + 24,88,50, 100},"Bloodthirsty",250, 12, 150, 5,20,0,false};
    Character enemy = {{900,300,350,350},{100 * countSpriteEnemy,130,100,136},"Warrior",250,10,150,3,20,0,false};
    Character player2 = {{200,250,350,350},{80 * countSpritePlayer2 + 80,130,78,90},"Monster",250,10,130,5,20,0,false};

    //Projeteis
    Projectile projetcilePlayer = {player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
    Projectile projetcileEnemy = {enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};

    //Posição padrão dos personagens
    Rectangle DefaultSourcePlayer1 = player.SourceRect;
    Rectangle DefaultSourcePlayer2 = enemy.SourceRect;
    Rectangle DefaultSourcePlayer3 = player2.SourceRect;
    Rectangle DefaultPositionPlayer2 = enemy.posRect;

    Rectangle posRect2;
    Rectangle posRect3;

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

    Texture2D texture = LoadTexture("OIG_resized.png");
    Texture2D texture2 = LoadTexture("Espadas_Duplas2.png");
    Texture2D texture3 = LoadTexture("Mapa1.png");
    Texture2D texture4 = LoadTexture("escudo.png");
    Texture2D texture5 = LoadTexture("hero3.png");
    Texture2D texture6 = LoadTexture("hero1.png");
    Texture2D texture7 = LoadTexture("ballblue.png");
    Texture2D texture8 = LoadTexture("ballred.png");
    Texture2D texture9 = LoadTexture("fundoEscolha.png");
    Texture2D texture10 = LoadTexture("gameOver.png");
    Texture2D texture11 = LoadTexture("escolhaMapa.png");
    Texture2D texture12 = LoadTexture("mapa2.png");
    Texture2D texture13 = LoadTexture("mapa3.png");
    Texture2D texture14;
    Texture2D texture15 = LoadTexture("hero2.png");
    
    // Estado inicial da tela
    EstadosDeTela estadoTela = Carregamento;
  
    //Definindo os botões do menu
    MenuItem itemsMenu[] = {
        {{793, 300, 400, 40}, "Mode 2 Players", WHITE, RED},
        {{793,350,400,40},"Mode SinglePlayer",WHITE,RED},
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

    while (!WindowShouldClose()){

        if(player1Active)
            posRect2 = (Rectangle){player.posRect.x,player.posRect.y,100,100};
        if(player2Active)
            posRect3 = (Rectangle){player2.posRect.x - 100,player.posRect.y,100,100};
                
        switch(estadoTela) {
            case Carregamento:
            {
                //Tela de carregamento por 2 segundos
                counterFps++;
                if (counterFps > 120) {
                    estadoTela = Titulo;
                    counterFps = 0;
                    tocaAudio(1);
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
                            PlaySound(fxSelecao);
                            if (i == 0) estadoTela = Personagem;
                                else if(i == 1){ 
                                    estadoTela = Solo;
                                    tocaAudio(2);
                                }
                                    else if (i == 2) estadoTela = Opcoes;
                                        else if(i == 3) estadoTela = Controles;
                        }
                    }
                }
            } break;
            case Personagem:
            {
                if (IsKeyPressed(KEY_P)) {
                    PlaySound(fxSelecao);
                    estadoTela = Mapa;
                }
                
                if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){100, 400, 350, 350})){
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(fxSelecao);
                        estadoTela = Mapa;
                        player1Active = 1;
                    }
                }
                if(CheckCollisionPointRec(GetMousePosition(),(Rectangle){450,330,350,350})){
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        PlaySound(fxSelecao);
                        estadoTela = Mapa;
                        player2Active = 1;
                    }
                }

            } break;
            case Mapa:{
                PerformTransition(vetor, &currentTextureIndex, &transitionOffset, &sentido, WIDTHSCREEN);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    // Store the index of the selected map
                    selectedMapIndex = currentTextureIndex;
                    estadoTela = Gameplay; // Change state to Gameplay when the left mouse button is pressed
                }

            }break;
            case Solo:
            {
                //Se apertar P fecha o jogo
                if (IsKeyPressed(KEY_P)) {
                    tocaAudio(3);
                    estadoTela = Final;
                }

                //Controle do cronometro
                counterFps++;
                if (counterFps == 120 || counterFps == fpsAtual + 60) {
                    fpsAtual = counterFps;
                    count--;
                }    
                if (counterFps == 7200) {
                    count = 120;
                    tocaAudio(3);
                    estadoTela = Final;
                }

                player1Active = 1;
                if(rand()%1000 == 6 && luckyLifePlayer == 0 && player.health < 220){
                        luckyLifePlayer = 1;
                }
                if(IsKeyPressed(KEY_E) && luckyLifePlayer == 1){
                    PlaySound(fxVidaExtra);
                    player.health = player.health + 30;
                    luckyLifePlayer = 0;
                }
                if (IsKeyDown(KEY_D)){
                    atualizarPersonagem(&player, 1, WIDTHSCREEN);
                    countSprite = ((int)GetTime())%3;
                    player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                }else
                    player.SourceRect = (Rectangle){24,88, 50, 110};
                
                if(IsKeyDown(KEY_A)){
                    atualizarPersonagem(&player, -1, WIDTHSCREEN);
                    countSprite = ((int)GetTime())%3;
                    player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                }

                if(IsKeyPressed(KEY_C) && player.isAttacking == false && cooldownPlayer < GetTime()) {
                    player.isAttacking = true;
                    timeAttack = GetTime() + 0.2; 
                    cooldownPlayer = GetTime() + 1;
                }

                if(player.isAttacking && GetTime() < timeAttack){
                    // Desenhe a animação de ataque
                    player.SourceRect = (Rectangle){325, 180, 70, 90};
                    if(CheckCollisionRecs(enemy.posRect, player.posRect)) {
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
                        enemy.health -= 2 * player.attackDamage;
                        projetcilePlayer.active = false;
                        projetcilePlayer = (Projectile){player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
                    }
                    // Empurra o jogador para trás
                    if (enemy.posRect.x < player.posRect.x) {
                        player.posRect.x += 3; 
                    }else
                        player.posRect.x -= 3;  
                    
                }
                if(IsKeyPressed(KEY_W) && !player.isJumping && countJump < 2) {
                    PlaySound(fxPulo);
                    countJump += 1;
                    player.isJumping = true;
                    player.jumpForce = 150;
                }
                aplicarGravidade(&player, &countJump,0);

                //CONTROLE DO BOOT!!!
                //------------------//
                if((abs(player.posRect.x - enemy.posRect.x) < 300) && player.posRect.x > enemy.posRect.x){           
                    atualizarPersonagem(&enemy,1,WIDTHSCREEN);
                    if((abs(player.posRect.x - enemy.posRect.x) < 200)){
                        if(enemy.isAttacking == false && cooldownEnemy < GetTime()) {
                            enemy.isAttacking = true;
                            timeAttackEnemy1 = GetTime() + 0.5; 
                            cooldownEnemy = GetTime() + 2;
                        }
                        if(enemy.isAttacking && GetTime() < timeAttackEnemy1) {
                        enemy.SourceRect = (Rectangle){925, 670, 95, 150};
                            if(controlHeight < 1){
                                enemy.posRect.y = enemy.posRect.y - 179;
                                controlHeight++;
                            }
                            if(CheckCollisionRecs(enemy.posRect, player.posRect)) 
                                player.health -= 1;
                        }else if(enemy.isAttacking && GetTime() >= timeAttackEnemy1) {
                        // Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                            enemy.SourceRect = DefaultSourcePlayer2;
                            enemy.posRect.y = DefaultPositionPlayer2.y;
                            controlHeight = 0;
                            enemy.isAttacking = false;
                        }
                    }
                }
                else if((abs(player.posRect.x - enemy.posRect.x) < 300) && player.posRect.x < enemy.posRect.x){           
                    atualizarPersonagem(&enemy,-1,WIDTHSCREEN);
                    if((abs(player.posRect.x - enemy.posRect.x) < 200)){
                        if(enemy.isAttacking == false && cooldownEnemy < GetTime()) {
                            enemy.isAttacking = true;
                            timeAttackEnemy1 = GetTime() + 0.5; 
                            cooldownEnemy = GetTime() + 2;
                        }
                        if(enemy.isAttacking && GetTime() < timeAttackEnemy1) {
                         enemy.SourceRect = (Rectangle){925, 670, 95, 150};
                            if(controlHeight < 1){
                                enemy.posRect.y = enemy.posRect.y - 39;
                            controlHeight++;
                            }
                            if(CheckCollisionRecs(enemy.posRect, player.posRect)) 
                                player.health -= 1;
                        }else if(enemy.isAttacking && GetTime() >= timeAttackEnemy1) {
                            //Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                            enemy.SourceRect = DefaultSourcePlayer2;
                            enemy.posRect.y = DefaultPositionPlayer2.y;
                            controlHeight = 0;
                            enemy.isAttacking = false;
                        }
                    }
                }
                if((abs(projetcilePlayer.Source.x - enemy.posRect.x) < 330) && (abs(projetcilePlayer.Source.y - enemy.posRect.y) < 330) && projetcilePlayer.active){
                    if(!enemy.isJumping && countJumpEnemy < 2) {
                        countJumpEnemy += 1;
                        enemy.isJumping = true;
                        enemy.jumpForce = 150;
                    }
                }
                if((abs(player.posRect.x - enemy.posRect.x) > 300)){
                   
                    if((rand()%20 == 4) && cooldownProjectileEnemy < GetTime()) {
                    dispararProjetil(&projetcileEnemy);
                    TimeProjectileEnemy = GetTime() + 1.0;
                    cooldownProjectileEnemy = GetTime() + 5;
                }
                //Se o projetil estiver ativo, tera uma taxa de atualização da posição X
                if (projetcileEnemy.active){
                    projetcileEnemy.Source.x -= projetcileEnemy.speed.x;

                    if(GetTime() < TimeProjectileEnemy)
                        enemy.SourceRect = (Rectangle){820,286,140,110};
                    else    
                        enemy.SourceRect = DefaultSourcePlayer2;

                    //Se o projeto ultrapassar os limites da tela, ele deixa de ser ativo
                    if(projetcileEnemy.Source.x < 0){
                        projetcileEnemy.active = false;
                        projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                
                    }
                    if(CheckCollisionRecs(projetcileEnemy.Source,posRect2)){
                        player.health -= enemy.attackDamage;
                        projetcileEnemy.active = false;
                        projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                    
                    }
                }
            }
                aplicarGravidade(&enemy, &countJumpEnemy,0);

                if (player.health <= 0 || enemy.health <= 0) {
                    if(player.health > enemy.health)
                        whoWins = 1;
                    else if(player.health < enemy.health)   
                        whoWins = 2;

                    estadoTela = Final;
                    tocaAudio(3);
                }
            }
            break;
            case Gameplay:
            {
                //Se apertar P fecha o jogo
                if (IsKeyPressed(KEY_P)) {
                    estadoTela = Final;
                    tocaAudio(3);
                }

                //Controle do cronometro
                counterFps++;
                if (counterFps == 120 || counterFps == fpsAtual + 60) {
                    fpsAtual = counterFps;
                    count--;
                }    
                if (counterFps == 7200) {
                    estadoTela = Final;
                    count = 120;
                    tocaAudio(3);
                }

                if(player1Active){
                    if(rand()%1000 == 6 && luckyLifePlayer == 0 && player.health < 220){
                        luckyLifePlayer = 1;
                    }
                    if(IsKeyPressed(KEY_E) && luckyLifePlayer == 1){
                            PlaySound(fxVidaExtra);
                            player.health = player.health + 30;
                            luckyLifePlayer = 0;
                    }
                    //Movimentação do personagem de numero 1
                    if (IsKeyDown(KEY_D)){
                        atualizarPersonagem(&player, 1, WIDTHSCREEN);
                        countSprite = ((int)GetTime())%3;
                        player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                    }else
                        player.SourceRect = (Rectangle){24,88, 50, 110};
                
                    if(IsKeyDown(KEY_A)){
                        atualizarPersonagem(&player, -1, WIDTHSCREEN);
                        countSprite = ((int)GetTime())%3;
                        player.SourceRect = (Rectangle){50 * countSprite + 24,88, 50, 100};
                    }
                    if(IsKeyPressed(KEY_C) && player.isAttacking == false && cooldownPlayer < GetTime()) {
                        player.isAttacking = true;
                        timeAttack = GetTime() + 0.2; 
                        cooldownPlayer = GetTime() + 1;
                    }
                    if (player.isAttacking && GetTime() < timeAttack){
                        //Desenhe a animação de ataque
                        player.SourceRect = (Rectangle){325, 180, 70, 90};
                        if (CheckCollisionRecs(enemy.posRect, player.posRect)) {
                            PlaySound(fxSoco);
                            enemy.health -= 1;
                        }
                    }
                    else if(player.isAttacking && GetTime() >= timeAttack) {
                        //Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                        player.SourceRect = DefaultSourcePlayer1;
                        player.isAttacking = false;
                    }
                    //Atualização da posição do projetil
                    if(IsKeyPressed(KEY_X) && cooldownProjectilePlayer < GetTime()) {
                        dispararProjetil(&projetcilePlayer);
                        PlaySound(fxHadouken);
                        TimeProjectile = GetTime() + 2.0;
                        cooldownProjectilePlayer = GetTime() + 3;
                    }
                    //Se o projetil estiver ativo, tera uma taxa de atualização da posição X
                    if(projetcilePlayer.active) {
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
                            enemy.health -= 2 * player.attackDamage;
                            projetcilePlayer.active = false;
                            projetcilePlayer = (Projectile){player,{player.posRect.x + 150,player.posRect.y + 30,100,100 },{200,120,490,420} ,(Vector2){15, 0}, false};
                        }
                        //Empurra o jogador para trás
                        if(enemy.posRect.x < player.posRect.x) {
                        player.posRect.x += 3; 
                        }else
                    player.posRect.x -= 3;  
                }
                if (IsKeyPressed(KEY_W) && !player.isJumping && countJump < 2) {
                    PlaySound(fxPulo);
                    countJump += 1;
                    player.isJumping = true;
                    player.jumpForce = 150;
                }
                aplicarGravidade(&player, &countJump,0);
                //Fim do personagem 1*/
            }

                if(player2Active){
                    if(rand()%1000 == 6 && luckyLifePlayer2 == 0 && player2.health < 220){
                        luckyLifePlayer2 = 1;
                    }
                    if(IsKeyPressed(KEY_E) && luckyLifePlayer2 == 1){
                            PlaySound(fxVidaExtra);
                            player2.health = player2.health + 30;
                            luckyLifePlayer2 = 0;
                    }
                    //Movimentação do personagem de numero 2
                    if(IsKeyDown(KEY_D)){
                        atualizarPersonagem(&player2, 1, WIDTHSCREEN);
                        countSpritePlayer2 = ((int)GetTime())%2;
                        player2.SourceRect = (Rectangle){80 * countSpritePlayer2 + 80,130,78,90};
                    }else
                        player2.SourceRect = (Rectangle){0,130,70,90};
                
                    if(IsKeyDown(KEY_A)){
                        atualizarPersonagem(&player2, -1, WIDTHSCREEN);
                        countSpritePlayer2 = ((int)GetTime())%2;
                        player2.SourceRect = (Rectangle){80 * countSpritePlayer2 + 80,130,78,90};
                    }

                    if(IsKeyPressed(KEY_C) && player2.isAttacking == false && cooldownPlayer2 < GetTime()) {
                        player2.isAttacking = true;
                        timeAttackPlayer2 = GetTime() + 0.2; 
                        cooldownPlayer2 = GetTime() + 1;
                    }
                    if(player2.isAttacking && GetTime() < timeAttackPlayer2){
                        //Desenhe a animação de ataque
                        player2.SourceRect = (Rectangle){280, 250, 94, 130};
                        if(CheckCollisionRecs(enemy.posRect, player2.posRect)) {
                            PlaySound(fxSoco);
                            enemy.health -= 1;
                        }
                    }
                    else if(player2.isAttacking && GetTime() >= timeAttackPlayer2) {
                        //Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                        player2.SourceRect = DefaultSourcePlayer3;
                        player2.isAttacking = false;
                    }
                    if(IsKeyPressed(KEY_W) && !player2.isJumping && countJumpPlayer2 < 2) {
                        PlaySound(fxPulo);
                        countJumpPlayer2 += 1;
                        player2.isJumping = true;
                        player2.jumpForce = 150;
                    }

                    if(IsKeyPressed(KEY_X) && Shield == false && cooldownShield < GetTime()){
                        Shield = true;
                        timeShield = GetTime() + 2;
                        cooldownShield = GetTime() + 2;
                        vidaInicial = player2.health;
                    }
                    if(Shield && GetTime() < timeShield){
                        player2.SourceRect = (Rectangle){90,255,65,128};
                    }
                    else if(Shield == true && GetTime() >= timeShield) {
                        //Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                        player2.SourceRect = DefaultSourcePlayer3;
                        Shield = false;
                        player2.health = vidaInicial;
                    }

                    aplicarGravidade(&player2, &countJumpPlayer2,1);
                    //Fim Controle do segundo personagem
                }

                if((rand()%1000 == 6) && luckyLifeEnemy == 0 && enemy.health < 220){
                    luckyLifeEnemy = 1;
                }
                if(IsKeyPressed(KEY_J) && luckyLifeEnemy == 1){
                    PlaySound(fxVidaExtra);
                    enemy.health = enemy.health + 30;
                    luckyLifeEnemy = 0;
                }
                if (IsKeyDown(KEY_RIGHT)){
                    atualizarPersonagem(&enemy, 1, WIDTHSCREEN);
                    countSpriteEnemy = ((int)GetTime())%3;
                    enemy.SourceRect = (Rectangle){100 * countSpriteEnemy,130,96,136};
                }else
                    enemy.SourceRect = (Rectangle){0,130,100,136};
                
                if (IsKeyDown(KEY_LEFT)){
                    atualizarPersonagem(&enemy, -1, WIDTHSCREEN);
                    countSpriteEnemy = ((int)GetTime())%3;
                    enemy.SourceRect = (Rectangle){100 * countSpriteEnemy,130,96,136};
                }

                if(IsKeyPressed(KEY_M) && enemy.isAttacking == false && cooldownEnemy < GetTime()) {
                    enemy.isAttacking = true;
                    timeAttackEnemy1 = GetTime() + 0.5; 
                    cooldownEnemy = GetTime() + 2;
                }
                if(enemy.isAttacking && GetTime() < timeAttackEnemy1) {
                    //Desenhe a animação de ataque
                    enemy.SourceRect = (Rectangle){925, 670, 95, 150};
                    if(controlHeight < 1){
                        enemy.posRect.y = enemy.posRect.y - 179;
                        controlHeight++;
                    }
                    if(player1Active){
                        if(CheckCollisionRecs(enemy.posRect, player.posRect)) {
                            player.health -= 1;
                            PlaySound(fxSoco);
                        }
                    }else if(player2Active){
                        if(CheckCollisionRecs(enemy.posRect, player2.posRect)) {
                            player2.health -= 1;
                            PlaySound(fxSoco);
                    }
                }
                }else if(enemy.isAttacking && GetTime() >= timeAttackEnemy1) {
                    // Se não estiver atacando ou o tempo de ataque acabar, volte para a animação padrão
                    enemy.SourceRect = DefaultSourcePlayer2;
                    enemy.posRect.y = DefaultPositionPlayer2.y;
                    controlHeight = 0;
                    enemy.isAttacking = false;
                }
                //Atualização da posição do projetil
                if(IsKeyPressed(KEY_N) && cooldownProjectileEnemy < GetTime()) {
                    dispararProjetil(&projetcileEnemy);
                    PlaySound(fxHadouken);
                    TimeProjectileEnemy = GetTime() + 2.0;
                    cooldownProjectileEnemy = GetTime() + 3;
                }
                //Se o projetil estiver ativo, tera uma taxa de atualização da posição X
                if (projetcileEnemy.active){
                    projetcileEnemy.Source.x -= projetcileEnemy.speed.x;

                    if(GetTime() < TimeProjectileEnemy)
                        enemy.SourceRect = (Rectangle){820,286,140,110};
                    else    
                        enemy.SourceRect = DefaultSourcePlayer2;

                    //Se o projeto ultrapassar os limites da tela, ele deixa de ser ativo
                    if(projetcileEnemy.Source.x < 0){
                        projetcileEnemy.active = false;
                        projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                
                    }
                    
                    if(player1Active){
                        if(CheckCollisionRecs(projetcileEnemy.Source,posRect2)){
                            player.health -= enemy.attackDamage;
                            projetcileEnemy.active = false;
                            projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                    
                        }
                    }else if(player2Active){
                        if(CheckCollisionRecs(projetcileEnemy.Source,posRect3)){
                            player2.health -= 2 * enemy.attackDamage;
                            projetcileEnemy.active = false;
                            projetcileEnemy = (Projectile){enemy,{enemy.posRect.x,enemy.posRect.y,100,100 },{0,0,220,220},{10, 0}, false};                    
                        }
                    }
                }
                if(IsKeyPressed(KEY_UP) && !enemy.isJumping && countJumpEnemy < 2) {
                    PlaySound(fxPulo);
                    countJumpEnemy += 1;
                    enemy.isJumping = true;
                    enemy.jumpForce = 150;
                }
                if(CheckCollisionRecs(player.posRect, enemy.posRect)) {

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
                        tocaAudio(3);
                    }
                }
                if(CheckCollisionRecs(player2.posRect, enemy.posRect)) {

                    // Ajustar as posições dos personagens para evitar sobreposição
                    if (player2.posRect.x < enemy.posRect.x) {
                        player2.posRect.x -= 1;
                        enemy.posRect.x += 1;
                    } else {
                        player2.posRect.x += 1;
                        enemy.posRect.x -= 1;
                    }
                    if (player2.health <= 0 || enemy.health <= 0) {
                        estadoTela = Final;
                        tocaAudio(3);
                    }
                }
                aplicarGravidade(&enemy, &countJumpEnemy,0);

                if (player.health <= 0 || enemy.health <= 0 || player2.health <= 0){
                    if(player.health > enemy.health && player1Active)
                        whoWins = 1;
                    else if(player2.health > enemy.health && player2Active)
                        whoWins = 1;
                    else if(player.health < enemy.health && player1Active)
                        whoWins = 3;
                    else if(player2.health < enemy.health && player2Active)
                        whoWins = 3;

                    estadoTela = Final;
                    tocaAudio(3);
                }
            } break;
            case Opcoes: 
            {
                ///Percorrendo as sub-opções do options
                //P volta a tela de titulo
                if (IsKeyPressed(KEY_P)) {
                    estadoTela = Titulo;
                    tocaAudio(1);
                }

                for (int i = 0; i < numItemsOptions; i++){
                    
                    if (CheckCollisionPointRec(GetMousePosition(), itemsOptions[i].rect)) {

                        //Checa se o mouse esta encima do botão, se verdadeiro altera a cor para indicação
                        if(GetMouseX() == itemsOptions[i].rect.x || GetMouseY() == itemsOptions[i].rect.y)
                            itemsOptions[i].rectColor = MAROON;

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            //Liga/desliga audio do jogo
                            if(i == 0) {
                                audioLigado = !audioLigado; //Invertendo estado do audio
                                if(audioLigado) 
                                    SetMasterVolume(1.0f);
                                else 
                                    SetMasterVolume(0.0f);
                            }

                            else if(i == 1) {
                                estadoTela = Titulo;
                            }
                        }
                    }
                    else {
                        itemsOptions[i].rectColor = GRAY;
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
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoTela = Titulo;
                    tocaAudio(1);
                }

                player1Active = 0;
                player2Active = 0;

                player.health = 250;
                player2.health = 250;
                enemy.health = 250;

                player.posRect = (Rectangle){200, 300, 350, 350};
                player2.posRect = (Rectangle){200,250,350,350};
                enemy.posRect = (Rectangle){900,300,350,350};
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
                DrawTextEx(font, "Escolha seu personagem", (Vector2){150, 100}, 60, 2, WHITE);

                DrawText("Bloodthirsty",150,300,40,WHITE);
                DrawText("Warrior",500,300,40,WHITE);
                DrawText("Oponente",900,300,40,WHITE);

                DrawTexturePro(texture5,(Rectangle){50 * 0 + 24,88,50, 100},(Rectangle){100, 400, 350, 350},(Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(texture6,(Rectangle){100 * 0,130,100,136},(Rectangle){900,400,350,350},(Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(texture15,(Rectangle){70 * 0,130,70,90},(Rectangle){450,330,350,350},(Vector2){0, 0}, 0, WHITE);
            }break;
            case Mapa:
            {
                 DrawTexturePro(vetor[currentTextureIndex], (Rectangle){0, 0, 1280, 620}, (Rectangle){0, 0, WIDTHSCREEN, HEIGHTSCREEN}, (Vector2){0, 0}, 0, WHITE);

                // **Transition Animation:**
                if (transitionOffset != 0){
                    
                    float progress = (float)abs(transitionOffset) / WIDTHSCREEN; // Calculate transition progress
                    if(sentido == 1){
                    // Draw the previous image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){WIDTHSCREEN * progress * sentido, 0, WIDTHSCREEN, HEIGHTSCREEN}, (Vector2){0, 0}, 0, WHITE);

                    // Draw the next image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex + 2) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){-WIDTHSCREEN * (1 - progress) * sentido, 0, WIDTHSCREEN, HEIGHTSCREEN}, (Vector2){0, 0}, 0, WHITE);
                    }
                    else if(sentido == -1){
                     DrawTexturePro(vetor[(currentTextureIndex) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){WIDTHSCREEN * progress * sentido, 0, WIDTHSCREEN, HEIGHTSCREEN}, (Vector2){0, 0}, 0, WHITE);

                    // Draw the next image with offset based on progress
                    DrawTexturePro(vetor[(currentTextureIndex + 1) % 3], (Rectangle){0, 0, 1280, 620}, (Rectangle){-WIDTHSCREEN * (1 - progress) * sentido, 0, WIDTHSCREEN, HEIGHTSCREEN}, (Vector2){0, 0}, 0, WHITE);
                    }
                    
                }

                DrawText("use as setas pra escolher o mapa", WIDTHSCREEN / 3.5 , 15, 30 ,WHITE);

            }break;

            case Solo:
            {
                DrawTexture(texture3, 0, 0, WHITE);
                desenharCharacter(&enemy,texture6);
                desenharCharacter(&player,texture5);

                if(luckyLifePlayer == 1){
                    DrawText("PRESS E TO GAIN LIFE BONUS",300,180,20,YELLOW);
                }

                //Desenhando a base(piso) dos personagems
                DrawRectangle(0, 600, WIDTHSCREEN, 120, DARKBROWN);

                DrawTextEx(font, "Bloody War", textPosition3, 100, 2, RED);

                DrawRectangle(10, 10, player.health * 2, 20, WHITE);
                DrawRectangle(770, 10, enemy.health * 2, 20, RED);

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

            }break;
            case Gameplay:
            {
                
                //Textura de fundo
                DrawTexture(vetor[selectedMapIndex], 0, 0, WHITE);
                desenharCharacter(&enemy,texture6);
                if(player1Active)
                    desenharCharacter(&player,texture5);
                if(player2Active)
                    desenharCharacter(&player2,texture15);

                if(luckyLifePlayer == 1 || luckyLifePlayer2 == 1){
                    DrawText("PRESS E TO GAIN LIFE BONUS",300,180,20,YELLOW);
                }
                if(luckyLifeEnemy == 1){
                    DrawText("PRESS J TO GAIN LIFE BONUS",900,180,20,YELLOW);
                }
                
                //Desenhando a base(piso) dos personagems
                DrawRectangle(0, 600, WIDTHSCREEN, 120, DARKBROWN);

                //Desenhando o titulo
                DrawTextEx(font, "Bloody War", textPosition3, 100, 2, RED);

                //Barra de vida dos personagens
                if(player1Active)
                    DrawRectangle(10, 10, player.health * 2, 20, WHITE);
                if(player2Active)
                    DrawRectangle(10, 10, player2.health *  2, 20, WHITE);
                    
                DrawRectangle(770, 10, enemy.health * 2, 20, RED);

                //Cronometro
                char numeroString[20];
                sprintf(numeroString, "%d", count);
                DrawText(numeroString, WIDTHSCREEN / 2, 15, 30, GREEN);

                //Desenha os nomes dos personagens 
                DrawText(player.name, 20, 70, 40, MAROON);
                DrawText(enemy.name, 1120, 70, 40, WHITE);

                //Desenhando o projetil caso esteja ativo
                if (projetcilePlayer.active == true) {
                   DrawTexturePro(texture7,projetcilePlayer.Position,projetcilePlayer.Source,(Vector2){0, 0}, 0, WHITE);
                }
                if (projetcileEnemy.active == true) {
                   DrawTexturePro(texture8,projetcileEnemy.Position,projetcileEnemy.Source,(Vector2){0, 0}, 0, WHITE);
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

                if(whoWins == 1)
                    DrawText("Player 1 Wins",250,600,50,WHITE);
                else if(whoWins == 2)
                    DrawText("Enemy Wins",250,600,50,WHITE);
                else if(whoWins == 3)
                    DrawText("Player 2 Wins",250,600,50,WHITE);

    
            }break;
            default: break;
        }
        EndDrawing();
    }

    UnloadSound(musicaFinal);
    UnloadSound(musicaLuta);
    UnloadSound(musicaMenu);
    UnloadSound(fxChute);
    UnloadSound(fxCombate);
    UnloadSound(fxHadouken);
    UnloadSound(fxPulo);
    UnloadSound(fxSelecao);
    UnloadSound(fxSoco);
    UnloadSound(fxVidaExtra);

    CloseAudioDevice();

    UnloadTexture(texture);
    UnloadTexture(texture2);
    UnloadTexture(texture3);
    UnloadTexture(texture4);
    UnloadTexture(texture5);
    UnloadTexture(texture6);
    UnloadTexture(texture7);
    UnloadTexture(texture8);
    UnloadTexture(texture9);
    UnloadTexture(texture10);
    UnloadTexture(texture11);
    UnloadTexture(texture12);
    UnloadTexture(texture13);
    UnloadTexture(texture15);

    CloseWindow();
    return 0;
}



//                            ................................,.........,.,....................,,...,,**,,....,**,(*.*****,...(,,,,,,*,.  . ../.,,,,,,,,,,,,,**,(((((((((((##########%#######%%%&%(#%(##
//                              ..................................,.....,..............,.,...,.,////(/***///#(//#(((###(##/#(#%%(/*.. ... .....**,*,,,,.....,**/(((((((((((((##################%%%&%((#%###
//                            ... .........,...... ..........,.,,.,,,....,..........,...,...,,,/#///((#/#####%#(####,//*(*,.. ..*.###* . ......,,..........,**(*/(((((((####(#################%%&&%#((#%%##
//                          ... ............,,.,.....,,.....,,,*,,,**,***,.*.,..,...,....,...,/((//(##%#(#%#&%###&(/*/#*(,....(/(../##(((((/.,,....,.....,,*/(*//##%#######(#################%%&&%#(#*(%%##
//                           .........    ...... , ,,.,...,.,,,*,(/##/(##(###(,,,*..,.....,,((((#%#(##%##%%##%#%#######((###(%###%((####((,,.,.,*,,,.,..,**//*//**##%#####(##################%&&%%(#*/*%%(#
//           .. .,.....      ..... ..,,....*/*/,,.,*,*.//,.,,*/###*###(*/######%###//,..,.((#(#(##%(%##%###%##%%(%#%%###%%##%%####%##%%##/.**.,,,,,....,**/*,//**/#######(##################%&&&%##*/*/#%##
//      .              ..  .....  ....((,,,/.*,.,/*,%(///(###%###(#%#(###(%%%%%#%#%%%#/*,/(#%########%#%####%%%##%%(#%%###%%%%%%&&%%%%##/*((/,**.,...,,*//*,*/***//%####(##(###############%%&&&(#*/*(/,,*%
//              ...    ..        .,,*/.,.,.,,.,**///(#(##########%%%%%%%%%&%##%&&&%%%%%##%#%#%%%%%%#%##%%%%%%%#%#####%%%%%%%%&%&&&&%%&%%####%##(##*.*,*/(*,*/*/*/(*,###(##################%%&&&(#////(/.,,#
// .       .  .               .   .,/,*.. .,,/,*//((###%###%%%%%&&&&&%&%&&%%&%&%&%&%&%%%%%&&%&%#%%%%%#&&%%%%%%#%%%#%%%&%&%%&%%&&%&&&&%%%%%%##%%%##%%##(/,,*///*///*,##((#((##############%%&&&(#////(/%,,,#
//                      ...  .   ,*/*,..,,*/***/(/(#(#((#%%#%%&&&&&&&%&&&&&&&&&&&&&&%&%&&#&%&%%&%&%&%%%%%%%%%%%%%&&&&&%%&%&%&&&&&&&&%%&%%%%&%%##%%#%#%#&(////*//(*,/,/###################%&&&(#(///(/%(..,(
//        .       .. ..  .. ,/*..*/******(/,,/#(/####(##%%%%&&&&&%&%&&&&&&&&&&&&&&&&%#%##%#%%%%%%%%%%%&&%%&%%%&&&&&&%%&&%&&&&&&&&%&&%%&%&%%%%%%%%%%%%%%%##(////(/,*/(((#################%&&%###//*(/%(#(%@%
//      .... .,,..,...,........,.,/*/,./(,//((#(#(%%#(#%%%&&%%&&&&&&&&&&&&&&&&&%%%##%(#/(((##(((((#####%#%%%%&&&&&&&%&&&&&&&&@&&&@&&@%&&%&&&&&%&%&&%%#%%##%#/#((///((#################%%&&&#(#//*((#(%%/,*/
//  .......,,..,..,/(/*/,*/#,.**.,*,(((#((##(%%%%%######%%(%&&#&&&&&&&&&&%%%%###((//(/*///////**(///(#((##(##%%%%%%&&&@&&@@&&&@&&&&&@&&@&@@&&&&&&%%%%%#%%%####(//(((((((((((((((###%%%%&&&#(#//*((((%%&/**/
// ,.        .  (/*,,,,./.*,*(((((##(#%(##((%%#(#%%#%#%##%%%%%%%&&&&&&&&%%##((//***,****,,*,********/***///(//((((#(#%%&@&@&&&&&@@&@&&&&%&%&%&&&%%%#%&%%#%####(/((((((***((/*/((/*///((#####*/*((/##%&&%//(
// ,   .      ./,,.,/,,*,*,**,//(###*/*(####%#####%#%%%%%#&%%%&%%&%%#%#((/////******,,,,,,,,,,,,,,*,,,************/*//((##%%&&&&%&&&&&@%&&&&&&%%&%%%%%%%##%#%%#(#(((((/////**/***,**(***/##*/*((/%##%&@(/((
// *//(/*(/,**/,.,..********((/(#/(/(((((((##%&#%&%&%&&&%%&%%%%%%&&%#((///***,,*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*,,******/((#%&&&%%%@&&&&&&&%&%%#%%%&#%###%%#%#%%##(((/,.,.....,,,**/*//((*/*///%(#%%&#//((
// (((((#/#/*,%,..,.,,,//,/,#(*(#((((*(#####/#%%%#&&&&%%%&%%%%%%%%###(///****,,*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*,,,**,*******((((#%&@@@&&&&&&%%%%%%#%#&#%#%%%##%#%(%%%###*,,***./(/,..,..,*/(*//(#(#%%&%///((
// /(#(#(##(*%..,,,,..,,*,/((((%(,(/(/(###(#####%#%&%&%%%%&%&%&%%###(///****,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*****//*(((#%&@@@&@&%%%&&&%#%##%###(#%#&#%%&%####(/,,,,,.,.,,.**..,////(((##%%&/////(
// #%%/(#((*#,,.,.,,.,***//(((#*((/((##(##%(#####%#&&&%%&&&%%%%%%##((///*****,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,**,*,*******///(((##&@@@@%%%%%%&&%%#%###(##%%#%%&%%#(##(/***#(/*//*,******/((/((##%&//////(
// %%#/(*(#//.,,...*/*/((*(##////(((#(#%%#####%#%%%%&&&&&&%%%%%%###((/*/*****,,,,*,*,,,,*,,,,,,,*,*,,,,,,,,,,*,,,************///((((#%@@&&&%&%%&&&&&##%%%#%%%%%%&%&%(###(/***/*****,,/(,,*(/(#((##&#(##%%&@
// %(*#*%#,*.,.,*/,(/,/*#(*(*/*((((/#(#%%%###%&#%%%%%&&%&%%%%%#%#((////*******,,,*,.,,,,,*,,,,,,,,,,,,,,,,,,,,,,*,,*********////((((##%&@%&%%&&&&&&&&%%&&@&%&&%###%&%%###(*/**//(#%%&%*%&&&@#((##&@@@@@&&&&
// */##(*/**.//*(.**/(#(/(,(/((#/((##%(#%#%(#%%%%&%&&%%&&%%%%%%#(((////**/****,,,*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,***********/////(((###%%%%%%%%@&&&&&&&&%&&&&%%&&%%%%%%%#%#((((/((////,(///((((#%%//******/
// %*#(*.***.*/.**,*((((///(%#//(#(%(%#%#%%%#%%#%%&&%&&&%&%######(((/*********,*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*,,*********///((((((#%#%#%%&&%%%&&@@@&&&&&&%%&&%#(&%%%%##(((//*/////*#//(((((#%/*********
// ////(***,,(,(/(//*((/*/(/(/###%##%#(##%###%#%&%&&%&&&&&%#####(((/*****,***,,,,,,,,,,,,,,,,.,,,,,,,,,,,,,,,,,,,,,,,,********///((((###%&#%&%%&&&@&@&&&#&&&&&&&%%%(&%%%%%%##(((///(((((##################%
// ///(//***,/(/((/#**(//#(/(##%((#%##((%(#%(##%%&&&%&%%%%%####((((//****,,,,,,,,,,,,,,,,,,,,,,,,,.,,,,,,,,,,,,,,,,,,,*,******///((((#%&%%%%&&&@&&@@%&&&&%&&&&&%%%###%%%@&%%#((((((##(##&%((((((((((((((((#
// **//****(//(((((#/*///(((%%&%##%%(##%(#(####%%%&&%%%&%%%%###((((//****,,,,,,,,,,,,,,,,,,,.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,***////(((#%%&%&%%&&&&%@&&&@&&&&%%&&%#%&&##%%%&@%&%%#(/(((((%#&((((((((((((((((##
// /**/*//#(/*((((##(/(/(((#&%%(#(&#%##(#####%#%%&%%%%#%%%%##(((#((//******,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*,********/////(#&&@&%%&%&%&&&@&%&&%&%##%##%%&%&%%%##%%%&#&#//*((##&((((#((#(((((((((#%
// ,,*(/***/./(#((###(/(/((#&%#%%%%%##/(((###%%&%%&&%#%%%%%##(((((((//***,**,,,****,,*,*,,,,,,,,,,,,,,,,,,,,,,***,*****,********////(#%&@&%#%&&&&@%&%#%%%%%(###&%&%%%%&&%%#&&@/%(#(((%%((((#%@@#((((((((#%&
// ,///******/(###((///((/##%%#%%%%#%(######(#%&&&&&%&%%%%%###(/(/////**********************,***,**,*,,*************/************////(#%&%%&%&#%&&%&&&%%%%###%%%%%&%%%%&&&%%%@%&%(&%%%######%#((((((((((#%&
// ,,****////#((##/(/(//((#%##(%%%(&####/(##%%%&&%&&%&%&%%&%#((((/(((/////////////////////****************/////////////(/((////(//////((%##%##&%@&%&&&@&%%##%##%&&&&%&&%#%&%%&&&#%%#%%###########((((((#%%%
// (*/#((((#((((**(((//(##%#&##&%%%##%(((#(#%#%&%&&&%&%%&%%%%(######%%%##%#%##%#%%%###(((/(///*/****///////((((((###%%%%%%#(/////((##########&&&%&@%%&&&&%%(%#%%%%%%%#%%%&&##%%%%%%%#%############((((##%%%
// #((/(*(#((/**/(//((/#%##%%%#%&&%#%#(/(#%%%#%&&&&&%&%%&%%&%%#%###(((((((((//(((/*(&&%%##(((((////////(/(((##%%&&,(((#%&%(, ..,,,*//%@@&#%%#%&%%&##&%&%&%%&/#(((&%%&#%%&###%%%%%%%%%####(((((###(((((#%&%#
// *((/##(/*,**,////#%(##%###%#%%%###((##%%&%&%&&%&%&%%%&&%%%&&%%%(&%%(/.,.,.**(//.#&%#(#//(#((///////(((#%(*/###&((//*%#(((,,..**,.(,%&&&%%#%%%&&#&%#&%&&&&&%#%#(##&#%%%##%%%%#(##%##(#((((((#%##(((#%&%#&
// #(((,*/*//,,/#/(#(###%###%%##%(###%##%%&%&%%&%#&&%%%&%%&%%&%#%%%%%%%/,.(,.,,**.(//((,%%%#(##/*****///(##%#%//(/(/#/#*/((#/,,**(,(%%&&%%#%%%%#&&##&&&%&&%%%&#%%###%#%%%#%%%####(#######(((((&%#(((##%%##&
// //*/*/,**/*(//(######%%#(#%###%&#####%%%%%%&%%%%&&%%%#%%%%%&##%&#%#&##(,*,*,,**./#(**##%&##(##((((((###(###((/((/#((#(%#%%((//*/##&&&%%&%/%(&&%#%#&%%&&&&&%%%%(#%####%%#(#(#((#######((((@&#(((((#%%##%@
// ((/((/*,*//*//(##(#%%%#%%(%%%#%#%##%%%%#%%&&%%#%%#&%%%%#%%&%&%%%%%&%(*,*((((/(//(/#(##%#(%#####%%&%%###%#%(/((//((*//%#**%##(%##%%&&%&&%##%%&%##%%#%&&@&&&&%&####(%##%#(##%(%#%%####%%&%##((((((#%%%##&%
// ((((/***//(//((#(##%%%%%%%%%%&#&######%%%&%&%##%%%&%&&%#%%&@%((%(#&&%*//**/#*(//(/#(##&%&####%%#(//(%&###(#/((####(%&&(#%%%#,,*%%&&@%&&##%#%###%#&@&&%&&@@&%%%#%%#&%&%%#%%%%%%&&%%%##((((((((((##%%##%&%
// /(#/*/,(#/(/((/###(%%%%%%%#%&%&%%(#%%####&&&%#%&%#%%%%%%%&&&&&##(##/#&%(*,*#/*(#(#*#%&#&&##%&(,,,..,,,(&(#(#((/((####(##(%%,*/%&%#&&&&%%&&&&#%%&&%&&&%&&@&&%%%%%%&%&&&%@%%#&@%#%#####((((((((((#%%%##&%(
// (#(///#(#((((#((####%%%%%#%%&%%%#%(#%###%#&%%#%%#%%%%(#&&&&&&&&&#(##/,%%%#(*/#((#(*((&&@%#%%*,,,.....,,,&##(#(#((#%#*,/#(//*(#%&&@%#%#%%%%#(&%@&&%(%&%%&&&&&%%&&&%&&&@&&&&&%%###%%##((((((((((#%%%##%%(/
// (#(/(#(##/(//(((##%(%%%&%%#%%#%%#####%#%%###&#%%%&%%#%%&&%#%%&%&&/*##(/,.,,&*///*((######&#*,,........,,*%%###(((%%,,/(*(**#%@&&/#%#%#&&####%%&%%&##%%%&@&&&&%&&@%&&&%&&%%#(%####%###((((((((#%%#%##&@@@
// ##(*###*/*//(((#%%#%#%%%%#####%#%%%#%#####(#%&%&&&%&%#%%#&&&##%&@&%####((///%#*/(((((((%&/,,,,,.......,,,*/&%(/((/(/,,*/(&%%&%(%(%%&#%%%%%((&&&&&%#%%#&&&%%&@#%&&&%%&&@&&%(##%%##%##((#(((((##%#%##%%(/*
// (###(%(//*//((#####%%#%%%%##(#%#%%(%%((#####%&%%&&%%%%#%%%%%#(%&&&@&&&#(***//%/(#*#*%%&%*,,,,,,.......,,,,**#&&%%#(/(((((((((%&&&%%%&%%#(#(##%%&&#%%%&@&&&@&&@&%%&%%&&%%#%(&#%#%###((#((((((%%#####%(/**
// (((###//*/*((#%%##%&&#%(%&%#####%#%##((((###&&@&&%%%%%###%%%%%%%%%%&&%&%&%%%%%%%%%%%%#/*,,,,,..........,,,,**/#%%%%%%%%%%%####(##(((((((((####%%##%&&@@&%%&&&%%&@&@%%%#%#(#%(%%###%##%#&%%&&&%%##(##(/**
// ((((/((/((%((###%%%&%#%%%#%%%#####&(#((#####&%@&%&%%%###(((((#(((((#####%%#########(//****,,,,.,.......,,,,,*//(((((((((((((((((/(/((((((((((##%%#&%@&&&#&&%&%%%#%%%%&&%&#%#%%#%#####(%%%%&%&&%&%&#(/***
// ((##/*/##&#####%%%%%#%%%&%%#%%((####%#####%#&&&@&&%%###(((((///***/////////////*//*////*,,,,,,.........,,,,,,*/((////////////////////((((((#(##%&%&@&@&%#%&@@%%%##%&%&&&%#%(###%####((%%#&&%&@&%&%((/**,
// (((/(%((&##%###%#%#(#%%%%#%%%&##((#######%%&&&&@&&&%####(((////*********//***/*/***///,,,,,,,,,....,,,,..,,,****(///***************//(#(((###%#%%&&&@&@&%&&&&%%#((#%#&&%%%#%###%#%######%#%&&%&&%((/**,,
// (//(#(#%#####(#(###%#%%%%%%%%&&#((((%#%#%%#&&&&&@&&%%###(((///********************///**,*******,,,,,*,,,,,*////**////**,*,,********///(#######%%&%#&&&@&&&&%%%###%%%%%%%%%%%%#(######(###(#####(((/**,,,
// /((((#(####(#(###%##%##%#%%#&#%##(#(#(##%#%%%%&&&&&&%####(((///*******************///****/(((//***////////((((///////**************//(#(####(&&&&#&#&&&&&%%&%%%%%&%#%%%%%%%%%%##%##(%#%%##%&%#(((//*,,,,
// (#(/#(((%####%####%##%#%%&#%%%##((((/((##%%#&%%&&&@&&%%###(((////*******************//((#%%&@&%#(((((((((%&%%##(///**************////((&###%@%&&@&&%%%&@&&%%%%%%%%##%%&&%%###%%%%###(#########(((((#((((
// #(#((##%(####%%##((#%%#%&%%#%%%#(((/#((((%#%%%&&&@@@&%#####(/////*******/*************/(#####((#######((((((###/*********/***/*//////#%((%#%%%%@&&@&%%&&&%##/#%%%###%&&%%%%%%#%%#######(##%##((**((/(/((
// (((#((#(#%#%#%%%######%%%%%(%%(%#(((/#((###&%&&&&&&@&%#####(//////****/*////****,*******////(((((((((#(((#%%(##((********//////////(#%((#%&&%#(@%&%#&#&&##%%#%%%&#%%%##%#%(#%%#%%%%#&#%#####(((((/(((/(#
// (####(##(%###%%&#(###&&%%(((&#%%##(/(###(#%#%%&&&&&&&%%#####//////****/////******,**,//(((((%######(##%%%%%&&%%##/*******/////(///(#(%##%@%@%&#&#%%#&&##%%#&%&%%##%%##(#%%%%#%%%%&#(%%(###%#((/(((////%(
// ##&(#((###%###%&%(##%%%%###(%(%%%&&####%#####&&&&@@&@&%%####((////**/**///********,//((#&%(%%&%%%&%%%%&&&&&&&&%&%&%%(/////(/////((((#%&%&@&@&@&%%%&&%%%&&#%%%%&#%%%#%%&%%&%&((%%%#(%%%#%%###(///(////#(/
// ,,.,///(##&##%%%%##%#%%#%(###&#%#%&%%###%###%#%%&@@@@&%%%###(((/////**//////(((###%(%&&%%%%&&%&&&&%%#%%%%&&&&&%&@&&&&&%%#%##(((((((((##&&&%&@%&%&%%&%&&%%%#%&%%%#%%&&&%&#%&@@&%#&#%#%%#%###(////////%(/(
// /*///(/(#####%%%##%#(%#%#(((%%%%%%(&%%##%%&#%%%%&@@@@@&%#####(((/////////(#####&#%%%%&%&%%#####(#((/(#//((##%%%%%%%&&&&&&&&&%%#(/(/((#%&@%%#@#&&&%%%&%%%%#%%%&%%&%&%&%&%&%%@%&&#@%#%%%##((###//////%(/(#
// ((((#((##%%%%&%#(####%%%###(##%#%%%%%%%%%&&&%&&&&@&@@@@%%##((((((///////(#%%%%%%%&%###((######((###((((#####%%%%%%###%%%&&&&@&#((/((##%@@&%%#%#%&%&#%&%&%%%#(%#%&#&&&&&&&&&#&&%&&&/(%%%%####((((((##(/#(
// (#//(##%&%%&%#%##/#%%%%%%##((###%&(%&#%%#%%&%%%&&@&@@@@@%%#(((#(((////(#%%#%%%%&%####%###%%%%%##%%%%%%%%##((((####%%&&&&&&&%&%#(((((#%&@@@&%%%%%&#&%%%%%%%%###(#&(%&&&%#%%%%%%%&%&&/%%##%%##%%#%##((/((#
// /((#%%%##(#%####@(#%%%%%%((#((###%##%%###%%&&%&&&&%@@@@@@%%##((((((//((%####%%%%&&&&%%#(///*,*,********/****///((((##((######%((/((%%&&%&%%#%%%%%%%%&&%%%%%%%%##(%%%%&&&%&%##%#%%&%%&#%####%###%#%(#(#(%
// (%(#%%##/*(###(&%%%&&##%%&#((####%%#%#(##%%&%&&&&&@@&@@@@&%###((#(((//((((######((((((////////////////////(((((((((((((/(((((#((((#%&&&@&%&#%&#%%&@&&&&%%%%#&%%%%%%%%(#%#%%%##(/(#%%%%%%%#######%%###(#(
// /((#%%%###(#(#%%%%%#%%%%%%#%%##%#%%%####%%&%&&%&@%&&@@@@@@&&%###(#(((((((((((((/((((//////(((((###############(((((((((///((((((##%%&&#@@&&@#%##&@@&&&&&%%#&&%%%&#%&%%#%%%&%%###%##%##&%#%%(%(##%##(%##/
// ((((%###(%#%#%#%%##%%&&%&%##(#%%%%((#%#%%%%&&%%#&&&&&@@@@@&&&%#####((((((((((///(((((////(/((##(######%%#######(#(((////(((((((##%%&&&@@&&%###&@@&&%&%@&%%%&%&#%##&&&%%%#%########&&&%#(/*##%##%###((#&%
// ///((#(###%%%%####%%###%%&&#(((#((####%%&%%%%%&&%%&&@@@@@@&&&&&%####(((((((((((((((((((//((((((((####(###(#(((((((((//((((((((##%&&&&&&&%&%%#&&%%&&&@&&&&&&#(%%%%%&&&%&&&%#%%##%&%%&####%####%####((#%%#
// /(/((%#(#%%%#%&#(#%##%#%%%&%&((((#%%%#%%%&%&%%&%@%#@@@@@@@&&%&&&&###(((((((((((((((((((((///(/((//////////////(/(((////((((###(%&&&&&&&&&&#&&@&&%&&&&&&&&%&%&&#%##%&&%&&&###%%%#(##%#####%###(/((((((/**
// #///(#%%####%%#&%#%%&&#%%&%&#(((#%##%#%%%&&&%&&&@&&&@@@@@@&&%%%&&&&###(((((((((((((((///////*************/////////////////(####&&%%%&&&#&&%&&&&%&%&&&&%&&&%&%&%%%%%&&&&&%%#%%%####(%#&%##%######(((((/**
// %#(((##&%%##%##%%&%&#%#%%&%%(/((####(#%%%%@&&&&&@&&@@@@@@@&%%%%%%&&&&%####((((////////(///********////*//////////////*///(###&&&&&@%&&&%&&&&%%&&&&%&&&%#&&%%%%%&&&&%&&&%%&%&%%%%##%%%(#########%%#((//**
// ((##(##&%%%%%#&&%(%%%%%&%&#%(//(###((##%%&%%&&&&&&&@@@@@@&%%%%%%%%&&&&&%#%#((((/////////////**//(///////////////////////((%%&&@@&%&&%&&&%&&%%&%%%@&&&&&%%(&&#%&&&&&&&&&%%&%%&&%%(((%#&###%&%%%##%###/(**
// ####(#%&%#&&&%%&&#%#&%%&%&%%/(/((((#(##%#%&%&#&&&@&@@&@@@&%%%%%%%%%%&&@@&&&%##(((/(////////////*////(/(/(((//(/(((((/((##&&@@@@@@@#%%%#%&&%%&%%##&%@&&&&%&%(###&&&&&&&&&%%%&&&%##/*/%%%%#%%#%%&%&%(%#(/*
// %#%(#&%##%%%%&%&&@%#&%&&%&%#(//(##(#((((((#%&&%&&@@@@@@@&&%%%%#####%%%%&&@@&&&%##(##((((((((((((//(((#(####((#######%%%&&@@@@@&@@&%%%%%%&&&#&%###(%&&&&&&&&@&%&&&%&&&&&&&&&%&%&&&%%%#%&&%%%%%####%%#*/**
// #&&&&%%%%%##%%#@&&&%%&&%%%%%(//(###%%%##%%%&&&%%&&@@@&&&&%%%%##########%%%@@&&@@&&&&&%%%%#%##########%%%%#%#%#%%%&%&%&@@&@@@@@@&@&@&%%%&&&&%&&%%#%%&&&&&&&&&%%%%@%&&&&&&%%&&&&&&%&%((((##%%#%#%%#((////,
// %&%@&&#%%%&%%&&&@@%%##&%&&&%%#((#%%%%%%%##%%&@&&&&@&&&&%&&%%###############%&%&@@@&@&&&&&&&%&&%&%&&&&&&&&&&%&&&&&&&@&@&@&@@@@@&@&&&&&&&&&&%&&&#&&%%%&&&&&&&&%#%%%%&%&#&&&&@&&&&&&#@%#(##%%%##%%%#%###(/*
// %%#%&&#%&%%&&%%&&&##%&&&%&%%#%(###%&%%%%%&%%&%@&&&&&&&&%%%%###################%##&&@@&&@@&@@@@@@@@@&&@@&@&&@@@&&&@&&@&%%&&@&@@@@&&&&&&&@@&&&%%&&%###%%&@&&%&%&%&#%%&%&&&%&&&&&&&&&&&&%&%&%%###((/(*(*/**
// %%%(%%%#&%%&#&%%#&&&%&&&%&%%%#%%#%%#&&&&&&%%%&@&&&&&&&&&%%%#%#####################%%%&%&&@@@@@@@@@@@@@@&@&@@%%#%%%###%%%@@@@@&&@&&&&@@@&&&%&&&&#%%(##%&&&&%%%%%&&%%%&&&&&@%&&@&@%%###%%%%%#***//*,*,.,*,
// #(#%%%#%%%%&%&%#&%%%%%%%#(%######%#%%%%&&&%&%&%&&&&&&&%%%%%%############((#########################%##%#############%#%%@@@@&@&&&&%#/(%&@@&&&&%%%%####&&%#(%##%&%&%%%&%@&@@&&@&&&&&%&(%&&(///////*,*/*,*
// %##%##%%%&%%%(#%%%%%&%&&((#((((#####%%%&&%%%%%%%%%%&&%%%%%%############(((((##########################################%#@@@@@@&&%%&%%%#&&%&%&%%#%##%#%&%##%&%%#####%%(&@&&@&@&%%%%%#&%#((*(,/(/,****/,**
// #%%%#%#&%&%&##((#%#%%%&&&##(###(###%%%%#%%%%%%%%%%%%&%%%%%%##############((((((################(#(####################%%&@@@&%##&&%&&&#%%#%%%%%%%#%#%%&%%%###%%%%&%%&&%@@@&&&%&&#%%%%%##/(((.//* *,,**,*
// &%#%#%&#&%%%#(//(%(#%&&&&&%(########%&&&##%%%%%%%%%%%&%%%%%#############((((((((((####################################%#%%@&&&%&&&%&&&&@&&%&%&%&%%%#%&%%###&&%%%&&&&&&&&@&&%&&&&&&%&&%%%(#/*./*/ **,,,,,
// %%#%#&##%(&%%/((##&%%%&&&@########%%%%%&&%%%%%%%%%%%%%%%%%%%#############(((((((((((######################################%%&@%%%%&&@&&&%#&%&&%%%%&&&%%%%###%######%&%%%&&@&&&&%%/%%&&&&%%#****.*,,*,***
// ###/((##%%%%%/(##%%&%%&&&@#%#####%%%%%%%&%%%%%%%%%%%%%%%%%%##############((((((((((########################################%%%&&&%%&%&%&&&&&%&%%%%%#%&&#####&####%%##*..,#@%@&&%&%%%%%%%%##/(* *,,,*,,,,
// #((((%##%%&%#%##%%%%%%&&&&&#######%%%%&%%%%%%%%%%%%%%###%%%%%################((#(###((#############################((###(((##(#&%####%&&&&%%&%&%%&&&&%&(%%%%#%%%%%#%(*,./.&@&&&&%%%%%%%%%%#(*.,,,,,,,,,,
// #((##%%%%%&%#%%%%&&&&%&&&&&%######%##%%%%%%%%#%#%%#########%%%#################((#((((#############################((####(((((##&#((#&%&&#%&&&(%%%%&&&%&&%%#%%%&%%%%%%*..(@&@@@&%&%%%%%%%%(/,*,,,,******

// ########((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((*((((((((((((((((((((((((((###(%##%@&#&%#@&&%%&&%/&##%&&&##&/(&#%%##########################################(((////((#(#
// ((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((,((((#((((((((((((((((((((#(#/@@%&&#%@@@%&@@%@@&@@#&#&&&&(@#(((&&#%#####################################################
// ((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((*(((((#((#%(((((((((((#(((((*#%(#%&@%&@@@@@@&@&@&@@@%@@@%###((####((##################################################%#
// ((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((#&(((%((%&#(#/((#####((((##&%&@&#&#&&@@@@&%&&@@@&@&&@%(#%%%%%(((#((((((((((((#((((((((#((#((##############################
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((#%&%#%#%&#((&%/#%&(%/*((((&&%&&@@&&&@@&&@#(%%&&&@@%%&@%#%###%((((##((((((((((((((((((((((((###((#########################
// (((/(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((#@/##&@(#%%&@&(@/(%%&@(/(#&##&#(&&&%##%@&&@(%%@&&,&#(%(%&%((#%%/(((((((((((((((((((((((((((((((((((((((###(#########((#####
// ###//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((#%%(&&%@%(%/#&(#%%%%%@&@@#&@@@(&(@&@@@@(@#&%&&&%(%%&#(((((&#%(##(#(((((((((((((((((((((((((((((((((((((((((##########(#
// &((//((//((//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((##(##%&%(%#%&//&#&%%@&%&(&&@@@@&&&&@@&@@@@@/%@%@@@%%%&#(%%%#%((((%((((((((((((((((((((((((((((((((((((((((((((((((((((((#
// #((((%(#/////(/(//((((((((((((((((((((((((((((((((((((((((((/((((((((((((((((((*/*(%&%%&&(%*&/(%@&&&%(%&#@((((&&&@@@@&%@@@@%&&@&@@(%@%#(##%&%#(((((((((((((((((((((((((((((((((((((((((((((((((((((((((#
// %%%##%%%&%%%(#%##(//(((/////////////////////////////////(((/#((/((((/(((((/((//#&(#%&&%&&@%#%@%&&@@@&@&%@@@%(#&@#%%&&%##&%(&@&#%%#&@&%/((((#((#%(((((((((((((((((((((((((((((((((((((((((((((((((((((((#
// #%(#&(%%#(%%##&%%&&(##(////////////////////////////////////#&##%(###/(((((((#(/(/%%(%#(&((((%%&&@&@@@@@@@@@&@@@@@&%%%%%(&%%&%(&%(##&(#(%#/&%##%((###%%(((((((((((((((((((((((((((((((((((((((((((((((((#
// %///%%(####((((#&(#((/(/////////////////////////////////////(#&@%#%#%&#%%###%%%(/%*(&%&&%%%#(%%&@@@@@@@@@@%&&@#&@@@%%#%%%(&@@@%&@%/&&(#%#%#(###&#(%#((%%(((((((((((((((((((((((((((((((((((((((((((((((#
// @@#%*(%#/%&&((#/(##(##(#/#//////////////////////////////////*#/&%#&%%&&&&@%&&%#%%#%/%&%&&@@&*#&@&%@@@@@@@@@@@@@@@@@#&&@#&%@&@@@%#&#(@#(%(&%(#((#&%%###((((((((((((((((((((((((((((((((((((((((((((((((((
// %###@&&&&%%##(/%,####/#(#/(%////////////////////////////((((((%%&&(%%%&(&(%(&%##/(#&&#&&(#&%#(&%&@@&&@&&@@@@&&@(%%@@&@&@&&%&##@&#(%&&#(&%&#(((((#&*%(#(/((((((%@&&%%%#*/,###%%((((((((((((((((((((((((((
// @@&#/((#(&%//%&@&/&(&##/%&##(%//#/%(%#(/##%&(((/////////((/(((%@&&@&@@@&&&&&%&&(#@##(#%#((#%%@@%@(#%#@@%@&@&@@@&&@&%###%&&#&&%&@&@&#(#%&%#(#(%((%(((#/#((((((%&&&&&%%%#/(####(((((((((((((((((((((((((((
// #&(%%@@%/%%%%&%@&&&##%(((((////%(///////(%%(%#/#//////////#((%##@@@&@@@@&@@@@&@&&&&%##%#/&#@@@@@@&&@%@&@@&@&@&@@@&&&&%@@%%&%#%@@&%&&#%&%(#/(((((/(((((((((((((&&%/*/.,(,(###((((((((((((((((((((((((####
// %@&&@@@&/%@@&&(*(/(#%&##/((%&%#/////%%(%&@#&&&&%&////////#((@%&@&@@@@@@&@@&@@&%%@@&@&%&&&@%@@&&&@&@&@&&&&&&&%%&&&&&&@@@@%%&&&%&&&&@%&&#%%&#(((((///////***,,,,*/,,..................*(((((((((((((((((((
// &#&(**#%#*/#%#%#/*(#//#&%(#/**(%////#@@@@&@@&%##&#&/%////%&&@@@@@@@@@@@@@@@@@@@@@@@@&#%##(&&%@@@@&&@@&&@@@&&&@&&&&&&@@@%&&&%%%&##&#%%%###%#&*.,., ..       . .,              .   .  ,(((((((((((((((((((
// @#(#*(#/###(&&&&&%&**,*/%&*%/(/(//(&@@@@@@@@@@&(/(//(@&&/%@@&@@@@@@@@@@@@@@&@@&@@@&&&&&#&@@@@@&@@@&&@&@&@@@@@@&&&&&@&@&&&&%%%&@%%%#@&%#&%#%(.  /.          . ,.        . .. ...,,(*(%%&&&&&@&@@&@@@@@@&&
// %&%%&%@#%@&%&@%(&%(##&@&&&%%(/#(@@@@@@@@@@%&%&@#//%*/((&&@@&&@@@@@@@@@@@@@@@@@@@@@@@&&@@@@@@@@&&&%&&&&&@&&@@@&@@@@@@@&@&@@@@&@@@@&%%&%@%%#%%@@&%@@@@@@&@@@@@&%@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&
// (/&%#&&&@&&&@@@@@@@&@@&@@&@&%#%@@@@&(&%@@@@@%(&@%//(#//%@@@&@@@@@@@@@@@@@@@&@@@@@&@@@@@@@@&@@@&&&&&&&&&&@@@@@@@@@@@@@@@&@@@&@@@&&&%##/&%%%(@@@%&@@@@@@@@@@@@@@@@@@@@@@@@@@&@@&&&&&&&&&&&&&&&&&&&&&&%&&&%
// (&&@@#&@%@&@@@&&&&%@&&(#@@@&#@@@%/@@&@@@@@@@@&@%@@(#@///%(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&%&&@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@#%@&&%(/  ,     ,%#%#%%%%%%##%&&&&&&&&&@@&&&&&&&&@&&/#/((((///////(#
// #/##%%&#%&%@@&@@@&&@&&@@@@@&&#&/&&@@@@@%&#&@@@&@%///@@@@@@&@@@@@@@@@@&@@@@@@@@@@@@@@@@@@&@@&&@&@@@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@&#(&%&##,..    ,((#((#((#((##&%%&&&&&/%*/&&&&&&&&&&((,(////////*//#
// #%((%/&@&&&@&#@%@&&@@&@@&&&#%****#&@&&&@@&&@&%%(*//#@%@@@@@@@#&@@@@@@@@@@@@@@@@@@@@@@@@@@@%@&@@&&@@@&&@@@@@@@@@@@@@@@@@&@@&@&@@@@@@@@@&&&##( */#/.  .((((((((/((%%%%%&&&&%(*/%&&&&&&&%&%((.*/(/(//*/**##
// %/(*%&%#&&@@@&@@@@@&&@&@&&&*********&@@@@@@@@@@&*******((*#(%@@@@&@@@@@@@@@@@@@@@@@@@@@&&/**,,#&&&&&@&@@&@&&%&&&&@&@&&@&@&&&&@@@@@@@@@@@&(##***(#,  .(((((#%####%%%&&&%&&%%##&(&%&&@&%###/*#(#(%*,,(%//#
// (%%&&&%&@@@@@@@@&@@@%&&@@@&&@@@@@&%&@&@@@@@@@@&/&%###&&%@@@@@@@@#@@@@@@@@@@@@@@@@@@(/////******,,**(&&@&&&%&%%#%##%%&&%#%#(##&&@@@&@@&@@&%%%(  *,. ../##(##%%&%%%%#&#(%&&%%&@@&&&&%&@@&&&%%#/&*,/,,%#**(
// &@@&#%&@%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%(((///************,,,,******///#%##%#((((###((((#%&@@@@&@&@&%%%., ,,.  .(##(*%&&&##%##&%&%&&#%%&&&&&&%%&%%%//*//(,..,.//**(
// @&(&%(@@@@@@@@@@@@@&&&@@@@@&&%&&@@#@&@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@%((////***,,,******,,,,,,,,,,,,**//*,,,,,,,,,****(%(((/(%%&&@@@@@@@@@@@&%.,&%%%#*/.,#%%#..,#*///***///*/*#&&&&&&%/*#*****//   . //**/
// /%@@@@&@@@@@@@@@@@@@@&&@@@@@@@&@@@@@@@@@@@@/&@@,.&@@@@@@@@@@@#/**,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*******,,,,,**/(////(%&&@@@@&@@@@%%%(     ./*#%%%#(,*,,,*****//*/,/%&%&&&%(//(/*,,**   . ....,
// %(/@(&%%&&&@@@@@(@@&@@@@@@@@@@@&@@@&@@@@&@@%(%&&&&@@&*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*,****,,,,,,,,,,,,,,,,,*******////*,,@@@@@@&@&#*                                  ,.....,.,..         .....
// (%(#%&@@&@&&@@@@@&%&&%@@@@@@@,%@#@@@@@@&&%%%%%%*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,********//(&&%#(///*//*******************///%&&@@@@&#,                                  *........,,              
// &%&&&@@@@@@&@&@@@@@@@@@@@@@@@&@@@&@&#%#%%%%%%%%#.,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,****/#&%##(#&&&&@@@@@@@@@@@&&&&&&&&%%####(#((********/****...                               ,. ...,,.,...            
// @@@@@@@@@@@@@@@,%&@%@@@@@@@@@@(@@#%%%%%%%%%%%&&&%/,,,,,,,,,,,,,,,,,,,,,,,,,*******//((&@@@@@@@@&&&%%##%%@@@@@@@@@@@@@@@&&&&&&&%#((////********,.......,.....,..   ..........  ......,                   
// &@@@@@&@@@@@@@@@&&@@@@@&&&&&&&%%%%#%%&&%%%%&&%%%&%%,,,,,,,,,,,,,,,,,,********///(&&&&&&&&&&@@@&#(#///**(@@@@@@@@@@@@@@@@@@@@@&&&&&&%%%#((((/**.,....,,,,,.,...... .....,,.,.,,***.*(###. ((#%# .**##* . 
// @&&@@&@@@@@%@@@@@@@@@@@&#%##%%%%%##%&&&%%%%%%%&&&%&%(*,,,,,,,,,,,*******///(##%#*,,,,,,,,,/(%&&&#%#((/(/@@@@@@@@@@@@@@@@@@@@@@@%((/(/#&&%&&. /#*    /#    .(/    /#.   .((    ,(/    /#     ((    ,#*   
// @@@@@@@@@@@@@@@@@@@@%%%%%%%%%%%%%%#%%%&&%%%#%%&%&&&&&#,***********/(((%%%%#*,,,,,,,,,,,,*,((&%&%&%###(/&@@@@@@@&&@@@@@@@@@@@@@@@&%#(((&&&&@#///*    ((    .(/    *(.    ((    ,((    /#.    ((    ,(/   
// .&&@@@@@@@@@&@@@@%%&&&%%%%%%#%&%%%%%%%%%%&%&%%%%%%&&&&%#(//////////(//*,,,,,,,,,,,,,,,,,**//%&&&&&%#((/////((#&&@@@@@@@@@@@@@@&((###((&&&&/*//(*    /#     ((    *(.    /(    .((    *#,    ((    .((   
// @%@@@@#@&%@@@####%%%%%%%%&&&%%#%%%%%%%%%%%%%%&%%%%&&&&&&%&@@@&&&&%%/**********,,,,,,,******(%@@@@@@&#(/***/(((#%%%#%%%##(((####(/(((((#&%**,**//    /#.    ((    ,(,    /#    .((    *#,    /#    .((   
// ..*@&@&&&####%%%%%%%%%##&&&&&&%%#&%%%%%&&%%%%%&%%%%%&%%&%%%@&&&&&%(//****************//(&@@@&&&&@@@&#(/**///((((#%%%%%&#%%##(((//((((/(%%/,,,**(*   /#.    /(    ,(*    /#     (#    ,#*    /#.    ((   
// &&&@@&##%%#%#%%%%&&%#&%%%@&@&&&%%#%&%%%&%&%%%%%&&%%%%%%%%%%%#/#&%&%&&%#((////(((((#&@@&&%#(((##%&@@&#(//*////(/(((((((##((/////////(((/(#//***,/,   /#, .  /(    ,(*    /#.    /#    ,#/    /#.    (#   
// @&%%%%%%%%%%%%%&&%%%%##%%&@@@&&&&%%%%%%%%%&&%%%%%&%%%%%%%#%%%##(%%(%@&&&@@%@@@@@@@%#(#%##(((((#(&@&%#(/****//////////(((((////**///////(/,**//*/,   /#* .  /#    .(/    *#,    /#,   .#(    *#*    /#   
// %#%%#%%%%%%%&&&&&%#%%%%%#%&&&@@@&%%%#%&%%%%%%&%%%%%%%#%%%%%%%%%%%%&#%@&@@@@&@@@&&%##(/////////(#&&&%#///*///////////////////*****//////(/*,*#***,   *#/..  /#.   .((    *#*    /#,   .#(    ,#/    /#.  
// %#%#%%%%##%&&&&&%%%%%%%#%%&&&%&@@@&%&@%%&%%%%%%&%%%%%%%%%%%%%%%%*/%&@@@@@@@&@@@&%%%#((///**//((#&&&%#//**///////////***//*********//////((***/*/    *#( .  *#.   .((    *#/    /#*    (#    ,#(    /#, .
// %#%#%%%%%%&%%&&%%%%%&%%%%##&%&&@@@&%%&@@&&&%%##%#%%%%%%%%#%%&//#%%#%@@@@@@@@&@@@&%(((//****///#&%%%%#//**//*///////((((/////**///////(((/*****/# .  *#( .  *#,    ((    ,#(    /#/    (#    .#(    *#* .
// %%%%%%%%%%&&&&%%%#%%%%&&%%%%%%&&@@@@&%%%@@@&%&%%&%%#%%&@@@@#&(#@&(%%@@@@@#%@(%@@&%##(///**//#@&&##%%(/////*///////////(%#((//////////(///***,.#%.   *## .  *#*..  (#.   ,((    *#(    (#.   .##    *#/  
// %%%%%%%&&&%&&%%#%%%%%#%%&%%#%%%%&&@@@&%##%@@&@@&@@%%@@@@@&&&(/@@(%%&@@@@@#&&*@@@@&&%#(/////@@@@@@@@@&%(((////////////////(&&#(((//(((///((,. .##..  ,##..  ,#/..  /#.   ,(#    *##    /#, .  ##    ,#(  
// #%#%%##%%%%%#%%%%%%%%%%%%&&%%%%%%&&&@@&%%####%#%&.,@&@@@@&@@&/@@@%&&@@@@&###*@@@@@&&%#(((#((%&@@@@@@@@&#((###(((##(/((/(/#(###&(////////((,. .##.   ,## .  ,#/..  /#.   .(#    ,##    /#*    (#    .##  
// &&&&%%%&&%&&%%%%%%%%%%#%%%%&%%%%%%#%%%%%%%##&&##(.&&&&@@@&%@#*%@@%&#@&@&#%(/(@@@@@@@&#(%%%%&&&&&&%%%%##(((((/*//////////(((###%%(//(//(/#(.. .#%,.  ,##..  ,#(..  /#,   .(#    ,##    *#/ .  (#. . .##  
// &&&&&&&%&&&%&&&&&&&&&&&&&%%%%%%#%%%&%##&#&&%//(#(,.((/&@@@##/*&@@@@&@@@#%%&#*@@@@@@@&(#%&&&&&@&&&%%%%####(((///((((((//(%%&(/(%(//////(*%#*. .#%*.  ,(# .  .#(..  *#*    (#.   .(#    *#(    /#* .  (#  
// %%%%&&&%%%%&&&%%%%%%%%%#%%%%&%&%&%%&(%@&#&%######.,(((@@@@@&*,@@&@@%@@&%&@&/(&@&@@@@@%/////&@@@@&%(##%#(((/*,**,**,,***%(/*//((//////( ,(#,.  (%/.  .(#... .(#..  *#/ .  /#,   .(#. . ,#(    /#/ .  (#. 
// %#%%#%#%%%%%%%#%%%%%#%%%#%%%%%#(#(%%%%&&@&#%%###(#./##@@%#&#**@@@@&#@@%&&@%*@@%@@@@@@&%/*/(#(((@&%**/**,,,,...*,,/*,/////***/(/////((, .(#,,  (%/,. .(#... .(#.,  ,#/ .  /#*   .(#, . .##    *#/ .  (#, 
// %%#%%%%%#%%####%%%%%#%%&&#@%,/*&(##%&&#%@&&%@#((/#/((*@@%#%#,/@@@@%%&&#%@@%/&@%&@@&%&@@&(//#%&%##(%%//((/**/***/////(/(/***//////((#/, .(#,.  /%(.. .(#,.. .(#..  ,#( .  /#/ .  (#* . .##  . *%(    /#* 
// %(%#%%%%%#%%%%%#%&@@@&(((#&&*,,&*@&((#%&@%#(///#(#(/((@@%(#(*/@@@@&&@@%%&&&&&&&&&&#@#%&@&%/(%@@&&#((((/////((//***///***/////////((//, .(#,, .(%(,. .(#*.. ./#..  ,#(..  *#( .  (#/ . .##  . ,##    /#/.
// %#%%%%&%&@&%%%%%%%#%&(###@#&&&%(*(#&##%&&&%%%%%#####(#@@%/((,%@@@@@@&&@@@@&&&&@@@&#%#%%%&@&#(&@@&&%%#(((((((((((//***//////////(((///, .(#,,  /##,. .(#/..  /#,.. ,(#.   *## .  /(( .  (#. . ,##    *#( 
// %%#%%%%%%%%&##%%%%&/@&&**#%,,(&@&##@&/&@&&%%%###@@##(*###(/#/%@@@@@@@@@&@@@@&@@@@@@&@@&&%&@@%##%&&&%#(((((((((((///////**///(((#(/////.,(#,,  *##,.  /#(.,  /#,.. .(#,   ,(# .  /## .  (#, . .##  . ,## 
// %%%%%%%%%%#%%%%/@%&&,,*%@@&&*#%((*&(((&&&&@@%(%%%%&@&&//(%@((@@%&@@@&@@@@@@@&&&&&%&&@@@@&&@@@&#(/(#####(#(((///********////#%%#///////*/(%,,  *##,.  /#(.,  *#*.. .(#.   ,(#    *##    /#* . .(#. . ,## 
// #%#%%#%%%%%%%%%**(@#&((#//(/#%@&%@&%#%#@&&&&%####/(###%@%#&/*&@&@@&@@@@@@@@@@@@%%&%&@@@&#%@@@@@&#(/((((/////*******//*/(/(#%#(****////*/##**../##,.  /##.,  *#*.. ./#,   ,(#    ,##  . /#/..  (#, . .##.
// &%%%%%%##%%&#@@@&%@@#,&@@%///#%@&@&(#%&@%@@@@%/*(@(/(%#/(/(&(/%#%&@&@@@@@@@@@&&&@@&%@%&&@%@@@@@@@&###((///****/***//((##%&(/********//*(###(* *##,,  *##.,  *#/.,  /#* . .(#. . ,(#. . *#(..  (#/ . .(#,
// %#%%%%#&@#(%/(@&#/##/(/(%/#%%%%@#@%(#%&&&&&&&&%%%@*///(//(/%#&@@@%%&@@@@@@@&@@&&%###%#&@@@@@@@@@@@@&&###((///////(##%&&#/***********/////%#//,/%/*,  *##,,. *#(.,  /#* . .(#, . ,(#, , *##... /#/ .  (#*
// %#%%%%@@&&(%*/,%%#/&.((#(//%@@%&&((#&&%@&@@@@&&%%##%@@&((#///@@@#%%@@@@@@@@@&&@@@@@@@@@@@@@@@@@@@@@@@@@&&&&%&&&%&&%/*****************//////@%,###*//**##,,. *#(.,  *#/.. .(#* . .(#* , ,##. . /#(..  /#/
// #%#%%&&&&&(/&/(%@&#./#%*%%%@@@#&%&&#%&%%%%###%%##&(%#(##(((#(#((%##(#(%&&@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&%#(///***********************/////////&#(#(/***(%,,. *(#,,. *#(.. ./#/ . ./#/ , ,##. . *##... /#(
// %#%%@@@@&@&*#(&#(&@%&/(/%#%%&%##%###%%((#%#((#%#(#(#(##/%#(#%%((#(((#&@&#((%&&(&@@@@@@@@@@@@&%##(((//(#%##(//*************,****************//#((#(#(/*##*,,.,(#,,. *#(,, ./#( , ./#(., ,(#, , ,##... *#(
// &&%@@@@@@&&#**#,/%%&&%#%%####%##(####%%(#%##(%%##(%/###(((#/%(%&&%%%%(#%@&%#%#%&%%%%&@%&&&%&&&%#((////(((((((///****************************%#####%##(#(/*,.*(%,,,.*##,, ./##., ./##,, .(#*., ,(#,.. *((
// ##&&@@@&&&@(/%%####%#%(##%####%#%(#####(######%%(#/(##%%%%%%%%%%%###%%##&@&&&%%&&&&%%%&&%%##%%%%#((///////////////*************************%######%###(((####((/,,.*#%,,../##,,../##,,.,(#(,, ,(#*.,.*((
// #%%%%&%%%%####%%%%#%%%##%%%%#%##%#%#%####(#####%#(#(#%%%%#%%%%%%%#%##%&&%#%&&&%&&&%&&#%%&&%%###%##((////*/////***************************%#%#####&%%%###(#(#####((#(#%**,,/#%,*,,/##*/**/#%**,*(#/,*,*##
// &%&&%%%&&%%%&&&%#%%%###%#&#%##%%%%##%#/(####%%#%##%##%%&%%#%%%&&%%%##%%#&&&&&&@&%&&%%&%%#%%%%&##%#((////**************************,***(%#####%%%&&&&%%%%#((#(###########(*/(%***,/##*/**/#%//*/(##//*/##
// &%%&%&&&%%&%&&&&&&&##&####%#%#&%%%%#%#%#(#(#&%#(#%&%#%##%%%%%%%%%%%####%%%&&&&&%@%&&&%%&&%%%%%%&&&%%(//*************************,***%####%%%%%&&%%%%%%%%#%(###########%%#%####///(#%/***/#%////(#%////(#
// %%%&%%&#%&%&&%&&&%&&%%&%##%%#%#%%#&%#######%%##(#%%%%#####&%%%#%%%%%#####%%&&%%&&&%&%%&%%%%&%%%%#%%%&%%#%#(***********,*******,/#%#%%#%%%%%%&&&%%%%%%%%%%%%%#(#########%%##%#%###(%%(///(#&#(//#%%(///##
// %%%%&&%%&&&%&&%%&%%&&%&&%##%#%%%#&#@%#%%##%%######%%%%#(##%%&%%%%%####%%%###%#%#%%%%%%%%%%%%%%&%%%%#%#%#%%%%%%#%%%##%##%########%%%%%%%%&&%%%%%%%%%%%%%%%%%%%%%##%#########%%#%####((#(((((((//(((((((#(
// %%%%%&%%&%&%%&&&&%%&&%&&&%%%##&#%&#%@%#&%##(#(%####%%%%(###%%%%%%%#%#%%##%(#%%#%%#%%%%%&%%%%%&%%%%%%%%%%%%##%%%%%%%%#%#%%#%##%%%%%%&%%&%%%%#######%%#%%%%%%%%%%%#####%%#%#%#%%#%%#%#####///////////*****
// %%%%%&%%&%%%%%&%&%%%%&&&&&%%%##%#%%#&&%#&%/####%%%#%%%%##(##%%%%%%%%%######%#%%##%##%%#%&%%%#%%%%%%%%%%%%%%%%%%%%%%%%%%%%#%%%%%%%%%%#######%#########%%%%%%%%%%%%%##%%##%%%%%%%#%#########%((((((((((((/
// %&#%%%%%%%&%%&%%%&%%&&%&%&&%&%%&%%&#&@%%%%####%%####%%%%##(####%%%%%######%#%##%(###%%%%%%%%###%%%%%%#%##%#%%%%%%%%%%%%%%%%%%%#%##########%############%##%%%#%####%%#%%#%%%#%%%%%%##%##%#%%((((((((((((
// %%%%%%%%%%%%&%&&&%&%&&%%&&%%&%%&%%%&%&&%%%%%%%%%%%%%###%%#######%%%%%%#####%##%%%%##%#%%%%%%%%##%%%%%%%#%%%%%%##%##%#%%%#%###%#%###########%###########%###%######%%%%%%%%%%##%%%####%####%###(((((((##(
// %%%&&%%%%%%&%&&%&&%%&&&&&&&&&%%&&%%%%&&#%%##%(%%%%%%###%%%##(#(##%%%%%%#%#%#%%%%#%%##%##%###%%%%%###%%%%%%%%%#%%%%#%##%#%###%#%%%###%####%#####################%%%&%%%%%#%%##%##%#%###(##%%######((#((##
// %&%%%&%%%%%&%%&&&%&%%&&&%%&%%%%&%&%%%&&%(%#%%##%%%&#%##%%%%%####%%%%%%%#%#%(%%&%%%%%%#%#%%###%%%%###%%%%%%#%%%%%%#%(%%%%##%%####%#########%#%#%%######%#######%#%%%%%#%#####%###%%%#%###%%%#####(((/#(((
// %%%%%%&&&&&%&&&&&&&%%&&%%%&&%%%&%%%%%&&%(##%#%#%%%%%###%%%%#####%##%%%%%%%%###%%&##%%##%(##%##%%%#%###%%#%#%%%#(%#%%#%#%#########%#%#%#%#%%%%%###%%%%#######%%%%%%######(#####%%%%####%%%%########(#(#%#
// %%&%%%&&&&&&%&&%&%&&&&&&&&%%%%&&&%%%#%%##%###%%%%&%%%%%###%%#%%%#%%###%%%%%%###%%%%(%&%%#####%##%%%##(##%%%%%%%%#%#%(%%%%#####%########%#%##%%%#%%%##%%##%#%%%%########%##%#%%###%#%##%%##%%##%%####%%%#
// #%%%%%&&&&&&&&&&%%%%%&&&&&%&%%%%%%%%###%#%%#%%%%%%&&%%%%%%%%%###%%###%%%%%%%###%%%%###&&%%##%%%%%%##%%###%#%%%#%%%%#%###%#######%%######%%%%##%#%%#%%%%#%%#%##(############%%#%#%%#####%##%%%%#######&&%
// &&%&%#%%&%&&&%&&&&%%%%&&&&%%%%%%%%%&(##%####%%###%%%%%%%%%%%%%%%#%##%%##%%%%%##%#%%&%##%&%#%%%#%%%%#%%%%##%%#%%#%#%#######%#%####%%########%###%###%%%%%%%%###((####%####%%###(##%###%###%%%%###%####%&&
// @&&%%%%%&%&&&&&%%%%%%%%&&%&%%%%&%%%#(##%#%%###%###%%%&%&#%##%%%####%(#%#%#%%%%%%#%#%&&#%%&%%#%%#%%%%%%%##(%#%####%#%%#%###%##%%#%#(%####%%#######%##%#%%%%%%%%%%#%%#######%#%((##%##%%%#%#%###%%######&&

// @@&%&@@@&&&%####(((((%&%((#%%&@@&%#(((((((///////(#########(((#(//////#%%###/,*/((#%###((#####%%%&%%#####%%%%%%%%%&&&&%###/,.....................,,,////(*,,,,**,.....................,******,,**,**//*,
// @@@@@@@&%&%#((((////(%%%##((#%%&&&%%##((((((//((##(((((#####(##((((///%@&&&%##%&&%%#%%%%##(((((((####%%%%&&&&&&&&&&&&&%%%#/,,,...................,,*/////*,,,*//,.,,..,,,,,,,,,,,,,,,,,,**,,,,,,,,,,.,**
// @@@@&&&%#(((((((#((#%%%%&%((#%#((###%##(((((((##############((((/(((//%&&&&%&&&&%%%####(#(((((#%%%&&@@@@@@@@@@@@@@@&&&&%#((#%%%%%###############(/*******,,,,.,*,,,,,,,*///************,,*********/*,,,*
// @@&%##(((((((((###(#%%%&&&%((((((#####((((((((((##%%###((####//(/(((//(/(%&&&&&&&&&&&&&&%%%%%%&&&&@@@@@@@@@@@@@@@@@@@@&&%##(##%##%##%%%&&&&&&&&%%(*****,,*,,,.,*,.,,,,,//////////(/((((/(#(*,...,/(/*,,*
// ((((((######(///((((#%%&&@&&%#(((((((((((((((((((((((((((((////////((/*/%&&&&&&&&&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&%#(###(*,,,..,,,...,,,,,,**,,,,,,*,,.,*,.,,,,*#%%#########((((((#/,,,,,,,*,,,,*
// ///////////////(((/(#%%&&&@&&%####%#(((((((((((((((((((/////((/((((((//#&&&&&&@&&&%%%%%%%%%&&&&&&&&&@@@@@@@@@@@@@@@@@@@&&%######/******,,........,,,,,,,,,....,,,,,,,,*(##((((((##########/,,,,,,,.....,
// ((((((/////////****/#%%%&&&&&%##############(((((((((((((((((((((///*,#&&@@&&%##(((((((((((((((((((###%%%&&&&@@@@@@@@@@&&%%&%%%/,,***,.........,*,,.,,*****,,,,...,,,,,,,,,,,,,,*******//,,,,,,,,.. ...,
// #((//*///////******(#%%%&&&&&%#########(((#####(#(((##(((((((((((/**,*%%&@@&%(//////////////////((((((((###%%&&&&&&@@@@@&&&&&&#*...,...........,*,,,,,,,,,,,,,,****,,,.................,..,,,,,.....   .
// ((((///******,,.,,*(%&&&&&&%(*********/(###(((((((#####((((/**/(//**/#&&&&%(/******///////////////////(((((##%&&&&@@@@@@@@@@@&#/**,,,****,,,,,,,..,,,,,,,,*,,,*****,,,**,,....................          
// **/(##(/**********(#&&&&&@&(*,,,,,,,,,,,**(((#############(/**/(//*/*(&&&%(//******/*//////////////////((((##%&&&@@@@@@@@@@@@&(,,,,,*///////((*,,.,,,,,,,*/****,****//*,................*//,,,,...      
// ##%##(((/**//****/#%&&&&@@%/*,,,,,,,,,,,,,*********//////*********//*/%&&%(*,.,,,,,,**//////////***////((((((##&&@@@@@@@@@@@@&(,..     .,,,,,,,,,,***,,,,,,,*,,,*//*,,,,,,,,,,............,*,,..    ....
// %#((((#(////(///((%&&&@@@&(*,,,,,,,,,,,,,,,,,,,,,,,,********,,,,,,*/*/#&&%/***********///////////////////(((###%&@@@@@@@@@@@&&(,,,,*,,,,,,,,//************,,,,,,,..,,*,,,,,***,,,,,,,,,.  .  .........,.
// %#(////((///((((##&&&&@@&(*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,************,*#&%#(##########(///((((##%%%%%%%######(###%%&@@@@@@@@@&#*,***,*****/**//****///(/*////*,,,,.....,,,,,........................,,,,.
// &&%%#((##(//###(##&&&@@@%*,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,*#&#//((##%%%##((///(((###%#%###%%%%%%##((##%&&@@@@@@@&%/,,*//**,,,,,/(/**///**((/**///**,,,,,,,,,*,,,..........,...  .............
// &&&&&%%%%####%###%&&&@@&#*,,,,,,,,,,,,,,,,*****,,,,******,,,,,,,,,,,,*(#(*/####%&&&%#/*,,*(##%%%%%%&&&%%%####((((#%&@@&&&&%%(//**********,****//(/**/////////****,..,,,*,,,***,,,*,,*,....,*,***,.......
// %#(((((((((((((((#&&@@@&(,,*,*****,,,,,,,,,,,****,,,,,,,,,,**,,,,,,,,,*/*,*/(((####(/*,.,*(########%%%%%%%###((((#%&&&%#####(,...,,.,,,*/(((/*****///**////////*******************,*,,,,,,***,****,,,...
// %&&%##((((((((((((%&@@@&(**********,,,,,,,,,,,,***,,,,,,,,,,,,,,,,,,,,,*,.,**///////*,..,/(((############(((((((((%&&%#((((#(,......,,..,,,,*//******,,*////*,,....,,,*,,,,,,,,,,,,,*///////////////////
// ((((((((#########((#%&@&(***********,,,,,,,,,,,,,,**,,,,,,,,,,,,,,,,,,,,....,,,,***,,,..*//(((((((((/////////(((((##(##%#(((/,.,,,,,,,,,,,,*********,,,****,..........,**,,,,.,,,,,,,,,,,,,,,,,,********
// &%####(((((//*//***//#%&(***********,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,******,,...,**//(((//////////////(((((((((##(((*,,,,,,,,,***/////(((/*,,,*****,..........,,,,,,,,,,,,.....................,.
// @@@@@@@@&&&%%#(((/////#&%(***********,,,,*,,,,,,,,,,,,,,,,,,,,,,,,,,,,***///((((//******///*((((((////**////((((((((((((((#%%%%&&&&&&&&&%%##(((#((#((/*,,,,,,,,,,......,,,,,,,,,........................
// @@@@@@@@@@@@@@@@@&&%%#%&&%/**********,****,,,,,,,,,,,,,*,,,,,,,,,,,,,,*///(((((/***/((######((((((((((/((((((((((((((///(%@@@@@@@@@@@&&&&%%###(((((####(//*,,,,,,,,,,,***************,,,,,,.,..........,
// &&&@@@@@@@@@@@@@@@@@@&@@@&/*//***/*************,,,,,,,,,*****,,,,******///(((**,,*/((######((//((((((((((((((((((###((((((((((((((((((((///////////////**/***,,,...........,,,,,****///(((((((((((((((((
// @@@&&&&&&&&&&&@@@@@@@@@@@@#///////////*********************,,,*////****////(((((((((((#####((((((((((((((((((((((#####(//////********************************,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,**********,
// @@@@@@@@@@@@@@@@@@@@@@@@@@&(///////////////*************,,*/(((////****////////(##(((((((###%%&%%##((((((((((((((((#%&&@@@&&&&&&&&&&&&&&&&&&%%%%%%%###((((((((((((((((((((((((//////////////////********
// @@@@@@@@@@@@@@@@@@@@@@@@@@@#//////////////////**********,/(#((///////***//////////(((((((#######(((((((((########((#%&&&&&&&&&&&&&&&&&&&&&&%%%%%%#(//***/(#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#########
// @@@@@@@@@@@@@@@@@@@@@@@@@@@%///////((((((((((////******/((((((((((((/////////////((((((########(((((((###########((((**************************************/(#%%%&%%%%%%%&&&&%&&%%&&&&&&&&&&&&&&&&&&&&&&
// @@@@@@@@@@@@@@@@@@@@@@@@@@@&(//((////((((((((((/////**/##((((((((((((((///////////(((####((#((((((((((##########(((((////***////////////////////////////******//(%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// &&@@@@@@@@@@@@@@@@@@@@@@@@@@&#//((((((((((######%%#(//####((((((((/((((#(((((////////////((((((((((############(((((//(##(/((((((((((((((((((((((((((((/////((/(((///(#&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// &@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#/////(((((((((####&@@%(######(((((((/(((((######//////((((((((((((###########(((((((/(#&@&%##((((((((((((((((((((((((((((((((((/(((#(((////((#%%&@@@@@@@@@@@@@@@@@@@@@@@@
// &&@@@&@@@@@@@@@@@@@@@@@@@@@@@@@&%((((##((((((#(##&@@@@@&&%%#######((((((#######%%#/((((((################(((((((((#&@@@&&&&&&%#######################(((((###(((((((###((((////((((((#%%%&&@@@@@@@@@@@@@
// &&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#//((((#(((((##%@@@@@@@@@&%%########%%%%%%%%&@@&#//((((########(#(((((((((((((#&@@@@@&&&&&&&&%##(######################((((#####(((((#%%##((((%&&%#((//////////((#%%%%
// &&@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@%(((((((((((((#&@@@@@@@&%##########%%%%%%%%&@@@%///((((((((((((((((((((((((#&@@@@@@@@@&&&&%%%%%%%&@@&%%%%%%%%#####################(((##%&%%#(#%&@@@@&&@&&&%%%##((////
// %##%%&&&&&@@@@@@@@@@@@@@@@@@&&@@@@@@#(((((((((((((%@@@@&%########%%%%%%%%%%%%&&@@@&#////((((((((((((((((#((#&@@@@@@@@@@@@&%%##%%%%%%###(////((((((((((#####%%%%#######%%######%&##%&&&&&&&&&&&&&&&&&&&&&
// %%%%%##%%%%%%%&&&&&&&&@@@@&&&&&&@@@@&(((((((((((((#@@&########%##%%%%%%%%%%%%&&@@@@%(////((((((((((((((((#&@@@@@@@@@@@@&&%%%%%%%%%%%%%%%#((//(((((((((###(#####%##%####%%&&%##%&%(//#%%%&&&&&&&&&&&&&&&&
// %%%%%%%%%%%%%%&&&&&&&&&&&&&&&&&&&&&&@%(/((////((((#&%##%%%%%%%%%%%%%%%%%%%%%&@&@@@&&(/////(((///((((((##&@@@@@@@@@@@@@&&%%%%%%%%%%%%%%%%%%%#(((((((((((#%%#(((#%%%%%%#%#%&@@@@@@@&#((#%&&&&&&&&&&&&&&&@@
// &&&&&&%%%%%%%%&&&&&&&@@@@@@@@@@@@@@@@@&#((((((((((##%%%%%%%%%%%%%%%%%%%%%%%%&@@&@@&&%(//////////((##&@@@@@@@@@@@@@@@&&&%%%%%%%#####%%%%%%%%%%%#(((((####(##(##&@@@&@@&&@@@@@@@&%##(((((###%%%&&&&&&&&@@@
// &&&&&%%%%%&&&&&&&&&&&@@@@@@@@@@@@@@@@@@@%((((((((#%%%%%%%%%%%%%%%%%%%%%%%%&&@@&@@&&&&&#(/((((#%&@@@@@@@@@@@@@@@@@@@&&%%%%%%%%%%%%#####%%&%%%%%%%#######%###%&@@@@@@@@@@@@@@@@@&@&&@@&&&&&%#((/////////((
// &&&&&%&&&&&&&&&&&&&&&&@@@@@@@@@@@@@@@@@@@&#(/(//#%%%%%%%%%%%%%%%%%%%%%%%%&&@@@&@&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#%%%%%%%&&@@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&@@&&&%%%
// &&&&&&&&&&&&@@@&&&&&&&@@@@@@@@@@@@@@@@@@@@@%(((#%%%%&%%%%%%%%%%%%%%%%%%%%&&&&&@&&&&&@@@@@@@@@@@@@@@@@@@&@@@@@@@@&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&&@@@@@@@@@@@@@@@@@@@%%%&&&&&&&&&&&&&&&&&&&&&
// @&&&&&&&@@@@@&&&&&&&&&&@@@@@@@@@@@@@@@@@@@@&(/#%%%%%&%%%%%%&%%%%%%%%%%%%%%&&&&&&&&&&&&&&&&&&&&@@@@@@@&&@@@@@@@@&%%%%%%%%%%%%%%%&&%%%%%%%%%%%%%%%%%%%%%%%%%&&&@@@@@@@@@@@@@@@@@@@%%&@@@@@@@@&&&@@&&@@@@&&
// &&&&&@@@@@@@@&@@@&&&@@@@@@@@@@@@@@@@@@@@@@@@@%%%%%%&&&%&&&&&%%%%%%%%%%%%%%&&&@&&&&&&&&&&&&&@@@@@@@&&&@@@@@@@&&&%%%%%%%%%%%%&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%&&&@@@@@&@@@@@@@@@@@@@%%%&&@@@@@@@@@@&%&&@@@@@
// &&&@@@@@@@@@&@@@&&&@@@@@@@@@@@@@@@@@@@@@@@@@&%%&&&&&&&&&&&&%%%%%%%%%%%%%%%%&&&&&&&&&&&&&@@@@@@@@&&&@@@@@@@&&%%#%%%%%%%%%&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%&&%&&@@@@@@@@@@@@@@@@@@&%#(((((((((#%%%%##%&&@@@@
// *****/////////////((((#%&@@@@@@@@@@@@@@@@@@@&%&&&&&&&&&&&&%%%%%%%%%%%%%%%%%&&&&&&&&&&@@@@@@@&&&&&@@@@@@@&&%%%%%&%%%%%%&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&@@@@@@@@@@@@@@@@@@@&&&&&&&&%%%####((((/////(
// **********************/(%@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%&&&&&&@@@@@@@@@&&&&@@@@@@@&&%%%%%%&&%%%%&@&%%%%%%%&%&&&&&&&&&&&&&&&&%&%%%%%%%%%&&@@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&@&&&&&&&
// ,,,,,,,,************//(%&@@@@@@@@@@@@@@@@&&&&&&&&@&&&&&&&&%%%%%%%%%%%%%%%%%%&&&&@@@@@@@@&&@@&@@@@@@@&%%%%%%%&&&&%%&@@&%%%%%%&&&&&&&&&&&&&&&&&&&&&&&&&&%&%%%%%&&@@@@@@@&&@@@@@@@@@@@@@@@&&&&&&&&&&&&&&&&&
// ,,,,,,,,,,,,,*******/(%&@@@@@@@@@@@@@@@@@@&&%&@&&&&&&@&&&&%%%%%%%%%%%%%%%%&%&&&@@@@@@@&&&&@&&@@@@@&%%%%%%%%&&&&&&&@&%%%%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%%%&&&@@@@@@&%&@@@@@@@@@@@@@@@@@@@@@@@@@@&&&@@&
// ,,,,,,,,,,,,,,,,,**/#&&@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&@&&&&%%%%%%%%%%%%%%&&%%&@@@@@@@&&&@&&&&@@@@&&&%%%%%&&&&@&&&@@&%&&&&%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@@&%&@@@@@@@@@@@@@@@@@@@@@@@@@@&&&@@@
// ,,,,,,,,,,,,,,,,*//#&&@@@@@@@@@@@@@@@@@&&&&%%&&@&&&&&&&&&&%%%%%%%%%%%%&&%%%%@@@@@@@&&&&&&&@&&&&&&&%%%%%&&&&@@&&@&&&@&&%&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@@@%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@&%&@@
// //((//**,,*******/#&&@@@@@@@@@@@@@@@@&&&%&%%&&&&&&&@&&&&&&%%%%%%%%%%%&&&%%%&@@@@@@&&&@@&&%&@&%&@&&%&&&&&&&@@&&@@&&&&&&@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@&&&@@@@@&%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&@@
// (((((((((/*,,,**(%&@@@@@@@@@@@@@@@@@&&&&%%%%%&&&&@@&@@@&&&%%%%%%%%%%&&%%%%&@@@@@@&&@@&&&%&@%%&&&&&&&&&&&&&@@&&@&@@&@@@&&&&&&%%&@@@&&&&&@&&&&&&&&&&&&@&&&@@&@@@&&&@@@@&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&@
// ((((((((((##/**(&&@@@@@@@@@@@@@@@@&&&&%%%%/,***/((###%@&&%&%%%%%%%&&%%%%&&@@@@@@&&@@&&&@@@&&&&&&&&&&&&&&&&@@@@@&&@@@&&&&&&&&@@@@@&&&&&&&&&&&&&&&&&&&&&@@&&@@@@@&@@&@&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&
// &@&&&&&&&&&&(/(&&@@@@@@@@@@@@@@@@@&&&%%%%(,......,,,*(&&&&%%%%%%%&%&%%%&&@@@@@@@@@&&%(#%%%%&@@&&&&&&&&@@&&@@@@@@@@@&&&&&&@@@@@@&&&&&&&&&&&&@@@&&&&@@&&&@@&&@@@@&@@@&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&
// ************/#&&@@@@@@@@@@@@@@@@@@&&&%%%(,,....,,,,,*/#&&&%%%%&&&&&%%%&@@@@@@@@&@&%%%&&&@@@&&&&&&&&&@@@@@&@@@@@@@@&&@@@@@@@@&&&&&&&&&@@@@@@&&&&&&&&@@@@&@@@@@@@&@@@@@@@@@@@@@@@@@@@@&&@@@@@@@@@@@@@@@@@@
// ,,,,,,,,,**(#&@@@@@@@@@@@@@@@@@@@&&&%%%#*,,,..,,,,,**/(%%%%%&&&&&%%#%&@@@@@@@@&&&%%%&&&&&&&&&&&&&&&&@@@@@@@@@@@&@@@@@@@&&&&@@@@@@@@@@@@@@@@@@@@@&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ,,,,,,,,**/%&&@@@@@@@@@@@@@@@@@@@&&%%&%/,,,.,,,,,,***/(%%%%%%&&&%#%%&@@@@@@@&&&&%%%%&&&&&&&&&&&&&&&@@@@@@@@@@@&&@@@@@@@@@@&&&&&&&@@@@@@@@&&&&@&@@@@@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ,,,,,,,**/%&&@@@@@@@@@@@@@@@@@@@@&&%&%(,,,,,,,,,,***/(#%%&&&&&%%%&&&@@@@@@@&&&&%%%%&&&&&&&&&&&&&@@@@@@@@@@@@@@@@@@@&&@@@@@@@@@@@@@@@&&&&&@@@@@@@@@@@@@@@@@@@@@&@&#**/((#%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// /,,,,,**(%&&@@@@@@@@@@@@@@@@@@@@&&&&%(,,,,,,,,,,***//(#%%&&&&&&&@@&@@@@@@@@&%&&&%%&&&&&&&&&&&&&@@@@@@@@@@@@@@&&&&&@@@@@@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&@@@@@@@@@&&@@@%(/(((#&@&@@@@&&&&&&&&&&&&&&&&&&&%#((
// (*,,,//#&&@@@@@@@@@@@@@@@@@@@@@@&&&&(,,,,,,,,,****//((#%%&&&&&@@@&&@@@@@@@&%&&&%%%&&&&&&&&&&&&&@@@@@@@@@@@@&&%&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#/((((#@@@@@@@@&&&&&&%%%%%%%%%%%%%%%
// (/***/#&&@@@@@@@@@@@@@@@@@@@@@@@&&&#*,,,,,,,,,***//(((#%%&&&&&@@&&@@@@@@@@&%&&%%%&&&&&&&&&&&@@@@@@@@@@@@@@@@&%%&&&&&&&&&&&&&&&&&&&&@@&&&&@&&&&@@@@@@@@@@@@@@@@@@@@@@#//((((&@@@@@@@@@@@@@&&&&&&&&&&&&&&&
// (/**/%&&@@@@@@@@@@@@@@@&&@@@@@@@&&%/,,,,,,,,*****///(((#%%&@@&&&&@@@@@@@@&%%&&%%&&&&@&&&&&&@@@@@@@@@@@@@@@@@&(/(##%%%&&&&&&&&&&&&&&&&&&&&&&&&@@@@@@@@@@@@@@@@@@@@@@&///(((#&@@@@@@@@@@@&&&&&&&&&&&&&&&&&
// ((//%&&@@&&&@@@@@@@@@@&&&@@@@@@@&%/,************/////((#%%%&@@&&@@@@@@@@&&&&&%%&&&@&&@@@@&&@@@@@@@@@@@@@@@@&(,,,,,,**//((###%%%%&&&&&&&&&&&&&&&&&&&&%#&@@@@@@@@@@@@#///(((%&@@@@@@@@@@@&&&&&&&&&&&&&&&%(
// (((%&&@@@&&@@@@@@@@@@&&&&@@@@@@@&(*,**********////////(#%%%&&&&&@@@@@@@@&&&%%%&&&&@@@@@@@@@@@@@@@@@@@@@@@@@&/,,,,,,,,,,******/*///////(((((((((((((((#@@@@@@@@@@&@#///(((#&@@@@@@@@@@@@&&&&&&&&&&&&&&&#*
// (#%&&@@@&&@@@@@@@@@&&&&&&@@@@@@@#*,,***//******////////#%%%&&&&@@@@@@@&&&&%%%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@#*,,,,,,,,,,,,,,,****////////////////(((((&@@@@@@@@@@&(/(((((#%@@@@@&&&&&&&&&&&&&&&&&&&&&&&&%
// %&&&@@@@&&@@@@@@@&&&&&&&&@@@@@@#*,,,***////******/////(#%%&&&&@@@@@@@&&&&%%%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@&(*,*,,,,,,,,,**********////////////(((((#%@@@@@@@@@%(/((((((#&@@@@@&&&&&%%%%&&&&@@@@@@@&&&&&&
// %%&@@@@@@@@@@@@@&&&&&&&&&@@@@&(,,,,,,**///////****///(##%%&&&&@@@@@@&&&&&%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%*,,,,,,,**************/////////((((((((#%@@@@@@@&#((((((((#&&@&&@@@&%%%%&@@@@@@@@@@@@&&&&&&&&
// %&@@@@@@@@@@@@@&&&&&&&&&&@@@%/,...,,,,***********///((#%%&&&&@@@@@@@@&&&%%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(,,**,,,,************///////(((((((((((#%@@@@@@%(((((((((#%&&@@@@@@@@@@@@@@@@@@@@@@&&&&%%%%%&&
// @@@@@@@@@@@@@@&&&&&&&&&&&@%/,.......,,,,,*********//(#%%&&&&@@@@@@@@@@&%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%*,,,,,*************///////(((((((((###%&@@&%##(((((((##%%%&&@@@&@@@@@@@@@@@@@@@&&&&%%%%%%%%%%%
// @@@@@@@@@@@@@@@@@@@@@@@&%*.................,,,,***//(#%%&&&&@@@@@@@@&%%%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&/,,,*,,**********////////(((((((###%%%%%%###(((((#%&&%%%%%%@@@@@@@@@@@@&&&&%%%%%%%%%%%%%%%%%%%%
// @@@@@@@@@@@@@@@@@@@@@@&(,,.................,,,,,**//(#%&&&&@@@@@@@&%%%&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&#*,*,,**********//(((######%%%%%&&&%%%%##(((##%&&&&%%%%%%%%%@@@@@@@@@&%###########%%%%%%%%%%%%%%
// &%%%&&&@@&&&@&&&&&&&&%/,,..................,,,,***/(#%%&@@&&@&&&%%##%&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&/*,************#%#%%%%%%%%%%%%%%%%%%%%%%&&@@&&&&&%%%%%%%%%#&@@@@@@@@@@@@@&&&&&%%%%%%%%%%%%%%%%%&
// ##(((((((((**,,.,,..,,,,.................,,,,,***/(##%&@@@&%###%%%%&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%**,***********(&@@@%(#%%&&&&&&@@@@@@&&&&&&&&&&&%%%%%&&&&%%%%&@@@@@@@@@@@@@@@@@@@@&&&&&&&&&&&&&@@
// ####((((((*........,,,,................,,,,,**//((####(((#%##%&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#**,,*********/&@@@@@&&&@@@&&&&&&&&&&&&&&&&&&&&&&%%%%&&&%%%#%@@@@@@@@&%&&&@@@@@@@&&&&&&&&&&&&&&&&
// #######((/........,,,,..............,,,,,,,**//((((###((####@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&(*,,,*********#&@@@@@&&&&&&&&&&&&&&&&&&&&@@@@@&%%%%%&&&&%%##%@@@@@@@&%(((#######%%%%&&&&&&&&&&&&&
// ######%#(,......,,,,.............,,,,,,,,***//(((#%######(//%&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%/,,,,,,******(&@@@@@@&&&&&@%%%%%%%%@@@@@@@&&&%%%%%&&&&%%%###&@@@@@@@&#(((#######%%%%%%%%%%%&&&&&&
// ######%%(,....,,,,..............,,,,,,,***/////(%%#((#(*,.,/(%&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&(*,,,,,******/#@@@@@@@&@&&&&%####%%@@@@@&&&&&&&&&&&&%%%%%###%&@@@@&@@&###########%%%%%%%%%%&&&&&&&
// #######%(,..,,,,,............,,,,,,*****/////(#%#/,,*,....,*(%&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%/,,,,,,*****/*(&@@@@@@&&@&&&%##%%&&&&&&&&&&&&&&&&&%%##%%&%###%@@@@@@@&%#######%%%%%%%%%%%%%%%%%%%%%
// %%#%###%(,,,,,,.........,,,,,,,*****///////(##(/*,.,,.....,//%@@&&@@@@@@@@@@@@@@@@@@@@@@@@@&&%%#((/,,,,,,,,*******/%@@@@@@&&&&&&&%%%%%%%%&&&&&&&&&&&%%%#%%%&&%####&@@@@@@@&####%%%%%%%%%%&&&&&&&&&&&&&&&
// %%%%###%(*,,,........,,,,,,,*****////////(##(*,.,,.,,.....,,/(%@@@@@@&&@@@@@@@&@@@&&&@&%#(*,,......,,,,,,,****////#@@@@@@@&&&&&&&%%%%&&&&@&&&&&&%%%%%#%&&&&%%####%@@@@@@@&&%%%%%%%%%&&%&&&&&&&&&&&&&&&&&
// %%%%%###/,.........,,,,,,****//////////(##(*,...,.........,,,/(#&@@@@@@@&&&%%&&@&%#/*,..,,..,,,,,,,,,,,*****/////(&@&@@@@&&&&&&&&&&&&&&&&&&&&&&&%%%%&@@@&&%&%%%##&@@@@@@@&&%%%%%%%%%&&%&&&&&&&&&&&&&&&&&
// %%%&&%/,..........,,,,,****///((////(##(/,.,...,,......,..,,,*/((%&@@&@&&&&%(/*,.........,,,,,,,,,,,,,****////(((%@&@@@@@&&&&@&&&&&&&&&&&&&%&&&&@@@@@@@@&&&&&%%%%&@@@@@@@&&%%%%%%%%%%%%&&&&&&&&&&&&&&&&&
