/* 
  libSzarp - SZARP library

*/
/*
 * $Id$
 */
#ifndef _MBRTU_H_
#define _MBRTU_H_

#include "modbus.h"

#define MAX_BODY_SIZE 255

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define IN_BUF 1
#define OUT_BUF 2
#define BOTH_BUF 3

/*
 * Parametry komunikacyjne 
 */
#define NO_PARITY 0
#define EVEN 1
#define ODD 2

/*
 * Kody bledow 
 */
#define CRC_ERROR -1
#define CRC_OK 1
/*
 * Kod b��d�w select-a
 */
#define GENERAL_ERROR -1
#define TIMEOUT_ERROR -2 /* B��d kiedy nie przyjd� dane */

/*
 * Maksymalna wieko�� bufora pakiet�w
 */
#define MAX_BUFFER_SIZE 1000

/*
 * Tablica m�wi�ca kt�re parametry w ramce MASTER s� parzyste
 */
#define NSF -1 //Znacznik Not Supported Function
#define PARITY 0
#define INPARITY 1

 /* Tablica jest indeksowana nuerami funkcji */
 
const  int  MasterParity [] = {NSF, //00
	  		    NSF, //01
			    NSF, //02
			    PARITY, //03
			    PARITY, //04
			    PARITY, //05	
  			    PARITY, //06
  			    NSF,  //07
  			    NSF,  //08
  			    NSF,  //09
  			    NSF,  //10
  			    NSF,  //11
                            NSF,  //12
  			    NSF,  //13
  			    NSF,  //14
  			    NSF,  //15
                            INPARITY}; //16

/*
 * Tablica m�wi�ca kt�re parametry w ramce SLAVE s� parzyste
 */
const  int  SlaveParity [] = {NSF, //00
	  		    NSF, //01
			    NSF, //02
			    INPARITY, //03
			    INPARITY, //04
			    INPARITY, //05	
			    INPARITY, //06
  			    NSF,  //07
  			    NSF,  //08
  			    NSF,  //09
  			    NSF,  //10
  			    NSF,  //11
                            NSF,  //12
  			    NSF,  //13
  			    NSF,  //14
  			    NSF,  //15
                            PARITY}; //16

extern	char            mbrtu_single;
/** Struktura zawieraj�ca budow� ramki master */
	typedef struct {
		unsigned char   DeviceId;
			 /**<Numer identyfikacyjny jednostki podrz�dnej*/
		unsigned char   FunctionId;
			   /**<Numer funkcji */
		unsigned short  Address;
			 /**<Adres rejestru do odczytu */
		unsigned short  Body[MAX_BODY_SIZE];
				      /**<Cialo pakietu w postaci gotowych rejestrow*/
		unsigned short  DataSize;
			  /**<Ilo�� przesy�anych s��w (S�owo jest dwubajtowe) */
	} tWMasterFrame;

/** Struktura zawieraj�ca budowe ramki slave */
	typedef struct {
		unsigned char   DeviceId;
			 /**<Numer identyfikacyjny jednostki podrzednej */
		unsigned char   FunctionId;
			   /**<Numer funkcji */
		unsigned short  DataSize;
			  /**<Ilosc przesylanych bajtow */
		unsigned short  Address;
			 /**<Adres rejestru do odczytu */
		unsigned short  Body[MAX_BODY_SIZE];
				      /**<Cialo pakietu w postaci gotowych rejestrow*/
	} tWSlaveFrame;

/** Struktura zawieraj�ca budow� odczytanej ramki MASTER */
	typedef struct {
		unsigned char   DeviceId;
			/**<Numer identyfikacyjny jednostki podrzednej */
		unsigned char   FunctionId;
			   /**<Numer funkcji */
		unsigned short  Address;
			 /**<Adres rejestru do odczytu*/
		unsigned short  DataSize;
			 /**<Wielko�� danych */
		unsigned short  Body[MAX_BODY_SIZE];
				     /**<Cialo pakietu w postaci gotowych rejestrow*/
		unsigned short  CRC;
		     /**<Suma kontrolna CRC*/
		unsigned short  PacketSize;
			    /**<Rzeczywista wielko�� pakietu*/
	} tRMasterFrame;

/** Struktura zawieraj�ca budow� odczytanej ramki SLAVE */
	typedef struct {
		unsigned char   DeviceId;
			/**<Numer identyfikacyjny jednostki podrzednej */
		unsigned char   FunctionId;
			   /**<Numer funkcji */
		unsigned short  Address;
			 /**<Adres rejestru do odczytu*/
		unsigned short  DataSize;
			  /**<Wielko�� danych */
		unsigned short  Body[MAX_BODY_SIZE];
				    /**<Cialo pakietu w postaci gotowych rejestrow*/
		unsigned short  CRC;
		    /**<Suma kontrolna CRC*/
		unsigned short  PacketSize;
			   /**<Rzeczywista wielko�� pakietu*/
	} tRSlaveFrame;

/** Struktura zawieraj�ca budow� ramki error */
	typedef struct {
		unsigned char   DeviceId;
			 /**<Numer identyfikacyjny jednostki podrz�dnej*/
		unsigned char   FunctionId;
			   /**<Numer funkcji */
		unsigned short  Exception;
			   /**<Kod b��du */
	} tWErrorFrame;

/**
 * Funkcja otwiera port szeregowy
 * @param Device Scie�ka do urz�dzenia np "/dev/ttyS0"
 * @param BaudRate Pr�dko��
 * @param StopBits bity stopu 1 albo 2
 * @param Parity Parzysto�� : NO_PARITY, ODD, EVEN 
 * @param return numer identyfikacyjny urzadzenia
 */
	int             InitComm(const char *Device, long BaudRate,
				 unsigned char DataBits,
				 unsigned char StopBits,
				 unsigned char Parity);

/**
 * Funkcja czy�ci bufory wej�cia/wyj�cia
 * @param CommId identyfikator portu
 * @param Mode tryb czyszczenia IN_BUF - bufor wej�cia, OUT_BUF - bufor wyj�cia, BOTH_BUF - obaq bufory
 */

	void            ClearPort(int CommId, int Mode);

/**
 * Funkcja Liczy sume CRC16 wg standart MODBUS-a 
 * @param Packet ca�y pakiet (dane)
 * @param PacketSize Ilo�� danych z kt�rych ma by� policzone CRC
 * @param return Warto�� CRC16
 */
	unsigned short  CRC16(unsigned char *Packet, int PacketSize);

/**
 * Funkcja konwertuje int na postac binarna
 * @param iint Wartosc int do konwersji
 * @param bMSB Bardziej znaczace slowo po konwersji na postac binarna
 * @param bLSB Mniej znaczace slowo po konwersji na postac binarna
 */

	void            int2bin(signed short iint, unsigned short *oBIN);

/**
 * Funkcja konwertuje postac binarna na int
 * @param iBIN warto�� binarna do konwersji na posta� int
 * @param oint Warto�� int po konwersji
 */
	void            bin2int(unsigned short iBIN, signed short *oint);

/**
 * Funkcja konwertuje float na postac binarna
 * @param ifloat Wartosc zmiennoprzecinkowa do konwersji
 * @param bMSB Bardziej znaczace slowo po konwersji na postac binarna
 * @param bLSB Mniej znaczace slowo po konwersji na postac binarna
 */


	void            float2bin(float ifloat, unsigned short *bMSB,
				  unsigned short *bLSB);

/**
 * Funkcja konwertuje postac binarna na float
 * @param bMSB Bardziej zn�czace s�owo do konwersji na posta� flo
 * @param bLSB Mniej znacz�ce s�owo do konwersji na posta� float
 * @param ofloat Warto�� zmiennoprzecinkowa po konwersji
 */
	void            bin2float(unsigned short bMSB, unsigned short bLSB,
				  float *ofloat);

/**
 * Funkcja konwertuje floata na inta z przecinkiem
 * @param ifloat float do konwersji
 * @param prec Precyzja int-a
 * @param return Przekonwertowana warto�� int
 */
	signed short    float2int(float ifloat, unsigned short prec);

/**
 * Funkcja konwertuje floata na inta4 z przecinkiem
 * * @param ifloat float do konwersji
 * @param prec Precyzja int-a
 * @param return Przekonwertowana warto�� int4
 */
	signed int  float2int4(float ifloat, unsigned short prec);

	
/**
 * Funkcja konwertuje inta z przecinkiem na floata
 * @param iint int do konwersji
 * @param prec Precyzja int-a
 * @param return Przekonwertowana warto�� float
 */
	float           int2float(signed short iint, unsigned short prec);

/**
 * Funkcja tworzy ramke typu MASTER
 * @param MasterFrame Struktura definiujaca parametry ramki
 * @param oPacket Pakiet przygotowany do wys�ania
 * @param MaxPacketSize Maksymalna wielko�� pakietu
 * @param PacketSize Wielko�� pakietu
 */
	void            CreateMasterPacket(tWMasterFrame MasterFrame,
					   unsigned char *oPacket,
					   int MaxPacketSize,
					   int *PacketSize);

/**
 * Funkcja dekoduje ramk� MASTER do bardziej czytelnej postaci
 * @param *iPacket wska�nik do bufora w kt�rym znajduje si� pakiet przychodz�cy
 * @param MaxPacketSize - Maksymalna wielko�� pakietu
 * @param *MasterFrame - Wska�nik do struktury zawieraj�cej zdekodowane dane
 */
	unsigned short  DecodeMasterPacket(unsigned char *iPacket,
					   int MaxPacketSize,
					   tRMasterFrame * MasterFrame);
/**
 * Funkcja tworzy ramk� typu SLAVE
 * @param SlaveFrame - struktura definiuj�ca parametry ramki
 * @param oPacket - Pakiet przygotowany do wys�ania
 * @param MaxPacketSize - Maksymalna wielko�� pakietu
 * @param PacketSize - Wielko�� pakietu
 */
	void            CreateSlavePacket(tWSlaveFrame SlaveFrame,
					  unsigned char *oPacket,
					  int MaxPacketSize, int *PacketSize);

/**
 * Funkcja tworzy ramk� typu SLAVE
 * @param ErrorFrame - struktura definiuj�ca parametry ramki
 * @param oPacket - Pakiet przygotowany do wys�ania
 * @param MaxPacketSize - Maksymalna wielko�� pakietu
 * @param PacketSize - Wielko�� pakietu
 */
	void            CreateErrorPacket(tWErrorFrame ErrorFrame,
					  unsigned char *oPacket,
					  int MaxPacketSize, int *PacketSize);

/**
 * Funkcja dekoduje ramk� SLAVE do bardziej czytelnej postaci
 * @param *iPacket wska�nik do bufora w kt�rym znajduje si� pakiet przychodz�cy
 * @param MaxPacketSize - Maksymalna wielko�� pakietu
 * @param *SlaveFrame - Wska�nik do struktury zawieraj�cej zdekodowane dane
 */

	unsigned short  DecodeSlavePacket(unsigned char *iPacket,
					  int MaxPacketSize,
					  tRSlaveFrame * SlaveFrame);


/**
 *  Funkcja wysy�aj�ca i odbieraj�ca dane z urz�dzenia master
 * @param CommId - numer identyfikacyjny urzadzenia 
 * @param iMasterFrame - Ramka do wyslania Master
 * @param return - kod b��du
 */
	int     SendMasterPacket(int CommId,
					 tWMasterFrame iMasterFrame);

/**
 *  Funkcja odbieraj�ca pakiet slave
 * @param CommId - numer identyfikacyjny urzadzenia 
 * @param iSlaveFrame - Ramka do wys�ania Slave
 * @param *CRCStatus - stan CRC CRC_OK | CRC_ERROR (moze byc NULL)
 * @param ReceiveTimeout - timeout dla select-a
 * @param DelayBetweenChars - opoznienie pomiedzy przychodzacymi znakami
 * @param return - Kod b��du
 */
	int     ReceiveSlavePacket(int CommId,
					   tRSlaveFrame * oSlaveFrame,
					   char *CRCStatus,
					 int ReceiveTimeout, 
					 int DelayBetweenChars);
/**
 *  Funkcja wysy�aj�ca pakiet slave do urz�dzenia master
 * @param CommId - numer identyfikacyjny urzadzenia 
 * @param iSlaveFrame - Ramka do wys�ania Slave
 * @param return - Kod b��du
 */
	int           SendSlavePacket(int CommId, tWSlaveFrame iSlaveFrame);

/**
 *  Funkcja wysy�aj�ca pakiet slave do urz�dzenia master
 * @param CommId - numer identyfikacyjny urzadzenia 
 * @param iSlaveFrame - Ramka do wys�ania Slave
 * @param ReceiveTimeout - timeout dla select-a
 * @param DelayBetweenChars - opoznienie pomiedzy przychodzacymi znakami
 * @param *CRCStatus - stan CRC CRC_OK | CRC_ERROR (moze byc NULL)
 * @param ReceiveTimeout - timeout dla select-a
 * @param DelayBetweenChars - opoznienie pomiedzy przychodzacymi znakami
 * @param return - Kod b��du
 */
	int     ReceiveMasterPacket(int CommId,
					    tRMasterFrame * oMasterFrame,
					    char *CRCStatus,
					    int ReceiveTimeout, 
					    int DelayBetweenChars);


/**
 *  Funkcja wysy�aj�ca pakiet error do urz�dzenia master
 * @param CommId - numer identyfikacyjny urzadzenia 
 * @param iErrorFrame - Ramka do wys�ania error
 */
	void            SendErrorPacket(int CommId, tWErrorFrame iErrorFrame);

/**
 * Funkcja zamyka po��czenie i przywraca dawne ustawienia
 * @param CommId - numer identyfikacyjny urzadzenia
*/
	void            CloseComm(int CommId);
#endif
