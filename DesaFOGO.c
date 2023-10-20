#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTALAGUA 3
#define TOTALFOGO 5
#define ALTURACAM 100*1.5
#define LARGURACAM 80*1.5
#define ALTURACIN 200*1.5
#define LARGURACIN 160*1.5

#define ALTURAPLAYER 40
#define LARGURAPLAYER 40
#define RAIOPLAYER 20

typedef struct {
    Vector2 pos; //posicao na tela
    float velocidade;
    int droptime;
    int pego;  //apagar quando cair no chao ou no jogador
    int caiu;  //essas variaveis levam 0 ou 1
} Caindo;

void Redimensiona(Texture2D imagem, int dimH, int dimV) { 
    imagem.width = dimH;
    imagem.height = dimV;
}

void PlayMusic(Music musica) {
    
    UpdateMusicStream(musica);

    // OBS: o if abaixo deve vir antes mesmo de declarar as variaveis como feito apaixo para evitar erros
    if(1) {
    	PlayMusicStream(musica); // se a musica nao estiver tocando, toca
    }
    
    float tempo_atual = GetMusicTimePlayed(musica);
    float tempo_total = GetMusicTimeLength(musica);
    
    if (tempo_atual >= tempo_total) {
    	StopMusicStream(musica);//garante que a musica seja tocada em loop
        PlayMusicStream(musica);
    }
}

void MenuInicial(Texture2D fundo_menu, Font menuFont, char* titulo, Vector2 fontPosition1, float tamanho_titulo, int screenWidth, int screenHeight, Music musica_menu) {
    PlayMusic(musica_menu);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Desenhar a imagem de fundo
    DrawTexture(fundo_menu, 0, 0, WHITE);

    // Desenhar o título do jogo centralizado
    DrawTextEx(menuFont, titulo, fontPosition1, tamanho_titulo, -1, RED);

    // Desenhar as informações de controles
    DrawText("O CIn precisa de voce! Precione ENTER para iniciar o jogo", screenWidth / 2 - MeasureText("O CIn precisa de voce! Precione ENTER para iniciar o jogo", 30) / 2, screenHeight * 3 / 4, 30, WHITE);

    EndDrawing();
}

// alem de carregar a imagem destinada ao plano de fundo do jogo, redimensiona para as dimensoes da tela
Texture2D LoadFundo(char *nome_arq, int dimH, int dimV) { 

    Texture2D fundo_atual = LoadTexture(nome_arq);

    // Redimensione a textura diretamente
    fundo_atual.width = dimH;
    fundo_atual.height = dimV;

    return fundo_atual;
}

// Função para exibir a tela de "Game Over"
void GameOver(bool *gameOver, Music music, bool *SairJanela) {
    
    const int screenWidth = 800 * 1.5;
    const int screenHeight = 600 * 1.5;
    
    // Carregue a imagem de fundo
    Texture2D backgroundTexture = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\cindesfocado.png", screenWidth, screenHeight);

    // Exibe a tela de "Game Over"
    while (!WindowShouldClose() && *gameOver == true) // Aguarda até que o jogador pressione R para reiniciar
    {
        
        PlayMusic(music); 
        
        BeginDrawing();

        ClearBackground(BLACK);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        
        DrawText("Game Over", GetScreenWidth() / 2 - MeasureText("Game Over", 40) / 2, GetScreenHeight() / 2 - 40, 40, RED);
        DrawText("Pressione R para reiniciar", GetScreenWidth() / 2 - MeasureText("Pressione R para reiniciar", 20) / 2, GetScreenHeight() / 2 + 20, 20, DARKGRAY);
        DrawText("Pressione ESC para sair", 30, 30, 20, DARKGRAY);
        DrawText("Vish! Infelizmente você não conseguiu salvar o CIn :(", 30, GetScreenHeight() / 2 + 220, 20, WHITE);
        DrawText("Agora Mateus ficará sem ao mosso e será punido com +1 falta em IC!", 30, GetScreenHeight() / 2 + 240, 20, WHITE);
        DrawText("Tente novamente para reverter essa situação!", 30, GetScreenHeight() / 2 + 260, 20, WHITE);

        
        EndDrawing();

        if (IsKeyPressed(KEY_R)) // Verifica se a tecla "R" (restart) foi pressionada
        {
           *gameOver = false; // Sai do estado de "Game Over"
        }
        if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
            (*SairJanela) = true;
        }
    }
    UnloadTexture(backgroundTexture);
}

void ElementosJogo(Texture2D elemento, Vector2 posicao){
    BeginDrawing();        
        DrawTextureEx(elemento, posicao, 0.0f, 2.0f, WHITE);
    ClearBackground(RAYWHITE);     
}

void ElementosJogo1(Texture2D elemento, Rectangle player, Vector2 position){
    BeginDrawing();        
     
    DrawTextureRec(elemento, player, position, WHITE);
    ClearBackground(RAYWHITE);     
}

void narrate_lore(int *frames_counter,char *msg1, char *msg2, int *flag) {
    if (IsKeyDown(KEY_SPACE)) (*frames_counter) += 8;
    else (*frames_counter)++;

    if(!(*flag))
    {
        DrawText(TextSubtext(msg1, 0, (*frames_counter)/5), 20, 20, 35, MAROON);
        if(IsKeyDown(KEY_T))
        {
            (*frames_counter)=0;
            (*flag) = 1;
        }
    }
    else if((*flag))
    {
        DrawText(TextSubtext(msg2, 0, (*frames_counter)/5), 20, 30, 35, MAROON);
    }

    if(IsKeyPressed(KEY_R))
    {
        (*frames_counter) = 0;
        (*flag) = 0;
    }
    DrawText("Pressione [ENTER] para JOGAR O JOGO!", 10 , 550, 25, LIGHTGRAY);
    DrawText("pressione [R] recomeçar a história!", 10, 585, 20, LIGHTGRAY);
    DrawText("pressione [T] para CONTINUAR A HISTÓRIA!", 10, 615, 20, LIGHTGRAY);
    DrawText("pressione [SPACE] para ACELERAR!", 10, 645, 20, LIGHTGRAY);
    DrawText("Controles do jogo:", 700, 530, 25, LIGHTGRAY);
    DrawText("-> ou <- para andar", 690, 560, 20, LIGHTGRAY);
    DrawText("Botão esquerdo do mouse NO PRÉDIO para descarregar as águas!", 600, 585, 18, LIGHTGRAY);
}

void GameWin(Caindo *agua, Caindo *fogo, int qtdAguaTotal, int qtdFogoTotal, Texture2D backgroundTexture, Texture2D aguaTexture, Texture2D fogoTexture, Texture2D caminhao_final, Texture2D bombeiroTexture, Music music){
    
    const int screenWidth = 800 * 1.5;
    const int screenHeight = 600 * 1.5;
    
    Redimensiona(backgroundTexture, screenWidth, screenHeight);
    Redimensiona(aguaTexture, 20, 20);
    Redimensiona(fogoTexture, 20, 20);
    Redimensiona(caminhao_final, 150, 150);
    Redimensiona(bombeiroTexture, (float)bombeiroTexture.width*15, (float)bombeiroTexture.height*15);

    Vector2 position = { 840, 230 }; 
    Rectangle player = { 0.0f, 0.0f, (float)bombeiroTexture.width/2, (float)bombeiroTexture.height/3};
    
    int currentFrame_x = 0, framesbombeiro_x = 0, currentFrame_y = 0, framesbombeiro_y = 0;
    
    //Contar os fogos e as aguas e o scoretotal
    int scoretotal = 0, aguapego = 0, fogopego = 0, todospego = 1, framesCounter = 0;
    
    for(int i=0; i<qtdAguaTotal; i++){
        aguapego += agua[i].pego;
        if(agua[i].pego == 1){
            todospego = 0;
        }
    }
    scoretotal += aguapego * 100;
    if(todospego == 1){
        scoretotal += 1000;
    }
    
    for(int i=0; i<qtdFogoTotal; i++){
        fogopego += fogo[i].pego;
    }
    
    scoretotal -= fogopego * 50;
    
    if(fogopego == 0){ // Desviou de todos os fogos
        scoretotal += 250;
    }
    //
    
    bool Sair = false;
    
    // Mensagem de agradecimento
    const char* agradecimento = "Obrigado por salvar o Cin, agora eu posso entregar minha lista a tempo :)";
    
    while (!WindowShouldClose() && !Sair) {
        
        PlayMusic(music); 
        
        framesCounter++;
        
        // Animação do bombeiro
        framesbombeiro_x++;
        framesbombeiro_y++;
        
        if (framesbombeiro_x == 10){
            
            framesbombeiro_x = 0;
            currentFrame_x++;

            if (currentFrame_x > 2) currentFrame_x = 0;

            player.x = (float)currentFrame_x*(float)bombeiroTexture.width/2;
        }
        
        if(framesbombeiro_y == 150){
            
            framesbombeiro_y = 0;
            currentFrame_y++;
            
            if(currentFrame_y > 1) currentFrame_y =0;
            
            player.y = (float)currentFrame_y*(float)bombeiroTexture.height/3;
        }
        //
        
        // Fundo
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(backgroundTexture, 0, 0, WHITE);

        // Msg de vitoria
        DrawText("Você Venceu!!", screenWidth / 2 - MeasureText("Você Venceu!!", 40) / 2, 100, 40, GOLD);
        
        DrawRectangleRounded((Rectangle){28, 220, 340, 160}, 0.3, 12, DARKGRAY);

        // Desenhar as estatísticas
        DrawText(TextFormat("Score: %d", scoretotal), 50, 240, 20, WHITE);
        DrawText(TextFormat("Águas Coletadas:   %d", aguapego), 50, 300, 20, BLUE);
        DrawText(TextFormat("Fogos Atingidos:   %d", fogopego), 50, 340, 20, RED);

        // Imagem de água e fogo ao lado das msg
        DrawTexture(aguaTexture, 290,297 , WHITE);
        DrawText(TextFormat("x"), 315, 297, 20, WHITE);
        DrawTexture(fogoTexture, 290, 337, WHITE);
        DrawText(TextFormat("x"), 315, 337, 20, WHITE);
        
        //Caminhão embaixo das estatísticas:
        DrawTexture(caminhao_final,0, 370, WHITE);
        
        //Desenhar bombeiro
        DrawTextureRec(bombeiroTexture, player, position, WHITE);
        
        // Desenhar a mensagem de agradecimento
       DrawText(TextSubtext(agradecimento, 0, framesCounter/3), (screenWidth - MeasureText(agradecimento, 20)) / 2, 580, 20, WHITE);

        // Voltar para o menu
        if(framesCounter >= 250)
            DrawText("Pressione ENTER para voltar ao jogo", (screenWidth - MeasureText("Pressione ENTER para voltar ao jogo", 20)) / 2, 630, 20, WHITE);
            
        if (IsKeyPressed(KEY_ENTER)) {
            Sair = true;
        }
        
        EndDrawing();
    }
}

//Pra qnd sair do gamewin reiniciar as variaveis:
void Zerartudo(bool *gameOver, int *mangueira, int *qtdAguatela, int *qtdAguatotal, int *qtdFogotela, int *qtdFogototal, int *fogocin, Color *corDireita1, Caindo **agua, Caindo **fogo){
            
            *mangueira = 0;
            *qtdAguatela = 0;
            *qtdAguatotal = 0;
            *qtdFogotela = 0;
            *qtdFogototal = 0;
            *fogocin = 22;
            *corDireita1 = BLACK;
            
           (*agua) = (Caindo *)realloc(*agua, 0 * sizeof(Caindo)); // Aloca inicialmente 0 elementos
           (*fogo) = (Caindo *)realloc(*fogo, 0 * sizeof(Caindo));
}

int main() {

    const int screenWidth = 800*1.5;
    const int screenHeight = 450*1.5;
    InitWindow(screenWidth, screenHeight, "DesaFogo");

    // variaveis do menu ----------------------------------

    // Carregar a imagem de fundo
    Texture2D fundo_menu = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\cindesfocado.png", screenWidth, screenHeight);
    Texture2D fundo_tela = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\cenario.png", screenWidth, screenHeight);
    Texture2D fundo_caminhao = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\caminhao.png", LARGURACAM, ALTURACAM);
    Texture2D aguaTexture = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\agua.png", 20, 20); //deixei 20, 20 mas pode mudar!
    Texture2D fogoTexture = LoadFundo("C:\\raylib\\raylib\\examples\\desafogo\\fogo.png", 20, 20);
    Texture2D bombeiroTexture = LoadTexture("C:\\raylib\\raylib\\examples\\desafogo\\bombeiro1.png");
    // Carregar as fontes e armazenar o texto
    Font menuFont = LoadFont("C:\\raylib\\raylib\\examples\\desafogo\\alagard.png");
    char titulo[] = "DesaFogo";
    float tamanho_titulo = 150.0f;
    
    // Carregar prédio do Cin
    Texture2D Cins = LoadTexture("C:\\raylib\\raylib\\examples\\desafogo\\cins.png"); 
    Vector2 posicao_cin = { screenWidth - LARGURACIN, screenHeight - ALTURACIN };
    Rectangle Rec_cin = { 0.0f, 0.0f, 240,(float)Cins.height };
    int currentFrame_cin = 0;

    // Calcular a largura do texto do título
    float tituloWidth = MeasureTextEx(menuFont, titulo, tamanho_titulo, 0).x;
    Vector2 fontPosition1 = {
        screenWidth / 2.0f - tituloWidth / 2, // centraliza horizontalmente
        (screenHeight - tamanho_titulo) / 3 // posiciona verticalmente
    };

    int frames_counter=0;
    int flag = 0;
    char msg[2][700];
    strcpy(msg[0],"Era uma noite de lua cheia no CIN, onde estudantes de Engenharia da\nComputação trabalhavam incansavelmente em seus projetos.\nTudo estava calmo, até que, de repente, o aroma delicioso de\npipoca começou a encher o ar. Não, não era um lanche comum - era\napenas matheus com H indo ao mossar.");
    strcat(msg[0],"\nMateus, conhecido por suas experiências inovadoras, estava\ntentando criar um algoritmo para fazer pipoca automaticamente \nem seu computador. Mas, como de costume, algo deu terrivelmente\nerrado.O código de Mateus entrou em um loop infinito e,\nem vez de fazer pipoca,seu computador começou a superaquecer.");
    strcpy(msg[1],"As chamas começaram a sair da torre do computador de Mateus,\ne um grito de pânico ecoou pelo CIN. Todos os estudantes\ncorreram para fora de suas salas, e logo o prédio estava\ncheio de alunos em pânico.");
    strcat(msg[1],"\n\nVocê, como o bombeiro não oficial do CIN, percebeu que alguém\n precisava assumir o controle da situação. Com sua bravura\ninabalável,você irá pegar todas as águas possíveis, desviar\ndos fogos e extinguir o incêndio heroicamente.");

    //a plataforma do lado--------
    Color corDireita = BLACK, corDireita1 = BLACK;
    //-----------------------------------
    
    //posicao e forma do jogador---------
    Vector2 posicaoplayer = { (float)screenWidth/6, ((float)screenHeight - (float)screenHeight*3/12) };
    Vector2 curr = { (float)screenWidth/6, ((float)screenHeight - (float)screenHeight*3/12) };
    Rectangle player = {posicaoplayer.x, posicaoplayer.y, (float)bombeiroTexture.width/2, (float)bombeiroTexture.height/3};
    int currentFrame = 0;
    int framesCounter = 0; //framecounter de dentro do jogo
    int framesSpeed = 8;
    //-----------------------------------
    
    //qtd de agua na mangueira-----------
    int mangueira = 0;
    int fogocin = 22;
    //-----------------------------------

    int qtdAgua = 0, qtdAguatela = 0, qtdAguatotal = 0; 
    int qtdFogo = 0, qtdFogotela = 0, qtdFogototal = 0;
    //qtd... = a quantidade sorteada, eu preciso criar uma variavel pra contar a quantidade de cada um NA TELA tambem
    //qtd...total = quantidade de cada um que caiu durante todo o jogo
    
    Caindo *aux = NULL, *agua = NULL, *fogo = NULL;
    //vetores para armazenar cada objeto que cai e as suas informacoes
    
    //ve a colisao dos objetos que caem com o jogador e a posicao que o mouse esta
    bool colisao;
    Vector2 posicaoMouse;

    bool gameOver = false;

    int cont_ENTER = 0; // conta a qtd em que ENTER foi pressionado
    int startGame = 0;
    // 0 --> menu
    // 1 --> instrucoes, controles
    // 2 --> jogo

    // variaveis musica menu e jogo
    InitAudioDevice();
    Music musica_menu = LoadMusicStream("C:\\raylib\\raylib\\examples\\desafogo\\bomb_guitarra.mp3"); PlayMusicStream(musica_menu);
    SetMusicVolume(musica_menu, 0.8f);
    Music musica_jogo = LoadMusicStream("C:\\raylib\\raylib\\examples\\desafogo\\musicajogo.mp3"); PlayMusicStream(musica_jogo);
    Music Vitoria = LoadMusicStream("C:\\raylib\\raylib\\examples\\desafogo\\Vitoria.mp3"); PlayMusicStream(Vitoria);
    Music m_keyboard = LoadMusicStream("C:\\raylib\\raylib\\examples\\desafogo\\keyboard.mp3"); PlayMusicStream(m_keyboard);

    bool SairJanela = false;
    SetTargetFPS(70);

    while (!WindowShouldClose() && !SairJanela) {
        if (IsKeyPressed(KEY_ENTER)) { // define qual etapa do jogo sera rodada
            cont_ENTER++;
            // faz sair do menu e ir para instrucoes
            if(cont_ENTER==1) startGame = 1;
            // quando enter eh precionado pela segunda vez (ao fim da tela de controles), o jogo comeca
            if(cont_ENTER>=2) startGame = 2;
        }

        if (startGame==0) { // if do menu
            MenuInicial(fundo_menu, menuFont, titulo, fontPosition1, tamanho_titulo, screenWidth, screenHeight, musica_menu);
        }
        else if(startGame==1) {            
            PlayMusic(m_keyboard);
            BeginDrawing();

            ClearBackground(BLACK);
            
            narrate_lore(&frames_counter, msg[0], msg[1], &flag);

            EndDrawing();
        }
        else if(startGame==2) { // if do JOGO
            PlayMusic(musica_jogo);

            // Update
            DrawTexture(fundo_tela, 0, 0, WHITE);
            DrawTexture(fundo_caminhao, 0, 500, WHITE);
            //----------------------------------------------------------------------------------
            posicaoMouse = GetMousePosition();
            corDireita = corDireita1;
            //^^pega a posicao do mouse em cada frame, esse negocio dos objetos comecarem cada frame com uma cor especifica e so um placeholder
            
            //--------------------------------------------------------
            if (!gameOver) {
            if(qtdAguatela < TOTALAGUA) {
                qtdAgua = GetRandomValue(0, TOTALAGUA); //sortea valor pra a qtd de pingos que vao cair
                qtdAguatela += qtdAgua;
                qtdAguatotal += qtdAgua;
                
                //realoca o vetor que contem todos os pingos de agua-------
                if(qtdAgua > 0) {
                    aux = (Caindo *) realloc(agua, qtdAguatotal * sizeof(Caindo));
                    if(aux == NULL) {
                        free(agua);
                        free(fogo);
                        exit(1);
                    }
                    agua = aux;
                    
                    for(int i=(qtdAguatotal - qtdAgua); i<qtdAguatotal; i++) { //o for abrange as 1, 2 ou 3 ultimas casinhas do vetor dependendo do numero sorteado
                        agua[i].caiu = 0;
                        agua[i].pego = 0;
                        agua[i].droptime = GetRandomValue(0, 100);
                        agua[i].velocidade = GetRandomValue(2.0f, 3.0f);
                        agua[i].pos.x = GetRandomValue(LARGURACAM + 20, screenWidth - LARGURACIN - 20);
                        agua[i].pos.y = 0; //posicao x aleatoria na tela mas sem passar dos dois blocos nos lados, posicao y la em cima
                    }
                }
                //-------------------------------------------------------------
            }
            //fazer cada pingo se mover em relacao a y em cada frame----------
            for(int i=0; i<qtdAguatotal; i++) {
                if(agua[i].droptime != 0) agua[i].droptime = GetRandomValue(0, 100);
                else {
                    agua[i].pos.y += agua[i].velocidade;
                    
                    if(agua[i].pos.y > posicaoplayer.y && !(agua[i].caiu || agua[i].pego)) { //checar se a agua ja nao caiu no chao ou no player antes tambem
                        agua[i].caiu = 1; //se a pos y do pingo for maior que a do player (ou seja, chao) ela apaga e a qtd na tela deveria diminuir
                        qtdAguatela--;
                    }
                }
            }
            //----------------------------------------------------------------
            
            
            //a mesma coisa so que com o fogo--------------------------------------------
            if(qtdFogotela < TOTALFOGO) {
                qtdFogo = GetRandomValue(0, TOTALFOGO);
                qtdFogotela += qtdFogo;
                qtdFogototal += qtdFogo;
                
                if(qtdFogo > 0) {
                    aux = (Caindo *) realloc(fogo, qtdFogototal * sizeof(Caindo));
                    if(aux == NULL) {
                        free(agua);
                        free(fogo);
                        exit(1);
                    }
                    fogo = aux;
                    
                    for(int i=(qtdFogototal - qtdFogo); i<qtdFogototal; i++) {
                        fogo[i].caiu = 0;
                        fogo[i].pego = 0;
                        fogo[i].droptime = GetRandomValue(0, 100);
                        fogo[i].velocidade = GetRandomValue(2.0f, 4.0f);
                        fogo[i].pos.x = GetRandomValue(LARGURACAM + 20, screenWidth - LARGURACIN - 20);
                        fogo[i].pos.y = 0;
                    }
                }
            }
            for(int i=0; i<qtdFogototal; i++) {
                if(fogo[i].droptime != 0) fogo[i].droptime = GetRandomValue(0, 100);
                else {
                    fogo[i].pos.y += fogo[i].velocidade;
                    if(fogo[i].pos.y > posicaoplayer.y && !(fogo[i].caiu || fogo[i].pego)) {
                        fogo[i].caiu = 1;
                        qtdFogotela--;
                    }
                }
            }
            //----------------------------------------------------------------------------------------
            
            
            //movimento do player--------------------------
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.y = ((float)bombeiroTexture.height)/3;//mudar o direcionamento do bombeiro!
                if (posicaoplayer.x + RAIOPLAYER + 10 < screenWidth - LARGURACIN) { //if pra fazer ele parar um pouco (10) antes de encostar nos dois blocos do lado
                    posicaoplayer.x += 5.0f;
                    player.x += 5.0f;
                }
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.y = 0.0f; //mudar o direcionamento do bombeiro!
                if (posicaoplayer.x - RAIOPLAYER - 10 > LARGURACAM) {
                    posicaoplayer.x -= 5.0f;
                    player.y += 5.0f;
                }
            }
            //--------------------------------------------
            
            
            //checar colisao com a agua e o fogo-------------------------
            for(int i=0; i<qtdAguatotal; i++) {
                colisao = CheckCollisionPointCircle(posicaoplayer, agua[i].pos, RAIOPLAYER + 15); //raio do circulo do player + raio do pingo/fogo (precisa definir isso depois)
                if(colisao && !(agua[i].caiu || agua[i].pego)) {
                    agua[i].pego = 1; //apaga o pingo e tira ele da tela
                    mangueira++;
                    if(mangueira > 5) mangueira = 5;
                    qtdAguatela--;
                }
            }
            
            for(int i=0; i<qtdFogototal; i++) { //o mesmo com o fogo
                colisao = CheckCollisionPointCircle(posicaoplayer, fogo[i].pos, RAIOPLAYER + 15);
                if(colisao && !(fogo[i].caiu || fogo[i].pego)) {
                    qtdFogotela--;
                    fogo[i].pego = 1;
                    mangueira -= 2;
                    
                    if(mangueira < 0) {
                        mangueira = 0; //e pra perder aqui
                        gameOver = true;
                    }    
                }
            }
            //------------------------------------------------------------
            if(fogocin == 22){
                currentFrame_cin = 0;
                Rec_cin.x = (float)currentFrame_cin*240;
            }
            
            //clicar na plataforma da direita-----------------------------
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (posicaoMouse.x > screenWidth - LARGURACIN && posicaoMouse.y > screenHeight - ALTURACIN) {
                        //se o mouse tiver na area da plataforma
                        if (posicaoplayer.x + RAIOPLAYER + 30 > screenWidth - LARGURACIN) {
                            //e o player tiver perto o suficiente dela
                            if(mangueira > 0) {
                                corDireita = MAROON; //muda a cor da plataforma so pra indicar que clicou
                                mangueira--;
                                fogocin--;
                                if(fogocin == 17){
                                    currentFrame_cin++;
                                    Rec_cin.x = (float)currentFrame_cin*240;
                                }
                                else if(fogocin == 12){
                                    currentFrame_cin++;
                                    Rec_cin.x = (float)currentFrame_cin*240;
                                }
                                else if(fogocin == 7){
                                    currentFrame_cin++;
                                    Rec_cin.x = (float)currentFrame_cin*240;
                                }
                                else if(fogocin == 3){
                                    currentFrame_cin++;
                                    Rec_cin.x = (float)currentFrame_cin*240;
                                }
                                else if(fogocin <= 0){
                                    currentFrame_cin++;
                                    Rec_cin.x = (float)currentFrame_cin*240;
                                    startGame = 3;
                                }
                            }
                        }
                    }
            }
        }
        //parte q vai fazer a animação do bombeiro---------------------------------------------------------------------    
        framesCounter++;

        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 1) currentFrame = 0;

            player.x = (float)currentFrame*(float)bombeiroTexture.width/2;
        }
        curr.x = posicaoplayer.x - player.width/4-30;
//----------------------------------------------------------------------------------------------------------------

            BeginDrawing();
            //desenha na tela a cada frame (o codigo inteiro tambem roda a cada frame acho)
            
            if (!gameOver) {
                ClearBackground(RAYWHITE);
            
                // Desenha o jogador e as plataformas (na posição e com as cores determinadas durante o código)
                ElementosJogo1(bombeiroTexture, player, curr /*position*/);
                DrawRectangle(screenWidth - LARGURACIN, screenHeight - ALTURACIN, LARGURACIN, ALTURACIN, corDireita);
                DrawTextureRec(Cins, Rec_cin, posicao_cin, WHITE); //predio do cin animado
                //------------------------------------------------------------------------------
            
                // Desenha cada pingo ou fogo de acordo com as informações atuais no código
                for (int i = 0; i < qtdAguatotal; i++) {
                    if ((!(agua[i].caiu || agua[i].pego)) && agua[i].droptime == 0) ElementosJogo(aguaTexture, agua[i].pos);
                    // Não desenhar se o pingo tiver sido apagado
                }
                for (int i = 0; i < qtdFogototal; i++) {
                    if ((!(fogo[i].caiu || fogo[i].pego)) && fogo[i].droptime == 0) ElementosJogo(fogoTexture, fogo[i].pos);
                }
                //------------------------------------------------------------------------------
                DrawRectangle(30, 30, 30 * 5, 30, GRAY);
                DrawRectangle(30, 30, 30 * mangueira, 30, SKYBLUE);
                
            } else if(gameOver == true) {
               
               GameOver(&gameOver, musica_menu, &SairJanela);
               Zerartudo(&gameOver, &mangueira, &qtdAguatela, &qtdAguatotal, &qtdFogotela, &qtdFogototal, &fogocin, &corDireita1, &agua, &fogo);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
        
        else if(startGame == 3){
            GameWin(agua, fogo, qtdAguatotal, qtdFogototal, fundo_menu, aguaTexture, fogoTexture, fundo_caminhao, bombeiroTexture, Vitoria); //brunao adicionou alguns argumentos aqui no dia 18/09
            Zerartudo(&gameOver, &mangueira, &qtdAguatela, &qtdAguatotal, &qtdFogotela, &qtdFogototal, &fogocin, &corDireita1, &agua, &fogo);
            startGame = 2;
        }
    }

    // unloads
    UnloadMusicStream(musica_jogo);
    UnloadMusicStream(musica_menu);
    UnloadMusicStream(m_keyboard);
    UnloadMusicStream(Vitoria);
    
    UnloadTexture(fundo_menu);
    UnloadTexture(fundo_tela);
    UnloadTexture(fundo_caminhao);
    UnloadTexture(aguaTexture);
    UnloadTexture(fogoTexture);
    UnloadTexture(bombeiroTexture);
    
    UnloadFont(menuFont);

    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}