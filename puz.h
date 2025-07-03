#ifndef PUZ_H
#define PUZ_H

static const char OOB = '.';
static const char BLANK = '-';

typedef enum Direction {
    ACROSS, 
    DOWN
} Direction;

typedef struct Coord {
    int x;
    int y;
} Coord;

typedef struct Clue {
    char identifier[10];
    char text[256];
    int answerLength;
    Coord start;
    Coord end;
    Direction direction;
} Clue;

typedef struct Crossword {
    int width;
    int height;
    int numClues;
    char* title;
    char* author;
    char* copyright;
    char** board;
    char** solution;
    char** clues;
} Crossword;

Crossword* createCrossword(const char* path);

Clue** createClueMap(const Crossword* cross);

Clue* createCluePointer();

void destroyCrossword(Crossword* cross);

void destroyClueMap(const Crossword* cross, Clue** clueMap);

int isBoundary(char c);

int isBlank(char c);

void printBoundary();

void printHighlight(char c);

#endif
