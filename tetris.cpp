#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int SCORE_PER_LINE = 10;

char board[BOARD_HEIGHT][BOARD_WIDTH] = {0};

struct Point {
    int x;
    int y;
};

struct Tetromino {
    Point blocks[4];
};

int score = 0;
int linesCompleted = 0;
int timeInSeconds = 0;
bool isPaused = false;

void initializeBoard() {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = ' ';
        }
    }
}

void drawBoard() {
    system("cls");

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            cout << board[i][j];
        }
        cout << endl;
    }

    cout << "Score: " << score << endl;
    cout << "Lines Completed: " << linesCompleted << endl;
    cout << "Time: " << timeInSeconds << "s" << endl;
    cout << "Press 'P' to pause/unpause" << endl;
}

bool isGameOver() {
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        if (board[0][i] != ' ')
            return true;
    }
    return false;
}

bool isValidPosition(const Tetromino& tetromino) {
    for (int i = 0; i < 4; ++i) {
        int x = tetromino.blocks[i].x;
        int y = tetromino.blocks[i].y;

        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
            return false;

        if (y >= 0 && board[y][x] != ' ')
            return false;
    }
    return true;
}

void placeTetromino(const Tetromino& tetromino) {
    for (int i = 0; i < 4; ++i) {
        int x = tetromino.blocks[i].x;
        int y = tetromino.blocks[i].y;

        board[y][x] = 'X';
    }
}

void removeCompletedRows() {
    int completedRows = 0;
    for (int i = BOARD_HEIGHT - 1; i >= 0; --i) {
        bool isRowComplete = true;
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == ' ') {
                isRowComplete = false;
                break;
            }
        }
        if (isRowComplete) {
            ++completedRows;
            for (int k = i; k > 0; --k) {
                for (int j = 0; j < BOARD_WIDTH; ++j) {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                board[0][j] = ' ';
            }
            ++linesCompleted;
        }
    }
    score += completedRows * SCORE_PER_LINE;
}

Tetromino generateRandomTetromino() {
    Tetromino tetromino;
    Point position;

    int shape = rand() % 7;

    position.x = BOARD_WIDTH / 2;
    position.y = 0;

    switch (shape) {
        case 0:  // I
            tetromino.blocks[0] = {position.x - 1, position.y};
            tetromino.blocks[1] = {position.x, position.y};
            tetromino.blocks[2] = {position.x + 1, position.y};
            tetromino.blocks[3] = {position.x + 2, position.y};
            break;
        case 1:  // J
            tetromino.blocks[0] = {position.x - 1, position.y};
            tetromino.blocks[1] = {position.x, position.y};
            tetromino.blocks[2] = {position.x + 1, position.y};
            tetromino.blocks[3] = {position.x + 1, position.y + 1};
            break;
        case 2:  // L
            tetromino.blocks[0] = {position.x - 1, position.y + 1};
            tetromino.blocks[1] = {position.x - 1, position.y};
            tetromino.blocks[2] = {position.x, position.y};
            tetromino.blocks[3] = {position.x + 1, position.y};
            break;
        case 3:  // O
            tetromino.blocks[0] = {position.x, position.y};
            tetromino.blocks[1] = {position.x + 1, position.y};
            tetromino.blocks[2] = {position.x, position.y + 1};
            tetromino.blocks[3] = {position.x + 1, position.y + 1};
            break;
        case 4:  // S
            tetromino.blocks[0] = {position.x - 1, position.y + 1};
            tetromino.blocks[1] = {position.x, position.y + 1};
            tetromino.blocks[2] = {position.x, position.y};
            tetromino.blocks[3] = {position.x + 1, position.y};
            break;
        case 5:  // T
            tetromino.blocks[0] = {position.x - 1, position.y};
            tetromino.blocks[1] = {position.x, position.y};
            tetromino.blocks[2] = {position.x + 1, position.y};
            tetromino.blocks[3] = {position.x, position.y + 1};
            break;
        case 6:  // Z
            tetromino.blocks[0] = {position.x - 1, position.y};
            tetromino.blocks[1] = {position.x, position.y};
            tetromino.blocks[2] = {position.x, position.y + 1};
            tetromino.blocks[3] = {position.x + 1, position.y + 1};
            break;
    }

    return tetromino;
}

void moveTetromino(Tetromino& tetromino, int dx, int dy) {
    for (int i = 0; i < 4; ++i) {
        tetromino.blocks[i].x += dx;
        tetromino.blocks[i].y += dy;
    }
}

void rotateTetromino(Tetromino& tetromino) {
    Point pivot = tetromino.blocks[1];

    for (int i = 0; i < 4; ++i) {
        int x = tetromino.blocks[i].x;
        int y = tetromino.blocks[i].y;

        int newX = pivot.x + (pivot.y - y);
        int newY = pivot.y - (pivot.x - x);

        tetromino.blocks[i].x = newX;
        tetromino.blocks[i].y = newY;
    }
}

void handleInput(char input, Tetromino& tetromino) {
    switch (input) {
        //case 'A':
        case 'a':
        {
            if (isValidPosition(tetromino))
                moveTetromino(tetromino, -1, 0);
            break;
        }
        //case 'D':
        case 'd':
        {
            if (isValidPosition(tetromino))
                moveTetromino(tetromino, 1, 0);
            break;
        }
        //case 'S':
        case 's':
        {
            if (isValidPosition(tetromino))
                moveTetromino(tetromino, 0, 1);
            break;
        }
        //case 'W':
        case 'w':
        {
            Tetromino rotatedTetromino = tetromino;
            rotateTetromino(rotatedTetromino);
            if (isValidPosition(rotatedTetromino))
                tetromino = rotatedTetromino;
            break;
        }
        //case 'P':
        case 'p':
        {
            isPaused = !isPaused;
            break;
        }
    }
}

int main() {
    srand(time(NULL));

    initializeBoard();

    Tetromino currentTetromino = generateRandomTetromino();

    time_t startTime = time(NULL);

    while (!isGameOver()) {
        if (!isPaused) {
            time_t currentTime = time(NULL);
            timeInSeconds = static_cast<int>(currentTime - startTime);

            if (_kbhit()) {
                char input = _getch();
                handleInput(input, currentTetromino);
            }

            if (isValidPosition(currentTetromino)) {
                moveTetromino(currentTetromino, 0, 1);
            } else {
                placeTetromino(currentTetromino);
                removeCompletedRows();

                currentTetromino = generateRandomTetromino();

                if (!isValidPosition(currentTetromino)) {
                    break;  // Game over
                }
            }

            drawBoard();
        }
    }

    drawBoard();
    cout << "Game Over!" << endl;

    return 0;
}