#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <asciiCharacters.h>
#include <screen.h>
#include <polls.h>

bool pollForYesOrNo() {
    while (true) {
        if (_kbhit() != 0) {
            char decisionCharacter = _getch();
            switch (decisionCharacter) {
            case 'n':
                return false;
            case 'y':
                return true;
            }
        }
    }
}

void pollForCommandMode(Screen* screen, int* x, int* y, bool* isLooping, char characterUnderCursor) {
    bool isPollingForCommands = true;
    while (isPollingForCommands) {
        if (_kbhit() != 0) {
            unsigned char commandCharacter = _getch();
            switch (commandCharacter) {
            case ASCII_NEWLINE:
                isPollingForCommands = false;
                break;
            case 'q':
                printf("Quit the program (unsaved paintings will be gone forever!)? [y/n]");
                pollForQuit(screen, isLooping);
                isPollingForCommands = false;
                break;
            case 'd':
                printf("Delete current painting? [y/n]");
                pollForDeletionScreen(x, y, screen);
                isPollingForCommands = false;
                break;
            case 'n':;
                printf("Delete current painting and make a new one? [y/n]");
                pollForNewScreen(x, y, screen);
                isPollingForCommands = false;
                break;
            case 's':
                printf("Write your file's name.. : ");
                char fileNameForSave[64];
                scanf("%s", fileNameForSave);

                FILE* newFile = fopen(fileNameForSave, "w");
                drawChar(*screen, *x, *y, characterUnderCursor);
                writeScreenToFile(*screen, newFile);
                fclose(newFile);

                isPollingForCommands = false;
                break;
            case 'o':
                printf("Write the file's name.. : ");

                char fileNameForOpen[64];
                scanf("%s", fileNameForOpen);
                FILE* file = fopen(fileNameForOpen, "r");

                if (file == NULL) {
                    printf("The file doesn't exist! :(\n");
                    waitForKeyPressToContinue();
                    isPollingForCommands = false;
                } else {
                    printf("Are you sure you want to open a file?\n");
                    printf("(unsaved paintings will be deleted! D:) [y/n]\n");
                    if (pollForYesOrNo()) {
                        free((*screen).buffer);

                        char buffer[1024];
                        fgets(buffer, 1024, file);
                        int rowSize = ((int) strlen(buffer)) - 1;

                        fseek(file, 0, SEEK_SET);
                        int lineAmount = 0;
                        char currentCharacter;
                        for (currentCharacter = fgetc(file); currentCharacter != EOF; currentCharacter = fgetc(file)) {
                            if (currentCharacter == '\n') {
                                lineAmount++;
                            }
                        }
                        *screen = createScreen(rowSize, lineAmount, EMPTY_SPACE);
                        fseek(file, 0, SEEK_SET);
                        for (int rowIndex=0; rowIndex < screen->height; rowIndex++) {
                            for (int columnIndex=0; columnIndex < screen->width + 1; columnIndex++) {
                                drawChar(
                                    *screen,
                                    columnIndex,
                                    rowIndex,
                                    (!feof(file)) ? fgetc(file) : EMPTY_SPACE
                                );
                            }
                            //fseek(file, 1, SEEK_CUR);
                        }
                    }
                }
                fclose(file);
                isPollingForCommands = false;
                break;
            }
        }
    }
}

void pollForQuit(Screen* screen, bool* isLooping) {
    if (pollForYesOrNo()) {
        free((*screen).buffer);
        *isLooping = false;
    }
}

void pollForNewScreen(int* x, int* y, Screen* screen) {
    if (pollForYesOrNo()) {
        system("cls");
        free(screen->buffer);
        *x = 0, *y = 0;

        int newWidth, newHeight;
        askForScreenInformation(&newWidth, &newHeight);
        *screen = createScreen(newWidth, newHeight, ' ');
    }
}

void pollForDeletionScreen(int* x, int* y, Screen* screen) {
    if (pollForYesOrNo()) {
        x = 0, y = 0;
        clearScreen(*screen, ' ');
    }
}

void waitForKeyPressToContinue() {
    printf("Press any key to continue..\n");
    while (_kbhit() == 0);
    _getch();
}

void askForScreenInformation(int* width, int* height) {
    printf("Enter the width of your painting. : ");
    scanf("%d", width);
    printf("Enter the height of your painting. : ");
    scanf("%d", height);
}
