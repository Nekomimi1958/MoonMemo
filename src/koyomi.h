//----------------------------------------------------------------------//
// KOYOMI.DLL�p���j�b�g													//
//																		//
//----------------------------------------------------------------------//
#ifndef KoyomiUnitH
#define KoyomiUnitH

//---------------------------------------------------------------------------
typedef int   (WINAPI *KOYOMI_SetdataDll)(double, double, double, int, int, int, int*);
typedef char* (WINAPI *KOYOMI_RokuyoDll)(int*, int*, int*);
typedef char* (WINAPI *KOYOMI_EtoDll)(int, int*, int*);
typedef char* (WINAPI *KOYOMI_SekkiDll)(int*, double*, double*);
typedef int   (WINAPI *KOYOMI_KisetuDll)(int, double*);

//---------------------------------------------------------------------------
class KoyomiUnit
{
private:
	HMODULE hKOYOMI;
	KOYOMI_SetdataDll	Koyomi_Setdata;
	KOYOMI_RokuyoDll	Koyomi_Rokuyo;
	KOYOMI_EtoDll		Koyomi_Eto;
	KOYOMI_SekkiDll		Koyomi_Sekki;
	KOYOMI_KisetuDll	Koyomi_Kisetu;

	UnicodeString ErrBuf;

public:
	bool Available;
	UnicodeString ErrMsg;

	double Latitude;		//�ܓx
	double Longitude;		//�o�x

	unsigned short Year;	//�Ώ۔N
	int DayN;				//1�N�ɓ���

	bool swShanichi;		//�Г��̌��ߕ�
							//	true:  �t���A�H���̎������l��/ 
							//	false: ���O (default)

	KoyomiUnit();
	~KoyomiUnit();
	UnicodeString GetInfoStr(int y, int m, int d);
};
//---------------------------------------------------------------------------
#endif
