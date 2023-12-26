#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stack.h>

Stack createStack(int amountOfElements, size_t elementSize) {
	Stack newStack;
	newStack.amountOfElements = amountOfElements;
	newStack.elementSize = elementSize;
	newStack.topIndex = -1;
	newStack.array = malloc(newStack.elementSize * amountOfElements);
	return newStack;
}

bool pushToStack(Stack* stack, void* inputPointer) {
	if (!(isFull(stack))) {
		stack->topIndex++;
		void* newTopPointer = (char *) (stack->array + (stack->topIndex * stack->elementSize));
		memcpy(newTopPointer, inputPointer, stack->elementSize);
		return true;
	} else {
		return false;
	}
}

void* popOffStack(Stack* stack) {
	if (!(isEmpty(stack))) {
		/*
		 * For security reasons, this returns a copy of the top value,
		 * not the pointer to the top of the stack.
		*/
        void *copiedValuePointer = malloc(stack->elementSize);
        void* topPointer = (char *) (stack->array + (stack->topIndex * stack->elementSize));
        memcpy(copiedValuePointer, topPointer, stack->elementSize);
        stack->topIndex--;
		return copiedValuePointer;
	} else {
		return NULL;
	}
}

void* seekTopOfStack(Stack* stack) {
	if (!(isEmpty(stack))) {
        void *copiedValue = malloc(stack->elementSize);
        void* topPointer = (char *) (stack->array + (stack->topIndex * stack->elementSize));
        memcpy(copiedValue, topPointer, stack->elementSize);
		return copiedValue;
	} else {
		return NULL;
	}
}

void cleanStack(Stack* stack) {
    free(stack->array);
    stack->array = NULL;
    stack->elementSize = 0;
    stack->amountOfElements = 0;
    stack->topIndex = -1;
}

void emptyStack(Stack* stack) {
    free(stack->array);
    stack->array = malloc(stack->amountOfElements * stack->elementSize);
    stack->topIndex = -1;
}

bool isEmpty(Stack* stack) {
	return (stack->topIndex == -1) ? true : false;
}

bool isFull(Stack* stack) {
	return (stack->topIndex < stack->amountOfElements) ? false : true;
}
