//----------------------------------------------------------------------//
// �j���e�[�u��															//
//																		//
//----------------------------------------------------------------------//
#ifndef HolidayH
#define HolidayH

//---------------------------------------------------------------------------
#include <tchar.h>

//---------------------------------------------------------------------------
#define wixSUN	1
#define wixMON	2
#define wixTUE	3
#define wixWED	4
#define wixTHU	5
#define wixFRI	6
#define wixSAT	7

extern const _TCHAR *WeekStrJ[7];
extern const _TCHAR *WeekStrE[7];
extern const _TCHAR *MonthStrE[12];

int  get_dayn(int y, int m);
int  get_wek(int y, int m, int d);
int  get_wday(int y, int m, int w, int n);

//---------------------------------------------------------------------------
// �j���e�[�u�� �N���X
//---------------------------------------------------------------------------
struct holiday_rec {
	unsigned short m;	//��
	unsigned short d;	//��
	_TCHAR name[31];	//����
};

class HolidayTable {
private:
	void Add(unsigned short m, unsigned short d, const _TCHAR *name);

public:
	int  Year;		//�Ώ۔N
	int  Count;		//����
	holiday_rec Table[30];

	HolidayTable();
	void InitTable(unsigned short y);
	bool IsHoliday(unsigned short y, unsigned short m, unsigned short d, _TCHAR *name);
};
//---------------------------------------------------------------------------
#endif
