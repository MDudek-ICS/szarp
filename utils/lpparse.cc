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
/*
 * $Id$
 *
 * Pawe� Pa�ucha (pawel@praterm.com.pl)
 * lpparse.c
 *
 * Program parsuj�cy plik konfiguracyjny SZARP'a na potrzeby program�w
 * nie mog�cych korzysta� bezpo�rednio z biblioteki libpar (a wi�c
 * np. programy w Tcl/Tk, Perlu, skrypty shelowe. 
 *
 * Wypisuje w kolejnych liniach wartosci ��danych parametr�w, opcja "-s"
 * ustala sekcj�, z kt�rej b�d� czytane dalsze parametry.
 *
 * Opcja -n z parametrem pozwala okre�li� napis, kt�ry b�dzie wypisywany
 * je�li parametr nie zostanie znaleziony (standardowo jest to pusta linia).
 * Jest to potrzebne do prawid�owego dzielenia na linie przez Tcl'a.
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "liblog.h"
#include "libpar.h"

void usage (void)
{
	printf(
"Usage:\n\
lpparse [ -n <string>][ -D<name>=<value> ] ... [ [ -s <section> ] \n\
	<parametr> ... ] ...\n"
		);
}

int main(int argc, char *argv[])
{
	int i;
	char * section;
	char *empty;
	
	loginit_cmdline(0, "", &argc, argv);
	libpar_read_cmdline(&argc, argv);
	if (argc <= 1) {
		usage();
		return 0;
	}
	libpar_init();
	i = 1;
	section = (char *) strdup("");
	empty = (char *) strdup("");
	while (i < argc) {
		if ((!strcmp(argv[i], "-n")) && ( i < argc-1 ) ) {
			i++;
			free(empty);
			empty = (char *) strdup(argv[i]);
		} else if ((!strcmp(argv[i], "-s")) && ( i < argc-1 ) ) {
			i++;
			free(section);
			section = (char *) strdup(argv[i]);
		} else {
			char *param = libpar_getpar(section, argv[i], 0);
			if (param != NULL) {
				printf("%s\n",param);
				free(param);
			} else
				printf("%s\n", empty);
		}
		i++;
	}
	return 0;
}
