/*
 * $Id: keyboard.c 64 2007-08-03 23:04:08Z Mira $
 * 
 * File name: keyboard.c
 * Date:      2006/07/31 22:21
 * Author:    Mira
 */
/**
 * @file keyboard.c
 * @brief
 * Modul obsluhuje kl�vesnici. �asov�n� je p�ipojeno k p�eru�en� INT1, 
 * data jsou p�ivedena na libovoln� pin. Hodnoty kl�ves jsou �teny dv�ma funkcemi.
 * get_char() a get_spec(). Modul je vybaven v�stupn�m bufferem, tak�e v p��pad�
 * stisku n�kolika kl�ves bezprost�edn� po sob�, nedojde ke ztr�t� v d�sledku
 * pomal�ho zpracov�v�n� dal��m programem.
 */

#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keyboard.h"
#include "lcd.h"
#include "convert.h"

/// Konstanta uv�d�, po kolika zavol�n�ch p�i nedoru�en�m bitu je resetov�n p��jem.
/// T�mto je o�et�en probl�m, kter� vznikal p�i p�ipojen� kla�vesnice k b��c�mu
/// za��zen�. Dorazila pak v�dy jen ��st bit� a p�i stisku dal�� kl�vesy byly
/// chybjej�c� data dopln�na za��tkam n�sleduj�c�ch. Zaveden� resetu probl�m �e��.
#define KEYB_RECV_WATCH_N 1000

/// Velikost v�stupn�ho bufferu kl�vesnice.
#define KEYB_BUFF_SIZE 3



/// Struktura pro popis stisknut� kl�vesy.
typedef struct key{
  /// Vlajka ukazuj�c� typ kl�vesy, kter� byla stisknuta.
  /// - 0 : nic
  /// - 1 : p�ijat char
  /// - 2 : efko, �ipka
  char flag;
  /// Vlastn� k�d kl�vesy. V p��pad� �e je flag == 0, je to ASCII.
  char kod;
}KEY;



static int8_t data_counter = 0;  ///< Po��tadl bit�, kter� ji� p�i�ly od kl�vesnice.
static char recivedc = 0; ///< St�ada� bit�.

static char minuly_kod = 0;
static uint16_t keyb_recv_watchdog = 0;

/// Buffer pro v�stupn� k�dy. Pracuje se s n�m prost�ednictv�m
/// funkc� keyb_in_buff() a get_key_from_buff().
/// Buffer je pole v n�m� jsou prvky typu key
/// Buffer je implementov�n jako nekruhov�. Je to z d�odu u�et�en�
/// n�kolika m�lo byt�. Zv��en� v�po�etn� n�ro�nosti je nev�znamn�, neb�
/// velikost bufferu je celkem mal� ( KEYB_BUFF_SIZE ). Posouv�n�
/// ka�d� polo�ky o jedno m�sto p�i ka�d�m p��chodu nov� kl�vesy a
/// ka�d�m �ten� nezpomaluje b�h programu.
static KEY keyb_buff[KEYB_BUFF_SIZE]; 



// FUNKCE
void zpracuj_kod(char);
static KEY get_key_from_buff(void);

/// Funkce vlo�� do bufferu dal�� kl�vesu.
/// Funkce je definov�na v keyboard.h . T�m je umo�n�no Na��t�n�
/// kl�ves i z jin�ho zdroje ne� z kl�vesnice.
/// @param flag P��znak typu kl�vesy.
/// - 0 : nic
/// - 1 : p�ijat char
/// - 2 : efko, �ipka
///
/// @param kod Vlastn� k�d kl�vesy.
void keyb_in_buff(char flag, char kod){
  int i = 0;

  if (flag == 0)
    return;
  
  for (i = 0; i < (KEYB_BUFF_SIZE - 1); i++){
    if ((keyb_buff[i+1].flag) != 0){
      keyb_buff[i] = keyb_buff[i+1];
    }
    else{
      keyb_buff[i].flag = flag;
      keyb_buff[i].kod = kod;
      return;
    }
  }
  keyb_buff[KEYB_BUFF_SIZE - 1].flag = flag;
  keyb_buff[KEYB_BUFF_SIZE - 1].kod = kod;
}
  
/// Funkce pro v�b�r kl�vesy z bufferu. P�i v�b�ru je nutn� posunout
/// v�echny prvky o jeden vp�ed.
static KEY get_key_from_buff(void){
  int i = 0;
  KEY out;

  out.flag = keyb_buff[0].flag;
  out.kod = keyb_buff[0].kod;
  if (keyb_buff[0].flag == 0)
    return out;

  for(i = 0; i < (KEYB_BUFF_SIZE - 1); i++){
    if (keyb_buff[i+1].flag != 0){
      keyb_buff[i] = keyb_buff[i+1];
    }
    else{
      keyb_buff[i] = keyb_buff[i+1];
      return out;
    }
  }
  keyb_buff[KEYB_BUFF_SIZE - 1].flag = 0;
  return out;
}
    




SIGNAL(SIG_INTERRUPT1){

  keyb_recv_watchdog = 0;  // 
  
  if ((data_counter > 0)&&(data_counter < 9)){
    recivedc |= ((PINC & 1) << (data_counter - 1));
  }
  else if(data_counter == 10) {
    data_counter = -1;
    //printxy(default_screen, 0,0,s);
    //printxy(default_screen, 0,2, int2string("   ",recivedc));
    zpracuj_kod(recivedc);
    recivedc = 0;
  }
  //printxy(default_screen, 0,2, int2string("    ",data_counter));
  data_counter++;
}

/// Funkce na z�klad� k�du p�ijat�ho z kl�vesnice vytv��� k�d pro funkce se kter�mi
/// pracuje zbytek programu. V p��pad�, �e je stisknuta kl�vesa tisknuteln�ho znaku,
/// je �tec�m funkc�m poskytnuta jeho jeho ASCII hodnota. V p��pad� stiskut� kl�vesy
/// jin�ho typu je nastaven recive_flag a funkce pro �ten� b�n�ch znak� ( get_char() )
/// nic nevr�t�. Pro na�ten� k�du speci�ln� kl�vesy je nutno pou��t funkci get_spec().
void zpracuj_kod(char c){
  if (minuly_kod == 0){
    switch (c){
    case (0x16):
      keyb_in_buff(1,'1');
      break;
    case (0x1e):
      keyb_in_buff(1,'2');
      break;
    case (0x26):
      keyb_in_buff(1,'3');
      break;
    case (0x25):
      keyb_in_buff(1,'4');
      break;
    case (0x2e):
      keyb_in_buff(1,'5');
      break;
    case (0x36):
      keyb_in_buff(1,'6');
      break;
    case (0x3d):
      keyb_in_buff(1,'7');
      break;
    case (0x3e):
      keyb_in_buff(1,'8');
      break;
    case (0x46):
      keyb_in_buff(1,'9');
      break;
    case (0x45):
      keyb_in_buff(1,'0');
      break;
    case (0x4e):
      keyb_in_buff(1,'-');
      break;
    case (0x55):
      keyb_in_buff(1,'=');
      break;
    case (0x66):  //Backspace 1
      keyb_in_buff(2,1);
      break;
      
    case (0x0d): //Ta 2
      keyb_in_buff(2,2);
      break;
    case (0x15):
      keyb_in_buff(1,'Q');
      break;
    case (0x1d):
      keyb_in_buff(1,'W');
      break;
    case (0x24):
      keyb_in_buff(1,'E');
      break;
    case (0x2d):
      keyb_in_buff(1,'R');
      break;
    case (0x2c):
      keyb_in_buff(1,'T');
      break;
    case (0x35):
      keyb_in_buff(1,'Y');
      break;
    case (0x3c):
      keyb_in_buff(1,'U');
      break;
    case (0x43):
      keyb_in_buff(1,'I');
      break;
    case (0x44):
      keyb_in_buff(1,'O');
      break;
    case (0x4d):
      keyb_in_buff(1,'P');
      break;
    case (0x54):
      keyb_in_buff(1,'[');
      break;
    case (0x5b):
      keyb_in_buff(1,']');
      break;
    case (0x5d):
      keyb_in_buff(1,'\\');
      break;
      
    case (0x58): //Caps Lock
      keyb_in_buff(2,3);
      break;
    case (0x1c):
      keyb_in_buff(1,'A');
      break;
    case (0x1b):
      keyb_in_buff(1,'S');
      break;
    case (0x23):
      keyb_in_buff(1,'D');
      break;
    case (0x2b):
      keyb_in_buff(1,'F');
      break;
    case (0x34):
      keyb_in_buff(1,'G');
      break;
    case (0x33):
      keyb_in_buff(1,
'H');
      break;
    case (0x3b):
      keyb_in_buff(1,'J');
      break;
    case (0x42):
      keyb_in_buff(1,'K');
      break;
    case (0x4b):
      keyb_in_buff(1,'L');
      break;
    case (0x4c):
      keyb_in_buff(1,';');
      break;
    case (0x52):
      keyb_in_buff(1,'\'');
      break;
    case (0x5a): //Enter
      keyb_in_buff(2,4);
      break;
      
    case (0x1a):
      keyb_in_buff(1,'Z');
      break;
    case (0x22):
      keyb_in_buff(1,'X');
      break;
    case (0x21):
      keyb_in_buff(1,'C');
      break;
    case (0x2a):
      keyb_in_buff(1,'V');
      break;
    case (0x32):
      keyb_in_buff(1,'B');
      break;
    case (0x31):
      keyb_in_buff(1,'N');
      break;
    case (0x3a):
      keyb_in_buff(1,'M');
      break;
    case (0x41):
      keyb_in_buff(1,',');
      break;
    case (0x49):
      keyb_in_buff(1,'.');
      break;
    case (0x4a):
      keyb_in_buff(1,'/');
      break;
      
    case (0x29):
      keyb_in_buff(1,' ');
      break;
      
    case (0x76): //Esc
      keyb_in_buff(2,5);
      break;
      
    case 0x70: // 0_num
      keyb_in_buff(1,'0');
      break;
    case 0x69: // 1_num
      keyb_in_buff(1,'1');
      break;
    case 0x72: // 2_num
      keyb_in_buff(1,'2');
      break;
    case 0x7A: // 3_num
      keyb_in_buff(1,'3');
      break;
    case 0x6B: // 4_num
      keyb_in_buff(1,'4');
      break;
    case 0x73: // 5_num
      keyb_in_buff(1,'5');
      break;
    case 0x74: // 6_num
      keyb_in_buff(1,'6');
      break;
    case 0x6C: // 7_num
      keyb_in_buff(1,'7');
      break;
    case 0x75: // 8_num
      keyb_in_buff(1,'8');
      break;
    case 0x7D: // 9_num
      keyb_in_buff(1,'9');
      break;
    case 0x7B: // -_num
      keyb_in_buff(1,'-');
      break;
    case 0x71: // ._num
      keyb_in_buff(1,'.');
      break;
    case 0x7C: // *_num
      keyb_in_buff(1,'*');
      break;
    case 0x79: // +_num
      keyb_in_buff(1,'+');
      break;


      
    case (0x05): //F1
      keyb_in_buff(2,11);
      break;
    case (0x06): //F2
      keyb_in_buff(2,12);
      break;
    case (0x04): //F3
      keyb_in_buff(2,13);
      break;
    case (0x0c): //F4
      keyb_in_buff(2,14);
      break;
    case (0x03): //F5
      keyb_in_buff(2,15);
      break;
    case (0x0b): //F6
      keyb_in_buff(2,16);
      break;
    case (0x83): //F7
      keyb_in_buff(2,17);
      break;
    case (0x0a): //F8
      keyb_in_buff(2,18);
      break;
    case (0x01): //F9
      keyb_in_buff(2,19);
      break;
    case (0x09): //F10
      keyb_in_buff(2,20);
      break;
    case (0x78): //F11
      keyb_in_buff(2,21);
      break;
    case (0x07): //F12
      keyb_in_buff(2,22);
      break;
    case (0xe0):
      minuly_kod = 0xe0;
      break;
      
    case (0xf0):
      minuly_kod = 0xf0;
      break;
    default:
      //keyb_in_buff(0;
      //rec_buf = 0;
      break;
      
    }
  }
  else if (minuly_kod == 0xe0){
    switch (c){
    case 0x71: //Del
      keyb_in_buff(2,6);
      break;
    case 0x75: // Up
      keyb_in_buff(2,7);
      break;
    case 0x72: // Down
      keyb_in_buff(2,8);
      break;
    case 0x6B: // Left
      keyb_in_buff(2,9);
      break;
    case 0x74: // Right
      keyb_in_buff(2,10);
      break;
    case 0x5A: // Enter_num
      keyb_in_buff(2,4);
      break;
    case 0x4A: // /_num
      keyb_in_buff(1,'/');
      break;
    case 0xF0:
      minuly_kod = 0xF0;
      break;
    case 0xE0:
      minuly_kod = 0xE0;
      break;
    default:
      break;
    }
  }
  else if (minuly_kod == 0xF0 ){
    // napriklad pri pusteni sipky
    minuly_kod = 0;
  }
  else{
    minuly_kod = 0;
  }
  
  
}
//TODO
/// Funkce vr�t� posledn� p�ijat� znak .
char get_char(void){
  if (keyb_buff[0].flag == 1){
    return get_key_from_buff().kod;
  }
  else
    return 0;
}

/// Funkce vr�t� posledn� p�ijat� stisk speci�ln� kl�vesy.
char get_spec(void){
  if (keyb_buff[0].flag == 2){
    return get_key_from_buff().kod;
  }
  else
    return 0;
}

void keyboard_init(void){
  MCUCR |= (1 << ISC11)|(0 << ISC10); // nastaven� p�eru�en� se sestupnou hranou
  GICR |= (1 << INT1);
  
  // nastaven� pull-up odporu na DATA pinu
  PORTC |= 1;

  //nastaven� pull-up na CLK
  PORTD |= (1 << 3);
}

/// V p��pad� �e b�hem posledn�ch p�ti vol�n� t�to funkce nedo�lo k p��jmu
/// ��dn�ho bitu od kl�vesnice, (tedy pokud u� dlouho nic nep�i�lo)
/// p�eru�� p��jem posledn�ho sign�lu a nastav� data_counter na nulu pou��v� se k
/// �e�en� probl�mu s s nespr�vn�m p�ijet�m zpr�vy a n�sledn�m posunut�m p�ijmu
/// funkce p�ijme t�eba jen polovinu bit�, ale nev� �e u� je konec vys�l�n� a kdy� 
/// doraz� prvn� polovina dal��ho k�du, zap�e je jako druhou ��st p�edchoz�ho.
void keyboard_recv_watchdog(void){
  if (keyb_recv_watchdog > KEYB_RECV_WATCH_N ){
    data_counter = 0;
    recivedc = 0;
  }
  else
    keyb_recv_watchdog++;
}

/* end of keyboard.c */
