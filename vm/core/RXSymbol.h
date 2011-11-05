
#ifndef __RX_CORE_SYMBOL_H__
#define __RX_CORE_SYMBOL_H__

#include "RXObject.h"

/*
 * Create predefined objects used in this module.
 */
void RXSymbol_setup(void);

/*
 * Return the symbol that corresponds to the given string.
 * If no such symbol exists, create a new one.
 */
RXSymbol_t* RXSymbol_symbolForCString(const char* str);

/*
 * Print the list of registered symbols.
 */
void RXSymbol_dumpAll(void);

#endif
