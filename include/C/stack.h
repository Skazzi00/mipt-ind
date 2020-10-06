#pragma once

#include <stdlib.h>

#define STACK_HASH_CHECK
#define STACK_MARKER_CHECK

typedef double StackElem;

#define STACK_ELEM_FORMAT "%lg"

enum StackError_e {
    STACK_NULL, STACK_CORRUPT, STACK_INVALID, STACK_OUT_OF_MEMORY, STACK_OK
};

#ifdef STACK_MARKER_CHECK
typedef size_t MarkerType;
#endif

#ifdef STACK_HASH_CHECK
typedef struct {
    unsigned mdHash;
    unsigned dataHash;
} StackHashT;

#endif

typedef struct {

#ifdef STACK_MARKER_CHECK
    MarkerType _beginMarker;
#endif

    size_t mSize;
    size_t mCapacity;
    StackElem *mData;

#ifdef STACK_HASH_CHECK
    StackHashT mHash;
#endif

#ifdef STACK_MARKER_CHECK
    MarkerType _endMarker;
#endif
} Stack;

typedef enum StackError_e StackError;

size_t StackSize(Stack * self);

StackElem StackTop(Stack *self);

StackError StackPop(Stack *self);

StackError StackPush(Stack *self, StackElem value);

StackError StackResize(Stack *self, size_t size);

void StackDelete(Stack *self);

void StackFree(Stack *self);

Stack *StackNew(size_t capacity);

StackError StackConstruct(Stack *self, size_t capacity);

void StackDump(Stack *self);

StackError StackValidate(Stack *self);
