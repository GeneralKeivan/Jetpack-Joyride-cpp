//A Game Created By
//Keivan Banan

#include "SBDL.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define change(a, b) \
    {                \
        a += b;      \
        b = a - b;   \
        a -= b;      \
    }

enum Skins
{
    Normal,
    Alien
};

struct EnemyActivity
{
    bool ZapActive = false;
    bool LasActive = false;
    bool MisActive = false;
    bool CoinActive = false;
};

struct Zap
{
    double x, y;
    double speed = 0;
    bool active = false;
};

struct Mis
{
    double x = 2048 - 100;
    double y = 0;
    double speed = 20;
    bool active = false;
    bool moving = false;
};

struct SpeedBoost
{
    double x, y;
    double speed = 20;
    bool active = false;
};
struct Coin
{
    double x;
    double y;
    bool active = false;
    SDL_Rect coins;
};

struct Smoke
{
    double x, y;
};

struct GT
{
    double x, y;
    bool active = false;
};

struct Movement
{
    int size;
    Texture *images;
    std::string FolderAddress;
    int CurrentIndex = 0;
};

struct Player
{
    double x, y;
    double a, v;
    int life;
};

Player Barry = {300, 365, 1.5, 1, 1};
Skins Costume = Normal;
EnemyActivity Activator;
Movement BG;
Movement PL;
Movement GPL;
Movement ST;
Movement ZapperD, ZapperH, ZapperV;

Movement Miss0, Miss1, Miss2, Miss3, Miss4;

Movement Smoke0, Smoke1, Smoke2, Smoke3, Smoke4;

Movement CN;

Movement LSR;

Coin C[10][50];
Mis M[5];
Smoke S[5];
Zap Zapper[3];
SpeedBoost Token;
GT G;

double SP = 10;
double BackSpeed;

std::string MVScore;
std::string Score;
bool GameOver = false;
bool lose = false;
bool Paused = false;
bool GravMode = false;

int SoundON = 1;
int MusicON = -1;

bool TimeToDegen = false;
bool TimeToFire = false;
bool Winner = false;

int Changer = 0;
int timer0 = 0, timer1 = 0, timer2 = 0, timer3 = 0, timer4 = 0;
int counter = 0;
int CoinsScore = 0;
int MoveScore = 0;
int NOA = 4;
int GravDir = 1;
int GC = 0;

std::string HS[10];

void swap(char *&s, char *&t)
{
    char *temp = s;
    s = t;
    t = temp;
}

void Sort(char *Highscores[], int size)
{
    int *HS = new int[size];
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        HS[i] = 0;
        while (Highscores[i][j] != '\0')
        {
            HS[i] = HS[i] * 10 + (Highscores[i][j] - '0');
            j++;
        }
        j = 0;
    }
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size - i - 1; k++)
        {
            if (HS[k + 1] > HS[k])
            {
                swap(Highscores[k], Highscores[k + 1]);
                change(HS[k], HS[k + 1]);
            }
        }
    }
}

void ShowLosingNote(std::string &FileName)
{
    Sound *Losing = SBDL::loadSound("assets/sound/womp-womp.wav");
    Sound *Omae = SBDL::loadSound("assets/sound/Omae wa moe shindeiru.wav");
    Sound *Nani = SBDL::loadSound("assets/sound/Nani.wav");
    int k = rand();
    switch (k % 13)
    {
    case 0:
        SBDL::playSound(Losing, 1);
        FileName = "Wow that was something.";
        break;
    case 1:
        SBDL::playSound(Omae, 1);
        FileName = "Omae wa mou shindeiru.";
        break;
    case 2:
        SBDL::playSound(Nani, 1);
        FileName = "Nani?1?!";
        break;
    case 3:
        SBDL::playSound(Losing, 1);
        FileName = "Even my dog can do better and I don't have a dog.";
        break;
    case 4:
        SBDL::playSound(Losing, 1);
        FileName = "You Suck!";
        break;
    case 5:
        SBDL::playSound(Losing, 1);
        FileName = "To put it simply , YOU DIED!";
        break;
    case 6:
        SBDL::playSound(Losing, 1);
        FileName = "I got cancer from that.";
        break;
    case 7:
        SBDL::playSound(Losing, 1);
        FileName = "They're forcing me to write these.";
        break;
    case 8:
        SBDL::playSound(Losing, 1);
        FileName = "Help me!";
        break;
    case 9:
        SBDL::playSound(Losing, 1);
        FileName = "Oh no it's Ninja Brian.";
        break;
    case 10:
        SBDL::playSound(Losing, 1);
        FileName = "He's Stabbing me!";
        break;
    case 11:
        SBDL::playSound(Losing, 1);
        FileName = "pneonrijeeeeeeeeeeeeeeeeeeeeeeee";
        break;
    case 12:
        SBDL::playSound(Losing, 1);
        FileName = "In a game bugs are just features.";
        break;
    }
    k++;
}

void ShowWinner()
{
    SBDL::clearRenderScreen();
    SBDL::stopMusic();
    Font *font = SBDL::loadFont("assets/font/Jetpackia.ttf", 50);
    Texture Note = SBDL::createFontTexture(font, "Who is a winner?", 0, 0, 0);
    Texture Note2 = SBDL::createFontTexture(font, "You! You is a winner!", 0, 0, 0);
    Texture Exit = SBDL::createFontTexture(font, "Press Space to resume", 255, 255, 255);
    Sound *Winning = SBDL::loadSound("assets/sound/Winning-sound-effect.wav");
    SBDL::playSound(Winning, 1);
    while (SBDL::isRunning())
    {
        SBDL::showTexture(BG.images[BG.CurrentIndex], 0, 0);
        SBDL::showTexture(Note, (2048 - Note.width) / 2, (460 - Note.height) / 2);
        SBDL::showTexture(Note2, (2048 - Note2.width) / 2, (460 + Note.height) / 2);
        SBDL::showTexture(Exit, 1948 - Exit.width, 400);
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
        if (SBDL::keyPressed(SDL_SCANCODE_SPACE))
        {
            return;
        }
    }
}

void ShowLoser()
{
    std::string Output;
    SBDL::stopMusic();
    Font *font = SBDL::loadFont("assets/font/Jetpackia.ttf", 50);

    ShowLosingNote(Output);
    Texture Note = SBDL::createFontTexture(font, Output, 255, 0, 0);
    Texture Exit = SBDL::createFontTexture(font, "Press Space to resume", 255, 255, 255);
    while (SBDL::isRunning())
    {
        SBDL::showTexture(BG.images[BG.CurrentIndex], 0, 0);
        SBDL::showTexture(Note, (2048 - Note.width) / 2, (460 - Note.height) / 2);
        SBDL::showTexture(Exit, 1948 - Exit.width, 400);
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
        if (SBDL::keyPressed(SDL_SCANCODE_SPACE))
        {
            return;
        }
    }
}

void CheckScore()
{
    SBDL::stopAllSounds();
    char *Check;
    char *HighScore[11];
    Check = new char[10];
    HighScore[0] = new char[10];
    sprintf(HighScore[0], "%d", MoveScore);
    sprintf(Check, "%d", MoveScore);
    std::ifstream fin("HighScore.txt");
    for (int i = 0; i < 10; i++)
    {
        HighScore[i + 1] = new char[10];
        fin >> HighScore[i + 1];
    }
    Sort(HighScore, 11);
    for (int i = 0; i < 10; i++)
    {
        if (strcmp(Check, HighScore[i]) == 0)
        {
            Winner = true;
        }
    }
    if (Winner)
    {
        ShowWinner();
    }
    else
    {
        ShowLoser();
    }
}

void CheckHighScore()
{
    char *HighScore[11];
    HighScore[0] = new char[10];
    sprintf(HighScore[0], "%d", MoveScore);
    std::ifstream fin("HighScore.txt");
    for (int i = 0; i < 10; i++)
    {
        HighScore[i + 1] = new char[10];
        fin >> HighScore[i + 1];
    }
    Sort(HighScore, 11);
    std::ofstream fout("HighScore.txt");
    for (int i = 0; i < 10; i++)
    {
        fout << HighScore[i] << std::endl;
    }
}

void ShowHighScore()
{
    SBDL::clearRenderScreen();
    Texture Back = SBDL::loadTexture("assets/pic/menu/back.png");
    Font *font = SBDL::loadFont("assets/font/Jetpackia.ttf", 40);
    SDL_Rect BackButton = {(2048 - Back.width) / 2, 400, Back.width, Back.height};
    int height;
    Texture HST[10];

    std::ifstream fin("HighScore.txt");
    for (int i = 0; i < 10; i++)
    {
        fin >> HS[i];
        HST[i] = SBDL::createFontTexture(font, HS[i], 0, 0, 0);
    }
    while (SBDL::isRunning())
    {
        for (int i = 0; i < 10; i++)
        {
            height = 40 * i;
            SBDL::showTexture(HST[i], 1024, height);
        }
        SBDL::showTexture(Back, (2048 - Back.width) / 2, 400);
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
        if (SBDL::mouseInRect(BackButton) && SBDL::Mouse.clicked())
        {
            break;
        }
    }
}

void ShowSoundMenu()
{
    Texture Menu = SBDL::loadTexture("assets/pic/menu/Background.png");
    Texture Back = SBDL::loadTexture("assets/pic/menu/back.png");
    Texture SoundY = SBDL::loadTexture("assets/pic/menu/sound on.png");
    Texture SoundN = SBDL::loadTexture("assets/pic/menu/sound off.png");
    SDL_Rect SND = {(2048 - Menu.width) / 2, Menu.height - SoundY.height, SoundY.width, SoundN.height};
    SDL_Rect BackButton = {1024 + Menu.width / 2 - Back.width, Menu.height - Back.height, Back.width, Back.height};
    while (SBDL::isRunning())
    {
        SBDL::showTexture(Menu, (2048 - Menu.width) / 2, 0);
        SBDL::showTexture(Back, 1024 + Menu.width / 2 - Back.width, Menu.height - Back.height);
        if (SoundON == 1)
        {
            SBDL::showTexture(SoundY, (2048 - Menu.width) / 2, Menu.height - SoundY.height);
        }
        else
        {
            SBDL::showTexture(SoundN, (2048 - Menu.width) / 2, Menu.height - SoundN.height);
        }
        if (SBDL::mouseInRect(SND) && SBDL::Mouse.clicked())
        {
            if (SoundON == 1)
            {
                SoundON--;
            }
            else
            {
                SoundON++;
            }
        }
        if (SBDL::mouseInRect(BackButton) && SBDL::Mouse.clicked())
        {
            return;
        }
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
    }
}

void Rebirth()
{
    SP = 10;

    GameOver = false;
    lose = false;
    Paused = false;
    GravMode = false;
    G.active = false;

    Changer = 0;
    GC = 0;
    GravDir = 1;

    Activator.MisActive = false;
    Activator.ZapActive = false;
    Activator.LasActive = false;

    PL.CurrentIndex = 0;
    GPL.CurrentIndex = 0;

    Miss0.CurrentIndex = 0;
    Miss1.CurrentIndex = 0;
    Miss2.CurrentIndex = 0;
    Miss3.CurrentIndex = 0;
    Miss4.CurrentIndex = 0;

    timer0 = 0;
    timer1 = 0;
    timer2 = 0;
    timer3 = 0;
    timer4 = 0;
    counter = 0;

    Barry = {300, 365, 1.5, 1, 1};

    Zapper[0].x = 2448;
    Zapper[1].x = Zapper[0].x + rand() % 400;
    Zapper[2].x = Zapper[1].x + rand() % 400;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            C[i][j].x = 0;
            C[i][j].y = 0;
            C[i][j].coins.x = 0;
            C[i][j].coins.y = 0;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        M[i].moving = false;
        M[i].active = false;
        M[i].x = 2448 - 100;
        M[i].y = 0;
    }

    for (int i = 0; i < 3; i++)
    {
        Zapper[i].active = false;
    }
}

void StartOver()
{
    SBDL::stopMusic();
    Music *music = SBDL::loadMusic("assets/sound/music.mp3");
    SBDL::playMusic(music , MusicON);
    
    Winner = false;

    Activator.MisActive = false;
    Activator.ZapActive = false;
    Activator.LasActive = false;
    Activator.CoinActive = false;
    Token.active = false;

    Token.x = 2048;
    Token.y = 0;

    PL.CurrentIndex = 0;
    GPL.CurrentIndex = 0;

    Miss0.CurrentIndex = 0;
    Miss1.CurrentIndex = 0;
    Miss2.CurrentIndex = 0;
    Miss3.CurrentIndex = 0;
    Miss4.CurrentIndex = 0;

    GravDir = 1;

    timer0 = 0;
    timer1 = 0;
    timer2 = 0;
    timer3 = 0;
    timer4 = 0;

    counter = 0;
    CoinsScore = 0;
    MoveScore = 0;
    NOA = 4;

    GameOver = false;
    lose = false;
    Paused = false;

    SP = 10;
    BackSpeed = 0;

    Barry = {300, 365, 1.5, 1, 1};

    GravMode = false;
    G.active = false;

    Zapper[0].x = 2448;
    Zapper[1].x = Zapper[0].x + rand() % 400;
    Zapper[2].x = Zapper[1].x + rand() % 400;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            C[i][j].active = false;
            C[i][j].x = 0;
            C[i][j].y = 0;
            C[i][j].coins.x = 0;
            C[i][j].coins.y = 0;           
        }
    }

    for (int i = 0; i < 5; i++)
    {
        M[i].moving = false;
        M[i].active = false;
        M[i].x = 2448 - 100;
    }

    for (int i = 0; i < 3; i++)
    {
        Zapper[i].active = false;
    }
}

void ShowChange()
{
    Texture Background = SBDL::loadTexture("assets/pic/back/1.png");
    Texture NBarry = SBDL::loadTexture("assets/pic/barry/Normal Barry/1.png");
    Texture ABarry = SBDL::loadTexture("assets/pic/barry/Alien/1.png");
    SDL_Rect NChange = {1024 - NBarry.width, 400 - NBarry.height, NBarry.width, NBarry.height};
    SDL_Rect AChange = {1024, 400 - ABarry.height, ABarry.width, ABarry.height};
    while (SBDL::isRunning())
    {
        SBDL::showTexture(Background, 0, 0);
        SBDL::showTexture(NBarry, 1024 - NBarry.width, 400 - NBarry.height);
        SBDL::showTexture(ABarry, 1024, 400 - ABarry.height, ABarry.width);
        SBDL::updateEvents();
        SBDL::updateRenderScreen();
        if (SBDL::mouseInRect(NChange) && SBDL::Mouse.clicked())
        {
            Costume = Normal;
            SBDL::clearRenderScreen();
            return;
        }
        if (SBDL::mouseInRect(AChange) && SBDL::Mouse.clicked())
        {
            Costume = Alien;
            SBDL::clearRenderScreen();
            return;
        }
    }
}

void StartMenu()
{
    Font *font = SBDL::loadFont("assets/font/Jetpackia.ttf", 50);
    Texture Name = SBDL::createFontTexture(font, "K-1", 255 , 0 , 0);
    Texture HighScores = SBDL::loadTexture("assets/pic/menu/highscores.png");
    Texture Background = SBDL::loadTexture("assets/pic/back/1.png");
    Texture Start = SBDL::loadTexture("assets/pic/menu/play.png");
    Texture ChangeSkin = SBDL::loadTexture("assets/pic/menu/change keys.png");
    Texture Quit = SBDL::loadTexture("assets/pic/menu/quit.png");
    Texture Menu = SBDL::loadTexture("assets/pic/menu/pauseb.png");
    SDL_Rect StartButton = {(2048 - Menu.width) / 2, Menu.height - Start.height, Start.width, Start.height};
    SDL_Rect ChangeButton = {(2048 - ChangeSkin.width) / 2, Menu.height - ChangeSkin.height, ChangeSkin.width, ChangeSkin.height};
    SDL_Rect QuitButton = {1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height, Quit.width, Quit.height};
    SDL_Rect HighButton = {(2048 - Menu.width) / 2, 0, HighScores.width, HighScores.height};
    while (SBDL::isRunning())
    {
        SBDL::showTexture(Background, 0, 0);
        SBDL::showTexture(Menu, (2048 - Menu.width) / 2, 0);
        SBDL::showTexture(Start, (2048 - Menu.width) / 2, Menu.height - Start.height);
        SBDL::showTexture(ChangeSkin, (2048 - ChangeSkin.width) / 2, Menu.height - ChangeSkin.height);
        SBDL::showTexture(Quit, 1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height);
        SBDL::showTexture(HighScores, (2048 - Menu.width) / 2, 0);
        SBDL::showTexture(Name, 200, 0);
        if (SBDL::mouseInRect(QuitButton) && SBDL::Mouse.clicked())
        {
            GameOver = true;
            break;
        }
        if (SBDL::mouseInRect(StartButton) && SBDL::Mouse.clicked())
        {
            break;
        }
        if (SBDL::mouseInRect(ChangeButton) && SBDL::Mouse.clicked())
        {
            ShowChange();
        }
        if (SBDL::mouseInRect(HighButton) && SBDL::Mouse.clicked())
        {
            ShowHighScore();
        }
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
    }
}

void ShowPauseMenu()
{
    Texture Menu = SBDL::loadTexture("assets/pic/menu/Background.png");
    Texture Resume = SBDL::loadTexture("assets/pic/menu/resume.png");
    Texture SOUND = SBDL::loadTexture("assets/pic/menu/sound.png");
    Texture Quit = SBDL::loadTexture("assets/pic/menu/quit.png");
    SDL_Rect ResumeButton = {(2048 - Menu.width) / 2, Menu.height - Resume.height, Resume.width, Resume.height};
    SDL_Rect SoundButton = {(2048 - SOUND.width) / 2, Menu.height - SOUND.height, SOUND.width, SOUND.height};
    SDL_Rect QuitButton = {1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height, Quit.width, Quit.height};
    while (SBDL::isRunning)
    {
        SBDL::showTexture(Menu, (2048 - Menu.width) / 2, 0);
        SBDL::showTexture(Resume, (2048 - Menu.width) / 2, Menu.height - Resume.height);
        SBDL::showTexture(SOUND, (2048 - SOUND.width) / 2, Menu.height - SOUND.height);
        SBDL::showTexture(Quit, 1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height);
        SBDL::updateRenderScreen();
        SBDL::updateEvents();
        if (SBDL::mouseInRect(ResumeButton) && SBDL::Mouse.clicked())
        {
            Paused = false;
            return;
        }
        if ((SBDL::mouseInRect(QuitButton) && SBDL::Mouse.clicked()) || SBDL::keyPressed(SDL_SCANCODE_Q))
        {
            GameOver = true;
            Paused = false;
            return;
        }
        if (SBDL::mouseInRect(SoundButton) && SBDL::Mouse.clicked())
        {
            ShowSoundMenu();
        }
    }
}

void ShowLoseMenu()
{
    CheckScore();
    SBDL::clearRenderScreen();
    SBDL::updateEvents();
    SBDL::updateRenderScreen();
    Texture Menu = SBDL::loadTexture("assets/pic/menu/Background.png");
    Texture Retry = SBDL::loadTexture("assets/pic/menu/retry.png");
    Texture Quit = SBDL::loadTexture("assets/pic/menu/quit.png");
    Texture Chance = SBDL::loadTexture("assets/pic/menu/resume 500.png");
    Texture HighScores = SBDL::loadTexture("assets/pic/menu/highscores.png");
    SDL_Rect QuitButton = {1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height, Quit.width, Quit.height};
    SDL_Rect RetryButton = {(2048 - Menu.width) / 2, Menu.height - Retry.height, Retry.width, Retry.height};
    SDL_Rect HighButton = {(2048 - HighScores.width) / 2, Menu.height - HighScores.height, HighScores.width, HighScores.height};
    SDL_Rect ChanceButton = {(2048 - Chance.width) / 2, Menu.height - Chance.height, Chance.width, Chance.height};
    while (SBDL::isRunning())
    {
        if (CoinsScore >= 100)
        {
            SBDL::showTexture(Menu, (2048 - Menu.width) / 2, 0);
            SBDL::showTexture(Chance, (2048 - Chance.width) / 2, Menu.height - Chance.height);
            SBDL::updateEvents();
            if (SBDL::mouseInRect(ChanceButton) && SBDL::Mouse.clicked())
            {
                Barry.life++;
                lose = false;
                CoinsScore -= 100;
                NOA++;
                Rebirth();
                return;
            }
        }
        else
        {

            SBDL::showTexture(Menu, (2048 - Menu.width) / 2, 0);
            SBDL::showTexture(Retry, (2048 - Menu.width) / 2, Menu.height - Retry.height);
            SBDL::showTexture(HighScores, (2048 - HighScores.width) / 2, Menu.height - HighScores.height);
            SBDL::showTexture(Quit, 1024 + Menu.width / 2 - Quit.width, Menu.height - Quit.height);
            SBDL::updateEvents();
            if (SBDL::mouseInRect(QuitButton) && SBDL::Mouse.clicked())
            {
                GameOver = true;
                return;
            }
            if (SBDL::mouseInRect(RetryButton) && SBDL::Mouse.clicked())
            {
                StartOver();
                return;
            }
            if (SBDL::mouseInRect(HighButton) && SBDL::Mouse.clicked())
            {
                ShowHighScore();
            }
        }
        SBDL::updateRenderScreen();
    }
}

void Math()
{
    if (counter % 5 == 0)
    {
        ST.CurrentIndex++;
        CN.CurrentIndex++;
        if (CN.CurrentIndex >= 6)
        {
            CN.CurrentIndex = 0;
        }
        if (ST.CurrentIndex >= 4)
        {
            ST.CurrentIndex = 0;
        }
    }

    BackSpeed -= SP;
    if (BackSpeed <= -2048)
    {
        BG.CurrentIndex++;
        BackSpeed = 0;
    }
    if (BG.CurrentIndex >= BG.size)
    {
        BG.CurrentIndex = 0;
    }

    if (Barry.y == 365 && !GravMode)
    {
        if (PL.CurrentIndex >= 2)
        {
            PL.CurrentIndex = 0;
        }
        if (PL.CurrentIndex == 0)
        {
            Changer = 1;
        }
        if (PL.CurrentIndex == 1)
        {
            Changer = -1;
        }
        PL.CurrentIndex += Changer;
    }

    if (GravMode && Barry.y == 365)
    {
        if (GPL.CurrentIndex == 0)
        {
            Changer = 1;
        }
        if (GPL.CurrentIndex == 1)
        {
            Changer = -1;
        }
        GPL.CurrentIndex += Changer;
    }
    if (GravMode && Barry.y == 0)
    {
        if (GPL.CurrentIndex == 2)
        {
            Changer = 1;
        }
        if (GPL.CurrentIndex == 3)
        {
            Changer = -1;
        }
        GPL.CurrentIndex += Changer;
    }
}

bool CheckInter(SDL_Rect Player, SDL_Rect coins)
{
    if (SBDL::hasIntersectionRect(Player, coins))
    {
        return true;
    }
    return false;
}

void PatternGenerator(std::string &FileName)
{
    int k = rand();
    switch (k % 27)
    {
    case 0:
        FileName = "assets/coinPatterns/coinPattern1.txt";
        break;
    case 1:
        FileName = "assets/coinPatterns/coinPattern2.txt";
        break;
    case 2:
        FileName = "assets/coinPatterns/coinPattern3.txt";
        break;
    case 3:
        FileName = "assets/coinPatterns/coinPattern4.txt";
        break;
    case 4:
        FileName = "assets/coinPatterns/coinPattern5.txt";
        break;
    case 5:
        FileName = "assets/coinPatterns/coinPattern6.txt";
        break;
    case 6:
        FileName = "assets/coinPatterns/coinPattern7.txt";
        break;
    case 7:
        FileName = "assets/coinPatterns/coinPattern8.txt";
        break;
    case 8:
        FileName = "assets/coinPatterns/coinPattern9.txt";
        break;
    case 9:
        FileName = "assets/coinPatterns/coinPattern10.txt";
        break;
    case 10:
        FileName = "assets/coinPatterns/coinPattern11.txt";
        break;
    case 11:
        FileName = "assets/coinPatterns/coinPattern12.txt";
        break;
    case 12:
        FileName = "assets/coinPatterns/coinPattern13.txt";
        break;
    case 13:
        FileName = "assets/coinPatterns/coinPattern14.txt";
        break;
    case 14:
        FileName = "assets/coinPatterns/coinPattern15.txt";
        break;
    case 15:
        FileName = "assets/coinPatterns/coinPattern16.txt";
        break;
    case 16:
        FileName = "assets/coinPatterns/coinPattern17.txt";
        break;
    case 17:
        FileName = "assets/coinPatterns/coinPattern18.txt";
        break;
    case 18:
        FileName = "assets/coinPatterns/coinPattern19.txt";
        break;
    case 19:
        FileName = "assets/coinPatterns/coinPattern20.txt";
        break;
    case 20:
        FileName = "assets/coinPatterns/coinPattern21.txt";
        break;
    case 21:
        FileName = "assets/coinPatterns/coinPattern22.txt";
        break;
    case 22:
        FileName = "assets/coinPatterns/coinPattern23.txt";
        break;
    case 23:
        FileName = "assets/coinPatterns/coinPattern24.txt";
        break;
    case 24:
        FileName = "assets/coinPatterns/coinPattern25.txt";
        break;
    case 25:
        FileName = "assets/coinPatterns/coinPattern26.txt";
        break;
    case 26:
        FileName = "assets/coinPatterns/coinPattern27.txt";
        break;
    }
    k++;
}

void GravityPatternGenerator(std::string &FileName)
{
    int k = rand();
    switch (k % 11)
    {
    case 0:
        FileName = "assets/coinPatterns/coinPatternbttf1.txt";
        break;
    case 1:
        FileName = "assets/coinPatterns/coinPatternbttf2.txt";
        break;
    case 2:
        FileName = "assets/coinPatterns/coinPatternbtff3.txt";
        break;
    case 3:
        FileName = "assets/coinPatterns/coinPatternbtff4.txt";
        break;
    case 4:
        FileName = "assets/coinPatterns/coinPatternCFT1.txt";
        break;
    case 5:
        FileName = "assets/coinPatterns/coinPatternCFT2.txt";
        break;
    case 6:
        FileName = "assets/coinPatterns/coinPatternCFT3.txt";
        break;
    case 7:
        FileName = "assets/coinPatterns/coinPatternDC1.txt";
        break;
    case 8:
        FileName = "assets/coinPatterns/coinPatternDC2.txt";
        break;
    case 9:
        FileName = "assets/coinPatterns/coinPatternDC3.txt";
        break;
    case 10:
        FileName = "assets/coinPatterns/coinPatternGravSpec.txt";
        break;
    case 11:
        FileName = "assets/coinPatterns/coinPatternGravSpec2.txt";
        break;
    }
    k++;
}

void CreateCoin()
{
    CN.images = new Texture[6];
    CN.size = 6;
    CN.CurrentIndex = 0;
    CN.FolderAddress = "assets/pic/coin/";
    for (int i = 0; i < CN.size; i++)
    {
        std::string temp = CN.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        CN.images[i] = SBDL::loadTexture(temp);
    }
}

void CoinCreator()
{
    std::string FileName;
    std::string temp;
    if (GravMode)
    {
        GravityPatternGenerator(FileName);
    }
    else
    {
        PatternGenerator(FileName);
    }
    std::ifstream reader(FileName);
    int LineCounter = 0;
    int LineLength = 0;
    while (getline(reader, temp))
    {
        LineLength = std::max(LineLength, (int)(temp.size()));
        LineCounter++;
    }
    reader.clear();
    reader.seekg(0, std::ios::beg);
    int j = 0;
    while (getline(reader, temp))
    {
        if (!temp.empty())
        {
            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i] == '1')
                {
                    C[j][i].active = true;
                }
                C[j][i].coins.h = 40;
                C[j][i].coins.w = 40;
            }
        }
        j++;
    }
    Activator.CoinActive = true;
}

void CoinDegnerator()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            C[i][j].active = false;
            C[i][j].x = 0;
            C[i][j].y = 0;
            C[i][j].coins.x = 0;
            C[i][j].coins.y = 0;
        }
    }
    Activator.CoinActive = false;
}

void ShowCoin()
{

    static int CoinSpeed = 0;

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            C[j][i].y = 40 * j;
            C[j][i].x = 2048 + 40 * i;
            if (C[j][i].active)
            {
                SBDL::showTexture(CN.images[CN.CurrentIndex], C[j][i].x + CoinSpeed, C[j][i].y);
                C[j][i].active = true;
                C[j][i].coins.x = C[j][i].x + CoinSpeed;
                C[j][i].coins.y = C[j][i].y;
            }
        }
    }

    CoinSpeed -= SP;
    if (CoinSpeed <= -4096)
    {
        CoinSpeed = 0;
        CoinDegnerator();
    }
    SBDL::updateRenderScreen();
}

void CoinScoring()
{
    int c = 0;
    Sound *CoinsSound = SBDL::loadSound("assets/sound/coin.wav");
    SDL_Rect Player = {Barry.x, Barry.y, 50, 60};
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (CheckInter(Player, C[j][i].coins))
            {
                if (C[j][i].active)
                {
                    C[j][i].active = false;
                    CoinsScore++;
                    c++;
                }
            }
        }
    }
    if (c > 0)
    {
        SBDL::playSound(CoinsSound, SoundON);
    }
}

void Create()
{
    BG.images = new Texture[12];
    BG.size = 12;
    BG.CurrentIndex = 0;
    BG.FolderAddress = "assets/pic/back/";
    for (int i = 0; i < BG.size; i++)
    {
        std::string temp = BG.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        BG.images[i] = SBDL::loadTexture(temp);
    }

    PL.images = new Texture[4];
    PL.size = 4;
    PL.CurrentIndex = 0;
    if (Costume == 0)
    {
        PL.FolderAddress = "assets/pic/barry/Normal Barry/";
    }
    else
    {
        PL.FolderAddress = "assets/pic/barry/Alien/";
    }
    for (int i = 0; i < PL.size; i++)
    {
        std::string temp = PL.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        PL.images[i] = SBDL::loadTexture(temp);
    }

    GPL.images = new Texture[4];
    GPL.size = 4;
    GPL.CurrentIndex = 0;
    GPL.FolderAddress = "assets/pic/barry/Gravity/";
    for (int i = 0; i < GPL.size; i++)
    {
        std::string temp = GPL.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        GPL.images[i] = SBDL::loadTexture(temp);
    }

    Miss0.images = new Texture[3];
    Miss0.size = 3;
    Miss0.CurrentIndex = 0;
    Miss0.FolderAddress = "assets/pic/missle/";
    for (int i = 0; i < Miss0.size; i++)
    {
        std::string temp = Miss0.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Miss0.images[i] = SBDL::loadTexture(temp);
    }

    Miss1.images = new Texture[3];
    Miss1.size = 3;
    Miss1.CurrentIndex = 0;
    Miss1.FolderAddress = "assets/pic/missle/";
    for (int i = 0; i < Miss1.size; i++)
    {
        std::string temp = Miss1.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Miss1.images[i] = SBDL::loadTexture(temp);
    }

    Miss2.images = new Texture[3];
    Miss2.size = 3;
    Miss2.CurrentIndex = 0;
    Miss2.FolderAddress = "assets/pic/missle/";
    for (int i = 0; i < Miss2.size; i++)
    {
        std::string temp = Miss2.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Miss2.images[i] = SBDL::loadTexture(temp);
    }

    Miss3.images = new Texture[3];
    Miss3.size = 3;
    Miss3.CurrentIndex = 0;
    Miss3.FolderAddress = "assets/pic/missle/";
    for (int i = 0; i < Miss3.size; i++)
    {
        std::string temp = Miss3.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Miss3.images[i] = SBDL::loadTexture(temp);
    }

    Miss4.images = new Texture[3];
    Miss4.size = 3;
    Miss4.CurrentIndex = 0;
    Miss4.FolderAddress = "assets/pic/missle/";
    for (int i = 0; i < Miss4.size; i++)
    {
        std::string temp = Miss4.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Miss4.images[i] = SBDL::loadTexture(temp);
    }

    Smoke0.images = new Texture[6];
    Smoke0.CurrentIndex = 0;

    Smoke1.images = new Texture[6];
    Smoke1.CurrentIndex = 0;

    Smoke2.images = new Texture[6];
    Smoke2.CurrentIndex = 0;

    Smoke3.images = new Texture[6];
    Smoke3.CurrentIndex = 0;

    Smoke4.images = new Texture[6];
    Smoke4.CurrentIndex = 0;

    Smoke0.FolderAddress = "assets/pic/smoke/";
    for (int i = 0; i < 6; i++)
    {
        std::string temp = Smoke0.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        Smoke0.images[i] = SBDL::loadTexture(temp);
        Smoke1.images[i] = SBDL::loadTexture(temp);
        Smoke2.images[i] = SBDL::loadTexture(temp);
        Smoke3.images[i] = SBDL::loadTexture(temp);
        Smoke4.images[i] = SBDL::loadTexture(temp);
    }

    ZapperH.images = new Texture[4];
    ZapperH.CurrentIndex = 0;
    ZapperH.FolderAddress = "assets/pic/zappers/Horizontal/";

    ZapperD.images = new Texture[4];
    ZapperD.CurrentIndex = 0;
    ZapperD.FolderAddress = "assets/pic/zappers/Vertical/";

    ZapperV.images = new Texture[4];
    ZapperV.CurrentIndex = 0;
    ZapperV.FolderAddress = "assets/pic/zappers/Vertical/";

    for (int i = 0; i < 4; i++)
    {
        std::string tempH = ZapperH.FolderAddress + std::to_string(i + 1);
        std::string tempD = ZapperD.FolderAddress + std::to_string(i + 1);
        std::string tempV = ZapperV.FolderAddress + std::to_string(i + 1);

        tempV.append(".png");
        tempD.append(".png");
        tempH.append(".png");

        ZapperH.images[i] = SBDL::loadTexture(tempH);
        ZapperD.images[i] = SBDL::loadTexture(tempD);
        ZapperV.images[i] = SBDL::loadTexture(tempV);
    }

    ST.images = new Texture[4];
    ST.CurrentIndex = 0;
    ST.size = 4;
    ST.FolderAddress = "assets/pic/speedToken/";
    for (int i = 0; i < ST.size; i++)
    {
        std::string temp = ST.FolderAddress + std::to_string(i + 1);
        temp.append(".png");
        ST.images[i] = SBDL::loadTexture(temp);
    }
}

void MissleDegenerator()
{
    if (M[0].active && M[0].x <= -100)
    {
        M[0].active = false;
        M[0].moving = false;
        M[0].x = 2048 - 100;
        Smoke0.CurrentIndex = 0;
        Miss0.CurrentIndex = 0;
        timer0 = 0;
        if (!M[1].active)
        {
            Activator.MisActive = false;
        }
    }
    if (M[1].active && M[1].x <= -100)
    {
        M[1].active = false;
        M[1].moving = false;
        M[1].x = 2048 - 100;
        Smoke1.CurrentIndex = 0;
        Miss1.CurrentIndex = 0;
        timer1 = 0;
        if (!M[2].active)
        {
            Activator.MisActive = false;
        }
    }
    if (M[2].active && M[2].x <= -100)
    {
        M[2].active = false;
        M[2].moving = false;
        M[2].x = 2048 - 100;
        Smoke2.CurrentIndex = 0;
        Miss2.CurrentIndex = 0;
        timer2 = 0;
        if (!M[3].active)
        {
            Activator.MisActive = false;
        }
    }
    if (M[3].active && M[3].x <= -100)
    {
        M[3].active = false;
        M[3].moving = false;
        M[3].x = 2048 - 100;
        Miss3.CurrentIndex = 0;
        timer3 = 0;
        if (!M[4].active)
        {
            Activator.MisActive = false;
        }
    }
    if (M[4].active && M[4].x <= -100)
    {
        M[4].active = false;
        M[4].moving = false;
        M[4].x = 2048 - 100;
        Miss4.CurrentIndex = 0;
        timer4 = 0;
    }
}

void MissleMover()
{
    if (timer0 >= 40)
    {
        M[0].moving = true;
    }
    if (timer1 >= 60)
    {
        M[1].moving = true;
    }
    if (timer2 >= 80)
    {
        M[2].moving = true;
    }
    if (timer3 >= 100)
    {
        M[3].moving = true;
    }
    if (timer4 >= 120)
    {
        M[4].moving = true;
    }
}

void ShowMissle()
{
    static int Winds = 0;
    int l = 0;
    Sound *Warning = SBDL::loadSound("assets/sound/missile_warning.wav");
    Sound *Launch = SBDL::loadSound("assets/sound/missile_launch.wav");
    if (M[0].active)
    {
        SBDL::showTexture(Miss0.images[Miss0.CurrentIndex], M[0].x, M[0].y);
        SBDL::showTexture(Smoke0.images[Smoke0.CurrentIndex], M[0].x + 35, M[0].y);
        if (timer0 < 40)
        {
            M[0].y = Barry.y;
        }
        if (timer0 == 40)
        {
            Miss0.CurrentIndex++;
            l++;
            SBDL::playSound(Warning, SoundON);
        }
        else if (M[0].moving)
        {
            Miss0.CurrentIndex = 2;
            M[0].x -= M[0].speed;
        }
    }
    if (M[1].active)
    {
        SBDL::showTexture(Miss1.images[Miss1.CurrentIndex], M[1].x, M[1].y);
        SBDL::showTexture(Smoke1.images[Smoke1.CurrentIndex], M[1].x + 35, M[1].y);
        if (timer1 < 60)
        {
            M[1].y = Barry.y;
        }
        if (timer1 == 60)
        {
            Miss1.CurrentIndex++;
            l++;
            SBDL::playSound(Warning, SoundON);
        }
        else if (M[1].moving)
        {
            Miss1.CurrentIndex = 2;
            M[1].x -= M[1].speed;
        }
    }
    if (M[2].active)
    {
        SBDL::showTexture(Miss2.images[Miss2.CurrentIndex], M[2].x, M[2].y);
        SBDL::showTexture(Smoke2.images[Smoke2.CurrentIndex], M[2].x + 35, M[2].y);
        if (timer2 < 80)
        {
            M[2].y = Barry.y;
        }
        if (timer2 == 80)
        {
            Miss2.CurrentIndex++;
            l++;
            SBDL::playSound(Warning, SoundON);
        }
        else if (M[2].moving)
        {
            Miss2.CurrentIndex = 2;
            M[2].x -= M[2].speed;
        }
    }
    if (M[3].active)
    {
        SBDL::showTexture(Miss3.images[Miss3.CurrentIndex], M[3].x, M[3].y);
        SBDL::showTexture(Smoke3.images[Smoke3.CurrentIndex], M[3].x + 35, M[3].y);
        if (timer3 < 100)
        {
            M[3].y = Barry.y;
        }
        if (timer3 == 100)
        {
            Miss3.CurrentIndex++;
            l++;
            SBDL::playSound(Warning, SoundON);
        }
        else if (M[3].moving)
        {
            Miss3.CurrentIndex = 2;
            M[3].x -= M[3].speed;
        }
    }
    if (M[4].active)
    {
        SBDL::showTexture(Miss4.images[Miss4.CurrentIndex], M[4].x, M[4].y);
        SBDL::showTexture(Smoke4.images[Smoke4.CurrentIndex], M[4].x + 35, M[4].y);
        if (timer4 < 120)
        {
            M[4].y = Barry.y;
        }
        if (timer4 == 120)
        {
            Miss4.CurrentIndex++;
            l++;
            SBDL::playSound(Warning, SoundON);
        }
        else if (M[4].moving)
        {
            Miss4.CurrentIndex = 2;
            M[4].x -= M[4].speed;
        }
    }
    if (l > 0 && (M[0].moving || M[1].moving || M[2].moving || M[3].moving || M[4].moving))
    {
        SBDL::playSound(Launch, SoundON);
        l = 0;
    }
    Smoke0.CurrentIndex++;
    if (Smoke0.CurrentIndex >= 6)
    {
        Smoke0.CurrentIndex = 0;
    }
    Smoke1.CurrentIndex++;
    if (Smoke1.CurrentIndex >= 6)
    {
        Smoke1.CurrentIndex = 0;
    }
    Smoke2.CurrentIndex++;
    if (Smoke2.CurrentIndex >= 6)
    {
        Smoke2.CurrentIndex = 0;
    }
    Smoke3.CurrentIndex++;
    if (Smoke3.CurrentIndex >= 6)
    {
        Smoke3.CurrentIndex = 0;
    }
    Smoke4.CurrentIndex++;
    if (Smoke4.CurrentIndex >= 6)
    {
        Smoke4.CurrentIndex = 0;
    }
}

void MissleCreator()
{
    if (!M[0].active && !M[1].active && !M[2].active && !M[3].active && !M[4].active && !Activator.LasActive)
    {
        Activator.MisActive = true;
        int random = rand() % 5;
        for (int i = 0; i <= random - NOA; i++)
        {
            M[i].active = true;
        }
    }
}

void CreateZapper()
{
    ZapperV.images = new Texture[4];
    ZapperH.images = new Texture[4];
    ZapperD.images = new Texture[4];
    ZapperV.size = 4;
    ZapperV.CurrentIndex = 0;
    ZapperH.CurrentIndex = 0;
    ZapperD.CurrentIndex = 0;
    ZapperV.FolderAddress = "assets/pic/zappers/Vertical/";
    ZapperH.FolderAddress = "assets/pic/zappers/Horizontal/";
    ZapperD.FolderAddress = "assets/pic/zappers/Diagonal/";
    for (int i = 0; i < ZapperV.size; i++)
    {
        std::string tempV = ZapperV.FolderAddress + std::to_string(i + 1);
        std::string tempH = ZapperH.FolderAddress + std::to_string(i + 1);
        std::string tempD = ZapperD.FolderAddress + std::to_string(i + 1);

        tempV.append(".png");
        tempH.append(".png");
        tempD.append(".png");

        ZapperV.images[i] = SBDL::loadTexture(tempV);
        ZapperH.images[i] = SBDL::loadTexture(tempH);
        ZapperD.images[i] = SBDL::loadTexture(tempD);
    }
}

void ZapperCreator()
{
    if (!Zapper[0].active && !Zapper[1].active && !Zapper[2].active && !Activator.LasActive)
    {
        Activator.ZapActive = true;
        int random = rand() % 3;
        for (int i = 0; i <= random; i++)
        {
            Zapper[i].active = true;
        }
    }
}

void ShowZapper()
{
    int l = 0;
    static int k = 0;
    static int Winds = 0;
    Sound *zappy = SBDL::loadSound("assets/sound/zapper.wav");
    if (!Zapper[0].active)
    {
        Zapper[0].x = 2048;
        Zapper[0].y = rand() % 400;
    }
    if (!Zapper[1].active)
    {
        Zapper[1].x = Zapper[0].x + rand() % 400;
        Zapper[1].y = rand() % 400;
    }
    if (!Zapper[2].active)
    {
        Zapper[2].x = Zapper[1].x + rand() % 400;
        Zapper[2].y = rand() % 400;
    }
    if (Zapper[0].active && Zapper[1].x - Zapper[0].x < 100)
    {
        Zapper[1].x += 100;
    }
    if (Zapper[1].active && Zapper[2].x - Zapper[1].x < 100)
    {
        Zapper[2].x += 100;
    }
    if (Zapper[0].active)
    {
        SBDL::showTexture(ZapperV.images[ZapperV.CurrentIndex], Zapper[0].x, Zapper[0].y);
        Zapper[0].x -= SP;
        l++;
    }
    if (Zapper[1].active)
    {
        SBDL::showTexture(ZapperH.images[ZapperH.CurrentIndex], Zapper[1].x, Zapper[1].y);
        Zapper[1].x -= SP;
        l++;
    }
    if (Zapper[2].active)
    {
        SBDL::showTexture(ZapperD.images[ZapperD.CurrentIndex], Zapper[2].x, Zapper[2].y);
        Zapper[2].x -= SP;
        l++;
    }
    if (Winds % 5 == 0)
    {
        ZapperD.CurrentIndex++;
        ZapperH.CurrentIndex++;
        ZapperV.CurrentIndex++;
    }
    if (ZapperV.CurrentIndex >= 4)
    {
        ZapperV.CurrentIndex = 0;
    }
    if (ZapperH.CurrentIndex >= 4)
    {
        ZapperH.CurrentIndex = 0;
    }
    if (ZapperD.CurrentIndex >= 4)
    {
        ZapperD.CurrentIndex = 0;
    }
    Winds++;
    k++;
    if (l > 0 && k % 10 == 1)
    {
        SBDL::playSound(zappy, SoundON);
    }
}

void ZapperDegenerator()
{
    if (Zapper[0].active && Zapper[0].x <= -850)
    {
        Zapper[0].active = false;
    }
    if (Zapper[1].active && Zapper[1].x <= -850)
    {
        Zapper[1].active = false;
    }
    if (Zapper[2].active && Zapper[2].x <= -850)
    {
        Zapper[2].active = false;
    }
    if(!Zapper[0].active && !Zapper[1].active && !Zapper[2].active)
    {
        Activator.ZapActive = false;
    }
}

void GravityCreator()
{
    Texture GravToken = SBDL::loadTexture("assets/pic/menu/gravity_token.png");
    if (!G.active && !GravMode)
    {
        G.x = 2048;
        G.y = rand() % 400;
        G.active = true;
    }
}

void ShowGravToken()
{
    Texture GravToken = SBDL::loadTexture("assets/pic/menu/gravity_token.png");
    if (G.active && !GravMode)
    {
        SBDL::showTexture(GravToken, G.x, G.y);
        G.x -= SP;
    }
}

void GravTokenDegenerator()
{
    if (G.active && G.x <= -100)
    {
        G.active = false;
        G.x = 2448;
        G.y = 0;
    }
}


void SpeedTokenDegenerator()
{
    if (Token.x <= -100)
    {
        Token.active = false;
    }
}

void SpeedTokenCreator()
{
    Token.active = true;
    Token.x = 2048;
    Token.y = rand() % 300;
}

void ShowSpeed()
{
    if (Token.active)
    {
        SBDL::showTexture(ST.images[ST.CurrentIndex], Token.x, Token.y);
    }
}

void MoveSpeedToken()
{
    static int dist = 0;
    static int TokenDir = -1;
    if (dist % 10 == 0)
    {
        TokenDir *= -1;
    }
    Token.x -= 15;
    Token.y += TokenDir * SP;
    dist++;
}

void HyperActiveMode()
{
    Texture Boost = SBDL::loadTexture("assets/pic/barry/barryst.png");
    Rebirth();
    for (int i = 0; i < 30; i++)
    {
        SBDL::clearRenderScreen();
        SBDL::showTexture(BG.images[BG.CurrentIndex], BackSpeed, 0);
        if (BG.CurrentIndex == BG.size - 1)
        {
            SBDL::showTexture(BG.images[0], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
        }
        else
        {
            SBDL::showTexture(BG.images[BG.CurrentIndex + 1], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
        }
        Barry.x += 15;
        Barry.y = 230;
        Math();
        MoveScore += 10;
        SBDL::showTexture(Boost, Barry.x, Barry.y);
        SBDL::updateRenderScreen();
    }
    for (int i = 0; i < 30; i++)
    {
        SBDL::clearRenderScreen();
        SBDL::showTexture(BG.images[BG.CurrentIndex], BackSpeed, 0);
        if (BG.CurrentIndex == BG.size - 1)
        {
            SBDL::showTexture(BG.images[0], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
        }
        else
        {
            SBDL::showTexture(BG.images[BG.CurrentIndex + 1], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
        }
        Barry.x -= 15;
        Barry.y = 230;
        Math();
        MoveScore += 10;
        SBDL::showTexture(Boost, Barry.x, Barry.y);
        SBDL::updateRenderScreen();
    }
}

void Move()
{
    if (SBDL::keyHeld(SDL_SCANCODE_SPACE))
    {
        PL.CurrentIndex = 3;

        if (Barry.y >= 0)
        {
            Barry.v -= Barry.a;
            Barry.y += Barry.v;
        }
    }
    else if (SBDL::keyReleased(SDL_SCANCODE_SPACE))
    {
        PL.CurrentIndex = 2;

        if (Barry.y != 0)
        {
            Barry.y += Barry.v / 2 + 5;
        }
        Barry.v = 0;
    }
    else
    {
        if (Barry.y != 365)
        {
            PL.CurrentIndex = 2;
        }
        if (Barry.y <= 365)
        {
            Barry.v += Barry.a;
            Barry.y += Barry.v;
        }
    }
    if (Barry.y <= 0)
    {
        Barry.y = 0;
        Barry.v = 0;
    }
    if (Barry.y >= 365)
    {
        Barry.y = 365;
        Barry.v = 0;
    }
    SBDL::updateEvents();
}

void GravMove()
{
    Sound *GravUp = SBDL::loadSound("assets/sound/grav_change_1.wav");
    Sound *GravDown = SBDL::loadSound("assets/sound/grav_change_2.wav");
    if (SBDL::keyHeld(SDL_SCANCODE_SPACE))
    {
        GC = 0;
        GravDir *= -1;
    }
    if (Barry.y >= 0 && Barry.y <= 365)
    {
        Barry.v += GravDir * Barry.a;
        Barry.y += Barry.v;
    }
    if (Barry.y < 0)
    {
        Barry.v = 0;
        Barry.y = 0;
    }
    if (Barry.y > 365)
    {
        Barry.v = 0;
        Barry.y = 365;
    }
    if (GravDir == -1 && GC == 0)
    {
        SBDL::playSound(GravUp, SoundON);
        GC++;
        GPL.CurrentIndex = 2;
    }
    else if (GravDir == 1 && GC == 0)
    {
        SBDL::playSound(GravDown, SoundON);
        GC++;
        GPL.CurrentIndex = 0;
    }
    SBDL::updateEvents();
}

void Generate()
{
    int a = rand();
    int b = rand();
    int random = a * b;
    int HardRandom = a * b;
    HardRandom %= 100;
    random %= 6;
    if (random == 1)
    {
        MissleCreator();
    }
    else if (random == 3 && !Activator.LasActive)
    {
        ZapperCreator();
    }
    if (HardRandom == 17)
    {
        GravityCreator();
    }
    else if (HardRandom == 11 && !Token.active && !GravMode)
    {
        SpeedTokenCreator();
    }
}

void Collision()
{
    SDL_Rect M0 = {M[0].x + 20, M[0].y + 20, 20, 13};
    SDL_Rect M1 = {M[1].x + 20, M[1].y + 20, 20, 13};
    SDL_Rect M2 = {M[2].x + 20, M[2].y + 20, 20, 13};
    SDL_Rect M3 = {M[3].x + 20, M[3].y + 20, 20, 13};
    SDL_Rect M4 = {M[4].x + 20, M[4].y + 20, 20, 13};

    SDL_Rect Coffee = {Token.x, Token.y, 50, 50};

    SDL_Rect ZV = {Zapper[0].x + 11 , Zapper[0].y + 21 , 3 , 90};
    SDL_Rect ZH = {Zapper[1].x + 21, Zapper[1].y + 11 , 90 , 3};
    SDL_Rect ZD = {Zapper[2].x + 20, Zapper[2].y + 25, 15, 85};

    SDL_Rect GravBox = {G.x, G.y, 100, 100};

    SDL_Rect Player = {Barry.x, Barry.y, 50, 60};

    if (SBDL::hasIntersectionRect(Player, M0) && M[0].active)
    {
        Barry.life--;
        M[0].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, M1) && M[1].active)
    {
        Barry.life--;
        M[1].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, M2) && M[2].active)
    {
        Barry.life--;
        M[2].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, M3) && M[3].active)
    {
        Barry.life--;
        M[3].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, M4) && M[4].active)
    {
        Barry.life--;
        M[4].active = false;
    }

    if (SBDL::hasIntersectionRect(Player, ZV) && Zapper[0].active)
    {
        Barry.life--;
        Zapper[0].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, ZH) && Zapper[1].active)
    {
        Barry.life--;
        Zapper[1].active = false;
    }
    if (SBDL::hasIntersectionRect(Player, ZD) && Zapper[2].active)
    {
        Barry.life--;
        Zapper[2].active = false;
    }

    if (SBDL::hasIntersectionRect(Player, Coffee) && Token.active)
    {
        Token.active = false;
        HyperActiveMode();
    }

    if (SBDL::hasIntersectionRect(Player, GravBox) && G.active)
    {
        Rebirth();
        GravMode = true;
        Barry.life++;
        G.active = false;
    }
}

int main()
{

    srand(time(NULL));
    const int windowwidth = 2048, windowheight = 460;
    SBDL::InitEngine("JetPack JoyRide", windowwidth, windowheight);

    Font *font = SBDL::loadFont("assets/font/Jetpackia.ttf", 50);
    Texture score;
    Texture MScore;

    StartMenu();
    Music *music = SBDL::loadMusic("assets/sound/music.mp3");
    SBDL::playMusic(music, MusicON);
    if (!Activator.CoinActive)
        CoinCreator();
    Create();
    CreateCoin();
    CreateZapper();

    const int FPS = 40;
    const int delay = 1500 / FPS;

    while (SBDL::isRunning)
    {
        if (SBDL::keyPressed(SDL_SCANCODE_P))
        {
            Paused = true;
        }
        SBDL::clearRenderScreen();
        if (Barry.life <= 0)
        {
            lose = true;
        }
        Score = std::to_string(CoinsScore);
        MVScore = std::to_string(MoveScore);
        score = SBDL::createFontTexture(font, Score, 255, 255, 255);
        MScore = SBDL::createFontTexture(font, MVScore, 255, 255, 255);
        counter++;
        SBDL::updateEvents();
        int LoopStartTime = SBDL::getTime();
        if (Paused)
        {
            ShowPauseMenu();
        }
        else
        {
            if (GameOver)
            {
                break;
            }
            else if (lose)
            {
                CheckHighScore();
                ShowLoseMenu();
            }
            else
            {
                if (Barry.life == 1 && GravMode)
                {
                    GC = 0;
                    GravDir = 1;
                    GravMode = false;
                    Changer = 0;
                    Rebirth();
                    PL.CurrentIndex = 0;
                    GPL.CurrentIndex = 0;
                }
                if (M[0].active)
                {
                    timer0++;
                }
                if (M[1].active)
                {
                    timer1++;
                }
                if (M[2].active)
                {
                    timer2++;
                }
                if (M[3].active)
                {
                    timer3++;
                }
                if (M[4].active)
                {
                    timer4++;
                }
                if (!Activator.CoinActive && !Activator.LasActive)
                    CoinCreator();
                Generate();
                MissleMover();
                MoveSpeedToken();

                SBDL::clearRenderScreen();
                SBDL::showTexture(BG.images[BG.CurrentIndex], BackSpeed, 0);
                if (BG.CurrentIndex == BG.size - 1)
                {
                    SBDL::showTexture(BG.images[0], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
                }
                else
                {
                    SBDL::showTexture(BG.images[BG.CurrentIndex + 1], BG.images[BG.CurrentIndex].width + BackSpeed, 0);
                }
                if (GravMode)
                {
                    SBDL::showTexture(GPL.images[GPL.CurrentIndex], Barry.x, Barry.y);
                }
                else
                {
                    SBDL::showTexture(PL.images[PL.CurrentIndex], Barry.x, Barry.y);
                }
                SBDL::showTexture(score, 150, 0);
                SBDL::showTexture(MScore, 1800, 0);

                Math();
                if (GravMode)
                {
                    GravMove();
                }
                else
                {
                    Move();
                }

                ShowZapper();
                ShowMissle();
                ShowGravToken();
                ShowSpeed();

                ShowCoin();

                CoinScoring();
                MissleDegenerator();
                ZapperDegenerator();
                GravTokenDegenerator();
                SpeedTokenDegenerator();
                Collision();
                SBDL::updateRenderScreen();
                MoveScore++;
                if (MoveScore % 500 == 0 && MoveScore > 0 && M[0].speed <= 30)
                {
                    SP += 1;
                    M[0].speed += SP;
                    M[1].speed += SP;
                    M[2].speed += SP;
                    M[3].speed += SP;
                    M[4].speed += SP;
                }
                if (MoveScore % 300 == 0 && MoveScore > 0)
                {
                    if (NOA > 0)
                    {
                        NOA--;
                    }
                    if (NOA == 0)
                    {
                        NOA += 2;
                    }
                }

                if (SBDL::keyPressed(SDL_SCANCODE_P))
                {
                    Paused = true;
                }
                if (GameOver)
                {
                    break;
                }
                SBDL::updateEvents();

                int ElapsedTime = SBDL::getTime() - LoopStartTime;
                if (ElapsedTime < delay)
                {
                    SBDL::delay(delay - ElapsedTime);
                }
            }
        }
    }
    SBDL::freeMusic(music);
    delete[] BG.images;
    delete[] PL.images;
}
