// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// =======================================================================
// DirectInput user interface class
// =======================================================================

#ifndef __INPUT_H
#define __INPUT_H

#include "Di7frame.h"

class DInput {
	friend class SpaceXpanse;

public:
	DInput (SpaceXpanse *pSpaceXpanse);
	~DInput ();

	HRESULT Create (HINSTANCE hInst);
	void Destroy ();

	bool CreateKbdDevice (HWND hRenderWnd);
	bool CreateJoyDevice (HWND hRenderWnd);
	void DestroyDevices ();

	inline CDIFramework7 *GetDIFrame() const { return diframe; }
	inline const LPDIRECTINPUTDEVICE8 GetKbdDevice() const { return diframe->GetKbdDevice(); }
	inline const LPDIRECTINPUTDEVICE8 GetJoyDevice() const { return diframe->GetJoyDevice(); }

	bool PollJoystick (DIJOYSTATE2 *js);

	struct JoyProp {
		bool bThrottle;  // joystick has throttle control
		bool bRudder;    // joystick has rudder control
		int ThrottleOfs; // throttle data offset
	};

protected:
	HRESULT SetJoystickProperties ();

private:
	SpaceXpanse *spacexpanse;
	CDIFramework7 *diframe;
	JoyProp joyprop;
};

#endif // !__INPUT_H