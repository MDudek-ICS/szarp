/* 
  libSzarp - SZARP library

*/
/*
 * $Id$
 *
 * Pawe� Pa�ucha
 * 19.06.2001
 * daemon.h
 *
 * libSzarp - biblioteka systemu SZARP 2.1
 *
 * Funkcja go_daemon - powoduje przejscie procesu w t�o, od��cza go
 * od konsoli, zmienia katalog aktualny na "/".
 * UWAGA: zmienia PID procesu, ustawia umask na 0000 !
 *
 */

#ifndef __DAEMON_H__
#define __DAEMON_H__

void go_daemon(void);

#endif
