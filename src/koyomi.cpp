//----------------------------------------------------------------------//
// KOYOMI.DLL�p���j�b�g													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include "UserFunc.h"
#include "koyomi.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
KoyomiUnit::KoyomiUnit()
{
	Available = false;
	ErrMsg	  = EmptyStr;

	try {
		hKOYOMI = ::LoadLibrary(_T("koyomi.dll"));
		if (hKOYOMI==NULL) throw EAbort("DLL���ǂݍ��߂܂���");
		Koyomi_Setdata = (KOYOMI_SetdataDll)::GetProcAddress(hKOYOMI, "SetdataDll");
		Koyomi_Rokuyo  = (KOYOMI_RokuyoDll)::GetProcAddress(hKOYOMI, "RokuyoDll");
		Koyomi_Eto	   = (KOYOMI_EtoDll)::GetProcAddress(hKOYOMI, "EtoDll");
		Koyomi_Sekki   = (KOYOMI_SekkiDll)::GetProcAddress(hKOYOMI, "SekkiDll");
		Koyomi_Kisetu  = (KOYOMI_KisetuDll)::GetProcAddress(hKOYOMI, "KisetuDll");

		if (Koyomi_Setdata==NULL || Koyomi_Rokuyo==NULL ||
			Koyomi_Eto==NULL || Koyomi_Sekki==NULL || Koyomi_Kisetu==NULL)
				throw EAbort("koyomi.dll�̏������Ɏ��s���܂���");

		//�ϑ��n�𓌋��ɉ��ݒ�
		Latitude  =  35.68;	//�ܓx
		Longitude = 139.75;	//�o�x

		Available = true;
	} catch(EAbort &e) {
		ErrMsg = e.Message;
	}
}
//---------------------------------------------------------------------------
KoyomiUnit::~KoyomiUnit()
{
	if (hKOYOMI) ::FreeLibrary(hKOYOMI);
}

//---------------------------------------------------------------------------
//���񕶎���̎擾
//---------------------------------------------------------------------------
UnicodeString KoyomiUnit::GetInfoStr(int y, int m, int d)
{
	if (!Available) return EmptyStr;
	if (y<1975 || y>2050) return EmptyStr;

	UnicodeString rstr;
	int k_m, k_d;
	int dummy_i, dummy_i2;
	double dummy_f, dummy_f2;

	Koyomi_Setdata(Latitude, Longitude, 0.0, y, m, d, &dummy_i);

	//����E�Z�j
	UnicodeString rokuyo_str = Koyomi_Rokuyo(&k_m, &k_d, &dummy_i);
	if (k_m<0)
		rstr.cat_sprintf(_T("�[%u��"), -k_m);
	else
		rstr.cat_sprintf(_T("%2u��"), k_m);
	rstr.cat_sprintf(_T("%2d��"), k_d);

	//���x
	UnicodeString eto_str = Koyomi_Eto(3, &dummy_i, &dummy_i2);
	if (eto_str.SubString(1, 2)=="��") {
		eto_str.Delete(1, 2); eto_str.Insert("��",1);
	}
	if (eto_str.SubString(1, 2)=="��") {
		eto_str.Delete(1, 2); eto_str.Insert("�",1);
	}

	rstr += " " + eto_str + " " + rokuyo_str;

	//�ߋG�E�G��
	UnicodeString sekki_str = Koyomi_Sekki(&dummy_i, &dummy_f, &dummy_f2);
	if (sekki_str=="�|�|")
		sekki_str = EmptyStr;
	else {
		if (sekki_str=="�y�p") {
			switch (m) {
			case  1: sekki_str = "�~�y�p"; break;
			case  4: sekki_str = "�t�y�p"; break;
			case  7: sekki_str = "�ēy�p"; break;
			case 10: sekki_str = "�H�y�p"; break;
			}
		};
	}

	//���̑��̎G��
	TDateTime dt = TDateTime(y, m, d);	//�Ώۓ��t

	//���t�N�Z
	TDateTime risyun_t	= TDateTime(y, 2, Koyomi_Kisetu(1, &dummy_f));
	if		(dt == risyun_t -   1) sekki_str = "�ߕ�";
	else if (dt == risyun_t +  87) sekki_str = "���\����";
	else if (dt == risyun_t + 209) sekki_str = "��S�\��";

	if (m==3) {
		//�t��
		int shunbun_d = Koyomi_Kisetu(2, &dummy_f);
		TDateTime shunbun_t = TDateTime(y, 3, shunbun_d);
		//�t�ފ�
		if		(dt == shunbun_t - 3) sekki_str = "�t�ފݓ�";
		else if (dt == shunbun_t + 3) sekki_str = "�t�ފݖ�";
		//�t�Г� (�O�㓯�����̏ꍇ�͒��O�̜��̓�)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 3, shunbun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 3, shunbun_d  + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("�t�Г�");
		}
	}
	if (m==9) {
		//�H��
		int shubun_d = Koyomi_Kisetu(3, &dummy_f);
		TDateTime shubun_t = TDateTime(y, 9, shubun_d);
		//�H�ފ�
		if		(dt == shubun_t - 3) sekki_str = "�H�ފݓ�";
		else if (dt == shubun_t + 3) sekki_str = "�H�ފݖ�";
		//�H�Г� (�O�㓯�����̏ꍇ�͒��O�̜��̓�)
		Koyomi_Setdata(Latitude, Longitude, 0.0, y, 9, shubun_d, &dummy_i);
		int eto_i;
		Koyomi_Eto(3, &eto_i, &dummy_i2);
		if (dt == TDateTime(y, 9, shubun_d + (4 - eto_i))) {
			if (!sekki_str.IsEmpty()) sekki_str.UCAT_TSTR(" ");
			sekki_str.UCAT_TSTR("�H�Г�");
		}
	}

	if (!sekki_str.IsEmpty()) rstr += " " + sekki_str;

	return rstr;
}
//---------------------------------------------------------------------------
