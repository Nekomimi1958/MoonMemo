//----------------------------------------------------------------------//
//祝日テーブル															//
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
	 = {_T("日"), _T("月"), _T("火"), _T("水"), _T("木"), _T("金"), _T("土")};
const _TCHAR *WeekStrE[7]
	 = {_T("Sunday"), _T("Monday"), _T("Tuesday"), _T("Wednesday"),
	    _T("Thursday"), _T("Friday"), _T("Saturday")};

const _TCHAR *MonthStrE[12]
	 = {_T("January"), _T("February"), _T("March"), _T("April"), _T("May"), _T("June"),
	    _T("July"), _T("August"), _T("September"), _T("October"), _T("November"), _T("December")};

//--------------------------------------
//指定年月の日数を取得
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
//指定年月日の曜日を取得
// 1:日,2:月 〜 7:土
//--------------------------------------
int get_wek(int y, int m, int d)
{
	if (m<3) {
		y--; m += 12;
	}
	return ((y + y/4 - y/100 + y/400 + (26*m + 16)/10 + d) % 7) + 1;
}

//--------------------------------------
//指定年月の第n w曜日(1〜7)の日を取得
//--------------------------------------
int get_wday(int y, int m, int w, int n)
{
	int d = 1;
	while (get_wek(y, m, d)!=w) d++;	//第1 w曜日を求める

	int i = 1;
	while (i<n) {
		d += 7; i++;
	}
	return d;
}


//---------------------------------------------------------------------------
// 祝日テーブル クラス
//---------------------------------------------------------------------------
HolidayTable::HolidayTable()
{
	Year  = 0;
	Count = 0;
}

//---------------------------------------------------------------------------
//指定年の祝日テーブル作成
//---------------------------------------------------------------------------
void HolidayTable::InitTable(unsigned short y)
{
	Year  = y;
	Count = 0;

	//元日
	Add( 1, 1, _T("元日"));

	//成人の日
	unsigned short d = (y<2000)? 15 : get_wday(y, 1, wixMON, 2);
	Add( 1, d, _T("成人の日"));

	//建国記念の日
	Add( 2,11, _T("建国記念の日"));

	//天皇誕生日(改元)
	if (y>=2020) Add( 2,23, _T("天皇誕生日"));

	//春分の日
	d = (20843100 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 3, d, _T("春分の日"));

	//みどりの日 -> 昭和の日
	if (y>=1989) Add( 4,29, (y<2007)? _T("みどりの日") : _T("昭和の日"));

	//即位の日 (2019のみ)
	if (y==2019) Add( 5, 1, _T("即位の日"));

	//憲法記念日
	Add( 5, 3, _T("憲法記念日"));

	//みどりの日
	if (y>=2007) Add( 5, 4, _T("みどりの日"));

	//こどもの日
	Add( 5, 5, _T("こどもの日"));

	//海の日(2020/2021の移動に対応)
	if (y>=1996) {
		d = (y<2003)? 20 : (y==2020)? 23 : (y==2021)? 22 : get_wday(y, 7, wixMON, 3);
		Add( 7, d, _T("海の日"));
	}

	//スポーツの日(2020/2021の移動)
	if (y==2020 || y==2021) Add( 7, (y==2020)? 24 : 23, _T("スポーツの日"));

	//山の日(2020/2021の移動に対応)
	if (y>=2016) {
		d = (y==2020)? 10 : (y==2021)? 8 : 11;
		Add( 8, d, _T("山の日"));
	}

	//敬老の日
	d = (y<2003)? 15 : get_wday(y, 9, wixMON, 3);
	Add( 9, d, _T("敬老の日"));

	//秋分の日
	d = (23248800 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 9, d, _T("秋分の日"));

	//スポーツの日(2020/2021を除く)
	if (y!=2020 && y!=2021) {
		d = (y<2000)? 10 : get_wday(y, 10, wixMON, 2);
		Add(10, d, (y>=2020)? _T("スポーツの日") : _T("体育の日"));
	}

	//即位礼正殿の儀 (2019のみ)
	if (y==2019) Add(10,22, _T("即位礼正殿の儀"));

	//文化の日
	Add(11, 3, _T("文化の日"));

	//勤労感謝の日
	Add(11,23, _T("勤労感謝の日"));

	//天皇誕生日(平成)
	if (y>=1989 && y<2019) Add(12,23, _T("天皇誕生日"));

	//祝日が日曜の場合
	for (int i=0; i<Count; i++) {
		unsigned short m = Table[i].m;
		unsigned short d = Table[i].d;
		if (get_wek(y, m, d)==wixSUN) {
			for (;;) {
				TDate dt = TDate(y, m, d);
				dt = IncDay(dt, 1);
				DecodeDate(dt, y, m, d);

				if (y<2007) break;
					//(<2007 その日が日曜の場合翌日は振替え休日
				if (!IsHoliday(y, m, d, NULL)) break;
					//(>=2007 最も近い「国民の祝日」でない日
			}
			Add(m, d, _T("振替え休日"));
		}
	}

	//祝日に挟まれた平日は国民の休日
	for (int i=0; i<Count; i++) {
		unsigned short yi = y;
		unsigned short mi = Table[i].m;
		unsigned short di = Table[i].d;
		//翌日
		TDate dt = TDate(yi, mi, di);
		dt = IncDay(dt, 1);
		DecodeDate(dt, yi, mi, di);
		//平日?
		if (!IsHoliday(yi, mi, di, NULL) && get_wek(yi, mi, di)!=wixSUN) {
			unsigned short m = mi;
			unsigned short d = di;
			dt = IncDay(dt, 1);
			DecodeDate(dt, yi, mi, di);
			if (yi==y && IsHoliday(yi, mi, di, NULL)) Add(m, d, _T("国民の休日"));
		}
	}
}

//---------------------------------------------------------------------------
//テーブルに祝日を追加
//---------------------------------------------------------------------------
void HolidayTable::Add(unsigned short m, unsigned short d, const _TCHAR *name)
{
	Table[Count].m = m;
	Table[Count].d = d;
	_tcscpy(Table[Count].name, name);
	Count++;
}

//---------------------------------------------------------------------------
//現年における指定月日の祝日判定
// 戻り値
//  true:祝日/ false:祝日ではない
//  name: NULL でなければ祝日名が返る
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
