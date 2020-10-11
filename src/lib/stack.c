#include <assert.h>
#include <stdio.h>
#include <math.h>


#include "stack.h"
#include "hash.h"
#include "log.h"

static const size_t NPOS = (size_t) -1;

#ifdef STACK_MARKER_CHECK
const size_t MARKER_CODE = 0x1337C0D3;
const size_t MARKER_SIZE = sizeof(MarkerType);
#else
const size_t MARKER_SIZE = 0;
#endif


#define ASSERT_OK(self) do {                    \
StackError errCode = StackValidate(self);       \
if (errCode != STACK_OK) {                      \
    fprintf(logfile, "Stack invalid! Dump:\n"); \
    StackDump(self);                            \
    return errCode;                             \
} } while(0)


#ifdef STACK_HASH_CHECK
#define REHASH(self) (self)->mHash = StackHash(self)
#else
#define REHASH(self)
#endif


#ifdef STACK_HASH_CHECK

StackHashT StackHash(Stack *stack) {
    const size_t mdShift = (size_t) &(((Stack *) 0)->mSize);
    const size_t mdEnd = (size_t) &(((Stack *) 0)->mHash);

    StackHashT result;
    result.mdHash = 0;
    result.dataHash = 0;
    if (!stack) return result;

    result.mdHash = hash(((char *) stack) + mdShift, mdEnd - mdShift);
    if (stack->mData)
        result.dataHash = hash(stack->mData, stack->mCapacity * sizeof(StackElem));
    return result;
}

#endif

StackError StackValidate(Stack *self) {
    if (!self)
        return STACK_NULL;

    if (!self->mData) {
        return STACK_INVALID;
    }
#ifdef STACK_MARKER_CHECK
    if (self->_endMarker != MARKER_CODE || self->_beginMarker != MARKER_CODE) {
        return STACK_CORRUPT;
    }
    if (*((MarkerType *) self->mData - 1) != MARKER_CODE ||
        *((MarkerType *) (self->mData + self->mCapacity)) != MARKER_CODE) {
        return STACK_CORRUPT;
    }
#endif

#ifdef STACK_HASH_CHECK
    StackHashT hashSum = StackHash(self);
    if (hashSum.dataHash != self->mHash.dataHash || hashSum.mdHash != self->mHash.mdHash) {
        return STACK_CORRUPT;
    }
#endif


    if (self->mCapacity < self->mSize) {
        return STACK_INVALID;
    }
    if (self->mCapacity == 0) {
        return STACK_INVALID;
    }
    return STACK_OK;
}

const char *errorToString(StackError err) {
    switch (err) {
        case STACK_NULL:
            return "NULL";
        case STACK_CORRUPT:
            return "CORRUPTED";
        case STACK_INVALID:
            return "INVALID";
        case STACK_OK:
            return "OK";
        case STACK_OUT_OF_MEMORY:
            return "OUT OF MEMORY";
        default:
            return "UNKNOWN";
    }
}

void StackDump(Stack *self) {
    StackError errCode = StackValidate(self);
    fprintf(logfile, "Stack (%s) [0x%zX] {\n", errorToString(errCode), (size_t) self);

    if (errCode == STACK_NULL) {
        fprintf(logfile, "\tData unavailable \n");
        fprintf(logfile, "}");
        return;
    }

#ifdef STACK_MARKER_CHECK
    fprintf(logfile, "\t_beginMarker = %zX\n", self->_beginMarker);
#endif

    fprintf(logfile, "\tmSize        = %zo\n", self->mSize);
    fprintf(logfile, "\tmCapacity    = %zo\n", self->mCapacity);
    fprintf(logfile, "\tmData [0x%zX] {\n", (size_t) self->mData);

    if (!self->mData) {
        fprintf(logfile, "\t\tData unavailable\n");
    } else {

#ifdef STACK_MARKER_CHECK
        fprintf(logfile, "\t\tbeginMarkerData = %zX\n", *((MarkerType *) self->mData - 1));
#endif

        for (size_t i = 0; i < self->mCapacity; ++i) {
            fprintf(logfile, "\t\t[%zu] = " STACK_ELEM_FORMAT "\n", i, self->mData[i]);
        }

#ifdef STACK_MARKER_CHECK
        fprintf(logfile, "\t\tendMarkerData = %zX\n", *((MarkerType *) (self->mData + self->mCapacity)));
#endif

    }
    fprintf(logfile, "\t}\n");

#ifdef STACK_HASH_CHECK
    fprintf(logfile,
            "\tmHash = {\n"
            "\t\tmdHash   = %X\n"
            "\t\tdataHash = %X\n"
            "\t}\n",
            self->mHash.mdHash,
            self->mHash.dataHash
    );
#endif

#ifdef STACK_MARKER_CHECK
    fprintf(logfile, "\t_endMarker = %zX\n", self->_endMarker);
#endif

    fprintf(logfile, "}\n");
}

StackError StackConstruct(Stack *self, size_t capacity) {
    assert(self);
    if (capacity == 0) {
        return STACK_INVALID;
    }

    self->mCapacity = capacity;
    self->mSize = 0;
    self->mData = calloc(1, capacity * sizeof(StackElem) + 2 * MARKER_SIZE);
    if (!self->mData) return STACK_OUT_OF_MEMORY;

#ifdef STACK_MARKER_CHECK
    self->_beginMarker = MARKER_CODE;
    self->_endMarker = MARKER_CODE;
    *((MarkerType *) self->mData) = MARKER_CODE;
    self->mData = (StackElem *) ((MarkerType *) self->mData) + 1;
    *((MarkerType *) (self->mData + capacity)) = MARKER_CODE;
#endif

    REHASH(self);
    return STACK_OK;
}

Stack *StackNew(size_t capacity) {
    Stack *self = calloc(1, sizeof(*self));
    if (!self) return self;

    if (StackConstruct(self, capacity) != STACK_OK) {
        free(self);
        return NULL;
    }

    return self;
}

StackError StackFree(Stack *self) {
    ASSERT_OK(self);
    free(((char *) self->mData) - MARKER_SIZE);
    return STACK_OK;
}

void StackDelete(Stack *self) {
    StackFree(self);
    free(self);
}

StackError StackResize(Stack *self, size_t size) {
    ASSERT_OK(self);
    self->mCapacity = size;

    char *data = (char *) self->mData;
    data -= MARKER_SIZE;
    data = realloc(data, size * sizeof(StackElem) + 2 * MARKER_SIZE);
    if (!data) return STACK_OUT_OF_MEMORY;
    self->mData = (StackElem *) (data + MARKER_SIZE);


#ifdef STACK_MARKER_CHECK
    *((MarkerType *) (data + self->mCapacity * sizeof(StackElem) + MARKER_SIZE)) = MARKER_CODE;
#endif

    REHASH(self);
    ASSERT_OK(self);
    return STACK_OK;
}


StackError StackPush(Stack *self, StackElem value) {
    ASSERT_OK(self);

    if (self->mCapacity <= self->mSize) {
        StackError err = StackResize(self, self->mCapacity * 2);
        if (err != STACK_OK) {
            StackDump(self);
            return err;
        }
    }
    self->mData[self->mSize++] = value;

    REHASH(self);
    ASSERT_OK(self);
    return STACK_OK;
}

StackError StackPop(Stack *self) {
    ASSERT_OK(self);
    if (StackSize(self) == 0) {
        fprintf(logfile, "Trying to pop from empty stack! Dump:\n");
        StackDump(self);
        return STACK_EMPTY;
    }
    self->mData[--self->mSize] = NAN;

    REHASH(self);
    ASSERT_OK(self);
    return STACK_OK;
}

StackElem StackTop(Stack *self) {
    StackError errCode = StackValidate(self);
    if (errCode != STACK_OK) {
        fprintf(logfile, "Stack invalid! Dump:\n");
        StackDump(self);
        return NAN;
    }

    if (self->mSize == 0) {
        fprintf(logfile, "Trying to read empty stack! Dump:\n");
        StackDump(self);
        return NAN;
    }

    return self->mData[self->mSize - 1];
}

size_t StackSize(Stack *self) {
    StackError errCode = StackValidate(self);
    if (errCode != STACK_OK) {
        fprintf(logfile, "Stack invalid! Dump:\n");
        StackDump(self);
        return NPOS;
    }
    return self->mSize;
}

#undef ASSERT_OK
#undef REHASH
