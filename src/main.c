#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <asciiCharacters.h>
#include <screen.h>
#include <stack.h>
#include <polls.h>

#define HEADER_PROGRAM_STRING "\
@@@@@@@@@@@@@@@@@@@@@@@@\n\
@                      @\n\
@ ASCII Painting! v1.0 @\n\
@                      @\n\
@@@@@@@@@@@@@@@@@@@@@@@@\n\
\n\
> Enter for changing into command mode,\n\
> Arrow keys to control the cursor!\n\
\n"

#define HEADER_COMMAND_MODE_STRING "\
[Command Mode]\n\
> 'q' to quit the program,\n\
> 'd' to delete current painting,\n\
> 'n' to create a new painting,\n\
> 's' to save the current painting,\n\
> 'o' to open a text file,\n\
> Press enter again to exit out of command mode.\n\
\n"


// Struct for storing an action (drawing, erasing, etc)
typedef struct ActionMoment {
    int x;
    int y;
    char character;
} ActionMoment;

Stack UndoHistory;
Stack RedoHistory;
void waitForKeyPressToContinue();
void askForScreenInformation(int* width, int* height);
void paintingLoop();
bool pushNewActionToUndoHistory(int x, int y, char character);
bool pushNewActionToRedoHistory(int x, int y, char character);
void switchCaseFunctionKeys(Screen screen, int* x, int* y, char* characterUnderCursor);
bool pollForYesOrNo();
void pollForCommandMode(Screen* screen, int* x, int* y, bool* isLooping, char characterUnderCursor);
void pollForQuit(Screen* screen, bool* isLooping);
void pollForNewScreen(int* x, int* y, Screen* screen);
void pollForDeletionScreen(int* x, int* y, Screen* screen);

int main()
{
    UndoHistory = createStack(4096, sizeof(ActionMoment));
    RedoHistory = createStack(2048, sizeof(ActionMoment));

    printf(HEADER_PROGRAM_STRING);

    waitForKeyPressToContinue();

    paintingLoop();

    return 0;
}

void paintingLoop() {
    int x = 0, y = 0;
    bool isLooping = true;
    Screen screen = createScreen(40, 30, EMPTY_SPACE);

    char characterUnderCursor = getChar(screen, x, y);

    system("cls");
    printf(HEADER_COMMAND_MODE_STRING);
    pollForCommandMode(
        &screen,
        &x,
        &y,
        &isLooping,
        characterUnderCursor
    );

    if (isLooping) {
        drawChar(screen, x, y, ASCII_BLOCK);
        drawScreen(screen);
        printf("[%d, %d]\n", x, y);
    }

    while (isLooping) {
        if (_kbhit() != 0) {
            unsigned char asciiCharacter = _getch();
            switch (asciiCharacter) {
                case ASCII_NEWLINE:;
                    system("cls");
                    printf(HEADER_COMMAND_MODE_STRING);
                    pollForCommandMode(
                        &screen,
                        &x,
                        &y,
                        &isLooping,
                        characterUnderCursor
                    );
                    break;
                case EXTENDED_ASCII_KEY:;
                    switchCaseFunctionKeys(screen, &x, &y, &characterUnderCursor);
                    break;
                case ASCII_BACKSPACE:;
                    characterUnderCursor = EMPTY_SPACE;
                    drawChar(screen, x, y, EMPTY_SPACE);
                    pushNewActionToUndoHistory(x, y, EMPTY_SPACE);
                    break;
                case WINDOWS_ASCII_CTRL_Y:;
                    ActionMoment* action = popOffStack(&RedoHistory);
                    if (action != NULL) {
                        if (
                            action->x == x
                            && action->y == y
                            ) {
                            characterUnderCursor = action->character;
                        }
                        pushNewActionToUndoHistory(action->x, action->y, action->character);
                        drawChar(screen, action->x, action->y, action->character);
                    }
                    break;
                case WINDOWS_ASCII_CTRL_Z:;
                    ActionMoment* mostRecentAction = (ActionMoment*) popOffStack(&UndoHistory);
                    if (mostRecentAction != NULL) {
                        if (
                            mostRecentAction->x == x
                            && mostRecentAction->y == y
                            ) {
                            characterUnderCursor = EMPTY_SPACE;
                        }
                        pushNewActionToRedoHistory(mostRecentAction->x, mostRecentAction->y, mostRecentAction->character);
                        ActionMoment* secondMostRecentAction = (ActionMoment*) seekTopOfStack(&UndoHistory);
                        drawChar(screen, mostRecentAction->x, mostRecentAction->y, EMPTY_SPACE);
                        if (secondMostRecentAction != NULL) {
                            drawChar(screen, secondMostRecentAction->x, secondMostRecentAction->y, secondMostRecentAction->character);
                            if (
                                secondMostRecentAction->x == x
                                && secondMostRecentAction->y == y
                                ) {
                                characterUnderCursor = secondMostRecentAction->character;
                            }
                        }
                    }
                    break;
                default:;
                    if (asciiCharacter > 32 && asciiCharacter < 127) {
                        if (pushNewActionToUndoHistory(x, y, asciiCharacter)) {
                            if (!(isEmpty(&RedoHistory))) {
                                emptyStack(&RedoHistory);
                            }
                            drawChar(screen, x, y, asciiCharacter);
                            characterUnderCursor = asciiCharacter;
                        } else {
                            system("cls");
                            printf("Error! Action history is full! You cannot input anymore!\n");
                            printf("You can however either undo a bunch of actions OR \n");
                            printf("save the current painting and then open it again! :)\n");
                            waitForKeyPressToContinue();
                        }
                    }
                    break;
                }
            drawScreen(screen);
            printf("[%d, %d]\n", x, y);
            }
        }
    system("cls");
    printf("Thanks for using the program! :)");
}

bool pushNewActionToUndoHistory(int x, int y, char character) {
    if (!(isFull(&UndoHistory))) {
        ActionMoment newAction;
        newAction.x = x, newAction.y = y;
        newAction.character = character;
        pushToStack(&UndoHistory, &newAction);
        return true;
    } else {
        return false;
    }
}

bool pushNewActionToRedoHistory(int x, int y, char character) {
    if (!(isFull(&RedoHistory))) {
        ActionMoment newAction;
        newAction.x = x, newAction.y = y;
        newAction.character = character;
        pushToStack(&RedoHistory, &newAction);
        return true;
    } else {
        return false;
    }
}

void switchCaseFunctionKeys(Screen screen, int* x, int* y, char* characterUnderCursor) {
    switch (_getch()) {
        case WINDOWS_ASCII_ARROW_LEFT:
            drawChar(screen, *x, *y, *characterUnderCursor);
            *x -= 1;
            *characterUnderCursor = getChar(screen, *x, *y);
            drawChar(screen, *x, *y, ASCII_BLOCK);
            break;
        case WINDOWS_ASCII_ARROW_RIGHT:
            drawChar(screen, *x, *y, *characterUnderCursor);
            *x += 1;
            *characterUnderCursor = getChar(screen, *x, *y);
            drawChar(screen, *x, *y, ASCII_BLOCK);
            break;
        case WINDOWS_ASCII_ARROW_UP:
            drawChar(screen, *x, *y, *characterUnderCursor);
            *y -= 1;
            *characterUnderCursor = getChar(screen, *x, *y);
            drawChar(screen, *x, *y, ASCII_BLOCK);
            break;
        case WINDOWS_ASCII_ARROW_DOWN:
            drawChar(screen, *x, *y, *characterUnderCursor);
            *y += 1;
            *characterUnderCursor = getChar(screen, *x, *y);
            drawChar(screen, *x, *y, ASCII_BLOCK);
            break;
    }
}
