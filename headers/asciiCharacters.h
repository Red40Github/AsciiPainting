#ifndef ASCIICHARACTERS_H_INCLUDED
#define ASCIICHARACTERS_H_INCLUDED

#define ASCII_ESCAPE 27
#define ASCII_NEWLINE 13
#define ASCII_BACKSPACE 8
#define ASCII_SPACE 32
#define ASCII_BLOCK 219
//On Windows, _getch (from conio.h) must be called twice
//to handle an arrow key. The first return value is 224,
//the second is 'A' , 'B', etc. as above. – yoyo, Jul 8, 2020 at 18:03
//https://stackoverflow.com/questions/10463201/getch-and-arrow-codes#comment111057052_11432632
//https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v=vs.60)
#define EXTENDED_ASCII_KEY 224
#define WINDOWS_ASCII_ARROW_LEFT 75
#define WINDOWS_ASCII_ARROW_RIGHT 77
#define WINDOWS_ASCII_ARROW_UP 72
#define WINDOWS_ASCII_ARROW_DOWN 80
#define WINDOWS_ASCII_CTRL_Z 26
#define WINDOWS_ASCII_CTRL_Y 25

#endif // ASCIICHARACTERS_H_INCLUDED
