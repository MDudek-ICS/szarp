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
#include <map>
#include <set>

#include <wx/config.h>

#ifdef __WXMSW__
#include <wx/generic/printps.h>
#include <wx/generic/prntdlgg.h>
#endif

#include <tr1/unordered_map>
#include <sstream>

#include "drawprint.h"
#include "drawview.h"
#include "draw.h"
#include "cconv.h"
#include "timeformat.h"
#include "xygraph.h"
#include "szframe.h"

using std::map;
using std::set;

class GraphPrinter;
class BackgroundPrinter;

#ifdef __WXMSW__ 
const int arrow_height = 42;
const int arrow_width = 30;
const int line_width = 7;
const int tick_len = 24;
#else
const int arrow_height = 7;
const int arrow_width = 5;
const int line_width = 1;
const int tick_len = 4;
#endif


/**Class represeting daws printout*/
class DrawsPrintout : public wxPrintout {
	/**Array of draws to print*/
	DrawPtrArray m_draws;

	/**Number of draws from array to print*/
	int m_draws_count;

	/**Prints names of draws at the bottom of the page*/
	void PrintDrawsInfo(wxDC *dc);

	/**Prints short names of draws at the top of the page*/
	void PrintShortDrawNames(GraphPrinter *gp, wxDC *dc);

	/**@return draws choosen for printing, draws having the same values range are grouped together*/
	std::set<std::set<int> > ChooseDraws();
	public:
	DrawsPrintout(DrawPtrArray draws, int count);
	/**prints page
	 * @param page number to print
	 * @return true if page has been successfully printed*/
	bool OnPrintPage(int page);

	/**@return true if page with given number is present in printout*/
	bool HasPage(int page);

	/**Obligatory method to implement by printout, simply calls the same method but from base class*/
	bool OnBeginDocument(int start, int end);

	/**Provides info on printout.*/
	void GetPageInfo(int *min, int *max, int *sel_from, int *sel_to);

};

namespace {

/**Simple struct representing values range*/
struct Axis {
	/**start of range*/
	double min;
	/**end of ragnge*/
	double max;

	int prec;

	wxString unit;

	Axis() : min(0), max(-1), prec(0), unit() {}

	int toint(double val) const { 
		for (int i = 0; i < prec; i++)
			val *= 10;
		return (int)val;
	}

	Axis& operator=(const Axis &r) {
		if (this != &r) {
			min = r.min;
			max = r.max;
			prec = r.prec;
			unit = r.unit;
		}
		return *this;
	}

	bool operator==(const Axis &r) const {
		if (this == &r)
			return true;

		return (prec == r.prec) && (toint(min) == toint(r.min)) && (toint(max) == toint(r.max)) && (unit == r.unit);
		
	}

};

struct AxisHasher {
	size_t operator()(const Axis& s) const { return s.prec + s.toint(s.min) + s.toint(s.max); }
};

}

const int print_top_margin = 5;
const int print_left_margin = 5;

typedef std::set<std::set<int> > SS;


/**Hold info on area that we print on*/
class PrintedRegion {
	/**Font to use for printing*/
	wxFont m_font;

	/**Widgth of area*/
	int m_width; 

	/**Height of area*/
	int m_height;
public:
	/**returns the size of area*/
	void GetSize(int *w, int *h) const;

	/**sets the size of area*/
	void SetSize(int w, int h);

	/**@return font to be used while priting text*/
	virtual wxFont GetFont() const;

	/**sets fonts that is used for printing*/
	void SetFont(wxFont &f);

	virtual ~PrintedRegion() {}
};

void PrintedRegion::GetSize(int *w, int *h) const {
	*w = m_width;
	*h = m_height;
}

void PrintedRegion::SetSize(int w, int h) {
	m_width = w;
	m_height = h;
}

void PrintedRegion::SetFont(wxFont &f) {
	m_font = f;
}

wxFont PrintedRegion::GetFont() const {
	return m_font;
}

/**Prints graphs background*/
class BackgroundPrinter : public BackgroundDrawer, public PrintedRegion {
	/**Finds distance (in 'pixels' between verticals axes*/
	int FindVerticalAxesDistance(wxDC *dc, DrawPtrArray draws, const SS &sd);
public:
	/**return size of region to print on*/
	virtual void GetSize(int* w, int *h) const;

	/**set size of region*/
	virtual void SetSize(int w, int h);

	/**@return device context used for printing*/
	virtual wxDC* GetDC();
	
	/**@return font for text printing*/ 
	virtual wxFont GetFont() const;

	/**Prints background
	* @param dc device context to print background on
	* @param draws draws do print
	* @param sd draws grouping (for printing vertical axes)
	* @return horizontal position of last printex vertical axe*/ 
	int PrintBackground(wxDC *dc, DrawPtrArray draws, const SS& sd);

	/**@return color of axes*/
	const wxColour& GetTimeAxisCol();

	/**@return color of 'odd' stripes. */
	const wxColour& GetBackCol1();

	/**@return color of 'even' stripes. */
	const wxColour& GetBackCol2();

	/**Color for background drawing - grey*/
	static const wxColour back1_col;

	BackgroundPrinter(int rightmargin,
			int topmargin,
			int bottommargin);

};

BackgroundPrinter::BackgroundPrinter(int rightmargin,
		int topmargin,
		int bottommargin){
	m_rightmargin = rightmargin;
	m_topmargin = topmargin;
	m_bottommargin = bottommargin;
}

const wxColour BackgroundPrinter::back1_col = wxColour(64, 64, 64);

void BackgroundPrinter::SetSize(int w, int h) {
	PrintedRegion::SetSize(w, h);
}

void BackgroundPrinter::GetSize(int *w, int *h) const {
	PrintedRegion::GetSize(w, h);
}

wxFont BackgroundPrinter::GetFont() const {
	return PrintedRegion::GetFont();
}

wxDC* BackgroundPrinter::GetDC() {
	return NULL;
}

int BackgroundPrinter::FindVerticalAxesDistance(wxDC *dc, DrawPtrArray draws, const SS& sd) {
	
	int d = 0;

	for (SS::const_iterator ssi = sd.begin(); ssi != sd.end(); ssi++) {
		set<int>::const_iterator si = (*ssi).begin();
		if ((*ssi).end() == si)
			assert(false);

		int di = *si;
		Draw *draw = draws[di];

		double max = draw->GetDrawInfo()->GetMax();
		double min = draw->GetDrawInfo()->GetMin();
		wxString maxs = draw->GetDrawInfo()->GetValueStr(max, _T(""));
		wxString mins = draw->GetDrawInfo()->GetValueStr(min, _T(""));

		wxString unit = draw->GetDrawInfo()->GetUnit();

		int maxext, minext, unitext, texth;
		dc->GetTextExtent(maxs, &maxext, &texth);
		dc->GetTextExtent(mins, &minext, &texth);
		dc->GetTextExtent(unit, &unitext, &texth);

		int msn = 0;
		do {
			int tw, th;
			dc->GetTextExtent(draws[*si]->GetDrawInfo()->GetUnit(), &tw, &th);
			msn = wxMax(msn, tw);
		} while (++si != (*ssi).end());
		

		int ext = wxMax(unitext, wxMax(maxext, minext));
		ext = wxMax(ext, msn) + line_width;

		d = wxMax(d, ext);

	} 

	return d;

}

int BackgroundPrinter::PrintBackground(wxDC *dc, DrawPtrArray draws, const SS& sd) {
	int xo = print_left_margin, yo = print_top_margin;
	dc->SetDeviceOrigin(xo, yo);

	dc->SetTextForeground(GetTimeAxisCol());
	dc->SetBrush(wxBrush(GetTimeAxisCol(), wxSOLID));

	int ax_dist = FindVerticalAxesDistance(dc, draws, sd);

	m_leftmargin = 0;

	SS::iterator ssi = sd.begin();
	do {
		set<int>::iterator si = (*ssi).begin();
		if ((*ssi).end() == si) 
			assert(false);
		int di = *si;

		m_draw = draws[di];

		m_leftmargin += ax_dist;

		SS::iterator next = ssi;
		next++;
		if (next == sd.end()) {
			int w,h;
			GetSize(&w, &h);
			w = w - m_leftmargin + ax_dist;
			SetSize(w, h);

			DrawBackground(dc);
#ifdef __WXMSW__
			DrawTimeAxis(dc, 48, 24, 15, 7);
#else
			DrawTimeAxis(dc, 8, 4, 6);
#endif
		}

		DrawYAxisVals(dc, tick_len, line_width);
		DrawYAxis(dc, arrow_height, arrow_width, line_width);

		int x = (int)(arrow_width * 1.2) , y = line_width;

		int textw, texth;
		DrawUnit(dc, x, y);
		dc->GetTextExtent(draws[*si]->GetDrawInfo()->GetUnit(), &textw, &texth);

		wxColour pc = dc->GetTextForeground();
		y = m_topmargin;
		for (set<int>::reverse_iterator i = (*ssi).rbegin(); i != (*ssi).rend(); i++) {
			DrawInfo* di = draws[*i]->GetDrawInfo();
			dc->GetTextExtent(di->GetShortName(), &textw, &texth);

			dc->SetTextForeground(di->GetDrawColor());
			dc->DrawText(di->GetShortName(), m_leftmargin - 2 * line_width - textw, y - texth - line_width);
			y -= texth + line_width;

		}
		dc->SetTextForeground(pc);

		++ssi;
		
	} while (ssi != sd.end());

	return m_leftmargin;

}

const wxColour& BackgroundPrinter::GetTimeAxisCol() {
	return *wxBLACK;
}
const wxColour& BackgroundPrinter::GetBackCol1() {
	return *wxLIGHT_GREY;
}
const wxColour& BackgroundPrinter::GetBackCol2() {
	return *wxWHITE;
}

/**Class resposible for printing graphs*/
class GraphPrinter : public GraphDrawer, public PrintedRegion {
protected:
	/**returns size of drawping region*/
	virtual void GetSize(int* w, int *h) const;
public:
	GraphPrinter(int circle_radius, 
			int leftmargin, 
			int rightmargin, 
			int topmargin, 
			int bottommargin);

	/**sets size of drawing region*/
	virtual void SetSize(int w, int h);

	/**@return dc used for drawing*/
	virtual wxDC* GetDC();

	/**@return true if given point shall be painted with different color*/
	virtual bool AlternateColor(int idx);

	/**Prints draws
	 * @param device contex graphs are to be printed with
	 * @param array of draws to print
	 * @param count number of draws from array that shall be printed*/
	void PrintDraws(wxDC *dc, DrawPtrArray draws, int count);

	/**Prints short name of draw
	 * @param dc device context to print with
	 * @param draw draw which short name is to be printed*/
	void PrintDrawShortName(wxDC *dc, Draw *draw);
	virtual wxFont GetFont() const;
};


void GraphPrinter::PrintDrawShortName(wxDC *dc, Draw *draw) {
	m_draw = draw;

	DrawInfo *draw_info = draw->GetDrawInfo();
	const Draw::VT& vt = draw->GetValuesTable();

	int i;
	for (i = vt.size() - 1; i >= 0; --i) 
		if (vt[i].IsData())
			break;

	if (i < 0)
		return;

	int x,y;
	GetPointPosition(dc, i, &x, &y);

	dc->SetTextForeground(draw_info->GetDrawColor());

	wxString sn = draw_info->GetShortName();

	int w, h;
	dc->GetTextExtent(sn, &w, &h);
	dc->DrawText(sn, x + 4, y - h / 2);

}

void GraphPrinter::SetSize(int w, int h) {
	PrintedRegion::SetSize(w, h);
}

void GraphPrinter::GetSize(int *w, int *h) const {
	PrintedRegion::GetSize(w, h);
}

wxFont GraphPrinter::GetFont() const {
	return PrintedRegion::GetFont();
}

GraphPrinter::GraphPrinter(int circle_radius, 
		int leftmargin,
		int rightmargin,
		int topmargin,
		int bottommargin) : GraphDrawer(circle_radius) {
	m_leftmargin = leftmargin;
	m_rightmargin = rightmargin;
	m_topmargin = topmargin;
	m_bottommargin = bottommargin;
}


wxDC* GraphPrinter::GetDC() {
	return NULL;
}

void GraphPrinter::PrintDraws(wxDC *dc, DrawPtrArray draws, int draws_count) {
	int sel = -1;
	for (int i = 0; i <= draws_count; ++i) {
		int j = i;

		if (i == draws_count)
			j = sel;
		else if (draws[i]->GetSelected()) {
			sel = i;
			continue;
		}

		if (draws[j]->GetEnable() == false)
			continue;

		m_draw = draws[j];
	
#ifdef __WXMSW__
		DrawAllPoints(dc, NULL, 7);
#else
		DrawAllPoints(dc, NULL);
#endif

	}

}

bool GraphPrinter::AlternateColor(int idx) {
	return false;
}

DrawsPrintout::DrawsPrintout(DrawPtrArray draws, int count) : m_draws(draws), m_draws_count(count) 
{}

std::set<std::set<int> > DrawsPrintout::ChooseDraws() {

	int count;
	if (wxConfig::Get()->Read(_T("MaxPrintedAxesNumber"), &count) == false)
		count = 3;
	if (count < 1)
		count = 1;

	Axis sr;
	std::tr1::unordered_map<Axis, std::set<int>, AxisHasher> ranges;

	for (int i = 0; i < m_draws_count; ++i) {

		Draw* draw = m_draws[i];

		if (!draw->GetEnable())
			continue;

		Axis r;
		r.min = draw->GetDrawInfo()->GetMin();
		r.max = draw->GetDrawInfo()->GetMax();
		r.prec = draw->GetDrawInfo()->GetPrec();
		r.unit = draw->GetDrawInfo()->GetUnit();


		if (m_draws[i]->GetSelected())
			sr = r;

		ranges[r].insert(i);

	}

	std::set<std::set<int> > ret;
	std::tr1::unordered_map<Axis, std::set<int>, AxisHasher>::iterator it = ranges.begin();
	for (int added = 0; added < count - 1 && it != ranges.end(); it++) {
		if (it->first == sr)
			continue;
		added++;
		ret.insert(it->second);
	}
	ret.insert(ranges[sr]);

	return ret;

}

bool DrawsPrintout::OnPrintPage(int page) {
	if (page != 1)
		return false;
	int sel = 0;
	for (; sel < m_draws_count; ++sel) 
		if (m_draws[sel]->GetSelected())
			break;
	if (sel == m_draws_count)
		return false;

#if 0
	wxConfigBase *cfg = wxConfig::Get();

	int circle_radius = cfg->Read(_T("PrintedCircleRadius"), int(2));
	int font_size = cfg->Read(_T("PrintedFontSize"), int(10));
#endif

	wxFont f;
#ifdef __WXMSW__ 
	f.Create(50, wxSWISS, wxNORMAL, wxNORMAL);
#else
	f.Create(8, wxSWISS, wxNORMAL, wxNORMAL);
#endif

	wxDC *dc = GetDC();
	dc->SetMapMode(wxMM_TEXT);
	dc->SetFont(f);

	int tw,th;
	dc->GetTextExtent(_T("Z"), &tw, &th);
	int topmargin, bottommargin, leftmargin, rightmargin;

	bottommargin = int(1.4 * th);
	topmargin = 0;
	rightmargin = 10;


	SS cd = ChooseDraws();
	for (SS::iterator i = cd.begin(); i != cd.end(); i++) {
		int tm = (th + line_width) * (i->size() + 1);
		if (tm > topmargin)
			topmargin = tm;
	}

	int w, h, pw, ph;
	dc->GetSize(&w, &h);
	GetPageSizePixels(&pw, &ph);
	dc->SetUserScale((float)w / (float)pw, (float)h / (float)ph);

	BackgroundPrinter bp(rightmargin, topmargin, bottommargin);
	bp.SetFont(f);
	bp.SetSize(pw - print_left_margin * 2, (ph - print_top_margin * 2) * 2 / 3);

	leftmargin = bp.PrintBackground(dc, m_draws, cd);
	bp.GetSize(&w, &h);
	GraphPrinter gp(
#ifdef __WXGTK__
			2, 
#else
			10, 
#endif
			leftmargin, rightmargin, topmargin, bottommargin);

	gp.SetSize(w, h);
	gp.SetFont(f);

	gp.PrintDraws(dc, m_draws, m_draws_count);	
	PrintShortDrawNames(&gp, dc);

	PrintDrawsInfo(dc);

	return true;
}

void DrawsPrintout::PrintShortDrawNames(GraphPrinter *gp, wxDC *dc) {

	int sel = -1;

	for (int i = 0; i <= m_draws_count; ++i) {
		int j = i;

		if (i == m_draws_count)
			j = sel;
		else if (m_draws[i]->GetSelected())  {
			sel = i;
			continue;
		}

		if (m_draws[j]->GetEnable() == false)
			continue;

		gp->PrintDrawShortName(dc, m_draws[j]);

	}
}

void DrawsPrintout::PrintDrawsInfo(wxDC *dc) {
	int w, h;
	int tw, th;
	int maxy = 5;


	GetPageSizePixels(&w, &h);
	int hw = w / 2;

	int a,b;
	dc->GetSize(&a, &b);

	dc->SetDeviceOrigin(0, b * 2 / 3);

	Draw* fd = m_draws[0];
	DrawInfo* fdi = m_draws[0]->GetDrawInfo();

	wxFont font = dc->GetFont();
	wxFont f = font;
#ifdef __WXMSW__
	f.SetPointSize(100);
#else

	f.SetPointSize(16);
#endif
	dc->SetTextForeground(*wxBLACK);
	dc->SetFont(f);

	wxString cn = fdi->GetDrawsSets()->GetID();
	dc->GetTextExtent(cn, &tw, &th);
	dc->DrawText(cn, hw - tw / 2, maxy);
	maxy += int(1.4 * th);

#ifdef __WXMSW__
	f.SetPointSize(65);
#else
	f.SetPointSize(8);
#endif
	dc->SetFont(f);

	wxString wt = fdi->GetSetName();
	dc->GetTextExtent(wt, &tw, &th);
	dc->DrawText(wt, hw - tw / 2, maxy);
	maxy += int(1.4 * th);

	PeriodType pt = fd->GetPeriod();
	wxString period = _("For period: ");
	switch (pt) {
		case PERIOD_T_YEAR:
			period += _("YEAR ");
			break;
		case PERIOD_T_MONTH:
			period += _("MONTH ");
			break;
		case PERIOD_T_WEEK:
			period += _("WEEK ");
			break;
		case PERIOD_T_DAY:
			period += _("DAY ");
			break;
		case PERIOD_T_SEASON:
			period += _("SEASON ");
			break;
		default:
			assert(false);
	}

	switch (pt) {
		case PERIOD_T_WEEK:
		case PERIOD_T_DAY:
			dc->GetTextExtent(period, &tw, &th);
			dc->DrawText(period, hw - tw / 2, maxy);
			maxy += int(1.4 * th);
			period = _T("");
			break;
		default:
			break;
	}

	period += _("From: ");
	period += FormatTime(fd->GetTimeOfIndex(0), pt);
	period += _(" to: ");
	period += FormatTime(fd->GetTimeOfIndex(fd->GetValuesTable().size() - 1), pt);

	dc->GetTextExtent(period, &tw, &th);
	dc->DrawText(period, hw - tw / 2, maxy);
	maxy += int(1.4 * th);

	for (int i = 0; i < m_draws_count; ++i) {
		Draw *d = m_draws[i];
		if (!d->GetEnable())
			continue;

		DrawInfo* di = d->GetDrawInfo();

		int cx = int(0.02 * a);

		wxString str = wxString::Format(_T("%s = %s "), 
					di->GetShortName().c_str(),
					di->GetName().c_str());

		dc->SetTextForeground(di->GetDrawColor());
		dc->GetTextExtent(str, &tw, &th);
		dc->DrawText(str, cx, maxy);

		cx += tw;

		const Draw::VT& vt = d->GetValuesTable();
		if (vt.m_count) {
			dc->SetTextForeground(*wxBLACK);

			wxString unit = di->GetUnit();

			str = wxString(_T(": ")) 
				+ _("min.=") + d->GetDrawInfo()->GetValueStr(vt.m_min, _T("- -")) + 
				+ _T(" ; ") + _("avg.=") + wxString(d->GetDrawInfo()->GetValueStr(vt.m_sum / vt.m_count, _T("- -"))) + 
				+ _T(" ; ") + _("max.=") + wxString(d->GetDrawInfo()->GetValueStr(vt.m_max, _T("- -"))) +
				+ _T(" ") + unit;

			dc->GetTextExtent(str, &tw, &th);
			dc->DrawText(str, cx, maxy);
			cx += tw;

			if (di->GetSpecial() == TDraw::HOURSUM) {
				wxString u = unit;
				if (u.Replace(_T("/h"), _T("")) == 0)
					u += _T("*h");
				wxString vals;
				double val = vt.m_hsum;
				wxString sunit = d->GetDrawInfo()->GetSumUnit();
				if (!sunit.IsEmpty()) {
					vals = wxString(di->GetValueStr(val, _T(""))) + _T(" ") + sunit;
				} else if (unit == _T("kW")) {
					vals = wxString(di->GetValueStr(val, _T(""))) + _T(" ") + _T("kWh") + 
						_T(" (") + wxString(di->GetValueStr(val * 3.6 / 1000, _T(""))) + _T(" GJ)");
				} else if (unit == _T("MW")) {
					vals = wxString(di->GetValueStr(val, _T(""))) + _T(" ") + _T("MWh") + 
						_T(" (") + wxString(di->GetValueStr(val * 3.6, _T(""))) + _T(" GJ)");
				} else if (unit.Replace(_T("/h"), _T("")) == 0) {
					u += _T("*h");
					vals = wxString(di->GetValueStr(val, _T(""))) + _T(" ") + u;
				} else {
					vals = wxString(di->GetValueStr(val, _T(""))) + _T(" ") + u;
				}
				str = wxString::Format(_T(" sum.: %s"), vals.c_str());

				dc->DrawText(str, cx, maxy);
			}
		}

		maxy += int(1.4 * th);

	}

}

bool DrawsPrintout::HasPage(int page) {
	return page == 1;
}

bool DrawsPrintout::OnBeginDocument(int start, int end) {
	return wxPrintout::OnBeginDocument(start, end);
}

void DrawsPrintout::GetPageInfo(int *min, int *max, int *sel_from, int *sel_to) {
	*min = 1;
	*max = 1;

	*sel_from = *sel_to = 1;
}

/**class representing xy grah printout*/
class XYGraphPrintout : public wxPrintout {
	/**page margins*/
	static const int bottom_margin;
	static const int top_margin;
	static const int left_margin;
	static const int right_margin;

	/**graph that is printed*/
	XYGraph *m_graph;

	/**@see XYGraphPainter*/
	XYGraphPainter m_painter;
	public:
	XYGraphPrintout(XYGraph *graph);

	/**prints page
	 * @param number of page to print*/
	bool OnPrintPage(int page);

	/**@return true if given page is present in printout*/
	bool HasPage(int page);

	/**Obligatory method to implement by printout, simply calls the same method but from base class*/
	bool OnBeginDocument(int start, int end);

	/**Provides info on printout.*/
	void GetPageInfo(int *min, int *max, int *sel_from, int *sel_to);

};

#ifdef __WXMSW__
const int XYGraphPrintout::left_margin = 64 * 6 ;
const int XYGraphPrintout::right_margin = 64 * 6;
const int XYGraphPrintout::top_margin = 24 * 6;
const int XYGraphPrintout::bottom_margin = 24 * 6;
#else
const int XYGraphPrintout::left_margin = 64;
const int XYGraphPrintout::right_margin = 64;
const int XYGraphPrintout::top_margin = 24;
const int XYGraphPrintout::bottom_margin = 24;
#endif

XYGraphPrintout::XYGraphPrintout(XYGraph *graph) :
	m_graph(graph), 
	m_painter(left_margin, right_margin, top_margin, bottom_margin)
{
	m_painter.SetGraph(graph);
}

bool XYGraphPrintout::OnPrintPage(int page) {
	if (page != 1)
		return false;

	wxFont f;
#ifdef __WXMSW__ 
	f.Create(50, wxSWISS, wxNORMAL, wxNORMAL);
#else
	f.Create(8, wxSWISS, wxNORMAL, wxNORMAL);
#endif

	wxDC *dc = GetDC();
	dc->SetMapMode(wxMM_TEXT);
	dc->SetFont(f);

	wxPen pen;
#if __WXMSW__
	pen = dc->GetPen();
	pen.SetWidth(5);
	dc->SetPen(pen);
#endif

	int w, h, pw, ph;
	dc->GetSize(&w, &h);
	GetPageSizePixels(&pw, &ph);
	dc->SetUserScale((float)w / (float)pw, (float)h / (float)ph);

	dc->SetDeviceOrigin(0, 20);

	w = w * 9 / 10;
	h = h * 2 / 3;

	m_painter.SetSize(w, h);

	m_painter.DrawXUnit(dc, 
#ifdef __WXGTK__
			6, 1
#else
			30, 5
#endif
			);
	m_painter.DrawYUnit(dc, 
#ifdef __WXGTK__
			1, 8
#else
			5, 35
#endif
			);
	m_painter.DrawXAxis(dc, 
#ifdef __WXGTK__
			7, 5, 2
#else
			35, 25, 10
#endif
			);
	m_painter.DrawYAxis(dc,
#ifdef __WXGTK__
			7, 5, 2
#else
			35, 25, 10
#endif
			);
	m_painter.DrawDrawsInfo(dc,
#ifdef __WXGTK__
			10, 2
#else
			50, 10
#endif
			);

	pen = dc->GetPen();
	pen.SetColour(m_graph->m_dy->GetDrawColor());
	dc->SetPen(pen);

	for (size_t i = 0; i < m_graph->m_visible_points.size(); ++i) {
		int x,y;
		m_painter.GetPointPosition(i, &x, &y);
		dc->DrawLine(x, y, x + 1, y + 1);
		if (m_graph->m_averaged) 
			dc->DrawCircle(x, y, 
#ifdef __WXGTK__
					2
#else
					10	
#endif
					);
	}

	dc->SetDeviceOrigin(0, h + 20);

	int tw, th;
	int ty = 0;

#ifdef __WXMSW__
	f.SetPointSize(70);
#else
	f.SetPointSize(12);
#endif
	dc->SetFont(f);

	wxString txt;

	txt = m_graph->m_dx->GetDrawsSets()->GetID();
	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw)  / 2, ty);

	ty += th * 12 / 10;

#ifdef __WXMSW__
	f.SetPointSize(50);
#else
	f.SetPointSize(8);
#endif
	dc->SetFont(f);

	txt = m_graph->m_dx->GetName();

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	txt = wxString(_("min.=")) + m_graph->m_dx->GetValueStr(m_graph->m_xmin, _T("")) 
			+ _T(" ; ") + _("avg.=") + m_graph->m_dx->GetValueStr(m_graph->m_xavg, _T(""))
			+ _T(" ; ") + _("max.=") + m_graph->m_dx->GetValueStr(m_graph->m_xmax, _T(""))
			+ _T(" ; ") + _("s.d.=") + m_graph->m_dx->GetValueStr(m_graph->x_standard_deviation, _T(""));

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	txt = m_graph->m_dy->GetName();

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	txt = wxString(_("min.=")) + m_graph->m_dy->GetValueStr(m_graph->m_ymin, _T("")) 
			+ _T(" ; ") + _("avg.=") + m_graph->m_dy->GetValueStr(m_graph->m_yavg, _T(""))
			+ _T(" ; ") + _("max.=") + m_graph->m_dy->GetValueStr(m_graph->m_ymax, _T(""))
			+ _T(" ; ") + _("s.d.=") + m_graph->m_dy->GetValueStr(m_graph->y_standard_deviation, _T(""));

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	txt = wxString(_("From:"))
			+ wxString(_T(" "))
			+ FormatTime(m_graph->m_start, m_graph->m_period);

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	txt = wxString(_("To:")) 
			+ wxString(_T(" ")) 
			+ FormatTime(m_graph->m_end, m_graph->m_period);

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	std::wstringstream wss;
	wss.precision(2);
	wss << m_graph->m_xy_correlation;

	txt = wxString(_("X/Y Correlation:")) 
			+ wxString(_T(" ")) 
			+ wss.str().c_str();

	wss.str(std::wstring());

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	wss << m_graph->m_xy_rank_correlation;
	txt = wxString(_("X/Y Rank correlation:")) 
			+ wxString(_T(" ")) 
			+ wss.str().c_str();

	dc->GetTextExtent(txt, &tw, &th);
	dc->DrawText(txt, (w - tw) / 2, ty);

	ty += th * 12 / 10;

	return true;

}

void XYGraphPrintout::GetPageInfo(int *min, int *max, int *sel_from, int *sel_to) {
	*min = 1;
	*max = 1;

	*sel_from = *sel_to = 1;
}

bool XYGraphPrintout::HasPage(int page) {
	return page == 1;
}

bool XYGraphPrintout::OnBeginDocument(int start, int end) {
	return wxPrintout::OnBeginDocument(start, end);
}

void Print::DoPrint(wxWindow *parent, DrawPtrArray draws, int count) {
	while (parent && parent->IsTopLevel() == false)
		parent = parent->GetParent();

	InitData();

	wxPrintDialogData print_dialog(*print_data);
	wxPrinter printer(&print_dialog);

	DrawsPrintout printout(draws, count);

	printer.Print(parent, &printout, true);
}

void Print::DoPrintPreviev(DrawPtrArray draws, int count) {
	InitData();

	wxPrintDialogData print_dialog_data(*print_data);

	wxPrintPreview *preview = new wxPrintPreview(new DrawsPrintout(draws, count), 
							new DrawsPrintout(draws, count), 
							&print_dialog_data);
	if (!preview->Ok()) {
		delete preview;
		return;
	}

	wxPreviewFrame *frame = new wxPreviewFrame(preview, NULL, _("Print Preview"), wxPoint(100, 100), wxSize(600, 650));
	frame->SetIcon(szFrame::default_icon);
	frame->Centre(wxBOTH);
	frame->Initialize();
	frame->Show();
}

void Print::InitData() {
	if (print_data == NULL)
		print_data = new wxPrintData();

	if (page_setup_data == NULL)
		page_setup_data = new wxPageSetupData();
}


void Print::DoXYPrintPreview(XYGraph *graph) {
	InitData();

	wxPrintDialogData print_dialog_data(*print_data);

	wxPrintPreview *preview = new wxPrintPreview(new XYGraphPrintout(graph), 
							new XYGraphPrintout(graph), 
							&print_dialog_data);
	if (!preview->Ok()) {
		delete preview;
		return;
	}

	wxPreviewFrame *frame = new wxPreviewFrame(preview, NULL, _("Print Preview"), wxPoint(100, 100), wxSize(600, 650));
	frame->Centre(wxBOTH);
	frame->Initialize();
	frame->Show();
}

void Print::DoXYPrint(wxWindow *parent, XYGraph *graph) {
	while (parent && parent->IsTopLevel() == false)
		parent = parent->GetParent();

	InitData();

	wxPrintDialogData print_dialog(*print_data);
	wxPrinter printer(&print_dialog);

	XYGraphPrintout printout(graph);

	printer.Print(NULL, &printout, true);

};

wxPrintData* Print::print_data = NULL;
wxPageSetupData* Print::page_setup_data = NULL;


