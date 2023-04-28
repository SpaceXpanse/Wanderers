/* Copyright © 2007-9 Steve Arch, Duncan Sharpe
** Copyright © 2011 atomicdryad - 'ENT' button & Pen allocation fix
** Copyright © 2013 Dimitris Gatsoulis (dgatsoulis) - Hacks
** Copyright © 2013 Szymon Ender (Enjo) - Auto-Min™, Auto-Center™ & other hacks
**
** X11 License ("MIT License")
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.*/

#ifndef BODYPROVIDER_H
#define BODYPROVIDER_H

#include <vector>
#include <map>
#include <SpaceXpansesdk.h>

struct GBODY{
	OBJHANDLE bodyhandle;
	GBODY *parent, *next, *previous; // next and previous bodies orbiting same body
	std::vector <GBODY*> satellites;
	double soisize2;
	double gravbodyratio2;
	double mass;
};

class BodyProvider
{
    public:
        BodyProvider();
        virtual ~BodyProvider();
        void InitialiseSolarSystem();
        GBODY * GetSun();
        GBODY * GetBody(OBJHANDLE handle);
    protected:
    private:
        void DeleteGBody(GBODY *body);
        GBODY *sun; // a tree of the sun, its satellites, the satellites' satellites etc.
        GBODY * bodyCached;
        OBJHANDLE handleCached;
        std::map<OBJHANDLE, GBODY*> bodyMap;
};

#endif // BODYPROVIDER_H
