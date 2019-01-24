//----------------------------------------------------------------------//
//	îƒópã§í ä÷êî														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFuncH
#define UsrFuncH

//---------------------------------------------------------------------------
#include <System.StrUtils.hpp>
#include <Vcl.StdCtrls.hpp>

//---------------------------------------------------------------------------
extern UnicodeString DebugTimeStr;

//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString src);
int pos_i(UnicodeString wd, UnicodeString s);

void add_ComboBox_history(TComboBox *cp, UnicodeString kwd);

void make_key_list(TStrings *lst);

UnicodeString get_VersionStr(UnicodeString fnam, bool is_num = false);
int get_VersionNo();

void msgbox_ERR(  UnicodeString msg, UnicodeString tit = EmptyStr);
void msgbox_WARN( UnicodeString msg, UnicodeString tit = EmptyStr);
void msgbox_OK(   UnicodeString msg, UnicodeString tit = EmptyStr);
bool msgbox_Y_N(  UnicodeString msg, UnicodeString tit = EmptyStr);
int  msgbox_Y_N_C(UnicodeString msg, UnicodeString tit = EmptyStr);

TDateTime get_Now();

bool WIC_resize_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	double ratio, unsigned int s_wd = 0, unsigned int s_hi = 0, int s_opt = 0);

//---------------------------------------------------------------------------
#define UCAT_TSTR(str)	cat_sprintf(_T("%s"), _T(str))

//---------------------------------------------------------------------------
#endif
