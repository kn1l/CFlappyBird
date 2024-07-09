#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>

#define PI 3.14159265359
#define EPSILON 1e-9

IMAGE canvas;

int isEnd = 0;
// bird
struct Bird {
    double x = 50, y = 190;
    double radian;
    double v;
    double radianV;
    int pose = 1;
} bird;
int posePace = 1, poseCounter;
// pipe
double pipeX[2] = { -100, -100 }, pipeY[2] = { -100, -100 };
// land
double landX = 0;
//当前得分
int score = 0;
//是否在管道中间
int inPipe = 0;
// 最高分
int bestScore;


//====常量====//
//加速度
const double G = 0.2;
const double R = PI / 600;
//管道间距
const int GAPX = 170;
const int GAPY = 110;
//管道速度
const double pipeV = 1;
const double landV = 1;



IMAGE BIRD[3][3];
IMAGE BACKGROUND[2];
IMAGE LAND;
IMAGE PIPE[2][2];
IMAGE BUTTON;
IMAGE MEDALS[4];
IMAGE NEW;
IMAGE NUMBER[3][10];
IMAGE SCORE_PANEL;
IMAGE TEXT_GAME_OVER;
IMAGE TEXT_READY;
IMAGE TITLE;
IMAGE TUTORIAL;

void StartGame();
void EndGame();

void LoadImages() {
    loadimage(&BIRD[0][0], _T("img/bird0_0.png"));
    loadimage(&BIRD[0][1], _T("img/bird0_1.png"));
    loadimage(&BIRD[0][2], _T("img/bird0_2.png"));
    loadimage(&BIRD[1][0], _T("img/bird1_0.png"));
    loadimage(&BIRD[1][1], _T("img/bird1_1.png"));
    loadimage(&BIRD[1][2], _T("img/bird1_2.png"));
    loadimage(&BIRD[2][0], _T("img/bird2_0.png"));
    loadimage(&BIRD[2][1], _T("img/bird2_1.png"));
    loadimage(&BIRD[2][2], _T("img/bird2_2.png"));
    loadimage(&BACKGROUND[0], _T("img/bg_day.png"));
    loadimage(&BACKGROUND[1], _T("img/bg_night.png"));
    loadimage(&LAND, _T("img/land.png"));
    loadimage(&PIPE[0][0], _T("img/pipe_down.png"));
    loadimage(&PIPE[0][1], _T("img/pipe_up.png"));
    loadimage(&PIPE[1][0], _T("img/pipe2_down.png"));
    loadimage(&PIPE[1][1], _T("img/pipe2_up.png"));
    loadimage(&BUTTON, _T("img/button_play.png"));
    loadimage(&NUMBER[2][0], _T("img/font_048.png"));
    loadimage(&NUMBER[2][1], _T("img/font_049.png"));
    loadimage(&NUMBER[2][2], _T("img/font_050.png"));
    loadimage(&NUMBER[2][3], _T("img/font_051.png"));
    loadimage(&NUMBER[2][4], _T("img/font_052.png"));
    loadimage(&NUMBER[2][5], _T("img/font_053.png"));
    loadimage(&NUMBER[2][6], _T("img/font_054.png"));
    loadimage(&NUMBER[2][7], _T("img/font_055.png"));
    loadimage(&NUMBER[2][8], _T("img/font_056.png"));
    loadimage(&NUMBER[2][9], _T("img/font_057.png"));
    loadimage(&MEDALS[0], _T("img/medals_0.png"));
    loadimage(&MEDALS[1], _T("img/medals_1.png"));
    loadimage(&MEDALS[2], _T("img/medals_2.png"));
    loadimage(&MEDALS[3], _T("img/medals_3.png"));
    loadimage(&NEW, _T("img/new.png"));
    loadimage(&NUMBER[0][0], _T("img/number_context_00.png"));
    loadimage(&NUMBER[0][1], _T("img/number_context_01.png"));
    loadimage(&NUMBER[0][2], _T("img/number_context_02.png"));
    loadimage(&NUMBER[0][3], _T("img/number_context_03.png"));
    loadimage(&NUMBER[0][4], _T("img/number_context_04.png"));
    loadimage(&NUMBER[0][5], _T("img/number_context_05.png"));
    loadimage(&NUMBER[0][6], _T("img/number_context_06.png"));
    loadimage(&NUMBER[0][7], _T("img/number_context_07.png"));
    loadimage(&NUMBER[0][8], _T("img/number_context_08.png"));
    loadimage(&NUMBER[0][9], _T("img/number_context_09.png"));
    loadimage(&NUMBER[1][0], _T("img/number_score_00.png"));
    loadimage(&NUMBER[1][1], _T("img/number_score_01.png"));
    loadimage(&NUMBER[1][2], _T("img/number_score_02.png"));
    loadimage(&NUMBER[1][3], _T("img/number_score_03.png"));
    loadimage(&NUMBER[1][4], _T("img/number_score_04.png"));
    loadimage(&NUMBER[1][5], _T("img/number_score_05.png"));
    loadimage(&NUMBER[1][6], _T("img/number_score_06.png"));
    loadimage(&NUMBER[1][8], _T("img/number_score_08.png"));
    loadimage(&NUMBER[1][7], _T("img/number_score_07.png"));
    loadimage(&NUMBER[1][9], _T("img/number_score_09.png"));
    loadimage(&SCORE_PANEL, _T("img/score_panel.png"));
    loadimage(&TEXT_GAME_OVER, _T("img/text_game_over.png"));
    loadimage(&TEXT_READY, _T("img/text_ready.png"));
    loadimage(&TITLE, _T("img/title.png"));
    loadimage(&TUTORIAL, _T("img/tutorial.png"));
}

//========Draw========//
void Draw(IMAGE* dstimg, int x, int y, IMAGE* srcimg) {
    if (dstimg == NULL) {
        return;
    }

    DWORD* dst = GetImageBuffer(dstimg);
    DWORD* src = GetImageBuffer(srcimg);
    int src_width = srcimg->getwidth();
    int src_height = srcimg->getheight();
    int dst_width = dstimg->getwidth();
    int dst_height = dstimg->getheight();

    for (int iy = 0; iy < src_height; iy++) {
        for (int ix = 0; ix < src_width; ix++) {
            int srcX = ix + iy * src_width;
            int sa = ((src[srcX] & 0xff000000) >> 24);
            int sr = ((src[srcX] & 0xff0000) >> 16);
            int sg = ((src[srcX] & 0xff00) >> 8);
            int sb = src[srcX] & 0xff;
            if (x + ix >= 0 && x + ix < dst_width
                && y + iy >= 0 && y + iy < dst_height) {
                int dstX = (x + ix) + (y + iy) * dst_width;
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                if (!(!sa && !sr && !sg && !sb)) {
                    sa = 0xff;
                    dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
                        | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
                        | (sb * sa / 255 + db * (255 - sa) / 255);
                }
            }
        }
    }

}

void DrawBird() {
    IMAGE tmp;
    rotateimage(&tmp, &BIRD[0][bird.pose], bird.radian);
    Draw(&canvas, bird.x, bird.y, &tmp);
}

void DrawPipe() {
    Draw(&canvas, pipeX[0], pipeY[0], &PIPE[0][0]);
    Draw(&canvas, pipeX[1], pipeY[1], &PIPE[0][0]);
    Draw(&canvas, pipeX[0], pipeY[0] + GAPY + 320, &PIPE[0][1]);
    Draw(&canvas, pipeX[1], pipeY[1] + GAPY + 320, &PIPE[0][1]);
}

void DrawLand() {
    Draw(&canvas, landX, 450, &LAND);
}

void DrawScore(int x, int y, int score, int type) { 
    int length = 0;
    int tmp = score;
    length = tmp == 0 ? 1 : 0;
    int numberX = NUMBER[type][0].getwidth();
    while (tmp) {
        length++;
        tmp /= 10;
    }
    for (int i = length - 1; i >= 0; i--) {
        Draw(&canvas, x - numberX / 2 * length + numberX * i, y, &NUMBER[type][score % 10]);
        score /= 10;
    }
}

void DrawGame() {
    Draw(&canvas, 0, 0, &BACKGROUND[0]);
    DrawPipe();
    DrawBird();
    DrawLand();
    DrawScore(144, 50, score, 2);
    putimage(0, 0, &canvas);
}

//========Logic========//

void LogicSpeed(int isUP = 0) {
    if (isUP) {
        bird.v = -4;
    }
    else {
        bird.v = bird.v + G;
    }
}

void LogicRadian(int isUP = 0) {
    if (isUP) {
        bird.radian = PI / 4;
        bird.radianV = 0;
    }
    else {
        bird.radian = bird.radian - bird.radianV;
        bird.radianV = bird.radianV + R;
    }
    if (bird.radian < (-PI / 2)) {
        bird.radian = -PI / 2;
        bird.radianV = 0;
    }
}


void LogicBirdPosition() {
    bird.y = bird.y + bird.v;
}

void LogicPipePosition() {
    pipeX[0] = pipeX[0] - pipeV;
    pipeX[1] = pipeX[0] + GAPX;
    if (pipeX[0] < -52) {
        pipeX[0] = pipeX[1];
        pipeY[0] = pipeY[1];
        pipeX[1] = 288;
        pipeY[1] = -10 - rand() % 280;
    }
}

void LogicScore() {
    if (bird.x >= pipeX[0] && bird.x < pipeX[0] + 52 && !inPipe) {
        score += 1;
        inPipe = 1;
    }
    if (bird.x >= pipeX[0] + 52 && inPipe) {
        inPipe = 0;
    }
}

void LogicPose() {
    if (fabs(bird.radian + PI / 2) < EPSILON) {
        bird.pose = 1;
        return;
    }
    if (posePace == 1 && bird.pose == 2) posePace = -1;
    if (posePace == -1 && bird.pose == 0) posePace = 1;
    if (poseCounter == 5) {
        bird.pose += posePace;
        poseCounter = 0;
    }
    else {
        poseCounter++;
    }
}

void LogicLand() {
    landX -= landV;
    if (landX <= 288 - 336) landX = 0;
}

void LogicHandler() {
    ExMessage message;
    peekmessage(&message);
    flushmessage();
    switch (message.message) {
    case WM_LBUTTONDOWN:
        LogicSpeed(1);
        LogicRadian(1);
        break;
    default:
        LogicSpeed(0);
        LogicRadian(0);
        break;
    }
    if (message.vkcode == 0x58) {
        closegraph();
        exit(0);
    }

    LogicLand();
    LogicPose();
    LogicBirdPosition();
    LogicPipePosition();
    LogicScore();
}

//========Collision Detection========//
void CollisionHandler() {
    // land
    if (bird.y + 40 > 450) {
        isEnd = 1;
    }
    // pipe
    if (bird.x + 34 > pipeX[0] && bird.x + 10 < pipeX[0] + 52) {
        // down or up
        if (bird.y + 10 < pipeY[0] + 320 || bird.y + 40 > pipeY[0] + 320 + GAPY) {
            isEnd = 1;
        }
    }
}

//========End========//
void BestScoreHandler() {
    FILE* fp = fopen("bestScore.txt", "r");
    if (fp == NULL) {
        bestScore = score;
        fp = fopen("bestScore.txt", "w");
        fprintf(fp, "%d", bestScore);
    }
    else {
        fscanf(fp, "%d", &bestScore);
    }
    printf("BestScore: %d\n", bestScore);
    printf("Score:%d\n", score);
    fclose(fp);
    if (bestScore < score) {
        fp = fopen("bestScore.txt", "w");
        bestScore = score;
        fprintf(fp, "%d", bestScore);
        fclose(fp);
    }
}

void EndAnimation() {
    // dark to light
    int dst_width = canvas.getwidth();
    int dst_height = canvas.getheight();
    int sr = 0;
    int sg = 0;
    int sb = 0;
    for (int sa = 0xff; sa >= 0; sa -= 10) {
        IMAGE tmp = canvas;
        DWORD* dst = GetImageBuffer(&tmp);
        for (int iy = 0; iy < dst_height; iy++) {
            for (int ix = 0; ix < dst_width; ix++) {
                int dstX = ix + iy * dst_width;
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
                    | (sb * sa / 255 + db * (255 - sa) / 255);
            }
        }
        putimage(0, 0, &tmp);
        Sleep(1);
    }
    // drop
    bird.v = 4;
    while (bird.y + 40 < 450) {
        LogicSpeed(0);
        LogicRadian(0);
        LogicBirdPosition();
        DrawGame();
        Sleep(10);
    }

}

void DrawEnd() {
    Draw(&canvas, 0, 0, &BACKGROUND[0]);
    DrawPipe();
    DrawBird();
    DrawLand();

    Draw(&canvas, 86, 350, &BUTTON);
    Draw(&canvas, 25, 200, &SCORE_PANEL);
    Draw(&canvas, 42, 130, &TEXT_GAME_OVER);
    Draw(&canvas, 55, 55, &TITLE);
    int medal = score / 10;
    medal = medal > 3 ? 3 : medal;
    Draw(&canvas, 55, 245, &MEDALS[medal]);
    //score
    DrawScore(212, 235, score, 1);
    //bestScore
    DrawScore(218, 275, bestScore, 0);

    putimage(0, 0, &canvas);
}

void EndGame() {
    EndAnimation();
    BestScoreHandler();
    DrawEnd();

    ExMessage message;
    while(1){
        getmessage(&message);
        flushmessage();
        int x = message.x, y = message.y;
        if (message.message == WM_LBUTTONDOWN && x >= 96 && x <= 194 && y >= 356 && y <= 408) {
            break;
        }
        if (message.vkcode == 0x58) {
            closegraph();
            exit(0);
        }
    }
    isEnd = 0;
}

void DrawStart() {
    Draw(&canvas, 0, 0, &BACKGROUND[0]);
    Draw(&canvas, 50, 350, &TEXT_READY);
    Draw(&canvas, 15, 200, &TUTORIAL);
    Draw(&canvas, 55, 40, &TITLE);
    DrawLand();
    putimage(0, 0, &canvas);
}

void StartGame() {
    DrawStart();
    bird.x = 50, bird.y = 190;
    score = 0;
    isEnd = 0;
    inPipe = 0;
    pipeX[0] = 288;
    pipeY[0] = -10 - rand() % 280;
    ExMessage message;
    int isStart = 0;
    while (!isStart) {
        getmessage(&message);
        flushmessage();
        switch (message.message) {
        case WM_LBUTTONDOWN:
            isStart = 1;
            LogicSpeed(1);
            LogicRadian(1);
            break;
        default:
            break;
        }
        if (message.vkcode == 0x58) {
            closegraph();
            exit(0);
        }
    }
    while (!isEnd) {
        LogicHandler();
        DrawGame();
        CollisionHandler();
        Sleep(10);
    }
}

void Init() {
    initgraph(288, 512);
    srand((unsigned int)time(NULL));

    LoadImages();

    loadimage(&canvas, _T("img/bg_day.png"));
    putimage(0, 0, &canvas);

}

int main() {
    Init();
    while (!isEnd) {
        StartGame();
        EndGame();
    }
    closegraph();
    return 0;
}
