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
/*------------------------------------------------------------------------------*/

#include "szdefines.h"

#define NO_PARAM 0xFFFF /* probka nie jest zbierana */

/**
*Numery cykli analizy
*/

#define A_START    0  /*!< Pierwszy cykl analizy */

#define A_UP       1  /*!< Cykl zwi�kszania powietrza */
#define A_UP_RET   2  /*!< Cykl powrotu do warto�ci powietrza po cyklu A_UP */
#define A_DOWN     3  /*!< Cykl zmniejszania powietrza */
#define A_DOWN_RET 4  /*!< Cykl powrotu do warto�ci powietrza po cyklu A_DOWN */

#define A_STOP  5 /*!< Cykl przerywaj�cy analiz� , po otrzymaniu braku zezwolenia od kot�a */

#define A_NUMBER_OF_CYCLES 4 /*!< Ilo�� cykli w analizie */


/*------------------------------------------------------------------------------*/


#define CFG_FILENAME "analiza.cfg"  /*! Nazwa pliku konfiguracyjnego */

/*------------------------------------------------------------------------------*/

/**
* Nazwy sekcji dost�pnych w pliku konfiguracyjnym dla poszczeg�lnych kot��w
*/

#define KOCIOL_1 "KOCIOL_1"
#define KOCIOL_2 "KOCIOL_2"
#define KOCIOL_3 "KOCIOL_3"
#define KOCIOL_4 "KOCIOL_4"
#define KOCIOL_5 "KOCIOL_5"
#define KOCIOL_6 "KOCIOL_6"

/**
* Dane odczytywane z pliku konfiguracyjnego w poszczeg�lnych sekcjach
*/

#define NUMBER_OF_PARAMS 14 /*!< Ilosc odczytywanych parametrow z pliku konfiguracyjnego dla jednego regulatora kot�a*/

#define INPAR_WRTYPE "InData.WRtype" /*!< Typ kotla: WR-25 = 0  , WR-10 = 1  , WR-5 = 2 , WR-2,5 = 3*/

#define INPAR_1     "InData.a_enable" /*!< Zezwolenie od regulatora kot�a na analiz� */

#define INPAR_2     "InData.work_timer_h"  /*!< Czas pracy regulatora kot�a w pracy automatycznej */

#define INPAR_3     "InData.Vc"            /*!< Aktualna obj�to�� wchodz�cego do kot�a w�gla */

#define INPAR_4     "InData.En_Vc"         /*!< Aktualna warto�� stosunku energia / objeto�� */

#define INPAR_5     "InData.Fp_4m3"        /*!< Warto�� (wzgl�dna) strumienia powietrza dla ilo�ci w�gla zale�nej od typu kot�a (dla kot�a WR-25 b�dzie to 4m3 w�gla,
																						* dla WR-10 - 1.6m3 , WR-5 - 0.8m3, WR-2.5 - 0.4m3, itp.)
																						*/

#define INPAR_6     "InData.Vr_l"        /*!< Aktualna pr�dko�� posuwu lewego rusztu	*/
#define INPAR_7     "InData.Vr_p"        /*!< Aktualna pr�dko�� posuwu prawego rusztu	*/
#define INPAR_8     "InData.Min_Fp_4m3"        /*!< Minimalna warto�� (wzgl�dna) strumienia powietrza	*/
#define INPAR_9     "InData.Max_Fp_4m3"        /*!< Maksymalna warto�� (wzgl�dna) strumienia powietrza	*/

#define INPAR_10     "InData.Hw_l"        /*!< Wysoko�� lewej warstwownicy */
#define INPAR_11     "InData.Hw_p"        /*!< Wysoko�� prawej warstwownicy */

#define NUMBER_OF_OUTPAR 2

#define OUTPAR_1    "OutData.Fp_4m3"       /*!< Wyj�ciowa warto�� (wzgl�dna) strumienia powietrza dla ilo�ci w�gla zale�nej od typu kot�a (dla kot�a WR-25 b�dzie to 4m3 w�gla,
																						* dla WR-10 - 1.6m3 , WR-5 - 0.8m3, WR-2.5 - 0.4m3, itp.)
																						*/

#define OUTPAR_2    "OutData.AnalyseStatus" /* !<status analizy - czy aktualnie jest wykonywana */


#define NUMBER_OF_VR_PART_PARMAS 13 /*!< Liczba odczytanych parametrow z pliku konfiguracyjnego dotyczacych parametrow przedzialow predkosci rusztu wyznaczajacych dlugosc cyklu analizy*/

#define NUMBER_OF_FLOAT_PARAMS 2 /*!< Liczba odczytanych parametrow z pliku konfiguracyjnego typu float*/

#define INPAR_D_VR  "InData.d_Vr" /*!< granica g�rna zmian pr�dko�ci rusztu podczas jednego cyklu analizy  */
#define INPAR_D_HW  "InData.d_Hw" /*!< granica g�rna zmian wysoko�ci warstownicy podczas jednego cyklu analizy */

/* Przedzial I */

#define INPAR_PART_I_1  "InData.Vr_PartI_Low" /*!< granica dolna przedzialu - predkosc rusztu */
#define INPAR_PART_I_2  "InData.Vr_PartI_Hi" /*!< granica gorna przedzialu - predkosc rusztu */
#define INPAR_PART_I_3  "InData.Vr_PartI_Duration" /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */

/* Przedzial II */

#define INPAR_PART_II_1 "InData.Vr_PartII_Low" /*!< granica dolna przedzialu - predkosc rusztu */
#define INPAR_PART_II_2 "InData.Vr_PartII_Hi"  /*!< granica gorna przedzialu - predkosc rusztu */
#define INPAR_PART_II_3 "InData.Vr_PartII_Duration" /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */


/* Przedzial III */

#define INPAR_PART_III_1 "InData.Vr_PartIII_Low" /*!< granica dolna przedzialu - predkosc rusztu */
#define INPAR_PART_III_2 "InData.Vr_PartIII_Hi"  /*!< granica gorna przedzialu - predkosc rusztu */
#define INPAR_PART_III_3 "InData.Vr_PartIII_Duration" /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */

/*-----------------------------------------------------------------------------*/

/**  Zmiany powietrza w analizie */

#define CHANGE_PERCENTS 10 /*!< Zmiana powietrza w punktach ( ilo�� punkt�w * 10) po kolejnych cyklach */

#define AIR_CHANGE 5 /*!< Dodawana/odejmowana cze�� powietrza po ka�dym cyklu analizy, w zale�no�ci od wyniku analizy */

/*------------------------------------------------------------------------------*/

/** Granice przedzia�u warto�ci strumienia powietrza, w kt�rych dokonywana jest analiza */

#define MIN_AIR 50 /*!< Minimalna warto�� strumienia powietrza*/

#define MAX_AIR 1500 /*!< Maksymalna warto�� strumienia powietrza*/

/*------------------------------------------------------------------------------*/
/** Liczniki wyznaczaj�ce stany pracy programu */

#define TIMER_1M    180 /*!< licznik w sekundach wyznaczj�cy cz�stotliwo�� odczytywania danych przed rozpocz�ciem analizy*/

/*#define TIMER_09H  10 */ /*!< licznik w sekundach - cz�stotliwo�� czytania danych z wyprzedzeniem, w razie braku komunikacji*/
/* #define TIMER_1H   20 */ /*!< licznik w sekundach - cz�stotlowo�� wykonywania cykli analizy*/
/* #define TIMER_11H  30 */ /*!< licznik w sekundach - cz�stotliwo�� czytania danych z op�nieniem, w razie braku komunikacji */

#define TIMER_1H  3600 /* licznik w sekundach */
#define TIMER_2H  7200 /* licznik w sekundach */
#define TIMER_2_4H   8040/* licznik w sekundach */
#define TIMER_3H  10800 /* licznik w sekundach */

/*------------------------------------------------------------------------------*/
/**
* Warto�ci dla logiki dwuwarto�ciowej
*/
#define YES  1
#define NO   0

struct phPTT
 {
  FILE *file;			/* plik PTT */
  unsigned short rev;			/* revision code */
  unsigned short blen;			/* ilosc parametrow dla bazy danych */
  unsigned short len;			/* pelna ilosc parametrow */
  unsigned short *tab;			/* tablica PTT */
 };

typedef struct phPTT tPTT;

/*------------------------------------------------------------------------------*/
/**
* Struktura przechowywuj�ca dane odczytywane z raportu kot�a
*/
typedef struct _RapData
 {
  unsigned char type;   /*!< Typ odczytywanych pr�bek: 0=Probe, 1=Minute, 2=10Minutes, 3=Hour, 4=Day */
  unsigned char lines;  /*!< Pojemnosc bufora pr�bek */
  unsigned short addr[90]; /*!< Adresy pr�bek w pami�ci dzielonej */
  short  a_enable ; /*!< Zezwolenie na analiz�*/
  short  work_timer_h; /*!< Ilo�� godzin automatycznej pracy kot�a  */
  short  prev_work_timer_h ; /*!< Ilo�� godzin automatycznej pracy kot�a w poprzednim cyklu */
  short  Vc; /*!< Obj�to�� wchodz�cego w�gla*/
  short  En_Vc ; /*!< Stosunek energia / obj�to�� */
  short  En_Vc_Probe ; /*!< Stosunek energia / obj�to�� */
  short  Fp_4m3; /*!< Strumie� powietrza */
  short  Vr ; /*!< Pr�dko�� posuwu rusztu*/
  short  Vr_l ; /*!< Pr�dko�� posuwu lewego rusztu*/
  short  Vr_p ; /*!< Pr�dko�� posuwu prawego rusztu*/
  short  MinVr ; /*!< Minimalna predkosc rusztu*/
  short  MaxVr ; /*!< Maksymalna predkosc rusztu*/
  short  Hw_l ; /*!< Wysokosc lewej warstwownicy */
  short  Hw_p ; /*!< Wysokosc prawej warstwownicy */
  short  Hw ; /*!< Usredniona wysokosc  warstwownicy */
  short  MinHw ; /*!< Minimalna wysokosc warstwownicy*/
  short  MaxHw ; /*!< Maksymalna wysokosc warstwownicy*/
  short  Min_Fp_4m3 ;         /*!< Minimalna warto�� (wzgl�dna) strumienia powietrza	*/
  short  Max_Fp_4m3 ;        /*!< Maksymalna warto�� (wzgl�dna) strumienia powietrza	*/
  int good_data; /*!< Czy otrzymane dane s� poprawne tzn. r�zne od NO_DATA ( YES ,NO )*/
  int NumberOfNO_DATA ; /*!< Ilo�� kolenjnych cykl 10-o minutowych w ktorych nie bylo komunikacji ze sterownikiem kotla*/
  int WRtype ;      /*!< Typ kot�a WR */
  int set_NO_DATA;  /*!< Otrzymane dane maja wartosc NO_DATA */
 } TRapData;

/*------------------------------------------------------------------------------*/

TRapData RapData; /*!< Zmienna z�o�ona b�d�ca struktur� przechowywuj�c� dane odczytywane z raportu kot�a */

#define NUMBER_OF_SAMPLES 5 /*!< Maksymalna ilo�c pr�bek w buforze na pr�bki otrzymane od kot�a */
#define NUMBER_OF_PROBES  180 /*!< Maksymalna ilosc pr�bek w buforze na pr�bki ( predkosc rusztu ) otrzymane od kotla */
short Vr_probes[NUMBER_OF_PARAMS] ; /*!< Bufor na pr�bki ( pr�dkosc rusztu) otrzymane od kolta */
short Samples[NUMBER_OF_PARAMS] ; /*!< Bufor na pr�bki chwilowe otrzymywane od regulatora kot�a */
short HSamples[NUMBER_OF_PARAMS] ; /*!< Bufor na pr�bki godzinowe otrzymywane od regulatora kot�a */


/*------------------------------------------------------------------------------*/

/**
* Struktura przechowywuj�ca dane zapisywane do pami�ci dzielonej i wysy�ane przez sendera do regulatora kot�a
*/

typedef struct SendData
 {
  short  Fp_4m3;            /*!< Wyj�ciowy strumie� powietrza w analizie */
  short AnalyseStatus ;     /*!< Status analizy - WORKING , STOPED */
 } TSendData;

/* Status analizy -  pracuje , zatrzymana */

#define WORKING  1

#define STOPED  0



/*------------------------------------------------------------------------------*/

TSendData SendData; /*!< Zmienna z�o�ona b�d�c� struktur� przechowywuj�c� dane zapisywane do pami�ci dzielonej i wysy�ane przez sendera do regulatora kot�a */

/*------------------------------------------------------------------------------*/

/** Ilo�� ostatnich odczytanych pr�bek buforze */
#define A_BUFFER_SIZE 3

/** Struktura przechowuj�ca kilka ostatnich (ilo�� okre�la A_BUFFER_SIZE) pr�bek */
typedef struct tActSamples
{
  int Vc[A_BUFFER_SIZE] ;      /*!< Pr�bki obj�to�ci w�gla */
  int En_Vc[A_BUFFER_SIZE] ;   /*!< Wsp�czynnik: energia/obj�to�� w�gla */
  int Fp_4m3[A_BUFFER_SIZE] ;  /*!< Ilo�� powietrza podmuchowego dla ilo�� w�gla zale�nej od typu kot�a */
  int Pointer ;                /*!< Aktualny stan zape�nienia bufora pr�bkami */

} TActSamples;


/*------------------------------------------------------------------------------*/

/** Definicja struktury przechowuj�cej dane wyliczeniowe, tymczasowe , itp. niezb�dne w procesie analizy*/
typedef struct tAnalysisData
{
int a_cycle ; /*!< Numer cyklu analizy */
long a_timer ; /*!< Inkrementowany licznik wyznaczj�cy kolejny cykl analizy*/
long prev_tv;  /*!< Zmienna pomocnicza dla wyliczania licznika wyznaczaj�cego cyklu analizy */
int a_dir ;    /*!< Kierunek analizy */
int a_Fp_4m3 ; /*!< Strumie� powietrza wyznaczany w procesie analizy */
int last_Fp_4m3 ; /*!< Strumie� powietrza przez zmianami  */
int global_timer ; /*!< Licznik licz�cy liczb� dotychczasowych cykl analiz */
int log ;          /*!< Czy zapisywa� przebieg analizy do pliku */
int NumberOfAnalyses;/*!< Zezwolenie na por�wnywanie efekt�w z kolejnych cykli analizy (YES,NO) */
FILE* LogFile; /*!< Uchwyt do pliku z logami*/
int confirmed ; /*!< Potwierdzenie otrzymanych danych od sterownika */
int restore  ;  /*!< Czy przywr�ci� powietrze po przerwaniu analizy ( YES, NO ) */
char LogFileName[100]; /*!< Nazwa pliku logu */
int Period  ; /* */ 
int VrPartition  ;
int DoNotSendData ;	     	

int VrPartILow ; /*!< granica dolna przedzialu - predkosc rusztu */
int VrPartIHi ; /*!< granica gorna przedzialu - predkosc rusztu */
int VrPartIDuration ; /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */

int VrPartIILow ; /*!< granica dolna przedzialu - predkosc rusztu */
int VrPartIIHi ; /*!< granica gorna przedzialu - predkosc rusztu */
int VrPartIIDuration ; /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */

int VrPartIIILow ; /*!< granica dolna przedzialu - predkosc rusztu */
int VrPartIIIHi ; /*!< granica gorna przedzialu - predkosc rusztu */
int VrPartIIIDuration ; /*!< czas przeprowadzania analizy dla tego przedzialu w sek. */

float d_Vr  ; /*!< granica g�rna zmian pr�dko�ci rusztu podczas jednego cyklu analizy */
float d_Hw  ; /*!< granica g�rna zmian wysoko�ci warstownicy podczas jednego cyklu analizy */

} TAnalysisData ;


TActSamples ActSamples; /*!< Zmienna z�o�ona b�d�ca strukrur� przechowywuj�ca dane wyliczeniowe, tymczasowe, niezb�dne w procesie analizy */

TAnalysisData  AnalysisData; /*!< Zmienna z�o�ona b�d�ca struktur� przechowuj�c� dane wyliczeniowe, tymczasowe , itp. niezb�dne w procesie analizy */

int vr_part_params[NUMBER_OF_VR_PART_PARMAS] ;

int float_params[NUMBER_OF_FLOAT_PARAMS] ;

tPTT PTTact={NULL,0,0,0,NULL};

struct tm tm_loctime ;
/*------------------------------------------------------------------------------*/

int fault_cyc(int a_cycle);

void check_conditionals();

int check_effect();

void analyse_effect();

void analyse();

void save_sample() ; /* funkcja zapisujaca probke dla bufora probek - struktura TSamples */

void save_RapData();

void  init_ActSamples() ;

int AverageEn_Vc(int Start,int Range,int index,int counter) ;

void  Init_RapData(int params[], int counter,bool ipk = false);

void Init_WriteData(int val1);

void open_log(char *filename) ;

void close_log() ;

int read_data();

