
#include <core/RXCore.h>
#include "RXObject.h"
#include "RXBoolean.h"
#include "RXInteger.h"
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

#define RXList_payload(self) ((RXList_t*)self)->payload

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
    fputs("list(", stdout);
    Eina_List* iter;
    RXObject_t* data;
    bool sep = false;
    EINA_LIST_FOREACH(RXList_payload(self), iter, data) {
        if (sep) {
            fputs(", ", stdout);
        }
        else {
            sep = true;
        }
        RXObject_respondTo(data, RXSymbol_print_o, 0);
    }
    fputs(")", stdout);
}

static RXNativeMethod_define(RXList, append) {
    assert(argumentCount >= 1);
    RXObject_t* arg = RXObject_valueOfArgumentAt(0);
    RXList_payload(self) = eina_list_append(RXList_payload(self), arg);
    assert(!eina_error_get());
    return self;
}

static RXNativeMethod_define(RXList, concat) {
    assert(argumentCount >= 1);
    // TODO RXList concat
}

static RXNativeMethod_define(RXList, prepend) {
    assert(argumentCount >= 1);
    RXList_payload(self) = eina_list_prepend(RXList_payload(self), RXObject_valueOfArgumentAt(0));
    return self;
}

static RXNativeMethod_define(RXList, insertAt) {
    assert(argumentCount >= 2);
    // TODO RXList insertAt
}

static RXNativeMethod_define(RXList, replaceAt) {
    assert(argumentCount >= 2);
    // TODO RXList insertAt
}

static RXNativeMethod_define(RXList, removeLast) {
    // TODO RXList removeLast
}

static RXNativeMethod_define(RXList, removeFirst) {
    // TODO RXList removeLast
}

static RXNativeMethod_define(RXList, removeAt) {
    assert(argumentCount >= 1);
    // TODO RXList removeAt
}

static RXNativeMethod_define(RXList, removeAll) {
    // TODO RXList removeAll
}

static RXNativeMethod_define(RXList, last) {
    // TODO RXList last
}

static RXNativeMethod_define(RXList, first) {
    RXObject_t* data = eina_list_data_get(RXList_payload(self));
    return data != NULL
        ? data
        : RXNil_o;
}

static RXNativeMethod_define(RXList, at) {
    assert(argumentCount >= 1);
    RXObject_t* data = eina_list_nth(RXList_payload(self), *(int*)RXObject_valueOfArgumentAt(0));
    return data != NULL
        ? data
        : RXNil_o;
}

static RXNativeMethod_define(RXList, count) {
    return (RXObject_t*)RXInteger_new(eina_list_count(RXList_payload(self)));
}

static RXNativeMethod_define(RXList, isEmpty) {
    return RXList_payload(self) != NULL
        ? RXBoolean_true_o
        : RXBoolean_false_o;
}

static RXNativeMethod_define(RXList, select) {
    assert(argumentCount >= 2);
    // TODO RXList select
}

static RXNativeMethod_define(RXList, collect) {
    assert(argumentCount >= 2);
    // TODO RXList collect
}

static RXNativeMethod_define(RXList, forEach) {
    assert(argumentCount >= 2);
    // TODO RXList forEach
}

static RXNativeMethod_define(RXList, forAll) {
    assert(argumentCount >= 2);
    // TODO RXList forAll
}

static RXNativeMethod_define(RXList, exists) {
    assert(argumentCount >= 2);
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
    RXNativeMethod_attach(RXList, replaceAt);
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
