
#ifndef __RX_CORE_SYMBOL_H__
#define __RX_CORE_SYMBOL_H__

typedef struct RXSymbol_s RXSymbol_t;

/*
 * Type RXSymbol_t is defined in RXObject.h
 */

extern RXSymbol_t* RXSymbol_o;
extern RXSymbol_t* RXSymbol_activate_o;
extern RXSymbol_t* RXSymbol_delegate_o;
extern RXSymbol_t* RXSymbol_lookup_o;
extern RXSymbol_t* RXSymbol_new_o;
extern RXSymbol_t* RXSymbol_delete_o;

/*
 * Create predefined objects used in this module.
 */
void RXSymbol_setup(void);

/*
 * Delete all objects created by this module.
 */
void RXSymbol_clean(void);

RXSymbol_t* RXSymbol_new(const char* str);

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
