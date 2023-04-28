===========================================================================
XRSound 3.0

Version Date: 31-Jul-2021
  
Copyright (c) 2018-2021 Douglas Beachy
Licensed under the MIT license

https://www.alteaaerospace.com
mailto:doug.beachy@outlook.com
    
  Requirements:
    SpaceXpanse 2016 or newer; XRSound will *not* run under older SpaceXpanse versions.
===========================================================================

---------
LICENSE
---------

MIT License

Copyright (c) 2018-2021 Douglas Beachy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------
SUMMARY
-------

XRSound automatically adds default sounds to any SpaceXpanse vessel that defines thrusters.  It includes over 450 default sounds and voice callouts, including all those used by the XR fleet (DeltaGlider-XR1, XR2 Ravenstar, and XR5 Vanguard).  Each sound is fully configurable and may be replaced or disabled by editing $SPACEXPANSE_ROOT\XRSound\XRSound.cfg and/or a vessel class's XRSound-<vessel class>.cfg file. 

For information about the XRSound C++ API, refer to $SPACEXPANSE_ROOT\SpaceXpansesdk\XRSound.h and $SPACEXPANSE_ROOT\Doc\XRSound User Manual.pdf.  

Note for developers: the included XRSound.lib is for release builds and XRSoundD.lib is for debug builds.  You need likey need Visual Studio 2015 or newer in order to properly link with XRSound.lib or XRSoundD.lib: older Visual Studio versions have not been tested.

XRSound includes a new ambient music track named `Solar Serenity` that plays in external views in space by default; it was created by and licensed from Grzegorz Lorens ("Loru" on SpaceXpanse-Forum).  If you are interested in a lossless version of Solar Serenity, take a look here: https://loru.bandcamp.com/track/solar-serenity

Please refer to the included $SPACEXPANSE_ROOT\Doc\XRSound User Manual.pdf for more information and details about how to install, configure, and use XRSound.

------------------
QUICK INSTALLATION 
------------------

Quick installation instructions are:

    1. Unzip the XRSound.zip installation file to your $SPACEXPANSE_ROOT folder; e.g., C:\SpaceXpanse.
    2. Bring up your SpaceXpanse launch pad and click "Modules".
    3. In the "Sound module for SpaceXpanse" section, click on "XRSound" to activate it.
     
You are now ready to launch SpaceXpanse, and you should hear default sounds playing for your active vessel, provided it defines any thrusters.  For troubleshooting information and XRSound status info, refer to the $SPACEXPANSE_ROOT\XRSound.log file that is written whenever XRSound runs.

*** Please refer to the included `$SPACEXPANSE_ROOT\Doc\XRSound User Manual.pdf` for detailed installation and configuration instructions. ***

Happy orbiting!

-- end --
