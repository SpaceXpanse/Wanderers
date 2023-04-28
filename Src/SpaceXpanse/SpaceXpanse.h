// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef SPACEXPANSE_H
#define SPACEXPANSE_H

#include "Config.h"
#include "Input.h"
#include "Select.h"
#include "Keymap.h"
#include <stdio.h>
#include <commctrl.h>
#include "Mesh.h"
#include "Astro.h"


class DInput;
class Config;
class State;
class Body;
class Vessel;
class ScreenNote;
class DialogManager;
class SpaceXpanseGraphics;
class SpaceXpanseConnect;
class SpaceXpanseServer;
class SpaceXpanseClient;
class PlaybackEditor;
class MemStat;
class DDEServer;
class ImageIO;
namespace spacexpanse {
	class ConsoleNG;
	class LaunchpadDialog;
}

//-----------------------------------------------------------------------------
// Structure for module callback functions
//-----------------------------------------------------------------------------
typedef void (*OPC_Proc)(void);

//-----------------------------------------------------------------------------
// Name: class TimeData
// Desc: stores timing information for current time step
//-----------------------------------------------------------------------------
class TimeData {
public:
	TimeData ();

	void Reset (double mjd_ref = 0.0);
	// Reset all sim and sys times to 0. Set time warp to 1
	// Disable fixed step mode

	void TimeData::SetFixedStep(double step);
	// set a fixed time interval for each time step [s]
	// step=0 disables the fixed step modus

	double TimeData::FixedStep() const { return (bFixedStep ? fixed_step : 0.0); }

	void BeginStep (double deltat, bool running);
	// advance time by deltat (seconds)

	void EndStep (bool running);
	// copy time data from next step to current step

	double JumpTo (double mjd);
	// jump to a new simulation date. Returns jump distance from current time [s]

	void SetWarp (double warp, double delay = 0.0);
	inline double Warp () const { return TWarp; }
	inline bool WarpChanged () const { return bWarpChanged; }
	inline size_t FrameCount() const { return framecount; }

	double MJD (double simt) const { return MJD_ref + Day(simt); }
	// Convert simulation time to MJD

	inline double FPS() const { return fps; }

	double  SysT0;        // current system time since simulation start [s]
	double  SysT1;        // next frame system time (=SysT0+SysDT)
	double  SysDT;        // current system step interval [s]
	double iSysDT;        // 1/SysDT
	double  SimT0;        // current simulation time since simulation start [s]
	double  SimT1;        // next frame simulation time (=SimT0+SimDT)
	double  SimDT;        // current simulation step interval [s] (updated at the beginning of the update phase)
	double  SimDT0;       // time step between currently published state (s0) and the previous state (updated at the end of the update phase)
	double iSimDT;        // 1/SimDT
	double iSimDT0;       // 1/SimDT0
	double  MJD0;         // Modified Julian date at current frame [days]
	double  MJD1;         // Modified Julian date at next frame [days]
	double  MJD_ref;      // Modified Julian date at simulation start [days]

private:
	double  SimT1_ofs, SimT1_inc;  // offset and increment parts of SimT1
	double  fixed_step;   // fixed base time step length (0=variable)
	double  TWarp;        // time acceleration factor
	double  TWarpTarget;  // target acceleration factor
	double  TWarpDelay;   // warp acceleration delay
	bool    bWarpChanged; // time acceleration changed in last step?
	bool    bFixedStep;   // use fixed time steps?
	size_t  framecount;   // number of frames since simulation start (including pause)
	size_t  frame_tick;   // number of frames since last fps calculation
	size_t  sys_tick;     // flush index for fps calculation
	double  syst_acc;     // accumulated system time for fps calculation
	double  fps;          // current frame rate [Hz]
};

//-----------------------------------------------------------------------------
// Name: class SpaceXpanse
// Desc: Main application class
//-----------------------------------------------------------------------------
class SpaceXpanse {
	friend class ScriptInterface;
	friend class oapi::GraphicsClient;
	friend class SpaceXpanseGraphics;

public:
	SpaceXpanse ();
	~SpaceXpanse ();

    HRESULT Create (HINSTANCE);
	VOID Launch (const char *scenario);
	void CloseApp (bool fast_shutdown = false);
	int GetVersion () const;
	HWND CreateRenderWindow (Config *pCfg, const char *scenario);
	void PreCloseSession();
	void CloseSession ();
	void GetRenderParameters ();
	bool InitializeWorld (char *name);
	void ScreenToClient (POINT *pt) const;
    LRESULT MsgProc (HWND, UINT, WPARAM, LPARAM);
	HRESULT Render3DEnvironment();
	VOID Output2DData ();
	void OutputLoadStatus (const char *msg, int line);
	void OutputLoadTick (int line, bool ok = true);
	void TerminateOnError();
	void UpdateServerWnd (HWND hWnd);
	void InitRotationMode ();
	void ExitRotationMode ();
	bool StickyFocus() const { return bKeepFocus; }
	void OpenVideoTab() { bStartVideoTab = true; }
	INT Run ();
	void SingleFrame ();
    void Pause (bool bPause);
	void Freeze (bool bFreeze);
	inline void TogglePause () { Pause (bRunning); }
	bool Timejump (double _mjd, int pmode);
	void Suspend (void); // elapsed time between Suspend() and Resume() is ignored
	void Resume (void); // A Suspend/Resume pair must be closed within a time step
	bool SaveScenario (const char *fname, const char *desc, int desc_type);
	void SaveConfig ();
	VOID Quicksave ();
	void StartCaptureFrames () { video_skip_count = 0; bCapture = true; }
	void StopCaptureFrames () { bCapture = false; }
	bool IsCapturingFrames() const { return bCapture; }
	void CaptureVideoFrame ();
	const char *KeyState() const;

	// dialog box processing
	HWND OpenDialog (int id, DLGPROC pDlg, void *context = 0); // This version expects the dialog resource in the SpaceXpanse instance
	HWND OpenDialog (HINSTANCE hInst, int id, DLGPROC pDlg, void *context = 0); // use this version for for calls from external dlls
	HWND OpenDialogEx (int id, DLGPROC pDlg, DWORD flag = 0, void *context = 0); // extended version
	HWND OpenDialogEx (HINSTANCE hInst, int id, DLGPROC pDlg, DWORD flag = 0, void *context = 0); // extended version
	HWND OpenHelp (HELPCONTEXT *hcontext);
	void OpenLaunchpadHelp (HELPCONTEXT *hcontext);
	//void OpenDialogAsync (int id, DLGPROC pDlg, void *context = 0);
	void CloseDialog (HWND hDlg);
	HWND IsDialog (HINSTANCE hInst, DWORD resId);
	bool RegisterWindow (HINSTANCE hInstance, HWND hWnd, DWORD flag);

	void UpdateDeallocationProgress();

	// plugin module loading/unloading
	HINSTANCE LoadModule (const char *path, const char *name);   // load a plugin

	/// \brief Unload a DLL plugin identified by its name
	/// \param name DLL name
	/// \return true on success (module found and unloaded)
	bool UnloadModule (const std::string &name);

	/// \brief Unload a DLL plugin identified by its instance handle
	/// \param hDLL DLL handle
	/// \return true on success (module found and unloaded)
	bool UnloadModule (HINSTANCE hDLL);

	Vessel *SetFocusObject (Vessel *vessel, bool setview = true);
	// Select a new user-controlled vessel
	// Return value is old focus object, or 0 if focus hasn't changed

	void SetView (Body *body, int mode);
	// Change camera tracking or cockpit target
	// mode: 0=internal, 1=external, 2=don't change

	void InsertVessel (Vessel *vessel);
	// Insert a newly created vessel into the simulation

	bool KillVessels();
	// Kill the vessels that have been marked for deletion in the last time step

	inline double ManCtrlLevel (THGROUP_TYPE thgt, DWORD device) const {
		switch (device) {
		case MANCTRL_KEYBOARD: return 0.001*ctrlKeyboard[thgt];
		case MANCTRL_JOYSTICK: return 0.001*ctrlJoystick[thgt];
		default:               return 0.001*ctrlTotal[thgt];
		}
	}

	void NotifyObjectJump (const Body *obj, const Vector &shift);
	void NotifyObjectSize (const Body *obj);

	void SetWarpFactor (double warp, bool force = false, double delay = 0.0);
	// Set time acceleration factor

	VOID SetFOV (double fov, bool limit_range = true);
	// Set camera field of view to fov (vertical half-screen) [rad]

	VOID IncFOV (double dfov);
	// Increase camera field of view by dfov

	// Accessor functions
	inline HINSTANCE GetInstance() const { return hInst; }
	inline HWND    GetRenderWnd() const { return hRenderWnd; }
	inline bool    IsFullscreen() const { return bFullscreen; }
	inline DWORD   ViewW() const { return viewW; }
	inline DWORD   ViewH() const { return viewH; }
	inline DWORD   ViewBPP() const { return viewBPP; }
	inline Config* Cfg() const { return pConfig; }
	inline ScriptInterface *Script() const { return script; }
	inline DialogManager *DlgMgr() const { return pDlgMgr; }
	inline spacexpanse::LaunchpadDialog *Launchpad() const { return m_pLaunchpad; }
	inline State*  PState() const { return pState; }
	inline bool    IsActive() const { return bActive; } // temporary
	inline bool    IsRunning() const { return bRunning; }
	inline bool    UseStencil() const { return bUseStencil; }
	inline void    SetFastExit (bool fexit) { bFastExit = fexit; }
	inline bool    UseHtmlInline() { return (pConfig->CfgDebugPrm.bHtmlScnDesc == 1 || pConfig->CfgDebugPrm.bHtmlScnDesc == 2 && !bWINEenv); }

	// DirectInput components
	inline CDIFramework7 *GetDInput() const { return pDI->GetDIFrame(); }
	inline LPDIRECTINPUTDEVICE8 GetKbdDevice() const { return pDI->GetKbdDevice(); }
	inline LPDIRECTINPUTDEVICE8 GetJoyDevice() const { return pDI->GetJoyDevice(); }

	// memory monitor
	MemStat *memstat;
	long simheapsize; // memory allocated during CreateRenderWindow

	// Onscreen annotation
	inline oapi::ScreenAnnotation *SNotePB() const { return snote_playback; }
	oapi::ScreenAnnotation *CreateAnnotation (bool exclusive, double size, COLORREF col);
	bool DeleteAnnotation (oapi::ScreenAnnotation *sn);

	// File locations - THESE FUNCTIONS ARE NOT THREADSAFE!
	inline char *ConfigPath (const char *name) { return pConfig->ConfigPath (name); }
	inline char *MeshPath   (const char *name) { return pConfig->MeshPath (name); }
	inline char *TexPath    (const char *name, const char *ext = 0)
		{ return pConfig->TexPath (name, ext); }
	inline char *HTexPath   (const char *name, const char *ext = 0)
		{ return pConfig->HTexPath (name, ext); }
	inline const char *ScnPath    (const char *name) { return pConfig->ScnPath (name); }

	FILE *OpenTextureFile (const char *name, const char *ext);
	// return texture file handle. Searches in hightex and standard directories

	SURFHANDLE RegisterExhaustTexture (char *name);

	Keymap keymap;
	// keyboard mapper

	bool ActivateRoughType();   // suppress font smoothing
	bool DeactivateRoughType(); // re-enable font smoothing

	// Flight recorder
	char *FRsysname;             // system event playback name
	std::ifstream *FRsys_stream; // system event playback file
	double frec_sys_simt;        // system event timer
	PlaybackEditor *FReditor;    // playback editor instance
	void ToggleRecorder (bool force = false, bool append = false);
	void EndPlayback ();
	inline int RecorderStatus() const { return (bRecord ? 1 : bPlayback ? 2 : 0); }
	inline bool IsPlayback() const { return bPlayback; }
	const char *GetDefRecordName (void) const;
	void FRecorder_Reset ();
	// reset flight recorder status
	bool FRecorder_PrepareDir (const char *fname, bool force);
	// clear the flight recording directory
	void FRecorder_Activate (bool active, const char *fname, bool append = false);
	// activate the flight recorder
	void FRecorder_SaveEvent (const char *event_type, const char *event);
	// save a system event
	void FRecorder_OpenPlayback (const char *scname);
	// open system playback file
	void FRecorder_ClosePlayback ();
	// close system playback file
	void FRecorder_SuspendPlayback ();
	// closes the system event stream (for on-the-fly editing)
	void FRecorder_RescanPlayback ();
	// re-read the system event stream up to current playback time
	// (for on-the-fly editing)
	void FRecorder_Play ();
	// scan system playback file to current sim time
	void FRecorder_ToggleEditor ();
	// toggle the playback editor

	typedef struct {
		char *label;
		char *desc;
		int id;
		CustomFunc func;
		void *context;
	} CUSTOMCMD;

	struct {
		double dt;
		int mode;
	} tjump;

	DWORD RegisterCustomCmd (char *label, char *desc, CustomFunc func, void *context);
	bool UnregisterCustomCmd (int cmdId);

	MeshManager     meshmanager;    // global mesh manager

	// Load a mesh from file, and store it persistently in the mesh manager
	const Mesh *LoadMeshGlobal (const char *fname);
	const Mesh *LoadMeshGlobal (const char *fname, LoadMeshClbkFunc fClbk);

	// graphics client shortcuts
	inline SURFHANDLE LoadTexture (const char *fname, DWORD flags = 0)
	{ return (gclient ? gclient->clbkLoadTexture (fname, flags) : NULL); }

	//inline SURFHANDLE CreateSurface (DWORD w, DWORD h, DWORD attrib)
	//{ return (gclient ? gclient->clbkCreateSurfaceEx (w, h, attrib) : NULL); }

	//inline SURFHANDLE CreateTexture (DWORD w, DWORD h)
	//{ return (gclient ? gclient->clbkCreateTexture (w, h) : NULL); }

	inline bool ReleaseSurface (SURFHANDLE surf)
	{ return (gclient ? gclient->clbkReleaseSurface (surf) : false); }

	inline bool SetSurfaceColourKey (SURFHANDLE surf, DWORD ckey)
	{ return (gclient ? gclient->clbkSetSurfaceColourKey (surf, ckey) : false); }

	inline DWORD GetDeviceColour (BYTE r, BYTE g, BYTE b)
	{ return (gclient ? gclient->clbkGetDeviceColour (r, g, b) : 0); }

	inline bool Blt (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, SURFHANDLE src, DWORD flag = 0)
	{ return (gclient ? gclient->clbkBlt (tgt, tgtx, tgty, src, flag) : false); }

	inline bool Blt (SURFHANDLE tgt, DWORD tgtx, DWORD tgty, SURFHANDLE src, DWORD srcx, DWORD srcy, DWORD w, DWORD h, DWORD flag = 0)
	{ return (gclient ? gclient->clbkBlt (tgt, tgtx, tgty, src, srcx, srcy, w, h, flag) : false); }

	inline bool FillSurface (SURFHANDLE surf, DWORD col)
	{ return (gclient ? gclient->clbkFillSurface (surf, col) : false); }

	inline bool FillSurface (SURFHANDLE surf, DWORD tgtx, DWORD tgty, DWORD w, DWORD h, DWORD col)
	{ return (gclient ? gclient->clbkFillSurface (surf, tgtx, tgty, w, h, col) : false); }

	inline HDC GetSurfaceDC (SURFHANDLE surf)
	{ return (gclient ? gclient->clbkGetSurfaceDC (surf) : NULL); }

	inline void ReleaseSurfaceDC (SURFHANDLE surf, HDC hDC)
	{ if (gclient) gclient->clbkReleaseSurfaceDC (surf, hDC); }

	bool SendKbdBuffered(DWORD key, DWORD *mod = 0, DWORD nmod = 0, bool onRunningOnly = false);
	// Simulate a buffered keypress with an optional list of modifier keys

	bool SendKbdImmediate(char kstate[256], bool onRunningOnly = false);
	// Simulate an immediate key state

protected:
	HRESULT UserInput ();
	void KbdInputImmediate_System    (char *kstate);
	void KbdInputImmediate_OnRunning (char *buffer);
	void KbdInputBuffered_System     (char *kstate, DIDEVICEOBJECTDATA *dod, DWORD n);
	void KbdInputBuffered_OnRunning  (char *kstate, DIDEVICEOBJECTDATA *dod, DWORD n);
	void UserJoyInput_System (DIJOYSTATE2 *js);
	void UserJoyInput_OnRunning (DIJOYSTATE2 *js);
	bool MouseEvent (UINT event, DWORD state, DWORD x, DWORD y);
	bool BroadcastMouseEvent (UINT event, DWORD state, DWORD x, DWORD y);
	bool BroadcastImmediateKeyboardEvent (char *kstate);
	void BroadcastBufferedKeyboardEvent (char *kstate, DIDEVICEOBJECTDATA *dod, DWORD n);

	void BroadcastGlobalInit();

	bool BeginTimeStep (bool running);
	// Initialise the next frame time from the current system time. Returns true if
	// time was advanced or if running==false (paused). Returns false if not enough time
	// has passed since the current frame time (i.e. skip this update)

	void EndTimeStep (bool running);
	// Finish step update by copying next frame time data to current frame time data

	bool SessionLimitReached() const;
	// Return true if a session duration limit has been reached (frame limit/time limit, if any)

	void ModulePreStep ();
	void ModulePostStep ();
	VOID UpdateWorld ();

	void IncWarpFactor ();
	void DecWarpFactor ();
	// Increment/decrement time acceleration factor to next power of 10

	void ApplyWarpFactor ();
	// broadcast new warp factor to components and modules

    HRESULT InitDeviceObjects ();
	HRESULT RestoreDeviceObjects ();
    HRESULT DeleteDeviceObjects ();
	void InitializeGDIResources (HWND hWnd);
	void ReleaseGDIResources ();

private:
	Config         *pConfig;
	State          *pState;
	spacexpanse::LaunchpadDialog *m_pLaunchpad;
	DialogManager  *pDlgMgr;
	spacexpanse::ConsoleNG* m_pConsole;    // The console window opened when SpaceXpanse server is launched without a graphics client
	DInput         *pDI;
	HINSTANCE       hInst;         // spacexpanse instance handle
	HWND            hRenderWnd;    // render window handle (NULL if no render support)
	HWND            hBk;           // background window handle (demo mode only)
	BOOL            bRenderOnce;   // flag for single frame render request
	BOOL            bEnableLighting;
	bool			bUseStencil;   // render device provides stencil buffer (and user requests it)
	bool            bKeepFocus;    // disable focus switch on mouse move (during rotations)
	bool            bStartVideoTab; // Open Launchpad dialog on video tab
	bool            bWINEenv;      // we are running under Linux/WINE
	bool            ignorefirst;   // flag for first joystick action
	long            plZ4;          // previous joystick throttle setting
	int             video_skip_count; // count skipped frames for frame sequence capturing
	oapi::ScreenAnnotation **snote;// onscreen annotations
	DWORD           nsnote;        // number of annotations
	oapi::ScreenAnnotation *snote_playback;// onscreen annotation during playback
	ScriptInterface *script;
	INTERPRETERHANDLE hScnInterp;

	// render parameters (only used if graphics client is present)
	bool			bFullscreen;   // renderer in fullscreen mode
	DWORD           viewW, viewH;  // render viewport dimensions
	DWORD			viewBPP;       // render colour depth (bits per pixel)

	char            cfgpath[256];
	int             cfglen;
	char            simkstate[256];// accumulated simulated key state

	DWORD           ms_prev;       // used for time step calculation
	DWORD           ms_suspend;    // used for time-skipping within a step
	bool            bActive;       // render window has focus
	bool            bAllowInput;   // allow input processing for the next frame even if render window doesn't have focus
	bool            bVisible;      // render window exists and is visible
	bool            bRunning;      // simulation is running
	bool            bRequestRunning; // pause/resume request
	bool            bSession;      // simulation session in progress
	BOOL			bRealtime;     // TRUE if TWarp == 1
	BOOL            bEnableAtt;    // TRUE if manual attitude control (keyboard or joystick) is enabled
	bool            bRecord;       // true if flight is being recorded
	bool            bPlayback;     // true if flight is being played back
	bool            bCapture;      // capturing frame sequence is active
	bool            bFastExit;     // terminate on simulation end?
	bool            bSysClearType; // is cleartype enabled on the user's system?
	bool            bRoughType;    // font-smoothing disabled?

	// Manual joystick/keyboard attitude inputs
	DWORD ctrlJoystick[15];
	DWORD ctrlKeyboard[15];
	DWORD ctrlTotal[15];

	VOID SavePlaybackScn (const char *fname);

	// === The plugin module interface ===
	struct DLLModule {
		HINSTANCE hDLL;        // DLL instance handle
		oapi::Module* pModule; // pointer to module instance, if the plugin registered one
		std::string sName;     // DLL name
		bool bLocalAlloc;      // locally allocated; should be freed by SpaceXpanse core
	};
	std::list<DLLModule> m_Plugin;

	oapi::Module *register_module;  // used during module registration
	friend OAPIFUNC void oapiRegisterModule (oapi::Module* module);

	void LoadFixedModules ();                   // load all startup plugins

	OPC_Proc FindModuleProc (HINSTANCE hDLL, const char *procname);
	// returns address of a procedure in a plugin module, or NULL if procedure not found

	// list of custom commands
	CUSTOMCMD *customcmd;
	DWORD ncustomcmd;
	friend class DlgFunction;

public:
#ifndef INLINEGRAPHICS
	// external graphics client
	bool AttachGraphicsClient (oapi::GraphicsClient *gc);
	bool RemoveGraphicsClient (oapi::GraphicsClient *gc);
#endif // !INLINEGRAPHICS
	inline oapi::GraphicsClient *GetGraphicsClient () { return gclient; }

private:
	oapi::GraphicsClient *gclient;       // external graphics client (renderer)
	SpaceXpanseGraphics *oclient;            // inline graphics client

public:
	inline SpaceXpanseGraphics *GetInlineGraphicsClient() { return oclient; }
	// (to access special inline graphics features. Eventually this should no longer
	// be necessary)

};

#endif // !SPACEXPANSE_H
