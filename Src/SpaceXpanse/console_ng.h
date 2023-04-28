// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __console_ng_h
#define __console_ng_h

#include <windows.h>

class SpaceXpanse;

namespace spacexpanse {

	class ConsoleNG {
	public:
		ConsoleNG(SpaceXpanse* pSpaceXpanse);
		~ConsoleNG();

		SpaceXpanse* GetSpaceXpanse() const { return m_pSpaceXpanse; }
		HWND WindowHandle() const { return m_hWnd; }
		bool ParseCmd();
		void Echo(const char* str) const;
		void EchoIntro() const;
		bool DestroyStatDlg();

	private:

		SpaceXpanse* m_pSpaceXpanse;
		HWND m_hWnd;       // console window handle
		HWND m_hStatWnd;   // stats dialog
		HANDLE m_hThread;  // console thread handle
	};

}

#endif // !__console_ng_h
