#pragma once

#include <stdlib.h>

#define STACK_HASH_CHECK
#define STACK_MARKER_CHECK

typedef double StackElem;
#define STACK_ELEM_FORMAT "%lg"

enum StackError_e {
    STACK_OK            = 0,
    STACK_NULL          = 1,
    STACK_CORRUPT       = 2,
    STACK_INVALID       = 3,
    STACK_OUT_OF_MEMORY = 4,
    STACK_EMPTY         = 5,
};
typedef enum StackError_e StackError;

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

/*!
 *
 * @param self pointer to Stack
 * @return count of elements in stack. In case of error return NPOS
 */
size_t StackSize(Stack *self);

/*!
 *
 * @param self pointer to Stack
 * @return Top element of Stack. In case of error return NAN
 */
StackElem StackTop(Stack *self);

/*!
 * Remove top element from Stack.
 *
 * @param self pointer to Stack
 * @return Error code
 */
StackError StackPop(Stack *self);

/*!
 * Push element to Stack.
 * @param self pointer to Stack
 * @param value element to push
 * @return Error code
 */
StackError StackPush(Stack *self, StackElem value);

/*!
 * Resize stack
 * @param self pointer to Stack
 * @param size expected size
 * @return Error code
 */
StackError StackResize(Stack *self, size_t size);

/*!
 * Delete stack created by StackNew
 * @param self pointer to stack
 */
void StackDelete(Stack *self);

/*!
 * Free stack resources
 * @param self pointer to stack
 * @return error code
 */
enum StackError_e StackFree(Stack *self);

/*!
 * Creates new stack in dynamic memory
 * @param capacity desired size
 * @return pointer to Stack
 */
Stack *StackNew(size_t capacity);

/*!
 * Construct stack
 * @param self pointer to Stack to be constructed
 * @param capacity desired size
 * @return Error code
 */
StackError StackConstruct(Stack *self, size_t capacity);


/*!
 * Dump stack to logfile
 * @param self pointer to Stack
 */
void StackDump(Stack *self);

/*!
 * Validate stack
 * @param self pointer to Stack
 * @return current error code
 */
StackError StackValidate(Stack *self);

#ifdef STACK_HASH_CHECK

/*!
 *
 * @param stack pointer to stack
 * @return hash of Stack
 */
StackHashT StackHash(Stack *stack);

#endif
