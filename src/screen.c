#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <screen.h>

Screen createScreen(int screenWidth, int screenHeight, char emptySpace) {
    emptySpace = (emptySpace == 0 ) ? emptySpace : EMPTY_SPACE;
    screenWidth = (screenWidth > 1) ? screenWidth : DEFAULT_SCREEN_WIDTH;
    screenHeight = (screenHeight > 1) ? screenHeight : DEFAULT_SCREEN_HEIGHT;

    char* newBuffer =
        (char*)
        malloc(((screenHeight * (screenWidth + 1)) + 1) * sizeof(char));

    int index = 0;

    for (int y=0; y < screenHeight; y++) {
        for (int x=0; x < screenWidth; x++) {
            newBuffer[index] = emptySpace;
            index++;
        }
        newBuffer[index] = '\n';
        index++;
    }

    newBuffer[screenHeight * (screenWidth + 1)] = 0;

    Screen newScreen;
    newScreen.width = screenWidth;
    newScreen.height = screenHeight;
    newScreen.buffer = newBuffer;

    return newScreen;
}

void clearScreen(Screen screen, char emptySpace) {
    emptySpace = (emptySpace != 0) ? emptySpace : EMPTY_SPACE;
    int index = 0;
    for (char y=0; y < screen.height; y++) {
        for (char x=0; x < screen.width; x++) {
            screen.buffer[index] = emptySpace;
            index++;
        }
        index++;
    }
}
void drawScreen(Screen screen) {
    system("cls");
    printf("%s", screen.buffer);
}


void drawChar(Screen screen, int x, int y, char character) {
    if (x > screen.width-1
        || x < 0
        || y > screen.height-1
        || y < 0
        || character == 0
        ) {
        return;
        }
    screen.buffer[(y * (screen.width + 1)) + x] = character;
}
char getChar(Screen screen, int x, int y) {
    if (x > screen.width - 1
        || x < 0
        || y > screen.height - 1
        || y < 0
        ) {
        return 0;
        }
    return screen.buffer[(y * (screen.width + 1)) + x];
}

void writeScreenToFile(Screen screen, FILE* textFile) {
    /*int index = 0;
    for (char y=0; y < screen.height; y++) {
        for (char x=0; x < screen.width; x++) {
            fputc(screen.buffer[index]);
            index++;
        }
        index++;
    }*/
    fputs(screen.buffer, textFile);
}
