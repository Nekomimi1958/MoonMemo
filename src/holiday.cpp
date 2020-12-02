//----------------------------------------------------------------------//
//j“úƒe[ƒuƒ‹															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "holiday.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
const _TCHAR *WeekStrJ[7]
	 = {_T("“ú"), _T("Œ"), _T("‰Î"), _T("…"), _T("–Ø"), _T("‹à"), _T("“y")};
const _TCHAR *WeekStrE[7]
	 = {_T("Sunday"), _T("Monday"), _T("Tuesday"), _T("Wednesday"),
	    _T("Thursday"), _T("Friday"), _T("Saturday")};

const _TCHAR *MonthStrE[12]
	 = {_T("January"), _T("February"), _T("March"), _T("April"), _T("May"), _T("June"),
	    _T("July"), _T("August"), _T("September"), _T("October"), _T("November"), _T("December")};

//--------------------------------------
//w’è”NŒ‚Ì“ú”‚ğæ“¾
//--------------------------------------
int get_dayn(int y, int m)
{
	int d;
	switch (m) {
	case 2:
    	if ((y%4==0 && y%100!=0) || y%400==0) d = 29; else d = 28;
		break;
	case 4: case 6: case 9: case 11:
		d = 30;
		break;
	default:
		d = 31;
	}
	return d;
}

//--------------------------------------
//w’è”NŒ“ú‚Ì—j“ú‚ğæ“¾
// 1:“ú,2:Œ ` 7:“y
//--------------------------------------
int get_wek(int y, int m, int d)
{
	if (m<3) {
		y--; m += 12;
	}
	return ((y + y/4 - y/100 + y/400 + (26*m + 16)/10 + d) % 7) + 1;
}

//--------------------------------------
//w’è”NŒ‚Ì‘æn w—j“ú(1`7)‚Ì“ú‚ğæ“¾
//--------------------------------------
int get_wday(int y, int m, int w, int n)
{
	int d = 1;
	while (get_wek(y, m, d)!=w) d++;	//‘æ1 w—j“ú‚ğ‹‚ß‚é

	int i = 1;
	while (i<n) {
		d += 7; i++;
	}
	return d;
}


//---------------------------------------------------------------------------
// j“úƒe[ƒuƒ‹ ƒNƒ‰ƒX
//---------------------------------------------------------------------------
HolidayTable::HolidayTable()
{
	Year  = 0;
	Count = 0;
}

//---------------------------------------------------------------------------
//w’è”N‚Ìj“úƒe[ƒuƒ‹ì¬
//---------------------------------------------------------------------------
void HolidayTable::InitTable(unsigned short y)
{
	Year  = y;
	Count = 0;

	//Œ³“ú
	Add( 1, 1, _T("Œ³“ú"));

	//¬l‚Ì“ú
	unsigned short d = (y<2000)? 15 : get_wday(y, 1, wixMON, 2);
	Add( 1, d, _T("¬l‚Ì“ú"));

	//Œš‘‹L”O‚Ì“ú
	Add( 2,11, _T("Œš‘‹L”O‚Ì“ú"));

	//“Vc’a¶“ú(‰üŒ³)
	if (y>=2020) Add( 2,23, _T("“Vc’a¶“ú"));

	//t•ª‚Ì“ú
	d = (20843100 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 3, d, _T("t•ª‚Ì“ú"));

	//‚İ‚Ç‚è‚Ì“ú -> º˜a‚Ì“ú
	if (y>=1989) Add( 4,29, (y<2007)? _T("‚İ‚Ç‚è‚Ì“ú") : _T("º˜a‚Ì“ú"));

	//‘¦ˆÊ‚Ì“ú (2019‚Ì‚İ)
	if (y==2019) Add( 5, 1, _T("‘¦ˆÊ‚Ì“ú"));

	//Œ›–@‹L”O“ú
	Add( 5, 3, _T("Œ›–@‹L”O“ú"));

	//‚İ‚Ç‚è‚Ì“ú
	if (y>=2007) Add( 5, 4, _T("‚İ‚Ç‚è‚Ì“ú"));

	//‚±‚Ç‚à‚Ì“ú
	Add( 5, 5, _T("‚±‚Ç‚à‚Ì“ú"));

	//ŠC‚Ì“ú(2020/2021‚ÌˆÚ“®‚É‘Î‰)
	if (y>=1996) {
		d = (y<2003)? 20 : (y==2020)? 23 : (y==2021)? 22 : get_wday(y, 7, wixMON, 3);
		Add( 7, d, _T("ŠC‚Ì“ú"));
	}

	//ƒXƒ|[ƒc‚Ì“ú(2020/2021‚ÌˆÚ“®)
	if (y==2020 || y==2021) Add( 7, (y==2020)? 24 : 23, _T("ƒXƒ|[ƒc‚Ì“ú"));

	//R‚Ì“ú(2020/2021‚ÌˆÚ“®‚É‘Î‰)
	if (y>=2016) {
		d = (y==2020)? 10 : (y==2021)? 8 : 11;
		Add( 8, d, _T("R‚Ì“ú"));
	}

	//Œh˜V‚Ì“ú
	d = (y<2003)? 15 : get_wday(y, 9, wixMON, 3);
	Add( 9, d, _T("Œh˜V‚Ì“ú"));

	//H•ª‚Ì“ú
	d = (23248800 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 9, d, _T("H•ª‚Ì“ú"));

	//ƒXƒ|[ƒc‚Ì“ú(2020/2021‚ğœ‚­)
	if (y!=2020 && y!=2021) {
		d = (y<2000)? 10 : get_wday(y, 10, wixMON, 2);
		Add(10, d, (y>=2020)? _T("ƒXƒ|[ƒc‚Ì“ú") : _T("‘Ìˆç‚Ì“ú"));
	}

	//‘¦ˆÊ—ç³“a‚Ì‹V (2019‚Ì‚İ)
	if (y==2019) Add(10,22, _T("‘¦ˆÊ—ç³“a‚Ì‹V"));

	//•¶‰»‚Ì“ú
	Add(11, 3, _T("•¶‰»‚Ì“ú"));

	//‹Î˜JŠ´Ó‚Ì“ú
	Add(11,23, _T("‹Î˜JŠ´Ó‚Ì“ú"));

	//“Vc’a¶“ú(•½¬)
	if (y>=1989 && y<2019) Add(12,23, _T("“Vc’a¶“ú"));

	//j“ú‚ª“ú—j‚Ìê‡
	for (int i=0; i<Count; i++) {
		unsigned short m = Table[i].m;
		unsigned short d = Table[i].d;
		if (get_wek(y, m, d)==wixSUN) {
			for (;;) {
				TDate dt = TDate(y, m, d);
				dt = IncDay(dt, 1);
				DecodeDate(dt, y, m, d);

				if (y<2007) break;
					//(<2007 ‚»‚Ì“ú‚ª“ú—j‚Ìê‡—‚“ú‚ÍU‘Ö‚¦‹x“ú
				if (!IsHoliday(y, m, d, NULL)) break;
					//(>=2007 Å‚à‹ß‚¢u‘–¯‚Ìj“úv‚Å‚È‚¢“ú
			}
			Add(m, d, _T("U‘Ö‚¦‹x“ú"));
		}
	}

	//j“ú‚É‹²‚Ü‚ê‚½•½“ú‚Í‘–¯‚Ì‹x“ú
	for (int i=0; i<Count; i++) {
		unsigned short yi = y;
		unsigned short mi = Table[i].m;
		unsigned short di = Table[i].d;
		//—‚“ú
		TDate dt = TDate(yi, mi, di);
		dt = IncDay(dt, 1);
		DecodeDate(dt, yi, mi, di);
		//•½“ú?
		if (!IsHoliday(yi, mi, di, NULL) && get_wek(yi, mi, di)!=wixSUN) {
			unsigned short m = mi;
			unsigned short d = di;
			dt = IncDay(dt, 1);
			DecodeDate(dt, yi, mi, di);
			if (yi==y && IsHoliday(yi, mi, di, NULL)) Add(m, d, _T("‘–¯‚Ì‹x“ú"));
		}
	}
}

//---------------------------------------------------------------------------
//ƒe[ƒuƒ‹‚Éj“ú‚ğ’Ç‰Á
//---------------------------------------------------------------------------
void HolidayTable::Add(unsigned short m, unsigned short d, const _TCHAR *name)
{
	Table[Count].m = m;
	Table[Count].d = d;
	_tcscpy(Table[Count].name, name);
	Count++;
}

//---------------------------------------------------------------------------
//Œ»”N‚É‚¨‚¯‚éw’èŒ“ú‚Ìj“ú”»’è
// –ß‚è’l
//  true:j“ú/ false:j“ú‚Å‚Í‚È‚¢
//  name: NULL ‚Å‚È‚¯‚ê‚Îj“ú–¼‚ª•Ô‚é
//---------------------------------------------------------------------------
bool HolidayTable::IsHoliday(unsigned short y, unsigned short m, unsigned short d, _TCHAR *name)
{
	if (y!=Year) InitTable(y);

	int n = -1;
	for (int i=0; i<Count; i++) {
		if (Table[i].m==m && Table[i].d==d) {
			n = i; break;
		}
	}

	if (name!=NULL) {
		if (n==-1)
			name[0]= '\0';
		else
			_tcscpy(name, Table[n].name);
	}
	return (n!=-1);
}
//---------------------------------------------------------------------------
