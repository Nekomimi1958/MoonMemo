//----------------------------------------------------------------------//
// KOYOMI.DLL用ユニット													//
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
		if (hKOYOMI==NULL) throw EAbort("DLLが読み込めません");
		Koyomi_Setdata = (KOYOMI_SetdataDll)::GetProcAddress(hKOYOMI, "SetdataDll");
		Koyomi_Rokuyo  = (KOYOMI_RokuyoDll)::GetProcAddress(hKOYOMI, "RokuyoDll");
		Koyomi_Eto	   = (KOYOMI_EtoDll)::GetProcAddress(hKOYOMI, "EtoDll");
		Koyomi_Sekki   = (KOYOMI_SekkiDll)::GetProcAddress(hKOYOMI, "SekkiDll");
		Koyomi_Kisetu  = (KOYOMI_KisetuDll)::GetProcAddress(hKOYOMI, "KisetuDll");

		if (Koyomi_Setdata==NULL || Koyomi_Rokuyo==NULL ||
			Koyomi_Eto==NULL || Koyomi_Sekki==NULL || Koyomi_Kisetu==NULL)
				throw EAbort("koyomi.dllの初期化に失敗しました");

		//観測地を東京に仮設定
		Latitude  =  35.68;	//緯度
		Longitude = 139.75;	//経度

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
//暦情報文字列の取得
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

	//旧暦・六曜
	UnicodeString rokuyo_str = Koyomi_Rokuyo(&k_m, &k_d, &dummy_i);
	if (k_m<0)
		rstr.cat_sprintf(_T("閏%u月"), -k_m);
	else
		rstr.cat_sprintf(_T("%2u月"), k_m);
	rstr.cat_sprintf(_T("%2d日"), k_d);

	//干支
	UnicodeString eto_str = Koyomi_Eto(3, &dummy_i, &dummy_i2);
	if (eto_str.SubString(1, 2)=="戌") {
		eto_str.Delete(1, 2); eto_str.Insert("戊",1);
	}
	if (eto_str.SubString(1, 2)=="葵") {
		eto_str.Delete(1, 2); eto_str.Insert("癸",1);
	}

	rstr += " " + eto_str + " " + rokuyo_str;

	//節季・雑節
	UnicodeString sekki_str = Koyomi_Sekki(&dummy_i, &dummy_f, &dummy_f2);
	if (sekki_str=="−−")
		sekki_str = EmptyStr;
	else {
		if (sekki_str=="土用") {
			switch (m) {
			case  1: sekki_str = "冬土用"; break;
			case  4: sekki_str = "春土用"; break;
			case  7: sekki_str = "夏土用"; break;
			case 10: sekki_str = "秋土用"; break;
			}
		};
	}

	//その他の雑節
	TDateTime dt = TDateTime(y, m, d);	//対象日付

	//立春起算
	TDateTime risyun_t	= TDateTime(y, 2, Koyomi_Kisetu(1, &dummy_f));
	if		(dt == risyun_t -   1) sekki_str = "節分";
	else if (dt == risyun_t +  87) sekki_str = "八十八夜";
	else if (dt == risyun_t + 209) sekki_str = "二百十日";

	if (m==3) {
		//春分
		int shunbun_d = Koyomi_Kisetu(2, &dummy_f);
		TDateTime shunbun_t = TDateTime(y, 3, shunbun_d);
		//春彼岸
		if		(dt == shunbun_t - 3) sekki_str = "春彼岸入";
		else if (dt == shunbun_t + 3) sekki_str = "春彼岸明";
		//春社日 (前後同日数の場合は直前の戌の日)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 3, shunbun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 3, shunbun_d  + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("春社日");
		}
	}
	if (m==9) {
		//秋分
		int shubun_d = Koyomi_Kisetu(3, &dummy_f);
		TDateTime shubun_t = TDateTime(y, 9, shubun_d);
		//秋彼岸
		if		(dt == shubun_t - 3) sekki_str = "秋彼岸入";
		else if (dt == shubun_t + 3) sekki_str = "秋彼岸明";
		//秋社日 (前後同日数の場合は直前の戌の日)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 9, shubun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 9, shubun_d + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("秋社日");
		}
	}

	if (!sekki_str.IsEmpty()) rstr += " " + sekki_str;

	return rstr;
}
//---------------------------------------------------------------------------
