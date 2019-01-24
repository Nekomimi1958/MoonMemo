//----------------------------------------------------------------------//
// 月メモ																//
//	本日のお知らせメッセージ											//
//																		//
//----------------------------------------------------------------------//
#ifndef Unit2H
#define Unit2H

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

#include "moonage.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class TTodayMsgForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TAction *CloseAction;
	TAction *EscAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TBevel *LeftBevel;
	TBevel *RightBevel;
	TMemo *Inf1Memo;
	TMemo *Inf2Memo;
	TMemo *Inf3Memo;
	TMemo *Inf4Memo;
	TMemo *MoonMemo;
	TPaintBox *HaderPaintBox;
	TPaintBox *MoonPaintBox;
	TPanel *BottomPanel;
	TPanel *InfoPanel;
	TPanel *MemPanel1;
	TPanel *MemPanel2;
	TPanel *MemPanel3;
	TPanel *MemPanel4;
	TPanel *MoonPanel;
	TPanel *OkBtnPanel;
	TSpeedButton *OkSpeedButton;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall HaderPaintBoxPaint(TObject *Sender);
	void __fastcall MoonPaintBoxPaint(TObject *Sender);
	void __fastcall CloseExecute(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString HoliInf;
	mooninf	MoonInf;

public:		// ユーザー宣言
	__fastcall TTodayMsgForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTodayMsgForm *TodayMsgForm;
//---------------------------------------------------------------------------
#endif
