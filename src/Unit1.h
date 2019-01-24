//----------------------------------------------------------------------//
// 月メモ																//
//	メインフォーム														//
//																		//
//----------------------------------------------------------------------//
#ifndef Unit1H
#define Unit1H

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Clipbrd.hpp>
#include "moonage.h"
#include "UIniFile.h"
#include "holiday.h"
#include "koyomi.h"
#include "RzTabs.hpp"
#include "RzPanel.hpp"
#include "RzStatus.hpp"

//---------------------------------------------------------------------------
#define SUPPORT_URL _T("http://nekomimi.la.coocan.jp/")

//---------------------------------------------------------------------------
typedef HWND (WINAPI *FUNC_HtmlHelp)(HWND, LPCWSTR, UINT, DWORD);

//---------------------------------------------------------------------------
#define WM_FORM_SHOWED  (WM_APP + 1)

//---------------------------------------------------------------------------
#define MAX_MEMO_PAGE	 3
#define MAX_CALBUTTON	42
#define MAX_FINDHISTORY 20

//---------------------------------------------------------------------------
//配色インデックス
#define	COLIX_BackGr	0
#define COLIX_ForeGr	1
#define	COLIX_Moon		2
#define	COLIX_Shadow	3
#define COLIX_WeekBg	4
#define COLIX_TodayBtn	5
#define COLIX_SelectBtn	6
#define COLIX_NormalBtn 7
#define COLIX_BlankBtn  8
#define COLIX_HoliDay	9
#define COLIX_WeekBas	10	//10..16
#define COLIX_Mark1		17
#define COLIX_Mark2		18
#define COLIX_Mark3		19
#define COLIX_MemoBg1	20
#define COLIX_MemoFg1	21
#define COLIX_MemoBg2	22
#define COLIX_MemoFg2	23
#define COLIX_MemoBg3	24
#define COLIX_MemoFg3	25
#define COLIX_MemoBg4	26
#define COLIX_MemoFg4	27
#define COLIX_MemoBgX	28
#define COLIX_MemoFgX	29
#define COLIX_MemoBgY	30
#define COLIX_MemoFgY	31
#define COLIX_MemoBgZ	32
#define COLIX_MemoFgZ	33
#define COLIX_FindBg	34
#define COLIX_FindFg	35

#define MAX_COLIX	37

//---------------------------------------------------------------------------
//フォントインデックス
#define FNTIX_Base		0
#define FNTIX_Date		1
#define FNTIX_Memo		2
#define FNTIX_Mark		3
#define FNTIX_Find		4
#define FNTIX_SttBar	5
#define FNTIX_Option	6

#define MAX_FNTIX	8

//---------------------------------------------------------------------------
//カレンダー情報
struct calinf_rec {
	unsigned short year;
	unsigned short mon;
	unsigned short day;
	int 		   wek;
	bool		   holiday;			//祝日?
	_TCHAR		   holname[32];		//祝日名
	//月情報
	double age;				//月齢
	double phase;			//月相	(0.0～1.0)
	double illuminated;		//輝面比(0.0～1.0)
	//データ
	UnicodeString datstr[4];
	bool changed;
};

//datstr のインデックス
#define WDIX_YOTEI1	0	//予定1
#define WDIX_YOTEI2	1	//予定2
#define WDIX_KINEN	2	//記念日
#define WDIX_KOYOMI	3	//暦情報


//---------------------------------------------------------------------------
class TMoonMemoForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TAction *BefDayAction;
	TAction *BefMonAction;
	TAction *BefWekAction;
	TAction *BefYearAction;
	TAction *ChgFindAction;
	TAction *ChgMemo1Action;
	TAction *ChgMemo2Action;
	TAction *ChgMemo3Action;
	TAction *ChgMemo4Action;
	TAction *ChgMemoAction;
	TAction *ChgMemoYAction;
	TAction *ChgMemoZAction;
	TAction *ChgWeekAction;
	TAction *CloseAction;
	TAction *CntDay1Action;
	TAction *CntDay2Action;
	TAction *DatSavAction;
	TAction *FindCopyAllAction;
	TAction *FindCopySelAction;
	TAction *HelpAction;
	TAction *InpDateAction;
	TAction *NxtDayAction;
	TAction *NxtFulAction;
	TAction *NxtMonAction;
	TAction *NxtNewAction;
	TAction *NxtWekAction;
	TAction *NxtYearAction;
	TAction *StartFindAction;
	TAction *TodayAction;
	TAction *TodayMsgAction;
	TActionList *ActionList1;
	TApplicationEvents *ApplicationEvents1;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TButton *ClrBtn;
	TButton *DatSavBtn;
	TButton *ExpColBtn;
	TButton *InpColBtn;
	TButton *MergeBtn;
	TButton *RefBackupBtn;
	TButton *RefColBtn;
	TButton *RefDataBtn;
	TButton *RefFontBtn;
	TButton *RefInfoBtn;
	TButton *RefMemo1Btn;
	TButton *RefMemo2Btn;
	TButton *RefMemo3Btn;
	TButton *RefSndBtn;
	TButton *SKChgBtn;
	TButton *TestMsgBtn;
	TCheckBox *AltCheckBox;
	TCheckBox *Chk1CheckBox;
	TCheckBox *Chk2CheckBox;
	TCheckBox *Chk3CheckBox;
	TCheckBox *Chk4CheckBox;
	TCheckBox *CntCurPosCheckBox;
	TCheckBox *CtrlCheckBox;
	TCheckBox *FitMsgCheckBox;
	TCheckBox *IMECheckBox;
	TCheckBox *KoyomiCheckBox;
	TCheckBox *ModalScrCheckBox;
	TCheckBox *MsgShowCheckBox;
	TCheckBox *NoChkCheckBox;
	TCheckBox *OnceCheckBox;
	TCheckBox *SaveUtf8CheckBox;
	TCheckBox *ShiftCheckBox;
	TColorDialog *ColorDialog1;
	TComboBox *CategoryComboBox;
	TComboBox *FindStrComboBox;
	TComboBox *KeyComboBox;
	TComboBox *MonTimComboBox;
	TComboBox *SavYearComboBox;
	TComboBox *StrWkComboBox;
	TComboBox *TabStyle1ComboBox;
	TComboBox *TabStyle2ComboBox;
	TEdit *CntDay1Edit;
	TEdit *CntDay2Edit;
	TEdit *CntMon1Edit;
	TEdit *CntMon2Edit;
	TEdit *CntYear1Edit;
	TEdit *CntYear2Edit;
	TEdit *Mark1Edit;
	TEdit *Mark2Edit;
	TEdit *Mark3Edit;
	TEdit *MsgSoundEdit;
	TEdit *OptBackupDirEdit;
	TEdit *OptDatFileEdit;
	TEdit *OptInfFileEdit;
	TEdit *OptMemFile1Edit;
	TEdit *OptMemFile2Edit;
	TEdit *OptMemFile3Edit;
	TEdit *OptMemTit1Edit;
	TEdit *OptMemTit2Edit;
	TEdit *OptMemTit3Edit;
	TEdit *TabStr1Edit;
	TEdit *TabStr2Edit;
	TEdit *TabStr3Edit;
	TEdit *TabStr4Edit;
	TFontDialog *FontDialog1;
	TGroupBox *BackupGroupBox;
	TGroupBox *CountGroupBox;
	TGroupBox *DatFilGroupBox;
	TGroupBox *InfFilBox3;
	TGroupBox *MarkGroupBox;
	TGroupBox *MemFilGroupBox;
	TGroupBox *MsgCndGroupBox;
	TGroupBox *TabStyleGroupBox;
	TGroupBox *WebTabGroupBox;
	TImage *IconImage;
	TImage *SpuitImage;
	TLabel *FindWdLabel;
	TLabel *Label1;
	TLabel *Label1_1;
	TLabel *Label1_2;
	TLabel *Label1_3;
	TLabel *Label2;
	TLabel *Label4_1;
	TLabel *Label5_1;
	TLabel *Label5_2;
	TLabel *Label5_3;
	TLabel *Label5_4;
	TLabel *Label5_c1;
	TLabel *Label5_c2;
	TLabel *Label5_c3;
	TLabel *Label5_c4;
	TLabel *Label5_c5;
	TLabel *Label5_c6;
	TLabel *Label5_m1;
	TLabel *Label5_m2;
	TLabel *Label5_m3;
	TLabel *TitleLabel;
	TLabel *UrlLabel;
	TLabel *VersionLabel;
	TLabeledEdit *ModalAlphaEdit;
	TListBox *FindListBox;
	TListBox *OptColListBox;
	TListBox *OptFontListBox;
	TListBox *ShortCutList;
	TMemo *ToDoMemo1;
	TMemo *ToDoMemo2;
	TMemo *ToDoMemo3;
	TMenuItem *CalPop_s1;
	TMenuItem *CalPop_s2;
	TMenuItem *FindCopyAllItem;
	TMenuItem *FindCopySelItem;
	TMenuItem *MovCntDay1Item;
	TMenuItem *MovCntDay2Item;
	TMenuItem *MovInpDayItem;
	TMenuItem *NxtFulMonItem;
	TMenuItem *NxtNewMonItem;
	TMenuItem *TodayItem;
	TOpenDialog *OpenDialog1;
	TPaintBox *CalPaintBox;
	TPaintBox *HeaderPaintBox;
	TPaintBox *WekBtnPaintBox;
	TPanel *BottomPanel3;
	TPanel *BottomPanel4;
	TPanel *CalBottomPanel;
	TPanel *CalBtnPanel;
	TPanel *CalPanel;
	TPanel *DatOptPanel;
	TPanel *FindBottomPanel;
	TPanel *ListPanel6;
	TPanel *OptPanel;
	TPanel *RightBackPanel;
	TPanel *RightPanel2;
	TPanel *RightPanel6;
	TPanel *SpuitPanel;
	TPanel *TopPanel4;
	TPanel *VersionPanel;
	TPanel *WebTabPanel;
	TPanel *WekBottomPanel;
	TPanel *WekMemPanel;
	TPanel *WekNavPanel;
	TPanel *WekPanel;
	TPopupMenu *CalPopupMenu;
	TPopupMenu *FindPopupMenu;
	TPopupMenu *JumpPopupMenu;
	TSaveDialog *SaveDialog1;
	TSpeedButton *BefMonBtn;
	TSpeedButton *BefWekBtn;
	TSpeedButton *BefYearBtn;
	TSpeedButton *FindBtn;
	TSpeedButton *HelpButton;
	TSpeedButton *NxtMonBtn;
	TSpeedButton *NxtWekBtn;
	TSpeedButton *NxtYearBtn;
	TSpeedButton *TodayBtn;
	TSpeedButton *TodayBtn2;
	TSplitter *Splitter1;
	TSplitter *WekSplitter;
	TTimer *SpuitTimer;
	TTimer *Timer1;
	TRzPageControl *RzPageControl1;
	TRzPageControl *RzPageControl2;
	TRzPanel *RzPanel1;
	TRzStatusBar *RzStatusBar1;
	TRzStatusPane *RzStatusPane1;
	TRzStatusPane *RzStatusPane2;
	TRzStatusPane *RzStatusPane3;
	TRzTabControl *MemRzTabControl;
	TRzTabSheet *FindSheet;
	TRzTabSheet *Memo1Sheet;
	TRzTabSheet *Memo2Sheet;
	TRzTabSheet *Memo3Sheet;
	TRzTabSheet *Opt1TabSheet;
	TRzTabSheet *Opt2TabSheet;
	TRzTabSheet *Opt3TabSheet;
	TRzTabSheet *Opt4TabSheet;
	TRzTabSheet *Opt5TabSheet;
	TRzTabSheet *Opt6TabSheet;
	TRzTabSheet *OptSheet;
	TRzTabSheet *VersionTabSheet;
	TRzTabSheet *WeekSheet;
	TCheckBox *SttMoonInfCheckBox;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall HeaderPaintBoxPaint(TObject *Sender);
	void __fastcall CalPaintBoxPaint(TObject *Sender);
	void __fastcall WekBtnPaintBoxPaint(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall DayChanged(TObject *Sender);
	void __fastcall CalPaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall WekBtnPaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall MoveBtnClick(TObject *Sender);
	void __fastcall CalPanelResize(TObject *Sender);
	void __fastcall WekMemoChange(TObject *Sender);
	void __fastcall MemRzTabControlChange(TObject *Sender);
	void __fastcall FindListBoxDblClick(TObject *Sender);
	void __fastcall RefColBtnClick(TObject *Sender);
	void __fastcall RefFontBtnClick(TObject *Sender);
	void __fastcall RefDataBtnClick(TObject *Sender);
	void __fastcall DatSavExecute(TObject *Sender);
	void __fastcall RefInfoBtnClick(TObject *Sender);
	void __fastcall RefMemo1BtnClick(TObject *Sender);
	void __fastcall WekMemPanelResize(TObject *Sender);
	void __fastcall RefSndBtnClick(TObject *Sender);
	void __fastcall UrlLabelClick(TObject *Sender);
	void __fastcall SKChgBtnClick(TObject *Sender);
	void __fastcall ClrBtnClick(TObject *Sender);
	void __fastcall ChgMemoActionExecute(TObject *Sender);
	void __fastcall ChgTabActionExecute(TObject *Sender);
	void __fastcall CloseActionExecute(TObject *Sender);
	void __fastcall TodayMsgActionExecute(TObject *Sender);
	void __fastcall StartFindActionExecute(TObject *Sender);
	void __fastcall FindStrComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall JumpPopupMenuPopup(TObject *Sender);
	void __fastcall JumpPopupItemClick(TObject *Sender);
	void __fastcall HelpActionExecute(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, int Data, bool &CallHelp);
	void __fastcall DatSavActionUpdate(TObject *Sender);
	void __fastcall StartFindActionUpdate(TObject *Sender);
	void __fastcall TestMsgBtnClick(TObject *Sender);
	void __fastcall WekMemoEnter(TObject *Sender);
	void __fastcall CalPopupMenuPopup(TObject *Sender);
	void __fastcall ChgDateExecute(TObject *Sender);
	void __fastcall NxtFulActionExecute(TObject *Sender);
	void __fastcall NxtNewActionExecute(TObject *Sender);
	void __fastcall OptMemTitEditChange(TObject *Sender);
	void __fastcall TabStrEditChange(TObject *Sender);
	void __fastcall InpDateActionExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall WekMemoExit(TObject *Sender);
	void __fastcall FindListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall StatusBar1DblClick(TObject *Sender);
	void __fastcall CategoryComboBoxChange(TObject *Sender);
	void __fastcall ShortCutListClick(TObject *Sender);
	void __fastcall RefBackupBtnClick(TObject *Sender);
	void __fastcall MergeBtnClick(TObject *Sender);
	void __fastcall FindCopySelActionExecute(TObject *Sender);
	void __fastcall FindCopySelActionUpdate(TObject *Sender);
	void __fastcall FindCopyAllActionExecute(TObject *Sender);
	void __fastcall FindCopyAllActionUpdate(TObject *Sender);
	void __fastcall OptionChanged(TObject *Sender);
	void __fastcall OptInfFileEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OptBackupDirEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OptColListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall OptFontListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall RzPageControl1Changing(TObject *Sender, int NewIndex, bool &AllowChange);
	void __fastcall MemRzTabControlChanging(TObject *Sender, int NewIndex, bool &AllowChange);
	void __fastcall WekSplitterMoved(TObject *Sender);
	void __fastcall RzStatusPane3DblClick(TObject *Sender);
	void __fastcall TabStyle1ComboBoxChange(TObject *Sender);
	void __fastcall TabStyle2ComboBoxChange(TObject *Sender);
	void __fastcall TabStyleComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall SpuitImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SpuitImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SpuitTimerTimer(TObject *Sender);
	void __fastcall InpColBtnClick(TObject *Sender);
	void __fastcall ExpColBtnClick(TObject *Sender);
	void __fastcall RzPageControl1Change(TObject *Sender);

private:	// ユーザー宣言
	int VersionNo;

	TMemo *WekMemo[7];

	int CalBtnHeight;
	int CalBtnWidth;
	int CalDateHeight;
	int CalMarkHeight;
	int WekBtnHeight;
	int WekDateHeight;
	int WekDateWidth;
	int WekMarkHeight;
	int WekMarkWidth;

	calinf_rec CalInf[MAX_CALBUTTON];
	calinf_rec WekInf[7];

	KoyomiUnit *Koyomi;		//暦情報

	unsigned short NowHour, NowMin, NowSec, NowMsc;		//現在時刻
	unsigned short LstMin;								//直前の分
	unsigned short CurYear, CurMon, CurDay, CurWeek;	//現在の選択位置
	unsigned short LstYear, LstMon, LstDay, LstWeek;	//直前の選択位置
	unsigned short CurCalRow, LstCalRow;				//カレンダーの行
	mooninf NowMoonInf;									//現在の月情報
	mooninf CurMoonInf;									//選択位置の月情報

	UnicodeString  MemoFile[MAX_MEMO_PAGE];		//メモテキスト
	UnicodeString  MemoTitle[MAX_MEMO_PAGE];	//メモのタイトル
	UnicodeString  TabTitle[4];					//切り替えタブのタイトル

	unsigned short CalMoonTime;		//月情報の時間

	UnicodeString  ExeDir;			//実行ファイルのディレクトリ
	UnicodeString  DataDir;			//データの場所
	UnicodeString  DataFile;		//基本データ
	UnicodeString  BackupDir;		//基本データのバックアップ先
	TStringList   *DataList;
	UnicodeString  ExtInfFile;		//拡張情報
	TStringList   *ExtInfList;

	bool FormShowed;		//メインフォームが表示された
	bool FitMsgToCal;		//お知らせをカレンダー部分に重ねて表示
	bool ReqTodayMsg;
	bool MemoExpanded;		//入力欄がいっぱいに開かれている
	bool reqAllUpdate;		//カレンダー・予定表の強制更新要求
	bool DataModified;		//基本データが変更された

	TCursor crSpuitTool;	//スポイト

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall ReadColors(UsrIniFile *ini_file);
	void __fastcall WriteColors(UsrIniFile *ini_file);
	void __fastcall InitStatusBar();
	UnicodeString __fastcall AdjustTabTitle(UnicodeString s);
	bool __fastcall LoadDataFile();
	int  __fastcall MergeDataFile(UnicodeString fnam);
	bool __fastcall SaveFileEx(TStrings *lst, UnicodeString fnam);
	void __fastcall SetDayInfo(int idx, int y, int m, int d, UnicodeString s);
	void __fastcall SetWekInfo();
	void __fastcall ChangeDate(TDateTime dt);
	void __fastcall ShowTodayMsg(bool sound = true);

public:		// ユーザー宣言
	HolidayTable *CurHoliday;		//祝日テーブル

	unsigned short NowYear, NowMon, NowDay, NowWeek;	//現在時刻

	TDate CountDate, CountDate2;	//カウントダウン日付
	unsigned short CntYear, CntMon, CntDay;
	unsigned short CntYear2, CntMon2, CntDay2;

	TFont  *FontSet[MAX_FNTIX];		//フォント
	TColor ColorSet[MAX_COLIX];		//配色

	__fastcall TMoonMemoForm(TComponent* Owner);

	UnicodeString __fastcall GetDayInfo(int idx, int y, int m, int d, TStringList *dlst = NULL);
	TDateTime  __fastcall GetNextMoonPhase(int y, int m, int d, double p);
	void __fastcall SetSavYearCombo();

	void __fastcall DrawDate(TPaintBox *pp,
		int yp, int fh1, int fh2,  int y, int m, int d = 0, int wk = 0);

	void __fastcall UpdateColor(int idx = -1, TColor col = clNone);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,		TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMoonMemoForm *MoonMemoForm;
//---------------------------------------------------------------------------
#endif
