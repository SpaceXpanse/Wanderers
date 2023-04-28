**ORBITER Configuration**

Copyright (c) 2000-2022 Martin Schweiger

Orbiter home: [orbit.medphys.ucl.ac.uk](http://orbit.medphys.ucl.ac.uk/)

# <span id="anchor"></span>Introduction

This document contains information about the various file formats used
by Orbiter, including configuration, scenario and mesh files. Orbiter
functionality can be modified and extended by editing or adding new
configuration files, e.g. to define a new spacecraft type, or to modify
its visual appearance.

# <span id="anchor-1"></span><span id="anchor-2"></span>Orbiter configuration files

Configuration files allow the customisation of various aspects of the
simulator. Configuration files have file extension .cfg. They are ASCII
text files which can be edited with any text editor capable of writing
plain text files (e.g. notepad).

Each line contains an item and its value, using the format

\<*item*\> = \<*value*\>

A semicolon starts a comment, continuing to the end of the line.

All configuration files except the main configuration file (see below)
are located in a subdirectory tree defined by the ConfigDir entry in the
main file, usually “.\\Config”.

## <span id="anchor-3"></span><span id="anchor-4"></span>Main configuration file

The main configuration file Orbiter.cfg is located in the Orbiter main
directory. It contains general settings for graphics modes, subdirectory
locations, simulation parameters, etc. Most of the options in this file
are accessible via the Orbiter Launchpad dialog, and manual editing of
the file should rarely be necessary.

Orbiter overwrites the main configuration file at the start and end of
each simulation session, to store any changes made by the user in the
Launchpad dialog.

Normally, only entries whose values differ from their default setting
are written to Orbiter.cfg. To force Orbiter to write out all values
(useful for debugging or manually editing the file), open Orbiter.cfg in
a text editor, change the value of EchoAllParams to TRUE, and save.
Subsequently, Orbiter will write all configuration entries to the file.

|                             |         |                                                                                                                                                                                                                                            |
|-----------------------------|---------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Item                        | Type    | Description                                                                                                                                                                                                                                |
| EchoAllParams               | Bool    | If TRUE, Orbiter writes all configuration parameters to Orbiter.cfg, including defaults. Default: FALSE                                                                                                                                    |
| LPadRect                    | Rect    | Screen position of the launchpad dialog window (pixels)                                                                                                                                                                                    |
| Subdirectory locations      |         |                                                                                                                                                                                                                                            |
| ConfigDir                   | String  | Subdirectory for configuration files. Default: .\\Config\\                                                                                                                                                                                 |
| MeshDir                     | String  | Subdirectory for mesh files. Default: .\\Meshes\\                                                                                                                                                                                          |
| TextureDir                  | String  | Subdirectory for textures. Default: .\\Textures\\                                                                                                                                                                                          |
| HightexDir                  | String  | Subdirectory for alternative high-resolution planetary textures. Default: .\\Textures2\\                                                                                                                                                   |
| ScenarioDir                 | String  | Subdirectory for scenarios. Default: .\\Scenarios\\                                                                                                                                                                                        |
| Logical parameters          |         |                                                                                                                                                                                                                                            |
| StartPaused                 | Bool    | Suspend simulation on launch. Default: FALSE                                                                                                                                                                                               |
| FlightModel                 | Int     | Flight model realism level. Currently supported: 0 (simple) and 1 (complex). Default: 1                                                                                                                                                    |
| DamageModel                 | Int     | Damage realism level. Currently supported: 0 (no damage) and 1 (damage modelling enabled). Default: 0                                                                                                                                      |
| UnlimitedFuel               | Bool    | Ignore spacecraft fuel consumption. Default: FALSE                                                                                                                                                                                         |
| RefuelOnPad                 | Bool    | Auto-refuel spacecraft parked on a landing pad. Default: TRUE                                                                                                                                                                              |
| MFDTransparent              | Bool    | Make multifunctional displays transparent in “glass cockpit” mode. Default: FALSE                                                                                                                                                          |
| CompactGlasspit             | Bool    | On widescreen formats, keep MFD displays in the screen centre. Default: TRUE                                                                                                                                                               |
| GenericMFDSize              | Int     | Scaling factor for MFD displays in “glass cockpit” mode. Supported values: 1–10. Default: 6                                                                                                                                                |
| MFDMapVersion               | Int     | Display style for Map MFD mode (0 = old, 1 = new). Default: 1                                                                                                                                                                              |
| InstrumentUpdate­Interval   | Float   | Interval between MFD display updates \[s\]. Default: 1.0                                                                                                                                                                                   |
| PanelScale                  | Float   | Scaling factor for instrument panel display. Default: 1.0                                                                                                                                                                                  |
| PanelScrollSpeed            | Float   | Speed factor for panel scrolling \[pixels/s\]. Default: 300.0                                                                                                                                                                              |
| User interface parameters   |         |                                                                                                                                                                                                                                            |
| FocusFollowsMouse           | Bool    | If TRUE, dialog windows receive input focus when the mouse moves over them. Default: TRUE                                                                                                                                                  |
| MenubarMode                 | Int     | Main menu mode (0 = on, 1 = off, 2 = auto-hide). Default: 2                                                                                                                                                                                |
| MenuLabelOnly               | Bool    | Disable main menu icon display. Default: FALSE                                                                                                                                                                                             |
| ShowWarpAlways              | Bool    | Display time acceleration even if menubar is hidden. Default: TRUE                                                                                                                                                                         |
| ShowWarpScientific          | Bool    | Display time acceleration in scientific mode. Default: FALSE                                                                                                                                                                               |
| InfobarMode                 | Int     | Info bar mode (0 = on, 1 = off, 2 = auto-hide). Default: 0                                                                                                                                                                                 |
| InfoAuxIdx                  | Int Int | Auxiliary info box modes left/right. Default: 0 0                                                                                                                                                                                          |
| MenubarOpacity              | Int     | Main menu opacity (0–10). Default: 4                                                                                                                                                                                                       |
| InfobarOpacity              | Int     | Info bar opacity (0–10). Default: 4                                                                                                                                                                                                        |
| MenubarSpeed                | Int     | Menu/info bar scroll speed (1–20). Default: 10                                                                                                                                                                                             |
| Visual parameters           |         |                                                                                                                                                                                                                                            |
| EnableShadows               | Bool    | Enable object shadows on planet surfaces. Default: TRUE                                                                                                                                                                                    |
| EnableVesselShadows         | Bool    | Enable vessel shadows on planet surfaces. Default: TRUE                                                                                                                                                                                    |
| EnableClouds                | Bool    | Enable rendering of planetary cloud layers. Default: TRUE                                                                                                                                                                                  |
| EnableCloudShadows          | Bool    | Enable rendering of cloud shadows on the ground (also requires CloudShadowDepth \< 1 in individual planet config files). Default: FALSE                                                                                                    |
| EnableNightlights           | Bool    | Enable rendering of night lighting effects on planetary surfaces. Default: TRUE                                                                                                                                                            |
| EnableWaterReflection       | Bool    | Enable rendering of specular reflections from water surfaces. Default: TRUE                                                                                                                                                                |
| EnableSpecularRipples       | Bool    | Enable microtextures on water surfaces for ripple effects. Default: FALSE                                                                                                                                                                  |
| EnableHorizonHaze           | Bool    | Enable rendering of atmospheric effects at the horizon. Default: TRUE                                                                                                                                                                      |
| EnableDistanceFog           | Bool    | Enable distance-dependent fog effects. Default: TRUE                                                                                                                                                                                       |
| EnableSpecular­Reflection   | Bool    | Enable specular reflection effects from polished surfaces Default: TRUE                                                                                                                                                                    |
| EnableReentryFlames         | Bool    | Enable shockwave effects during reentry. Default: TRUE                                                                                                                                                                                     |
| EnableParticleStreams       | Bool    | Enable particle generation for exhaust and reentry effects. Default: TRUE                                                                                                                                                                  |
| EnableLocalLights           | Bool    | Enable localised point and spot light emitters. Default: FALSE                                                                                                                                                                             |
| MaxLights                   | Int     | Max. simultaneously active lights (0 = query device). Default: 0                                                                                                                                                                           |
| AmbientLevel                | Int     | Ambient light level (brightness of not directly lit surfaces). (0–255). Default: 10                                                                                                                                                        |
| PlanetMaxPatchLevel         | Int     | Max. texture resolution for planetary surfaces (1–14). Default: 14                                                                                                                                                                         |
| PlanetPatchRes              | Float   | Texture resolution bias for planet surfaces (0.1–10.0). Higher values result in higher render resolution at a given apparent radius, but reduce performance. Default: 1.0                                                                  |
| NightlightBrightness        | Float   | Brightness level of night lighting effects (0.0–1.0). Default: 0.5                                                                                                                                                                         |
| StarPrm                     | List    | Brightness and scaling parameters for stars rendered as pixels. Values: app. mag. limit for brightest stars / app. mag. limit for faintest stars / render brightness for faintest stars / lin-log scaling flag. Default: \[0.0 7.0 0.1 0\] |
| CSphereBgImage              | String  | File name for celestial sphere background image. Default: \<none>                                                                                                                                                                          |
| CSphereBgPath               | String  | File path to celestial sphere background image.                                                                                                                                                                                            |
| CSphereBgIntensity          | Float   | Brightness of background image (0.0–1.0). Default: 0.5                                                                                                                                                                                     |
| Screen capture parameters   |         |                                                                                                                                                                                                                                            |
| CaptureTarget               | Int     | 0 = clipboard, 1 = file. Default: 0                                                                                                                                                                                                        |
| CaptureFile                 | String  | File name for next screen capture                                                                                                                                                                                                          |
| CaptureSequenceDir          | String  | Directory name for next screen capture sequence                                                                                                                                                                                            |
| CaptureImageFormat          | Int     | 0 = BMP, 1 = PNG, 2 = JPG, 3 = TIFF                                                                                                                                                                                                        |
| CaptureImageQuality         | Int     | 1–10                                                                                                                                                                                                                                       |
| CaptureSequenceStart        | Int     | Number of next file in sequence                                                                                                                                                                                                            |
| CaptureSequenceSkip         | Int     | Number of frames to skip in sequence recording                                                                                                                                                                                             |
| Instrument parameters       |         |                                                                                                                                                                                                                                            |
| ForceMfdPow2                | String  | TRUE = MFD sizes are set to powers of 2; FALSE = sizes can be arbitrary; AUTO = Orbiter guesses from display caps.                                                                                                                         |
| MfdHiresThreshold           | Int     | MFD threshold size for switching from 256×256 to 512×512 pixels (only used if Pow2 size is active)                                                                                                                                         |
| Visual helper parameters    |         |                                                                                                                                                                                                                                            |
| Planetarium                 | Int     | Bit flags for display elements in “Planetarium” mode (F9). Default: 4330.                                                                                                                                                                  |
| BodyForces                  | List    | Display parameters for force vector visualisation. Values: bit flags for force types / vector scale factor / opacity. Default: \[60 1.0 1.0\]                                                                                              |
| CoordinateAxes              | List    | Display parameters for object axis visualisation. Values: bit flags for object types / axis scale factor / opacity. Default: \[4 1.0 1.0\]                                                                                                 |
| Debugging options           |         |                                                                                                                                                                                                                                            |
| ShutdownMode                | Int     | Simulation shutdown method (0 = dealloc memory, 1 = respawn, 2 = terminate). Default: 0                                                                                                                                                    |
| FixedStep                   | Float   | Fixed time interval per frame \[s\]. Default: 0 (disable fixed frame intervals)                                                                                                                                                            |
| TimerMode                   | Int     | Simulation timer mode (0 = auto, 1 = hires hardware timer, 2 = lores software timer). Default: 0                                                                                                                                           |
| DisableFontSmoothing        | Bool    | Turn off font smoothing while running Orbiter to improve performance. Default: TRUE                                                                                                                                                        |
| ForceReenableFont­Smoothing | Bool    | Re-enable font smoothing on closing Orbiter even if it was not active at program start. Default: FALSE                                                                                                                                     |
| HtmlScnDesc                 | Int     | Use inline Html viewer for Launchpad scenario descriptions. 0 = no, 1 = yes, 2 = auto (off for Linux/Wine)                                                                                                                                 |
| SaveExitScreen              | Bool    | Take screenshot on session exit to display in scenario description. Default: TRUE                                                                                                                                                          |
| Physics engine              |         |                                                                                                                                                                                                                                            |
| DistributedVesselMass       | Bool    | Enable gravity gradient torque effects as result of anisotropic inertia tensor. Default: FALSE                                                                                                                                             |
| NonsphericalGravity­Sources | Bool    | Enable orbit perturbations due to nonspherical gravitational potentials. Default: FALSE                                                                                                                                                    |
| RadiationPressure           | Bool    | Enable orbit perturbations due to radiation pressure. Default: FALSE                                                                                                                                                                       |
| StabiliseOrbits             | Bool    | Use Encke’s method for improved state propagation stability at large time steps. Default: TRUE                                                                                                                                             |
| StabilisePLimit             | Float   | Field perturbation limit for orbit stabilisation. Default: 0.05                                                                                                                                                                            |
| StabiliseSLimit             | Float   | Fractional orbit step limit for orbit stabilisation. Default: 0.01                                                                                                                                                                         |
| PertPropSubsampling         | List    | Orbit stabilisation subsampling parameters. Values: max. steps / fractional orbit step limit. Default: \[10 0.02\]                                                                                                                         |
| PertPropNonspherical­Limit  | Float   | Fractional orbit step beyond which nonspherical gravity effects are ignored. Default: 0.05                                                                                                                                                 |
| PropStages                  | Int     | Number of integrator stages for vessel propagation (1–5). Default: 4                                                                                                                                                                       |
| PropStage\<i>               | List    | Integrator parameters for propagator stage \<i> (0–4). Values: integrator index / time step limit. Default: i = 0: \[2 0.5\], i = 1: \[4 20.0\], i = 2: \[6 100.0\], i = 3: \[8 N/A\]                                                      |
| PropSubsampling             | Int     | Max. subsampling steps. Default: 100                                                                                                                                                                                                       |
| PlanetPreloadMode           | Int     | Planetary texture load mode. 0 = load on demand, 1 = preload at simulation start. Default: 0                                                                                                                                               |
| PlanetTexLoadFreq           | Float   | Texture patch loading frequency \[Hz\]. Default: 20                                                                                                                                                                                        |
| PlanetAnisoMode             | Int     | Planet anisotropic filter level (1 = none). Default: 1                                                                                                                                                                                     |
| PlanetMipmapMode            | Int     | Planet texture mipmap mode (0 = none, 1 = point sampling, 2 = linear interpolation). Default: 1                                                                                                                                            |
| PlanetMipmapBias            | Float   | Mipmap level bias (−1.0 – +1.0), where \< 0 is sharper, \> 0 is smoother. Default: 0                                                                                                                                                       |
| Map dialog parameters       |         |                                                                                                                                                                                                                                            |
| MapDlgFlag                  | Int     | Bitflags for map dialog parameter settings                                                                                                                                                                                                 |
| Camera parameters           |         |                                                                                                                                                                                                                                            |
| CameraPanspeed              | Float   | Camera speed in ground observer mode. Default: 100                                                                                                                                                                                         |
| HUDColIdx                   | Int     | HUD colour index. Default: 0 (green)                                                                                                                                                                                                       |
| Device settings             |         |                                                                                                                                                                                                                                            |
| DeviceIndex                 | Int     | Enumeration index for current 3D device (do not edit manually)                                                                                                                                                                             |
| ModeIndex                   | Int     | Screen mode index (do not edit manually)                                                                                                                                                                                                   |
| DeviceForceEnum             | Bool    | If TRUE, enumerate 3D devices at each start. Default: TRUE                                                                                                                                                                                 |
| Fullscreen                  | Bool    | TRUE for fullscreen mode, FALSE for windowed mode. Default: FALSE                                                                                                                                                                          |
| Stereo                      | Bool    | Currently not used                                                                                                                                                                                                                         |
| NoVSync                     | Bool    | Disable vertical refresh synchronisation. Default: FALSE                                                                                                                                                                                   |
| StencilBuffer               | Bool    | Use stencil buffering for semi-opaque shadows, if supported. Default: FALSE                                                                                                                                                                |
| FullscreenPageflip          | Bool    | Enable hardware page-flipping in fullscreen mode. Default: TRUE                                                                                                                                                                            |
| WindowWidth                 | Int     | Horizontal window size for windowed modes \[pixel\]                                                                                                                                                                                        |
| WindowHeight                | Int     | Vertical window size for windowed modes \[pixel\]                                                                                                                                                                                          |
| Joystick parameters         |         |                                                                                                                                                                                                                                            |
| JoystickIndex               | Int     | Enumeration index for current joystick (0 = none). Default: 0                                                                                                                                                                              |
| JoystickThrottleAxis        | Int     | Axis index for joystick throttle (0 = Z, 1 = slider 0, 2 = slider 1). Default: 1                                                                                                                                                           |
| JoystickThrottle­Saturation | Int     | Saturation zone for joystick throttle control (0–10000). A setting of 9000 means that the throttle will saturate over the last 10% of its range at either end. Default: 9500                                                               |
| JoystickDeadzone            | Int     | Deadzone at joystick axis centres (0–10000). A setting of 2000 means that the joystick is considered neutral within 20% from the central position. Default: 2500                                                                           |
| IgnoreThrottleOnStart       | Bool    | Ignore throttle at simulation start until moved. Default: TRUE                                                                                                                                                                             |
| Demo parameters             |         |                                                                                                                                                                                                                                            |
| DemoMode                    | Bool    | Start Orbiter in demo mode (auto-launch scenarios). Default: FALSE                                                                                                                                                                         |
| BackgroundImage             | Bool    | Cover screen background with an image in demo mode. Default: FALSE                                                                                                                                                                         |
| BlockExit                   | Bool    | Don’t allow users to exit Orbiter in demo mode. Default: FALSE                                                                                                                                                                             |
| MaxDemoTime                 | Float   | Max simulation runtime in demo mode \[s\]. Default: 300                                                                                                                                                                                    |
| MaxLaunchpadIdleTime        | Float   | Max. time for launchpad to be open before auto-launching a scenario \[s\]. Default: 15                                                                                                                                                     |
| Record/play parameters      |         |                                                                                                                                                                                                                                            |
| RecordPosFrame              | Int     | Flight recorder: reference frame for position data (0 = ecliptic, 1 = equatorial). Default: 1                                                                                                                                              |
| RecordAttFrame              | Int     | Flight recorder: reference frame for attitude data (0 = ecliptic, 1 = equatorial). Default: 1                                                                                                                                              |
| RecordTimeWarp              | Bool    | Save time acceleration events in recording stream. Default: TRUE                                                                                                                                                                           |
| RecordFocusEvent            | Bool    | Save vessel focus changes in recording stream. Default: TRUE                                                                                                                                                                               |
| ReplayTimeWarp              | Bool    | Set time acceleration during playback from stream data. Default: TRUE                                                                                                                                                                      |
| ReplayFocusEvent            | Bool    | Set vessel focus during playback from stream data. Default: TRUE                                                                                                                                                                           |
| ReplayCameraEvent           | Bool    | Set camera parameters during playback from stream data. Default: TRUE                                                                                                                                                                      |
| SystimeSampling             | Bool    | Use system time (rather than simulation time) for recording sample intervals. Default: TRUE                                                                                                                                                |
| PlaybackNotes               | Bool    | Display onscreen annotations from stream during playback. Default: TRUE                                                                                                                                                                    |
| Font parameters             |         |                                                                                                                                                                                                                                            |
| DialogFont_Scale            | Float   | Scaling factor for dialog font size. Default: 1.0                                                                                                                                                                                          |
| DialogFont1_Face            | String  | Standard dialog font face. Default: Arial                                                                                                                                                                                                  |
| Window positions            |         |                                                                                                                                                                                                                                            |
| DlgXXXPos                   | Rect    | Screen positions of dialog windows                                                                                                                                                                                                         |
| LpadXXXListWidth            | Int     | Width of Launchpad splitter lists                                                                                                                                                                                                          |
| Modules list                |         |                                                                                                                                                                                                                                            |
| ActiveModules               | List    | List of active plugin modules                                                                                                                                                                                                              |

## <span id="anchor-5"></span><span id="anchor-6"></span>Planetary systems

Planetary systems contain stars, planets and moons. Each planetary
system requires at least one star. Stars, planets and moons are defined
in the planetary system’s configuration file

.\\Config\\\<*Solsys-name*\>.cfg

Orbiter can support multiple planetary systems, but only one per
scenario. The planetary system to be used by a scenario is referenced in
the scenario file (see Chapter [3](#anchor-7)).

### General parameters

|            |        |                                                                                                                |
|------------|--------|----------------------------------------------------------------------------------------------------------------|
| Item       | Type   | Description                                                                                                    |
| Name       | String | A name for the planetary system                                                                                |
| MarkerPath | String | Directory path containing surface marker lists for the planet. Default: .\\Config\\\<*Solsys-name*\>\\Marker\\ |

The MarkerPath entry is used only for legacy-style marker definitions
(see Chapter [2.5](#anchor-8)). For new quadtree-based markers, see
*Planetary Textures*, Chapter 9 “Label tile format”.

### Object list

The object list defines the celestial bodies populating the planetary
system and their hierarchy.

Star entries:

Star\<*i*\> = \<*Name*\>

where \<*i*\> is an index running from 1 upward. (Note: planetary
systems with more than one central star are not currently supported).

Planet entries:

Planet\<*i*\> = \<*Name*\>

where \<*i*\> is an index running from 1 upward.

Moon entries:

\<*Planet*\>:Moon\<*i*\> = \<*Name*\>

where \<*Planet*\> is the name of a planet defined before, and \<*i*\>
is an index enumerating the moons of this planet, running from 1 upward.

Example:

Star1 = Sun

Planet1 = Mercury

Planet2 = Venus

Planet3 = Earth

Earth:Moon1 = Moon

Planet4 = Mars

Mars:Moon1 = Phobos

Mars:Moon2 = Deimos

## <span id="anchor-9"></span><span id="anchor-10"></span>Planets

Adding a new celestial body (planet or moon) to a planetary system
requires the following steps:

-   Add an entry for the body in the planetary system configuration file
    (see previous chapter).
-   Create a configuration file for the planet, defining its orbital,
    physical and visual parameters, located in
    .\\Config\\\<*Planet-name*\>.cfg, for example, Config\\Earth.cfg.
    The configuration entries are described below.
-   Create the required surface texture maps, and optionally elevation,
    night light, water mask, cloud layer and feature label maps. See
    document *PlanetTextures.pdf* for details.
-   Optionally, create configuration files for surface bases in
    .\\Config\\\<*Planet-name*\>\\Base and reference them in the planet
    configuration file. See Chapter [2.4](#anchor-11) for details on
    surface base definitions.
-   Optionally, compile a DLL module for the planet for ephemeris
    calculations and atmospheric parameter computation. See the Orbiter
    sources for examples, e.g. the Src\\Celbody\\Vsop87 project.
-   Optionally, create polyline definitions for coastlines
    (.\\Config\\\<*Planet-name*\>\\Data\\coast.vec) and/or topography
    (.\\Config\\\<*Planet-name*\>\\Data\\contour.vec) to be used in
    scalable map displays. See .\\Config\\Earth\\Data\\coast.vec for an
    example.

The components of the planet configuration file are listed below. An
example for a configuration file can be found in .\\Config\\Earth.cfg.

### General parameters

|               |        |                                                                                                                                                                                                                   |
|---------------|--------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Item          | Type   | Description                                                                                                                                                                                                       |
| Name          | String | Planet name                                                                                                                                                                                                       |
| Module        | String | Name of a dynamic link library performing calculations for the planet. Default: none                                                                                                                              |
| ErrorLimit    | Float  | Max. rel. error for position/velocity calculations (only used if the module supports precision adjustment)                                                                                                        |
| EllipticOrbit | Bool   | If TRUE, use analytic 2-body solution for planet position/velocity calculation, otherwise update dynamically (ignored if module supports position/velocity calculation)                                           |
| HasElements   | Bool   | If TRUE, the initial position/velocity is calculated from the provided set of orbital elements, otherwise from an explicit position/velocity pair (ignored if the module supports posi­tion/velocity calculation) |

Notes:

-   If the module calculates the planet position and velocity from a
    numerical series expansion solution, the value of ErrorLimit affects
    the number of terms used for the calculation. A lower value
    increases the number of terms required to achieve that limit, and
    thus increases calculation time. The valid range for ErrorLimit
    depends on the module, but is typically 10<sup>−3</sup> ≤ ErrorLimit
    ≤ 10<sup>−8</sup>.

### Orbital parameters

Ignored if a module is active that supports position/velocity
calculation or if HasElements = FALSE.

|                |       |                                                                      |
|----------------|-------|----------------------------------------------------------------------|
| Item           | Type  | Description                                                          |
| Epoch          | Float | Orbital element reference epoch (e.g. 2000)                          |
| ElReference    | Flag  | Orbit reference frame (ParentEquator or Ecliptic). Default: Ecliptic |
| SemiMajorAxis  | Float | Orbit semi-major axis *a* \[m\]                                      |
| Eccentricity   | Float | Orbit eccentricity *e*.                                              |
| Inclination    | Float | Orbit inclination against reference frame *i* \[rad\]                |
| LongAscNode    | Float | Longitude of ascending node *Ω* \[rad\]                              |
| LongPerihelion | Float | Longitude of periapsis *ϖ* \[rad\]                                   |
| MeanLongitude  | Float | Mean longitude *l* at epoch \[rad\]                                  |

### Physical parameters

|      |       |                          |
|------|-------|--------------------------|
| Item | Type  | Description              |
| Mass | Float | Planet mass \[kg\]       |
| Size | Float | Mean planet radius \[m\] |

### Rotation and precession elements

See also Doc\\Technotes\\precession.pdf.

|                     |       |                                                                                                         |
|---------------------|-------|---------------------------------------------------------------------------------------------------------|
| Item                | Type  | Description                                                                                             |
| SidRotPeriod        | Float | Sidereal rotation period \[s\]. Default: infinite                                                       |
| SidRotOffset        | Float | Rotation at epoch \[rad\]. Default: 0                                                                   |
| Obliquity           | Float | Obliquity of axis: angle between planet axis and precession reference axis \[rad\]. Default: 0          |
| LAN                 | Float | Longitude of ascending node of equatorial plane \[rad\]. Default: 0                                     |
| LAN_MJD             | Float | Reference data for LAN \[MJD\]. Default: 51544.5                                                        |
| PrecessionPeriod    | Float | Period of precession of axis \[days\]. Default: infinite                                                |
| PrecessionObliquity | Float | Obliquity of precession reference axis with respect to ecliptic north pole at J2000 \[rad\]. Default: 0 |
| PrecessionLAN       | Float | Longitude of ascending node of precession reference plane \[rad\]. Default: 0                           |

### Atmospheric parameters

Only required if the planet defines an atmosphere.

<table>
<tbody>
<tr class="odd">
<td>Item</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>AtmPressure0</td>
<td>Float</td>
<td>(Mean) atmospheric pressure at zero altitude (reference radius) [Pa]</td>
</tr>
<tr class="odd">
<td>AtmDensity0</td>
<td>Float</td>
<td>(Mean) atmospheric density at zero altitude [kg/m<sup>3</sup>]</td>
</tr>
<tr class="even">
<td>AtmGasConstant</td>
<td>Float</td>
<td>Specific gas constant [J K<sup>−1</sup> kg<sup>−1</sup>]. Default: 286.91 (Earth value)</td>
</tr>
<tr class="odd">
<td>AtmGamma</td>
<td>Float</td>
<td>Ratio of specific heats <em>c</em><sub><em>p</em></sub>/<em>c</em><sub><em>v</em></sub>. Default: 1.4 (Earth value)</td>
</tr>
<tr class="even">
<td>AtmColor0</td>
<td>Vec3</td>
<td>RGB triplet for atmospheric colour at ground level (0–1 each)</td>
</tr>
<tr class="odd">
<td>AtmAltLimit</td>
<td>Float</td>
<td>Altitude limit beyond which atmospheric effects can be ignored [m]</td>
</tr>
<tr class="even">
<td>AtmHazeExtent</td>
<td>Float</td>
<td>Width parameter for extent of horizon haze rendering. Range: 0 (thinnest) to 1 (widest). Default: 0.1</td>
</tr>
<tr class="odd">
<td>AtmHazeShift</td>
<td>Float</td>
<td>Shift the reference altitude of the haze base line (in units of planet radius). Can be used to adjust haze altitude to a cloud layer. Default: 0 (align with surface horizon). Shift is not applied if camera is below cloud layer.</td>
</tr>
<tr class="even">
<td>AtmHazeDensity</td>
<td>Float</td>
<td>Modify the density at which the horizon haze is rendered (basic density is calculated from atmospheric density). Default: 1.0</td>
</tr>
<tr class="odd">
<td>AtmHazeColor</td>
<td>Vec3</td>
<td>RGB triplet for horizon haze colour (0–1 each). Default: use AtmColor0 values.</td>
</tr>
<tr class="even">
<td>AtmFogColor</td>
<td>Vec3</td>
<td>RGB triplet for distance fog colour (0–1 each).</td>
</tr>
<tr class="odd">
<td>AtmFogParam</td>
<td>Vec3</td>
<td><p>Value 1: fog density at surface level</p>
<p>Value 2: fog density at reference altitude</p>
<p>Value 3: reference altitude [m]</p></td>
</tr>
<tr class="even">
<td>AtmTintColor</td>
<td>Vec3</td>
<td>RGB triplet for additive colour component (0–1 each) added to surface rendering at high altitude</td>
</tr>
<tr class="odd">
<td>AtmHorizonAlt</td>
<td>Float</td>
<td>Altitude scale for horizon haze rendering [m]. Default: 0.01 of planet radius.</td>
</tr>
<tr class="even">
<td>ShadowDepth</td>
<td>Float</td>
<td>Depth (“blackness”) of object shadows (0–1, where 0 = black, 1 = no shadows). Default: exp(−<em>ρ</em><sub>0</sub>/2), where <em>ρ</em><sub>0</sub> is atmospheric density at the surface. This option is only used when stencil buffering is enabled, otherwise shadows are always black.</td>
</tr>
</tbody>
</table>

### Cloud parameters

Only required if the planet defines a cloud layer.

<table>
<tbody>
<tr class="odd">
<td>Item</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>CloudFormat</td>
<td>Int</td>
<td>1 = legacy, 2 = 2016 format. Default: 1</td>
</tr>
<tr class="odd">
<td>CloudAlt</td>
<td>Float</td>
<td>Altitude of cloud layer [m]</td>
</tr>
<tr class="even">
<td>CloudShadowDepth</td>
<td>Float</td>
<td>Depth (“blackness”) of cloud shadows on the ground (0–1, where 0 = black, 1 = don’t render shadows). Default: 1</td>
</tr>
<tr class="odd">
<td>CloudRotPeriod</td>
<td>Float</td>
<td>Rotation period of cloud layer against surface [s]. Default: 0 (static cloud layer)</td>
</tr>
<tr class="even">
<td>CloudMicrotextureAlt</td>
<td>Float Float</td>
<td><p>Altitude range for cloud microtexturing.</p>
<p>Value 1: altitude at which full microtexturing is applied.</p>
<p>Value 2: altitude at which microtexture starts to kick in</p>
<p>Value 1 ≥ 0 and Value 2 &gt; Value 1 is required. Default: no microtexture</p></td>
</tr>
<tr class="odd">
<td>BrightenClouds</td>
<td>Bool</td>
<td>Use a brighter cloud rendering algorithm</td>
</tr>
</tbody>
</table>

### Visualisation parameters

|                    |       |                                                                                                                                                                |
|--------------------|-------|----------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Item               | Type  | Description                                                                                                                                                    |
| TileFormat         | Int   | 1 = legacy, 2 = 2016 format. Default: 1                                                                                                                        |
| MaxPatchResolution | Int   | Max. resolution level for surface texture maps (1–19)                                                                                                          |
| MinCloudResolution | Int   | Min. resolution at which clouds are rendered as a separate layer (1–8)                                                                                         |
| MaxCloudResolution | Int   | Max. cloud resolution level (MinCloudResolution – 19)                                                                                                          |
| MinElevation       | Float | Min. surface elevation rel. to planet mean radius \[m\]. Used to adjust lower edge of rendered horizon.                                                        |
| SpecularRipple     | Bool  | If TRUE, and if “Specular ripples” option is enabled in the Launchpad dialog, specularly reflecting surfaces use a “water ripple” microtexture. Default: FALSE |

### Surface marker parameters (optional)

|            |        |                                                                                                        |
|------------|--------|--------------------------------------------------------------------------------------------------------|
| Item       | Type   | Description                                                                                            |
| MarkerPath | String | Directory path to surface marker files for the planet. Default: .\\Config\\\<*Planet-name*\>\\Marker\\ |

This entry refers to legacy surface markers (see Chapter
[2.5](#anchor-8)). For new quadtree-based marker definitions, see
*Planetary Textures*, Chapter 9 “Label tile format”.

### Surface bases (optional)

This is a list containing the names and locations of surface landing
installations (“spaceports”). Each entry in the list must be accompanied
by a configuration file for the corresponding surface base (see Chapter
[2.4](#anchor-11)).

BEGIN_SURFBASE

\<*base list*\>

END_SURFBASE

Base list entries have the following format:

\<*name*\> \<*lng*\> \<*lat*\>

where

\<*name*\> Name identifying the base config file (\<*name*\>.cfg). The
actual base name as it appears in Orbiter is given by the NAME tag in
the base config file.

\<*lng*\> \<*lat*\> Base centre position in equatorial coordinates
\[deg\]

### Ground-based observer sites (optional)

This is a list containing the pre-defined locations for ground-based
observers (launch cameras, spectators, etc.) which can be selected in
the Camera dialog. The format of the list is

BEGIN_OBSERVER

\<*observer list*\>

END_OBSERVER

List entries have the following format:

\<*site*\>:\<*spot*\>: \<*lng*\> \<*lat*\> \<*alt*\>

where

\<*site*\> Name identifying the site (e.g. KSC)

\<*spot*\> The particular location at the site (e.g. Launch pad 39A)

\<*lng*\> \<*lat*\> Observer position in equatorial coordinates \[deg\]

\<*alt*\> observer altitude (> 0) \[m\]

The easiest way to find the coordinates for a new observer spot is to
open the Camera dialog (<sub></sub>), and select a nearby location
under the Ground tab. Then move the camera to the new spot using
<sub></sub> and <sub></sub>. The coordinates are displayed in the
dialog and can be directly copied into the configuration file.

### Navbeacon transmitter list (optional)

This is a list containing the specs of all navigation radio transmitters
on the planet surface except for those directly associated with a
spaceport, which are defined in the base definition file (see Chapter
[2.4](#anchor-11)). The list format is as follows:

BEGIN_NAVBEACON

\<*NAV list*\>

END_NAVBEACON

List entries defining individual transmitters have the following format:

\<*type*\> \<*id*\> \<*lng*\> \<*lat*\> \<*freq*\> \[\<*range*\>\]

where

\<*type*\> transmitter type, currently supported: VOR

\<*id*\> identifier code (up to 4 characters)

\<*lng*\> \<*lat*\> transmitter position in equatorial coordinates
\[deg\]

\<*freq*\> transmitter frequency \[MHz\]

\<*range*\> transmitter range \[m\]. Default: 500 000

## <span id="anchor-12"></span><span id="anchor-11"></span>Surface bases

Surface bases (or “spaceports”) are launch and landing sites on the
surface of planets or moons, usually equipped with launchpads or
runways, for vertical and horizontal take-off and/or landing. Each
surface base is defined in its own configuration file. When Orbiter
loads a planet configuration, it scans all surface base definitions and
creates the corresponding bases.

### Base definition file

Each surface base must be defined with a base configuration file. By
default, this will be located in

.\\Config\\\<*Planet-name*\>\\Base\\\<*Base-name*\>.cfg

where \<*Planet-name*\> is the name of the celestial body the base is
located on, and \<*Base-name*\> is the name of the surface base.
Different locations for the base folder can be defined, for example to
allow selective base loading (see *Linking surface bases to planets*
below).

The format of the surface base definition file is as follows:

BASE-V2.0

NAME = \<*Base-name*\>

LOCATION = \<*lng*\> \<*lat*\>

SIZE = \<*size*\>

OBJECTSIZE = \<*osize*\>

MAPOBJECTSTOSPHERE = \[TRUE\|FALSE\]

BEGIN_NAVBEACON

\<*NAV list*\>

END_NAVBEACON

BEGIN_OBJECTLIST

\<*Object list*\>

END_OBJECTLIST

BEGIN_SURFTILELIST

\<*Surface tile list*\>

END_SURFTILELIST

where

BASE-V2.0

Format identifier (must be placed in the first line of the file). This
item is optional if the base is defined directly in the planet’s base
list.

NAME = \<*Base-name*\>

Logical name of the base as it will be displayed by Orbiter. Doesn’t
need correspond to the base file name.

LOCATION = \<*lng*\> \<*lat*\>

The position of the base reference point on the planet surface, where
\<*lng*\> \[deg\] is longitude (\< 0 for west, \>0 for east), and
\<*lat*\> \[deg\] is latitude (\< 0 for south, \> 0 for north). This
item is optional if the base is defined directly in the planet’s base
list.

SIZE = \<*size*\>

Specifies the size of the base’s footprint on the planet surface
(defined as its mean radius \[m\]. This parameter determines the camera
distance at which Orbiter will start rendering the base.

OBJECTSIZE = \<*osize*\>

Defines the “typical” size of individual base objects (buildings, etc.).
In addition to the SIZE parameter, this value is used by Orbiter to
fine-tune the rendering of base objects. Objects will not be rendered if
the apparent size of an object of size \<*osize*\> located at the base
reference point, would be smaller than 1 pixel. The default value for
\<*osize*\> is 100.0.

MAPOBJECTSTOSPHERE = \[TRUE\|FALSE\]

If TRUE, the altitude of objects in the object list will be adjusted
automatically to map them from a flat plane onto a spherical planet
surface. So for example, a building defined with altitude 0 will be
rendered at altitude 0 relative to the planet’s reference sphere. If
FALSE, elevation 0 maps onto the flat plane tangential to the base
reference point (and therefore to a point above the reference sphere for
any objects not located at the reference point). Default: FALSE

Note: Currently this function is only implemented for a limited number
of base object types.

\<*NAV list*\>

Contains a list of navigation radio transmitters associated with the
base. The format of the list is identical to the \<*NAV list*\> in the
planet configuration file (see Chapter [2.3](#anchor-10)).

\<*Object list*\>

Contains a list of objects which make up the visual elements of the
base. See next section for details.

\<*Surface tile list*\>

This list is deprecated and is only retained for backward compatibility
of old planet definitions. See document *PlanetTextures.pdf* for details
on how to define surface textures for new planet definitions, including
high-resolution patches around surface bases.

### Linking surface bases to planets

After creating the base configuration file, it must be referenced by a
planet to instantiate the base. There are several ways to make Orbiter
read a surface base definition:

-   Place the base configuration file in the default base configuration
    folder for the planet. By default, Orbiter scans the folder
    .\\Config\\\<*Planet-name*\>\\Base for base definitions. The default
    folder is scanned only if the planet doesn’t explicitly define a
    base list (see below).
-   To make Orbiter scan different folders, create a surface base list
    in the planet’s configuration file, enclosed by BEGIN_SURFBASE and
    END_SURFBASE tags (see Chapter [2.3](#anchor-10)). In this list,
    specify the new surface base directory with the line ‘DIR
    \<*folder*\>’, where \<*folder*\> is the path to the folder
    containing the base configurations (relative to the Config folder).
    Multiple folders can be specified. If the same base is defined in
    more than one of the scanned folders, only the first is used. This
    allows to replace base definitions without having to delete the
    original configuration file. Example:

BEGIN_SURFBASE

DIR Earth\\MyBases

DIR Earth\\MoreBases

END_SURFBASE

If the surface base list exists in a planet’s configuration file, the
default base configuration folder is not scanned, unless it is
explicitly listed.

-   Base references can be placed directly into the base list, using the
    following format:

\<*Base-name*\>:\<*lng*\> \<*lat*\>

where \<*Base-name*\> is the name of the base configuration file,
.\\Config\\\<*Base-name*\>.cfg, and \<*lng*\> and \<*lat*\> are the
equatorial coordinates (longitude and latitude) of the base reference
point in degrees. This position supersedes the one defined in the base
configuration file, so the same base can be used in multiple locations.
When using this format, the base configuration file must be located in
the .\\Config folder.

### Selective loading of bases

To provide more control over the loading of surface bases in a specific
simulation scenario, two conditional flags can be set with a DIR entry
in the base list:

-   Specify a time interval with the PERIOD parameter. The corresponding
    directory is only scanned if the scenario start date is inside this
    interval. This allows to add or replace surface bases only during
    specific time periods, for example to set up Kennedy Space Center
    for the Apollo lunar missions. Syntax:

DIR \<*folder*\> PERIOD \<*MJD0*\> \<*MJD1*\>

where \<*MJD0*\> and \<*MJD1*\> are the start and end dates of the
period over which the base folder is scanned, in MJD (Modified Julian
Date) format \[days\]. Either can be set to ‘–’ to disable the limit at
one end.

-    Specify a scenario context with the CONTEXT parameter. The
    corresponding directory is scanned only if the scenario specifies
    the same context string in its environment context entry (see
    Chapter [3](#anchor-7)). This allows to add or replace bases only
    within a defined set of scenarios. Syntax:

DIR \<*folder*\> CONTEXT \<*string*\>

where \<*string*\> is the context string to be matched against the
scenario context.

The PERIOD and CONTEXT parameters can be used simultaneously. In that
case the directory is only scanned if both conditions are satisfied.
Examples:

BEGIN_SURFBASE

DIR Earth\\1969Base PERIOD 40222 42048

DIR Earth\\TempBases CONTEXT RichScenery

DIR Earth\\OtherBases PERIOD - 40000 CONTEXT EarlyBases

DIR Earth\\Base

END_SURFBASE

Note that the list order is important to allow the custom base
definitions to replace standard bases in the Base subfolder.

### Adding objects to surface bases

Surface bases are composed of objects (buildings, hangars, launch pads,
train lines, etc.) The configuration file for each surface base contains
a list of its objects:

BEGIN_OBJECTLIST

\<*Object 0*\>

\<*Object 1*\>

...

END_OBJECTLIST

Each object entry in the list defines a particular object and its
properties (type, position, size, textures, etc.) An object can either
be a pre-defined type or a generic mesh. Each object entry has the
following format:

\<*Type*\>

\<*Parameters*\>

END

Note that textures used by base objects must be listed in the texture
list of the Base.cfg configuration file.

The following pre-defined object types are currently supported:

BLOCK

A 5-sided “brick” (without a floor) which can be used as a simple
generic building, or as part of a more complex structure. The following
parameters are supported:

|           |       |                                                                                                                                 |
|-----------|-------|---------------------------------------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                                                     |
| POS       | V     | Centre of the block’s base rectangle (in local base coordinates) The y-coordinate is the elevation above ground. Default: 0 0 0 |
| SCALE     | V     | Object size in the three coordinate axes. Default: 1 1 1                                                                        |
| ROT       | F     | Rotation around vertical axis \[deg\]. Default: 0                                                                               |
| TEX1      | S F F | Texture name and u, v scaling factors for walls along the x-axis. Default: no texture                                           |
| TEX2      | S F F | Texture name and u, v scaling factors for walls along the z-axis. Default: no texture                                           |
| TEX3      | S F F | Texture name and u, v scaling factors for roof. Default: no texture                                                             |

(V = Vector, F = Float, S = String)

HANGAR

A hangar-type building with a barrel-shaped roof. The following
parameters are supported:

|           |       |                                                                                                                                  |
|-----------|-------|----------------------------------------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                                                      |
| POS       | V     | Centre of the object’s base rectangle (in local base coordinates) The y-coordinate is the elevation above ground. Default: 0 0 0 |
| SCALE     | V     | Object size in the three coordinate axes. Default: 1 1 1                                                                         |
| ROT       | F     | Rotation around vertical axis \[deg\]. Default: 0                                                                                |
| TEX1      | S F F | Texture name and u, v scaling factors for walls. Default: no texture                                                             |
| TEX2      | S F F | Texture name and u, v scaling factors for front gate. Default: no texture                                                        |
| TEX3      | S F F | Texture name and u, v scaling factors for roof. Default: no texture                                                              |

(V = Vector, F = Float, S = String)

HANGAR2

A hangar-type building with a tent-shaped roof. The following parameters
are supported:

|           |       |                                                                                                                                  |
|-----------|-------|----------------------------------------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                                                      |
| POS       | V     | Centre of the object’s base rectangle (in local base coordinates) The y-coordinate is the elevation above ground. Default: 0 0 0 |
| SCALE     | V     | Object size in the three coordinate axes. Default: 1 1 1                                                                         |
| ROT       | F     | Rotation around vertical axis \[deg\]. Default: 0                                                                                |
| TEX1      | S F F | Texture name and u, v scaling factors for front and back walls. Default: no texture                                              |
| TEX2      | S F F | Texture name and u, v scaling factors for side walls. Default: no texture                                                        |
| TEX3      | S F F | Texture name and u, v scaling factors for roof. Default: no texture                                                              |
| ROOFH     | F     | Roof height from base to ridge. Default: 1/2 building height.                                                                    |

(V = Vector, F = Float, S = String)

HANGAR3

A hangar-type building with a barrel-shaped roof reaching to the ground.
The following parameters are supported:

|           |       |                                                                                                                                  |
|-----------|-------|----------------------------------------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                                                      |
| POS       | V     | Centre of the object’s base rectangle (in local base coordinates) The y-coordinate is the elevation above ground. Default: 0 0 0 |
| SCALE     | V     | Object size in the three coordinate axes. Default: 1 1 1                                                                         |
| ROT       | F     | Rotation around vertical axis \[deg\]. Default: 0                                                                                |
| TEX1      | S F F | Texture name and u, v scaling factors for front and back walls. Default: no texture                                              |
| TEX2      | S F F | Texture name and u, v scaling factors for front gate. Default: no texture                                                        |
| TEX3      | S F F | Texture name and u, v scaling factors for roof. Default: no texture                                                              |

(V = Vector, F = Float, S = String)

TANK

A fuel tank-like upright cylinder with a flat top. The following
parameters are supported:

|           |       |                                                                                                                               |
|-----------|-------|-------------------------------------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                                                   |
| POS       | V     | Centre of the object’s base circle (in local base coordinates) The y-coordinate is the elevation above ground. Default: 0 0 0 |
| SCALE     | V     | Cylinder radii in x and z, and height in y. Default: 1 1 1                                                                    |
| ROT       | F     | Rotation around vertical axis \[deg\]. Default: 0                                                                             |
| NSTEP     | I     | Number of segments to approximate circle. Default: 12                                                                         |
| TEX1      | S F F | Texture name and u, v scaling factors for mantle. Default: no texture                                                         |
| TEX2      | S F F | Texture name and u, v scaling factors for top. Default: no texture                                                            |

(V = Vector, F = Float, I = Integer, S = String)

RUNWAY

Texturing for a runway. The texture mapping can be split into segments,
to allow the inclusion of markings, overruns, etc. The RUNWAY object
does not include any lighting, which is instead provided with the
RUNWAYLIGHTS object (see below).

<table>
<tbody>
<tr class="odd">
<td>Parameter</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>END1</td>
<td>V</td>
<td>First end point of the runway (centre line), including any overruns to be textured.</td>
</tr>
<tr class="odd">
<td>END2</td>
<td>V</td>
<td>Second end point of the runway centre line.</td>
</tr>
<tr class="even">
<td>WIDTH</td>
<td>F</td>
<td>Runway width [m]</td>
</tr>
<tr class="odd">
<td>ILS1</td>
<td>F</td>
<td>Localiser frequency for approach towards END1 [MHz]. Range: 108.00–139.95. Default: no ILS support</td>
</tr>
<tr class="even">
<td>ILS2</td>
<td>F</td>
<td>Localiser frequency for approach towards END2. Can be the same frequency as ILS1. Default: no ILS support</td>
</tr>
<tr class="odd">
<td>NRWSEG</td>
<td>I</td>
<td>Number of texture segments</td>
</tr>
<tr class="even">
<td>RWSEGx</td>
<td>I F F F F F</td>
<td><p>Definition of segment x (x = 1–NRWSEG)</p>
<p>Parameters:</p>
<p>1. Number of mesh sub-segments (≥ 1)</p>
<p>2. Fractional length of segment (sum of all segments must be 1)</p>
<p>3. Texture coordinate u<sub>0</sub> of segment</p>
<p>4. Texture coordinate u<sub>1</sub></p>
<p>5. Texture coordinate v<sub>0</sub></p>
<p>6. Texture coordinate v<sub>1</sub></p></td>
</tr>
<tr class="odd">
<td>RWTEX</td>
<td>S</td>
<td>Texture name for all segments</td>
</tr>
</tbody>
</table>

(V = Vector, F = Float, I = Integer, S = String)

RUNWAYLIGHTS

Complete lighting for a single runway, including optional Precision
Approach Path Indicator (PAPI) and Visual Approach Slope Indicator
(VASI). Runway markers are turned off during daytime, but PAPI and VASI
indicators are always active.

<table>
<tbody>
<tr class="odd">
<td>Parameter</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>END1</td>
<td>V</td>
<td>First end point of the runway (centre line).</td>
</tr>
<tr class="odd">
<td>END2</td>
<td>V</td>
<td>Second end point of the runway centre line.</td>
</tr>
<tr class="even">
<td>WIDTH</td>
<td>F</td>
<td>Runway width [m]</td>
</tr>
<tr class="odd">
<td>COUNT1</td>
<td>I</td>
<td>Number of lights along the runway centre line (≥ 2). Default: 40</td>
</tr>
<tr class="even">
<td>PAPI</td>
<td>F F F</td>
<td><p>Precision Approach Path Indicator (PAPI). Default: no PAPI</p>
<p>Parameters:</p>
<p>Designated approach descent angle [deg]</p>
<p>Approach cone aperture [deg]</p>
<p>Offset of PAPI location from runway endpoints [m]</p></td>
</tr>
<tr class="odd">
<td>VASI</td>
<td>F F F</td>
<td><p>Visual Approach Slope Indicator (VASI). Default: no VASI</p>
<p>Parameters:</p>
<p>Designated approach descent angle [deg]</p>
<p>Distance between white and red indicator lights [m]</p>
<p>Offset of VASI (red bar) location from runway endpoints [m]</p></td>
</tr>
</tbody>
</table>

(V = Vector, F = Float, I = Integer)

BEACONARRAY

A linear array of illuminated beacons, usable e.g. for taxiway night
lighting.

|           |       |                                                                                                    |
|-----------|-------|----------------------------------------------------------------------------------------------------|
| Parameter | Type  | Description                                                                                        |
| END1      | V     | First end point of beacon array in local base coordinates. y-coordinate is elevation above ground. |
| END2      | V     | Second end point of beacon array.                                                                  |
| COUNT     | I     | Number of beacons in the array (≥ 2). Default: 10                                                  |
| SIZE      | F     | Size (radius) of each beacon light. Default: 1.0                                                   |
| COL       | F F F | Beacon colour (RGB). Range: 0–1 for each value. Default: 1 1 1 (white)                             |

(V = Vector, F = Float, I = Integer)

SOLARPLANT

A grid of ground-mounted solar panels, smart enough to align themselves
with the Sun. The following parameters are supported:

|           |           |                                                                       |
|-----------|-----------|-----------------------------------------------------------------------|
| Parameter | Type      | Description                                                           |
| POS       | V         | Centre position of the panel grid. Default: 0 0 0                     |
| SCALE     | F         | Scaling factor for each panel. Default: 1.0                           |
| SPACING   | F F       | Distance between panels in x and z directions \[m\]. Default: 40 40   |
| GRID      | I I       | Grid dimensions in x and z directions. Default: 2 2                   |
| ROT       | F         | Rotation of plant around vertical axis \[deg\]. Default: 0            |
| TEX       | S \[F F\] | Texture name and u, v scaling factors for panels. Default: no texture |

(V = Vector, F = Float, I = Integer, S = String)

TRAIN1

A monorail-type train on a straight track. The following parameters are
supported:

|           |      |                                                                                |
|-----------|------|--------------------------------------------------------------------------------|
| Parameter | Type | Description                                                                    |
| END1      | V    | First end point of track                                                       |
| END2      | V    | Second end point of track                                                      |
| MAXSPEED  | F    | Maximum speed of train \[m/s\]. Default: 30                                    |
| SLOWZONE  | F    | Distance over which the train slows down at the track ends \[m\]. Default: 100 |
| TEX       | S    | Texture name                                                                   |

(V = Vector, F = Float, S = String)

TRAIN2

Suspended train on elevated straight track. The following parameters are
supported:

|           |      |                                                                                |
|-----------|------|--------------------------------------------------------------------------------|
| Parameter | Type | Description                                                                    |
| END1      | V    | First end point of track                                                       |
| END2      | V    | Second end point of track                                                      |
| HEIGHT    | F    | Elevation of suspension track over ground \[m\]. Default: 11                   |
| MAXSPEED  | F    | Maximum speed of train \[m/s\]. Default: 30                                    |
| SLOWZONE  | F    | Distance over which the train slows down at the track ends \[m\]. Default: 100 |
| TEX       | S    | Texture name                                                                   |

(V = Vector, F = Float, S = String)

LPAD1

An octagonal bordered landing pad. Default diameter 80 m (at scale 1).
Landing pads are numbered in the order they appear in the list. This
landing pad style can be assigned numbers 1–9, so must be within the
first 9 landing pad definitions of the base object list. Orbiter
provides a texture map compatible with LPAD1 (Textures\\Lpad01.dds). Any
user-defined texture should adhere to the same layout.

|           |      |                                                                                       |
|-----------|------|---------------------------------------------------------------------------------------|
| Parameter | Type | Description                                                                           |
| POS       | V    | Pad centre coordinates in local base frame.                                           |
| SCALE     | F    | Scaling factor. Default: 1                                                            |
| ROT       | F    | Rotation around vertical axis \[deg\]. Default: 0                                     |
| TEX       | S    | Texture name. Default: no texture                                                     |
| NAV       | F    | Frequency of VTOL nav transmitter \[MHz\] (range 85.0–140.0). Default: no transmitter |

(V = Vector, F = Float, S = String)

LPAD2

A square landing pad. Default size 80 m (at scale 1). Landing pads are
numbered in the order they appear in the list. This landing pad style
can be assigned numbers 1–99, so must be within the first 99 landing pad
definitions of the base object list. Orbiter provides a texture map
compatible with LPAD2 (Textures\\Lpad02.dds). Any user-defined texture
should adhere to the same layout.

|           |      |                                                                                       |
|-----------|------|---------------------------------------------------------------------------------------|
| Parameter | Type | Description                                                                           |
| POS       | V    | Pad centre coordinates in local base frame.                                           |
| SCALE     | F    | Scaling factor. Default: 1                                                            |
| ROT       | F    | Rotation around vertical axis \[deg\]. Default: 0                                     |
| TEX       | S    | Texture name. Default: no texture                                                     |
| NAV       | F    | Frequency of VTOL nav transmitter \[MHz\] (range 85.0–140.0). Default: no transmitter |

(V = Vector, F = Float, S = String)

LPAD2A

Similar to LPAD2, but uses a different layout for the texture map,
providing a higher resolution at the same texture size. Orbiter provides
a texture map compatible with LPAD2A (Textures\\Lpad02a.dds). Any
user-defined texture should adhere to the same layout. The supported
parameters are the same as for LPAD2.

MESH

Generic mesh for custom objects. Mesh files must be in Orbiter mesh file
format (see Chapter [5](#anchor-13)).

|               |                                                                                                                                                             |                                                                                                                                       |
|---------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| Parameter     | Type                                                                                                                                                        | Description                                                                                                                           |
| FILE          | S                                                                                                                                                           | Mesh file name (without path and extension). Mesh files must be located in the mesh subdirectory (as defined in the main config file) |
| POS           | V                                                                                                                                                           | Position of the mesh origin in local base coordinates                                                                                 |
| SCALE         | V                                                                                                                                                           | Scaling factors in along the 3 axes (height in y). Default: 1 1 1                                                                     |
| ROT           | F                                                                                                                                                           | Rotation around the vertical axis \[deg\]. Default: 0                                                                                 |
| TEX           | S                                                                                                                                                           | Texture name. Default: none                                                                                                           |
| WRAPTOSURFACE | Wrap mesh vertices to the planet surface (e.g. taxiways and similar)                                                                                        |                                                                                                                                       |
| SHADOW        | Render the shadow cast on the ground by the object (cannot be used together with OWNSHADOW)                                                                 |                                                                                                                                       |
| OWNSHADOW     | Use group shadow flags in mesh file to set shadows for individual mesh groups (cannot be used together with SHADOW)                                         |                                                                                                                                       |
| UNDERSHADOWS  | Object can be covered by shadows cast on the ground by other objects (e.g. roads, landing pads, etc.). Default: object not covered by ground shadows        |                                                                                                                                       |
| OWNMATERIAL   | Use materials and textures defined in the mesh file. This overrides the TEX entry.                                                                          |                                                                                                                                       |
| LPAD          | Object is a landing pad                                                                                                                                     |                                                                                                                                       |
| PRELOAD       | Mesh should be loaded at program start. This can reduce disc activity during the simulation, but increases main memory usage. Default: load only when used. |                                                                                                                                       |

(V = Vector, F = Float, S = String)

Notes:

-   If the mesh only uses a single texture it is more efficient to
    specify it via the TEX entry instead of defining it in the mesh and
    using the OWNMATERIAL flag, because Orbiter can merge objects with
    the same TEX entries for improved performance.
-   If OWNSHADOW is used, any mesh groups which have bit 0 set in their
    FLAG entry do not cast shadows, otherwise they do cast shadows (see
    Chapter [5](#anchor-13)).

## <span id="anchor-14"></span><span id="anchor-8"></span>Custom markers

*Note:* As regards markers for planetary surface featues, this section
contains information for old-style (pre-Orbiter 2016) marker
definitions. These are retained for backward compatibility. For new,
quadtree-based marker definitions, see Chapter *Label Tile Format* of
document *PlanetTextures.pdf*.

Labels can be defined to mark objects on the celestial sphere (e.g.
bright stars, navigation stars, nebulae, etc.) or on planetary surfaces
to locate natural landmarks, points of interest, historic landing sites,
navigational aids, etc.

The user can display these markers during the simulation with the
*Visual helpers* option (<sub></sub> or <sub></sub>).

By default, all files for celestial and planetary surface markers are
placed in a subdirectory

.\\Config\\\<*name*\>\\Marker

where \<name> is the name of the planetary system (for celestial
markers) or the celestial body (for surface markers) they are referring
to. A different location for marker files can be specified with the
MarkerPath option in the planet’s or planetary system’s configuration
file (see Chapters [2.2](#anchor-5) and [2.3](#anchor-10)). Marker files
are text files with extension .mkr. Multiple files can be defined for a
single planet or planetary system, which the user can turn on and off
individually. The format is

BEGIN_HEADER

InitialState \[on\|off\]

ShapeIdx \[0–6\]

ColourIdx \[0–5\]

Size \[0.1–2\]

DistanceFactor \[1e−5 – 1e3\]

Frame \[celestial\|ecliptic\]

END_HEADER

BEGIN_DATA

\<*lng*\> \<*lat*\> : \<*label*\> \[: \<*label*\>\]

\<*lng*\> \<*lat*\> : \<*label*\> \[: \<*label*\>\]

. . .

The header section contains some configuration options:

-   InitialState defines if the labels are initially visible when the
    user activates surface or celestial markers in the *Visual helpers*
    dialog. The user can turn lists on and off individually during the
    simulation. The default is off.
-   ShapeIdx is an integer between 0 and 6 defining the shape of the
    label markers:

|     |     |                   |
|-----|-----|-------------------|
| 0   |    | box (default)     |
| 1   | ○   | circle            |
| 2   | ◊   | diamond           |
| 3   | △   | nabla             |
| 4   | ▽   | delta             |
| 5   |    | crosshair         |
| 6   |    | rotated crosshair |

-   ColourIdx is an integer between 0 and 5 defining the colour of the
    labels:

|     |     |                |
|-----|-----|----------------|
| 0   |     | yellow         |
| 1   |     | cyan (default) |
| 2   |     | red            |
| 3   |     | purple         |
| 4   |     | green          |
| 5   |     | blue           |

-   Size scales the marker size. Default: 1.0

-   DistanceFactor resets the distance up to which camera distance the
    markers are displayed. Default: 1.0

-   Frame (used for celestial markers only) defines the reference to
    which the coordinates in the list refer:

    -   ecliptic: coordinates are ecliptic longitude and latitude
    -   celestial: coordinates are right ascension and declination of
        J2000 equator and equinox

Each item in the header section is optional. If missing, the default
value is substituted. The header can also be omitted altogether, in
which case the BEGIN_DATA tag is also not required.

In the data section, each line defines a label. It consists of
equatorial position: longitude \[deg\], with eastern longitudes positive
and western longitudes negative, latitude \[deg\], with northern
latitudes positive and southern latitudes negative, and one or two label
strings to be displayed above and below the marker.

## <span id="anchor-15"></span>Vessels

All vessel configuration files are by default located in
.\\Config\\Vessels. Simple vessel types can be defined by their
configuration files alone. More complex functions and capabilities
require either a compiled vessel module or a script interface.

Below is a description of the default vessel configuration options
recognised by Orbiter. Not all options are required, in particular if
the vessel is controlled by a module or script (in which case only the
Module or Script tags are mandatory). Also, custom vessel modules may
read additional non-standard options from the configuration file.

|                             |                 |                                                                                                                                                                                                                                                                                                                                                                                                                        |
|-----------------------------|-----------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Item                        | Type            | Description                                                                                                                                                                                                                                                                                                                                                                                                            |
| ClassName                   | String          | Vessel class name. Usually the same as the configuration file name.                                                                                                                                                                                                                                                                                                                                                    |
| BaseClass                   | String          | Optional parent class. Missing entries are taken from this class. Allows the construction of class hierarchies. Make sure not to introduce circular dependencies.                                                                                                                                                                                                                                                      |
| Module                      | String          | Optional; name of plugin module for vessel control. The module must be located in the .\\Modules subdirectory                                                                                                                                                                                                                                                                                                          |
| EditorModule                | String          | Optional: name of plugin module containing custom extensions for scenario editor plugin (see *ScenarioEditor.pdf*). Can be the same as Module                                                                                                                                                                                                                                                                          |
| Script                      | String          | For script-based vessels (Module = ScriptVessel) only. Defines the vessel script file (can be the same as the configuration file). See .\\Config\\Vessels\\ScriptPB.cfg for an example.                                                                                                                                                                                                                                |
| Help                        | String, String  | Optional: name of help file to be used for vessel class-specific help when the user presses the “Vessel” button in the Help dialog. The help file must be a compiled html file (.chm) and located in directory .\\Html\\Vessels. The entry contains the file name without path and extension and (separated by comma) the name of the first page to be displayed (without extension). Default: no vessel-specific help |
| EditorCreate                | Bool            | If FALSE, the vessel type does not appear in the list of the vessel creation page of the scenario editor. Default: TRUE                                                                                                                                                                                                                                                                                                |
| ImageBmp                    | String          | File name of a bitmap file (.bmp) displaying the vessel. The name should include the path (relative to the Orbiter main directory) and extension (.bmp). The image is shown in the vessel creation page of the scenario editor. For best results, it should be size 164×240 pixels.                                                                                                                                    |
| MeshName                    | String          | Name of the mesh file used for vessel visualisation (without .msh file extension, located in .\\Meshes subdirectory, or with optional path relative to .\\Meshes)                                                                                                                                                                                                                                                      |
| EnableFocus                 | Bool            | If TRUE, vessel can receive input focus. Default: TRUE                                                                                                                                                                                                                                                                                                                                                                 |
| EnableXPDR                  | Bool            | TRUE if vessel carries a transponder. Default: FALSE                                                                                                                                                                                                                                                                                                                                                                   |
| XPDR                        | Int             | Transponder channel (≥ 0) in units of 0.05 MHz from 108.0 MHz). Only used if EnableXPDR = TRUE.                                                                                                                                                                                                                                                                                                                        |
| Mass                        | Float           | Vessel mass (empty) \[kg\]                                                                                                                                                                                                                                                                                                                                                                                             |
| Size                        | Float           | Mean vessel radius \[m\]                                                                                                                                                                                                                                                                                                                                                                                               |
| ClipRadius                  | Float           | Vessel clip radius \[m\]. Radius of a sphere enclosing the vessel mesh to avoid visual clipping. Set this to \> Size if visual artefacts (clipping) occur when the camera moves close to the vessel. Default: same as Size.                                                                                                                                                                                            |
| MaxMainThrust               | Float           | Max. thrust force of main thruster(s) in vacuum \[N\]                                                                                                                                                                                                                                                                                                                                                                  |
| MaxRetroThrust              | Float           | Max. thrust force of retro thuster(s) in vacuum \[N\]                                                                                                                                                                                                                                                                                                                                                                  |
| MaxHoverThrust              | Float           | Max. thrust force of hover thruster(s) in vacuum \[N\]                                                                                                                                                                                                                                                                                                                                                                 |
| MaxAttitudeThrust           | Float           | If provided, this creates a complete set of thrusters for a reaction control system (RCS) to control the vessel’s attitude. The value defines the vacuum thrust \[N\] of each individual RCS thruster. All thrusters are assumed to have an offset of 1 m from the vessel’s centre of mass, so the thrust value must be scaled accordingly to provide a given target torque.                                           |
| TouchdownPoints             | Vec3 × 3        | 3 surface contact points in local vessel coordinates. For aircraft-like configurations these are: nose wheel, left main wheel, right main wheel (the order is important to define the ‘up’ direction). Other spacecraft types may interpret the points differently.                                                                                                                                                    |
| CameraOffset                | Vec3            | Camera position inside the vessel for cockpit view                                                                                                                                                                                                                                                                                                                                                                     |
| CW                          | Float × 4       | Airflow resistance coefficients: forward, backward, transversal, vertical. Only used by legacy flight model (if no airfoils are defined in the module).                                                                                                                                                                                                                                                                |
| WingAspect                  | Float           | The wing aspect ratio (wingspan<sup>2</sup> / wing area). Used for atmospheric drag calculation in legacy flight model                                                                                                                                                                                                                                                                                                 |
| WingEffectiveness           | Float           | A wing form factor: \~3.1 for elliptic wings, \~2.8 for tapered wings, \~2.5 for rectangular wings. Only used for legacy flight model.                                                                                                                                                                                                                                                                                 |
| CrossSections               | Vec3            | Cross sections in axis directions (z = longitudinal) \[m<sup>2</sup>\]                                                                                                                                                                                                                                                                                                                                                 |
| RotResistance               | Vec3            | Resistance against rotation around axes in atmosphere, where angular deceleration due to atmospheric friction is                                                                                                                                                                                                                                                                                                       
                                                 $${\\overset{˙}{\\omega}}\_{x,y,z}{\\mspace{9mu} = {\\mspace{9mu} - \\omega\_{x,y,z}}}\\rho r\_{x,y,z}$$                                                                                                                                                                                                                                                                                                                
                                                 with angular velocity *ω* and atmospheric density *ρ*.                                                                                                                                                                                                                                                                                                                                                                  |
| Inertia                     | Vec3            | Principal moments of inertia, mass normalised (see below) \[m<sup>2</sup>\]                                                                                                                                                                                                                                                                                                                                            |
| GravityGradient­Damping     | Float           | Damping coefficient for gravity gradient torque. Determines relaxation time for tidal locking. Default: 0 (undamped)                                                                                                                                                                                                                                                                                                   |
| PropellantResource\<*i*\>   | Float \[Float\] | Specs for propellant resource \<*i*\> (*i* ≥ 1). First value: max fuel capacity \[kg\]. Second value: fuel efficiency factor (> 0, default: 1)                                                                                                                                                                                                                                                                         |
| MaxFuel                     | Float           | Max. fuel mass \[kg\]. Obsolete; only used if no propellant resources are defined.                                                                                                                                                                                                                                                                                                                                     |
| Isp                         | Float           | Default value for fuel-specific impulse \[m/s\]: amount of thrust \[N\] obtained by burning 1 kg of fuel per second. Vessel modules can override this value for individual engines.                                                                                                                                                                                                                                    |
| MEngineRef\<*i*\>           | Vec3            | Reference position for main thruster \<*i*\> (\<*i*\> ≥ 1)                                                                                                                                                                                                                                                                                                                                                             |
| REngineRef\<*i*\>           | Vec3            | Reference position for hover thruster \<*i*\> (\<*i*\> ≥ 1)                                                                                                                                                                                                                                                                                                                                                            |
| HEngineRef\<*i*\>           | Vec3            | Reference position for hover thruster \<*i*\> (\<*i*\> ≥ 1)                                                                                                                                                                                                                                                                                                                                                            |
| AttRef\<*d*\>\<*i*\>\<*j*\> | Vec3            | Reference position for attitude thruster (for rotation around axis \<*d*\> (\<*d*\> = x,y,z), rotation direction \<*i*\> (\<*i*\> = 1,2) and thruster index \<*j*\> (\<*j*\> = 1,2) for a total of 12 attitude thrusters                                                                                                                                                                                               |
| LongAttRef\<*i*\>\<*j*\>    | Vec3            | Reference position for attitude thrusters (for linear forward/backward translation), direction \<*i*\> (\<*i*\> = 1,2) and thruster index \<*j*\> (\<*j*\> = 1.2) for a total of 4 attitude thrusters                                                                                                                                                                                                                  |
| DockRef                     | Vec3            | Docking reference point for first docking port (obsolete)                                                                                                                                                                                                                                                                                                                                                              |
| DockDir                     | Vec3            | Docking approach direction for first docking port (obsolete)                                                                                                                                                                                                                                                                                                                                                           |
| DockRot                     | Vec3            | Longitudinal alignment direction (normal to DockDir) for first docking port (obsolete)                                                                                                                                                                                                                                                                                                                                 |
| \<*Docklist*\>              | List            | List of positions and approach directions for docking ports (see below)                                                                                                                                                                                                                                                                                                                                                |
| \<*Attachment list*\>       | List            | List of positions and approach directions for attachment points (see below)                                                                                                                                                                                                                                                                                                                                            |

Notes:

-   A vessel class can be derived from a different vessel class by
    defining the BaseClass entry. All properties not defined in the new
    class configuration file are taken from the base class.
-   The mesh name (MeshName value) does not include the file extension
    (.msh assumed) or directory path (Orbiter’s mesh directory assumed,
    by default .\\Meshes).
-   The MaxFuel entry has been replaced by PropellantResource, which
    allows the definition of multiple propellant resources (fuel tanks).
-   The DockRef, DockDir, DockRot entries have been replaced with the
    more versatile Docklist, which allows the configuration of multiple
    docking ports and IDS (instrument docking system) frequencies.

BEGIN_DOCKLIST

\<*Dock-spec 0*\>

\<*Dock-spec 1*\>

. . .

END_DOCKLIST

\<*x*<sub>*i*</sub>\> \<*y*<sub>*i*</sub>\> \<*z*<sub>*i*</sub>\>
\<*dx*<sub>*i*</sub>\> \<*dy*<sub>*i*</sub>\> \<*dz*<sub>*i*</sub>\>
\<*rx*<sub>*i*</sub>\> \<*ry*<sub>*i*</sub>\> \<*rz*<sub>*i*</sub>\>
\[\<*ids-channel*<sub>*i*</sub>\>\]

-   The attachment list is similar to the docklist: It allows to specify
    points at which vessels can be connected to each other. Unlike
    peer-to-peer docking ports, attachment points define parent-child
    hierarchies, and each attachment point is either a parent or a child
    port. For more details see the Vessel attachment management section
    in the API reference manual.

BEGIN_ATTACHMENT

\<*Attach-spec 0*\>

\<*Attach-spec 1*\>

. . .

END_ATTACHMENT

-   \<*type*<sub>*i*</sub>\> \<*x*<sub>*i*</sub>\> \<*y*<sub>*i*</sub>\>
    \<*z*<sub>*i*</sub>\> \<*dx*<sub>*i*</sub>\> \<*dy*<sub>*i*</sub>\>
    \<*dz*<sub>*i*</sub>\> \<*rx*<sub>*i*</sub>\> \<*ry*<sub>*i*</sub>\>
    \<*rz*<sub>*i*</sub>\> \<*id*<sub>*i*</sub>\>

    \<*type*<sub>*i*</sub>\> is a single character: ‘P’ (attach to
    parent) or ‘C’ (attach to child). The next 9 entries define the
    attachment position and direction in the same way as docking ports.
    \<*id*<sub>*i*</sub>\> is a string of up to 8 characters used for
    defining compatibility between attachment points.

-   The 3 values of the Inertia tag are the *principal moments of
    inertia*, defined as the diagonal elements (*J*<sub>*xx*</sub>,
    *J*<sub>*yy*</sub>, *J*<sub>*zz*</sub>) of the mass-normalised
    inertia tensor **J** in the vessel’s principal frame, where **J** is
    diagonal. **J** in an arbitrary frame is given by

    $$\\mathbf{\\mathrm{J}}{\\mspace{9mu} = \\mspace{9mu}}\\frac{1}{M}{\\int\\limits\_{V}{m{(r)}\\begin{bmatrix}
    {y^{2} + z^{2}} & \\mathit{xy} & \\mathit{xz} \\\\
    \\mathit{yx} & {x^{2} + z^{2}} & \\mathit{yz} \\\\
    \\mathit{xz} & \\mathit{xy} & {x^{2} + y^{2}} \\\\
    \\end{bmatrix}\\mathit{dr}}}$$

    The Orbitersdk\\Utils folder contains a simple tool (shipedit.exe)
    to calculate the inertia tensor of an object from its mesh file. The
    tool requires “well-behaved” mesh files (composed of closed compact
    surfaces) and assumes a homogeneous density distribution inside the
    mesh (*m*(*r*) = const). The latter is not very realistic, so the
    results must be interpreted realistically. They should still serve
    as a good starting point for experimentation.

# <span id="anchor-7"></span><span id="anchor-16"></span>Scenario files

Scenarios are simulation startup definitions which contain all
parameters required to set up the starting conditions of the simulation
at a particular time. They are used for loading and saving simulation
states. A scenario file is generated during the simulation when the user
saves a state, or automatically at the simulation termination so it can
be picked up later.

Scenario files are text files that can be created or modified manually.
An easier way to edit scenarios is with the *Scenario Editor* plugin,
which can be used to modify and save a running simulation. The Scenario
Editor is described in a separate document (ScenarioEditor.pdf).

Scenario files are located in a subdirectory tree specified by the
ScenarioDir entry in the main configuration file (see Chapter
[2.1](#anchor-3)), usually .\\Scenarios. They have file extension .scn.

### Format

Scenario files consist of a series of category blocks describing
different aspects of the simulation state:

\<*Description block*\>

\<*Environment block*\>

\<*Focus block*\>

\<*Camera block*\>

\<*Panel block*\>

\<*VC block*\>

\<*HUD block*\>

\<*Left MFD block*\>

\<*Right MFD block*\>

\<*Vessel list*\>

### Description block (optional):

Contains a short description of the scenario which is presented to the
user in Orbiter’s Launchpad dialog window on highlighting the scenario.
There are two versions of the description block available:

BEGIN_DESC

\<*Text-description*\>

END_DESC

where \<*Text-description*\> contains a scenario description in plain
text format. The description can span multiple lines. An empty line is
converted into a new line on display. More powerful formatting options
are available with the alternative

BEGIN_HYPERDESC

\<*HTML-description*\>

END_HYPERDESC

where \<*HTML-description*\> is passed to an HTML parser for rendering
in the Launchpad window and can contain HTML formatting tags such as
\<h1>\</h1>, \<b>\</b>, \<br>, etc. However, references to external
objects, such as images or links, are not allowed.

Finally, the description can also be served from an external HTML or CHM
(compiled multi-page HTML) file, using the format

BEGIN_URLDESC

\<*URL*\>

END_URLDESC

where \<*URL*\> is a reference to a local HTML or CHM file containing
the scenario description. The file is expected to be located in the
HTML\\Scenarios directory. It can also be in a subdirectory of that
directory, but the path relative to HTML\\Scenarios must then be added
to the \<*URL*\> reference. The file extension must not be included in
the URL specification. For HTML files, file extension htm is expected.
For CHM files, extension chm is expected. CHM references also must
specify the page to display, separated by comma. For example,

BEGIN_URLDESC

MyScenarios\\MyScenarioDescription,TitlePage

END_URLDESC

parses the description from file
.\\Html\\Scenarios\\MyScenarios\\MyScenarioDescription.chm, reading page
TitlePage.htm from the CHM file. CHM files can contain multiple
documents, such as additional pages or images. The scenario description
URL can be the same as the Help URL used for inline scenario help (see
Help tag in the environment block).

Note that when running Orbiter in some environments (e.g. in Linux under
WINE), the HTML render function may not be available. For this case you
can provide a DESC block in addition to the URLDESC or HYPERDESC block,
to use as a fallback option. If no DESC block is found, Orbiter will try
to convert a HYPERDESC block to plain text, but scenarios with only a
URLDESC block will just show an empty description box.

### Environment block (optional):

Contains the simulation environment. Format:

BEGIN_ENVIRONMENT

\<*Environment parameters*\>

END_ENVIRONMENT

The following environment parameters are supported:

<table>
<tbody>
<tr class="odd">
<td>Item</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>SYSTEM</td>
<td>String</td>
<td>Name of the planetary system. A configuration file for this system must exist (see Chapter <a href="#anchor-5">2.2</a>). Default: “Sol”</td>
</tr>
<tr class="odd">
<td>DATE</td>
<td></td>
<td><p>Contains the simulation start time. Allowed formats are:</p>
<p>MJD &lt;mjd&gt; (Modified Julian date [days])</p>
<p>JD &lt;jd&gt; (Julian date)</p>
<p>JE &lt;je&gt; (Julian epoch)</p>
<p>Default is current system time, but this should be avoided if the scenario contains objects defined by position/velocity state vectors at a fixed time, which cannot easily be propagated</p></td>
</tr>
<tr class="even">
<td>HELP</td>
<td>String [,String]</td>
<td>Scenario help file (in HTML or compressed CHM format). For a HTML page, specify the location of the page relative to the Html\Scenarios folder and without extension (.htm is assumed). For a page in a CHM file, specify the location of the file relative to the Html\Scenarios folder without extension (.chm is assumed), followed by a comma and the name of the page inside the file without extension (.htm is assumed). Scenario help files can be opened during the simulation with Alt-F1. Default: no help file</td>
</tr>
<tr class="odd">
<td>CONTEXT</td>
<td>String</td>
<td>Optional context string. This can be used to fine-tune the setup of a planetary system, e.g. by selective loading of surface bases (see Chapter <a href="#anchor-11">2.4</a>).</td>
</tr>
<tr class="even">
<td>SCRIPT</td>
<td>String</td>
<td>A script file to launch at the scenario start. The string should include any path relative to the “Script” subdirectory, but no file extension (.lua is assumed). Default: no script</td>
</tr>
</tbody>
</table>

Notes:

-   If the DATE entry is not present, Orbiter reads the computer system
    clock, adds the time zone offset to convert to Universal Time (UTC)
    and adds another offset of 66.184 seconds to map from UTC to
    Barycentric Dynamical Time (TDB).
-   If the HELP entry is present, the description block is ignored.
-   The script launched with the SCRIPT entry can be used for demos,
    interactive tutorials, etc. For Lua scripting in Orbiter, see
    .\\Orbitersdk\\doc\\orbiter_lua.chm.

### Focus block:

Contains parameters for the user-controlled spacecraft. Format:

BEGIN_FOCUS

\<*Focus parameters*\>

END_FOCUS

with the following parameters supported:

|      |        |                                                                                         |
|------|--------|-----------------------------------------------------------------------------------------|
| Item | Type   | Description                                                                             |
| SHIP | String | Name of the user-controlled ship. The ship must be listed in the ship list (see below). |

### Camera block (optional):

Camera mode and parameters. If the camera block is missing, the camera
is set to cockpit view in the current focus object. Format:

BEGIN_CAMERA

\<*Camera parameters*\>

END_CAMERA

with supported parameters

|                 |                  |                                                                                                                                            |
|-----------------|------------------|--------------------------------------------------------------------------------------------------------------------------------------------|
| Item            | Type             | Description                                                                                                                                |
| MODE            | Flag             | Extern \| Cockpit (selects external or cockpit camera mode)                                                                                |
| TARGET          | String           | Camera view target object (external modes only, cockpit mode always refers to the current focus object)                                    |
| POS             | Vec3             | Camera position relative to target (external modes only)                                                                                   |
| TRACKMODE       | Flag \[+String\] | TargetRelative \| AbsoluteDirection \| GlobalFrame \| TargetTo \<*ref*\> \| TargetFrom \<*ref*\> \| Ground \<*ref*\> (external modes only) |
| GROUNDLOCATION  | Vec3             | longitude \[deg\], latitude \[deg\] and altitude \[m\] of ground observer (*Ground* trackmode only)                                        |
| GROUNDDIRECTION | Float Float      | polar coordinates of ground observer orientation (*free Ground* trackmode only)                                                            |
| FOV             | Float            | Field of view (angular aperture from from top to bottom of view window) \[deg\]                                                            |

### Panel block (optional):

2D instrument panel parameters. If neither this nor the VC block is
present, Orbiter initially displays the generic glass cockpit view in
internal (cockpit) modes. If both are present, the 2D panel view takes
precedence. Format:

BEGIN_PANEL

\<*Panel parameters*\>

END_PANEL

Currently no panel parameters are supported, but the presence of the
panel block indicates a preference for 2D panel modes on startup, if
supported by the focus vessel.

### VC block (optional):

Virtual cockpit parameters. If neither this nor the panel block is
present, Orbiter initially displays the generic glass cockpit view in
internal (cockpit) modes. Format:

BEGIN_VC

\<*VC parameters*\>

END_VC

Currently VC parameters are supported, but the presence of the VC block
indicates a preference for virtual cockpit modes on startup, if
supported by the focus vessel.

### HUD block (optional):

HUD (head up display) mode and parameters for cockpit views. If the HUD
block is missing, no HUD is displayed at simulation start. Format:

BEGIN_HUD

\<*HUD parameters*\>

END_HUD

with supported parameters

|      |      |                             |
|------|------|-----------------------------|
| Item | Type | Description                 |
| TYPE | Flag | Orbit \| Surface \| Docking |

### Left/right MFD blocks (optional):

Left and right MFD (multifunctional display) mode and parameters. If an
MFD block is missing, the corresponding MFD is switched off. Note that
custom MFD modes can define their own set of parameters. If the current
cockpit mode supports more than two MFD instruments, only the parameters
for the first to are preserved. Format:

BEGIN_MFD \[Left \| Right\]

\<*MFD parameters*\>

END_MFD

The following parameters are supported (most of which are relevant only
for specific MFD modes):

|           |        |                                                                                                                           |
|-----------|--------|---------------------------------------------------------------------------------------------------------------------------|
| Item      | Type   | Description                                                                                                               |
| TYPE      | Flag   | MFD mode: Orbit \| Surface \| Map \| Launch \| Docking \| OAlign \| OSync \| Transfer                                     |
| REF       | String | Reference object (Orbit and Map modes only)                                                                               |
| TARGET    | String | Target object (Orbit, OAlign, OSync modes only)                                                                           |
| BTARGET   | String | Base target (Map mode only)                                                                                               |
| OTARGET   | String | Orbit target (Map mode only)                                                                                              |
| PROJ      | Flag   | Ecliptic \| Ship \| Target (Orbit mode only)                                                                              |
| MODE      | Flag   | Intersect 1 \| Intersect 2 \| Sh periapsis \| Sh apoapsis \| Tg periapsis \| Tg apoapsis \| Manual axis (OSync mode only) |
| MANUALREF | Float  | Reference axis position \[deg\] (OSync mode in manual axis setting only)                                                  |
| LISTLEN   | Int    | Length or orbit list (OSync mode only)                                                                                    |

### Ship list:

A list of all spacecraft in the simulation and their state parameters.
The list must contain at least one vessel referenced by the Focus block.
Format:

BEGIN_SHIPS

\<*Ship 0*\>

\<*Ship 1*\>

...

END_SHIPS

where the entry format for each ship is given by

\<*Vessel name*\>\[: \<*Class name*\>\]

\<*Vessel parameters*\>

END

If both the vessel and class names are present, the vessel is created as
an instance of the vessel class, and a configuration file for the vessel
class, \<*Class name*\>.cfg must exist. If only the vessel name is
present, it is assumed to be a soliton (the only representative of its
class) and the vessel class is assumed to coincide with its name.

The following generic parameters are supported:

<table>
<tbody>
<tr class="odd">
<td>Item</td>
<td>Type</td>
<td>Description</td>
</tr>
<tr class="even">
<td>STATUS</td>
<td>Flag</td>
<td>Landed &lt;<em>planet</em>&gt; | Orbiting &lt;<em>planet</em>&gt;</td>
</tr>
<tr class="odd">
<td>BASE</td>
<td></td>
<td>&lt;<em>base</em>&gt;:&lt;<em>lpad</em>&gt; (only for status <em>Landed</em>)</td>
</tr>
<tr class="even">
<td>HEADING</td>
<td>Float</td>
<td>Orientation (only for status <em>Landed</em>)</td>
</tr>
<tr class="odd">
<td>RPOS</td>
<td>Vec3</td>
<td>Position relative to reference body (only for status <em>Orbiting</em>)</td>
</tr>
<tr class="even">
<td>RVEL</td>
<td>Vec3</td>
<td>Velocity relative to reference body (only for status <em>Orbiting</em>)</td>
</tr>
<tr class="odd">
<td>ELEMENTS</td>
<td>List</td>
<td><p>Osculating orbital elements. This is an alternative to the state vector specification (RPOS and RVEL) for vessels with status <em>Orbiting</em>. The list contains 7 entries:</p>
<p>semi-major axis <em>a</em> [m],</p>
<p>eccentricity <em>e</em>,</p>
<p>inclination <em>i</em> [deg],</p>
<p>longitude of ascending node <em>Ω</em> [deg],</p>
<p>longitude of periapsis <em>ϖ</em> [deg],</p>
<p>mean longitude at epoch <em>l</em> [deg]</p>
<p>epoch <em>t</em> [MJD]</p></td>
</tr>
<tr class="even">
<td>AROT</td>
<td>Vec3</td>
<td>Orientation: rotation angles of object frame (only for status <em>Orbiting</em>)</td>
</tr>
<tr class="odd">
<td>VROT</td>
<td>Vec3</td>
<td>Angular velocity [deg/s] (only for status <em>Orbiting</em>)</td>
</tr>
<tr class="even">
<td>FUEL</td>
<td>Float</td>
<td>Fuel level (0–1). This tag sets the level of all propellant resources to the same level. For individual settings use PRPLEVEL instead.</td>
</tr>
<tr class="odd">
<td>PRPLEVEL</td>
<td>List</td>
<td>List of propellant resource levels. Each entry is of the form &lt;<em>id</em>&gt;:&lt;<em>level</em>&gt;, where &lt;<em>id</em>&gt; is the resource identifier (Int ≥ 0) and &lt;<em>level</em>&gt; is the propellant resource level (Float, 0–1)</td>
</tr>
<tr class="even">
<td>THLEVEL</td>
<td>List</td>
<td>List of thruster settings. Each entry is of the form &lt;<em>id</em>&gt;:&lt;<em>level</em>&gt;, where &lt;<em>id</em>&gt; is the thruster identifier (Int ≥ 0) in the order of thruster definition and &lt;<em>level</em>&gt; is the thrust level (Float, 0–1). Thrusters with level 0 can be omitted.</td>
</tr>
<tr class="odd">
<td>DOCKINFO</td>
<td>List</td>
<td>Docking status list. This contains information about all docked vessels. Each entry is of the form &lt;<em>id</em>&gt;:&lt;<em>rid</em>&gt; &lt;<em>rvessel</em>&gt;, where &lt;<em>id</em>&gt; is the docking port identifier (Int ≥ 0), &lt;<em>rid</em>&gt; is the docking port identifier of the docked vessel, and &lt;<em>rvessel</em>&gt; is the docked vessel. Only engaged docking ports are listed. See notes below.</td>
</tr>
</tbody>
</table>

In addition to these generic parameters, individual vessel classes may
define class-specific parameters.

Defining docked assemblies:

There are two ways to define vessels as being assembled into a
superstructure by docking them together:

-   Place the vessels so that their docking point locations coincide (by
    using the appropriate RPOS, RVEL, AROT and VROT parameters for
    both). Orbiter will dock two vessels automatically if their docking
    ports are close enough.
-   Define the DOCKINFO lists for both vessels so that they reference
    each other. Orbiter then attaches the vessels accordingly even if
    their position parameters are not aligned. In that case, the
    position of the first vessel in the superstructure list defines the
    position of the assembly. The position parameters of all subsequent
    vessels in the list are ignored.

In general it is easier to set up vessel states within the simulation by
using the ScenarioEditor plugin and writing out the resulting scenario
file than manually editing the file.

# <span id="anchor-17"></span>Keyboard function mapping

The keymap.cfg file in the Orbiter root directory contains the key
mapping definitions for the default Orbiter functions. Individual vessel
classes and other plugins may define additional key functions.

keymap.cfg is a text file and can be edited with any editor capable of
writing plain ASCII text files. Each line contains a key definition in
the format

\<*function*\> = \<*key*\>

where \<*function*\> is one of the function identifiers listed in the
table below, and \<*key*\> is a key identifier, optionally followed by
one or more modifier keys.

For supported key identifiers, see Chapter *Keyboard key identifiers* in
document Orbitersk\\doc\\API_Reference.chm. The key identifiers in the
format required by keymap.cfg are as defined in the list, minus the
OAPI_KEY\_ prefix.

Supported modifier keys are LSHIFT, RSHIFT, LCTRL, RCTRL, CTRL, LALT,
RALT, ALT.

Missing entries in keymap.cfg are substituted by their default values.
To revert to the original key mapping, simply delete keymap.cfg. Orbiter
will create a new one the next time it is run.

|                         |                |                                              |
|-------------------------|----------------|----------------------------------------------|
| Function                | Default value  | Description                                  |
| CockpitCamRotateLeft    | LEFT ALT       | Rotate camera left in cockpit mode           |
| CockpitCamRotateRight   | RIGHT ALT      | Rotate camera right in cockpit mode          |
| CockpitCamRotateUp      | UP ALT         | Rotate camera up in cockpit mode             |
| CockpitCamRotateDown    | DOWN ALT       | Rotate camera down in cockpit mode           |
| CockpitCamDontLean      | DOWN CTRL ALT  | Return camera to default position in VC mode |
| CockpitCamLeanForward   | UP CTRL ALT    | Lean forward in VC mode                      |
| CockpitCamLeanLeft      | LEFT CTRL ALT  | Lean left in VC mode                         |
| CockpitCamLeanRight     | RIGHT CTRL ALT | Lean right in VC mode                        |
| CockpitResetCam         | HOME           | Return to default cockpit camera direction   |
| PanelShiftLeft          | LEFT           | Scroll 2D panel left                         |
| PanelShiftRight         | RIGHT          | Scroll 2D panel right                        |
| PanelShiftUp            | UP             | Scroll 2D panel up                           |
| PanelShiftDown          | DOWN           | Scroll 2D panel down                         |
| PanelSwitchLeft         | LEFT CTRL      | Switch to left neighbour panel               |
| PanelSwitchRight        | RIGHT CTRL     | Switch to right neighbour panel              |
| PanelSwitchUp           | UP CTRL        | Switch to upper neighbour panel              |
| PanelSwitchDown         | DOWN CTRL      | Switch to lower neighbour panel              |
| TrackCamRotateLeft      | LEFT CTRL      | Rotate camera left in external track mode    |
| TrackCamRotateRight     | RIGHT CTRL     | Rotate camera right in external track mode   |
| TrackCamRotateUp        | UP CTRL        | Rotate camera up in external track mode      |
| TrackCamRotateDown      | DOWN CTRL      | Rotate camera down in external track mode    |
| TrackCamAdvance         | PGDOWN         | Move camera closer in external track mode    |
| TrackCamRetreat         | PGUP           | Move camera away in external track mode      |
| GroundCamTiltLeft       | LEFT           | Tilt camera left in ground observer mode     |
| GroundCamTiltRight      | RIGHT          | Tilt camera right in ground observer mode    |
| GroundCamTiltUp         | UP             | Tilt camera up in ground observer mode       |
| GroundCamTiltDown       | DOWN           | Tilt camera down in ground observer mode     |
| IncMainThrust           | ADD CTRL       | Increment main thrust level                  |
| DecMainThrust           | SUBTRACT CTRL  | Decrement main thrust level                  |
| KillMainRetroThrust     | MULTIPLY       | Kill main and retro thrusters                |
| OverrideFullMainThrust  | ADD            | Temporarily set full main thrust             |
| OverrideFullRetroThrust | SUBTRACT       | Temporarily set full retro thrust            |
| IncHoverThrust          | NUMPAD0        | Increment hover thrust level                 |
| DecHoverThrust          | DECIMAL        | Decrement hover thrust level                 |
| RCSEnable               | DIV CTRL       | Enable/disable RCS                           |
| RCSMode                 | DIVIDE         | Rotational/translational RCS mode            |
| RCSPitchUp              | NUMPAD2        | RCS pitch up                                 |
| RCSPitchDown            | NUMPAD8        | RCS pitch down                               |
| RCSYawLeft              | NUMPAD1        | RCS yaw left                                 |
| RCSYawRight             | NUMPAD3        | RCS yaw right                                |
| RCSBankLeft             | NUMPAD4        | RCS bank left                                |
| RCSBankRight            | NUMPAD6        | RCS bank right                               |
| RCSUp                   | NUMPAD2        | RCS translate up                             |
| RCSDown                 | NUMPAD8        | RCS translate down                           |
| RCSLeft                 | NUMPAD1        | RCS translate left                           |
| RCSRight                | NUMPAD3        | RCS translate right                          |
| RCSForward              | NUMPAD6        | RCS translate forward                        |
| RCSBack                 | NUMPAD9        | RCS translate backward                       |
| LPRCSPitchUp            | NUMPAD2 CTRL   | low power RCS pitch up                       |
| LPRCSPitchDown          | NUMPAD8 CTRL   | low power RCS pitch down                     |
| LPRCSYawLeft            | NUMPAD1 CTRL   | low power RCS yaw left                       |
| LPRCSYawRight           | NUMPAD3 CTRL   | low power RCS yaw right                      |
| LPRCSBankLeft           | NUMPAD4 CTRL   | low power RCS bank left                      |
| LPRCSBankRight          | NUMPAD6 CTRL   | low power RCS bank right                     |
| LPRCSUp                 | NUMPAD2 CTRL   | low power RCS translate up                   |
| LPRCSDown               | NUMPAD8 CTRL   | low power RCS translate down                 |
| LPRCSLeft               | NUMPAD1 CTRL   | low power RCS translate left                 |
| LPRCSRight              | NUMPAD3 CTRL   | low power RCS translate right                |
| LPRCSForward            | NUMPAD6 CTRL   | low power RCS translate forward              |
| LPRCSBack               | NUMPAD9 CTRL   | low power RCS translate backward             |
| NMHoldAltitude          | A              | navmode hold altitude                        |
| NMHLevel                | L              | navmode wings level                          |
| NMPrograde              | LBRACKET       | navmode prograde                             |
| NMRetrograde            | RBRACKET       | navmode retrograde                           |
| NMNormal                | SEMICOLON      | navmode orbit-normal                         |
| NMAntinormal            | APOSTROPHE     | navmode orbit-antinormal                     |
| NMKillrot               | NUMPAD5        | navmode kill rotation                        |
| Undock                  | D CTRL         | disengage main docking port                  |
| IncElevatorTrim         | DELETE         | Increment elevator trim setting              |
| DecElevatorTrim         | INSERT         | Decrement elevator trim setting              |
| WheelbrakeLeft          | COMMA          | Apply wheel brake at left main gear          |
| WheelbrakeRight         | PERIOD         | Apply wheel brake at right main gear         |
| HUD                     | H CTRL         | Switch HUD on/off                            |
| HUDMode                 | H              | Cycle to next HUD mode                       |
| HUDReference            | R CTRL         | Select HUD reference body                    |
| HUDTarget               | R CTRL ALT     | Select HUD target object                     |
| HUDColour               | H ALT          | Cycle to next HUD colour                     |
| IncSimSpeed             | T              | Increment simulation speed ×10               |
| DecSimSpeed             | R              | Decrement simulation speed /10               |
| IncFOV                  | X              | Zoom camera out                              |
| DecFOV                  | Z              | Zoom camera in                               |
| StepIncFOV              | X CTRL         | Zoom camera out to next 10° step             |
| StepDecFOV              | Z CTRL         | Zoom camera in to next 10° step              |
| MainMenu                | F4             | Show/hide main menu                          |
| DlgHelp                 | F1 ALT         | Open help window                             |
| DlgCamera               | F1 CTRL        | Open camera dialog                           |
| DlgSimSpeed             | F2 CTRL        | Open time acceleration dialog                |
| DlgCustomCmd            | F4 CTRL        | Open custom function dialog                  |
| DlgVisualHelpers        | F9 CTRL        | Open visual helpers dialog                   |
| DlgRecorder             | F5 CTRL        | Open record/playback control dialog          |
| DlgInfo                 | I CTRL         | Open object info window                      |
| DlgMap                  | M CTRL         | Open map window                              |
| DlgSelectVessel         | F3             | Open vessel selection dialog                 |
| SelectPrevVessel        | F3 CTRL        | Switch to previous focus vessel              |
| ToggleCamInternal       | F1             | Switch cockpit/external view                 |
| ToggleTrackMode         | F2             | Cycle to next external track mode            |
| TogglePanelMode         | F8             | Cycle to next cockpit mode                   |
| TogglePlanetarium       | F9             | Switch planetarium mode on/off               |
| ToggleRecPlay           | C CTRL         | Switch recorder/playback of/off              |
| Pause                   | P CTRL         | Pause/resume simulation                      |
| Quicksave               | S CTRL         | Save current simulation state                |
| DlgCapture              | SYSRQ CTRL     | Open screen capture control dialog           |
| Quit                    | Q CTRL         | Quit simulation session                      |

# <span id="anchor-18"></span><span id="anchor-13"></span>Mesh files

Orbiter uses a custom mesh file format. Mesh files are ASCII text files,
and are by default located in the .\\Meshes subdirectory, unless the
MeshDir entry in the main configuration file points to a different
location (see Chapter [2.1](#anchor-3)). Some objects (e.g. Vessel
classes) may place their meshes into their own subdirectory below
.\\Meshes to keep them grouped together.

Vertex coordinates are defined in the object’s local coordinate system.
Remember that Orbiter uses a left-handed coordinate system, where +x =
right axis, +y = up axis, +z = forward axis. To conform with Orbiter’s
default vessel orientation conventions, vessel meshes should be aligned
so that the main thrust direction points to the positive z-direction,
the positive x-axis points right, and the positive y-axis points up.

Vertex coordinate values are in units of meters \[m\].

The mesh file format is given by

MSHX1 header

GROUPS \<*n*\> \<*n*\>: number of mesh groups

\<*group 1*\> group spec 1

\<*group 2*\> group spec 2

. . .

\<*group n*\> group spec *n*

MATERIALS \<*m*\> \<*m*\>: number of material definitions

\<*mtrl-name 1*\> material name 1

\<*mtrl-name 2*\> material name 2

. . .

\<*mtrl-name m*\> material name *m*

\<*material 1*\> material definition 1

\<*material 2*\> material definition 2

. . .

\<*material m*\> material definition *m*

TEXTURES \<*t*\> \<*t*\>: number of texture definitions

\<*tex-name 1*\> texture name 1

\<*tex-name 2*\> texture name 2

\<*tex-name t*\> texture name *t*

The rest of this chapter contains details about the components of the
mesh file format.

## <span id="anchor-19"></span>Mesh groups

Meshes are subdivided into mesh groups. Each mesh group has one material
and one texture map associated with it. If you want different parts of
the objects to have different material properties, the mesh must be
split into groups accordingly. Mesh groups act as a rigid sub-units
within the mesh for the purposes of mesh animation.

### Group specs

A mesh group specification inside the mesh file has the following
format:

\[LABEL \<*label*\>\] group label, optional

\[MATERIAL \<*i*\>\] material index, optional

\[TEXTURE \<*j*\>\] texture index, optional

\[TEXWRAP \<*wrap*\>\] texture wrap mode: \<*wrap*\> = U or V or UV,
optional

\[NONORMAL\] “no normals” flag, see below, optional

\[FLAG \<*f*\>\] multi-purpose bit flags, see below, optional

GEOM \<*nv*\> \<*nt*\> \<*nv*\>: vertex count, \<*nt*\> triangle count

\<*vtx 0*\> vertex spec 0

\<*vtx 1*\> vertex spec 1

. . .

\<*vtx nv−1*\> vertex spec *nv*−1

\<*tri 0*\> triangle spec 0

\<*tri 1*\> triangle spec 1

\<*tri nt−1*\> triangle spec *nt*−1

Group components:

-   A group can be assigned an optional label (tag LABEL) to make it
    easier to address from within the vessel module code (e.g. for
    coding an animation sequence). The label must be a single word
    without white space. A separate tool
    (.\\Orbitersdk\\Utils\\meshc.exe) can be used to scan a mesh file
    and write out a C++ header file containing the group labels and
    their integer indices. This header file can then be included in the
    vessel module code. Make sure to re-run meshc whenever the mesh
    group structure changes, to keep the associations up to date.
-   A material index can be used to assign a material to the group.
    Indices ≥ 1 assign a material from the mesh material list. Index 0
    refers to the default material (white, diffuse and opaque). If the
    group doesn’t specify a material index, it inherits the previous
    group’s material. If the first mesh group does not specify a
    material index, index 0 is implied.
-   A texture index can be used to assign a texture map to the group.
    Indices ≥ 1 assign a texture from the mesh texture list. Index 0
    indicates “no texture”. If the group doesn’t specify a texture
    index, it inherits the previous group’s texture. If the first mesh
    group does not specify a texture index, index 0 is implied.
-   The optional TEXWRAP flag defines how textures wrap around the
    object. “U” causes textures to wrap in the u-coordinate direction in
    texel space, “V” wraps in v-coordinate direction, and “UV” wraps in
    both directions. Default is no wrapping.
-   The optional NONORMAL flag indicates that vertex definitions in this
    group don’t contain normal information, meaning that the first two
    numbers after the vertex coordinate (x,y,z) triplet are interpreted
    as texture coordinate (u,v) pair.
-   The optional FLAG tag allows to specify a user-defined 32-bit flag
    whose interpretation is context-dependent. Below is a list of bit
    settings currently recognised by Orbiter:

|           |            |                                                      |
|-----------|------------|------------------------------------------------------|
| Mesh type | Flag       | Interpretation                                       |
| Vessel    | 0x00000001 | Do not use this group to render ground shadows       |
| Vessel    | 0x00000002 | Do not render this group                             |
| Vessel    | 0x00000004 | Do not apply lighting when rendering this group      |
| Vessel    | 0x00000008 | Texture blending directive: additive with background |

-   The GEOM specification heads the vertex and triangle list for the
    mesh group. It specifies the lengths of both lists.
-   A vertex list (see below)
-   A triangle list (see below)

### Vertex list

Each group contains a vertex list, defining the positions, and
optionally normal directions and texture coordinates of the vertices in
the group.

Each line in the list defines a vertex and contains up to 8 floating
point numbers (separated by spaces:

\<*x*\> \<*y*\> \<*z*\> \[\<*nx*\> \<*ny*\> \<*nz*\>\] \[\<*tu*\>
\<*tv*\>\]

-   The first 3 numbers contain the Cartesian vertex coordinates (*x*,
    *y*, *z*) in the object’s local frame of reference. Units are meters
    \[m\].
-   The next 3 numbers (if present) contain the surface normal direction
    at the vertex location (*nx*, *ny*, *nz*), unless the group has the
    NONORMAL flag set. Orbiter needs the normal direction for lighting
    calculations. If no normal specification is available (or if the
    NONORMAL flag is present), Orbiter guesses the normal direction as
    the average of the normals of the adjacent triangles. This works
    well for smooth surfaces, but should be avoided for surfaces that
    contain sharp edges. Vertices located on edges require two vertex
    entries with different normals, referenced by the appropriate
    triangles. Normal definitions should be normalised:
    sqrt(*nx*<sup>2</sup> + *ny*<sup>2</sup> + *nz*<sup>2</sup>) = 1.
-   The next 2 numbers (if present) contain the vertex texture
    coordinates (*u*, *v*). Texture coordinates are only required if the
    group uses a texture (has texture index ≥ 1). Texture coordinates
    define how a rectangular 2D texture is mapped onto the object
    surface. Texture coordinate (0, 0) refers to the lower left corner
    of the texture, (1, 1) refers to the upper right corner. Coordinates
    \> 1 and \< 0 are allowed and cause textures to repeat periodically.

### Triangle list

The group’s triangle list follows immediately below the vertex list. It
defines the triangles which compose the group’s polygonal (piecewise
flat) surface.

-   Each line in the list defines a triangle and consists of 3 integer
    numbers (*i*, *j*, *k*). Each is an index referencing a vertex in
    the vertex list (starting from 0).
-   Only the “clockwise” (CW) side of the triangle is rendered: the side
    which, when looked at, has the vertices arranged in a clockwise
    order. The opposite, “counterclockwise” (CCW) side is invisible.
-   If both sides of the triangle must be rendered (e.g. if representing
    part of a thin plate), two triangles must be defined, one for each
    side.
-   To flip the rendered side of a triangle (e.g. to correct for “inside
    out” artefacts), the vertex index order must be rearranged: (*i*,
    *j*, *k*) → (*i*, *k*, *j*).

## <span id="anchor-20"></span>Material list

Material specifications allow specification of general light reflection
and emission properties, as well as opacity values, that can be applied
to mesh group surfaces. The material list consists of

-   A header line, MATERIALS \<*m*\>, defining the number \<*m*\> of
    materials.
-   A list of material names
-   A list of material properties

Each material property specification consists of

-   A header line, MATERIAL \<*name*\>, where \<*name*\> is an entry
    from the list of material names
-   A line containing the *diffuse reflection* colour and opacity:
    \<*r*<sub>*d*</sub>\> \<*g*<sub>*d*</sub>\> \<*b*<sub>*d*</sub>\>
    \<*a*<sub>*d*</sub>\>. This is the colour that is diffusely (in all
    directions) reflected from an illuminated surface.
-   A line containing the *ambient* material colour and opacity:
    \<*r*<sub>*a*</sub>\> \<*g*<sub>*a*</sub>\> \<*b*<sub>*a*</sub>\>
    \<*a*<sub>*a*</sub>\>. This is the colour of an unlit surface.
-   A line containing the the *specular* colour, opacity and reflection
    characteristics: \<*r*<sub>*s*</sub>\> \<*g*<sub>*s*</sub>\>
    \<*b*<sub>*s*</sub>\> \<*a*<sub>*s*</sub>\> \[\<*p*\>\]. This is the
    colour of light reflected by a polished surface into a narrow cone.
    The power entry (\<*p*\>) specifies the width of the cone into which
    light is reflected. Higher values represent a narrower cone, i.e.
    sharper reflections. Typical values are around 10. If \<*p*\> is
    omitted, the default value is 0.
-   A line containing the *emissive* colour and opacity:
    \<*r*<sub>*e*</sub>\> \<*g*<sub>*e*</sub>\> \<*b*<sub>*e*</sub>\>
    \<*a*<sub>*e*</sub>\>. This is the colour of light emitted by a
    glowing surface.

For each colour specification, \<*r*\>, \<*g*\> and \<*b*\> denote the
red, green and blue (RGB) colour components. \<*a*\> is the opacity. RGB
values should be between 0 and 1, but can be \> 1 for special effects.
Opacity values must be between 0 and 1.

## <span id="anchor-21"></span>Texture list

The texture list contains the names of the texture files loaded by the
mesh. Textures must be in DDS (Direct Draw Surface) format. The names in
the list must contain the file extension (.dds). They can contain a path
specification, which is interpreted relative to Orbiter’s main Texture
directory (.\\Textures by default).

-   A DirectX SDK tool, dxtex, which is included in the Orbiter SDK
    package, can be used to convert to DDS from other bitmap formats.
-   Use either DXT1 compressed format (for opaque textures or textures
    with binary transparency) or DXT5 (for textures containing
    semi-transparent areas). DXT1 compresses to half the size of DXT5
    but has slightly higher compression artefacts.
-   Texture bitmaps should have sizes of powers of 2 for maximum
    compatibility (in particular with older graphics cards). Also avoid
    texture maps \> 2048×2048 pixels.
-   If a texture is to be dynamically updated during the simulation
    (e.g. instrument panels in virtual cockpits) the texture name in the
    list should be followed by the flag ‘D’. Orbiter decompresses these
    surfaces on loading to allow more efficient dynamic updates.

## <span id="anchor-22"></span>Performance optimisation

Complex meshes can improve the visual appearance of the object they
represent, but they also impose a computational cost on the render
engine, leading to lower simulation frame rates and less fluid
animation. It can also potentially lead to a loss of accuracy in the
trajectory calculation. A balance between visualisation quality and
simulation performance should be kept in mind. There are a few ways to
optimise the mesh structure to improve performance:

-   Mesh groups using the same texture should be listed in sequence in
    the mesh. Unnecessary switching between textures can degrade
    performance if textures must be swapped in and out of video memory.

-   Within a sequence of groups using the same texture, groups using the
    same material should be kept together for the same reason.

-   Avoid large numbers of very small groups. If small mesh groups use
    the same texture and material parameters, and are not animated
    individually, consider merging them into a single group.

-   To avoid visual artefacts, groups using transparent textures or
    materials should be sorted to the end of the mesh. If transparent
    groups overlap, the innermost ones should be listed before the outer
    ones.

    In order to render transparency correctly, DirectX requires the
    scene seen through the transparent surfaces to be fully built before
    the transparent part itself is rendered. Any objects rendered after
    the transparent part can be obscured by it (z-buffering artefact).

-   Surfaces with transparency and specular reflection are more
    expensive to render than opaque and diffusive surfaces.

-   And most importantly: Keep the vertex count reasonably low!

## <span id="anchor-23"></span>Mesh converters

To convert an existing model in a standard mesh format to an Orbiter
mesh, check the Orbiter web forum for mesh converters created by other
users. There is currently a converter which converts from Truespace asc
format, which many 3D editors can export. If you have written your own
mesh editor or converter, publish it!

## <span id="anchor-24"></span>Mesh utilities

The Orbiter SDK contains a few utilities that help to extract data from
mesh files. They are located in the Orbitersdk\\utils folder.

-   **shipedit.exe**: Extracts geometric information from a mesh that is
    useful for setting up the physical vessel parameters in its
    configuration file or module code. These include the mesh bounding
    box, volume, cross-sectional areas, and inertia tensor assuming a
    homogeneous density distribution.
-   **meshc.exe**: Mesh compiler. Currently, this extracts mesh
    parameters and group labels into a C++ header file that can be
    included by the vessel code for convenient access to named mesh
    groups, e.g. to address them for animations and dynamic material
    updates.
