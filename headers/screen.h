#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#define DEFAULT_SCREEN_HEIGHT 30
#define DEFAULT_SCREEN_WIDTH 40

#define EMPTY_SPACE ' '
#define BORDER_CHARACTER 178

typedef struct Screen {
    int width;
    int height;
    char* buffer;
} Screen;

Screen createScreen(int screenWidth, int screenHeight, char emptySpace);

void clearScreen(Screen screen, char emptySpace);
void drawScreen(Screen screen);

void drawChar(Screen screen, int x, int y, char character);
char getChar(Screen screen, int x, int y);

void writeScreenToFile(Screen screen, FILE* textFile);

#endif
