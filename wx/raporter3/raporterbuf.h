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
/* $Id$
 *
 * SZARP

 * ecto@praterm.com.pl
 */

#ifndef _RAPORTERBUF_H
#define _RAPORTERBUF_H

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if 0
/** Raporter buffer size dialog */
class szRaporterBuf : public wxDialog {
  DECLARE_CLASS(szRaporterBuf)
  DECLARE_EVENT_TABLE()
public:
  /** returned data */
  struct {
    /** buffer size */
    int m_bufsize;
  }g_data;
  /** */
  szRaporterBuf(wxWindow* parent, wxWindowID id, const wxString& title);
  /** event: button: help */
  void OnHelp(wxCommandEvent &ev);
};
#endif

#endif //_RAPORTERBUF_H
