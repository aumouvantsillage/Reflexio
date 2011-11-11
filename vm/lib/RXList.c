
#include <core/RXCore.h>
#include "RXList.h"

// Private -------------------------------------------------------------

struct RXList_s {
    RXObject_declaration;
    Eina_List* payload;
};

inline static RXList_t* RXList_new(void) {
    RXList_t* self = (RXList_t*)RXCore_allocateObjectOfType(RXList_t);
    RXObject_initialize((RXObject_t*)self);
    self->payload = NULL;
    return self;
}

// Methods -------------------------------------------------------------

static RXNativeMethod_define(RXList, new) {
    RXList_t* result = RXList_new();
    RXObject_setSlot((RXObject_t*)result, RXSymbol_delegate_o, self);
    // TODO clone list from self
    return (RXObject_t*)result;
}

static RXNativeMethod_define(RXList, asString) {
    // TODO RXList asString
}

static RXNativeMethod_define(RXList, print) {
    // TODO RXList print
}

static RXNativeMethod_define(RXList, append) {
    // TODO RXList append
}

static RXNativeMethod_define(RXList, concat) {
    // TODO RXList join
}

static RXNativeMethod_define(RXList, prepend) {
    // TODO RXList append
}

static RXNativeMethod_define(RXList, insertAt) {
    // TODO RXList insertAt
}

static RXNativeMethod_define(RXList, removeLast) {
    // TODO RXList removeLast
}

static RXNativeMethod_define(RXList, removeFirst) {
    // TODO RXList removeLast
}

static RXNativeMethod_define(RXList, removeAt) {
    // TODO RXList removeAt
}

static RXNativeMethod_define(RXList, removeAll) {
    // TODO RXList removeAll
}

static RXNativeMethod_define(RXList, last) {
    // TODO RXList last
}

static RXNativeMethod_define(RXList, first) {
    // TODO RXList first
}

static RXNativeMethod_define(RXList, at) {
    // TODO RXList at
}

static RXNativeMethod_define(RXList, count) {
    // TODO RXList count
}

static RXNativeMethod_define(RXList, isEmpty) {
    // TODO RXList isEmpty
}

static RXNativeMethod_define(RXList, select) {
    // TODO RXList select
}

static RXNativeMethod_define(RXList, collect) {
    // TODO RXList collect
}

static RXNativeMethod_define(RXList, forEach) {
    // TODO RXList forEach
}

static RXNativeMethod_define(RXList, forAll) {
    // TODO RXList forAll
}

static RXNativeMethod_define(RXList, exists) {
    // TODO RXList exists
}

// Public --------------------------------------------------------------

RXList_t* RXList_o;

void RXList_setup(void) {
    RXList_o = RXList_new();
    RXObject_setSlot((RXObject_t*)RXList_o, RXSymbol_delegate_o, RXObject_o);
    RXObject_setSlot(RXObject_o, RXSymbol_symbolForCString("List"), (RXObject_t*)RXList_o);
    
    RXNativeMethod_attach(RXList, new);
    RXNativeMethod_attach(RXList, asString);
    RXNativeMethod_attach(RXList, print);
    RXNativeMethod_attach(RXList, append);
    RXNativeMethod_attach(RXList, concat);
    RXNativeMethod_attach(RXList, prepend);
    RXNativeMethod_attach(RXList, insertAt);
    RXNativeMethod_attach(RXList, removeLast);
    RXNativeMethod_attach(RXList, removeFirst);
    RXNativeMethod_attach(RXList, removeAt);
    RXNativeMethod_attach(RXList, removeAll);
    RXNativeMethod_attach(RXList, last);
    RXNativeMethod_attach(RXList, first);
    RXNativeMethod_attach(RXList, at);
    RXNativeMethod_attach(RXList, count);
    RXNativeMethod_attach(RXList, isEmpty);
    RXNativeMethod_attach(RXList, select);
    RXNativeMethod_attach(RXList, collect);
    RXNativeMethod_attach(RXList, forEach);
    RXNativeMethod_attach(RXList, forAll);
    RXNativeMethod_attach(RXList, exists);
}

void RXList_clean(void) {
    RXObject_deleteSlot((RXObject_t*)RXList_o, RXSymbol_delegate_o);
}
