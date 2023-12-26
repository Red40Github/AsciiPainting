#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack {
	int amountOfElements;
	int topIndex;
	size_t elementSize;
	void* array;
} Stack;

Stack createStack(int amountOfElements, size_t elementSize);
bool pushToStack(Stack* stack, void* inputPointer);
void* popOffStack(Stack*);
void* seekTopOfStack(Stack* stack);
void cleanStack(Stack* stack);
void emptyStack(Stack* stack);
bool isEmpty(Stack* stack);
bool isFull(Stack* stack);
