//----------------------------------------------------------------------//
// 月メモ																//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

USEFORM("Unit1.cpp", MoonMemoForm);
USEFORM("Unit2.cpp", TodayMsgForm);
USEFORM("ModalScr.cpp", ModalScrForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	//ミューテックスにより二重起動を防止
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("MoonMemo_is_running"));
	if (::GetLastError()==ERROR_ALREADY_EXISTS) {
		//二重起動なら、既存の MonMemo をアクティブに
		HWND hFrmWnd = ::FindWindow(_T("TMoonMemoForm"), NULL);
		if (hFrmWnd) {
			HWND hApp = ::GetWindow(hFrmWnd, GW_OWNER);
			if (::IsIconic(hApp)) ::ShowWindow(hApp, SW_RESTORE);
			::SetForegroundWindow(::GetLastActivePopup(hApp));
		}
		::CloseHandle(hMutex);
		return -1;
	}

	try {
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "MoonMemo";
		Application->CreateForm(__classid(TMoonMemoForm), &MoonMemoForm);
		Application->CreateForm(__classid(TTodayMsgForm), &TodayMsgForm);
		Application->CreateForm(__classid(TModalScrForm), &ModalScrForm);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	::CloseHandle(hMutex);
	return 0;
}
//---------------------------------------------------------------------------
