//----------------------------------------------------------------------//
// KOYOMI.DLL用ユニット													//
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

	double Latitude;		//緯度
	double Longitude;		//経度

	unsigned short Year;	//対象年
	int DayN;				//1年に日数

	bool swShanichi;		//社日の決め方
							//	true:  春分、秋分の時刻を考慮/ 
							//	false: 直前 (default)

	KoyomiUnit();
	~KoyomiUnit();
	UnicodeString GetInfoStr(int y, int m, int d);
};
//---------------------------------------------------------------------------
#endif
