
#ifndef __RX_CORE_SYMBOL_H__
#define __RX_CORE_SYMBOL_H__

typedef struct RXSymbol_s RXSymbol_t;

extern RXSymbol_t* RXSymbol_o;
extern RXSymbol_t* RXSymbol_activate_o;
extern RXSymbol_t* RXSymbol_delegate_o;
extern RXSymbol_t* RXSymbol_lookup_o;
extern RXSymbol_t* RXSymbol_new_o;

/*
 * Create predefined objects used in this module.
 */
void RXSymbol_setup(void);

/*
 * Delete all objects created by this module.
 * Release all symbols.
 */
void RXSymbol_clean(void);

/*
 * Create a new symbol from a character string.
 * This method is used only to bootstrap the virtual machine
 * with predefined symbols.
 *
 * Symbols are automatically retained at creation, and released
 * when this module is cleaned up.
 *
 * Use RXSymbol_symbolForCString instead.
 */
RXSymbol_t* RXSymbol_new(const char* str);

/*
 * Return the symbol that corresponds to the given string.
 * If no such symbol exists, create a new one.
 *
 * Symbols are automatically retained at creation, and released
 * when this module is cleaned up.
 */
RXSymbol_t* RXSymbol_symbolForCString(const char* str);

#endif
