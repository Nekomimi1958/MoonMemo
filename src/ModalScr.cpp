//----------------------------------------------------------------------//
// モーダル表示効果用スクリーン(お知らせ用)								//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include "UserFunc.h"
#include "ModalScr.h"
#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModalScrForm *ModalScrForm;

//---------------------------------------------------------------------------
__fastcall TModalScrForm::TModalScrForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::FormShow(TObject *Sender)
{
	TRect mon_rc = Application->MainForm->Monitor->BoundsRect;	mon_rc.Inflate(-1, -1);
	TRect app_rc = Application->MainForm->BoundsRect;
	BoundsRect = app_rc.Contains(mon_rc)? mon_rc : app_rc;
}
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::FormActivate(TObject *Sender)
{
	if (TodayMsgForm->Visible) ::SetForegroundWindow(TodayMsgForm->Handle);
}
//---------------------------------------------------------------------------
