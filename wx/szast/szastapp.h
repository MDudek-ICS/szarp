/* 
  SZARP: SCADA software 
  

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
#ifndef __SZASTAPP_H__
#define __SZASTAPP_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "szapp.h"

/**
 * Main application class.
 */
class SzastApp : public szApp
{
	/**
	 * Method called on application start.
	 */
	virtual bool OnInit();

#ifndef MINGW32
	bool CheckGroup();
#endif
private:        
        wxLocale locale;        /**< Application locale object. */
	/**
	 * Method is responsible for parsing command line. It sets
	 * program's geometry and also prints usage info.
	 * @return 0 if OK, -1 if error occured
	 */
	int ParseCommandLine();
	int x, y;	/**< Program's window position. */
	int width, height;
			/**< Program's windows size. */
};

DECLARE_APP(SzastApp)

#endif

        
