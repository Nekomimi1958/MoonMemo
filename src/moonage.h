//----------------------------------------------------------------------//
//	月齢/月相/満月計算													//
//																		//
//----------------------------------------------------------------------//
#ifndef moonageH
#define moonageH
//---------------------------------------------------------------------------
#include <vcl.h>

//月についての情報を取得
struct mooninf {
	TDateTime cur_dt;		//現在の日付
	double age;				//月齢
	double phase;			//月相	(0.0〜1.0)
	double illuminated;		//輝面比(0.0〜1.0)
};

void GetMoonInfo(mooninf *mf);

void NextMoonPhase(TDateTime *t, double p);

void DrawMoon(TCanvas *cv, int x0, int y0, int rr,
	double mp, TColor mc, TColor sc, TColor bg);

//---------------------------------------------------------------------------
#endif
