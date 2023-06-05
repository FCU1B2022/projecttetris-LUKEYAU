#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 10
#define PLAYER_HEIGHT 3

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Player {
    int x;
    int y;
};

struct Ball {
    int x;
    int y;
    enum Direction vertical_direction;
    enum Direction horizontal_direction;
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawMap() {
    system("cls");
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (j == 0 || j == MAP_WIDTH - 1) {
                printf("\033[33m");
                printf("|");
            }
            else 
                printf(" ");
        }
        printf("\n");
    }
}

void drawPlayer(struct Player* player) {
    for (int i = 0; i < PLAYER_HEIGHT; i++) {
        gotoxy(player->x, player->y + i);
        printf("\033[31m");
        printf("=");
    }
}

void erasePlayer(struct Player* player) {
    for (int i = 0; i < PLAYER_HEIGHT; i++) {
        gotoxy(player->x, player->y + i);
        printf(" ");
    }
}

void movePlayer(struct Player* player, enum Direction direction) {
    erasePlayer(player);

    if (direction == UP && player->y > 0) 
        player->y--;
    else if (direction == DOWN && player->y < MAP_HEIGHT - PLAYER_HEIGHT) 
        player->y++;

    drawPlayer(player);
}

void drawBall(struct Ball* ball) {
    gotoxy(ball->x, ball->y);
    printf("\033[34m");
    printf("O");
}

void eraseBall(struct Ball* ball) {
    gotoxy(ball->x, ball->y);
    printf(" ");
}

void moveBall(struct Ball* ball) {
    eraseBall(ball);

    if (ball->vertical_direction == UP && ball->y == 0) 
        ball->vertical_direction = DOWN;
    else if (ball->vertical_direction == DOWN && ball->y == MAP_HEIGHT - 1) 
        ball->vertical_direction = UP;
    else {
        if (ball->vertical_direction == UP) 
            ball->y--;
        else if (ball->vertical_direction == DOWN) 
            ball->y++;
    }

    if (ball->horizontal_direction == LEFT && ball->x == 0) 
        ball->horizontal_direction = RIGHT;
    else if (ball->horizontal_direction == RIGHT && ball->x == MAP_WIDTH - 1) 
        ball->horizontal_direction = LEFT;
    else {
        if (ball->horizontal_direction == LEFT) 
            ball->x--;
        else if (ball->horizontal_direction == RIGHT) 
            ball->x++;
    }

    drawBall(ball);
}

int checkCollision(struct Ball* ball, struct Player* player) {
    if (ball->x == player->x + 1) {
        if (ball->y >= player->y && ball->y <= player->y + PLAYER_HEIGHT - 1) {
            ball->horizontal_direction =
                (ball->horizontal_direction == LEFT) ? RIGHT : LEFT;
            return 1;
        }
    }
    return 0;
}
int checkCollision2(struct Ball* ball, struct Player* player) {
    if (ball->x == player->x - 1) {
        if (ball->y >= player->y && ball->y <= player->y + PLAYER_HEIGHT - 1) {
            ball->horizontal_direction =
                (ball->horizontal_direction == LEFT) ? RIGHT : LEFT;
            return 1;
        }
    }
    return 0;
}

void showIntroScreen() {
    system("cls");
    printf("Welcome to the Pong Game!\n");
    printf("Player 1 controls: 'w' to move up, 's' to move down\n");
    printf("Player 2 controls: 'i' to move up, 'k' to move down\n");
    printf("ESC to Leave the game \nPress space to start the game...\n");

    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == ' ')
                break;
        }
    }
}

int main() {
    struct Player player1 = { 1, 3 };
    struct Player player2 = { MAP_WIDTH - 2, 3 };
    struct Ball ball = { MAP_WIDTH / 2, MAP_HEIGHT / 2, UP, LEFT };

    showIntroScreen();

    drawMap();
    drawPlayer(&player1);
    drawPlayer(&player2);
    drawBall(&ball);

    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'w')
                movePlayer(&player1, UP);
            else if (key == 's')
                movePlayer(&player1, DOWN);
            else if (key == 'i')
                movePlayer(&player2, UP);
            else if (key == 'k')
                movePlayer(&player2, DOWN);
        }

        moveBall(&ball);

        if (ball.x == 0 || ball.x == MAP_WIDTH - 1) {
            printf("\033[37m\nGame Over\n");
            break;
        }
        if (checkCollision(&ball, &player1) || checkCollision2(&ball, &player2))
            continue;
        Sleep(200);
    }

    return 0;
}
