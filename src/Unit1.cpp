//----------------------------------------------------------------------//
// 月メモ																//
//	メインフォーム														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <stdio.h>
#include <mmsystem.h>
#include <ShlObj.h>
#include <htmlhelp.h>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.HtmlHelpViewer.hpp>
#include "UserFunc.h"
#include "ModalScr.h"
#include "Unit1.h"
#include "Unit2.h"

#pragma link "Vcl.HTMLHelpViewer"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzTabs"
#pragma link "RzPanel"
#pragma link "RzStatus"
#pragma resource "*.dfm"
TMoonMemoForm *MoonMemoForm;

HINSTANCE	  hHHctrl	  = NULL;
FUNC_HtmlHelp lpfHtmlHelp = NULL;

//---------------------------------------------------------------------------
__fastcall TMoonMemoForm::TMoonMemoForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FormCreate(TObject *Sender)
{
	FormShowed = false;

	VersionNo = get_VersionNo();

	ExeDir = ExtractFileDir(Application->ExeName);
	SetCurrentDir(ExeDir);

	DataList   = new TStringList();
	ExtInfList = new TStringList();

	crSpuitTool = (TCursor)7;	//スポイト
	Screen->Cursors[crSpuitTool] = ::LoadCursor(HInstance, _T("SPUIT_TOOL"));

	IniFile = new UsrIniFile(ChangeFileExt(Application->ExeName, ".INI"));

	//フォント情報読み込み
	for (int i=0; i<MAX_FNTIX; i++) FontSet[i] = NULL;
	FontSet[FNTIX_Base]   = IniFile->ReadFontInf("BaseFont",   Font);
	FontSet[FNTIX_Date]   = IniFile->ReadFontInf("DateFont",   Font);
	FontSet[FNTIX_Memo]   = IniFile->ReadFontInf("MemoFont",   Font);
	FontSet[FNTIX_Mark]   = IniFile->ReadFontInf("MarkFont",   Font);
	FontSet[FNTIX_Find]   = IniFile->ReadFontInf("FindFont",   Font);
	FontSet[FNTIX_SttBar] = IniFile->ReadFontInf("SttBarFont", Font);
	FontSet[FNTIX_Option] = IniFile->ReadFontInf("OptionFont", Font);

	Font->Assign(FontSet[FNTIX_Base]);

	UnicodeString sct = "Option";
	SaveUtf8CheckBox->Checked = IniFile->ReadBool(sct, "SaveUTF8");

	sct = "Test";
	DebugTimeStr = IniFile->ReadString(sct, "DebugTime"	);

	sct = "General";
	//基本データ
	DataFile  = IniFile->ReadString(sct, "DataFile",	ExeDir + "\\MM_DATA.TXT");
	DataDir	  = IniFile->ReadString(sct, "DataDir",		ExtractFileDir(DataFile));
	BackupDir = IniFile->ReadString(sct, "BackupDir"	);
	LoadDataFile();

	//拡張情報データ
	ExtInfFile = IniFile->ReadString(sct, "ExtInfFile");

	if (FileExists(ExtInfFile)) ExtInfList->LoadFromFile(ExtInfFile);

	//切り替えタブ
	TabTitle[0] = IniFile->ReadString( sct, "TabStr1", "予定1");
	TabTitle[1] = IniFile->ReadString( sct, "TabStr2", "予定2");
	TabTitle[2] = IniFile->ReadString( sct, "TabStr3", "記念日");
	TabTitle[3] = IniFile->ReadString( sct, "TabStr4", "暦情報");
	for (int i=0; i<4; i++) MemRzTabControl->Tabs->Items[i]->Caption = AdjustTabTitle(TabTitle[i]);

	//メモデータ
	for (int i=0; i<MAX_MEMO_PAGE; i++) {
		TRzTabSheet *tp;
		TMemo *mp;
		switch (i) {
		case 0: tp = Memo1Sheet; mp = ToDoMemo1; break;
		case 1: tp = Memo2Sheet; mp = ToDoMemo2; break;
		case 2: tp = Memo3Sheet; mp = ToDoMemo3; break;
		}

		if (i==0) {
			MemoFile[i]  = IniFile->ReadString(sct, "MemoFile",  ExeDir + "\\MM_MEMO.TXT");
			MemoTitle[i] = IniFile->ReadString(sct, "MemoTitle", "メモ");
		}
		else {
			MemoFile[i]  = IniFile->ReadString(sct,
				"MemoFile" + IntToStr(i + 1), ExeDir + "\\MM_MEMO" + IntToStr(i + 1) + ".TXT");
			MemoTitle[i] = IniFile->ReadString(sct,
				"MemoTitle" + IntToStr(i + 1));
		}

		if (!MemoTitle[i].IsEmpty()) {
			tp->Caption    = AdjustTabTitle(MemoTitle[i]);
			tp->TabVisible = true;
			if (FileExists(MemoFile[i])) mp->Lines->LoadFromFile(MemoFile[i]);
		}
		else
			tp->TabVisible = false;
	}

	FindSheet->Caption = AdjustTabTitle("検索");
	OptSheet->Caption  = AdjustTabTitle("設定");

	//ヘルプファイル初期化
	Application->HelpFile = ChangeFileExt(Application->ExeName, ".CHM");
	hHHctrl = ::LoadLibrary(_T("hhctrl.ocx"));
	if (hHHctrl) lpfHtmlHelp = (FUNC_HtmlHelp)::GetProcAddress(hHHctrl, "HtmlHelpW");

	//ショートカットを設定
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)ActionList1->Actions[i];
		UnicodeString tmpstr = IniFile->ReadString("ShortCut", ap->Name);
		if (tmpstr.IsEmpty()) tmpstr = ShortCutToText(ap->ShortCut);
		ap->ShortCut = TextToShortCut(tmpstr);
	}

	//ジャンプメニューを設定
	for (int i=0; i<21; i++) {
		TMenuItem *mp = new TMenuItem(JumpPopupMenu->Items);
		mp->OnClick   = JumpPopupItemClick;
		mp->Tag 	  = i;
		JumpPopupMenu->Items->Add(mp);
	}

	//暦情報の初期化
	Koyomi = new KoyomiUnit();
	if (!Koyomi->ErrMsg.IsEmpty()) msgbox_ERR(Koyomi->ErrMsg);

	//日付情報の初期化
	CurHoliday = new HolidayTable();
	DecodeDate(get_Now(), NowYear, NowMon, NowDay);
	DecodeTime(get_Now(), NowHour, NowMin, NowSec, NowMsc);
	CurYear = LstYear = NowYear;
	CurMon	= LstMon  = NowMon;
	CurDay	= LstDay  = NowDay;
	CurWeek = LstWeek = NowWeek = get_wek(NowYear, NowMon, NowDay);
	LstMin	= 0;
	LstCalRow = CurCalRow = ((get_wek(CurYear, CurMon, 1) -1) + CurDay -1) / 7;

	SetSavYearCombo();

	CalMoonTime = (unsigned short)IniFile->ReadInteger(sct, "CalMoonTime", 21);
	if (CalMoonTime>23) CalMoonTime = 21;

	NowMoonInf.cur_dt = get_Now();
	GetMoonInfo(&NowMoonInf);
	CurMoonInf = NowMoonInf;

	CurHoliday->InitTable(CurYear);

	CalBtnWidth = CalBtnHeight = 0;

	//カレンダー情報の初期化
	for (int i=0; i<MAX_CALBUTTON; i++) {
		calinf_rec *cp = &CalInf[i];
		cp->year = cp->mon = cp->day = 0;
		cp->datstr[WDIX_YOTEI1] = EmptyStr;
		cp->datstr[WDIX_YOTEI2] = EmptyStr;
		cp->datstr[WDIX_KINEN]  = EmptyStr;
		cp->changed = false;
	}

	for (int i=0; i<7; i++) {
		//週間情報の初期化
		calinf_rec *wp = &WekInf[i];
		wp->year = wp->mon = wp->day  = 0;
		wp->datstr[WDIX_YOTEI1] = EmptyStr;
		wp->datstr[WDIX_YOTEI2] = EmptyStr;
		wp->datstr[WDIX_KINEN]  = EmptyStr;
		wp->changed = false;
		//週間情報入力欄を作成
		TMemo *mp = new TMemo(this);
		mp->Parent	   = WekMemPanel;
		mp->Font->Assign(FontSet[FNTIX_Memo]);
		mp->ScrollBars = ssVertical;
		mp->OnChange   = WekMemoChange;
		mp->OnEnter    = WekMemoEnter;
		mp->Tag 	   = i;
		mp->Font->Assign(FontSet[FNTIX_Memo]);
		WekMemo[i]	   = mp;
	}

	ToDoMemo1->Font->Assign(FontSet[FNTIX_Memo]);
	ToDoMemo2->Font->Assign(FontSet[FNTIX_Memo]);
	ToDoMemo3->Font->Assign(FontSet[FNTIX_Memo]);
	FindListBox->Font->Assign(FontSet[FNTIX_Find]);
	RzPageControl2->Font->Assign(FontSet[FNTIX_Option]);

	DataModified = false;
	MemoExpanded = false;
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FormShow(TObject *Sender)
{
	IniFile->LoadFormPos(this, 720, 540);

	UnicodeString sct = "General";

	//※設定ページのレイアウト乱れを防ぐ
	int wd = IniFile->ReadInteger(sct, "CalWidth",	240);
	RzPageControl1->ActivePage = OptSheet;
	CalPanel->ClientWidth	   = wd + 1;
	CalPanel->ClientWidth	   = wd;
	RzPageControl1->ActivePage = WeekSheet;

	WekBtnPaintBox->Width = IniFile->ReadInteger(sct, "WekBtnWidth",	120);
	WekSplitterMoved(NULL);

	UrlLabel->Caption = SUPPORT_URL;
	VersionLabel->Caption = "Version " + FloatToStrF(VersionNo/100.0, ffFixed, 4, 2);

	TAnchors ak;
	ak << akLeft << akRight;
	for (int i=0; i<7; i++) {
		TMemo *mp	= WekMemo[i];
		mp->Left	= 0;
		mp->Top 	= WekBtnHeight * i;
		mp->Width	= WekMemPanel->ClientWidth;
		mp->Height	= WekBtnHeight;
		mp->Anchors = ak;
	}

	sct = "Option";
	//お知らせ
	MsgShowCheckBox->Checked  = IniFile->ReadBool(  sct, "MsgShow"		);
	OnceCheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgOnce"		);
	Chk1CheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgCheck1",		true);
	Chk2CheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgCheck2",		true);
	Chk3CheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgCheck3",		true);
	Chk4CheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgCheck4",		true);
	NoChkCheckBox->Checked	  = IniFile->ReadBool(  sct, "MsgNoCheck"	);
	MsgSoundEdit->Text		  = IniFile->ReadString(sct, "MsgSound");
	FitMsgCheckBox->Checked   = IniFile->ReadBool(  sct, "FitMsgToCal"	);
	ModalScrCheckBox->Checked = IniFile->ReadBool(  sct, "MsgModalScr"	);
	ModalAlphaEdit->Text	  = IniFile->ReadString(sct, "MsgModalAlpha",	"128");

	//切り替えボタン
	TabStr1Edit->Text = TabTitle[0];
	TabStr2Edit->Text = TabTitle[1];
	TabStr3Edit->Text = TabTitle[2];
	TabStr4Edit->Text = TabTitle[3];

	//週の始まり
	for (int i=0; i<7; i++) StrWkComboBox->Items->Add(WeekStrJ[i]);
	StrWkComboBox->ItemIndex	= IniFile->ReadInteger(sct, "WeekStart",	wixSUN) - 1;

	SttMoonInfCheckBox->Checked = IniFile->ReadBool(sct, "SttMoonInf");
	KoyomiCheckBox->Checked 	= IniFile->ReadBool(sct, "ShowKoyomi",	true);
	IMECheckBox->Checked		= IniFile->ReadBool(sct, "IME_AutoOn");

	//まで何日
	UnicodeString tmpstr = IniFile->ReadString(sct, "CountDate");
	if (!tmpstr.IsEmpty()) {
		CountDate = TDate(tmpstr);
		DecodeDate(CountDate, CntYear, CntMon, CntDay);
		CntYear1Edit->Text = CntYear;
		CntMon1Edit->Text  = CntMon;
		CntDay1Edit->Text  = CntDay;
		MovCntDay1Item->Caption = FormatDateTime("yy/mm/dd", CountDate);
		CntDay1Action->Tag = int(CountDate);
	}
	else
		CntYear = CntMon = CntDay = 0;

	tmpstr = IniFile->ReadString(sct, "CountDate2");
	if (!tmpstr.IsEmpty()) {
		CountDate2 = TDate(tmpstr);
		DecodeDate(CountDate2, CntYear2, CntMon2, CntDay2);
		CntYear2Edit->Text = CntYear2;
		CntMon2Edit->Text  = CntMon2;
		CntDay2Edit->Text  = CntDay2;
		MovCntDay2Item->Caption = FormatDateTime("yy/mm/dd", CountDate2);
		CntDay2Action->Tag = int(CountDate2);
	}
	else
		CntYear2 = CntMon2 = CntDay2 = 0;

	CntCurPosCheckBox->Checked = IniFile->ReadBool(sct, "CountCurPos");

	//マーク
	Mark1Edit->Text = IniFile->ReadString(sct, "MarkStr1", "●");
	Mark2Edit->Text = IniFile->ReadString(sct, "MarkStr2", "●");
	Mark3Edit->Text = IniFile->ReadString(sct, "MarkStr3", "★");

	//配色
	ReadColors(IniFile);
	UpdateColor();

	//検索履歴
	IniFile->LoadComboBoxItems(FindStrComboBox, "FindHistory", MAX_FINDHISTORY);

	OptDatFileEdit->Text   = DataFile;
	OptInfFileEdit->Text   = ExtInfFile;
	OptMemFile1Edit->Text  = MemoFile[0];
	OptMemFile2Edit->Text  = MemoFile[1];
	OptMemFile3Edit->Text  = MemoFile[2];
	OptMemTit1Edit->Text   = MemoTitle[0];
	OptMemTit2Edit->Text   = MemoTitle[1];
	OptMemTit3Edit->Text   = MemoTitle[2];
	OptBackupDirEdit->Text = BackupDir;

	//オプション - 配色
	OptColListBox->Items->Text =
		"画面背景\r\n"
		"画面の文字\r\n"
		"|月の輝面\r\n"
		"月の影\r\n"
		"曜日表示の背景\r\n"
		"本日のボタン背景\r\n"
		"選択中のボタン背景\r\n"
		"その他のボタン背景\r\n"
		"空白ボタン背景\r\n"
		"|祝日の文字\r\n"
		"日曜日の文字\r\n"
		"月曜日の文字\r\n"
		"火曜日の文字\r\n"
		"水曜日の文字\r\n"
		"木曜日の文字\r\n"
		"金曜日の文字\r\n"
		"土曜日の文字\r\n"
		"|予定1のマーク\r\n"
		"予定2のマーク\r\n"
		"記念日のマーク\r\n"
		"|予定1の背景\r\n"
		"予定1の文字\r\n"
		"予定2の背景\r\n"
		"予定2の文字\r\n"
		"記念日の背景\r\n"
		"記念日の文字\r\n"
		"暦情報の背景\r\n"
		"暦情報の文字\r\n"
		"|メモの背景\r\n"
		"メモの文字\r\n"
		"メモ2の背景\r\n"
		"メモ2の文字\r\n"
		"メモ3の背景\r\n"
		"メモ3の文字\r\n"
		"検索リストの背景\r\n"
		"検索リストの文字\r\n";

	OptColListBox->ItemHeight = abs(OptColListBox->Font->Height) * 3 / 2;
	OptColListBox->ItemIndex  = 0;

	//オプション - フォント
	OptFontListBox->Items->Text =
		"画面の文字\r\n"
		"日付\r\n"
		"予定／メモのテキスト\r\n"
		"マーク\r\n"
		"検索リスト\r\n"
		"ステータスバー\r\n"
		"設定ページ\r\n";

	OptFontListBox->ItemHeight = abs(OptFontListBox->Font->Height) + 6;
	OptFontListBox->ItemIndex  = 0;

	for (int i=0; i<24; i++) MonTimComboBox->Items->Add(i);
	MonTimComboBox->ItemIndex = CalMoonTime;

	//オプション - デザイン
	TabStyle1ComboBox->Items->Text = 
		"BackSlant\r\n"
		"CutCorner\r\n"
		"DoubleSlant\r\n"
		"RoundCorners\r\n"
		"SingleSlant\r\n"
		"SquareCorners\r\n";
	TabStyle2ComboBox->Items->Text = TabStyle1ComboBox->Items->Text;
	TabStyle1ComboBox->ItemIndex   = IniFile->ReadInteger(sct, "TabStyle1",	4);
	TabStyle2ComboBox->ItemIndex   = IniFile->ReadInteger(sct, "TabStyle2",	0);
	TabStyle1ComboBoxChange(NULL);
	TabStyle2ComboBoxChange(NULL);

	//ショートカット
	CategoryComboBox->Items->Clear();
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)ActionList1->Actions[i];
		if (ap->Category=="内部") break;
		ShortCutList->Items->Add(ap->Hint + "\t" + ShortCutToText(ap->ShortCut));
		//カテゴリ選択コンボを設定
		if (CategoryComboBox->Items->IndexOf(ap->Category)==-1)
			CategoryComboBox->Items->Add(ap->Category);
	}
	CategoryComboBox->ItemIndex = 0;
	ShortCutList->ItemIndex 	= 0;

	//キー選択リスト
	make_key_list(KeyComboBox->Items);

	MemRzTabControlChange(NULL);
	CalPanelResize(NULL);
	WekMemPanelResize(NULL);

	reqAllUpdate = true;
	ChangeDate(get_Now());

	InitStatusBar();

	Timer1->Enabled = true;

	//お知らせ
	ReqTodayMsg = false;
	if (MsgShowCheckBox->Checked) {
		//重ねて表示
		if (FitMsgCheckBox->Checked)
			//メイン画面が表示された直後に表示
			ReqTodayMsg = true;
		//本日のお知らせフォーム表示前に表示
		else
			TodayMsgActionExecute(NULL);
	}
	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WmFormShowed(TMessage &msg)
{
	//※ステータスバーの文字位置が下がる現象への対処
	Repaint();
	RzStatusBar1->Repaint();

	FormShowed = true;

	//本日のお知らせをフォーム表示後に表示
	if (ReqTodayMsg) TodayMsgActionExecute(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	//------------------------
	//設定内容を保存
	//------------------------
	IniFile->SaveFormPos(this);

	UnicodeString sct = "General";
	IniFile->WriteInteger(sct, "CalWidth",		CalPanel->ClientWidth);
	IniFile->WriteInteger(sct, "WekBtnWidth",	WekBtnPaintBox->Width);

	IniFile->WriteString( sct, "DataFile",		DataFile);
	IniFile->WriteString( sct, "DataDir",		DataDir);
	IniFile->WriteString( sct, "BackupDir",		BackupDir);
	IniFile->WriteString( sct, "ExtInfFile",	ExtInfFile);

	IniFile->WriteString( sct, "TabStr1",		TabTitle[0]);
	IniFile->WriteString( sct, "TabStr2",		TabTitle[1]);
	IniFile->WriteString( sct, "TabStr3",		TabTitle[2]);
	IniFile->WriteString( sct, "TabStr4",		TabTitle[3]);

	IniFile->WriteString( sct, "MemoFile",		MemoFile[0]);
	IniFile->WriteString( sct, "MemoFile2",		MemoFile[1]);
	IniFile->WriteString( sct, "MemoFile3",		MemoFile[2]);
	IniFile->WriteString( sct, "MemoTitle", 	MemoTitle[0]);
	IniFile->WriteString( sct, "MemoTitle2",	MemoTitle[1]);
	IniFile->WriteString( sct, "MemoTitle3",	MemoTitle[2]);

	WriteColors(IniFile);

	//フォント情報
	IniFile->WriteFontInf("BaseFont",	FontSet[FNTIX_Base]);
	IniFile->WriteFontInf("DateFont",	FontSet[FNTIX_Date]);
	IniFile->WriteFontInf("MemoFont",	FontSet[FNTIX_Memo]);
	IniFile->WriteFontInf("MarkFont",	FontSet[FNTIX_Mark]);
	IniFile->WriteFontInf("FindFont",	FontSet[FNTIX_Find]);
	IniFile->WriteFontInf("SttBarFont",	FontSet[FNTIX_SttBar]);
	IniFile->WriteFontInf("OptionFont",	FontSet[FNTIX_Option]);

	//その他
	sct = "Option";
	IniFile->WriteBool(   sct, "SaveUTF8",		SaveUtf8CheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgShow", 		MsgShowCheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgOnce", 		OnceCheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgCheck1",		Chk1CheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgCheck2",		Chk2CheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgCheck3",		Chk3CheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgCheck4",		Chk4CheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgNoCheck",	NoChkCheckBox->Checked);
	IniFile->WriteString( sct, "MsgSound", 		MsgSoundEdit->Text);
	IniFile->WriteBool(   sct, "FitMsgToCal",	FitMsgCheckBox->Checked);
	IniFile->WriteBool(   sct, "MsgModalScr",	ModalScrCheckBox->Checked);
	IniFile->WriteString( sct, "MsgModalAlpha",	ModalAlphaEdit->Text);
	IniFile->WriteInteger(sct, "WeekStart",		StrWkComboBox->ItemIndex + 1);
	IniFile->WriteBool(   sct, "SttMoonInf",	SttMoonInfCheckBox->Checked);
	IniFile->WriteBool(   sct, "ShowKoyomi",	KoyomiCheckBox->Checked);
	IniFile->WriteBool(   sct, "IME_AutoOn",	IMECheckBox->Checked);
	IniFile->WriteString( sct, "CountDate", 	(CntDay==0)?  EmptyStr : CountDate.DateString());
	IniFile->WriteString( sct, "CountDate2", 	(CntDay2==0)? EmptyStr : CountDate2.DateString());
	IniFile->WriteBool(   sct, "CountCurPos",	CntCurPosCheckBox->Checked);
	IniFile->WriteInteger(sct, "CalMoonTime",	CalMoonTime);
	IniFile->WriteString( sct, "MarkStr1",		Mark1Edit->Text);
	IniFile->WriteString( sct, "MarkStr2",		Mark2Edit->Text);
	IniFile->WriteString( sct, "MarkStr3",		Mark3Edit->Text);
	IniFile->WriteInteger(sct, "TabStyle1",		TabStyle1ComboBox->ItemIndex);
	IniFile->WriteInteger(sct, "TabStyle2",		TabStyle2ComboBox->ItemIndex);

	//ショートカット保存
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)ActionList1->Actions[i];
		IniFile->WriteString("ShortCut",ap->Name, ShortCutToText(ap->ShortCut));
	}

	//検索履歴を保存
	IniFile->SaveComboBoxItems(FindStrComboBox, "FindHistory", MAX_FINDHISTORY);

	if (!IniFile->UpdateFile()) msgbox_ERR("INIファイルの保存に失敗しました");

	try {
		//------------------------
		//基本データ保存
		//------------------------
		SetWekInfo();
		if (DataModified) {
			int i = 0;
			while (i<DataList->Count) {
				UnicodeString dbuf = DataList->Strings[i];
				//不正なデータは削除
				if (dbuf.IsEmpty()) {
					DataList->Delete(i); continue;
				}
				if (dbuf[1]!='0') {
					DataList->Delete(i); continue;
				}
				i++;
			}
			DataList->Sort();
			if (!SaveFileEx(DataList, DataFile)) Abort();
		}

		//------------------------
		//メモデータ保存
		//------------------------
		if (ToDoMemo1->Modified) {
			if (!SaveFileEx(ToDoMemo1->Lines, MemoFile[0])) Abort();
		}
		if (ToDoMemo2->Modified) {
			if (!SaveFileEx(ToDoMemo2->Lines, MemoFile[1])) Abort();
		}
		if (ToDoMemo3->Modified) {
			if (!SaveFileEx(ToDoMemo3->Lines, MemoFile[2])) Abort();
		}
	}
	catch(...) {
		if (!msgbox_Y_N("このまま終了してもよいですか?")) CanClose = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FormDestroy(TObject *Sender)
{
	delete DataList;
	delete ExtInfList;

	delete Koyomi;
	delete CurHoliday;

	for (int i=0; i<MAX_FNTIX; i++) if (FontSet[i]) delete FontSet[i];

	delete IniFile;

	if (hHHctrl) ::FreeLibrary(hHHctrl);
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
	if (!Timer1->Enabled) return;

	//ヘルプのキーメッセージ処理
	if (lpfHtmlHelp) {
		Handled = (lpfHtmlHelp(NULL, NULL, HH_PRETRANSLATEMESSAGE, (DWORD)&Msg) != NULL);
	}
}

//---------------------------------------------------------------------------
//配色の読込
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ReadColors(UsrIniFile *ini_file)
{
	UnicodeString sct = "Color";
	ColorSet[COLIX_BackGr]	  = ini_file->ReadColor(sct, "BackGr",		clBtnFace);
	ColorSet[COLIX_ForeGr]	  = ini_file->ReadColor(sct, "ForeGr",		clBlack);
	ColorSet[COLIX_Moon]	  = ini_file->ReadColor(sct, "Moon",		clSilver);
	ColorSet[COLIX_Shadow]	  = ini_file->ReadColor(sct, "Shadow",		clNavy);
	ColorSet[COLIX_WeekBg]	  = ini_file->ReadColor(sct, "WeekBg",		clBlack);
	ColorSet[COLIX_TodayBtn]  = ini_file->ReadColor(sct, "TodayBtn",	clOlive);
	ColorSet[COLIX_SelectBtn] = ini_file->ReadColor(sct, "SelectBtn",	clTeal);
	ColorSet[COLIX_NormalBtn] = ini_file->ReadColor(sct, "NormalBtn",	clBlack);
	ColorSet[COLIX_BlankBtn]  = ini_file->ReadColor(sct, "BlankBtn",	clLtGray);
	ColorSet[COLIX_HoliDay]   = ini_file->ReadColor(sct, "HoliDay",		clRed);
	for (int i=0; i<7; i++)
		ColorSet[COLIX_WeekBas + i] = ini_file->ReadColor(
			sct, "Week" + IntToStr(i), ((i==0)? clRed : clWhite));
	ColorSet[COLIX_Mark1]	= ini_file->ReadColor(sct, "Mark1",		clRed);
	ColorSet[COLIX_Mark2]	= ini_file->ReadColor(sct, "Mark2",		clGreen);
	ColorSet[COLIX_Mark3]	= ini_file->ReadColor(sct, "Mark3",		clYellow);
	ColorSet[COLIX_MemoBg1] = ini_file->ReadColor(sct, "MemoBg1",	clWindow);
	ColorSet[COLIX_MemoFg1] = ini_file->ReadColor(sct, "MemoFg1",	clWindowText);
	ColorSet[COLIX_MemoBg2] = ini_file->ReadColor(sct, "MemoBg2",	clWindow);
	ColorSet[COLIX_MemoFg2] = ini_file->ReadColor(sct, "MemoFg2",	clWindowText);
	ColorSet[COLIX_MemoBg3] = ini_file->ReadColor(sct, "MemoBg3",	clWindow);
	ColorSet[COLIX_MemoFg3] = ini_file->ReadColor(sct, "MemoFg3",	clWindowText);
	ColorSet[COLIX_MemoBg4] = ini_file->ReadColor(sct, "MemoBg4",	clWindow);
	ColorSet[COLIX_MemoFg4] = ini_file->ReadColor(sct, "MemoFg4",	clWindowText);
	ColorSet[COLIX_MemoBgX] = ini_file->ReadColor(sct, "MemoBgX",	clWindow);
	ColorSet[COLIX_MemoFgX] = ini_file->ReadColor(sct, "MemoFgX",	clWindowText);
	ColorSet[COLIX_MemoBgY] = ini_file->ReadColor(sct, "MemoBgY",	clWindow);
	ColorSet[COLIX_MemoFgY] = ini_file->ReadColor(sct, "MemoFgY",	clWindowText);
	ColorSet[COLIX_MemoBgZ] = ini_file->ReadColor(sct, "MemoBgZ",	clWindow);
	ColorSet[COLIX_MemoFgZ] = ini_file->ReadColor(sct, "MemoFgZ",	clWindowText);
	ColorSet[COLIX_FindBg]	= ini_file->ReadColor(sct, "FindBg",	clWindow);
	ColorSet[COLIX_FindFg]	= ini_file->ReadColor(sct, "FindFg",	clWindowText);
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WriteColors(UsrIniFile *ini_file)
{
	UnicodeString sct = "Color";
	ini_file->WriteInteger(sct, "BackGr",		ColorSet[COLIX_BackGr]);
	ini_file->WriteInteger(sct, "ForeGr",		ColorSet[COLIX_ForeGr]);
	ini_file->WriteInteger(sct, "Moon",			ColorSet[COLIX_Moon]);
	ini_file->WriteInteger(sct, "Shadow",		ColorSet[COLIX_Shadow]);
	ini_file->WriteInteger(sct, "WeekBg",		ColorSet[COLIX_WeekBg]);
	ini_file->WriteInteger(sct, "TodayBtn",		ColorSet[COLIX_TodayBtn]);
	ini_file->WriteInteger(sct, "SelectBtn",	ColorSet[COLIX_SelectBtn]);
	ini_file->WriteInteger(sct, "NormalBtn",	ColorSet[COLIX_NormalBtn]);
	ini_file->WriteInteger(sct, "BlankBtn",		ColorSet[COLIX_BlankBtn]);
	ini_file->WriteInteger(sct, "HoliDay",		ColorSet[COLIX_HoliDay]);
	for (int i=0; i<7; i++) ini_file->WriteInteger(sct, "Week" + IntToStr(i), ColorSet[COLIX_WeekBas + i]);
	ini_file->WriteInteger(sct, "Mark1",		ColorSet[COLIX_Mark1]);
	ini_file->WriteInteger(sct, "Mark2",		ColorSet[COLIX_Mark2]);
	ini_file->WriteInteger(sct, "Mark3",		ColorSet[COLIX_Mark3]);
	ini_file->WriteInteger(sct, "MemoBg1",		ColorSet[COLIX_MemoBg1]);
	ini_file->WriteInteger(sct, "MemoFg1",		ColorSet[COLIX_MemoFg1]);
	ini_file->WriteInteger(sct, "MemoBg2",		ColorSet[COLIX_MemoBg2]);
	ini_file->WriteInteger(sct, "MemoFg2",		ColorSet[COLIX_MemoFg2]);
	ini_file->WriteInteger(sct, "MemoBg3",		ColorSet[COLIX_MemoBg3]);
	ini_file->WriteInteger(sct, "MemoFg3",		ColorSet[COLIX_MemoFg3]);
	ini_file->WriteInteger(sct, "MemoBg4",		ColorSet[COLIX_MemoBg4]);
	ini_file->WriteInteger(sct, "MemoFg4",		ColorSet[COLIX_MemoFg4]);
	ini_file->WriteInteger(sct, "MemoBgX",		ColorSet[COLIX_MemoBgX]);
	ini_file->WriteInteger(sct, "MemoFgX",		ColorSet[COLIX_MemoFgX]);
	ini_file->WriteInteger(sct, "MemoBgY",		ColorSet[COLIX_MemoBgY]);
	ini_file->WriteInteger(sct, "MemoFgY",		ColorSet[COLIX_MemoFgY]);
	ini_file->WriteInteger(sct, "MemoBgZ",		ColorSet[COLIX_MemoBgZ]);
	ini_file->WriteInteger(sct, "MemoFgZ",		ColorSet[COLIX_MemoFgZ]);
	ini_file->WriteInteger(sct, "FindBg",		ColorSet[COLIX_FindBg]);
	ini_file->WriteInteger(sct, "FindFg",		ColorSet[COLIX_FindFg]);
}

//---------------------------------------------------------------------------
//ステータスバーの初期化
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::InitStatusBar()
{
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	TCanvas *cv = bmp->Canvas;
	cv->Font->Assign(FontSet[FNTIX_SttBar]);

	RzStatusBar1->Font->Assign(FontSet[FNTIX_SttBar]);
	RzStatusBar1->Font->Color		 = ColorSet[COLIX_ForeGr];
	RzStatusBar1->GradientColorStart = ColorSet[COLIX_BackGr];
	RzStatusBar1->GradientColorStop  = ColorSet[COLIX_BackGr];

	RzStatusBar1->ClientHeight = cv->TextHeight("Q") + 4;
	RzStatusPane1->Width = cv->TextWidth(" 00:00  月齢 29.50 (100.00%)↑ ");
	RzStatusPane2->Width = cv->TextWidth(" 9999日後 ");
}

//---------------------------------------------------------------------------
//タブ幅が小さくなりすぎないように調整
//---------------------------------------------------------------------------
UnicodeString __fastcall TMoonMemoForm::AdjustTabTitle(UnicodeString s)
{
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	TCanvas *cv = bmp->Canvas;
	cv->Font->Assign(FontSet[FNTIX_Base]);
	int min_wd = cv->TextWidth("週間予定");		//***
	for (int j=0; j<8; j++) {
		if (Canvas->TextWidth(s + " ") > min_wd) break;
		if (j%2==0) s += " "; else s.Insert(" ", 1);
	}
	return s;
}

//---------------------------------------------------------------------------
//データ整理の年指定コンボボックスを設定
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SetSavYearCombo()
{
	int oldest_y = NowYear;
	for (int i=0; i<DataList->Count; i++) {
		UnicodeString dbuf = DataList->Strings[i];
		int id = dbuf.SubString(1, 2).ToIntDef(99);
		int y  = dbuf.SubString(3, 4).ToIntDef(9999);
		if (id!=WDIX_KINEN && y<oldest_y) oldest_y = y;
	}
	for (int y=NowYear-1; y>=oldest_y; y--) SavYearComboBox->Items->Add(IntToStr(y));
	if (SavYearComboBox->Items->Count>0) SavYearComboBox->ItemIndex = 0;
}

//---------------------------------------------------------------------------
// TPaintBox に日付を描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::DrawDate(TPaintBox *pp,
	int yp,	int fh1, int fh2,
	int y, int m,		//年、月
	int d, int wk)		//日、曜日 =0(デフォルト)で非表示
{
	TCanvas *cv = pp->Canvas;

	UnicodeString s[7];
	s[0] = UnicodeString().sprintf(_T("%4u"), y);  s[1] = "年 ";
	s[2] = UnicodeString().sprintf(_T("%2u"), m);  s[3] = "月";
	if (d>0) {
		s[4] = UnicodeString().sprintf(_T("%2u"), d);  s[5] = "日";
		s[6] = EmptyStr;
		if (wk>0) s[6].sprintf(_T(" (%s)"), WeekStrJ[wk - 1]);
	}
	else
		s[4] = s[5] = s[6] = EmptyStr;

	int wd[7];
	int sw = 0;
	for (int i=0; i<(d>0? 6 : 4); i++) {
		cv->Font->Height = ((i%2==0)? fh1 : fh2);
		sw += (wd[i] = cv->TextWidth(s[i]) + 2);
	}
	if (wk>0)  {
		cv->Font->Height = fh2;
		sw += (wd[6] = cv->TextWidth(s[6]) + 2);
	}

	//日付
	int xp = (pp->ClientWidth - sw)/2;
	int yp2 = yp+(fh1-fh2);
	for (int i=0; i<(d>0? 6 : 4); i++) {
		if (i%2==0) {
			cv->Font->Height = fh1;
			cv->TextOut(xp, yp, s[i]);
		}
		else {
			cv->Font->Height = fh2;
			cv->TextOut(xp, yp2, s[i]);
		}
		xp += wd[i];
	}
	//曜日
	if (wk>0)  {
		cv->Font->Height = fh2;
		cv->Font->Color  = CurHoliday->IsHoliday(y, m, d, NULL)?
			 ColorSet[COLIX_HoliDay] : ColorSet[COLIX_WeekBas + wk -1];
		cv->TextOut(xp, yp2, s[6]);
	}
}
//---------------------------------------------------------------------------
//見出し
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::HeaderPaintBoxPaint(TObject *Sender)
{
	//年月
	TCanvas *cv = HeaderPaintBox->Canvas;
	cv->Font->Assign(FontSet[FNTIX_Base]);
	cv->Font->Color  = ColorSet[COLIX_ForeGr];
	cv->Font->Style  = (cv->Font->Style << fsBold);
	cv->Brush->Color = ColorSet[COLIX_BackGr];
	DrawDate(HeaderPaintBox,
		HeaderPaintBox->ClientHeight/8, CalBtnWidth*2/3, CalBtnWidth*1/2,
		CurYear, CurMon);

	//曜日
	cv->Font->Assign(FontSet[FNTIX_Date]);
	cv->Font->Height = CalBtnWidth/2;
	cv->Font->Style  = (cv->Font->Style >> fsBold);
	cv->Brush->Color = ColorSet[COLIX_WeekBg];
	int yp = HeaderPaintBox->ClientHeight - abs(cv->Font->Height) - 4;
	TRect rc = Rect(0, yp, CalBtnWidth, HeaderPaintBox->ClientHeight - 1);
	rc.Inflate(-1, 0);

	int w = 0;
	for (int i=0; i<7; i++) w = std::max(w, cv->TextWidth(WeekStrJ[i]));
	int xp = (CalBtnWidth - w)/2;

	for (int i=0; i<7; i++) {
		cv->FillRect(rc);
		int wix = (i + StrWkComboBox->ItemIndex)%7;
		cv->Font->Color = ColorSet[COLIX_WeekBas + wix];
		cv->TextOut(xp, yp, WeekStrJ[wix]);
		xp += CalBtnWidth;
		OffsetRect(rc, CalBtnWidth, 0);
	}
}
//---------------------------------------------------------------------------
//カレンダーの描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CalPaintBoxPaint(TObject *Sender)
{
	TCanvas *cv = CalPaintBox->Canvas;

	UnicodeString mkstr[3];
	mkstr[0] = Mark1Edit->Text;
	mkstr[1] = Mark2Edit->Text;
	mkstr[2] = Mark3Edit->Text;

	for (int i=0; i<MAX_CALBUTTON; i++) {
		int row = i/7;
		int col = i%7;
		int xp	= col * CalBtnWidth;
		int yp	= row * CalBtnHeight;
		TRect brc = Rect(xp, yp, xp + CalBtnWidth, yp + CalBtnHeight);
		brc.Inflate(-1, -1);
		calinf_rec *cp = &CalInf[i];
		if (cp->day!=0) {
			int idx;
			//ボタン背景
			cv->Brush->Color = ColorSet[
					(NowYear==CurYear && NowMon==CurMon && NowDay==cp->day)? COLIX_TodayBtn :
					(cp->day == CurDay)? COLIX_SelectBtn : COLIX_NormalBtn];
			cv->FillRect(brc);
			//日付
			cv->Font->Assign(FontSet[FNTIX_Date]);
			cv->Font->Height = CalDateHeight;
			cv->Font->Color = ColorSet[cp->holiday? COLIX_HoliDay : (COLIX_WeekBas + cp->wek -1)];
			if (cp->day == CurDay)
				cv->Font->Style = (cv->Font->Style << fsBold);
			else
				cv->Font->Style = (cv->Font->Style >> fsBold);
			UnicodeString s; s.sprintf(_T("%u"), cp->day);
			if (s.Length()==1) s.Insert(" ", 1);
			cv->TextOut(xp + (CalBtnWidth - cv->TextWidth(s))/2, yp + 4, s);

			//マーク
			cv->Font->Assign(FontSet[FNTIX_Mark]);
			cv->Font->Height = CalMarkHeight;
			int dx	= cv->TextWidth("●") + 1;
			int yp1 = yp + CalDateHeight*4/3;
			calinf_rec *kp = (row==CurCalRow)? &WekInf[col] : cp;
			for (int j=0; j<3; j++) {
				if (kp->datstr[j].IsEmpty()) continue;
				cv->Font->Color = ColorSet[COLIX_Mark1 + j];
				cv->TextOut(xp + j*dx + CalMarkHeight/2, yp1, mkstr[j]);
			}

			//月
			int rr = CalBtnWidth*3/4;
			int y0 = yp1 + CalMarkHeight*4/3;
			if ((CalBtnHeight - (CalDateHeight + CalMarkHeight)*4/3) >rr) {
				int x0 = xp + CalBtnWidth/8;
				if (cp->day==CurDay) x0++;
				DrawMoon(cv, x0, y0, rr, cp->phase,
					ColorSet[COLIX_Moon], ColorSet[COLIX_Shadow], cv->Brush->Color);
			}
			//本日
			if (cp->day==CurDay) ::DrawEdge(cv->Handle, &brc, BDR_RAISEDOUTER, BF_RECT);
		}
		else {
			cv->Brush->Color = ColorSet[COLIX_BlankBtn];
			cv->FillRect(brc);
		}
	}
}
//---------------------------------------------------------------------------
//週間予定ボタンの描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekBtnPaintBoxPaint(TObject *Sender)
{
	TCanvas *cv = WekBtnPaintBox->Canvas;
	cv->Font->Assign(FontSet[FNTIX_Date]);
	cv->Font->Height = WekMarkHeight;
	int hwd = cv->TextWidth("勤労感謝の日");
	int ymg = 2;
	int dw  = WekDateWidth;

	UnicodeString mkstr[3];
	mkstr[0] = Mark1Edit->Text;
	mkstr[1] = Mark2Edit->Text;
	mkstr[2] = Mark3Edit->Text;

	for (int i=0; i<7; i++) {
		calinf_rec *wp = &WekInf[i];
		//背景色
		int y0 = WekBtnHeight * i;
		TRect brc = Rect(0, y0, WekBtnPaintBox->Width, y0 + WekBtnHeight);
		TColor bcol = ColorSet[
				(NowYear==wp->year && NowMon==wp->mon && NowDay==wp->day)? COLIX_TodayBtn :
				(wp->day == CurDay)? COLIX_SelectBtn : COLIX_NormalBtn];
		//文字色
		cv->Font->Assign(FontSet[FNTIX_Date]);
		cv->Font->Height = WekDateHeight;
		cv->Font->Color  = wp->holiday ? ColorSet[COLIX_HoliDay] : ColorSet[COLIX_WeekBas + wp->wek -1];

		//背景を描画
		cv->Brush->Style = bsSolid;
		//選択中
		if (CurWeek == (((i + StrWkComboBox->ItemIndex)%7) + 1)) {
			cv->Brush->Color = bcol;
			cv->FillRect(brc);
			::DrawEdge(cv->Handle, &brc, BDR_RAISEDOUTER, BF_RECT);
			cv->Font->Style = (cv->Font->Style << fsBold);
			dw	+= 2;
			ymg += 2;
		}
		//通常
		else {
			cv->Brush->Color = ColorSet[COLIX_BackGr];
			cv->FillRect(brc);
			brc.Left += 4;
			brc.Inflate(-1, -1);
			cv->Brush->Color = bcol;
			cv->FillRect(brc);
			cv->Font->Style = (cv->Font->Style >> fsBold);
		}
		//日付・曜日
		int ysz = ymg;
		int xsz = dw;
		UnicodeString s1 = wp->day;
		UnicodeString s2 = WeekStrJ[(i + StrWkComboBox->ItemIndex) % 7];
		cv->Font->Height = WekDateHeight;
		int w1 = cv->TextWidth(s1) + WekDateHeight/2;
		cv->Font->Height = WekDateHeight*4/5;
		int w2 = cv->TextWidth(s2);
		cv->Font->Height = WekDateHeight;
		cv->TextOut(brc.Left + dw - (w1 + w2), y0 + ysz, s1);
		cv->Font->Height = WekDateHeight*4/5;
		cv->TextOut(brc.Left + dw - w2, y0 + ysz + WekDateHeight/5, s2);
		ysz += (WekDateHeight + WekMarkHeight/2);

		//マーク
		cv->Font->Assign(FontSet[FNTIX_Mark]);
		cv->Font->Height = WekMarkHeight;
		int dx = WekMarkWidth + 2;
		int y2 = y0 + ymg + WekDateHeight + WekMarkHeight/2;
		for (int j=0; j<3; j++) {
			if (wp->datstr[j].IsEmpty()) continue;
			cv->Font->Color = ColorSet[COLIX_Mark1 + j];
			cv->TextOut(brc.Left + j*dx + dx/2, y2, mkstr[j]);
		}

		//余裕があれば月を表示
		int rr = WekBtnHeight*4/5;
		bool mflag = ((WekBtnPaintBox->Width - WekDateWidth*5/4) > rr);
		if (mflag) {
			int x0 = brc.right - rr - 4;
			int y0 = brc.top + (WekBtnHeight - rr)/2;
			DrawMoon(cv, x0, y0, rr, wp->phase,
				ColorSet[COLIX_Moon], ColorSet[COLIX_Shadow], bcol);
			xsz += (rr+4);
		}

		//余裕があれば祝日を表示
		ysz += (WekMarkHeight + WekMarkHeight/3);
		if (wp->holiday) {
			cv->Font->Assign(FontSet[FNTIX_Date]);
			cv->Font->Height = WekMarkHeight;
			cv->Font->Color  = ColorSet[COLIX_HoliDay];
			cv->Font->Style  = (cv->Font->Style >> fsBold);
			cv->Brush->Style = bsClear;
			//日付の右
			if ((mflag && ((brc.Right - xsz + rr/2) > hwd)) || (!mflag && ((brc.Right - xsz) > hwd))) {
				cv->TextOut(
					brc.Left + dw + 4,
					y0 + ymg + WekDateHeight - WekMarkHeight,
					wp->holname);
			}
			//マークの下
			else if (WekBtnHeight-ysz > ymg+WekMarkHeight) {
				cv->TextOut(brc.Left + WekMarkHeight, y0 + ysz, wp->holname);
			}
		}
	}
}
//---------------------------------------------------------------------------
//カレンダーボタン
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CalPaintBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	POINT p = Point(X, Y);
	int bix = -1;
	for (int i=0; i<MAX_CALBUTTON; i++) {
		int xp = (i%7)*CalBtnWidth;
		int yp = (i/7)*CalBtnHeight;
		TRect brc = Rect(xp, yp, xp + CalBtnWidth, yp + CalBtnHeight);
		if (::PtInRect(&brc, p)) {
			bix = i;
			break;
		}
	}

	if (bix != -1) {
		calinf_rec *cp = &CalInf[bix];
		//有効な日付ボタンが押された
		if (cp->day!=0) {
			if (!Shift.Contains(ssDouble)) {
				//日付が変化した
				if (cp->day!=CurDay) {
					if (MemoExpanded) {
						for (int i=0; i<7; i++) WekMemo[i]->Align = alNone;
						for (int i=0; i<7; i++) {
							TMemo *mp  = WekMemo[i];
							if (i==(bix%7)) {
								mp->Align	= alClient;
								mp->Visible = true;
							}
							else
								mp->Visible = false;
						}
					}
					ChangeDate(TDate(cp->year, cp->mon, cp->day));
				}
			}

		}
		//空欄が押された
		else {
			if (Button==mbLeft) {
				if (bix<7)
					//前の空欄で前の月へ
					BefMonAction->Execute();
				else
					//後の空欄で次の月へ
					NxtMonAction->Execute();
			}
		}
	}
}
//---------------------------------------------------------------------------
//週ボタン
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekBtnPaintBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	POINT p = Point(X, Y);
	int bix = -1;
	for (int i=0; i<7; i++) {
		int yp = WekBtnHeight * i;
		TRect brc = Rect(0, yp, WekBtnPaintBox->ClientWidth, yp + WekBtnHeight);
		if (::PtInRect(&brc, p)) {
			bix = i;	break;
		}
	}
	if (bix==-1) return;

	//同じ日付が再度押された
	if (CurDay==WekInf[bix].day) {
		if (Button==mbLeft) {
			TMemo *mp = WekMemo[bix];
			//開く
			if (!MemoExpanded) {
				for (int i=0; i<7; i++) if (i!=bix) WekMemo[i]->Visible = false;
				mp->Align = alClient;
				MemoExpanded = true;
			}
			//戻す
			else {
				for (int i=0; i<7; i++) WekMemo[i]->Align = alNone;
				TAnchors ak;
				ak << akLeft << akRight;
				for (int i=0; i<7; i++) {
					TMemo *mp	= WekMemo[i];
					mp->Top 	= WekBtnHeight * i;
					mp->Height	= WekBtnHeight;
					mp->Visible = true;
					mp->Anchors = ak;
				}
				MemoExpanded = false;
			}
		}

	}
	//違う日付が押された
	else {
		if (MemoExpanded) {
			for (int i=0; i<7; i++) WekMemo[i]->Align = alNone;
			for (int i=0; i<7; i++) {
				TMemo *mp  = WekMemo[i];
				if (i==bix) {
					mp->Align	= alClient;
					mp->Visible = true;
				}
				else
					mp->Visible = false;
			}
		}
		calinf_rec *wp = &WekInf[bix];
		ChangeDate(TDate(wp->year, wp->mon, wp->day));
	}
}
//---------------------------------------------------------------------------
//基本データファイルの読み込み
// 成功すると DataList にデータが設定される
//---------------------------------------------------------------------------
bool __fastcall TMoonMemoForm::LoadDataFile()
{
	bool okflag = false;
	std::unique_ptr<TStringList> fbuf(new TStringList());
	if (FileExists(DataFile)) {
		fbuf->LoadFromFile(DataFile);
		if (fbuf->Count>0) {
			//月メモ用データかをチェック
			UnicodeString dbuf = fbuf->Strings[0];
			do {
				if (dbuf.Length()<11)	break;
				if (dbuf[1]!='0') 		break;
				if (dbuf[11]!='=')		break;
				okflag = true;
			} while (0);

			if (!okflag) msgbox_WARN("不正なデータ内容です\nファイルを選択し直してください");
		}
		else okflag = true;
	}
	else okflag = true;

	if (okflag) {
		DataList->Assign(fbuf.get());
		DataModified = false;
	}

	return okflag;
}
//---------------------------------------------------------------------------
//基本データファイルに指定ファイルをマージ
//---------------------------------------------------------------------------
int __fastcall TMoonMemoForm::MergeDataFile(UnicodeString fnam)
{
	if (!FileExists(fnam)) return 0;

	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->LoadFromFile(fnam);
	int dcnt = 0;
	for (int i=0; i<fbuf->Count; i++) {
		UnicodeString dbuf = fbuf->Strings[i];
		if (dbuf.Length()<11)	continue;
		if (dbuf[11]!='=')		continue;
		if (dbuf.SubString(1, 2).ToIntDef(99) > 2) continue;
		DataList->Add(dbuf);
		dcnt++;
	}

	if (dcnt>0) {
		DataList->Sort();
		int i = 0;
		while (i<DataList->Count-1) {
			UnicodeString dbuf1 = DataList->Strings[i];
			UnicodeString dbuf2 = DataList->Strings[i + 1];
			//重複データを削除
			if (dbuf1==dbuf2) {
				DataList->Delete(i + 1);
			}
			//同日付データを合併
			else if (dbuf1.SubString(1, 10)==dbuf2.SubString(1, 10)) {
				dbuf2.Delete(1, 11);
				DataList->Strings[i] = dbuf1 + "\\n" + dbuf2;
				DataList->Delete(i + 1);
			}
			else i++;
		}
		SetSavYearCombo();
		DataModified = true;
	}

	return dcnt;
}

//---------------------------------------------------------------------------
//データの保存
//---------------------------------------------------------------------------
bool __fastcall TMoonMemoForm::SaveFileEx(TStrings *lst, UnicodeString fnam)
{
	if (fnam.IsEmpty()) return false;

	//保存
	UnicodeString dstr = ExtractFileDir(fnam);
	UnicodeString msg;
	if (DirectoryExists(dstr)) {
		try {
			lst->SaveToFile(fnam, SaveUtf8CheckBox->Checked? TEncoding::UTF8: TEncoding::Default);
		}
		catch (...) {
			msgbox_ERR(msg.sprintf(_T(" [%s] の保存に失敗しました"), fnam.c_str()));
			return false;
		}
	}
	else {
		msgbox_ERR(msg.sprintf(_T("ディレクトリ [%s] が存在しません\n設定を確認してください"), dstr.c_str()));
		return false;
	}

	//バックアップ
	if (!BackupDir.IsEmpty()) {
		if (DirectoryExists(BackupDir)) {
			UnicodeString bnam = BackupDir + "\\" + ExtractFileName(fnam);
			try {
				lst->SaveToFile(bnam, SaveUtf8CheckBox->Checked? TEncoding::UTF8 : TEncoding::Default);
			}
			catch (...) {
				msgbox_ERR(msg.sprintf(_T(" [%s] のバックアップに失敗しました"), ExtractFileName(fnam).c_str()));
				return false;
			}
		}
		else {
			msgbox_ERR("バックアップ先が存在しません\n設定を確認してください");
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//指定した日付のデータを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TMoonMemoForm::GetDayInfo(int idx, int y, int m, int d, TStringList *dlst)
{
	UnicodeString rstr;

	//暦情報
	if (idx==WDIX_KOYOMI) {
		_TCHAR hbuf[31];
		if (CurHoliday->IsHoliday(y, m, d, hbuf)) rstr = hbuf;
		//旧暦、干支、六曜、節気・雑節
		if (KoyomiCheckBox->Checked) {
			UnicodeString kbuf = "旧" + Koyomi->GetInfoStr(y, m, d);
			if (!kbuf.IsEmpty()) {
				if (!rstr.IsEmpty()) rstr.UCAT_TSTR("\r\n");
				rstr += kbuf;
			}
		}
		//拡張情報
		if (ExtInfList->Count>0) {
			UnicodeString hstr = UnicodeString().sprintf(_T("%04u/%02u/%02u"), y, m, d);
			UnicodeString dbuf = ExtInfList->Values[hstr];
			if (!dbuf.IsEmpty()) {
				if (!rstr.IsEmpty()) rstr.UCAT_TSTR("\r\n");
				rstr += dbuf;
			}
		}
	}
	//基本データ
	else {
		if (!dlst) dlst = DataList;
		if (dlst->Count>0)
			rstr = dlst->Values[UnicodeString().sprintf(_T("%02u%04u%02u%02u"), idx, y, m, d)];
	}

	return rstr;
}
//---------------------------------------------------------------------------
//指定した日付へデータを設定
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SetDayInfo(int idx, int y, int m, int d, UnicodeString s)
{
	if (d==0 || idx==WDIX_KOYOMI) return;

	UnicodeString hstr = UnicodeString().sprintf(_T("%02u%04u%02u%02u"), idx, y, m, d);
	int lidx = DataList->IndexOfName(hstr);
	if (s.IsEmpty()) {
		if (lidx!=-1) DataList->Delete(lidx);
	}
	else {
		UnicodeString dbuf = hstr + "=" + s;
		if (lidx!=-1)
			DataList->Strings[lidx] = dbuf;
		else
			DataList->Add(dbuf);
	}
}
//---------------------------------------------------------------------------
//週間データを更新
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SetWekInfo()
{
	int idx = MemRzTabControl->TabIndex;
	for (int i=0; i<7; i++) {
		calinf_rec *wp = &WekInf[i];
		if (wp->changed) {
			int y = wp->year;
			if (idx==WDIX_KINEN) y = 0;
			SetDayInfo(idx, y, wp->mon, wp->day, wp->datstr[idx]);
		}
		wp->changed = false;
	}
}

//---------------------------------------------------------------------------
//日付移動
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ChangeDate(TDateTime dt)
{
	SetWekInfo();
	LstYear = CurYear;
	LstMon  = CurMon;
	LstDay  = CurDay;

	DecodeDate(dt, CurYear, CurMon, CurDay);

	//CurCalRow を更新
	LstCalRow = CurCalRow;
	int wk0 = get_wek(CurYear, CurMon, 1) -1;
	bool flag = false;
	int td = 0;
	for (int i=0; i<MAX_CALBUTTON; i++) {
		if (!flag) {
			if (((i + StrWkComboBox->ItemIndex)%7) == wk0) {
				td = 1;  flag = true;
			}
		}
		if (flag) {
			if (td==CurDay) {
				CurCalRow = i/7; break;
			}
			td++;
		}
	}

	DayChanged(CalPaintBox);
}

//---------------------------------------------------------------------------
//現在の日付位置が変更されたときの処理
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::DayChanged(TObject *Sender)
{
	//------------------------
	//カレンダーの情報を設定
	//------------------------
	unsigned short ty = CurYear;
	unsigned short tm = CurMon;
	unsigned short td = 0;
	int stw = StrWkComboBox->ItemIndex;

	//高速化のため必要なデータのみ抽出しておく
	std::unique_ptr<TStringList> lst_y1(new TStringList());		//予定1
	std::unique_ptr<TStringList> lst_y2(new TStringList());		//予定2
	std::unique_ptr<TStringList> lst_k(new TStringList());		//記念日

	UnicodeString h_y1_c, h_y1_p, h_y1_n;
	h_y1_c.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI1, ty, tm);											//当月
	h_y1_p.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI1, ((tm==1)?  ty - 1 : ty), ((tm==1)? 12 : tm - 1));	//前月
	h_y1_n.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI1, ((tm==12)? ty + 1 : ty), ((tm==12)? 1 : tm + 1));	//翌月

	UnicodeString h_y2_c, h_y2_p, h_y2_n;
	h_y2_c.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI2, ty, tm);
	h_y2_p.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI2, ((tm==1)?  ty - 1 : ty), ((tm==1)? 12 : tm - 1));
	h_y2_n.sprintf(_T("%02u%04u%02u"), WDIX_YOTEI2, ((tm==12)? ty + 1 : ty), ((tm==12)? 1 : tm + 1));

	UnicodeString h_k_c, h_k_p, h_k_n;
	h_k_c.sprintf(_T("%02u0000%02u"), WDIX_KINEN,  tm);
	h_k_p.sprintf(_T("%02u0000%02u"), WDIX_KINEN,  ((tm==1)? 12 : tm - 1));
	h_k_n.sprintf(_T("%02u0000%02u"), WDIX_KINEN,  ((tm==12)? 1 : tm + 1));

	for (int i=0; i<DataList->Count; i++) {
		UnicodeString lbuf = DataList->Strings[i];
		if		(StartsStr(h_y1_c, lbuf) || StartsStr(h_y1_p, lbuf) || StartsStr(h_y1_n, lbuf)) lst_y1->Add(lbuf);
		else if (StartsStr(h_y2_c, lbuf) || StartsStr(h_y2_p, lbuf) || StartsStr(h_y2_n, lbuf)) lst_y2->Add(lbuf);
		else if (StartsStr(h_k_c,  lbuf) || StartsStr(h_k_p,  lbuf) || StartsStr(h_k_n,  lbuf)) lst_k->Add(lbuf);
	}

	//年・月が変化
	if (reqAllUpdate || LstYear!=CurYear || LstMon!=CurMon) {
		int topwk = get_wek(ty, tm, 1);
		int dayn  = get_dayn(ty, tm);
		int stt = 0;
		for (int i=0; i<MAX_CALBUTTON; i++) {
			calinf_rec *cp = &CalInf[i];
			cp->year = cp->mon = cp->day = 0;
			if (stt==0) {
				if (((i + stw)%7) == (topwk - 1)) {
					td	 = 1;
					stt  = 1;
				}
			}
			if (stt==1) {
				cp->year = ty;
				cp->mon  = tm;
				cp->day  = td;
				cp->wek  = get_wek(ty, tm, td);
				cp->holiday = CurHoliday->IsHoliday(ty, tm, td, cp->holname);
				mooninf minf;
				minf.cur_dt = EncodeDate(ty, tm, td) + EncodeTime(CalMoonTime, 0, 0, 0);
				GetMoonInfo(&minf);
				cp->age 		= minf.age;
				cp->phase		= minf.phase;
				cp->illuminated = minf.illuminated;
				cp->datstr[WDIX_YOTEI1] = GetDayInfo(WDIX_YOTEI1, ty, tm, td, lst_y1.get());
				cp->datstr[WDIX_YOTEI2] = GetDayInfo(WDIX_YOTEI2, ty, tm, td, lst_y2.get());
				cp->datstr[WDIX_KINEN]  = GetDayInfo(WDIX_KINEN,   0, tm, td, lst_k.get());
				cp->datstr[WDIX_KOYOMI] = GetDayInfo(WDIX_KOYOMI, ty, tm, td);
				if (td<dayn) td++; else stt = 2;
			}
		}
	}
	//行が変化
	else if (LstCalRow != CurCalRow) {
		//マーク表示のために予定情報を更新しておく
		for (int i=0; i<7; i++) {
			calinf_rec *cp = &CalInf[LstCalRow * 7 + i];
			if (cp->day!=0) {
				cp->datstr[WDIX_YOTEI1] = GetDayInfo(WDIX_YOTEI1, cp->year, cp->mon, cp->day, lst_y1.get());
				cp->datstr[WDIX_YOTEI2] = GetDayInfo(WDIX_YOTEI2, cp->year, cp->mon, cp->day, lst_y2.get());
				cp->datstr[WDIX_KINEN]  = GetDayInfo(WDIX_KINEN,         0, cp->mon, cp->day, lst_k.get());
			}
		}
	}

	//------------------------
	//週間情報を設定
	//------------------------
	int wofs = LstWeek - (stw + 1);  if (wofs<0) wofs += 7;
	TDate lsttop = TDate(LstYear, LstMon, LstDay) - wofs;
	LstWeek  = CurWeek;
	CurWeek  = get_wek(CurYear, CurMon, CurDay);
	wofs = CurWeek - (stw + 1);  if (wofs<0) wofs += 7;
	TDate topday = TDate(CurYear, CurMon, CurDay) - wofs;
	if (reqAllUpdate || lsttop!=topday) {
		//週の先頭の日付が変化した
		DecodeDate(topday, ty, tm, td);
		for (int i=0; i<7; i++) {
			calinf_rec *wp = &WekInf[i];
			wp->year = ty;
			wp->mon  = tm;
			wp->day  = td;
			wp->wek  = get_wek(ty, tm, td);
			wp->holiday = CurHoliday->IsHoliday(ty, tm, td, wp->holname);
			mooninf minf;
			minf.cur_dt = EncodeDate(ty, tm, td) + EncodeTime(CalMoonTime, 0, 0, 0);
			GetMoonInfo(&minf);
			wp->age 		= minf.age;
			wp->phase		= minf.phase;
			wp->illuminated = minf.illuminated;
			wp->datstr[WDIX_YOTEI1] = GetDayInfo(WDIX_YOTEI1, ty, tm, td, lst_y1.get());
			wp->datstr[WDIX_YOTEI2] = GetDayInfo(WDIX_YOTEI2, ty, tm, td, lst_y2.get());
			wp->datstr[WDIX_KINEN]  = GetDayInfo(WDIX_KINEN,   0, tm, td, lst_k.get());
			UnicodeString dbuf;
			dbuf.sprintf(_T("月齢 %5.2f (%6.2f%%"), minf.age, minf.illuminated*100.0);
			dbuf += ((minf.phase<0.50) ? "↑" : "↓");
			dbuf.UCAT_TSTR(")\r\n");
			wp->datstr[WDIX_KOYOMI] = dbuf + GetDayInfo(WDIX_KOYOMI, ty, tm, td);
			WekMemo[i]->Text = ReplaceStr(wp->datstr[MemRzTabControl->TabIndex], "\\n", "\r\n");
			wp->changed 	 = false;
			//日++
			topday += 1;
			DecodeDate(topday, ty, tm, td);
		}
	}

	reqAllUpdate = false;

	if (Sender) {
		HeaderPaintBox->Repaint();
		CalPaintBox->Repaint();
		WekBtnPaintBox->Repaint();
		int bix = 0;
		for (int i=0; i<7; i++) if (WekInf[i].wek==CurWeek) bix = i;
		if (RzPageControl1->ActivePage==WeekSheet && WekMemo[bix]->Visible) WekMemo[bix]->SetFocus();
		Timer1Timer(NULL);
	}
}

//---------------------------------------------------------------------------
TDateTime __fastcall TMoonMemoForm::GetNextMoonPhase(int y, int m, int d, double p) {
	TDateTime rt = EncodeDate(y, m, d);
	NextMoonPhase(&rt, p);
	if ((int)rt == (int)EncodeDate(y, m, d)) {
		rt += 1;
		NextMoonPhase(&rt, p);
	}
	return rt;
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CalPanelResize(TObject *Sender)
{
	HeaderPaintBox->ClientHeight = HeaderPaintBox->ClientWidth/4;
	CalBtnWidth   = CalPaintBox->ClientWidth/7;
	CalBtnHeight  = CalPaintBox->ClientHeight/6;
	CalDateHeight = CalBtnWidth/2;
	CalMarkHeight = CalBtnWidth/4;

	HelpButton->Visible = (HelpButton->Left + HelpButton->Width) < BefYearBtn->Left;
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekMemPanelResize(TObject *Sender)
{
	WekBtnHeight = WekPanel->ClientHeight/7;
	WekDateHeight = std::min(22, WekBtnHeight*2/5);
	WekMarkHeight = std::min(16, WekBtnHeight/4);

	TCanvas *cv = WekBtnPaintBox->Canvas;
	cv->Font->Assign(FontSet[FNTIX_Mark]);
	cv->Font->Height = WekMarkHeight;
	WekMarkWidth = cv->TextWidth("●");

	cv->Font->Assign(FontSet[FNTIX_Date]);
	cv->Font->Height = WekDateHeight;
	WekDateWidth = cv->TextWidth("30 土") + std::max(4, abs(cv->Font->Height)/2);

	for (int i=0; i<7; i++) {
		TMemo *mp	= WekMemo[i];
		mp->Top 	= WekBtnHeight * i;
		mp->Height	= WekBtnHeight;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekSplitterMoved(TObject *Sender)
{
	WekNavPanel->Width = WekMemPanel->Left;
}

//---------------------------------------------------------------------------
//本日のお知らせ
// sound : true = 音を鳴らす (default = true);
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ShowTodayMsg(bool sound)
{
	if (sound && !MsgSoundEdit->Text.IsEmpty()) ::sndPlaySound(MsgSoundEdit->Text.c_str(), SND_ASYNC);

	if (FormShowed && ModalScrCheckBox->Checked) {
		ModalScrForm->Color   = clBlack;
		ModalScrForm->AlphaBlendValue = ModalAlphaEdit->Text.ToIntDef(128);
		ModalScrForm->Visible = true;
		TodayMsgForm->ShowModal();
		ModalScrForm->Visible = false;
	}
	else
		TodayMsgForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TodayMsgActionExecute(TObject *Sender)
{
	TodayMsgForm->Inf1Memo->Text
		= ReplaceStr(GetDayInfo(WDIX_YOTEI1, NowYear, NowMon, NowDay), "\\n", "\r\n");
	TodayMsgForm->Inf2Memo->Text
		= ReplaceStr(GetDayInfo(WDIX_YOTEI2, NowYear, NowMon, NowDay), "\\n", "\r\n");
	TodayMsgForm->Inf3Memo->Text
		= ReplaceStr(GetDayInfo(WDIX_KINEN, 	   0, NowMon, NowDay), "\\n", "\r\n");
	TodayMsgForm->Inf4Memo->Text
		= ReplaceStr(GetDayInfo(WDIX_KOYOMI, NowYear, NowMon, NowDay), "\\n", "\r\n");

	//起動時
	if (Sender==NULL) {
		UnicodeString hstr = UnicodeString().sprintf(_T("%04u/%02u/%02u"), NowYear, NowMon, NowDay);
		if (NoChkCheckBox->Checked ||
			(Chk1CheckBox->Checked && !TodayMsgForm->Inf1Memo->Text.IsEmpty()) ||
			(Chk2CheckBox->Checked && !TodayMsgForm->Inf2Memo->Text.IsEmpty()) ||
			(Chk3CheckBox->Checked && !TodayMsgForm->Inf3Memo->Text.IsEmpty()) ||
			(Chk4CheckBox->Checked && !ExtInfList->Values[hstr].IsEmpty()))
		{
			bool flag = true;
 			if (OnceCheckBox->Checked) {
				//一日一回
				UnicodeString tbuf = IniFile->ReadString("Log", "LastMsgAction");
				if (!tbuf.IsEmpty()) {
					if ((int)StrToDateTime(tbuf) == (int)get_Now()) flag = false;
				}
			}
			if (flag) {
				ShowTodayMsg();
				IniFile->WriteString("Log", "LastMsgAction", FormatDateTime("c", get_Now()));
			}
		}
	}
	//ショートカットからの呼び出し
	else {
		ShowTodayMsg(false);
	}
}

//---------------------------------------------------------------------------
//タイマー処理
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::Timer1Timer(TObject *Sender)
{
	unsigned short ty, tm, td;
	DecodeDate(get_Now(), ty, tm, td);
	DecodeTime(get_Now(), NowHour, NowMin, NowSec, NowMsc);

	//分が変わった
	if (Sender==NULL || NowMin!=LstMin) {
		LstMin = NowMin;
		NowMoonInf.cur_dt = get_Now();
		GetMoonInfo(&NowMoonInf);

		CurMoonInf.cur_dt = EncodeDate(CurYear, CurMon, CurDay) + EncodeTime(NowHour, NowMin, NowSec, NowMsc);
		GetMoonInfo(&CurMoonInf);
	}

	//日付が変わった
	if (Sender==NULL || td != NowDay) {
		NowYear = ty;
		NowMon  = tm;
		NowDay  = td;
	}

	//月情報
	UnicodeString s;
	mooninf minf = SttMoonInfCheckBox->Checked? CurMoonInf : NowMoonInf;
	s.sprintf(_T("%02u:%02u  月齢 %.2f (%.2f%%"), NowHour, NowMin, minf.age, minf.illuminated*100.0);
	s += ((minf.phase<0.50) ? "↑)" : "↓)");
	RzStatusPane1->Caption = s;

	//今日から
	TDate now_dat = EncodeDate(NowYear, NowMon, NowDay);
	TDate cur_dat = EncodeDate(CurYear, CurMon, CurDay);
	int n = cur_dat - now_dat;
	UnicodeString sbuf;
	if (n==0)
		sbuf = " 本日";
	if (n>0)
		sbuf.sprintf(_T("%d日後"), n);
	else if (n<0)
		sbuf.sprintf(_T("%d日前"), -n);
	RzStatusPane2->Caption = sbuf;

	//まで(から)何日1
	sbuf = EmptyStr;
	if (CntDay>0) {
		s = FormatDateTime("<yy/mm/dd> ", CountDate);
		n = CountDate - (CntCurPosCheckBox->Checked? cur_dat : now_dat);
		if (n==0)
			s.UCAT_TSTR(" 当日!");
		else if (n>0)
			s.cat_sprintf(_T("まで%d日"), n);
		else
			s.cat_sprintf(_T("から%d日"), -n);
		sbuf += s;
	}

	//まで(から)何日2
	if (CntDay2>0) {
		s = FormatDateTime("<yy/mm/dd> ", CountDate2);
		n = CountDate2 - (CntCurPosCheckBox->Checked? cur_dat : now_dat);
		if (n==0)
			s.UCAT_TSTR(" 当日!");
		else if (n>0)
			s.cat_sprintf(_T("まで%d日"), n);
		else
			s.cat_sprintf(_T("から%d日"), -n);
		if (!sbuf.IsEmpty()) sbuf.UCAT_TSTR("   ");
		sbuf += s;
	}

	RzStatusPane3->Caption = sbuf;
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RzPageControl1Changing(TObject *Sender, int NewIndex,
          bool &AllowChange)
{
	SetWekInfo();
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RzPageControl1Change(TObject *Sender)
{
	int bix = 0;
	for (int i=0; i<7; i++) if (WekInf[i].wek==CurWeek) bix = i;
	if (RzPageControl1->ActivePage==WeekSheet && WekMemo[bix]->Visible) WekMemo[bix]->SetFocus();
}

//---------------------------------------------------------------------------
//移動ボタン
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::MoveBtnClick(TObject *Sender)
{
	unsigned short ty = CurYear;
	unsigned short tm = CurMon;
	unsigned short td = CurDay;
	TDateTime tmpdat;

	switch (((TComponent *)Sender)->Tag) {
	case -4:	//前の年
		tmpdat = TDateTime(ty - 1, tm, td); break;
	case -3:	//前の月
		tmpdat = TDateTime(ty, tm, td) - td; break;
	case -2:	//前の週
		tmpdat = TDateTime(ty, tm, td) - 7; break;
	case -1:
		tmpdat = TDateTime(ty, tm, td) - 1; break;
	case 0:		//今日
		tmpdat = get_Now(); break;
	case 1:		//次の日
		tmpdat = TDateTime(ty, tm, td) + 1; break;
	case 2:		//次の週
		tmpdat = TDateTime(ty, tm, td) + 7; break;
	case 3:		//次の月
		tmpdat = TDateTime(ty, tm, td) + (get_dayn(ty, tm) - td + 1); break;
	case 4:		//次の年
		tmpdat = TDateTime(ty + 1, tm, td); break;
	}

	ChangeDate(tmpdat);
}

//---------------------------------------------------------------------------
//次の満月へ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::NxtFulActionExecute(TObject *Sender)
{
	TDateTime tmpdat = GetNextMoonPhase(CurYear, CurMon, CurDay, 180.0);
	NxtFulMonItem->Caption = FormatDateTime("'次の満月(&F)' yy/mm/dd hh 時", tmpdat);
	ChangeDate(tmpdat);
}
//---------------------------------------------------------------------------
//次の新月へ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::NxtNewActionExecute(TObject *Sender)
{
	TDateTime tmpdat = GetNextMoonPhase(CurYear, CurMon, CurDay, 0.0);
	NxtFulMonItem->Caption = FormatDateTime("'次の新月(&N)' yy/mm/dd hh 時", tmpdat);
	ChangeDate(tmpdat);
}

//---------------------------------------------------------------------------
//指定した日付へ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::InpDateActionExecute(TObject *Sender)
{
	UnicodeString tstr = EmptyStr;
	if (InputQuery("指定した日付へ", "日付を入力してください", tstr)) {
		if (!tstr.IsEmpty()) {
			int y, m, d;
			int p = tstr.Pos("/");
			if (p>0) {
				UnicodeString s1, s2, s3;
				s1 = tstr.SubString(1, p - 1);  tstr.Delete(1, p);
				p = tstr.Pos("/");
				if (p>0) {
					s2 = tstr.SubString(1, p - 1);  tstr.Delete(1, p);
					s3 = tstr;
					y = s1.ToIntDef(0);
					m = s2.ToIntDef(1);
					d = s3.ToIntDef(1);
				}
				else {
					s2 = tstr;
					y = s1.ToIntDef(0);
					m = s2.ToIntDef(1);
					if (y<=12) {
						d = m;  m = y;  y = CurYear;
					}
					else
						d = CurDay;
				}
			}
			else {
				y = tstr.ToIntDef(0);
				if (y<=12) {
					m = y;  y = CurYear;
				}
				else
					m = CurMon;
				d = CurDay;
			}
			if (y<100) y += 1900;

			try {
				ChangeDate(TDate(y, m, d));
			}
			catch(EConvertError &e) {
				msgbox_ERR("不正な日付です");
			}
		}
	}
}

//---------------------------------------------------------------------------
//ジャンプメニューを作成
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::JumpPopupMenuPopup(TObject *Sender)
{
	int cnt = JumpPopupMenu->Items->Count;
	for (int i=0; i<cnt-2; i++) {
		TMenuItem *mp = JumpPopupMenu->Items->Items[i];
		int n;
		if		(i==6) n = 100;
		else if (i<6)  n = CurMon - (6 - i);
		else if (i>6)  n = CurMon + (i - 6);
		UnicodeString  s;
		if		(n==100) s = "-";
		else if (n>12)   s.sprintf(_T("%4u/%02u"), CurYear + 1,	n - 12);
		else if (n<1)    s.sprintf(_T("%4u/%02u"), CurYear - 1,	n + 12);
		else 			 s.sprintf(_T("%4u/%02u"), CurYear,		n);
		mp->Caption = s;
	}
	JumpPopupMenu->Items->Items[cnt - 2]->Caption = "-";
	JumpPopupMenu->Items->Items[cnt - 1]->Action  = InpDateAction;
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::JumpPopupItemClick(TObject *Sender)
{
	TMenuItem *mp = (TMenuItem *)Sender;
	if (mp->Tag < JumpPopupMenu->Items->Count - 1) {
		UnicodeString tstr = mp->Caption + "/01";
		if (!tstr.IsEmpty()) {
			try {
				ChangeDate(TDate(tstr));
			}
			catch(EConvertError &e) {
				msgbox_ERR("不正な日付です");
			}
		}
	}
}

//---------------------------------------------------------------------------
//ジャンプ右クリックメニューを設定
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CalPopupMenuPopup(TObject *Sender)
{
	//次の満月・新月の日付表示を設定
	TDateTime nxt_ful = GetNextMoonPhase(CurYear, CurMon, CurDay, 180.0);
	TDateTime nxt_new = GetNextMoonPhase(CurYear, CurMon, CurDay,   0.0);
	NxtFulMonItem->Caption = FormatDateTime("'次の満月(&F)' yy/mm/dd hh 時", nxt_ful);
	NxtFulAction->Tag = (int)nxt_ful;
	NxtNewMonItem->Caption = FormatDateTime("'次の新月(&N)' yy/mm/dd hh 時", nxt_new);
	NxtNewAction->Tag = (int)nxt_new;

	//まで(から)何日1の表示を設定
	TDate now_dat = EncodeDate(NowYear, NowMon, NowDay);
	TDate cur_dat = EncodeDate(CurYear, CurMon, CurDay);
	UnicodeString s;
	if (CntDay>0) {
		s = FormatDateTime("yy/mm/dd  ", CountDate);
		int n = CountDate - (CntCurPosCheckBox->Checked? cur_dat : now_dat);
		if (n==0)
			s = EmptyStr;
		else if (n>0)
			s.cat_sprintf(_T("%d日後"), n);
		else
			s.cat_sprintf(_T("%d日前"), -n);
		MovCntDay1Item->Caption = s;
	}
	MovCntDay1Item->Visible = !s.IsEmpty();
	//まで(から)何日2の表示を設定
	s = EmptyStr;
	if (CntDay2>0) {
		s = FormatDateTime("yy/mm/dd  ", CountDate2);
		int n = CountDate2 - (CntCurPosCheckBox->Checked? cur_dat : now_dat);
		if (n==0)
			s = EmptyStr;
		else if (n>0)
			s.cat_sprintf(_T("%d日後"), n);
		else
			s.cat_sprintf(_T("%d日前"), -n);
		MovCntDay2Item->Caption = s;
	}
	MovCntDay2Item->Visible = !s.IsEmpty();

	//本日
	TodayItem->Caption = FormatDateTime("yy/mm/dd ", get_Now()).UCAT_TSTR(" 本日");
	TodayItem->Visible = (cur_dat != now_dat);
}
//---------------------------------------------------------------------------
//指定日付(Sender->Tag)に移動
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ChgDateExecute(TObject *Sender)
{
	ChangeDate(((TComponent *)Sender)->Tag);
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::StatusBar1DblClick(TObject *Sender)
{
	TStatusBar *sp = (TStatusBar*)Sender;
	UnicodeString sbuf = sp->Panels->Items[2]->Text;
	if (!sbuf.IsEmpty()) {
		//まで(から)何日の日付をダブルクリックしたら移動
		TPoint p = sp->ScreenToClient(Mouse->CursorPos);
		int px = p.x - sp->Panels->Items[0]->Width - sp->Panels->Items[1]->Width;

		if (CntDay>0) {
			UnicodeString dstr = FormatDateTime("<yy/mm/dd>", CountDate);
			int p0 = sbuf.Pos(dstr);
			if (p0>0) {
				p0 = sp->Canvas->TextWidth(sbuf.SubString(1, p0 - 1)) + 6;
				int p1 = p0 + sp->Canvas->TextWidth(dstr);
				if (px>=0 && px<=p1) ChangeDate(CountDate);
			}
		}

		if (CntDay2>0) {
			UnicodeString dstr = FormatDateTime("<yy/mm/dd>", CountDate2);
			int p0 = sbuf.Pos(dstr);
			if (p0>0) {
				p0 = sp->Canvas->TextWidth(sbuf.SubString(1, p0 - 1)) + 6;
				int p1 = p0 + sp->Canvas->TextWidth(dstr);
				if (px>=p0 && px<=p1) ChangeDate(CountDate2);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RzStatusPane3DblClick(TObject *Sender)
{
	TRzStatusPane *sp = (TRzStatusPane *)Sender;
	UnicodeString sbuf = sp->Caption;
	if (!sbuf.IsEmpty()) {
		//まで(から)何日の日付をダブルクリックしたら移動
		TPoint p = sp->ScreenToClient(Mouse->CursorPos);
		if (CntDay>0) {
			UnicodeString dstr = FormatDateTime("<yy/mm/dd>", CountDate);
			int p0 = sbuf.Pos(dstr);
			if (p0>0) {
				p0 = sp->Canvas->TextWidth(sbuf.SubString(1, p0 - 1)) + 6;
				int p1 = p0 + sp->Canvas->TextWidth(dstr);
				if (p.x>=0 && p.x<=p1) ChangeDate(CountDate);
			}
		}
		if (CntDay2>0) {
			UnicodeString dstr = FormatDateTime("<yy/mm/dd>", CountDate2);
			int p0 = sbuf.Pos(dstr);
			if (p0>0) {
				p0 = sp->Canvas->TextWidth(sbuf.SubString(1, p0 - 1)) + 6;
				int p1 = p0 + sp->Canvas->TextWidth(dstr);
				if (p.x>=p0 && p.x<=p1) ChangeDate(CountDate2);
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekMemoEnter(TObject *Sender)
{
	TMemo *mp = (TMemo *)Sender;
	if (!mp->ReadOnly && IMECheckBox->Checked) {
		//IMEオン
		HIMC hIMC = ::ImmGetContext(mp->Handle);
		if (hIMC!=0) {
			if (!::ImmGetOpenStatus(hIMC)) ::ImmSetOpenStatus(hIMC, TRUE);
			::ImmReleaseContext(mp->Handle, hIMC);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekMemoExit(TObject *Sender)
{
	TMemo *mp = (TMemo *)Sender;
	if (!mp->ReadOnly && IMECheckBox->Checked) {
		//IMEオフ
		HIMC hIMC = ::ImmGetContext(mp->Handle);
		if (hIMC) {
			if (::ImmGetOpenStatus(hIMC)) ::ImmSetOpenStatus(hIMC, FALSE);
			::ImmReleaseContext(mp->Handle, hIMC);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::WekMemoChange(TObject *Sender)
{
	TMemo *mp = (TMemo *)Sender;
	if (!mp->Focused()) return;

	//週間予定の情報を更新
	calinf_rec *wp = &WekInf[mp->Tag];
	int idx = MemRzTabControl->TabIndex;
	wp->datstr[idx] = ReplaceStr(mp->Text, "\r\n", "\\n");
	wp->changed = true;

	//週間予定のマーク表示部分を更新
	int mw	= WekMarkWidth + 2;
	int ymg = 2;
	int x0	= idx*mw + mw/2;
	if (CurWeek == wp->wek) ymg += 2; else x0 += 4;
	int y0 = WekBtnHeight * mp->Tag + ymg + WekDateHeight + WekMarkHeight/2;
	int x1 = x0 + mw + 2;
	int y1 = y0 + WekMarkHeight + 2;
	TRect rc = Rect(x0, y0, x1, y1);
	::InvalidateRect(WekBtnPaintBox->Parent->Handle, &rc, FALSE);

	//カレンダーのマーク表示部分を更新
	if (wp->mon == CurMon) {
		for (int i=0; i<MAX_CALBUTTON; i++) {
			calinf_rec *cp = &CalInf[i];
			if (cp->day==wp->day) {
				int xp	= (i%7)*CalBtnWidth;
				int yp	= (i/7)*CalBtnHeight;
				TRect rc = Rect(xp, yp, xp + CalBtnWidth, yp + CalBtnHeight);
				rc.Inflate(-1, -1);
				rc.Top += CalDateHeight*4/3 - 1;
				rc.Bottom = rc.Top + CalMarkHeight + 1;
				::InvalidateRect(CalPaintBox->Parent->Handle, &rc, FALSE);
				break;
			}
		}
	}

	DataModified = true;
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::MemRzTabControlChanging(TObject *Sender, int NewIndex,
          bool &AllowChange)
{
	SetWekInfo();
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::MemRzTabControlChange(TObject *Sender)
{
	SetWekInfo();

	int idx = MemRzTabControl->TabIndex;
	for (int i=0; i<7; i++) {
		calinf_rec *wp = &WekInf[i];
		TMemo *mp = WekMemo[i];
		mp->Text = ReplaceStr(wp->datstr[idx], "\\n", "\r\n");
		switch (idx) {
		case WDIX_YOTEI1:
			mp->Color		= ColorSet[COLIX_MemoBg1];
			mp->Font->Color = ColorSet[COLIX_MemoFg1];
			break;
		case WDIX_YOTEI2:
			mp->Color		= ColorSet[COLIX_MemoBg2];
			mp->Font->Color = ColorSet[COLIX_MemoFg2];
			break;
		case WDIX_KINEN:
			mp->Color		= ColorSet[COLIX_MemoBg3];
			mp->Font->Color = ColorSet[COLIX_MemoFg3];
			break;
		case WDIX_KOYOMI:
			mp->Color		= ColorSet[COLIX_MemoBg4];
			mp->Font->Color = ColorSet[COLIX_MemoFg4];
			break;
		}
		mp->ReadOnly = (idx==WDIX_KOYOMI);
		wp->changed = false;
	}
}
//---------------------------------------------------------------------------
//予定切り換え
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ChgMemoActionExecute(TObject *Sender)
{
	SetWekInfo();
	RzPageControl1->ActivePage = WeekSheet;
	MemRzTabControl->TabIndex = ((TComponent *)Sender)->Tag;
	MemRzTabControlChange(NULL);
}
//---------------------------------------------------------------------------
//画面切り換え
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ChgTabActionExecute(TObject *Sender)
{
	TRzTabSheet *tp = NULL;

	switch (((TComponent *)Sender)->Tag) {
	case  0: tp = WeekSheet;  break;
	case 10: tp = Memo1Sheet; break;
	case 11: tp = Memo2Sheet; break;
	case 12: tp = Memo3Sheet; break;
	case 20: tp = FindSheet;  break;
	case 30: tp = OptSheet;   break;
	}

	if (tp) {
		if (tp->TabVisible) RzPageControl1->ActivePage = tp;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindStrComboBoxKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	switch (Key) {
	case VK_RETURN:
		StartFindActionExecute(NULL);
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//検索
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::StartFindActionExecute(TObject *Sender)
{
	UnicodeString wd = trim_ex(FindStrComboBox->Text);
	//検索実行
	FindListBox->Clear();
	for (int i=0; i<DataList->Count; i++) {
		UnicodeString dbuf = DataList->Strings[i];
		if (dbuf.Pos(wd)==0) continue;
		int p = dbuf.Pos("=");	if (p==0) continue;
		UnicodeString hstr = dbuf.SubString(1, p - 1);
		hstr = hstr.SubString(1, 2) + " "
			 + hstr.SubString(3, 4) + "/" + hstr.SubString(7, 2) + "/" + hstr.SubString(9, 2) + " ";
		dbuf.Delete(1, p);
		dbuf = ReplaceStr(dbuf, "\\n", "／");
		FindListBox->Items->Add(hstr + dbuf);
	}

	if (FindListBox->Items->Count>0)
		add_ComboBox_history(FindStrComboBox, wd);	//履歴の更新
	else
		msgbox_OK("見つかりませんでした");
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::StartFindActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !FindStrComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//検索結果の日付に移動
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindListBoxDblClick(TObject *Sender)
{
	if (FindListBox->ItemIndex!=-1) {
		LstYear = CurYear;
		LstMon  = CurMon;
		LstDay  = CurDay;

		UnicodeString s = FindListBox->Items->Strings[FindListBox->ItemIndex];
		int idx = s.SubString( 1, 2).ToIntDef(0);
		int y   = s.SubString( 4, 4).ToIntDef(0);
		int m   = s.SubString( 9, 2).ToIntDef(0);
		int d   = s.SubString(12, 2).ToIntDef(0);
		if (idx>=0 && idx<=2 && m!=0 && d!=0) {
			if (y!=0) CurYear = y;
			CurMon	= m;
			CurDay	= d;
			MemRzTabControl->TabIndex = idx;
			DayChanged(WekBtnPaintBox);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindListBoxKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	switch (Key) {
	case VK_RETURN:
		FindListBoxDblClick(NULL);
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//検索結果の選択項目をコピー
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindCopySelActionExecute(TObject *Sender)
{
	if (FindListBox->ItemIndex!=-1) {
		Clipboard()->AsText = FindListBox->Items->Strings[FindListBox->ItemIndex];
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindCopySelActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (FindListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//検索結果の全ての項目をコピー
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindCopyAllActionExecute(TObject *Sender)
{
	Clipboard()->AsText = FindListBox->Items->Text;
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::FindCopyAllActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (FindListBox->Count>0);
}

//---------------------------------------------------------------------------
//メモのタイトル変更
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptMemTitEditChange(TObject *Sender)
{
	TEdit *ep = (TEdit *)Sender;
	TRzTabSheet *tp;

	int n = ep->Tag;
	switch (n) {
	case 0:  tp = Memo1Sheet; break;
	case 1:  tp = Memo2Sheet; break;
	case 2:  tp = Memo3Sheet; break;
	default: tp = NULL;
	}

	if (tp) {
		MemoTitle[n]   = ep->Text;
		tp->Caption    = AdjustTabTitle(MemoTitle[n]);
		tp->TabVisible = !MemoTitle[n].IsEmpty();
	}
}

//---------------------------------------------------------------------------
//切り替えボタンの表示変更
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TabStrEditChange(TObject *Sender)
{
	TEdit *ep = (TEdit *)Sender;
	TabTitle[ep->Tag] = ep->Text;
	MemRzTabControl->Tabs->Items[ep->Tag]->Caption = AdjustTabTitle(TabTitle[ep->Tag]);
}
//---------------------------------------------------------------------------
//配色一覧の描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptColListBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox *)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->FillRect(Rect);

	//カラー
	TRect rc = Rect;  rc.Right = rc.Left + 30;
	cv->Brush->Color = ColorSet[Index];
	cv->FillRect(rc);

	//説明
	UnicodeString lbuf = lp->Items->Strings[Index];
	bool brk = false;
	if (StartsStr("|", lbuf)) {
		brk = true;
		lbuf.Delete(1, 1);
	}

	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->TextOut(Rect.Left + 34, Rect.Top + 2, lbuf);

	//区切り線
	if (brk) {
		cv->Pen->Color = clGrayText;
		cv->Pen->Style = psSolid;
		cv->Pen->Width = 1;
		cv->MoveTo(Rect.Left,  Rect.Top);
		cv->LineTo(Rect.Right, Rect.Top);
	}
}

//---------------------------------------------------------------------------
//色変更を反映
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::UpdateColor(int idx, TColor col)
{
	if (idx!=-1) ColorSet[idx] = col;

	if (idx==-1 || idx==COLIX_BackGr) {
		TColor bg = ColorSet[COLIX_BackGr];
		Color									= bg;
		RzPageControl1->TabColors->Unselected	= bg;
		MemRzTabControl->TabColors->Unselected	= bg;
	}

	if (idx==-1 || idx==COLIX_ForeGr) {
		TColor fg = ColorSet[COLIX_ForeGr];
		Font->Color 							= fg;
		RzPageControl1->TextColors->Selected	= fg;
		RzPageControl1->TextColors->Unselected	= fg;
		MemRzTabControl->TextColors->Selected	= fg;
		MemRzTabControl->TextColors->Unselected = fg;
	}

	ToDoMemo1->Color			 = ColorSet[COLIX_MemoBgX];
	ToDoMemo1->Font->Color		 = ColorSet[COLIX_MemoFgX];
	ToDoMemo2->Color			 = ColorSet[COLIX_MemoBgY];
	ToDoMemo2->Font->Color		 = ColorSet[COLIX_MemoFgY];
	ToDoMemo3->Color			 = ColorSet[COLIX_MemoBgZ];
	ToDoMemo3->Font->Color		 = ColorSet[COLIX_MemoFgZ];
	FindListBox->Color			 = ColorSet[COLIX_FindBg];
	FindListBox->Font->Color	 = ColorSet[COLIX_FindFg];

	RzStatusBar1->Font->Color		 = ColorSet[COLIX_ForeGr];
	RzStatusBar1->GradientColorStart = ColorSet[COLIX_BackGr];
	RzStatusBar1->GradientColorStop  = ColorSet[COLIX_BackGr];

	if (idx!=-1) OptColListBox->Repaint();
}

//---------------------------------------------------------------------------
//色参照
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefColBtnClick(TObject *Sender)
{
	int idx = OptColListBox->ItemIndex;
	ColorDialog1->Color = ColorSet[idx];
	if (ColorDialog1->Execute()) {
		UpdateColor(idx, ColorDialog1->Color);
		OptionChanged(NULL);
	}
}
//---------------------------------------------------------------------------
//スポイトでマウス位置のピクセル色を取得
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SpuitTimerTimer(TObject *Sender)
{
	HDC hDc = ::GetDC(0);
	if (hDc) {
		COLORREF c = ::GetPixel(hDc, Mouse->CursorPos.x, Mouse->CursorPos.y);
		::ReleaseDC(0, hDc);
		SpuitPanel->Color = (TColor)c;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft && OptColListBox->ItemIndex!=-1) {
		SpuitImage->Visible = false;
		Screen->Cursor		= crSpuitTool;
		SpuitTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SpuitImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (SpuitTimer->Enabled) {
		Screen->Cursor		= crDefault;
		SpuitTimer->Enabled = false;
		TColor col = SpuitPanel->Color;
		SpuitPanel->Color	= clBtnFace;
		SpuitImage->Visible = true;
		UpdateColor(OptColListBox->ItemIndex, col);
		OptionChanged(NULL);
	}
}
//---------------------------------------------------------------------------
//配色のインポート
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::InpColBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "配色のインポート";
	OpenDialog1->Filter 	= "設定ファイル (*.ini)|*.INI";
	OpenDialog1->DefaultExt = "INI";
	OpenDialog1->InitialDir = ExeDir;
	OpenDialog1->FileName	= "*.INI";
	if (OpenDialog1->Execute()) {
		std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(OpenDialog1->FileName));
		ReadColors(inp_file.get());
		UpdateColor();
		OptionChanged(NULL);
	}
}
//---------------------------------------------------------------------------
//配色のエクスポート
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ExpColBtnClick(TObject *Sender)
{
	SaveDialog1->Title		= "配色のエクスポート";
	SaveDialog1->Filter		= "設定ファイル (*.ini)|*.INI";
	SaveDialog1->DefaultExt	= "INI";
	SaveDialog1->InitialDir = ExeDir;
	SaveDialog1->FileName	= "Colors.INI";
	if (SaveDialog1->Execute()) {
		Screen->Cursor = crHourGlass;
		std::unique_ptr<UsrIniFile> exp_file(new UsrIniFile(SaveDialog1->FileName));
		WriteColors(exp_file.get());
		if (!exp_file->UpdateFile(true)) msgbox_ERR("保存に失敗しました");
		Screen->Cursor = crDefault;
	}
}

//---------------------------------------------------------------------------
//フォント一覧の描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptFontListBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox *)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->Font->Color  = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->FillRect(Rect);

	int xp = Rect.Left + 2;
	int yp = Rect.Top + 2;
	cv->TextOut(xp, yp, lp->Items->Strings[Index]);
	xp += cv->TextWidth("予定／メモのテキスト") + 20;
	TFont *f = FontSet[Index];

	//フォントサイズ
	cv->TextOut(xp, yp, f->Size);
	xp += (cv->TextWidth("口")*2);

	//フォント名
	cv->Font->Assign(f);
	cv->Font->Size	= lp->Font->Size;
	cv->Font->Color = State.Contains(odSelected)? clHighlightText : clWindowText;
	cv->TextOut(xp, yp, f->Name);
}

//---------------------------------------------------------------------------
//フォント参照
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefFontBtnClick(TObject *Sender)
{
	int idx = OptFontListBox->ItemIndex;
	FontDialog1->Font->Assign(FontSet[idx]);
	if (FontDialog1->Execute()) {
		FontSet[idx]->Assign(FontDialog1->Font);
		if (idx==FNTIX_Option) {
			if (FontSet[idx]->Size>10) FontSet[idx]->Size = 10;
			FontSet[idx]->Color = clWindowText;
		}
		//画面に反映
		Font->Assign(FontSet[FNTIX_Base]);
		Font->Color = ColorSet[COLIX_ForeGr];
		OptFontListBox->Repaint();

		for (int i=0; i<7; i++) WekMemo[i]->Font->Assign(FontSet[FNTIX_Memo]);

		ToDoMemo1->Font->Assign(FontSet[FNTIX_Memo]);
		ToDoMemo2->Font->Assign(FontSet[FNTIX_Memo]);
		ToDoMemo3->Font->Assign(FontSet[FNTIX_Memo]);
		ToDoMemo1->Color	   = ColorSet[COLIX_MemoBgX];
		ToDoMemo1->Font->Color = ColorSet[COLIX_MemoFgX];
		ToDoMemo2->Color	   = ColorSet[COLIX_MemoBgY];
		ToDoMemo2->Font->Color = ColorSet[COLIX_MemoFgY];
		ToDoMemo3->Color	   = ColorSet[COLIX_MemoBgZ];
		ToDoMemo3->Font->Color = ColorSet[COLIX_MemoFgZ];

		FindListBox->Font->Assign(FontSet[FNTIX_Find]);
		FindListBox->Font->Color = ColorSet[COLIX_FindFg];

		RzPageControl2->Font->Assign(FontSet[FNTIX_Option]);

		InitStatusBar();
		CalBottomPanel->Repaint();
		WekBottomPanel->Repaint();
		OptionChanged(NULL);
	}
}

//---------------------------------------------------------------------------
//データファイルの選択
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefDataBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "予定データファイルの選択";
	OpenDialog1->Filter 	= "テキスト (*.txt)|*.TXT";
	OpenDialog1->DefaultExt = "TXT";
	OpenDialog1->InitialDir = DataDir;
	OpenDialog1->FileName	= "*.TXT";
	if (OpenDialog1->Execute()) {
		if (!DataFile.IsEmpty()) {
			SetWekInfo();
			DataList->Sort();
			SaveFileEx(DataList, DataFile);
		}

		DataFile = OpenDialog1->FileName;
		DataDir  = ExtractFileDir(DataFile);
		if (LoadDataFile())
			OptDatFileEdit->Text = DataFile;
		else
			DataFile = OptDatFileEdit->Text;
		OptionChanged(NULL);
	}
}
//---------------------------------------------------------------------------
//拡張情報ファイルの選択
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefInfoBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "拡張情報ファイルの選択";
	OpenDialog1->Filter 	= "テキスト (*.txt)|*.TXT";
	OpenDialog1->DefaultExt = "TXT";
	OpenDialog1->InitialDir = DataDir;
	OpenDialog1->FileName	= "*.TXT";
	if (OpenDialog1->Execute()) {
		if (FileExists(OpenDialog1->FileName)) {
			ExtInfFile = OpenDialog1->FileName;
			ExtInfList->LoadFromFile(ExtInfFile);
			OptInfFileEdit->Text = ExtInfFile;
		}
		OptionChanged(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptInfFileEditKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	switch (Key) {
	case VK_DELETE:
		OptInfFileEdit->Text = ExtInfFile = EmptyStr;
		OptInfFileEdit->Repaint();
		ExtInfList->Clear();
		OptionChanged(NULL);
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//メモファイルの選択
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefMemo1BtnClick(TObject *Sender)
{
	int n = ((TComponent *)Sender)->Tag;
	TMemo *mp;
	TEdit *ep;

	switch (n) {
	case 0: ep = OptMemFile1Edit; mp = ToDoMemo1; break;
	case 1: ep = OptMemFile2Edit; mp = ToDoMemo2; break;
	case 2: ep = OptMemFile3Edit; mp = ToDoMemo3; break;
	}

	OpenDialog1->Title		= "メモファイルの選択";
	OpenDialog1->Filter 	= "テキスト (*.txt)|*.TXT";
	OpenDialog1->DefaultExt = "TXT";
	OpenDialog1->InitialDir = DataDir;
	OpenDialog1->FileName	= "*.TXT";

	if (OpenDialog1->Execute()) {
		SaveFileEx(mp->Lines, MemoFile[n]);
		mp->Lines->Clear();
		MemoFile[n] = OpenDialog1->FileName;
		ep->Text	= MemoFile[n];
		if (FileExists(MemoFile[n])) {
			mp->Lines->LoadFromFile(MemoFile[n]);
			OptionChanged(NULL);
		}
	}
}
//---------------------------------------------------------------------------
//バックアップ先の選択
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefBackupBtnClick(TObject *Sender)
{
	UnicodeString dirstr = BackupDir;
	if (SelectDirectory("バックアップ先の選択", "", dirstr, TSelectDirExtOpts() << sdNewUI, this)) {
		if (!dirstr.IsEmpty()) {
			BackupDir = dirstr;
			OptBackupDirEdit->Text = BackupDir;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptBackupDirEditKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	switch (Key) {
	case VK_DELETE:
		OptBackupDirEdit->Text = BackupDir = EmptyStr;
		OptBackupDirEdit->Repaint();
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//お知らせ音の指定
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::RefSndBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "お知らせ音の指定";
	OpenDialog1->Filter 	= "サウンド (*.wav)|*.WAV";
	OpenDialog1->DefaultExt = "WAV";
	OpenDialog1->InitialDir = ExeDir;
	OpenDialog1->FileName	= "*.WAV";
	if (OpenDialog1->Execute()) {
		if (FileExists(OpenDialog1->FileName)) {
			MsgSoundEdit->Text = OpenDialog1->FileName;
			::sndPlaySound(MsgSoundEdit->Text.c_str(), SND_ASYNC);
		}
	}
}
//---------------------------------------------------------------------------
//お知らせのテスト表示
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TestMsgBtnClick(TObject *Sender)
{
	ShowTodayMsg();
}

//---------------------------------------------------------------------------
//タブスタイルの変更
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TabStyle1ComboBoxChange(TObject *Sender)
{
	switch (TabStyle1ComboBox->ItemIndex) {
	case 0: RzPageControl1->TabStyle = tsBackSlant; 	break;
	case 1: RzPageControl1->TabStyle = tsCutCorner;		break;
	case 2: RzPageControl1->TabStyle = tsDoubleSlant;	break;
	case 3: RzPageControl1->TabStyle = tsRoundCorners;	break;
	case 4: RzPageControl1->TabStyle = tsSingleSlant;	break;
	case 5: RzPageControl1->TabStyle = tsSquareCorners;	break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TabStyle2ComboBoxChange(TObject *Sender)
{
	switch (TabStyle2ComboBox->ItemIndex) {
	case 0: MemRzTabControl->TabStyle = tsBackSlant; 		break;
	case 1: MemRzTabControl->TabStyle = tsCutCorner;		break;
	case 2: MemRzTabControl->TabStyle = tsDoubleSlant;		break;
	case 3: MemRzTabControl->TabStyle = tsRoundCorners;		break;
	case 4: MemRzTabControl->TabStyle = tsSingleSlant;		break;
	case 5: MemRzTabControl->TabStyle = tsSquareCorners;	break;
	}
}

//---------------------------------------------------------------------------
//タブスタイルの描画
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::TabStyleComboBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas   *cv = cp->Canvas;
	bool btm_sw = cp->Tag==1;

	cv->Brush->Color = State.Contains(odSelected)? clHighlight : clWindow;
	cv->FillRect(Rect);

	TRect rc = Rect;
	rc.Inflate(0, -2);
	rc.Left += 4;
	rc.Right = rc.Left + 60;
	int x0 = rc.Left;
	int y0 = rc.Top;
	int x1 = rc.Right;
	int y1 = rc.Bottom;
	int h  = rc.Height();
	int s  = btm_sw? -1 : 1;
	if (btm_sw) std::swap(y0, y1);
	TPoint shape[8];

	switch (Index) {
	case 0:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0 + h, y0);
		shape[2] = Point(x1 - 2, y0);
		shape[3] = Point(x1,	 y0 + 2*s);
		shape[4] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 4);
		break;
	case 1:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0,	 y0);
		shape[2] = Point(x1 - 4, y0);
		shape[3] = Point(x1,	 y0 + 4*s);
		shape[4] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 4);
		break;
	case 2:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0 + h, y0);
		shape[2] = Point(x1 - h, y0);
		shape[3] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 3);
		break;
	case 3:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0,	 y0 + 2*s);
		shape[2] = Point(x0 + 2, y0);
		shape[3] = Point(x1 - 2, y0);
		shape[4] = Point(x1,	 y0 + 2*s);
		shape[5] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 5);
		break;
	case 4:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0,	 y0 + 2*s);
		shape[2] = Point(x0 + 2, y0);
		shape[3] = Point(x1 - h, y0);
		shape[4] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 4);
		break;
	case 5:
		shape[0] = Point(x0,	 y1);
		shape[1] = Point(x0,	 y0);
		shape[2] = Point(x1,	 y0);
		shape[3] = Point(x1,	 y1);
		cv->Pen->Color	 = RzPageControl1->FlatColor;
		cv->Pen->Style	 = psSolid;
		cv->Brush->Color = ColorSet[COLIX_BackGr];
		cv->Polygon(shape, 3);
		break;
	}
}

//---------------------------------------------------------------------------
//ショートカットキーのカテゴリ選択
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CategoryComboBoxChange(TObject *Sender)
{
	for (int i=0; i<ActionList1->ActionCount; i++) {
		TAction *ap = (TAction*)ActionList1->Actions[i];
		if (ap->Category == CategoryComboBox->Items->Strings[CategoryComboBox->ItemIndex]) {
			ShortCutList->TopIndex	= i;
			ShortCutList->ItemIndex = i;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ShortCutListClick(TObject *Sender)
{
	int idx = ((TListBox *)Sender)->ItemIndex;
	if (idx!=-1) {
		TAction *ap = (TAction*)ActionList1->Actions[idx];
		CategoryComboBox->ItemIndex = CategoryComboBox->Items->IndexOf(ap->Category);
		UnicodeString skstr = ShortCutToText(ap->ShortCut);
		ShiftCheckBox->Checked = (skstr.Pos("Shift+")>0);
		skstr = ReplaceStr(skstr, "Shift+", EmptyStr);
		CtrlCheckBox->Checked  = (skstr.Pos("Ctrl+")>0);
		skstr = ReplaceStr(skstr, "Ctrl+", EmptyStr);
		AltCheckBox->Checked   = (skstr.Pos("Alt+")>0);
		skstr = ReplaceStr(skstr, "Alt+", EmptyStr);
		KeyComboBox->ItemIndex = KeyComboBox->Items->IndexOf(skstr);
	}
}

//---------------------------------------------------------------------------
//ショートカットキー変更
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::SKChgBtnClick(TObject *Sender)
{
	int idx = ShortCutList->ItemIndex;
	if (idx!=-1) {
		UnicodeString tmpstr = ShortCutList->Items->Strings[idx];
		int p = tmpstr.LastDelimiter("\t");
		if (p>0) tmpstr = tmpstr.SubString(1, p);
		UnicodeString skstr;
		if (ShiftCheckBox->Checked) skstr.UCAT_TSTR("Shift+");
		if (CtrlCheckBox->Checked)  skstr.UCAT_TSTR("Ctrl+");
		if (AltCheckBox->Checked)   skstr.UCAT_TSTR("Alt+");
		skstr += KeyComboBox->Text;

		try {
			//重複チェック
			for (int i=0; i<ActionList1->ActionCount; i++) {
				TAction *ap = (TAction*)ActionList1->Actions[i];
				if (i==idx) continue;
				if (ap->ShortCut == TextToShortCut(skstr)) throw Exception("");
			}
			ShortCutList->Items->Strings[idx] = tmpstr + skstr;
			TAction *ap  = (TAction*)ActionList1->Actions[idx];
			ap->ShortCut = TextToShortCut(skstr);
		}
		catch(...) {
			msgbox_WARN("ショートカットキーが重複しています");
		}
	}
}
//---------------------------------------------------------------------------
//ショートカットキー解除
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::ClrBtnClick(TObject *Sender)
{
	int idx=ShortCutList->ItemIndex;
	if (idx!=-1) {
		UnicodeString tmpstr = ShortCutList->Items->Strings[idx];
		int p = tmpstr.LastDelimiter("\t");
		if (p>0) tmpstr = tmpstr.SubString(1, p);
		ShortCutList->Items->Strings[idx] = tmpstr;
		TAction *ap  = (TAction*)ActionList1->Actions[idx];
		ap->ShortCut = TextToShortCut(EmptyStr);
		ShiftCheckBox->Checked = false;
		CtrlCheckBox->Checked  = false;
		AltCheckBox->Checked   = false;
		KeyComboBox->ItemIndex = -1;
	}
}


//---------------------------------------------------------------------------
//オプション変更有り
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::OptionChanged(TObject *Sender)
{
	if (!Visible) return;
	if (RzPageControl1->ActivePage != OptSheet) return;

	if (MonTimComboBox->ItemIndex!=-1) CalMoonTime = MonTimComboBox->ItemIndex;

	if (!CntYear1Edit->Text.IsEmpty() && !CntMon1Edit->Text.IsEmpty() && !CntDay1Edit->Text.IsEmpty()) {
		CntYear = CntYear1Edit->Text.ToIntDef(CntYear);
		if (CntYear<100) CntYear += ((CntYear<50) ? 2000 : 1900);
		CntMon = CntMon1Edit->Text.ToIntDef(CntMon);
		CntDay = CntDay1Edit->Text.ToIntDef(CntDay);
		try {
			CountDate = EncodeDate(CntYear, CntMon, CntDay);
			MovCntDay1Item->Caption = FormatDateTime("yy/mm/dd", CountDate);
			CntDay1Action->Tag = int(CountDate);
		}
		catch(EConvertError &e) {
			CntYear1Edit->Text = EmptyStr;
			CntMon1Edit->Text  = EmptyStr;
			CntDay1Edit->Text  = EmptyStr;
			CntYear = CntMon = CntDay = 0;
		}
	}
	else {
		CntYear = CntMon = CntDay = 0;
	}

	if (!CntYear2Edit->Text.IsEmpty() && !CntMon2Edit->Text.IsEmpty() && !CntDay2Edit->Text.IsEmpty()) {
		CntYear2 = CntYear2Edit->Text.ToIntDef(CntYear2);
		if (CntYear2<100) CntYear2 += ((CntYear2<50)? 2000 : 1900);
		CntMon2 = CntMon2Edit->Text.ToIntDef(CntMon2);
		CntDay2 = CntDay2Edit->Text.ToIntDef(CntDay2);
		try {
			CountDate2 = EncodeDate(CntYear2, CntMon2, CntDay2);
			MovCntDay2Item->Caption = FormatDateTime("yy/mm/dd", CountDate2);
			CntDay2Action->Tag = int(CountDate2);
		}
		catch(EConvertError &e) {
			CntYear2Edit->Text = EmptyStr;
			CntMon2Edit->Text  = EmptyStr;
			CntDay2Edit->Text  = EmptyStr;
			CntYear2 = CntMon2 = CntDay2 = 0;
		}
	}
	else {
		CntYear2 = CntMon2 = CntDay2 = 0;
	}

	reqAllUpdate = true;
	DayChanged(NULL);
	MemRzTabControlChange(NULL);
	HeaderPaintBox->Repaint();
	CalPaintBox->Repaint();
	WekBtnPaintBox->Repaint();
}

//---------------------------------------------------------------------------
//データ整理
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::DatSavExecute(TObject *Sender)
{
	SaveDialog1->Title		= "今年以前のデータを整理して保存";
	SaveDialog1->Filter		= "テキスト (*.txt)|*.TXT";
	SaveDialog1->DefaultExt	= "TXT";
	SaveDialog1->InitialDir = DataDir;
	SaveDialog1->FileName	= "*.TXT";
	if (SaveDialog1->Execute()) {
		if (SaveDialog1->FileName!=DataFile) {
			std::unique_ptr<TStringList> tmp_lst(new TStringList());
			std::unique_ptr<TStringList> old_lst(new TStringList());
			tmp_lst->Assign(DataList);
			int i = 0;
			while (i<tmp_lst->Count) {
				UnicodeString dbuf = tmp_lst->Strings[i];
				int id = dbuf.SubString(1, 2).ToIntDef(99);
				int y  = dbuf.SubString(3, 4).ToIntDef(9999);
				if (id!=WDIX_KINEN && y<NowYear) {
					old_lst->Add(dbuf);
					tmp_lst->Delete(i);
				}
				else i++;
			}
			try {
				old_lst->SaveToFile(SaveDialog1->FileName,
					SaveUtf8CheckBox->Checked? TEncoding::UTF8 : TEncoding::Default);
				DataList->Clear();
				for (int i= 0; i<tmp_lst->Count; i++) DataList->Add(tmp_lst->Strings[i]);
				DataModified = true;
				msgbox_OK("整理保存が完了しました");
			}
			catch (...) {
				msgbox_ERR("データの保存に失敗しました");
			}
		}
		else
			msgbox_WARN("現データと同名のファイルには保存できません");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::DatSavActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (!DataFile.IsEmpty() && SavYearComboBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//データのマージ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::MergeBtnClick(TObject *Sender)
{
	OpenDialog1->Title		= "マージするデータファイルの選択";
	OpenDialog1->Filter 	= "テキスト (*.txt)|*.TXT";
	OpenDialog1->DefaultExt = "TXT";
	OpenDialog1->InitialDir = DataDir;
	OpenDialog1->FileName	= "*.TXT";
	if (OpenDialog1->Execute()) {
		int dcnt = MergeDataFile(OpenDialog1->FileName);
		if (dcnt>0) {
			OptionChanged(NULL);
			msgbox_OK(UnicodeString().sprintf(_T("%u個の項目をマージしました"), dcnt));
		}
		else
			msgbox_WARN("マージするデータがありません");
	}
}

//---------------------------------------------------------------------------
//Webページへ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::UrlLabelClick(TObject *Sender)
{
	::ShellExecute(NULL, _T("open"), SUPPORT_URL, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
//ヘルプ
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::HelpActionExecute(TObject *Sender)
{
	Application->HelpShowTableOfContents();
}

//---------------------------------------------------------------------------
//状況感知ヘルプ
//---------------------------------------------------------------------------
bool __fastcall TMoonMemoForm::FormHelp(WORD Command, int Data, bool &CallHelp)
{
	if (Command!=HELP_FINDER) {
		//ショートカットにF1が指定されていたら状況感知型ヘルプを抑止
		for (int i=0; i<ActionList1->ActionCount; i++) {
			TAction *ap = (TAction*)ActionList1->Actions[i];
			if (ap->ShortCut == ShortCut(VK_F1, TShiftState())) {
				CallHelp = false;	break;
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//終了
//---------------------------------------------------------------------------
void __fastcall TMoonMemoForm::CloseActionExecute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

