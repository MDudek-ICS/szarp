/* 
  libSzarp - SZARP library

*/
/**
 * Pawe� Pa�ucha, 6.10.2001
 * SZARP 2.1
 *
 * $Id$
 *
 * getpasswd - wczytywanie has�a bez echa na ekran
 */

#ifndef __PASSWD_H__
#define __PASSWD_H__


char *getpasswd(int print_stars);
int ch_passwd(char *passwd, char *encr);

#endif
