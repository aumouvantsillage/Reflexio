
#include <stdbool.h>
#include <assert.h>

/*
 * Accessor to the core data of a given object.
 */
#define RXObject_coreData(self) (self)->__coreData__[-1]

#define RXObject_flagIsLookingUp (1<<31)
#define RXObject_flagIsNativeMethod (1<<30)
#define RXObject_flags (RXObject_flagIsLookingUp | RXObject_flagIsNativeMethod)

inline static bool RXObject_isNativeMethod(const RXObject_t* self) {
    return (RXObject_coreData(self).meta & RXObject_flagIsNativeMethod) != 0;
}

inline static void RXObject_setIsNativeMethod(RXObject_t* self) { 
    RXObject_coreData(self).meta |= RXObject_flagIsNativeMethod;
}

#define RXObject_sizeOfCoreData sizeof(RXObjectCoreData_t)

/*
 * Allocate memory for a new object with a given amount of payload bytes.
 * This macro allocates memory for the core object data and the payload.
 */
inline static RXObject_t* RXCore_allocateObjectWithSize(const size_t size) {
    return (RXObject_t*)((char*)malloc(size + RXObject_sizeOfCoreData) + RXObject_sizeOfCoreData);
}

/*
 * Allocate memory for a new object with type t.
 * This macro allocates memory for the core object data and the payload.
 */
#define RXCore_allocateObjectOfType(t) RXCore_allocateObjectWithSize(sizeof(t))

/*
 * Deallocate memory for a previously allocated object.
 */
inline static void RXCore_deallocateObject(RXObject_t* self) {
    free((char*)self - RXObject_sizeOfCoreData);
}

/*
 * Initialize a newly allocated object.
 * Use this macro to initialize the core object data.
 */
inline static void RXObject_initialize(RXObject_t* self) {
    RXObject_coreData(self).slots = NULL;
    RXObject_coreData(self).meta = 0;
}

inline static RXObject_t* RXObject_new(void) {
    RXObject_t* self = RXCore_allocateObjectOfType(RXObject_t);
    RXObject_initialize(self);
    return self;
}

extern RXObject_t* RXSymbol_delegate_o;

inline static RXObject_t* RXObject_spawn(RXObject_t* self) {
    RXObject_t* result = RXObject_new();
    RXObject_setSlot(result, RXSymbol_delegate_o, self);
    return result;
}

typedef struct RXObjectNode_s RXObjectNode_t;

/*
 * Prepare the given object to be deleted
 */
inline static void RXObject_finalize(RXObject_t* self) {
    eina_rbtree_delete(RXObject_coreData(self).slots, EINA_RBTREE_FREE_CB(free), NULL);
}
