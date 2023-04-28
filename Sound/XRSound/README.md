# XRSound
XRSound is an add-on for the SpaceXpanse Metaverse Simulator that automatically plays sounds for all SpaceXpanse vessels, as well as enabling custom SpaceXpanse vessels to play custom sounds.


## License

XRSound is open source and is licensed under the MIT license. Refer to the [LICENSE](./LICENSE) file for details.

## Installing and Using XRSound

Refer to the [XRSound User Manual](./XRSound/assets/Doc/XRSound%20User%20Manual.pdf) for details about how to install and use XRSound.

## Building XRSound 

You do not need to build XRSound in order to use it with SpaceXpanse. However, if you want to build XRSound from the source, follow the steps below. These instructions assume you are building both the 32-bit and 64-bit versions of XRSound. However, you may build only one version if you prefer.

1. Install Visual Studio 2019 from https://visualstudio.microsoft.com/downloads/.
2. Download the 32-bit and 64-bit versions of the irrKlang sound engine from https://www.ambiera.com/irrklang/downloads.html.
3. Download and install (or build) SpaceXpanse 2016 or later from either https://github.com/mschweiger/spacexpanse or http://orbit.medphys.ucl.ac.uk/download.html.
4. Clone the SpaceXpanse repository (which includes XRSound) from GitHub to your local machine with:
```bash
git clone git@github.com:mschweiger/spacexpanse.git
```
or
```bash
git clone https://github.com/mschweiger/spacexpanse.git
```

If you're looking for an excellent GUI that makes working with Git easier, I recommend [Tower](https://www.git-tower.com/).

5. Create six environment variables pointing to where you plan to install each of the following software packages:

* `SPACEXPANSE_ROOT` => your 32-bit Debug* SpaceXpanse root folder
* `SPACEXPANSE_ROOT_X64` => your 64-bit Debug* SpaceXpanse root folder
* `SPACEXPANSE_ROOT_RELEASE` => your 32-bit Release SpaceXpanse root folder
* `SPACEXPANSE_ROOT_RELEASE_X64` => your 64-bit Release SpaceXpanse root folder
* `SPACEXPANSE_EXE` => `path\filename` relative to SpaceXpanse root folder of your preferred 32-bit SpaceXpanse executable; e.g., `spacexpanse.exe`
* `SPACEXPANSE_EXE_X64` => `path\filename` relative to SpaceXpanse root folder of your preferred 64-bit SpaceXpanse executable; e.g., `Modules\Server\spacexpanse.exe`

6. Install or build 32-bit Debug* SpaceXpanse to `%SPACEXPANSE_ROOT%`.
7. Install or build 64-bit Debug* SpaceXpanse to `%SPACEXPANSE_ROOT_X64%`.
8. Install or build 32-bit Release SpaceXpanse to `%SPACEXPANSE_ROOT_RELEASE%`.
9. Install or build 64-bit Release SpaceXpanse to `%SPACEXPANSE_ROOT_RELEASE_X64%`.

\* Note: you can always compile and test debug (as well as release) versions of the XR vessels against _release_ builds of SpaceXpanse, so can always set `SPACEXPANSE_ROOT` to match `SPACEXPANSE_ROOT_RELEASE` and `SPACEXPANSE_ROOT_X64` to match `SPACEXPANSE_ROOT_RELEASE_X64` if you prefer.

10. Extract 32-bit irrKlang to %IRRKLANG_ROOT%.
11. Extract 64-bit irrKlang to %IRRKLANG_ROOT_X64%.
12. Copy `%IRRKLANG_ROOT%\bin\win32-visualStudio\*.dll` to `%SPACEXPANSE_ROOT%` and `%SPACEXPANSE_ROOT_RELEASE%`.
13. Copy `%IRRKLANG_ROOT_X64%\bin\winx64-visualStudio\*.dll` to `%SPACEXPANSE_ROOT_X64%` and `%SPACEXPANSE_ROOT_RELEASE_X64`.
14. Copy everything under `XRSound\assets\XRSound\` to `%SPACEXPANSE_ROOT%\XRSound` (or create a symbolic link if you prefer).
15. Copy everything under `XRSound\assets\XRSound\` to `%SPACEXPANSE_ROOT_X64%\XRSound` (or create a symbolic link if you prefer).
16. Copy everything under `XRSound\assets\XRSound\` to `%SPACEXPANSE_ROOT_RELEASE%\XRSound` (or create a symbolic link if you prefer).
17. Copy everything under `XRSound\assets\XRSound\` to `%SPACEXPANSE_ROOT_RELEASE_X64%\XRSound` (or create a symbolic link if you prefer).

Now you are ready to compile and link XRSound.

18. Bring up Visual Studio 2019 and open the solution `spacexpanse\Sound\XRSound\XRSound\src\XRSound.sln`.
19. Set the build target to `Debug` and `x86` (i.e., 32-bit) to start. Click `Build -> Rebuild Solution`. This will build `XRSoundD.lib` and `XRSound.dll` and copy those files to their proper locations under `%SPACEXPANSE_ROOT%` via a Post-Build step. If you get build errors, double-check that the above environment variables are set correctly and that you restarted Visual Studio 2019 _after_ you defined those environment variables.
20. After the build succeeds, bring up SpaceXpanse and activate the XRSound module as detailed in the [XRSound User Manual](./XRSound/assets/Doc/XRSound%20User%20Manual.pdf).
21. Launch a DeltaGlider scenario -- you should hear an audio greeting from your ship's new A.I. If you don't hear any sound, check your `%SPACEXPANSE_ROOT%\XRSound.log` file for error messages.

For more information and support about SpaceXpanse and XRSound, visit https://www.spacexpanse-forum.com/.

Happy Orbiting!
