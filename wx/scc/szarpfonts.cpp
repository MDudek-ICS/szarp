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
/*
 * scc - Szarp Control Center - setting fonts for gtk applications and draw
 * SZARP

 * Pawe� Pa�ucha pawel@praterm.com.pl
 *
 * $Id$
 */

#include "szarpfonts.h"

#ifndef MINGW32

#ifdef CONFIG_SZARPDRAW
#include "libpar.h"
#endif

#include "cconv.h"

BEGIN_EVENT_TABLE(szCommonFontDlg, wxDialog)
	EVT_CHOICE(wxID_ANY, szCommonFontDlg::OnChoice)
	EVT_BUTTON(wxID_OK, szCommonFontDlg::OnApply)
END_EVENT_TABLE()

/** Standard logging is set to stderr, so we use wxMessageBox to display error
 * messages. */
#define PopUpError(s) \
		wxMessageBox(s, _("Error"), wxOK | wxICON_ERROR)
	
szCommonFontDlg::szCommonFontDlg(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, _("Setting default font size"))
{
	
	main_s = new wxBoxSizer(wxVERTICAL);
	wxSizer* but_s = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* lab_s = new wxStaticBoxSizer(wxHORIZONTAL, this);
	wxSizer* top_s = new wxBoxSizer(wxHORIZONTAL);

	
	lab_s->Add(m_text = new wxStaticText(this, wxID_ANY, 
				_("Default font size for SZARP")),
				1);
	top_s->Add(lab_s, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 5);

	wxFont font = m_text->GetFont();
	m_fsize = font.GetPointSize();
	if (m_fsize < min_font_size) {
		m_fsize = min_font_size;
	} else if (m_fsize > max_font_size) {
		m_fsize = max_font_size;
	}
	font.SetPointSize(m_fsize);
	m_text->SetFont(font);

	wxArrayString sizes;
	for (int i = min_font_size; i <= max_font_size; i++) {
		sizes.Add(wxString::Format(_T("%d"), i));
	}
	wxChoice* choice = new wxChoice(this, wxID_ANY, wxDefaultPosition,
			wxDefaultSize, sizes);
	choice->SetSelection(m_fsize - min_font_size);
	top_s->Add(choice, 0, wxCENTER | wxALL, 5);
	
	main_s->Add(top_s, 0, wxEXPAND | wxALL, 5);


	but_s->AddStretchSpacer(1);
	but_s->Add(new wxButton(this, wxID_OK, _("Apply")),
			0,
			 wxEXPAND | wxALL, 10);
	but_s->AddStretchSpacer(1);
	but_s->Add(new wxButton(this, wxID_CANCEL, _("Cancel")),
			0, wxEXPAND | wxALL, 10);
	but_s->AddStretchSpacer(1);
	
	main_s->Add(but_s, 0, wxALIGN_BOTTOM | wxEXPAND);
	
	SetSizer(main_s);
	main_s->SetSizeHints(this);
}

void szCommonFontDlg::OnChoice(wxCommandEvent& event)
{
	wxFont font = m_text->GetFont();
	m_fsize = event.GetSelection() + min_font_size;
	font.SetPointSize(m_fsize);
	m_text->SetFont(font);
	Fit();
	main_s->SetSizeHints(this);
}

void szCommonFontDlg::OnApply(wxCommandEvent& event)
{
	event.Skip(true);
	wxString dir = CreateSzarpDir();
	if (dir.IsEmpty()) {
		return;
	}
	CreateGtkConfig(dir);
#ifdef CONFIG_RAPORTER
	CreateRaporterConfig(dir);
#endif /* CONFIG_RAPORTER */
#ifdef CONFIG_SZARPDRAW
	bool ubuntu = false;
	char *c = libpar_getpar("", "ubuntu", 0);
	if (c != NULL) {
		if (!strcmp(c, "yes")) {
			ubuntu = true;
		}
		free(c);
	}
	CreateSzarpDrawConfig(dir, ubuntu);
#endif /* CONFIG_SZARPDRAW */
	wxMessageBox(
			_("Changes will have effect only \nin newly started programs."),
			_("Message"), 
			wxOK | wxICON_INFORMATION, 
			this
			);
}

wxString szCommonFontDlg::CreateSzarpDir()
{
	wxString dir;
	if (wxGetEnv(_T("HOME"), &dir) == false) {
		PopUpError(_("Cannot get HOME environment variable"));
		return wxEmptyString;
	}
	if (wxDirExists(dir) == false) {
		PopUpError(wxString::Format(_("Your home directory (%s) does not exists!"),
				dir.c_str()));
		return wxEmptyString;
	}
	dir += _T("/");
	dir += SZARPDIR;
	if (wxDirExists(dir)) {
		return dir;
	}
	if (wxMkdir(dir, 0775) == false) {
		PopUpError(wxString::Format(_("Cannot create directory %s - errno %d\n%s"), 
				dir.c_str(), wxSysErrorCode(),
				wxSysErrorMsg()));
		return wxEmptyString;
	}
	return dir;
}

void szCommonFontDlg::CreateGtkConfig(wxString dir)
{
	wxString path = dir + _T("/gtk.rc");
	FILE *  f = fopen(SC::S2A(path).c_str(), "w");
	if (f == NULL) {
		PopUpError(wxString::Format(_("Cannot create file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(), 
				wxSysErrorMsg()));
		return;
	}
	int ret = fprintf(f, "\
# SZARP Gtk resource file\n\
# Automaticaly generated by SCC - do not edit\n\
\n\
style \"szarpfont\" {\n\
        font_name = \"Sans Regular %d\"\n\
}\n\
\n\
widget_class \"*\" style \"szarpfont\"\n\
\n\
# vim: set filetype=gtkrc :\n\
\n\
",
			m_fsize
			);
	if (ret <= 0) {
		PopUpError(wxString::Format(_("Error writing to file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(),
				wxSysErrorMsg));
	}
	fclose(f);
}

#ifdef CONFIG_RAPORTER
void szCommonFontDlg::CreateRaporterConfig(wxString dir)
{
	wxString path = dir + _T("/raporter.cfg");
	FILE *  f = fopen(SC::S2A(path).c_str(), "w");
	if (f == NULL) {
		PopUpError(wxString::Format(_("Cannot create file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(), 
				wxSysErrorMsg()));
		return;
	}
	int ret = fprintf(f, "\
# SZARP Raporter 2.X (Tcl/Tk) config file.\n\
# Automaticaly generated by SCC - do not edit\n\
\n\
FontSize=%d\n\
\n\
",
			m_fsize
			);
	if (ret <= 0) {
		PopUpError(wxString::Format(_("Error writing to file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(),
				wxSysErrorMsg));
	}
	fclose(f);
}
#endif /* CONFIG_RAPORTER */

#ifdef CONFIG_SZARPDRAW
void szCommonFontDlg::CreateSzarpDrawConfig(wxString dir, bool ubuntu)
{
	wxString path = dir;
	if (ubuntu) {
		path += _T("/SzarpDrawUbuntu.res");
	} else {
		path += _T("/SzarpDraw.res");
	}
	FILE *  f = fopen(SC::S2A(path).c_str(), "w");
	if (f == NULL) {
		PopUpError(wxString::Format(_("Cannot create file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(), 
				wxSysErrorMsg()));
		return;
	}
	int small_size = m_fsize * 10 / 12;
	int ret;
	if (ubuntu) {
		/* On Ubuntu we have small and ugly fonts...
		 * Lets make them bigger */
		int n = (int) (m_fsize * 1.2);
		int s = (int) (small_size * 1.2);
		ret = fprintf(f, "\
! SZARP Draw 2.X (Motif) resources file.\n\
! Automaticaly generated by SCC - do not edit\n\
\n\
! Include main resources file\n\
# include \""PREFIX"/resources/Motif/SzarpDraw/SzarpDrawUbuntu.res\"\n\
\n\
Draw*fontList:\\\n\
-*-*-medium-r-normal-*-%d-*-*-*-m-*-iso8859-2=charset0,\\\n\
-*-*-medium-r-normal-*-%d-*-*-*-m-*-iso8859-2=charset1,\\\n\
-*-*-medium-r-normal-*-%d-*-*-*-m-*-iso8859-2=charset2,\\\n\
-*-fixed-medium-*-normal-*-%d-*-*-*-*-*-*-*=charset3\n\
\n\
Draw.form.baloon_popup.?.fontList:\\\n\
-*-*-medium-r-normal-*-%d-*-*-*-m-*-iso8859-2\n\
\n\
! vim: set filetype=xdefaults :\n\
\n\
",
			n, s, s, s, s
			);
	} else {
		ret = fprintf(f, "\
! SZARP Draw 2.X (Motif) resources file.\n\
! Automaticaly generated by SCC - do not edit\n\
\n\
! Include main resources file\n\
# include \""PREFIX"/resources/Motif/SzarpDraw/SzarpDraw.res\"\n\
\n\
Draw*fontList:\\\n\
-b&h-lucidatypewriter-medium-r-normal-sans-%d-*-*-*-m-*-iso8859-2=charset0,\\\n\
-b&h-lucidatypewriter-medium-r-normal-sans-%d-*-*-*-m-*-iso8859-2=charset1,\\\n\
-b&h-lucidatypewriter-medium-r-normal-sans-%d-*-*-*-m-*-iso8859-2=charset2,\\\n\
-*-fixed-medium-*-normal-*-%d-*-*-*-*-*-*-*=charset3\n\
\n\
Draw.form.baloon_popup.?.fontList:\\\n\
-b&h-lucidatypewriter-medium-r-normal-sans-%d-*-*-*-m-*-iso8859-2\n\
\n\
! vim: set filetype=xdefaults :\n\
\n\
",
			m_fsize, small_size, small_size, small_size, small_size
			);
	}
	if (ret <= 0) {
		PopUpError(wxString::Format(_("Error writing to file %s - errno %d\n%s"),
				path.c_str(), wxSysErrorCode(),
				wxSysErrorMsg));
	}
	fclose(f);
}
#endif /* CONFIG_SZARPDRAW */

#endif /* ! MINGW32 */

