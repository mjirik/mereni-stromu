#include <stdio.h>
#include <stdint.h>

/* globální funkce */
void uint2string(char * msg, uint32_t number1);//), uint32_t number2);
void string2uint(char * msg, uint32_t * number1);//), uint32_t * number2);
/* globální funkce */

/**
 * Funkce vloží jedno číslo do řetězce. řetězec musí mít dostatečnou velikost.
 */
void uint2string(char * msg, uint32_t number1){//, uint32_t number2){
  uint32_t * uint32;
  // zde se vytvoří ukazatel na první datový byte zprávy, řekne se že je to
  // uint32 a přiřadí se do něj hodnota
  uint32 = ((uint32_t *)&(msg[0]));
  *uint32 = number1;

/*
  if (number2 != NULL) {

    uint32 = ((uint32_t *)&(msg[4]));
    *uint32 = number2;
  }
*/
}

/// Funkce získá ze zprávy dvě čísla uint32_t
/// @param number1 ukazatel na číslo, kam se má vrátit první číslo
/// @param number2 ukazatel na číslo, kam se má vrátit druhé číslo, pokud
//     je NULL, neprovádí se nic
/// @param msg zpráva z níž mají být čísla získána
void string2uint(char * msg, uint32_t * number1){//, uint32_t * number2){
  uint32_t * uint32;

  uint32 = (uint32_t *) (msg);
  *number1 = *uint32;

/*
  if (number2 != NULL){
    uint32 = (uint32_t *) (msg+4);
    *number2 = *uint32;
  }
*/

}
