//----------------------------------------------------------------------//
//	�ėp���ʊ֐�														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <utilcls.h>
#include "UserFunc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UnicodeString DebugTimeStr;

//---------------------------------------------------------------------------
//�S�p�X�y�[�X���l�������g���~���O
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString src)
{
	for (;;) {
		if (src.IsEmpty()) break;
		if (src.Pos("�@")==1) {
			src.Delete(1, 1); continue;
		}
		if (src[1]==' ' || src[1]=='\t') {
			src.Delete(1, 1); continue;
		}
		break;
	}
	for (;;) {
		if (src.IsEmpty()) break;
		int n = src.Length();
		if (n>1) {
			if (src.SubString(n - 1, 1)=="�@") {
				src.Delete(n - 1, 1); continue;
			}
		}
		if (src[n]==' ' || src[n]=='\t') {
			src.Delete(n, 1); continue;
		}
		break;
	}
	return src;
}

//---------------------------------------------------------------------------
//�召��������ʂ����ɕ���������� (2�o�C�g�����Ή�)
//  wd: ������
//   s: �Ώە�����
//---------------------------------------------------------------------------
int pos_i(UnicodeString wd, UnicodeString s)
{
	if (wd.IsEmpty()) return 0;
	s  = s.UpperCase();
	wd = wd.UpperCase();
	return s.Pos(wd);
}

//---------------------------------------------------------------------------
//ComboBox �ɗ�����ǉ�
//---------------------------------------------------------------------------
void add_ComboBox_history(TComboBox *cp, UnicodeString kwd)
{
	if (!kwd.IsEmpty()) {
		TStrings *lp = cp->Items;
		int idx = lp->IndexOf(kwd);
		if (idx!=0) {
			if (idx>0) lp->Delete(idx);
			lp->Insert(0, kwd);	//�擪�ɒǉ�
		}
		cp->Text = kwd;
	}
}

//---------------------------------------------------------------------------
//�L�[���X�g�̍쐬
//---------------------------------------------------------------------------
void make_key_list(TStrings *lst)
{
	lst->Clear();

	for (int i=0; i<26; i++) lst->Add(UnicodeString().sprintf(_T("%c"), 'A' + i));	//�p��
	for (int i=0; i<10; i++) lst->Add(UnicodeString().sprintf(_T("%c"), '0' + i));	//����
	for (int i=0; i<12; i++) lst->Add(UnicodeString().sprintf(_T("F%u"), i  + 1));	//F�L�[

	lst->Add("Del");
	lst->Add("Ins");
	lst->Add("BkSp");
	lst->Add("PgUp");
	lst->Add("PgDn");
	lst->Add("Home");
	lst->Add("End");
	lst->Add("Left");
	lst->Add("Up");
	lst->Add("Right");
	lst->Add("Down");
	lst->Add("Tab");
	lst->Add("Esc");
	lst->Add("Enter");
	lst->Add("Space");
}

//---------------------------------------------------------------------------
//�A�v���P�[�V�����̐��i�o�[�W�����擾
// is_num: ���� true = "nnn"/ false = "n.n.n.0" (default = false)
//---------------------------------------------------------------------------
UnicodeString get_VersionStr(UnicodeString fnam, bool is_num)
{
	UnicodeString verstr;
	unsigned mj, mi, bl;
	if (GetProductVersion(fnam, mj, mi, bl)) {
		verstr.sprintf(is_num? _T("%u%u%u") : _T("%u.%u.%u.0"), mj, mi, bl);
	}
	return verstr;
}
//---------------------------------------------------------------------------
int get_VersionNo()
{
	return get_VersionStr(Application->ExeName, true).ToIntDef(0);
}

//---------------------------------------------------------------------------
//�G���[���b�Z�[�W
//---------------------------------------------------------------------------
void msgbox_ERR(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�G���[";	//�f�t�H���g
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------
//�x�����b�Z�[�W
//---------------------------------------------------------------------------
void msgbox_WARN(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�x��";	//�f�t�H���g
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK|MB_ICONWARNING);
}

//---------------------------------------------------------------------------
//���b�Z�[�W
//---------------------------------------------------------------------------
void msgbox_OK(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�m�F";	//�f�t�H���g
	Application->MessageBox(msg.c_str(), tit.c_str(), MB_OK);
}
//---------------------------------------------------------------------------
//�m�F���b�Z�[�W
//---------------------------------------------------------------------------
bool msgbox_Y_N(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�m�F";	//�f�t�H���g
	int res = Application->MessageBox(msg.c_str(), tit.c_str(), MB_YESNO|MB_ICONQUESTION);
	return (res==IDYES);
}
//---------------------------------------------------------------------------
int msgbox_Y_N_C(UnicodeString msg, UnicodeString tit)
{
	if (tit.IsEmpty()) tit = "�m�F";	//�f�t�H���g
	return Application->MessageBox(msg.c_str(), tit.c_str(), MB_YESNOCANCEL|MB_ICONQUESTION);
}

//---------------------------------------------------------------------------
//���ݎ������擾
// ��DebugTimeStr ���ݒ肳��Ă����炻�̎���
//---------------------------------------------------------------------------
TDateTime get_Now()
{
	if (DebugTimeStr.IsEmpty())
		return Now();
	else {
		try {
			return TDateTime(DebugTimeStr);
		}
		catch (EConvertError &e) {
			return Now();
		}
	}
}

//---------------------------------------------------------------------------
//�摜�̏k���E�g��
//  i_bmp : ���̓r�b�g�}�b�v
//  o_bmp : �o�̓r�b�g�}�b�v
//
//	ratio : �{��		0.0 : ����
//   s_wd : ���T�C�Y	  0 : ����
//   s_hi : �c�T�C�Y
//			�{�����T�C�Y�̂����ꂩ���w��
//   s_opt: �A���S���Y��
//---------------------------------------------------------------------------
bool WIC_resize_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	double ratio, unsigned int s_wd, unsigned int s_hi, int s_opt)
{
	if (ratio<=0.0 && (s_wd==0 || s_hi==0)) return false;

	bool res = true;
	try {
		::CoInitialize(NULL);
		try {
			TComInterface<IWICImagingFactory>	factory;
			TComInterface<IWICBitmap>			bitmap;
			TComInterface<IWICBitmapScaler> 	scaler;

			if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
				CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
					Abort();

			//�\�[�X����r�b�g�}�b�v���쐬
			i_bmp->PixelFormat = pf24bit;
			if (FAILED(factory->CreateBitmapFromHBITMAP(
				i_bmp->Handle, NULL, WICBitmapIgnoreAlpha, &bitmap)))
					Abort();

			if (FAILED(factory->CreateBitmapScaler(&scaler))) Abort();

			//���T�C�Y
			UINT i_wd, i_hi;
			if (ratio>0) {
				i_wd = i_bmp->Width  * ratio;
				i_hi = i_bmp->Height * ratio;
			}
			else {
				i_wd = s_wd;
				i_hi = s_hi;
			}
			WICBitmapInterpolationMode opt;
			switch (s_opt) {
			case 1:  opt = WICBitmapInterpolationModeLinear; break;		//�o�C���j�A
			case 2:  opt = WICBitmapInterpolationModeCubic;	 break;		//�o�C�L���[�r�b�N
			case 3:  opt = WICBitmapInterpolationModeFant;	 break;		//�t�@���g���T���v�����O
			default: opt = WICBitmapInterpolationModeNearestNeighbor;	//�ŋߖT�@
			}
			if (FAILED(scaler->Initialize(bitmap, i_wd, i_hi, opt)))
				Abort();

			//�C���[�W���������ɃR�s�[
			UINT stride = i_wd * 3;
			std::unique_ptr<BYTE []> ibuf(new BYTE[stride * i_hi]);
			if (FAILED(scaler->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
			//����������r�b�g�}�b�v�ɃR�s�[
			o_bmp->PixelFormat = pf24bit;
			o_bmp->SetSize(i_wd, i_hi);
			BYTE *bp = ibuf.get();
			for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(o_bmp->ScanLine[i], bp, stride);
		}
		catch (...) {
			res = false;
		}
	}
	__finally {
		::CoUninitialize();
	}
	return res;
}
//---------------------------------------------------------------------------
