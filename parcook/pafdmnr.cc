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
 * SZARP 2.0
 * parcook
 * pafdmnr.c - wersja pafdmn.c wsp�pracuj�ca z licznikiem
 * z Rawicza
 */

#define _IPCTOOLS_H_

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/uio.h>
#include<sys/time.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<time.h>
#include<termio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<libgen.h>
#include<math.h>
#include "libpar.h"
#include "msgerror.h"
#include "ipcdefines.h"

// #define WRITE_MESSAGE_SIZE 7
#define WRITE_MESSAGE_SIZE 11

#define READ_MESSAGE_SIZE 256

#define NUMBER_OF_VALS 9

// offset danych
#define OF -1

/*
 * 1 + 1 + 4 + 3 + 4 + 1 + 1
 */

static int      LineDes;

static int      LineNum;

static int      SemDes;

static int      ShmDes;

static unsigned char Diagno = 0;

static unsigned char Simple = 0;

static int      VTlen;

static short   *ValTab;

static struct sembuf Sem[2];

static char     parcookpat[81];

static char     linedmnpat[81];

#define SIZE_OF_BUF 8

// unsigned char outbuf[WRITE_MESSAGE_SIZE] = { '?' , '/', '?', '!', '\r', 
// '\n', '?'} ;
unsigned char   outbuf[WRITE_MESSAGE_SIZE] =
    { '/', '?', '!', '\r', '\n', 6, '1', '0', 'A', '\r', '\n' };

int             buf[5];

void
Initialize(unsigned char linenum)
{
    libpar_init();
    libpar_readpar("", "parcook_path", parcookpat, sizeof(parcookpat), 1);
    libpar_readpar("", "linex_cfg", linedmnpat, sizeof(linedmnpat), 1);
    libpar_done();

    VTlen = NUMBER_OF_VALS;
    if (Diagno)
	printf("Cooking deamon for line %d - Pafal 2EC8 deamon\n",
	       linenum);
    if (Diagno)
	printf
	    ("Unit 1: no code/rapid/subid, parameters: in=2, no out, period=1\n");
    if (Simple)
	return;
    if ((ShmDes =
	 shmget(ftok(linedmnpat, linenum - 1), sizeof(short) * VTlen,
		00600)) < 0)
	ErrMessage(3, "linedmn");
    if ((SemDes =
	 semget(ftok(parcookpat, SEM_PARCOOK), SEM_LINE + 2 * linenum, 00600)) < 0)
	ErrMessage(5, "parcook sem");
}


static int
OpenLine(char *line)
{
    int             linedes;
    int 	    serial;
    struct termios   rsconf;
   linedes = open(line, O_RDWR | O_NDELAY | O_NONBLOCK);

    if (linedes < 0)
	return (-1);

    	tcgetattr(linedes, &rsconf);
	rsconf.c_cflag = B300 | CS7 | CLOCAL | CREAD | CSTOPB | PARENB;
	rsconf.c_iflag = 0;
	rsconf.c_oflag = 0;
	rsconf.c_lflag = 0;
	rsconf.c_cc[VMIN] = 10;
	rsconf.c_cc[VTIME] = 0;
	tcsetattr(linedes, TCSANOW, &rsconf);
	ioctl(linedes, TIOCMGET, &serial);
	serial |= TIOCM_DTR; //DTR is LOW
	serial |= TIOCM_RTS; //RTS is LOW
	ioctl(linedes, TIOCMSET, &serial);
    return (linedes);
}

/*
 * Ustawia zawartosc wiadomosci wysylanej do licznika
 */
/*
 * buf - bufor z 
 */
unsigned char
Suma(unsigned char *tab, int count)
{

    int             i;
    unsigned char   sum;
    sum = 0;
    for (i = 0; i < count; i++) {

	sum += tab[i];
    }
    sum = sum % 256;
    if (sum < 0x20)
	sum += 0x20;
    return sum;
}

/*
 * Umieszczenie w buforze buf danych z licznika energii zawartych w
 * komunikacie inbuf
 */
void
PartitionMessage(unsigned char *inbuf, unsigned short lenght, int *buf,
		 unsigned short *size)
{

    int             i;
    unsigned char   tempbuf[7];
    int             sign1;
    int             sign2;
    unsigned char   comma[3];

    int             PValue;
    int             QValue;

    int             IValue1;
    int             IValue2;
    int             IValue3;

    int             UValue1;
    int             UValue2;
    int             UValue3;

    int             FValue;

    int             value;

    PValue = 0;
    QValue = 0;

    IValue1 = 0;
    IValue2 = 0;
    IValue3 = 0;

    UValue1 = 0;
    UValue2 = 0;
    UValue3 = 0;

    FValue = 0;

    sign1 = inbuf[OF+168] - 48;
    sign2 = inbuf[OF+169] - 48;
    //sign1 = inbuf[OF+168];
    //sign2 = inbuf[OF+169];
    if (Diagno) {
	    printf("SIGN: %x %x\n", inbuf[OF+168], inbuf[OF+169]);
    }

    // Obliczanie mocy czynnej

    for (i = 0; i <= 16; i += 8) {

	tempbuf[0] = inbuf[OF+46 + i];
	tempbuf[1] = inbuf[OF+47 + i];
	tempbuf[2] = inbuf[OF+44 + i];
	tempbuf[3] = inbuf[OF+45 + i],
	tempbuf[4] = inbuf[OF+42 + i];
	tempbuf[5] = inbuf[OF+43 + i];
	tempbuf[6] = '\0';

	comma[0] = inbuf[OF+48 + i];
	comma[1] = inbuf[OF+49 + i];
	comma[2] = '\0';

	value = atoi((char*)tempbuf);

	if (i == 0) {
	    if ((sign1 & 8) == 8) {
		value = -value;
	    }
	}
	if (i == 8) {
	    if ((sign1 & 4) == 4)
		value = -value;
	}
	if (i == 16) {
	    if ((sign1 & 2) == 2)
		value = -value;
	}

	if (Diagno)
	    printf("MOC (%d): %s\n", i / 8, tempbuf);

	if (strcmp((char*)comma, "3F") == 0)
	    value = value / 10;
	if (strcmp((char*)comma, "3E") == 0)
	    value = value / 100;
	if (strcmp((char*)comma, "3D") == 0)
	    value = value / 1000;
	if (strcmp((char*)comma, "3C") == 0)
	    value = value / 10000;

	PValue += value;
    }

    // Obliczanie mocy biernej

    for (i = 0; i <= 16; i += 8) {

	tempbuf[0] = inbuf[OF+75 + i];
	tempbuf[1] = inbuf[OF+76 + i];
	tempbuf[2] = inbuf[OF+73 + i];
	tempbuf[3] = inbuf[OF+74 + i],
	tempbuf[4] = inbuf[OF+71 + i];
	tempbuf[5] = inbuf[OF+72 + i];
	tempbuf[6] = '\0';

	comma[0] = inbuf[OF+77 + i];
	comma[1] = inbuf[OF+78 + i];
	comma[2] = '\0';

	value = atoi((char*)tempbuf);

	if (i == 0) {
	    if ((sign2 & 8) == 8)
		value = -value;
	}
	if (i == 8) {
	    if ((sign2 & 4) == 4)
		value = -value;
	}
	if (i == 16) {
	    if ((sign2 & 2) == 2)
		value = -value;
	}

	if (Diagno)
	    printf("MOC BIERNA (%d): %s\n", i/8, tempbuf);

	if (strcmp((char*)comma, "3F") == 0)
	    value = value / 10;
	if (strcmp((char*)comma, "3E") == 0)
	    value = value / 100;
	if (strcmp((char*)comma, "3D") == 0)
	    value = value / 1000;
	if (strcmp((char*)comma, "3C") == 0)
	    value = value / 10000;

	QValue += value;
    }

    // Obliczanie napiecia

    for (i = 0; i <= 16; i += 8) {

	tempbuf[0] = inbuf[OF+104 + i];
	tempbuf[1] = inbuf[OF+105 + i];
	tempbuf[2] = inbuf[OF+102 + i];
	tempbuf[3] = inbuf[OF+103 + i],
	tempbuf[4] = inbuf[OF+100 + i];
	tempbuf[5] = inbuf[OF+101 + i];
	tempbuf[6] = '\0';

	comma[0] = inbuf[OF+106 + i];
	comma[1] = inbuf[OF+107 + i];
	comma[2] = '\0';

	value = atoi((char*)tempbuf);

	if (Diagno)
	    printf("V (%d): %s\n", i/8, tempbuf);

	if (strcmp((char*)comma, "3F") == 0)
	    value = value / 10;
	if (strcmp((char*)comma, "3E") == 0)
	    value = value / 100;
	if (strcmp((char*)comma, "3D") == 0)
	    value = value / 1000;
	if (strcmp((char*)comma, "3C") == 0)
	    value = value / 10000;

	if (i == 0)
	    UValue1 = value;
	if (i == 8)
	    UValue2 = value;
	if (i == 16)
	    UValue3 = value;
    }

    // Obliczanie pradu

    for (i = 0; i <= 16; i += 8) {

	tempbuf[0] = inbuf[OF+133 + i];
	tempbuf[1] = inbuf[OF+134 + i];
	tempbuf[2] = inbuf[OF+131 + i];
	tempbuf[3] = inbuf[OF+132 + i],
    	tempbuf[4] = inbuf[OF+129 + i];
	tempbuf[5] = inbuf[OF+130 + i];
	tempbuf[6] = '\0';

	comma[0] = inbuf[OF+135 + i];
	comma[1] = inbuf[OF+136 + i];
	comma[2] = '\0';

	value = atoi((char*)tempbuf);

	if (Diagno)
	    printf("I (%d): %s\n", i/8, tempbuf);

	if (strcmp((char*)comma, "3E") == 0)
	    value = value / 10;
	if (strcmp((char*)comma, "3D") == 0)
	    value = value / 100;
	if (strcmp((char*)comma, "3C") == 0)
	    value = value / 1000;

	if (i == 0)
	    IValue1 = value;
	if (i == 8)
	    IValue2 = value;
	if (i == 16)
	    IValue3 = value;
    }

    // Obliczanie fazy


    tempbuf[0] = inbuf[OF+162];
    tempbuf[1] = inbuf[OF+163];
    tempbuf[2] = inbuf[OF+160];
    tempbuf[3] = inbuf[OF+161],
    tempbuf[4] = inbuf[OF+158];
    tempbuf[5] = inbuf[OF+159];
    tempbuf[6] = '\0';

    comma[0] = inbuf[OF+164 + i];
    comma[1] = inbuf[OF+165 + i];
    comma[2] = '\0';

    value = atoi((char*)tempbuf);

    if (Diagno)
	printf("Fi (%d): %s\n", i/8, tempbuf);

    if (strcmp((char*)comma, "3F") == 0)
	value = value / 10;
    if (strcmp((char*)comma, "3E") == 0)
	value = value / 100;
    if (strcmp((char*)comma, "3D") == 0)
	value = value / 1000;
    if (strcmp((char*)comma, "3C") == 0)
	value = value / 10000;

    FValue = value;

    buf[0] = PValue;
    buf[1] = QValue;
    buf[2] = UValue1;
    buf[3] = UValue2;
    buf[4] = UValue3;
    buf[5] = IValue1;
    buf[6] = IValue2;
    buf[7] = IValue3;
    buf[8] = FValue;

    if (Diagno)
	printf(" %d,%d,%d,%d,%d,%d,%d,%d,%d\n", buf[0], buf[1], buf[2],
	       buf[3], buf[4], buf[5], buf[6], buf[7], buf[8]);
    *size = 8;
}

 /*
  * zwraca dlugosc wczytanej komendy 
  */
int
ReadOutput(int sock, int *buf, unsigned short *size)
{
    int             i,
                    k,
                    j;
    unsigned short  lenght;
    unsigned char   inbuf[READ_MESSAGE_SIZE];
    unsigned char   test[6] = { 6, '1', '0', 'A', '\r', '\n' };
    int retval;
    fd_set rfds;
    struct timeval tv;
      
    /*
     * ustalenie dok�adnej postaci ramki - ilo�� odczytywanych rejestr�w
     * (number) oraz adres pocz�tkowego rejestru odczytywanych danych
     */
    errno = 0;
    lenght = 180 + OF;
    memset(inbuf, 0, READ_MESSAGE_SIZE);
    /*
     * wys�anie komunikatu do urz�dzenia 
     */
    i = write(sock, outbuf, WRITE_MESSAGE_SIZE);
    if (Diagno)
	printf("frame %x%x%x%x%x%x%x%x%x%x%x was sended \n", outbuf[0],
	       outbuf[1], outbuf[2], outbuf[3], outbuf[4], outbuf[5],
	       outbuf[6], outbuf[7], outbuf[8], outbuf[9], outbuf[10]);
    if (i < 0)
	return -1;
    /*
     * d�ugo�� odczytywanej ramki - wynika z tego jak� wysy�amy ramk� do
     * licznika 
     */
    /*
     * odczyt odpowiedzi urz�dzenia 
     */
	usleep (10*1000);
    k = 0;
    i = 0;
    
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    retval = select(sock+1, &rfds, NULL, NULL, &tv);
  
    if (retval == -1){
	  perror("select()");
	  return -1 ;
    }
 	else
    if (retval) {
//	    i = read(sock, inbuf, 20);	 
	   for (i = 0; read(sock, &inbuf[i], 1) == 1; i++) {
		   usleep(14*1000);
	   }
	   i--;

    }
      else return -1;
        
    if (i != 20) {
	if (Diagno)
    printf("Error %d bytes was received!\n", i);
	return -1;
    }
    
    for (j = 0; j < i; j++)
	if (Diagno)
	    printf(" %c", inbuf[j]);
    if (Diagno)
	printf(" was received \n");
	usleep(100000);
	sleep(2) ; 

	
	
    i = write(sock, test, 6);
    if (i < 0)
	return -1;
    if (Diagno)
	printf("frame %d%c%c%c%c%c was sended \n", test[0], test[1],
	       test[2], test[3], test[4], test[5]);
    k = 0;
    i = 0;
//  sleep(15);
    sleep(6);
    retval = select(sock+1, &rfds, NULL, NULL, &tv);
  
    if (retval == -1){
	  perror("select()");
	  return -1 ;
    }
 	else
    if (retval){
	   for (i = 1; read(sock, &inbuf[i], 1) == 1; i++) {
		   usleep(14*1000);
	   }
	
//	    i = read(sock, inbuf, lenght);
	if (Diagno){	    
	    printf("Received data size :%d; Actual Data size: %d\n",i,lenght);
	}
    }
      else {
	      i = 0;
      }
   
  //  i = read(sock, inbuf, lenght);

    for (j = 0; j <= i; j++)
	if (Diagno)
	    printf("%c", inbuf[j]);
    if (Diagno)
	printf(" was received , %d \n", i);
    if (i != lenght) {
	if (Diagno)
	    printf("Error  %d bytes  was received (shold be %d)\n", i,lenght);
	return -1;
    }
    PartitionMessage(inbuf, i, buf, size);
    // if (errno) return -1;
    return i;
}


void
ReadData(char *linedev)
{
    int             vals[NUMBER_OF_VALS];
    unsigned short           size;
    int             i,
                    j,
                    res;

    vals[0] = vals[1] = vals[2] = vals[3] = vals[4] = SZARP_NO_DATA;

    j = 0;
    res = -1;

    while (j < 5 && res < 0) {

	if (ReadOutput(LineDes, vals, &size) > 0)
	{
	    res = 1;
	    if (Diagno)
		printf("Odczyt danych: ");
	    if (Diagno)
		printf
		    ("vals[0] = %d , vals[1] = %d , vals[2] = %d , vals[3] = %d, vals[4] = %d, vals[5] = %d, vals[6] = %d, vals[7] = %d, vals[8] = %d\n",
		     vals[0], vals[1], vals[2], vals[3], vals[4], vals[5],
		     vals[6], vals[7], vals[8]);
	}

	else {
	    vals[0] = vals[1] = vals[2] = vals[3] = vals[4] = vals[5] =
		vals[6] = vals[7] = vals[8] = SZARP_NO_DATA;
	    if (Diagno)
		printf
		    ("Brak danych: vals[0] = vals[1] = vals[2] = vals[3] = vals[4] = SZARP_NO_DATA \n");
	}
	sleep(2);
	j++;
    }



    /*
     * UWAGA - usrednianie 
     */
    /*
     * oryginalnie wszystko fajnie pod warunkiem, ze sterownik nie
     * przesyla dla jakiegokolwiek parametru wartosci SZARP_NO_DATA - jesli tak, 
     * to trzeba trzymac takze tablice SumCnt dla kazdego parametru 
     */
    if (Simple)
	return;
    Sem[0].sem_num = SEM_LINE + 2 * (LineNum - 1) + 1;
    Sem[0].sem_op = 1;
    Sem[1].sem_num = SEM_LINE + 2 * (LineNum - 1);
    Sem[1].sem_op = 0;
    Sem[0].sem_flg = Sem[1].sem_flg = SEM_UNDO;
    semop(SemDes, Sem, 2);
    for (i = 0; i < VTlen; i++)
	ValTab[i] = (short) vals[i];
    Sem[0].sem_num = SEM_LINE + 2 * (LineNum - 1) + 1;
    Sem[0].sem_op = -1;
    Sem[0].sem_flg = SEM_UNDO;
    semop(SemDes, Sem, 1);
}

void
Usage(char *name)
{
    printf
	("Pafal 2EC8 driver; can be used on both COM and Specialix ports\n");
    printf("Usage:\n");
    printf("      %s [s|d] <n> <port>\n", name);
    printf("Where:\n");
    printf("[s|d] - optional simple (s) or diagnostic (d) modes\n");
    printf("<n> - line number (necessary to read line<n>.cfg file)\n");
    printf("<port> - full port name, eg. /dev/term/01 or /dev/term/a12\n");
    printf("Comments:\n");
    printf("No parameters are taken from line<n>.cfg (base period = 1)\n");
    printf
	("Port parameters: 300 baud, 7E1; signals required: TxD, RxD, GND\n");
    exit(1);
}


int
main(int argc, char *argv[])
{
    unsigned char   parind;
    char            linedev[30];

    libpar_read_cmdline(&argc, argv);
    if (argc < 3)
	Usage(argv[0]);
    Diagno = (argv[1][0] == 'd');
    Simple = (argv[1][0] == 's');
    if (Simple)
	Diagno = 1;
    if (Diagno)
	parind = 2;
    else
	parind = 1;
    if (argc < (int) parind + 2)
	Usage(argv[0]);
    LineNum = atoi(argv[parind]);
    strcpy(linedev, argv[parind + 1]);
    Initialize(LineNum);
    LineNum--;
    if (!Simple) {
	if ((ValTab =
	     (short *) shmat(ShmDes, (void *) 0, 0)) == (void *) -1) {
	    printf("Can not attach shared segment\n");
	    exit(-1);
	}
    }

    while (1) {
    	if ((LineDes = OpenLine(linedev)) < 0) {
		ErrMessage(2, linedev);
		perror("");
		exit(1);
    	}
	ReadData(linedev);
	sleep(5);
        close(LineDes);	
    }

    return 0;
}

