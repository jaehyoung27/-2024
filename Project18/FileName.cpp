#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 20
#define PLAYER_CHAR 'O' // 플레이어의 모양
#define OBSTACLE_CHAR '@' // 장애물의 모양
#define MAX_OBSTACLES 2 // 최대 장애물 개수
#define GRAVITY 1       // 플레이어의 중력
#define JUMP_FORCE -3   // 점프 높이
#define PLAYER_HEIGHT 2 // 플레이어의 세로 크기

typedef struct {
    int x;
    int y;
} Position; // 좌표를 나타내는 구조체

// 커서를 특정 위치로 이동하는 함수
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 커서를 숨기는 함수
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 문자를 특정 위치에 그리는 함수
void drawCharacter(char character, int x, int y) {
    gotoxy(x, y);
    printf("%c", character);
}

// 화면을 지우는 함수
void clearScreen() {
    system("cls");
}

// 게임 오버를 출력하는 함수
void gameover(int score) {
    clearScreen();
    gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 1);
    printf("Game Over!");
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    printf("Your score: %d", score);
}

// 장애물을 초기화하는 함수
Position* initializeObstacles() {
    srand(time(NULL));
    Position* obstacles = (Position*)malloc(MAX_OBSTACLES * sizeof(Position));
    if (obstacles == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = SCREEN_WIDTH - 1; // 장애물을 화면 오른쪽 끝에서 시작
        obstacles[i].y = rand() % (SCREEN_HEIGHT / 3) + (2 * SCREEN_HEIGHT / 3); // 장애물의 y 좌표를 맵의 아래 셋째로 설정
    }
    return obstacles;
}

// 게임 루프
void gameLoop(int* score) {
    Position player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT }; // 플레이어의 초기 위치 (세로 크기만큼 높이에서 빼줍니다)
    Position* obstacles = initializeObstacles(); // 장애물 배열

    hideCursor(); // 커서 숨기기

    int jumpFlag = 0; // 점프 플래그 초기화
    int jumpVelocity = 0; // 플레이어의 수직 속도 초기화

    while (1) {
        clearScreen(); // 화면 지우기

        // 플레이어 그리기
        for (int i = 0; i < PLAYER_HEIGHT; i++) {
            drawCharacter(PLAYER_CHAR, player.x, player.y + i);
        }

        // 장애물 그리고 이동하기
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            drawCharacter(OBSTACLE_CHAR, obstacles[i].x, obstacles[i].y);
            obstacles[i].x--;

            // 장애물이 화면을 벗어나면 다시 화면 오른쪽에서 시작하고 높이를 랜덤으로 설정
            if (obstacles[i].x < 0) {
                obstacles[i].x = SCREEN_WIDTH - 1;
                obstacles[i].y = rand() % (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 2);
            }

            // 플레이어와 장애물이 스치면 게임 오버
            if (obstacles[i].x == player.x && obstacles[i].y >= player.y && obstacles[i].y <= player.y + PLAYER_HEIGHT - 1) {
                gameover(*score);
                free(obstacles); // 동적으로 할당된 메모리 해제
                return;
            }
        }

        // 키 입력 감지
        if (_kbhit()) {
            char key = _getch();
            if (key == ' ' && !jumpFlag) {
                jumpVelocity = JUMP_FORCE; // 플레이어의 점프 속도 설정
                jumpFlag = 3; // 점프 플래그 설정
            }
        }

        // 플레이어의 수직 이동
        player.y += jumpVelocity; // 플레이어의 y 좌표에 수직 속도를 더함
        jumpVelocity += GRAVITY; // 중력에 의해 점프 속도 감소

        // 플레이어가 화면 아래로 이동하는 경우
        if (player.y >= SCREEN_HEIGHT - PLAYER_HEIGHT) {
            player.y = SCREEN_HEIGHT - PLAYER_HEIGHT; // 플레이어가 화면 아래에 위치하도록 설정
            jumpVelocity = 0; // 플레이어가 땅에 닿으면 점프 속도 초기화
            jumpFlag = 0; // 점프 플래그 초기화
        }

        // 점수 증가
        (*score)++; // 점수 증가

        // 점수 표시
        gotoxy(SCREEN_WIDTH - 10, 0); // 화면 오른쪽 위에 점수 표시
        printf("Score: %d", *score);

        Sleep(30); // 일시 정지
    }
    free(obstacles); // 동적으로 할당된 메모리 해제
}

int main() {
    int score = 0; // 점수 변수 초기화
    printf("Press the spacebar to start the game!\n");
    while (_getch() != ' ') {} // 스페이스 바 입력 대기
    gameLoop(&score); // 게임 루프 시작 및 점수 전달
    return 0;
}
