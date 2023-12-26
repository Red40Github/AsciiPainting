#include <stdbool.h>
#include <screen.h>

#ifndef POLLFORCOMMANDMODE_H_INCLUDED
#define POLLFORCOMMANDMODE_H_INCLUDED

void pollForCommandMode(Screen* screen, int* x, int* y, bool* isLooping, char characterUnderCursor);
bool pollForYesOrNo();
void pollForQuit(Screen* screen, bool* isLooping);
void pollForNewScreen(int* x, int* y, Screen* screen);
void pollForDeletionScreen(int* x, int* y, Screen* screen);
void waitForKeyPressToContinue();
void askForScreenInformation(int* width, int* height);

#endif // POLLFORCOMMANDMODE_H_INCLUDED
