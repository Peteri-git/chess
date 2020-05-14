#include "GameWindow.h"
#include <iostream>
using namespace Gtk;
GameWindow::GameWindow() : m_button1("Button 1"), m_button2("Button 2"), king("C:\\Users\\Petr\\source\\repos\\GTKwindow\\Debug\\black_king.png")
{
	set_title("Chess");
	set_border_width(10);
	add(m_box1);
	m_button1.set_image(king);
	m_button1.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &GameWindow::on_button_clicked), "button 1"));

	
	m_box1.pack_start(m_button1);

	
	m_button1.show();
	king.show();

	m_button2.signal_clicked().connect(sigc::bind<-1, Glib::ustring>(sigc::mem_fun(*this, &GameWindow::on_button_clicked), "button 2"));

	m_box1.pack_start(m_button2);

	
	m_button2.show();
	m_box1.show();
}

GameWindow::~GameWindow()
{
}

void GameWindow::on_button_clicked(Glib::ustring data)
{
	std::cout << "Hello World - " << data << " was pressed" << std::endl;
}