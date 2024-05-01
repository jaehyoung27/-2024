#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 20
#define PLAYER_CHAR 'O' // �÷��̾��� ���
#define OBSTACLE_CHAR '@' // ��ֹ��� ���
#define MAX_OBSTACLES 2 // �ִ� ��ֹ� ����
#define GRAVITY 1       // �÷��̾��� �߷�
#define JUMP_FORCE -3   // ���� ����
#define PLAYER_HEIGHT 2 // �÷��̾��� ���� ũ��

typedef struct {
    int x;
    int y;
} Position; // ��ǥ�� ��Ÿ���� ����ü

// Ŀ���� Ư�� ��ġ�� �̵��ϴ� �Լ�
void gotoxy(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Ŀ���� ����� �Լ�
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 1, 0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// ���ڸ� Ư�� ��ġ�� �׸��� �Լ�
void drawCharacter(char character, int x, int y) {
    gotoxy(x, y);
    printf("%c", character);
}

// ȭ���� ����� �Լ�
void clearScreen() {
    system("cls");
}

// ���� ������ ����ϴ� �Լ�
void gameover(int score) {
    clearScreen();
    gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2 - 1);
    printf("Game Over!");
    gotoxy(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    printf("Your score: %d", score);
}

// ��ֹ��� �ʱ�ȭ�ϴ� �Լ�
Position* initializeObstacles() {
    srand(time(NULL));
    Position* obstacles = (Position*)malloc(MAX_OBSTACLES * sizeof(Position));
    if (obstacles == NULL) {
        printf("Memory allocation failed");
        exit(1);
    }
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = SCREEN_WIDTH - 1; // ��ֹ��� ȭ�� ������ ������ ����
        obstacles[i].y = rand() % (SCREEN_HEIGHT / 3) + (2 * SCREEN_HEIGHT / 3); // ��ֹ��� y ��ǥ�� ���� �Ʒ� ��°�� ����
    }
    return obstacles;
}

// ���� ����
void gameLoop(int* score) {
    Position player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT }; // �÷��̾��� �ʱ� ��ġ (���� ũ�⸸ŭ ���̿��� ���ݴϴ�)
    Position* obstacles = initializeObstacles(); // ��ֹ� �迭

    hideCursor(); // Ŀ�� �����

    int jumpFlag = 0; // ���� �÷��� �ʱ�ȭ
    int jumpVelocity = 0; // �÷��̾��� ���� �ӵ� �ʱ�ȭ

    while (1) {
        clearScreen(); // ȭ�� �����

        // �÷��̾� �׸���
        for (int i = 0; i < PLAYER_HEIGHT; i++) {
            drawCharacter(PLAYER_CHAR, player.x, player.y + i);
        }

        // ��ֹ� �׸��� �̵��ϱ�
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            drawCharacter(OBSTACLE_CHAR, obstacles[i].x, obstacles[i].y);
            obstacles[i].x--;

            // ��ֹ��� ȭ���� ����� �ٽ� ȭ�� �����ʿ��� �����ϰ� ���̸� �������� ����
            if (obstacles[i].x < 0) {
                obstacles[i].x = SCREEN_WIDTH - 1;
                obstacles[i].y = rand() % (SCREEN_HEIGHT / 2) + (SCREEN_HEIGHT / 2);
            }

            // �÷��̾�� ��ֹ��� ��ġ�� ���� ����
            if (obstacles[i].x == player.x && obstacles[i].y >= player.y && obstacles[i].y <= player.y + PLAYER_HEIGHT - 1) {
                gameover(*score);
                free(obstacles); // �������� �Ҵ�� �޸� ����
                return;
            }
        }

        // Ű �Է� ����
        if (_kbhit()) {
            char key = _getch();
            if (key == ' ' && !jumpFlag) {
                jumpVelocity = JUMP_FORCE; // �÷��̾��� ���� �ӵ� ����
                jumpFlag = 3; // ���� �÷��� ����
            }
        }

        // �÷��̾��� ���� �̵�
        player.y += jumpVelocity; // �÷��̾��� y ��ǥ�� ���� �ӵ��� ����
        jumpVelocity += GRAVITY; // �߷¿� ���� ���� �ӵ� ����

        // �÷��̾ ȭ�� �Ʒ��� �̵��ϴ� ���
        if (player.y >= SCREEN_HEIGHT - PLAYER_HEIGHT) {
            player.y = SCREEN_HEIGHT - PLAYER_HEIGHT; // �÷��̾ ȭ�� �Ʒ��� ��ġ�ϵ��� ����
            jumpVelocity = 0; // �÷��̾ ���� ������ ���� �ӵ� �ʱ�ȭ
            jumpFlag = 0; // ���� �÷��� �ʱ�ȭ
        }

        // ���� ����
        (*score)++; // ���� ����

        // ���� ǥ��
        gotoxy(SCREEN_WIDTH - 10, 0); // ȭ�� ������ ���� ���� ǥ��
        printf("Score: %d", *score);

        Sleep(30); // �Ͻ� ����
    }
    free(obstacles); // �������� �Ҵ�� �޸� ����
}

int main() {
    int score = 0; // ���� ���� �ʱ�ȭ
    printf("Press the spacebar to start the game!\n");
    while (_getch() != ' ') {} // �����̽� �� �Է� ���
    gameLoop(&score); // ���� ���� ���� �� ���� ����
    return 0;
}
