/** \mainpage SpaceXpanse API Reference Manual
  \image html banner2016.png
  <center>SpaceXpanse Software and documentation Copyright (C) 2016 Martin Schweiger</center>

  The SpaceXpanse SDK Package contains this document in compiled HTML format (CHM) and in
  hyperlinked PDF format.

  \section intro Introduction
  This reference document contains the specification for the SpaceXpanse
  Application Programming Interface (OAPI). 
  The intended audience are developers who want to write DLL plugin modules
  for SpaceXpanse.
  The document is \e not required for using SpaceXpanse.

  The programming interface allows the development of third party modules to
  enhance the functionality of the SpaceXpanse core. Examples for modules are:
  - new spacecraft (including custom flight models, instrument panels, etc.)
  - new celestial bodies (including trajectory and atmospheric code)
  - additional instruments, such as MFD (multifunctional display) modes
  - global modules (networking, sound, etc.)

  \section Contents
  The following sections of this document are important for developers of
  SpaceXpanse addon modules:
  - Section \ref oapi contains a set of functions for getting and setting
    general simulation parameters in a running SpaceXpanse simulation session.
	They are used by all types of plugin modules.
  - The \ref VESSEL, \ref VESSEL2 and \ref VESSEL3 classes are the base classes
    for vessel definitions. They are particularly useful for developers who
	want to create their own spacecraft plugins.
  - The \ref MFD and \ref MFD2 classes provide an interface for creating new
    multifunctional display modes.
  - The \ref oapi::GraphicsClient class provides an interface between SpaceXpanse
    and external render engine modules. It is interesting for developers
	who want to subsititute SpaceXpanse's default graphics module with their own
	render engine plugin.
*/