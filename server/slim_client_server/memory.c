/**
 * @file memory.c
 * @brief
 * Modul pro práci s pamětí. Funkce pro alokaci jsou vybaveny 
 * počítadlem, takže je možné zhruba zjistit kolikrát bylo žádáno
 * o paměť a kolikrát byla vrácena.
 */
#include <stdint.h>
#include <stdlib.h>
#include "error.h"

// global
void * mmalloc (unsigned int size);
void mfree(void * pointer);
uint16_t used_mem(void);
//global

/// Počet použitých paměťových bloků
uint16_t used_mem_blocks = 0;

/// Má alokační funkce. Testuje, zda nebyla naplněna paměť. Pokud ano, vypíše
/// chybovou hlášku.
void * mmalloc (unsigned int size){
  void * pointer;
  pointer = malloc (size);
  if (pointer == NULL){
    new_error(88);
    return NULL;
  }
  else {
    used_mem_blocks++;
    return pointer;
    
  }
}



void mfree(void * pointer){
  free(pointer);
  used_mem_blocks--;
}


uint16_t used_mem(void){
  return used_mem_blocks;
}
