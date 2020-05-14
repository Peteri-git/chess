#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class HelloWorld : public Gtk::Window
{
public:
	HelloWorld();
	virtual ~HelloWorld();
protected:

	void on_button_clicked(Glib::ustring data);

	// Child widgets:
	Gtk::Box m_box1;
	Gtk::Button m_button1, m_button2;
	Gtk::Image king;// = Gtk::Image("C:\\Users\\Petr\\source\\repos\\GTKwindow\\Debug\\black_king.png");
};

#endif 
