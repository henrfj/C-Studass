#include "GUI.h"
#include "std_lib_facilities.h"
#include <sstream>

using namespace Graph_lib;


void Button::attach(Graph_lib::Window& win)
{
	pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}

int Choice::add(const string& item)
{
	return reference_to<Fl_Choice>(pw).add(item.c_str());
}

void Choice::attach(Graph_lib::Window& win)
{
	pw = new Fl_Choice(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}

int In_box::get_int()
{
	Fl_Input& pi = reference_to<Fl_Input>(pw);
	const char* p = pi.value();
	if (!isdigit(p[0]))
		return -999999;
	return atoi(p);
}

string In_box::get_string()
{
	Fl_Input& pi = reference_to<Fl_Input>(pw);
	return string(pi.value());
}

void In_box::attach(Graph_lib::Window& win)
{
	pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}

void Out_box::put(int i)
{
	Fl_Output& po = reference_to<Fl_Output>(pw);
	std::stringstream ss;
	ss << i;
	po.value(ss.str().c_str());
}

void Out_box::put(const string& s)
{
	reference_to<Fl_Output>(pw).value(s.c_str());
}

void Out_box::attach(Graph_lib::Window& win)
{
	pw = new Fl_Output(loc.x, loc.y, width, height, label.c_str());
	own = &win;
}

void Multiline_out_box::put(int i)
{
	Fl_Multiline_Output& po = reference_to<Fl_Multiline_Output>(pw);
	std::stringstream ss;
	ss << i;
	po.value(ss.str().c_str());
}

void Multiline_out_box::put(const string& s)
{
	reference_to<Fl_Multiline_Output>(pw).value(s.c_str());
}

void Multiline_out_box::attach(Graph_lib::Window& win)
{
	pw = new Fl_Multiline_Output(loc.x, loc.y, width, height, label.c_str());
	pw->align(FL_ALIGN_TOP);
	own = &win;
}

int Menu::attach(Button& b)
{
	_attach(b);
	selection.push_back(b); // Push back reference (Menu owns the object)
	return static_cast<int>(selection.size() - 1);
}

int Menu::attach(Button* b)
{
	_attach(*b);
	selection.push_back(b); // Push back pointer (Menu does not own the object)
	return static_cast<int>(selection.size() - 1);
}

void Menu::_attach(Button& b)
{
	b.width = width;
	b.height = height;

	switch (k) {
	case horizontal:
		b.loc = Point{loc.x + offset, loc.y};
		offset += b.width;
		break;
	case vertical:
		b.loc = Point{loc.x, loc.y + offset};
		offset += b.height;
		break;
	}
}
