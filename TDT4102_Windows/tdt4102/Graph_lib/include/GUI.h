//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#pragma once

#include "Window.h"
#include "Graph.h"

namespace Graph_lib {

//------------------------------------------------------------------------------

using Address = void*; // Address is a synonym for void*
using Callback = void(*)(Address, Address); // FLTK's required function type for all callbacks
// Callback is a pointer to a function

//------------------------------------------------------------------------------

template <class W> W& reference_to(Address pw)
// treat an address as a reference to a W
{
	return *static_cast<W*>(pw);
}

//------------------------------------------------------------------------------

class Widget
{
	// Widget is a handle to an Fl_widget - it is *not* an Fl_widget
	// We try to keep our interface classes at arm's length from FLTK
public:
	Widget(Point xy, int w, int h, const string& s, Callback cb)
		: loc(xy), width(w), height(h), label(s), do_it(cb)
	{}

	void redraw()
	{
		pw->redraw(); // Schedules redrawing of widget, as per FLTK docs
	}

	virtual void move(int dx, int dy)
	{
		hide();
		pw->position(loc.x += dx, loc.y += dy);
		show();
	}
	virtual void hide() { pw->hide(); }
	virtual void show() { pw->show(); }
	virtual void attach(Window&) = 0;

	virtual void clear_value() {}

	virtual void set_label(string l)
	{
		label = l;
		pw->label(label.c_str());
	}

	Point loc;
	int width;
	int height;
	string label;
	Callback do_it;

	virtual ~Widget()
	{}

protected:
	Window* own;   // every Widget belongs to a Window
	Fl_Widget* pw; // connection to the FLTK Widget
private:
	Widget& operator=(const Widget&); // don't copy Widgets
	Widget(const Widget&);
};

//------------------------------------------------------------------------------

struct Button : Widget
{
	Button(Point xy, int w, int h, const string& label, Callback cb) : Widget(xy, w, h, label, cb) {}

	void attach(Window&);
};

//------------------------------------------------------------------------------

struct Choice : Widget
{
	Choice(Point xy, int w, int h, const string& s) : Widget(xy, w, h, s, 0)
	{}

	int add(const string& item);

	void clear_value() override
	{
		static_cast<Fl_Choice*>(pw)->value(0);
	}

	void attach(Graph_lib::Window& win) override;

	int value()
	{
		return static_cast<Fl_Choice*>(pw)->value();
	}
};

//------------------------------------------------------------------------------

struct In_box : Widget
{
	In_box(Point xy, int w, int h, const string& s) : Widget(xy, w, h, s, nullptr)
	{}
	int get_int();
	string get_string();
	void clear_value() override { static_cast<Fl_Input*>(pw)->value(""); }

	void attach(Window& win) override;
};

//------------------------------------------------------------------------------

struct Out_box : Widget
{
	Out_box(Point xy, int w, int h, const string& s) : Widget(xy, w, h, s, nullptr)
	{}
	void put(int);
	void put(const string&);

	void attach(Graph_lib::Window& win) override;
};

//------------------------------------------------------------------------------

struct Multiline_out_box : Widget
/* Similar to out_box, supporting multiple lines.
 * Newline characther, '\n', is used to break the lines.
 */
{
	Multiline_out_box(Point xy, int w, int h, const string& s) : Widget(xy, w, h, s, nullptr)
	{}
	void put(int);
	void put(const string&);

	void attach(Window& win);
};

//------------------------------------------------------------------------------

struct Menu : Widget
{
	enum Kind { horizontal, vertical };
	Menu(Point xy, int w, int h, Kind kk, const string& label)
		: Widget(xy, w, h, label, nullptr), k(kk), offset(0) {}

	Vector_ref<Button> selection;
	Kind k;
	int offset;
	int attach(Button& b); // Menu does not delete &b
	int attach(Button* p); // Menu deletes p

	void show() // show all buttons
	{
		for (int i = 0; i < selection.size(); ++i)
			selection[i].show();
	}
	void hide() // hide all buttons
	{
		for (int i = 0; i < selection.size(); ++i)
			selection[i].hide();
	}
	void move(int dx, int dy) // move all buttons
	{
		for (int i = 0; i < selection.size(); ++i)
			selection[i].move(dx, dy);
	}

	void attach(Window& win) // attach all buttons
	{
		for (int i = 0; i < selection.size(); ++i)
			win.attach(selection[i]);
		own = &win;
	}

private:
	void _attach(Button& b); // heavy lifting of attaching button to menu
};

//------------------------------------------------------------------------------

} // namespace Graph_lib
