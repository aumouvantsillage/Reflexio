
#include "RXLib.h"
#include <stdbool.h>
#include <assert.h>

// Private -------------------------------------------------------------

RXObject_defineType(RXList_t,
    Eina_List* first;
);

#define RXList_first(self) ((RXList_t*)self)->first

// Methods -------------------------------------------------------------

RXNativeMethod_define(RXList, spawn) {
    return RXList_spawn(self, RXList_first(self));
}

RXNativeMethod_define(RXList, with) {
    RXObject_t* result = RXList_spawn(self, RXList_first(self));
    for (int i = 0; i < argumentCount; i ++) {
        RXList_append(result, RXExpression_valueOfArgumentAt(i, context));
    }
    return result;
}

RXNativeMethod_define(RXList, asString) {
    int count = RXList_count(self);
    RXObject_t* msgSrc[count];
    int len = 10;
    int index = 0;
    Eina_List* iter;
    RXObject_t* msg;
    EINA_LIST_FOREACH(*(Eina_List**)self, iter, msg) {
        RXObject_t* src = RXObject_respondTo(msg, RXSymbol_asSource_o, RXNil_o, 0);
        len += strlen((char*)src) + 2;
        msgSrc[index++] = src;
    }
    char result[len];
    strcpy(result, "List with(");
    for (index = 0; index < count; index ++) {
        if (index > 0) {
            strcat(result, ", ");
        }
        strcat(result, (char*)(msgSrc[index]));
    }
    strcat(result, ")");
    return RXSymbol_symbolForCString(result);
}

RXNativeMethod_define(RXList, asBoolean) {
    return RXList_first(self) != NULL
        ? RXBoolean_true_o
        : RXBoolean_false_o;
}

RXNativeMethod_define(RXList, print) {
    fputs("list(", stdout);
    Eina_List* iter;
    RXObject_t* data;
    bool sep = false;
    EINA_LIST_FOREACH(RXList_first(self), iter, data) {
        if (sep) {
            fputs(", ", stdout);
        }
        else {
            sep = true;
        }
        RXObject_respondTo(data, RXSymbol_print_o, RXNil_o, 0);
    }
    fputs(")", stdout);
}

RXNativeMethod_define(RXList, append) {
    if (argumentCount > 0) {
        RXObject_t* item = RXExpression_valueOfArgumentAt(0, context);
        RXList_first(self) = eina_list_append(RXList_first(self), item);
        // TODO mark new object in the same reachability as self
        assert(!eina_error_get());
    }
    return self;
}

RXNativeMethod_define(RXList, concat) {
    assert(argumentCount >= 1);
    // TODO RXList concat
}

RXNativeMethod_define(RXList, prepend) {
    if (argumentCount > 0) {
        RXObject_t* item = RXExpression_valueOfArgumentAt(0, context);
        RXList_first(self) = eina_list_prepend(RXList_first(self), item);
        // TODO mark new object in the same reachability as self
    }
    return self;
}

RXNativeMethod_define(RXList, insertAt) {
    assert(argumentCount >= 2);
    // TODO RXList insertAt
    // TODO mark new object in the same reachability as self
}

RXNativeMethod_define(RXList, replaceAt) {
    assert(argumentCount >= 2);
    // TODO RXList insertAt
    // TODO mark new object in the same reachability as self
    // TODO mark replaced object as possibly collectable
}

RXNativeMethod_define(RXList, removeLast) {
    // TODO RXList removeLast
    // TODO mark removed object as possibly collectable
}

RXNativeMethod_define(RXList, removeFirst) {
    // TODO RXList removeLast
    // TODO mark removed object as possibly collectable
}

RXNativeMethod_define(RXList, removeAt) {
    assert(argumentCount >= 1);
    // TODO RXList removeAt
    // TODO mark removed object as possibly collectable
}

RXNativeMethod_define(RXList, removeAll) {
    // TODO RXList removeAll
    // TODO mark removed objects as possibly collectable
}

RXNativeMethod_define(RXList, last) {
    // TODO RXList last
}

RXNativeMethod_define(RXList, first) {
    RXObject_t* data = eina_list_data_get(RXList_first(self));
    return data != NULL
        ? data
        : RXNil_o;
}

RXNativeMethod_define(RXList, at) {
    if (argumentCount > 0) {
        RXObject_t* index = RXExpression_valueOfArgumentAt(0, context);
        RXObject_t* data = eina_list_nth(RXList_first(self), *(int*)index);
        return data != NULL ? data : RXNil_o;
    }
    else {
        // TODO raise exception
        return RXNil_o;
    }
}

RXNativeMethod_define(RXList, count) {
    return RXInteger_spawn(RXInteger_o, eina_list_count(RXList_first(self)));
}

RXNativeMethod_define(RXList, isEmpty) {
    return RXList_first(self) != NULL
        ? RXBoolean_true_o
        : RXBoolean_false_o;
}

RXNativeMethod_define(RXList, select) {
    assert(argumentCount >= 2);
    // TODO RXList select
}

RXNativeMethod_define(RXList, collect) {
    assert(argumentCount >= 2);
    // TODO RXList collect
}

RXNativeMethod_define(RXList, forEach) {
    assert(argumentCount >= 2);
    // TODO RXList forEach
}

RXNativeMethod_define(RXList, forAll) {
    assert(argumentCount >= 2);
    // TODO RXList forAll
}

RXNativeMethod_define(RXList, exists) {
    assert(argumentCount >= 2);
    // TODO RXList exists
}

// Public --------------------------------------------------------------

RXObject_t* RXList_o;
RXObject_t* RXSymbol_List_o;

RXObject_t* RXList_spawn(RXObject_t* self, Eina_List* list) {
    RXObject_t* result = RXObject_allocateType(RXList_t);
    RXObject_initialize(result);
    RXObject_setDelegate(result, self);
    RXList_first(result) = eina_list_clone(list);
    return result;
}

int RXList_count(const RXObject_t* self) {
    return eina_list_count(*(Eina_List**)self);
}

void RXList_append(RXObject_t* self, RXObject_t* obj) {
    RXList_first(self) = eina_list_append(RXList_first(self), obj);
}

RXObject_t* RXList_at(RXObject_t* self, int index) {
    return eina_list_nth(RXList_first(self), index);
}

void RXList_setup(void) {
    RXList_o = RXList_spawn(RXObject_o, NULL);
    
    RXNativeMethod_attach(RXList, spawn);
    RXNativeMethod_attach(RXList, with);
    RXNativeMethod_attach(RXList, asString);
    RXNativeMethod_attach(RXList, asBoolean);
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

    RXSymbol_List_o = RXSymbol_symbolForCString("List");
    RXObject_setSlot(RXLobby_o, RXSymbol_List_o, RXList_o);
}
