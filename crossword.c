#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "puz.h"

static Crossword* cross = NULL;
static Clue** clueMap = NULL;

int isInBounds(int row, int col) {
    if (row < 0 || row >= cross->height || col < 0 || col >= cross->width) {
        return 0;
    }
    return 1;
}

void findClues() {
    int clueNum = 1;
    int index = 0;

    for (int r = 0; r < cross->height; r++) {
        for (int c = 0; c < cross->width; c++) {
            if (isBoundary(cross->board[r][c])) {
                continue;
            }

            int isStartAcross = (c == 0 || isBoundary(cross->board[r][c - 1])) && (c + 1 < cross->width && !isBoundary(cross->board[r][c + 1]));
            int isStartDown = (r == 0 || isBoundary(cross->board[r - 1][c])) && (r + 1 < cross->height && !isBoundary(cross->board[r + 1][c]));

            if (isStartAcross || isStartDown) {
                if (isStartAcross) {
                    Clue* clue = createCluePointer();
                    clue->start.x = r;
                    clue->start.y = c;
                    clue->direction = ACROSS;
                    clue->answerLength = 1;
                    while (c + clue->answerLength < cross->width && !isBoundary(cross->board[r][c + clue->answerLength])) {
                        clue->answerLength++;
                    }
                    clue->end.x = r;
                    clue->end.y = c + clue->answerLength - 1;
                    sprintf(clue->identifier, "%d Across", clueNum);
                    strcpy(clue->text, cross->clues[index]);
                    clueMap[index++] = clue;
                }

                if (isStartDown) {
                    Clue* clue = createCluePointer();
                    clue->start.x = r;
                    clue->start.y = c;
                    clue->direction = DOWN;
                    clue->answerLength = 1;
                    while (r + clue->answerLength < cross->height && !isBoundary(cross->board[r + clue->answerLength][c])) {
                        clue->answerLength++;
                    }
                    clue->end.x = r + clue->answerLength - 1;
                    clue->end.y = c;
                    sprintf(clue->identifier, "%d Down", clueNum);
                    strcpy(clue->text, cross->clues[index]);
                    clueMap[index++] = clue;
                }
                clueNum++;
            }
        }
    }
}

void printClues() {
    for (int i = 0; i < cross->numClues; i++) {
        Clue* clue = clueMap[i];
        printf("%s at (%d,%d):\n%s\n\n", clue->identifier, clue->start.x, clue->start.y, clue->text);
    }
}

void printNumbers() {
    int clueNum = 1;
    int numBoard[cross->height][cross->width];
    memset(numBoard, 0, sizeof(numBoard));

    for (int r = 0; r < cross->height; r++) {
        for (int c = 0; c < cross->width; c++) {
            if (isBoundary(cross->board[r][c])) continue;
            int isStartAcross = (c == 0 || isBoundary(cross->board[r][c - 1])) && (c + 1 < cross->width && !isBoundary(cross->board[r][c + 1]));
            int isStartDown = (r == 0 || isBoundary(cross->board[r - 1][c])) && (r + 1 < cross->height && !isBoundary(cross->board[r + 1][c]));
            if (isStartAcross || isStartDown) {
                numBoard[r][c] = clueNum++;
            }
        }
    }

    for (int r = 0; r < cross->height; r++) {
        for (int c = 0; c < cross->width; c++) {
            if (isBoundary(cross->board[r][c])) {
                printBoundary();
            } else if (numBoard[r][c] != 0) {
                printf("%2d", numBoard[r][c]);
            } else {
                printf(" .");
            }
        }
        printf("\n");
    }
}

void printBoard(Coord start, Coord end) {
    for (int r = 0; r < cross->height; r++) {
        for (int c = 0; c < cross->width; c++) {
            if (isBoundary(cross->board[r][c])) {
                printBoundary();
            } else if ((r >= start.x && r <= end.x && c == start.y) || (c >= start.y && c <= end.y && r == start.x)) {
                printHighlight(cross->board[r][c]);
            } else {
                printf("%c", cross->board[r][c]);
            }
        }
        printf("\n");
    }
}

int isSolved() {
    for (int r = 0; r < cross->height; r++) {
        for (int c = 0; c < cross->width; c++) {
            if (cross->solution[r][c] != cross->board[r][c]) {
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: ./crossword puzzle.puz\n");
        return 1;
    }

    cross = createCrossword(argv[1]);
    clueMap = createClueMap(cross);
    findClues();

    printf("Title: %s\n", cross->title);
    printf("Author: %s\n", cross->author);
    printf("Copyright: %s\n", cross->copyright);

    char command[100];
    while (!isSolved()) {
        Coord empty = { -1, -1 };
        printBoard(empty, empty);
        printf("> ");
        scanf("%s", command);

        if (strcmp(command, "clues") == 0) {
            printClues();
        } else if (strcmp(command, "numbers") == 0) {
            printNumbers();
        } else if (strcmp(command, "solve") == 0) {
            int num;
            char dir[10];
            scanf("%d %s", &num, dir);
            Direction d = (strcmp(dir, "Across") == 0) ? ACROSS : DOWN;
            for (int i = 0; i < cross->numClues; i++) {
                int n;
                sscanf(clueMap[i]->identifier, "%d", &n);
                if (n == num && clueMap[i]->direction == d) {
                    printf("%s\n", clueMap[i]->text);
                    char input[256];
                    do {
                        printf("Your answer: ");
                        scanf("%s", input);
                    } while ((int)strlen(input) != clueMap[i]->answerLength);

                    for (int j = 0; j < clueMap[i]->answerLength; j++) {
                        int r = clueMap[i]->start.x + (d == DOWN ? j : 0);
                        int c = clueMap[i]->start.y + (d == ACROSS ? j : 0);
                        cross->board[r][c] = input[j];
                    }
                }
            }
        } else if (strcmp(command, "reveal") == 0) {
            int num;
            char dir[10];
            scanf("%d %s", &num, dir);
            Direction d = (strcmp(dir, "Across") == 0) ? ACROSS : DOWN;
            for (int i = 0; i < cross->numClues; i++) {
                int n;
                sscanf(clueMap[i]->identifier, "%d", &n);
                if (n == num && clueMap[i]->direction == d) {
                    for (int j = 0; j < clueMap[i]->answerLength; j++) {
                        int r = clueMap[i]->start.x + (d == DOWN ? j : 0);
                        int c = clueMap[i]->start.y + (d == ACROSS ? j : 0);
                        cross->board[r][c] = cross->solution[r][c];
                    }
                }
            }
        } else if (strcmp(command, "quit") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    if (isSolved()) {
        printf("You solved it!\n");
    }

    destroyClueMap(cross, clueMap);
    destroyCrossword(cross);

    return 0;
}
