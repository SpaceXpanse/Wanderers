// Copyright (c) Martin Schweiger
// Licensed under the MIT License

#ifndef __SCRIPT_H
#define __SCRIPT_H

#include "SpaceXpanse.h"

class ScriptInterface {
public:
	ScriptInterface (SpaceXpanse *pSpaceXpanse);
	INTERPRETERHANDLE NewInterpreter();
	int DelInterpreter (INTERPRETERHANDLE);
	INTERPRETERHANDLE RunInterpreter (const char *cmd);
	bool ExecScriptCmd (INTERPRETERHANDLE hInterp, const char *cmd);
	bool AsyncScriptCmd (INTERPRETERHANDLE hInterp, const char *cmd);
	lua_State *GetLua (INTERPRETERHANDLE hInterp);

protected:
	HINSTANCE LoadInterpreterLib();
	
private:
	SpaceXpanse *spacexpanse;
	HINSTANCE hLib;
};

#endif // !__INTERPRETER_H