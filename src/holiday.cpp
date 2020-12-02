//----------------------------------------------------------------------//
//�j���e�[�u��															//
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
	 = {_T("��"), _T("��"), _T("��"), _T("��"), _T("��"), _T("��"), _T("�y")};
const _TCHAR *WeekStrE[7]
	 = {_T("Sunday"), _T("Monday"), _T("Tuesday"), _T("Wednesday"),
	    _T("Thursday"), _T("Friday"), _T("Saturday")};

const _TCHAR *MonthStrE[12]
	 = {_T("January"), _T("February"), _T("March"), _T("April"), _T("May"), _T("June"),
	    _T("July"), _T("August"), _T("September"), _T("October"), _T("November"), _T("December")};

//--------------------------------------
//�w��N���̓������擾
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
//�w��N�����̗j�����擾
// 1:��,2:�� �` 7:�y
//--------------------------------------
int get_wek(int y, int m, int d)
{
	if (m<3) {
		y--; m += 12;
	}
	return ((y + y/4 - y/100 + y/400 + (26*m + 16)/10 + d) % 7) + 1;
}

//--------------------------------------
//�w��N���̑�n w�j��(1�`7)�̓����擾
//--------------------------------------
int get_wday(int y, int m, int w, int n)
{
	int d = 1;
	while (get_wek(y, m, d)!=w) d++;	//��1 w�j�������߂�

	int i = 1;
	while (i<n) {
		d += 7; i++;
	}
	return d;
}


//---------------------------------------------------------------------------
// �j���e�[�u�� �N���X
//---------------------------------------------------------------------------
HolidayTable::HolidayTable()
{
	Year  = 0;
	Count = 0;
}

//---------------------------------------------------------------------------
//�w��N�̏j���e�[�u���쐬
//---------------------------------------------------------------------------
void HolidayTable::InitTable(unsigned short y)
{
	Year  = y;
	Count = 0;

	//����
	Add( 1, 1, _T("����"));

	//���l�̓�
	unsigned short d = (y<2000)? 15 : get_wday(y, 1, wixMON, 2);
	Add( 1, d, _T("���l�̓�"));

	//�����L�O�̓�
	Add( 2,11, _T("�����L�O�̓�"));

	//�V�c�a����(����)
	if (y>=2020) Add( 2,23, _T("�V�c�a����"));

	//�t���̓�
	d = (20843100 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 3, d, _T("�t���̓�"));

	//�݂ǂ�̓� -> ���a�̓�
	if (y>=1989) Add( 4,29, (y<2007)? _T("�݂ǂ�̓�") : _T("���a�̓�"));

	//���ʂ̓� (2019�̂�)
	if (y==2019) Add( 5, 1, _T("���ʂ̓�"));

	//���@�L�O��
	Add( 5, 3, _T("���@�L�O��"));

	//�݂ǂ�̓�
	if (y>=2007) Add( 5, 4, _T("�݂ǂ�̓�"));

	//���ǂ��̓�
	Add( 5, 5, _T("���ǂ��̓�"));

	//�C�̓�(2020/2021�̈ړ��ɑΉ�)
	if (y>=1996) {
		d = (y<2003)? 20 : (y==2020)? 23 : (y==2021)? 22 : get_wday(y, 7, wixMON, 3);
		Add( 7, d, _T("�C�̓�"));
	}

	//�X�|�[�c�̓�(2020/2021�̈ړ�)
	if (y==2020 || y==2021) Add( 7, (y==2020)? 24 : 23, _T("�X�|�[�c�̓�"));

	//�R�̓�(2020/2021�̈ړ��ɑΉ�)
	if (y>=2016) {
		d = (y==2020)? 10 : (y==2021)? 8 : 11;
		Add( 8, d, _T("�R�̓�"));
	}

	//�h�V�̓�
	d = (y<2003)? 15 : get_wday(y, 9, wixMON, 3);
	Add( 9, d, _T("�h�V�̓�"));

	//�H���̓�
	d = (23248800 + (y - 1980)*242194)/1000000 - (y - 1980)/4;
	Add( 9, d, _T("�H���̓�"));

	//�X�|�[�c�̓�(2020/2021������)
	if (y!=2020 && y!=2021) {
		d = (y<2000)? 10 : get_wday(y, 10, wixMON, 2);
		Add(10, d, (y>=2020)? _T("�X�|�[�c�̓�") : _T("�̈�̓�"));
	}

	//���ʗ琳�a�̋V (2019�̂�)
	if (y==2019) Add(10,22, _T("���ʗ琳�a�̋V"));

	//�����̓�
	Add(11, 3, _T("�����̓�"));

	//�ΘJ���ӂ̓�
	Add(11,23, _T("�ΘJ���ӂ̓�"));

	//�V�c�a����(����)
	if (y>=1989 && y<2019) Add(12,23, _T("�V�c�a����"));

	//�j�������j�̏ꍇ
	for (int i=0; i<Count; i++) {
		unsigned short m = Table[i].m;
		unsigned short d = Table[i].d;
		if (get_wek(y, m, d)==wixSUN) {
			for (;;) {
				TDate dt = TDate(y, m, d);
				dt = IncDay(dt, 1);
				DecodeDate(dt, y, m, d);

				if (y<2007) break;
					//(<2007 ���̓������j�̏ꍇ�����͐U�ւ��x��
				if (!IsHoliday(y, m, d, NULL)) break;
					//(>=2007 �ł��߂��u�����̏j���v�łȂ���
			}
			Add(m, d, _T("�U�ւ��x��"));
		}
	}

	//�j���ɋ��܂ꂽ�����͍����̋x��
	for (int i=0; i<Count; i++) {
		unsigned short yi = y;
		unsigned short mi = Table[i].m;
		unsigned short di = Table[i].d;
		//����
		TDate dt = TDate(yi, mi, di);
		dt = IncDay(dt, 1);
		DecodeDate(dt, yi, mi, di);
		//����?
		if (!IsHoliday(yi, mi, di, NULL) && get_wek(yi, mi, di)!=wixSUN) {
			unsigned short m = mi;
			unsigned short d = di;
			dt = IncDay(dt, 1);
			DecodeDate(dt, yi, mi, di);
			if (yi==y && IsHoliday(yi, mi, di, NULL)) Add(m, d, _T("�����̋x��"));
		}
	}
}

//---------------------------------------------------------------------------
//�e�[�u���ɏj����ǉ�
//---------------------------------------------------------------------------
void HolidayTable::Add(unsigned short m, unsigned short d, const _TCHAR *name)
{
	Table[Count].m = m;
	Table[Count].d = d;
	_tcscpy(Table[Count].name, name);
	Count++;
}

//---------------------------------------------------------------------------
//���N�ɂ�����w�茎���̏j������
// �߂�l
//  true:�j��/ false:�j���ł͂Ȃ�
//  name: NULL �łȂ���Ώj�������Ԃ�
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
