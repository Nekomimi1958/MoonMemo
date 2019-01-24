//----------------------------------------------------------------------//
// 月メモ																//
//	本日のお知らせメッセージ											//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include "UserFunc.h"
#include "UIniFile.h"
#include "Unit1.h"
#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTodayMsgForm *TodayMsgForm;

//---------------------------------------------------------------------------
__fastcall TTodayMsgForm::TTodayMsgForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::FormCreate(TObject *Sender)
{
	MoonInf.cur_dt = Now();
	GetMoonInfo(&MoonInf);
}

//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::FormShow(TObject *Sender)
{
	TMoonMemoForm *pp = MoonMemoForm;

	UnicodeString sct = "TodayMsg";
	//カレンダー部分に重ねる
	if (pp->FitMsgCheckBox->Checked) {
		Left		 = pp->Left;
		Top 		 = pp->Top;
		ClientWidth  = pp->CalPanel->Width;
		ClientHeight = pp->CalPanel->Height - 2;
	}
	//通常表示
	else {
		Left   = IniFile->ReadInteger(sct, "WinLeft",	pp->Left);
		Top    = IniFile->ReadInteger(sct, "WinTop",	pp->Top);
		Width  = IniFile->ReadInteger(sct, "WinWidth",	320);
		Height = IniFile->ReadInteger(sct, "WinHeight",	320);
	}

	Constraints->MinHeight = (abs(Inf1Memo->Font->Height) + 20) * 4 + 20 +
		MoonPanel->Height + BottomPanel->Height + (Height - ClientHeight);

	Color		= pp->ColorSet[COLIX_BackGr];
	Font->Color = pp->ColorSet[COLIX_ForeGr];
	OkSpeedButton->Font->Style = (OkSpeedButton->Font->Style << fsBold);

	Inf1Memo->Font->Assign(pp->FontSet[FNTIX_Memo]);
	Inf2Memo->Font->Assign(pp->FontSet[FNTIX_Memo]);
	Inf3Memo->Font->Assign(pp->FontSet[FNTIX_Memo]);
	Inf4Memo->Font->Assign(pp->FontSet[FNTIX_Memo]);
	MemPanel1->Color = pp->ColorSet[COLIX_MemoBg1];
	MemPanel2->Color = pp->ColorSet[COLIX_MemoBg2];
	MemPanel3->Color = pp->ColorSet[COLIX_MemoBg3];
	MemPanel4->Color = pp->ColorSet[COLIX_MemoBg4];
	Inf1Memo->Color  = pp->ColorSet[COLIX_MemoBg1];
	Inf2Memo->Color  = pp->ColorSet[COLIX_MemoBg2];
	Inf3Memo->Color  = pp->ColorSet[COLIX_MemoBg3];
	Inf4Memo->Color  = pp->ColorSet[COLIX_MemoBg4];
	Inf1Memo->Font->Color = pp->ColorSet[COLIX_MemoFg1];
	Inf2Memo->Font->Color = pp->ColorSet[COLIX_MemoFg2];
	Inf3Memo->Font->Color = pp->ColorSet[COLIX_MemoFg3];
	Inf4Memo->Font->Color = pp->ColorSet[COLIX_MemoFg4];

	//情報内容を設定
	unsigned short ty = pp->NowYear;
	unsigned short tm = pp->NowMon;
	unsigned short td = pp->NowDay;
	Inf1Memo->Text = ReplaceStr(pp->GetDayInfo(WDIX_YOTEI1, ty, tm, td), "\\n", "\r\n");
	Inf2Memo->Text = ReplaceStr(pp->GetDayInfo(WDIX_YOTEI2, ty, tm, td), "\\n", "\r\n");
	Inf3Memo->Text = ReplaceStr(pp->GetDayInfo(WDIX_KINEN,   0, tm, td), "\\n", "\r\n");
	Inf4Memo->Text = ReplaceStr(pp->GetDayInfo(WDIX_KOYOMI, ty, tm, td), "\\n", "\r\n");

	_TCHAR hbuf[31];
	if (pp->CurHoliday->IsHoliday(ty, tm, td, hbuf))
		HoliInf = hbuf;
	else
		HoliInf = EmptyStr;

	FormResize(NULL);

	//月情報
	MoonMemo->Lines->Clear();
	MoonMemo->Lines->Add(EmptyStr);
	UnicodeString s = FormatDateTime("hh:nn ", Now());
	s.cat_sprintf(_T("月齢 %.2f (%.2f%%"), MoonInf.age, MoonInf.illuminated*100.0);
	s += ((MoonInf.phase<0.50) ? "↑)" : "↓)");
	MoonMemo->Lines->Add(s);
	TDateTime nxt_ful = pp->GetNextMoonPhase(ty, tm, td, 180.0);
	TDateTime nxt_new = pp->GetNextMoonPhase(ty, tm, td,   0.0);
	if (nxt_ful<nxt_new)
		s = FormatDateTime("次の満月 yy/mm/dd hh 時", nxt_ful);
	else
		s = FormatDateTime("次の新月 yy/mm/dd hh 時", nxt_new);
	MoonMemo->Lines->Add(s);
	if (nxt_ful<nxt_new)
		s = FormatDateTime("次の新月 yy/mm/dd hh 時", nxt_new);
	else
		s = FormatDateTime("次の満月 yy/mm/dd hh 時", nxt_ful);
	MoonMemo->Lines->Add(s);
}

//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	if (!MoonMemoForm->FitMsgCheckBox->Checked) {
		UnicodeString sct = "TodayMsg";
		IniFile->WriteInteger(sct, "WinTop",	Top);
		IniFile->WriteInteger(sct, "WinLeft",	Left);
		IniFile->WriteInteger(sct, "WinWidth",	Width);
		IniFile->WriteInteger(sct, "WinHeight",	Height);
	}
}

//---------------------------------------------------------------------------
//見出しを描画
//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::HaderPaintBoxPaint(TObject *Sender)
{
	TMoonMemoForm *pp = MoonMemoForm;
	TCanvas *cv = HaderPaintBox->Canvas;
	cv->Font->Assign(pp->FontSet[FNTIX_Base]);
	cv->Font->Style  = (cv->Font->Style << fsBold);
	cv->Font->Color  = pp->ColorSet[COLIX_ForeGr];
	int fh1 = std::min(HaderPaintBox->Height/2, HaderPaintBox->Width/12);
	int fh2 = fh1 * 4/5;
	pp->DrawDate(HaderPaintBox,
		(HaderPaintBox->ClientHeight - fh1)/2, fh1, fh2,
		pp->NowYear, pp->NowMon, pp->NowDay, pp->NowWeek);
}
//---------------------------------------------------------------------------
//月を描画
//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::MoonPaintBoxPaint(TObject *Sender)
{
	TCanvas *cv = MoonPaintBox->Canvas;
	cv->Brush->Color = clBlack;
	cv->FillRect(MoonPaintBox->ClientRect);

	TMoonMemoForm *pp = MoonMemoForm;
	int w = std::min(MoonPaintBox->Height, MoonPaintBox->Width);
	int rr = w * 4/5;
	int x0 = (MoonPaintBox->Width - rr)/2;
	int y0 = (MoonPaintBox->Height - rr)/2;
	DrawMoon(cv, x0, y0, rr, MoonInf.phase,
		pp->ColorSet[COLIX_Moon], pp->ColorSet[COLIX_Shadow], clBlack);
}

//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::FormResize(TObject *Sender)
{
	int fh = abs(Inf1Memo->Font->Height) + 4;
	int mh = (InfoPanel->ClientHeight - 4*3)/4;
	MemPanel1->ClientHeight = Inf1Memo->Text.IsEmpty()? fh : mh;
	MemPanel2->ClientHeight = Inf2Memo->Text.IsEmpty()? fh : mh;
	MemPanel3->ClientHeight = Inf3Memo->Text.IsEmpty()? fh : mh;

	LeftBevel->Width = (BottomPanel->Width - OkBtnPanel->Width)/2;
}

//---------------------------------------------------------------------------
void __fastcall TTodayMsgForm::CloseExecute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

