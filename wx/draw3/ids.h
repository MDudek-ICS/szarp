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
 * draw3
 * SZARP
 
 * Pawe� Pa�ucha pawel@praterm.com.pl
 *
 * $Id$
 * Widgets identifiers.
 */

#ifndef __IDS_H__
#define __IDS_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** Available widgets identifiers */
enum { drawID_SELDRAW = wxID_HIGHEST, 
	drawID_SELSET, 
	drawID_SELDRAWCB, 
	drawTB_MENUBAR, 
	drawTB_ABOUT, 
	drawTB_SUMWIN, 
	drawTB_SPLTCRS, 
	drawTB_REFRESH, 
	drawTB_EXIT, 
	drawTB_FIND,
	drawTB_LANGUAGE,
	langID_pl,
	langID_en,
	langID_de,
	langID_fr,
	langID_sr,
	langID_it,
	langID_hu,
	drawID_SUMMWIN,
	seldrawID_CTX_BLOCK_MENU,
	seldrawID_CTX_DOC_MENU,
	seldrawID_CTX_EDIT_PARAM,
	seldrawID_PSC,
	drawpickID_COLOR,
	drawpickTB_SAVE,
	drawpickTB_LOAD,
	drawpickTB_REMOVE,
	drawpickTB_EDIT,
	drawpickTB_PARAM_EDIT,
	drawpickTB_COLOR,
	drawpickTB_UP,
	drawpickTB_DOWN,
	drawTB_FILTER,
	incsearch_TEXT,
	incsearch_DIALOG,
	incsearch_CHOICE,
	incsearch_LIST,
	incsearch_MENU_REMOVE,
	incsearch_MENU_EDIT,
	psc_REMOVE,
	psc_EDIT,
	psc_ADD,
	psc_TO_CLK,
	psc_RELOAD,
	XY_GRAPH_FRAME,
	XY_GRAPH_DIALOG,
	XY_START_TIME,
	XY_END_TIME,
	XY_XAXIS_BUTTON,
	XY_CHOICE_PERIOD,
	XY_YAXIS_BUTTON,
	XY_GOTO_GRAPH_BUTTON,
	drawID_XYDIAG,
	STAT_DIAG,
	STAT_DRAW_BUTTON,
	STAT_START_TIME,
	STAT_END_TIME,
	STAT_CHOICE_PERIOD,
	STAT_CALCULATE_BUTTON,
	STAT_CLOSE_BUTTON,
	XY_CHANGE_GRAPH,
	XY_PRINT,
	XY_PRINT_PREVIEW,
	XY_ZOOM_OUT,
	drawID_STAT_DIAG,

//     myID_PROPERTIES,
//     myID_INDENTINC,
//     myID_INDENTRED,
//     myID_BRACEMATCH,
//     myID_PAGEACTIVE,
//     myID_DISPLAYEOL,
//     myID_INDENTGUIDE,
//     myID_LINENUMBER,
//     myID_LONGLINEON,
//     myID_WHITESPACE,
//     myID_FOLDTOGGLE,
//     myID_OVERTYPE,
//     myID_READONLY,
//     myID_WRAPMODEON,
//     myID_CHANGECASE,
//     myID_CHANGELOWER,
//     myID_CHANGEUPPER,
//     myID_HILIGHTLANG,
//     myID_HILIGHTFIRST,
//     myID_HILIGHTLAST = myID_HILIGHTFIRST + 99,
//     myID_CONVERTEOL,
//     myID_CONVERTCR,
//     myID_CONVERTCRLF,
//     myID_CONVERTLF,
//     myID_USECHARSET,
//     myID_CHARSETANSI,
//     myID_CHARSETMAC,
//     myID_PAGEPREV,
//     myID_PAGENEXT,
//     myID_SELECTLINE,
// 
//     // other IDs
//     myID_STATUSBAR,
//     myID_TITLEBAR,
//     myID_ABOUTTIMER,
//     myID_UPDATETIMER,
// 
//     // dialog find IDs
//     myID_DLG_FIND_TEXT,
// 
//     // preferences IDs
//     myID_PREFS_LANGUAGE,
//     myID_PREFS_STYLETYPE,
//     myID_PREFS_KEYWORDS,


    codeEditorID_INDENTINC,
    codeEditorID_INDENTRED,
    codeEditorID_SELECTLINE,
    codeEditorID_BRACEMATCH,
    codeEditorID_INDENTGUIDE,
    codeEditorID_LINENUMBER,
    codeEditorID_LONGLINEON,
    codeEditorID_WHITESPACE,
    codeEditorID_FOLDTOGGLE,
    codeEditorID_OVERTYPE,
    codeEditorID_READONLY,
    codeEditorID_WRAPMODEON,
    codeEditorID_CHARSETANSI,
    codeEditorID_CHARSETMAC,
    codeEditorID_CHANGELOWER,
    codeEditorID_CHANGEUPPER,
    codeEditorID_CONVERTCR,
    codeEditorID_CONVERTCRLF,
    codeEditorID_CONVERTLF,
    psc_CHOICE,
    LAST,
    first_frame_id } ;

/** Enumeration type for timer-driven action. Keyboard-invoked
 * actions are after mouse events, so it is easier in code to
 * check type of action. */
enum ActionKeyboardType { 
	NONE, 		/**< No action - update displayed time. */
	CURSOR_UP, 	/**< Move cursor up */
	CURSOR_DOWN, 	/**< Move cursor down */
	CURSOR_UP_KB,	/**< Move cursor up from keyboard,
			this is the first keyboard event. */
	CURSOR_DOWN_KB,	/**< Move cursor down from keyboard,
			this is the first keyboard event. */
	CURSOR_LEFT, 	/**< Move cursor left */
	CURSOR_RIGHT, 	/**< Move cursor write */
	CURSOR_LEFT_KB,	/**< Move cursor left from keyboard,
			this is the first keyboard event. */
	CURSOR_RIGHT_KB,
			/**< Move cursor write from keyboard */
	CURSOR_LONG_LEFT_KB,	/**< Move cursor long left from keyboard. */
	CURSOR_LONG_RIGHT_KB,	/**< Move cursor long right from keyboard. */
	CURSOR_HOME_KB,	/**< Move cursor to the begin of screen */
	CURSOR_END_KB,	/**< Move cursor to the end of screen */
	SCREEN_LEFT_KB, /**< Move screen left. */
	SCREEN_RIGHT_KB, /**< Move screen right. */
};

#define DRAW3_BG_COLOR wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND) 

/** Number of draws per set */
#define MAX_DRAWS_COUNT 12

#define RIGHT_PANEL_LENGTH	300

/** Enumeration type for period shown on time axis. */
typedef enum { PERIOD_T_YEAR = 0, PERIOD_T_MONTH, PERIOD_T_WEEK,
	PERIOD_T_DAY, PERIOD_T_SEASON, PERIOD_T_OTHER, PERIOD_T_LAST } PeriodType;

/**period names*/
const wxString period_names[PERIOD_T_LAST] = 
	{ _("YEAR"), _("MONTH"), _("WEEK"), _("DAY"), _("SEASON") };

/**Type of database inquires identificators*/
typedef int InquirerId;

#endif

