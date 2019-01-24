//----------------------------------------------------------------------//
// KOYOMI.DLL—pƒ†ƒjƒbƒg													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include "UserFunc.h"
#include "koyomi.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
KoyomiUnit::KoyomiUnit()
{
	Available = false;
	ErrMsg	  = EmptyStr;

	try {
		hKOYOMI = ::LoadLibrary(_T("koyomi.dll"));
		if (hKOYOMI==NULL) throw EAbort("DLL‚ª“Ç‚İ‚ß‚Ü‚¹‚ñ");
		Koyomi_Setdata = (KOYOMI_SetdataDll)::GetProcAddress(hKOYOMI, "SetdataDll");
		Koyomi_Rokuyo  = (KOYOMI_RokuyoDll)::GetProcAddress(hKOYOMI, "RokuyoDll");
		Koyomi_Eto	   = (KOYOMI_EtoDll)::GetProcAddress(hKOYOMI, "EtoDll");
		Koyomi_Sekki   = (KOYOMI_SekkiDll)::GetProcAddress(hKOYOMI, "SekkiDll");
		Koyomi_Kisetu  = (KOYOMI_KisetuDll)::GetProcAddress(hKOYOMI, "KisetuDll");

		if (Koyomi_Setdata==NULL || Koyomi_Rokuyo==NULL ||
			Koyomi_Eto==NULL || Koyomi_Sekki==NULL || Koyomi_Kisetu==NULL)
				throw EAbort("koyomi.dll‚Ì‰Šú‰»‚É¸”s‚µ‚Ü‚µ‚½");

		//ŠÏ‘ª’n‚ğ“Œ‹‚É‰¼İ’è
		Latitude  =  35.68;	//ˆÜ“x
		Longitude = 139.75;	//Œo“x

		Available = true;
	} catch(EAbort &e) {
		ErrMsg = e.Message;
	}
}
//---------------------------------------------------------------------------
KoyomiUnit::~KoyomiUnit()
{
	if (hKOYOMI) ::FreeLibrary(hKOYOMI);
}

//---------------------------------------------------------------------------
//—ïî•ñ•¶š—ñ‚Ìæ“¾
//---------------------------------------------------------------------------
UnicodeString KoyomiUnit::GetInfoStr(int y, int m, int d)
{
	if (!Available) return EmptyStr;
	if (y<1975 || y>2050) return EmptyStr;

	UnicodeString rstr;
	int k_m, k_d;
	int dummy_i, dummy_i2;
	double dummy_f, dummy_f2;

	Koyomi_Setdata(Latitude, Longitude, 0.0, y, m, d, &dummy_i);

	//‹Œ—ïE˜Z—j
	UnicodeString rokuyo_str = Koyomi_Rokuyo(&k_m, &k_d, &dummy_i);
	if (k_m<0)
		rstr.cat_sprintf(_T("‰[%uŒ"), -k_m);
	else
		rstr.cat_sprintf(_T("%2uŒ"), k_m);
	rstr.cat_sprintf(_T("%2d“ú"), k_d);

	//Š±x
	UnicodeString eto_str = Koyomi_Eto(3, &dummy_i, &dummy_i2);
	if (eto_str.SubString(1, 2)=="œú") {
		eto_str.Delete(1, 2); eto_str.Insert("•è",1);
	}
	if (eto_str.SubString(1, 2)=="ˆ¨") {
		eto_str.Delete(1, 2); eto_str.Insert("á¡",1);
	}

	rstr += " " + eto_str + " " + rokuyo_str;

	//ß‹GEGß
	UnicodeString sekki_str = Koyomi_Sekki(&dummy_i, &dummy_f, &dummy_f2);
	if (sekki_str=="||")
		sekki_str = EmptyStr;
	else {
		if (sekki_str=="“y—p") {
			switch (m) {
			case  1: sekki_str = "“~“y—p"; break;
			case  4: sekki_str = "t“y—p"; break;
			case  7: sekki_str = "‰Ä“y—p"; break;
			case 10: sekki_str = "H“y—p"; break;
			}
		};
	}

	//‚»‚Ì‘¼‚ÌGß
	TDateTime dt = TDateTime(y, m, d);	//‘ÎÛ“ú•t

	//—§t‹NZ
	TDateTime risyun_t	= TDateTime(y, 2, Koyomi_Kisetu(1, &dummy_f));
	if		(dt == risyun_t -   1) sekki_str = "ß•ª";
	else if (dt == risyun_t +  87) sekki_str = "”ª\”ª–é";
	else if (dt == risyun_t + 209) sekki_str = "“ñ•S\“ú";

	if (m==3) {
		//t•ª
		int shunbun_d = Koyomi_Kisetu(2, &dummy_f);
		TDateTime shunbun_t = TDateTime(y, 3, shunbun_d);
		//t”ŞŠİ
		if		(dt == shunbun_t - 3) sekki_str = "t”ŞŠİ“ü";
		else if (dt == shunbun_t + 3) sekki_str = "t”ŞŠİ–¾";
		//tĞ“ú (‘OŒã“¯“ú”‚Ìê‡‚Í’¼‘O‚Ìœú‚Ì“ú)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 3, shunbun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 3, shunbun_d  + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("tĞ“ú");
		}
	}
	if (m==9) {
		//H•ª
		int shubun_d = Koyomi_Kisetu(3, &dummy_f);
		TDateTime shubun_t = TDateTime(y, 9, shubun_d);
		//H”ŞŠİ
		if		(dt == shubun_t - 3) sekki_str = "H”ŞŠİ“ü";
		else if (dt == shubun_t + 3) sekki_str = "H”ŞŠİ–¾";
		//HĞ“ú (‘OŒã“¯“ú”‚Ìê‡‚Í’¼‘O‚Ìœú‚Ì“ú)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 9, shubun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 9, shubun_d + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("HĞ“ú");
		}
	}

	if (!sekki_str.IsEmpty()) rstr += " " + sekki_str;

	return rstr;
}
//---------------------------------------------------------------------------
