#include "GameWindow.h"
#include <iostream>
using namespace std;
using namespace Gtk;
GameWindow::GameWindow() :m_button2("show board")
{
	set_title("Chess");
	set_border_width(10);
	/*set_default_size(1920,1080);*/
	
	//fullscreen();
	add(gridBox);
	m_button1.show();
	king.show();

	m_button2.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ShowBoard));
	add(m_button2);
	gridBox.attach(m_button2, 0, 1, 1, 1);
	gridBox.show_all();
}

GameWindow::~GameWindow()
{
}

void GameWindow::on_button_clicked(Glib::ustring data)
{
	std::cout << "Hello World - " << data << " was pressed" << std::endl;
}
void GameWindow::ShowMoves(string figurine, string color)
{

}
void GameWindow::ShowBoard()
{
	m_button2.hide();
	gridBox.remove(m_button2);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Button* tile = new Button();
			if (GrpcGameService::Figurine_Name(board[i][j].figurine) != "NONE")
			{
				string path = "D:\\GTKwindow\\Debug\\";
				string fig = GrpcGameService::Figurine_Name(board[i][j].figurine);
				string color = GrpcGameService::Color_Name(board[i][j].color);
				path.append(color);
				path.append("_");
				path.append(fig);
				path.append(".png");
				Image* figurine = new Image(path);
				tile->set_image(*figurine);
			}
			board[i][j].button = tile;
			gridBox.attach(*tile, j, i, 1, 1);
		}
	}
	gridBox.show_all();
}