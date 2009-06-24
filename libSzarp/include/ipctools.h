/* 
  SZARP: SCADA software 
  

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
/* $Id$ */

/*
 * SZARP 2.0
 * biblioteka MyInc
 * ipctools.h
 */


/*
 * Modified by Codematic 15.01.1998
 */

#ifndef _IPCTOOLS_H_
#define _IPCTOOLS_H_

extern "C" {
#include <sys/types.h> /* For ushort - Codematic */
}

#include "ipcdefines.h"


extern short *Probe;		/* wsp�lna pami�� pr�bki */	

extern short *Minute;		/* wsp�lna pami�� minuta */

extern short *Min10;		/* wsp�lna pami�� 10minut */

extern short *Hour;		/* wsp�lna pami�� godzina */

extern short *Day;		/* wsp�lna pami�� doba */

extern unsigned char *Alert;    /* wsp�lna pami�� alarmy przekrocze� */

struct phPTTEntry		/* opis pojedynczego parametru */
 {
  ushort addr;			/* adres w dzielonych tablicach */
  unsigned char dot;		/* pozycja kropki */
  char sym[SHORT_NAME_LEN];	/* nazwa skr�towa */
  char full[FULL_NAME_LEN];	/* pe�na nazwa */
  char alt[ALT_NAME_LEN];	/* nazwa alternatywna dla Przegladajacego*/
 };

typedef struct phPTTEntry tPTTEntry, *pPTTEntry;

struct phPTTInfo
 {
  ushort rev;			/* revision code */
  ushort len;			/* ilo�� wszystkich parametr�w */
  ushort blen;			/* ilo�� parametr�w dla bazy */
  short  dlen;			/* ilo�� parametr�w definiowanych (0 to brak) */ 
  tPTTEntry tab[2];		/* faktyczna d�ugo�� len element�w */	
 };

typedef struct phPTTInfo tPTTInfo;
typedef struct phPTTInfo * pPTTInfo;

extern pPTTInfo PTT;		/* wsp�lna pami�� PTT */

extern ushort VTlen, ExtraPars;

// from (void) to (int) - moified by Vooyeck

/* inicjalizacja segment�w dzielonych parametr�w */
extern int ipcInitialize();

/** wola przez podana liczbe sekund @see ipcInitialize ( co jedna sekunde )
 * wychodzi gdy zostanie przekorczony czas lub funkcja ipcInitialize zworci
 * cos innego niz -1
 * @param attempts ilosc prob
 * @return warosc ostatniego wywolania @see ipcInitialize
 */
int ipcInitializewithSleep(int attempts);

#if 0
int ipcPTTGet(char *programname);	/* inicjalizacja segmentu dzielonego PTT */
#endif
//int ipcPTTGetLocal(char *programname);	/* inicjalizacja prywatnej kopii PTT */
int ipcRdGetInto(unsigned char shmdes);	/* wej�cie dla czytaczy */
int ipcRdGetOut(unsigned char shmdes);		/* wyj�cie dla czytaczy */
int ipcWrGetInto(unsigned char shmdes); 	/* wej�cie pisarzy */
int ipcWrGetOut(unsigned char shmdes);		/* wyj�cie pisarzy */

#endif

