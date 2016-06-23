/*
SeaQuest:
INF01202 Algoritmos e Programação (2014/1 Turma E)

Carolina Lange Mello e Natália Felix Tergolina
*/



#include <stdio.h>//printf
#include <string.h>//strcpy
#include <ctype.h>//tolower
#include <windows.h>//sleep
#include <conio.h>//gotoxy
#include <conio2.h>//textcolor e textbackground
#include <time.h>//time
#include <stdlib.h>//rand

#define MAXOXIGENIO 50
#define MAXMUNICAO 40
#define MAXVIDAS 5 //calculado para vidas no formato "S2 S2 S2 S2 ..."
#define MAXMERGULHADORES 6 //calculado para mergulhadores salvos no formato "~o ~o ~o ..."
#define DISTANCIADABORDALATERAL 5
#define SUPERFICIE 5
#define CHAO 22
#define LARGURASUBMARINO 9
#define ALTURASUBMARINO 3
#define DIFICULDADEMAXIMA 5

int menuprincipal();
int simounao();
void morte();
void apagarvidas();
void apagaroxigenio();
void apagarsubmarino();
void apagarmunicao();
void apagarmergulhadoressalvos();
void resetavariaveis();
void printmergulhadoressalvos();
void hud();
void printtempo();
void printoxigenio();
void printvidas();
void printscore();
void printmunicao();
void printsubmarino();
void criamergulhadores();
void criapeixes();
void criatiro();
void movemergulhador();
void movepeixes();
void movetiro();
void salvar();
void pausar();
void leaderboards(int decisao);
void quit(char *c);
void carregarjogo();
void novojogo();
void verificacolisao();
void mudadificuldade();
void howtoplay();
void move(char *c);

/*  Variáveis Globais
    -Usar com cuidado;
*/
int opcao=1, sairprograma=0, sairpartida, tempocarregado;//determina a continuidade do loop principal do jogo
char nome[26];
typedef struct str_jogo
{
    char nomearq[30];
    int x, y;//coordenadas do submarino
    int peixe[17], xp[17], yp[17];//sentido e posição de cada peixe possível
    int mergulhador[3], xm[3], ym[3];//sentido e posição de cada mergulhador possivel
    int tiro[MAXMUNICAO], xt[MAXMUNICAO], yt[MAXMUNICAO];
    int posicaoo2, posicaovidas, posicaomergulhadores, posicaomunicao;
    int score, morto, dificuldade, timer, sentido, tempoinicial, tempodecorrido;
}JOGO;

JOGO jogo;

/*  Função ResetaVariaveis:
    -Reseta todas variáveis iniciais para um novo jogo.
*/
void resetavariaveis()
{
    int i;
    jogo.x=37;//(meio da tela)-LARGURASUBMARINO/2)
    jogo.y=SUPERFICIE;
    jogo.posicaoo2=MAXOXIGENIO;
    jogo.posicaovidas=(MAXVIDAS*3)-1;
    jogo.posicaomergulhadores=0;
    jogo.posicaomunicao=MAXMUNICAO;
    jogo.score=0;
    jogo.morto=0;
    jogo.dificuldade=0;
    jogo.timer=0;
    jogo.sentido=-1;
    jogo.tempoinicial=time(NULL);
    jogo.tempodecorrido=0;

    for(i=0; i<20; i++)
        jogo.tiro[i]=0;
    for(i=0; i<17; i++)
        jogo.peixe[i]=0;
    for(i=0; i<3; i++)
        jogo.mergulhador[i]=0;
}

/*  Função HUD;
    -Imprime a parte estática da tela (letreiros e delimitações).
*/
void hud()
{
    int i;

    for(i=1; i<26; i++)
    {
        gotoxy(1, i);
        if(i>SUPERFICIE&&i<CHAO)
            textbackground(1);
        else
        if(i<=SUPERFICIE)
            textbackground(9);
        if(i>=CHAO)
            textbackground(2);

        printf("                                                                                ");
    }

    textcolor(15);
    textbackground(9);
    gotoxy(34, 1);
    printf("TIME: ");

    gotoxy(DISTANCIADABORDALATERAL, SUPERFICIE-2);
    printf("LIVES: ");

    gotoxy(DISTANCIADABORDALATERAL+7+((MAXVIDAS*3)-1)+4, SUPERFICIE-2);
    printf("SCORE: ");

    gotoxy(DISTANCIADABORDALATERAL+7+((MAXVIDAS*3)-1)+23, SUPERFICIE-2);
    printf("RESCUED: ");

    textbackground(2);
    gotoxy(DISTANCIADABORDALATERAL, CHAO+1);
    printf("OXIGEN: ");

    gotoxy(DISTANCIADABORDALATERAL, CHAO+2);
    printf("AMMO: ");


    for(i=1; i<=80; i++)//preenche a superficie da agua com "~"
    {
        textbackground(1);
        textcolor(9);
        gotoxy(i, SUPERFICIE+1);
        printf("~");
    }
}

/*  Função VerificaColisão:
    -Compara os X e Y de cada peixe/mergulhador com o X e Y do submarino.
*/
void verificacolisao()
{
    textbackground(1);
    int i, j;
//verifica a colisão entre peixes e tiros
    for(i=0; i<MAXMUNICAO; i++)
        for(j=0; j<17; j++)
            if((jogo.tiro[i]==1||jogo.tiro[i]==-1)&&(jogo.peixe[j])&&(jogo.xt[i]==jogo.xp[j]||jogo.xt[i]==jogo.xp[j]+1||jogo.xt[i]==jogo.xp[j]+2)&&(jogo.yt[i]==jogo.yp[j]))
            {
                Beep(100, 50);
                jogo.score=jogo.score+50+5*jogo.dificuldade;//50 pontos +5 pontos por nivel pra cada peixe acertado
                if(jogo.dificuldade==5)
                    jogo.score=jogo.score+125;
                gotoxy(jogo.xp[j], jogo.yp[j]);
                printf("   ");
                jogo.peixe[j]=0;
                jogo.tiro[i]=2;
                jogo.xp[j]=0;
            }
//verifica a colisão entre mergulhadores e tiros
    for(i=0; i<MAXMUNICAO; i++)
        for(j=0; j<3; j++)
            if((jogo.tiro[i]==1||jogo.tiro[i]==-1)&&(jogo.mergulhador[j]!=0)&&(jogo.xt[i]==jogo.xm[j]||jogo.xt[i]==jogo.xm[j]+1)&&(jogo.yt[i]==jogo.ym[j]))
            {
                Beep(100, 50);
                gotoxy(jogo.xm[j], jogo.ym[j]);
                printf("  ");
                jogo.mergulhador[j]=0;
                jogo.tiro[i]=2;
                jogo.xm[j]=0;
                if(jogo.dificuldade!=5)
                {
                    if(jogo.score-200>0)
                        jogo.score=jogo.score-200;
                    else
                        jogo.score=0;
                }
            }
//verifica a colisão entre peixes e submarino
    for(i=0; i<17; i++)
        if((jogo.peixe[i])&&((jogo.xp[i]==jogo.x+8||jogo.xp[i]==jogo.x+7||jogo.xp[i]==jogo.x+6||jogo.xp[i]==jogo.x+5||jogo.xp[i]==jogo.x+4||jogo.xp[i]==jogo.x+3||jogo.xp[i]==jogo.x+2||jogo.xp[i]==jogo.x+1||jogo.xp[i]==jogo.x||jogo.xp[i]+1==jogo.x||jogo.xp[i]+2==jogo.x)&&(jogo.yp[i]==jogo.y||jogo.yp[i]==jogo.y+1||jogo.yp[i]==jogo.y+2))&&!((jogo.xm[i]+2==jogo.x+2||jogo.xm[i]+2==jogo.x+1||jogo.xm[i]+2==jogo.x||jogo.xm[i]+1==jogo.x||jogo.xm[i]+1==jogo.x+2||jogo.xm[i]==jogo.x+5||jogo.xm[i]==jogo.x+6||jogo.xm[i]==jogo.x+7||jogo.xm[i]==jogo.x+8)&&(jogo.ym[i]==jogo.y)))
        {
            Beep(700, 50);
            jogo.peixe[i]=0;
            gotoxy(jogo.xp[i], jogo.yp[i]);
            printf("   ");
            printsubmarino();
            jogo.xp[i]=0;
            jogo.posicaoo2=0;
        }
//verifica a colisão entre mergulhadores e submarino
    for(i=0; i<3; i++)
        if((jogo.mergulhador[i])&&((jogo.xm[i]==jogo.x+8||jogo.xm[i]==jogo.x+7||jogo.xm[i]==jogo.x+6||jogo.xm[i]==jogo.x+5||jogo.xm[i]==jogo.x+4||jogo.xm[i]==jogo.x+3||jogo.xm[i]==jogo.x+2||jogo.xm[i]==jogo.x+1||jogo.xm[i]==jogo.x||jogo.xm[i]+1==jogo.x)&&(jogo.ym[i]==jogo.y||jogo.ym[i]==jogo.y+1||jogo.ym[i]==jogo.y+2))&&!((jogo.xm[i]+1==jogo.x||jogo.xm[i]+1==jogo.x+2||jogo.xm[i]==jogo.x+5||jogo.xm[i]==jogo.x+6||jogo.xm[i]==jogo.x+7||jogo.xm[i]==jogo.x+8)&&(jogo.ym[i]==jogo.y)))
        {
            Beep(200, 50);
            jogo.mergulhador[i]=0;
            gotoxy(jogo.xm[i], jogo.ym[i]);
            printf("  ");
            printsubmarino();
            jogo.xm[i]=0;
            if(jogo.posicaomergulhadores<MAXMERGULHADORES*3)
                jogo.posicaomergulhadores=jogo.posicaomergulhadores+3;

            if(jogo.posicaomergulhadores<=MAXMERGULHADORES*3)
                printmergulhadoressalvos();

            if(jogo.dificuldade==5)
            {
                jogo.posicaomunicao=MAXMUNICAO;
                for(i=0; i<20; i++)
                    if(jogo.tiro[i]==2)
                        jogo.tiro[i]=0;
            }
            printmunicao();
        }
}

/*  Função PrintSubmarino:
    -Imprime o submarino com o submarino[0][0] na posição (x, y);
*/
void printsubmarino()
{
    char submarino[ALTURASUBMARINO][LARGURASUBMARINO];

    /*
       ▀█
    ▄██████▄█
    ▀██████▀█

        █▀
    █▄██████▄
    █▀██████▀

    (char)223=▀
    (char)219=█
    (char)220=▄
    */
    textcolor(8);

    if(jogo.sentido<0)
    {
        if(jogo.y==SUPERFICIE)
            textbackground(9);
        else
            textbackground(1);

        gotoxy(jogo.x+3, jogo.y);
        printf("%c%c", (char)223, (char)219);

        textbackground(1);
        gotoxy(jogo.x, jogo.y+1);
        printf("%c%c%c%c%c%c%c%c%c",(char)220, (char)219, (char)219, (char)219, (char)219, (char)219, (char)219, (char)220, (char)219);

        gotoxy(jogo.x, jogo.y+2);
        printf("%c%c%c%c%c%c%c%c%c", (char)223, (char)219, (char)219, (char)219, (char)219, (char)219, (char)219, (char)223, (char)219);
    }
    else
    {
        if(jogo.y==SUPERFICIE)
            textbackground(9);
        else
            textbackground(1);

        gotoxy(jogo.x+4, jogo.y);
        printf("%c%c", (char)219, (char)223);

        textbackground(1);
        gotoxy(jogo.x, jogo.y+1);
        printf("%c%c%c%c%c%c%c%c%c", (char)219, (char)220, (char)219, (char)219, (char)219, (char)219, (char)219, (char)219, (char)220);

        gotoxy(jogo.x, jogo.y+2);
        printf("%c%c%c%c%c%c%c%c%c", (char)219, (char)223, (char)219, (char)219, (char)219, (char)219, (char)219, (char)219, (char)223);
    }
}

/*  Função PrintMergulhador/PrintPeixe:
    -Imprime o mergulhador/peixe "i".
*/
void printmergulhador(int i)
{
    textcolor(9);
    textbackground(1);
    gotoxy(jogo.xm[i], jogo.ym[i]);

    if(jogo.mergulhador[i]==-1&&jogo.xm[i]>1)
        printf("o<");
    else
    if(jogo.mergulhador[i]==-1&&jogo.xm[i]==1)
        printf("<");
    else
    if(jogo.mergulhador[i]==1&&jogo.xm[i]==80)
        printf(">");
    else
    if(jogo.mergulhador[i]==1&&jogo.xm[i]<80)
        printf(">o");
}
void printpeixe(int i)
{
    textbackground(1);

    if(jogo.dificuldade==0)
        textcolor(3);
    else
    if(jogo.dificuldade==1)
        textcolor(13);
    else
    if(jogo.dificuldade==2)
        textcolor(14);
    else
    if(jogo.dificuldade==3)
        textcolor(12);
    else
    if(jogo.dificuldade==4)
        textcolor(5);
    else
    if(jogo.dificuldade==5)
        textcolor(0);


    gotoxy(jogo.xp[i], jogo.yp[i]);

    if(jogo.peixe[i]==-1&&jogo.xp[i]>2)
        printf("<><");
    else
    if((jogo.peixe[i]==-1&&jogo.xp[i]==2)||(jogo.peixe[i]==1&&jogo.xp[i]==79))
        printf("><");
    else
    if(jogo.peixe[i]==-1&&jogo.xp[i]==1)
        printf("<");
    else
    if(jogo.peixe[i]==1&&jogo.xp[i]<79)
        printf("><>");
    else
    if(jogo.peixe[i]==1&&jogo.xp[i]==80)
        printf(">");
}

/*  Função SimOuNão:
    -Imprime na tela a opção SIM e a opção NÃO;
    -O usuário usa as setas para escolher e pressiona enter para confirmar a decisão;
    -A função retorna um valor, 1=sim ou 0=não;
*/
int simounao()
{
    int i=1, ysn=19, csn, decisao;
    char e, f;
    e=(char)205;
    f=(char)186;
    //imprime o SIM
    gotoxy(35, ysn-3);
    printf("  Y E S  ");
    //imprime o NAO
    gotoxy(36, ysn-1);
    printf("%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, (char)187);
    gotoxy(36, ysn);
    printf("%c N O %c", f, f);
    gotoxy(36, ysn+1);
    printf("%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, (char)188);
    //escolher se sim ou nao utilizando as setas:
    while(i)
    {
        if(kbhit)
        {
            csn=getch();
            if(csn==-32)
                csn=getch();
        }
        if(csn==72&&ysn!=16)
        {
            gotoxy(36, ysn-1);
            printf("       ");
            gotoxy(36, ysn);
            printf("  N O  ");
            gotoxy(36, ysn+1);
            printf("       ");
            ysn=ysn-3;
            Beep(350, 90);
            gotoxy(35, ysn-1);
            printf("%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, (char)187);
            gotoxy(35, ysn);
            printf("%c Y E S %c", f, f);
            gotoxy(35, ysn+1);
            printf("%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, (char)188);
        }
        if(csn==80&&ysn!=19)
        {
            gotoxy(35, ysn-1);
            printf("         ");
            gotoxy(35, ysn);
            printf("  Y E S  ");
            gotoxy(35, ysn+1);
            printf("         ");
            ysn=ysn+3;
            Beep(350, 90);
            gotoxy(36, ysn-1);
            printf("%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, (char)187);
            gotoxy(36, ysn);
            printf("%c N O %c", f, f);
            gotoxy(36, ysn+1);
            printf("%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, (char)188);
        }
        if(csn==13||csn==8)
            i=0;
    }
    if(csn==8)
    {
        Beep(150, 90);
        decisao=0;
    }
    else
    {
        if(ysn==19)
        {
            Beep(150, 90);
            decisao=0;
        }
        if(ysn==16)
        {
            Beep(500, 90);
            decisao=1;
        }
    }
    return decisao;
}

/*  Função  Salva, Pause, Quit, CarregaJogo e NovoJogo:
    -Podem ser chamadas durante a partida;
    -Pause congela a tela e salva o jogo no seu estado atual;
    -Quit sai do jogo e salva;
    -Salva apenas salva o jogo no seu estado atual;
    -NovoJogo salva e inicia um novo jogo;
*/
void salvar()
{
    jogo.score++;

    //salva o jogo em um arquivo bin com o nome do jogador
    FILE*save;
    save=fopen(jogo.nomearq, "wb");
    rewind(save);
    fwrite(&jogo, sizeof(JOGO), 1, save);
    fclose(save);
    //////////////////////////////////////////////
    //salva as informaçõs do jogador no highscore
    FILE*highscore;

    char matrizaux[1000][30], buffer[25];
    int linha=0, coluna, i, j, scoreauxi, scoreauxj;

    //salva os dados do player no FINAL do highscores.txt
    highscore=fopen("highscores.txt", "a");
    //imprime no txt o nome do player
    fputs(nome, highscore);//imprime nome no arquivo
    fputs("\n", highscore);//pula linha
    //imprime no txt o score do player
    sprintf(buffer, "%d", jogo.score);//coloca o inteiro jogo.score em formato de string na string buffer
    fputs(buffer, highscore);//imprime o score no arquivo
    fputs("\n", highscore);//pula linha
    fclose(highscore);//fecha o arquivo

    //copia o arquivo para a matriz auxiliar, ordena a matriz e depois imprime a matriz no arquivo
    highscore=fopen("highscores.txt", "r+");//abre para leitura e escrita SEM substituir arquivo
    rewind(highscore);//posiciona o ponteiro no inicio do arquivo
    linha=0;//inicia com 0 o contador de quantas linhas tem o arquivo highscores.txt

    //copia o arquivo para a matriz
    while(!feof(highscore))
    {
        fscanf(highscore,"%s\n", buffer);//le uma linha e pula pra proxima
        strcpy(matrizaux[linha], buffer);//cada string lida é armazenada na posição [linha] da matrizaux
        linha++;//conta quantas linhas foram lidas
    }
    fclose(highscore);//fecha o arquivo txt

    //agora lida-se apenas com a matriz para substituir scores de nomes iguais
    for(i=0; i<linha; i=i+2)
    {
        for(j=i+2; j<linha; j=j+2)
        {
            if(strcmp(matrizaux[i], matrizaux[j])==0)
            {
                scoreauxi=atoi(matrizaux[i+1]);
                scoreauxj=atoi(matrizaux[j+1]);
                if(scoreauxj>scoreauxi)
                {
                    strcpy(matrizaux[i+1], matrizaux[j+1]);
                    strcpy(matrizaux[j], "");
                    strcpy(matrizaux[j+1], "");
                }
            }
        }
    }

    //ordena os nomes e scores do maior para o menor
    for(i=0; i<linha; i=i+2)//le a primeira linha, compara com todas outras que vem depois. Le a terceira linha, compara com todas outras que vem depois...
    {                       //avança de 2 em 2 a partir do 0. Dessa forma, apenas os nomes serão lidos
        for(j=i+2; j<linha; j=j+2)//compara cada i com todos nomes/scores seguintes para ver se acha algum maior
        {

            scoreauxi=atoi(matrizaux[i+1]);//se i e j pulam de 2 em 2 e leem apenas nomes, i+1 e j+1 pulando de 2 em 2 são os scores de i e j
            scoreauxj=atoi(matrizaux[j+1]);//armazena a string do score em uma variável int para que seja efetuada a comparação de grandesas
            if(scoreauxj>scoreauxi)//se achar um maior
            {
                //inverte os scores da posição i+1 para j+1 e vice-versa
                strcpy(buffer, matrizaux[j+1]);
                strcpy(matrizaux[j+1], matrizaux[i+1]);
                strcpy(matrizaux[i+1], buffer);
                //inverte os nomes da posição i para j e vice-versa
                strcpy(buffer, matrizaux[j]);
                strcpy(matrizaux[j], matrizaux[i]);
                strcpy(matrizaux[i], buffer);
            }

        }
    }


    highscore=fopen("highscores.txt", "w+");//abre aquivo teste para imprimir o que seria impresso no highscores
    for(i=0; i<linha; i++)//imprime toda matriz
    {
        fputs(matrizaux[i], highscore);//imprime a string [i]
        fputs("\n", highscore);//pula a linha
    }
    fclose(highscore);

    Beep(500, 150);//avisa quando salva
    Beep(350, 150);
    Beep(200, 150);
}
void pausar()
{
    salvar();

    int xcentro=31;
    char e=(char)205, a;

    textbackground(0);
    textcolor(15);
    gotoxy(xcentro, 11);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)187);
    gotoxy(xcentro, 12);
    printf("%c                 %c", (char)186, (char)186);
    gotoxy(xcentro, 13);
    printf("%c   P A U S E D   %c", (char)186, (char)186);
    gotoxy(xcentro, 14);
    printf("%c                 %c", (char)186, (char)186);
    gotoxy(xcentro, 15);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)188);
    gotoxy(xcentro-2, 16);
    printf("press 'p' key to resume");
    do{
        a=getch();
    }while(a!='p');
    textbackground(1);
    gotoxy(xcentro, 11);
    printf("                   ");
    gotoxy(xcentro, 12);
    printf("                   ");
    gotoxy(xcentro, 13);
    printf("                   ");
    gotoxy(xcentro, 14);
    printf("                   ");
    gotoxy(xcentro, 15);
    printf("                   ");
    gotoxy(xcentro-2, 16);
    printf("                       ");
    Beep(500, 90);

    printsubmarino();

    int i;

    for(i=0; i<17; i++)
        printpeixe(i);

    for(i=0; i<3; i++)
        printmergulhador(i);
}
void quit(char *c)
{
    char e=(char)205;
    int decisao;

    textbackground(0);
    textcolor(15);
    gotoxy(17, 9);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)187);
    gotoxy(17, 10);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 11);
    printf("%c   ARE YOU SURE YOU WANT TO QUIT THE GAME?   %c", (char)186, (char)186);
    gotoxy(17, 12);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 13);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 14);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 15);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 16);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 17);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 18);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 19);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 20);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 21);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)188);

    c=0;

    decisao=simounao();
    if(decisao)
    {
        salvar();
        menuprincipal();
    }

    int i;
    for(i=SUPERFICIE+2; i<CHAO; i++)
    {
            textbackground(1);
            gotoxy(1, i);
            printf("                                                                                ");

    }
    printsubmarino();
    for(i=0; i<17; i++)
        printpeixe(i);
    for(i=0; i<3; i++)
        printmergulhador(i);
}
void carregarjogo()
{
    int i;
    FILE*save;
    textbackground(0);
    textcolor(15);

    clrscr();

    gotoxy(32, 13);
    printf("TYPE IN YOUR NAME:\n");
    gotoxy(32, 14);
    scanf("%s", jogo.nomearq);
    strcpy(nome, jogo.nomearq);
    strcat(jogo.nomearq, ".bin");

    while((save=fopen(jogo.nomearq, "rb"))==NULL)
    {
        textbackground(0);
        textcolor(15);
        clrscr();
        gotoxy(20, 13);
        printf("UNABLE TO LOAD GAME, CHOOSE A DIFFERENT NAME");
        gotoxy(20, 14);
        printf("     or enter 'N' to create a new game");
        gotoxy(19, 15);
        printf(" ");
        scanf("%s", jogo.nomearq);

        strcpy(nome, jogo.nomearq);
        strcat(jogo.nomearq, ".bin");

        if(strcmp(jogo.nomearq,"n.bin")==0||strcmp(jogo.nomearq,"N.bin")==0)
            novojogo();
    }
    fread(&jogo, sizeof(JOGO), 1, save);
    fclose(save);
    if(jogo.morto)
    {
        clrscr();
        gotoxy(26, 13);
        printf("THIS QUEST HAS ALREADY FAILED!");
        gotoxy(23, 14);
        printf("load another game or start a new one");
        while(!kbhit());
        getch();
        abrirsubmenu(11);
    }

    clrscr();
    hud();//função chamada para imprimir o HUD permanente na tela
    printsubmarino();
    printscore();
    printmergulhadoressalvos();

    for(i=0; i<3; i++)
        printmergulhador(i);
    for(i=0; i<17; i++)
        printpeixe(i);
}
void novojogo()
{
    FILE*save;

    clrscr();

    gotoxy(22, 13);
    printf("TYPE IN YOUR NAME:");
    gotoxy(22, 14);
    scanf("%s", jogo.nomearq);
    if(strlen(jogo.nomearq)>25)
    {
        clrscr();
        gotoxy(19, 13);
        printf("YOUR NAME MUST NOT HAVE MORE THAN 25 CHARACTERS!");
        while(!kbhit());
        getch();
        novojogo();
    }

    strcpy(nome, jogo.nomearq);
    strcat(jogo.nomearq, ".bin");

    while((save=fopen(jogo.nomearq, "rb"))!=NULL)
    {
        clrscr();
        gotoxy(18, 13);
        printf("'%s' ALREADY IN USE, CHOOSE A DIFFERENT NAME", nome);
        gotoxy(21, 14);
        printf(" ");
        scanf("%s", jogo.nomearq);

        strcpy(nome, jogo.nomearq);
        strcat(jogo.nomearq, ".bin");
    }
    resetavariaveis();
    hud();//função chamada para imprimir o HUD permanente na tela
    printsubmarino();
    tempocarregado=jogo.tempoinicial;
    salvar();
}

/*  Função  Leaderboards:
    -Chamada ao selecionar a terceira opção do menu principal;
    -Imprime o que está no arquivo highscores.bin;
    -A organização deverá ser *nome* em cima, *score* embaixo e *pular linha* por último.
*/
void leaderboards(int decisao)
{
    if(!decisao)
        salvar();

    FILE *highscore;
    int linha;
    char matrizaux[1000][30], buffer[30], i, j, scoreauxi, scoreauxj;

    //copia o arquivo para a matriz auxiliar, ordena a matriz e depois imprime a matriz no arquivo
    highscore=fopen("highscores.txt", "r+");//abre para leitura e escrita SEM substituir arquivo
    rewind(highscore);//posiciona o ponteiro no inicio do arquivo
    linha=0;//inicia com 0 o contador de quantas linhas tem o arquivo highscores.txt

    //copia o arquivo para a matriz
    while(!feof(highscore))
    {
        fscanf(highscore,"%s\n", buffer);//le uma linha e pula pra proxima
        strcpy(matrizaux[linha], buffer);//cada string lida é armazenada na posição [linha] da matrizaux
        linha++;//conta quantas linhas foram lidas
    }
    fclose(highscore);//fecha o arquivo txt

    char e=(char)205;

    textbackground(0);
    textcolor(15);
    gotoxy(17, 7);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)187);
    gotoxy(17, 8);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 9);
    printf("%c    1:                                       %c", (char)186, (char)186);
    gotoxy(17, 10);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 11);
    printf("%c    2:                                       %c", (char)186, (char)186);
    gotoxy(17, 12);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 13);
    printf("%c    3:                                       %c", (char)186, (char)186);
    gotoxy(17, 14);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 15);
    printf("%c    4:                                       %c", (char)186, (char)186);
    gotoxy(17, 16);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 17);
    printf("%c    5:                                       %c", (char)186, (char)186);
    gotoxy(17, 18);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 19);
    printf("%c                                             %c", (char)186, (char)186);
    gotoxy(17, 20);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, (char)188);


    for(i=0; i<10; i++)
    {
        gotoxy(25, 9+i);
        printf("%s\n", matrizaux[i]);
    }

    while(!kbhit());
    getch();

    if(decisao)
        menuprincipal();
    else
    {
        for(i=SUPERFICIE+2; i<CHAO; i++)
        {
                textbackground(1);
                gotoxy(1, i);
                printf("                                                                                ");
        }
        printsubmarino();
        for(i=0; i<17; i++)
            printpeixe(i);
        for(i=0; i<3; i++)
            printmergulhador(i);
    }
}

/* Função HowToPlay:
    -Pega o arquivo txt denominado intrucoes, o qual contem as instrucoes necessarias para o jogador;
    -Imprime o arquivo txt na tela.
*/
void howtoplay()
{
    int contl, contc; //contadores de linha e coluna
    char str[24][81]; //o maximo horizontal da tela eh 24, e o maximo da tela vertical eh 81 (para que tudo apareca)
    FILE *instrucoes;

    instrucoes = fopen ("instrucoes.txt", "r"); //abre o arquivo txt para leitura

    textbackground(0);
    textcolor(15);

    for (contl=0; contl<24; contl++) //para percorrer todas as linhas
    {
        fscanf(instrucoes, "%s", str[contl]);

        for (contc=0; contc<81; contc++)
        {
            if(str[contl][contc]==64) //se for @
                str[contl][contc]=' '; //substitui por um espaço
            if(str[contl][contc]==35) //se for #
                str[contl][contc]=201;
            if(str[contl][contc]==37) //se for %
                str[contl][contc]=187;
            if(str[contl][contc]==36) //se for $
                str[contl][contc]=200;
            if(str[contl][contc]==38) //se for &
                str[contl][contc]=188;
            if(str[contl][contc]==33) //se for !
                str[contl][contc]=205;
            if(str[contl][contc]==42) //se for *
                str[contl][contc]=186;
        }
    }

    for (contl=0; contl<24; contl++)
    {
        printf("%s", str[contl]);
    }

    fclose(instrucoes);

    while(!kbhit());
    getch();

    menuprincipal();
}

/*  Função PrintScore:
    -Apaga o score antigo e imprime um novo atualizado de acordo com as mudanças na variável global "score".
*/
void printscore()
{
    textbackground(9);
    textcolor(15);
    gotoxy(DISTANCIADABORDALATERAL+7+((MAXVIDAS*3)-1)+4+7, SUPERFICIE-2);
    printf("%8d", jogo.score);
}

/*  Função Oxigênio:
    -Fica decrementando 1 da variavel oxigenio enquanto o submarino estiver abaixo da superfície;
    -Imprime na tela uma barra (vetor de caracteres) que indica a situação do o2;
    -Quando o contador do vetor o2 chegar a 0, perde uma vida e reseta a o vetor oxigênio.
*/
void oxigenio()
{
    int i;

    if(jogo.posicaoo2==0)
    {
        Beep(500 , 50);
        Beep(350 , 50);
        Beep(200 , 50);
        jogo.posicaovidas=jogo.posicaovidas-3;
        jogo.x=37;
        jogo.y=SUPERFICIE;
        jogo.timer=0;
        for(i=SUPERFICIE+2; i<CHAO; i++)
        {
            textbackground(1);
            gotoxy(1, i);
            printf("                                                                                ");
        }
        for(i=1; i<=80; i++)//preenche a superficie da agua com "~"
        {
            textbackground(1);
            textcolor(9);
            gotoxy(i, SUPERFICIE+1);
            printf("~");
        }
        printsubmarino();
        for(i=0; i<3; i++)
        {
            jogo.xm[i]=0;
            jogo.mergulhador[i]=0;
        }
        for(i=0; i<17; i++)
        {
            jogo.xp[i]=0;
            jogo.peixe[i]=0;
        }
        while(jogo.posicaomergulhadores>0)
        {
            jogo.posicaomergulhadores--;
            apagarmergulhadoressalvos();
        }
        if(jogo.posicaovidas<0)
            morte();
        jogo.posicaoo2=MAXOXIGENIO;
    }
    if(jogo.y>SUPERFICIE)
        jogo.posicaoo2--;
    else
        jogo.posicaoo2=MAXOXIGENIO;

    apagarvidas();
    apagaroxigenio();
    printvidas();
    printoxigenio();
    printmunicao();
}

/*  Função PrintOxigenio/PrintVidas/PrintMergulhadoresSalvos/Munição/Tempo:
    -Imprime a barra de oxigênio/vidas/mergulhadores salvos/munição/tempo na posição indicada.
*/
void printoxigenio()
{
    char o2[MAXOXIGENIO];
    int i, j;

    for(i=0; i<MAXOXIGENIO; i++)
        o2[i]=(char)219;
    for(i=0; i<jogo.posicaoo2; i++)
    {
        textbackground(2);
        textcolor(15);
        gotoxy(DISTANCIADABORDALATERAL+10+i, CHAO+1);
        printf("%c", o2[i]);
    }

    for(j=i+1; j<MAXOXIGENIO; j++)
    {
        textbackground(4);
        gotoxy(DISTANCIADABORDALATERAL+10+j, CHAO+1);
        printf(" ", o2[i]);
    }
}
void printmergulhadoressalvos()
{
    int i;
    for(i=0; i<jogo.posicaomergulhadores; i=i+3)
    {
        textbackground(9);
        textcolor(15);
        gotoxy(DISTANCIADABORDALATERAL+7+((MAXVIDAS*3)-1)+32+i, SUPERFICIE-2);
        printf(">o ");
    }
}
void printvidas()
{
    int i;
    for(i=0; i<jogo.posicaovidas; i=i+3)
    {
        textbackground(9);
        textcolor(15);
        gotoxy(DISTANCIADABORDALATERAL+7+i, SUPERFICIE-2);
        printf("<3 ");
    }
}
void printmunicao()
{
    char municao[MAXMUNICAO];
    int i;

    textbackground(2);
    textcolor(15);

    for(i=0; i<MAXMUNICAO; i++)
        municao[i]='-';

    for(i=0; i<jogo.posicaomunicao; i++)
    {
        gotoxy(DISTANCIADABORDALATERAL+15+i, CHAO+2);
        printf("%c", municao[i]);
    }
}
void printtempo()
{
    textbackground(9);
    textcolor(15);

    jogo.tempodecorrido++;
    gotoxy(40, 1);
    printf("%5d", jogo.tempodecorrido);

    jogo.score=jogo.score+jogo.dificuldade+1;


    printscore();//função que apaga o score antigo e imprime o novo atualizado
}

/*  Função ApagarOxigenio/Vidas/Submarino/MergulhadoresSalvos:
    -Preenche com espaços o local onde o submarino/barra de oxigênio/vidas/MergulhadoresSalvos está/estão para que ele/eles/ela possa ser atualizada;
    -O preenchimento começa a partir da posição 0 do vetor/matriz o2/vidas/submarino.
*/
void apagaroxigenio()
{
    textbackground(4);

    if(jogo.posicaoo2<MAXOXIGENIO)
    {
        gotoxy(DISTANCIADABORDALATERAL+10+jogo.posicaoo2, CHAO+1);//limpa o local da barra de oxigênio
        printf(" ");
    }
}
void apagarvidas()
{
    textbackground(9);

    gotoxy(DISTANCIADABORDALATERAL+7+jogo.posicaovidas, SUPERFICIE-2);//limpa o local das vidas
    printf("   ");
}
void apagarsubmarino()
{


    if(jogo.sentido<0)
        gotoxy(jogo.x+3, jogo.y);
    else
        gotoxy(jogo.x+4, jogo.y);

    if(jogo.y==SUPERFICIE)
            textbackground(9);
        else
            textbackground(1);

    printf("  ");

    textbackground(1);
    gotoxy(jogo.x, jogo.y+1);
    printf("         ");

    gotoxy(jogo.x, jogo.y+2);
    printf("         ");
}
void apagarmergulhadoressalvos()
{
    textbackground(9);

    gotoxy(DISTANCIADABORDALATERAL+7+((MAXVIDAS*3)-1)+32+jogo.posicaomergulhadores, SUPERFICIE-2);
    printf("   ");
}
void apagarmunicao()
{
    textbackground(2);

    if(jogo.posicaomunicao<MAXMUNICAO)
    {
        gotoxy(DISTANCIADABORDALATERAL+15+jogo.posicaomunicao, CHAO+2);//limpa o local da barra de oxigênio
        printf(" ");
    }
}

/*  Função MoveTiro:
    -Move o tiro seguindo o sentido com o qual foi disparado;
    -O tiro desaparece quando chega nas laterais ou quando colide (ver função "verificacolisão()".
*/
void movetiro()
{
    verificacolisao();

    textcolor(15);
    textbackground(1);

    int i;
    for(i=0; i<MAXMUNICAO; i++)
        if(jogo.tiro[i]==1||jogo.tiro[i]==-1)
        {
            gotoxy(jogo.xt[i], jogo.yt[i]);
            printf(" ");
            jogo.xt[i]=jogo.xt[i]+jogo.tiro[i];
            gotoxy(jogo.xt[i], jogo.yt[i]);
            printf("-");
            if(jogo.xt[i]==1||jogo.xt[i]==80)
            {
                gotoxy(jogo.xt[i], jogo.yt[i]);
                printf(" ");
                jogo.xt[i]=0;
                jogo.tiro[i]=2;
            }
        }
}

/*  Função CriaTiro:
    -Cria balas da mesma forma que são criados peixes e mergulhadores;
    -Se jogo.tiro[i]=1, tiro existe e está no sentido positivo;
    -Se jogo.tiro[i]=-1, tiro existe e está no sentido negativo;
    -Se jogo.tiro[i]=0, tiro ainda não foi criado e está disponível para ser usado;
    -Se jogo.tiro[i]=2, tiro já foi criado e já desapareceu (colidindo ou saindo pela lateral) e NÃO está disponível para uso;
    -Tiros são recarregados quando o submarino morre ou quando são resgatados os 6 mergulhadores (ver função "mudadificuldade()").
*/
void criatiro()
{
    int i;


    for(i=0; i<MAXMUNICAO; i++)
        if((!jogo.tiro[i])&&(jogo.posicaomunicao>0))
        {
            if(jogo.sentido<0)
                jogo.xt[i]=jogo.x-1;
            else
                jogo.xt[i]=jogo.x+LARGURASUBMARINO;

            jogo.yt[i]=jogo.y+2;
            jogo.tiro[i]=jogo.sentido;
            jogo.posicaomunicao--;
            apagarmunicao();
            i=MAXMUNICAO;
        }
}

/*  Função MoveMergulhador:
    -Le as coordenadas do mergulhador;
    -Apaga o mergulhador;
    -Le o sentido do mergulhador;
    -Le as coordenadas do mergulhador somando ou subtraindo 1 no eixo x de acordo com o sentido do mergulhador;
    -Imprime o mergulhador na sua nova posição;
*/
void movemergulhador(int i)
{
    verificacolisao();

    textbackground(1);

    if(jogo.mergulhador[i]==1)
    {
        gotoxy(jogo.xm[i], jogo.ym[i]);
        printf(" ");
    }
    else
    if(jogo.mergulhador[i]==-1)
    {
        gotoxy(jogo.xm[i], jogo.ym[i]);
        printf("  ");
    }

    jogo.xm[i]=jogo.xm[i]+jogo.mergulhador[i];

    if(jogo.xm[i]==0||jogo.xm[i]==81)
    {
        jogo.mergulhador[i]=0;
        jogo.score=jogo.score-200;
    }
    else
        printmergulhador(i);
}

/*  Função CriaMergulhadores:
    -Cria mergulhadores a cada espaço de tempo (randomico);
    -Verifica a existência do mergulhador antes de criar (maximo de 5);
    -Inicializa o mergulhador com sentido e coordenadas iniciais.
*/
void criamergulhadores()
{
    int yrand, i;

    srand(time(NULL));
    yrand=((rand())%(CHAO-SUPERFICIE-4))+9;

    textbackground(1);
    textcolor(9);

    if(!(jogo.timer%(yrand*50)))
        for(i=0; i<3; i++)
        {
            if((jogo.mergulhador[i]==0)&&!(yrand%2))
            {
                jogo.mergulhador[i]=-1;
                jogo.xm[i]=78;
                jogo.ym[i]=yrand;
                gotoxy(jogo.xm[i], jogo.ym[i]);
                printf("o<");
                i=20;
            }else{
                if((jogo.mergulhador[i]==0)&&(yrand%2))
                {
                    jogo.mergulhador[i]=1;
                    jogo.xm[i]=1;
                    jogo.ym[i]=yrand;
                    gotoxy(jogo.xm[i], jogo.ym[i]);
                    printf(">o");
                    i=20;
                }
            }
        }

    for(i=0; i<3; i++)
        if(jogo.mergulhador[i]!=0)
            movemergulhador(i);
}

/*  Função MovePeixes:
    -Le as coordenadas do peixe;
    -Apaga o peixe;
    -Le o sentido do peixe;
    -Le as coordenadas do peixe somando ou subtraindo 1 no eixo x de acordo com o sentido do peixe;
    -Imprime o peixe na sua nova posição;
*/
void movepeixes(int i)
{
    verificacolisao();


    textbackground(1);

    if(jogo.peixe[i]==1)
    {
        gotoxy(jogo.xp[i], jogo.yp[i]);
        printf(" ");
    }
    else
    if(jogo.peixe[i]==-1)
    {
        gotoxy(jogo.xp[i], jogo.yp[i]);
        printf("   ");
    }

    jogo.xp[i]=jogo.xp[i]+jogo.peixe[i];

    if(jogo.xp[i]==0||jogo.xp[i]==81)
        jogo.peixe[i]=0;
    else
        printpeixe(i);


}

/*  Função CriaPeixes:
    -Cria peixes a cada espaço de tempo (randomico);
    -Verifica a existência do peixe antes de criar;
    -Inicializa o peixe com sentido e coordenadas iniciais.
*/
void criapeixes()
{
    int yrand, i;

    textbackground(1);

    if(jogo.dificuldade==0)
        textcolor(3);
    else
    if(jogo.dificuldade==1)
        textcolor(13);
    else
    if(jogo.dificuldade==2)
        textcolor(14);
    else
    if(jogo.dificuldade==3)
        textcolor(12);
    else
    if(jogo.dificuldade==4)
        textcolor(5);
    else
    if(jogo.dificuldade==5)
        textcolor(0);

    srand(time(NULL));
    yrand=((rand())%(CHAO-SUPERFICIE-2))+7;

    if(!(jogo.timer%(yrand*(12-(jogo.dificuldade*2)))))
        for(i=0; i<17; i++)
        {
            if((jogo.peixe[i]==0)&&!(yrand%2))
            {
                jogo.peixe[i]=-1;
                jogo.xp[i]=78;
                jogo.yp[i]=yrand;
                gotoxy(jogo.xp[i], jogo.yp[i]);
                printf("<><");
                i=20;
            }else
            if((jogo.peixe[i]==0)&&(yrand%2))
            {
                jogo.peixe[i]=1;
                jogo.xp[i]=1;
                jogo.yp[i]=yrand;
                gotoxy(jogo.xp[i], jogo.yp[i]);
                printf("><>");
                i=20;
            }
        }

    for(i=0; i<17; i++)
        if(jogo.peixe[i]!=0)
            movepeixes(i);
}

/*  Função AbrirSubMenu:
    -Dependendo da coordenada ymenu no momento do "enter", desvia para um submenu diferente.
*/
int abrirsubmenu(int ymenu)
{
    Beep(500, 90);
    clrscr();
    int decisao;
    if(ymenu==20)//caso o retangulo esteja selecionando a palavra "exit" no momento em que o enter foi pressionado
    {
        gotoxy(22, 13);
        printf("ARE YOU SURE YOU WANNA EXIT SEAQUEST?");
        decisao=simounao();
        clrscr();
        if(!decisao)
            menuprincipal();
        else
        {
            opcao=0;
            sairpartida=1;
            sairprograma=1;
        }
    }
    if(ymenu==17)//caso o retangulo esteja selecionando a palavra "leaderboards" no momento em que o enter foi pressionado
    {
        leaderboards(1);
    }
    if(ymenu==14)//caso o retangulo esteja selecionando a palavra "how to play" no momento em que o enter foi pressionado
    {
        howtoplay();
    }
    if(ymenu==11)//caso o retangulo esteja selecionando a palavra "play" no momento em que o enter foi pressionado
    {
        int i=1, ync=19, cnc, decisao;
        char e, f;
        e=(char)205;
        f=(char)186;
        gotoxy(15, 13);
        printf("DO YOU WANNA START A NEW GAME OR LOAD A PREVIOUS ONE?");
        //imprime o NEW
        gotoxy(35, ync-3);
        printf("  N E W  ");
        //imprime o LOAD
        gotoxy(34, ync-1);
        printf("%c%c%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, e, e, (char)187);
        gotoxy(34, ync);
        printf("%c L O A D %c", f, f);
        gotoxy(34, ync+1);
        printf("%c%c%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, e, e, (char)188);
        //escolher se quer carregar ou iniciar um novo jogo utilizando as setas:
        while(i)
        {
            if(kbhit)
            {
                cnc=getch();
                if(cnc==-32)
                    cnc=getch();
            }
            if(cnc==72&&ync!=16)
            {
                gotoxy(34, ync-1);
                printf("           ");
                gotoxy(34, ync);
                printf("  L O A D  ");
                gotoxy(34, ync+1);
                printf("           ");
                ync=ync-3;
                Beep(350, 90);
                gotoxy(35, ync-1);
                printf("%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, (char)187);
                gotoxy(35, ync);
                printf("%c N E W %c", f, f);
                gotoxy(35, ync+1);
                printf("%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, (char)188);
            }
            if(cnc==80&&ync!=19)
            {
                gotoxy(35, ync-1);
                printf("         ");
                gotoxy(35, ync);
                printf("  N E W  ");
                gotoxy(35, ync+1);
                printf("         ");
                ync=ync+3;
                Beep(350, 90);
                gotoxy(34, ync-1);
                printf("%c%c%c%c%c%c%c%c%c%c%c", (char)201, e, e, e, e, e, e, e, e, e, (char)187);
                gotoxy(34, ync);
                printf("%c L O A D %c", f, f);
                gotoxy(34, ync+1);
                printf("%c%c%c%c%c%c%c%c%c%c%c", (char)200, e, e, e, e, e, e, e, e, e, (char)188);
            }
            if(cnc==13||cnc==8)
                i=0;
        }
        if(cnc==8)
        {
            Beep(150, 90);
            menuprincipal();
        }
        else
        {
            if(ync==19)
            {
                Beep(150, 90);
                carregarjogo();
            }
            if(ync==16)
            {
                Beep(500, 90);
                novojogo();
            }
        }
    }
}

/*  Função ApagarSeleção:
    -Apagae o retângulo em volta da palavra para que outro possa ser impresso em outra palavra, indicando mudança na seleção.
*/
int apagarselecao(int *xmenu, int *ymenu, int *tampalavra)
{
    int i;

    gotoxy(*xmenu, *ymenu);
    printf(" ");
    gotoxy(*xmenu+*tampalavra-1, *ymenu);
    printf(" ");

    for(i=1; i<*tampalavra-1; i++)
    {
        gotoxy(*xmenu+i, *ymenu);
        printf(" ");
    }

    gotoxy(12, *ymenu+1);
    printf("%c", (char)186);
    for(i=13; i<*xmenu; i++)
    {
        gotoxy(i, *ymenu+1);
        printf(" ");
    }
    gotoxy(*xmenu, *ymenu+1);
    printf(" ");

    gotoxy(69, *ymenu+1);
    printf("%c", (char)186);
    for(i=*xmenu+*tampalavra; i<69; i++)
    {
        gotoxy(i, *ymenu+1);
        printf(" ");
    }
    gotoxy(*xmenu+*tampalavra-1, *ymenu+1);
    printf(" ");

    for(i=1; i<*tampalavra-1; i++)
    {
        gotoxy(*xmenu+i, *ymenu+2);
        printf(" ");
    }

    gotoxy(*xmenu, *ymenu+2);
    printf(" ");
    gotoxy(*xmenu+*tampalavra-1, *ymenu+2);
    printf(" ");
}

/*  Função PrintSeleção:
    -Imprime o retângulo em volta da palavra, indicando qual está selecionada no menu.
*/
int printselecao(int *xmenu, int *ymenu, int *tampalavra)
{
    int i;

    Beep(350, 90);

    gotoxy(*xmenu, *ymenu);
    printf("%c", (char)201);
    gotoxy(*xmenu+*tampalavra-1, *ymenu);
    printf("%c", (char)187);

    for(i=1; i<*tampalavra-1; i++)
    {
        gotoxy(*xmenu+i, *ymenu);
        printf("%c", (char)205);
    }

    gotoxy(12, *ymenu+1);
    printf("%c", (char)204);
    for(i=13; i<*xmenu; i++)
    {
        gotoxy(i, *ymenu+1);
        printf("%c", (char)205);
    }
    gotoxy(*xmenu, *ymenu+1);
    printf("%c", (char)185);

    gotoxy(69, *ymenu+1);
    printf("%c", (char)185);
    for(i=*xmenu+*tampalavra; i<69; i++)
    {
        gotoxy(i, *ymenu+1);
        printf("%c", (char)205);
    }
    gotoxy(*xmenu+*tampalavra-1, *ymenu+1);
    printf("%c", (char)204);

    for(i=1; i<*tampalavra-1; i++)
    {
        gotoxy(*xmenu+i, *ymenu+2);
        printf("%c", (char)205);
    }

    gotoxy(*xmenu, *ymenu+2);
    printf("%c", (char)200);
    gotoxy(*xmenu+*tampalavra-1, *ymenu+2);
    printf("%c", (char)188);
}

/*  Função NavegarMenuPrincipal:
    -Imprime na tela o menu principal junto com o letreiro "SEAQUEST".
*/
int navegarmenuprincipal()
{
    char cmenu=0, a, b, c, d, e, f, g;
    int maximo=17, minimo=11;
    int i=1, ymenu=11, xmenu=35;
    int xaux, tampalavra=11;
    a=(char)187;
    b=(char)185;
    c=(char)188;
    d=(char)201;
    e=(char)205;
    f=(char)186;
    g=(char)200;

    printselecao(&xmenu, &ymenu, &tampalavra);
    xaux=xmenu;

    while(i)
    {
        while(cmenu!=72&&cmenu!=80&&cmenu!=13&&cmenu!=8)
        {
            if(kbhit())//segue a mesma logica do kbhit da main
            {
                cmenu=getch();//mesma logica do loop interno da função main
                if(cmenu==-32)
                    cmenu=getch();//chama outro getch caso cmenu seja -32 (sintoma de que foi digitado uma das setas)
            }
        }

        if(cmenu==72&&ymenu>=minimo)//mesma logica da função move. Não foi feita em uma função separada por ser mais compacta
        {
            if(ymenu==minimo+3)
            {
                apagarselecao(&xaux, &ymenu, &tampalavra);
                ymenu=ymenu-3;
                xaux=35;
                tampalavra=11;
                printselecao(&xaux, &ymenu, &tampalavra);
            }
            else
            {
                if(ymenu==minimo+6)
                {
                    apagarselecao(&xaux, &ymenu, &tampalavra);
                    ymenu=ymenu-3;
                    xaux=35-7;//se ajusta ao tamanho do escrito leaderboards
                    tampalavra=25;
                    printselecao(&xaux, &ymenu, &tampalavra);
                }
                else
                {
                    if(ymenu==minimo+9)
                    {
                        apagarselecao(&xaux, &ymenu, &tampalavra);
                        ymenu=ymenu-3;
                        xaux=35-8;//se ajusta ao tamanho do escrito how to play
                        tampalavra=27;
                        printselecao(&xaux, &ymenu, &tampalavra);
                    }
                }
            }
        }
        if(cmenu==80&&ymenu<=maximo)
        {
            if(ymenu==maximo-6)
            {
                apagarselecao(&xaux, &ymenu, &tampalavra);
                ymenu=ymenu+3;
                xaux=35-7;
                tampalavra=25;
                printselecao(&xaux, &ymenu, &tampalavra);
            }
            else
            {
                if(ymenu==maximo-3)
                {
                    apagarselecao(&xaux, &ymenu, &tampalavra);
                    ymenu=ymenu+3;
                    xaux=35-8;
                    tampalavra=27;
                    printselecao(&xaux, &ymenu, &tampalavra);
                }
                else
                {
                    if(ymenu==maximo)
                    {
                        apagarselecao(&xaux, &ymenu, &tampalavra);
                        ymenu=ymenu+3;
                        xaux=35;
                        tampalavra=11;
                        printselecao(&xaux, &ymenu, &tampalavra);
                    }
                }
            }
        }
        if(cmenu!=13&&cmenu!=8)
            cmenu=0;
            else
                i=0;
    }
    if(cmenu==13)
        abrirsubmenu(ymenu);//chama a função que irá decidir qual submenu chamar
    else
    {
        ymenu=20;
        abrirsubmenu(ymenu);
    }
}

/*  Função MenuPrincipal:
    -Imprime na tela o menu principal junto com o letreiro "SEAQUEST".
*/
int menuprincipal()
{
    int opcao, k;
    char a, b, c, d, e, f, g, h, i;
    int xmenu=35, yp=12, yi, yl, yq;//posição do escrito play, leaderboards, instructions e quit, sendo que a coordenada X é a mesma para ambos
    yi=yp+3;
    yl=yi+3;
    yq=yl+3;
    a=(char)187;
    b=(char)219;
    c=(char)188;
    d=(char)201;
    e=(char)205;
    f=(char)186;
    g=(char)200;
    h=(char)220;
    i=(char)203;

    clrscr();

    gotoxy(7, 2);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c ", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c  ", b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c ", b, b, b, b, b, b, a);
    printf("%c%c%c   ", b, b, a);
    printf("%c%c%c", b, b, a);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, b, a);

    gotoxy(7, 3);
    printf("%c%c%c%c%c%c%c%c", b, b, d, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c", b, b, d, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c", b, b, d, e, e, b, b, a);
    printf("%c%c%c%c%c%c%c%c%c%c%c", b, b, d, e, e, e, b, b, a, b, b);
    printf("%c   %c%c%c", f, b, b, f);
    printf("%c%c%c%c%c%c%c%c", b, b, d, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c", b, b, d, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c%c", g, e, e, b, b, d, e, e, c);

    gotoxy(7, 4);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c  ", b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c%c%c%c   ", b, b, b, b, b, b, b, f, b, b, f);
    printf("%c%c%c%c%c%c   ", b, b, f, b, b, f);
    printf("%c%c%c%c%c%c%c%c%c%c ", b, b, f, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c   %c%c%c", b, b, b, b, b, b, b, a, b, b, f);

    gotoxy(7, 5);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c  ", g, e, e, e, e, b, b, f, b, b, d, e, e, c);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c ", b, b, d, e, e, b, b, f, b, b, f, h, h);
    printf("%c%c%c%c%c%c   ", b, b, f, b, b, f);
    printf("%c%c%c%c%c%c%c%c%c%c  ", b, b, f, b, b, d, e, e, e, c);
    printf("%c%c%c%c%c%c%c   %c%c%c", g, e, e, e, b, b, f, b, b, f);

    gotoxy(7, 6);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, f);
    printf("%c%c%c%c%c%c%c%c%c%c%c  ", b, b, b, b, b, b, b, a, b, b, f);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c", b, b, f, g, b, b, b, b, b, b, d, c);
    printf("%c%c%c%c%c%c%c%c%c", g, b, b, b, b, b, b, d, c);
    printf("%c%c%c%c%c%c%c%c", b, b, b, b, b, b, b, a);
    printf("%c%c%c%c%c%c%c%c   %c%c%c", b, b, b, b, b, b, b, f, b, b, f);

    gotoxy(7, 7);
    printf("%c%c%c%c%c%c%c%c", g, e, e, e, e, i, e, c);
    printf("%c%c%c%c%c%c%c%c", g, e, e, e, e, e, e, c);
    printf("%c%c%c  ", g, e, c);
    printf("%c%c%c  ", g, e, c);
    printf("%c%c%c%c%c%c  ", g, e, b, b, e, c);
    printf("%c%c%c%c%c%c%c ", g, e, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c", g, e, e, e, e, e, e, c);
    printf("%c%c%c%c%c%c%c%c   ", g, e, e, e, e, e, e, c);
    printf("%c%c%c  ", g, i, c);

    for(k=8; k<24; k++)
    {
        gotoxy(12, k);
        printf("%c", f);
    }
    for(k=8; k<24; k++)
    {
        gotoxy(69, k);
        printf("%c", f);
    }
    gotoxy(12, 24);
    printf("%c", g);
    gotoxy(69, 24);
    printf("%c", c);
    for(k=13; k<69; k++)
    {
        gotoxy(k, 24);
        printf("%c", e);
    }

    gotoxy(xmenu, yp);
    printf("  P L A Y  ");
    gotoxy(xmenu-8, yl);
    printf("  L E A D E R B O A R D S  ");
    gotoxy(xmenu-7, yi);
    printf("  H O W   T O   P L A Y  ");
    gotoxy(xmenu, yq);
    printf("  Q U I T  ");

    navegarmenuprincipal();
}

/*  Função AumentaDificuldade:
    -Aumenta o coeficiente da dificuldade em 1 se a dificuldade não for a máxima (5);
    -
*/
void mudadificuldade()
{
    int i;

    for(i=0; i<MAXMUNICAO; i++)
            if(jogo.tiro[i]==2)
                jogo.tiro[i]=0;

    jogo.posicaomunicao=MAXMUNICAO;

    if(i>0)
        jogo.score=jogo.score+12;
    else
    if(jogo.score-2*jogo.posicaomergulhadores>=0)
        jogo.score=jogo.score-2*jogo.posicaomergulhadores;

    if(jogo.dificuldade<DIFICULDADEMAXIMA)
            jogo.dificuldade=jogo.dificuldade+1;

    jogo.timer=0;

    for(i=SUPERFICIE+2; i<CHAO; i++)
    {
        textbackground(1);
        gotoxy(1, i);
        printf("                                                                                ");
    }

    printsubmarino();

    for(i=0; i<3; i++)
    {
        jogo.xm[i]=0;
        jogo.mergulhador[i]=0;
    }
    for(i=0; i<17; i++)
    {
        jogo.xp[i]=0;
        jogo.peixe[i]=0;
    }
    printmunicao();
    apagarmergulhadoressalvos();
    printmergulhadoressalvos();
    salvar();
}

/*  Função Move:
    -Chama a função que apaga o submarino para limpa-lo da posição anterior ao comando de movimento;
    -Imprime o submarino na sua nova posição;
    -Altera o valor de X ou Y baseado na tecla atribuida a 'c' na main;
    -Se o submarino estiver encostado na borda, a devida condicional não será atendida e as coordenadas continuarão as mesmas.
*/
void movesubmarino(char *c)
{
    verificacolisao();

    textcolor(9);
    textbackground(1);

    if(*c==72&&jogo.y>SUPERFICIE)//pra cima = 72
    {
        apagarsubmarino();
        jogo.y--;
        printsubmarino();
        if(jogo.y==SUPERFICIE)
            if(jogo.posicaomergulhadores==18)
            {
                while(jogo.posicaomergulhadores>0)
                {
                    Beep(200, 100);
                    Beep(350, 100);
                    Beep(500, 100);
                    jogo.posicaomergulhadores=jogo.posicaomergulhadores-3;
                    jogo.score=jogo.score+200;
                    apagarmergulhadoressalvos();
                    printmergulhadoressalvos();
                    printscore();
                }
                mudadificuldade();
            }
    }
    else
    if(*c==75&&jogo.x>1)//pra esquerda = 75
    {
        apagarsubmarino();
        if(jogo.y<SUPERFICIE+2)//completa a array da superficie quando o submarino se desloca para esquerda
        {
            gotoxy(jogo.x+LARGURASUBMARINO-5, SUPERFICIE+1);
            printf("~~~~~");
        }
        jogo.x--;
        jogo.sentido=-1;
        printsubmarino();//imprime o submarino
    }
    else
    if(*c==80&&jogo.y<CHAO-ALTURASUBMARINO)//pra baixo = 80
     {
        apagarsubmarino();
        gotoxy(jogo.x, SUPERFICIE+1);//completa a array da superficie quando o submarino se desloca para baixo
        printf("~~~~~~~~~");
        jogo.y++;
        printsubmarino();//imprime o submarino
     }
    else
    if(*c==77&&jogo.x<=80-LARGURASUBMARINO)//pra direita = 77
    {
        apagarsubmarino();
        if(jogo.y<SUPERFICIE+2)//completa a array da superficie quando o submarino se desloca para direita
        {
            gotoxy(jogo.x, SUPERFICIE+1);
            printf("~~~~~");
        }
        jogo.x++;
        jogo.sentido=1;
        printsubmarino();//imprime o submarino e depois chama as funções de impressao da barra de o2 e da barra de vidas
    }
    *c=0;
}

/*  Função Morte:
    -Limpa a tela;
    -Imprime uma mensagem na tela;
    -Chama um menu de opções.
*/
void morte()
{
    apagarvidas();
    jogo.morto=1;
    salvar();
    Beep(700, 500);
    Beep(300, 500);
    Beep(700, 500);
    Beep(300, 500);
    Beep(700, 500);
    Beep(300, 500);
    textbackground(0);
    clrscr();//limpa a tela para imprimir a mensagem de game over
    textcolor(12);
    gotoxy(20, 6);
    printf("WOW");
    textcolor(9);
    gotoxy(20, 8);
    printf("                                    MUCH DEAD");
    textcolor(10);
    gotoxy(20, 10);
    printf("     VERY FAILURE");
    textcolor(13);
    gotoxy(20, 12);
    printf("                    SUCH DISAPOINTMENT");
    Sleep(500);
    Beep(450, 30);
    Beep(550, 850);
    Beep(350, 30);
    Beep(450, 850);
    Beep(200, 30);
    Beep(300, 900);
    Beep(100, 30);
    Beep(150, 2500);
    gotoxy(20, 14);
    textcolor(15);
    printf("TRY AGAIN?\n");
    opcao=simounao();

    if(opcao)
        abrirsubmenu(11);
    else
        menuprincipal();
}

/*  Função Main:
    -Chama o menu principal, que definirá a entrada nos loops internos;
    -Cria um loop que aguarda mudança na varáavel 'c';
    -Se 'c' receber um valor (deixar de ser 0), chama a função de movimento;
    -Reseta a variável 'c' para manter o loop, a não ser que a tecla digitada seja 'q', de quit;
    -Cria um timer interno que regula a chamada das funções do jogo.
*/
int main()
{
    int pause=0;//variável que mantem o loop externo da partida//variável que mantem o loop interno da partida//variavel que pausa temporariamente o loop interno da partida
    char c=0;//comando para os movimentos do submarino
    while(!sairprograma)
    {
        menuprincipal();
        while(opcao)//opção PLAY
        {
            sairpartida=0;
            hud();//função chamada para imprimir o HUD permanente na tela
            jogo.timer=0;
            printsubmarino();//chama pela primeira vez a função que imprime o submarino
            while(!sairpartida)//loop interno que controla o decorrer da partida
            {
                if(kbhit())
                {
                    c=getch();//le um comando qualquer
                    if(c==-32)
                        c=getch();//le o comando seta
                }

                //funções que dependem da tecla digitada in-game para serem chamadas:
                if(c==32)
                    criatiro();
                else
                if(c=='p')
                    pausar();
                else
                if(c=='q')
                    quit(&c);
                else
                if(c=='s')
                    salvar();
                else
                if(c=='x')
                    leaderboards(0);
                else
                if(c=='n')
                {
                    salvar();
                    abrirsubmenu(11);
                }

                //funções que dependem do timer para serem chamadas:
                if(!(jogo.timer%98))
                    printtempo();
                if(!(jogo.timer%2))
                    movesubmarino(&c);//chama a função de movimento, que altera os valores de x e y baseado na tecla digitada
                if(!(jogo.timer%(70-(jogo.dificuldade*5))))//a cada 10 ticks do timer, chama a função oxigênio
                    oxigenio();
                if(!(jogo.timer%(12-(jogo.dificuldade*2))))
                    criapeixes();
                if(!(jogo.timer%50))
                    criamergulhadores();


                movetiro();

                jogo.timer++;//acrescenta 1 ao timer a cada loop

                Sleep(10);//leve delay pro submarino não andar rapido demais
            }
        }
    }
    clrscr();
}
