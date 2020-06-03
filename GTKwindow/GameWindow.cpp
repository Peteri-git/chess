#include "GameWindow.h"
#include <iostream>
#include <gtkmm/cssprovider.h>
using namespace std;
using namespace grpc;
using namespace google::protobuf;
using namespace GrpcGameService;
using namespace Gtk;
GameWindow::GameWindow() :start_button("show board")
{
	set_title("Chess");
	set_resizable(false);
	set_border_width(1);
	add(gridBox);
	start_button.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::ShowBoard));
	gridBox.attach(start_button, 0, 1, 1, 1);
	gridBox.show_all();
	dispatcher.connect(sigc::mem_fun(*this, &GameWindow::UpdateMoves));
}
GameWindow::~GameWindow()
{
}
void GameWindow::UpdateMoves()
{
	set_title("it did something");
	int oldX = lastMove.from().row();
	int oldY = lastMove.from().column();
	int newX = lastMove.to().row();
	int newY = lastMove.to().column();
	Glib::RefPtr<Gtk::CssProvider> css_white = Gtk::CssProvider::create();
	css_white->load_from_data("button {background-image: image(white);}");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			copy_board[i][j].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i == oldX && j == oldY)
			{
				Button* tile = new Button();
				gridBox.remove(*board[i][j].button);
				board[i][j].button = tile;
				gridBox.attach(*tile, j, i, 1, 1);
			}
		}
	}
	gridBox.show_all();
	string path = "D:\\GTKwindow\\Debug\\";
	string fig = board[oldX][oldY].figurine;
	string color = board[oldX][oldY].color;
	path.append(color);
	path.append("_");
	path.append(fig);
	path.append(".png");
	Image* figurine = new Image(path);
	Image* emptyImage = new Image();
	board[newX][newY].figurine = board[oldX][oldY].figurine;
	board[newX][newY].color = board[oldX][oldY].color;
	board[newX][newY].button->set_image(*figurine);
	board[newX][newY].button->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), board[newX][newY].figurine, board[newX][newY].color, newX, newY));
	board[oldX][oldY].figurine = "None";
	board[oldX][oldY].color = "NONE";
	board[oldX][oldY].button->set_image(*emptyImage);
	Turn();
}
void GameWindow::Promotion() 
{

}
void GameWindow::Turn()
{
	if (turn == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j].color == "WHITE")
				{
					Button* tile = new Button();
					string path = "D:\\GTKwindow\\Debug\\";
					string fig = board[i][j].figurine;
					string color = board[i][j].color;
					path.append(color);
					path.append("_");
					path.append(fig);
					path.append(".png");
					Image* figurine = new Image(path);
					tile->set_image(*figurine);
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					gridBox.attach(*tile, j, i, 1, 1);
				}
				if (board[i][j].color == "BLACK")
				{
					Button* tile = new Button();
					string path = "D:\\GTKwindow\\Debug\\";
					string fig = board[i][j].figurine;
					string color = board[i][j].color;
					path.append(color);
					path.append("_");
					path.append(fig);
					path.append(".png");
					Image* figurine = new Image(path);
					tile->set_image(*figurine);
					tile->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), fig, color, i, j));
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					gridBox.attach(*tile, j, i, 1, 1);
				}
			}
		}
		turn = 1;
	}
	else if (turn == 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j].color == "WHITE")
				{
					Button* tile = new Button();
					string path = "D:\\GTKwindow\\Debug\\";
					string fig = board[i][j].figurine;
					string color = board[i][j].color;
					path.append(color);
					path.append("_");
					path.append(fig);
					path.append(".png");
					Image* figurine = new Image(path);
					tile->set_image(*figurine);
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					tile->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), fig, color, i, j));
					gridBox.attach(*tile, j, i, 1, 1);
				}
				if (board[i][j].color == "BLACK")
				{
					Button* tile = new Button();
					string path = "D:\\GTKwindow\\Debug\\";
					string fig = board[i][j].figurine;
					string color = board[i][j].color;
					path.append(color);
					path.append("_");
					path.append(fig);
					path.append(".png");
					Image* figurine = new Image(path);
					tile->set_image(*figurine);
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					gridBox.attach(*tile, j, i, 1, 1);
				}
			}
		}
		turn = 0;
	}
	gridBox.show_all();
}
void GameWindow::SendMoves(int oldX,int oldY,int newX,int newY) 
{
	//status ok ale nefunguje
	ClientContext ctx;
	GameMoveRequest req;
	Position *from = new Position();
	Position *To = new Position();
	from->set_row(oldX);
	from->set_column(oldY);
	To->set_row(newX);
	To->set_column(newY);
	GameMove *gm = new GameMove();
	gm->set_allocated_from(from);
	gm->set_allocated_to(To);
	req.set_gameid(gameid);
	req.set_allocated_move(gm);
	GameMoveResponse res;
	auto status=client->Move(&ctx,req,&res);
	if (status.ok())
	{
		Glib::RefPtr<Gtk::CssProvider> css_white = Gtk::CssProvider::create();
		css_white->load_from_data("button {background-image: image(white);}");
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				board[i][j].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
			}
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (board[i][j].hasFunc == true)
				{
					Button* tile = new Button();
					if (board[i][j].figurine != "None")
					{
						string path = "D:\\GTKwindow\\Debug\\";
						string fig = board[i][j].figurine;
						string color = board[i][j].color;
						path.append(color);
						path.append("_");
						path.append(fig);
						path.append(".png");
						Image* figurine = new Image(path);
						tile->set_image(*figurine);
						tile->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), fig, color, i, j));
					}
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					gridBox.attach(*tile, j, i, 1, 1);

					board[i][j].hasFunc = false;
				}
				if (i == oldX && j == oldY)
				{
					Button* tile = new Button();
					gridBox.remove(*board[i][j].button);
					board[i][j].button = tile;
					gridBox.attach(*tile, j, i, 1, 1);
				}
			}
		}
		gridBox.show_all();
		string path = "D:\\GTKwindow\\Debug\\";
		string fig = board[oldX][oldY].figurine;
		string color = board[oldX][oldY].color;
		path.append(color);
		path.append("_");
		path.append(fig);
		path.append(".png");
		Image* figurine = new Image(path);
		Image* emptyImage = new Image();
		board[newX][newY].figurine = board[oldX][oldY].figurine;
		board[newX][newY].color = board[oldX][oldY].color;
		board[newX][newY].button->set_image(*figurine);
		board[newX][newY].button->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), board[newX][newY].figurine, board[newX][newY].color, newX, newY));
		board[oldX][oldY].figurine = "None";
		board[oldX][oldY].color = "NONE";
		board[oldX][oldY].button->set_image(*emptyImage);
		Turn();
	}
}
void GameWindow::ShowMoves(string figurine, string color,int x,int y)
{
	Glib::RefPtr<Gtk::CssProvider> css_red = Gtk::CssProvider::create();
	Glib::RefPtr<Gtk::CssProvider> css_white = Gtk::CssProvider::create();
	css_red->load_from_data("button {background-image: image(red);}");
	css_white->load_from_data("button {background-image: image(white);}");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j].hasFunc == true)
			{
				Button* tile = new Button();
				if (board[i][j].figurine != "None")
				{
					string path = "D:\\GTKwindow\\Debug\\";
					string fig = board[i][j].figurine;
					string color = board[i][j].color;
					path.append(color);
					path.append("_");
					path.append(fig);
					path.append(".png");
					Image* figurine = new Image(path);
					tile->set_image(*figurine);
					tile->signal_clicked().connect(sigc::bind<string, string, int, int>(sigc::mem_fun(*this, &GameWindow::ShowMoves), fig, color, i, j));
				}
				gridBox.remove(*board[i][j].button);
				board[i][j].button = tile;
				gridBox.attach(*tile, j, i, 1, 1);

				board[i][j].hasFunc = false;
			}
		}
	}
	gridBox.show_all();
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
	}
	if (figurine == "King")
	{
		if (color == "BLACK" && this->color == "BLACK")
		{
			if (x - 1 >= 0)
			{
				if (board[x - 1][y].color != "BLACK")
				{
					board[x - 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x - 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y));
					board[x - 1][y].hasFunc = true;
				}
				if (y - 1 < 8)
				{
					if (board[x - 1][y + 1].color != "BLACK")
					{
						board[x - 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y + 1));
						board[x - 1][y + 1].hasFunc = true;
					}
				}
				if (y - 1 >= 0)
				{
					if (board[x - 1][y - 1].color != "BLACK")
					{
						board[x - 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y - 1));
						board[x - 1][y - 1].hasFunc = true;
					}
				}
			}
			if (y - 1 >= 0)
			{
				if (board[x][y-1].color != "BLACK")
				{
					board[x][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - 1));
					board[x][y - 1].hasFunc = true;
				}
			}
			if (y + 1 < 8)
			{
				if (board[x][y + 1].color != "BLACK")
				{
					board[x][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + 1));
					board[x][y + 1].hasFunc = true;
				}
			}
			if (x + 1 <8)
			{
				if (board[x + 1][y].color != "BLACK")
				{
					board[x + 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x + 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y));
					board[x + 1][y].hasFunc = true;
				}
				if (y - 1 < 8)
				{
					if (board[x + 1][y + 1].color != "BLACK")
					{
						board[x + 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y + 1));
						board[x + 1][y + 1].hasFunc = true;
					}
				}
				if (y - 1 >= 0)
				{
					if (board[x + 1][y - 1].color != "BLACK")
					{
						board[x + 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y - 1));
						board[x + 1][y - 1].hasFunc = true;
					}
				}
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			if (x - 1 >= 0)
			{
				if (board[x - 1][y].color != "WHITE")
				{
					board[x - 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x - 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y));
					board[x - 1][y].hasFunc = true;
				}
				if (y - 1 < 8)
				{
					if (board[x - 1][y + 1].color != "WHITE")
					{
						board[x - 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y + 1));
						board[x - 1][y + 1].hasFunc = true;
					}
				}
				if (y - 1 >= 0)
				{
					if (board[x - 1][y - 1].color != "WHITE")
					{
						board[x - 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y - 1));
						board[x - 1][y - 1].hasFunc = true;
					}
				}
			}
			if (y - 1 >= 0)
			{
				if (board[x][y - 1].color != "WHITE")
				{
					board[x][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - 1));
					board[x][y - 1].hasFunc = true;
				}
			}
			if (y + 1 < 8)
			{
				if (board[x][y + 1].color != "WHITE")
				{
					board[x][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + 1));
					board[x][y + 1].hasFunc = true;
				}
			}
			if (x + 1 < 8)
			{
				if (board[x + 1][y].color != "WHITE")
				{
					board[x + 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x + 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y));
					board[x + 1][y].hasFunc = true;
				}
				if (y - 1 < 8)
				{
					if (board[x + 1][y + 1].color != "WHITE")
					{
						board[x + 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y + 1));
						board[x + 1][y + 1].hasFunc = true;
					}
				}
				if (y - 1 >= 0)
				{
					if (board[x + 1][y - 1].color != "WHITE")
					{
						board[x + 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y - 1));
						board[x + 1][y - 1].hasFunc = true;
					}
				}
			}
		}
	}
	if (figurine == "Queen")
	{
		if (color == "BLACK"&& this->color == "BLACK")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0)
				{
					if (board[x - i][y].color != "BLACK")
					{
						board[x - i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y));
						board[x - i][y].hasFunc = true;
						if (board[x - i][y].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8)
				{
					if (board[x + i][y].color != "BLACK")
					{
						board[x + i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y));
						board[x + i][y].hasFunc = true;
						if (board[x + i][y].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y - i >= 0)
				{
					if (board[x][y - i].color != "BLACK")
					{
						board[x][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - i));
						board[x][y - i].hasFunc = true;
						if (board[x][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y + i < 8)
				{
					if (board[x][y + i].color != "BLACK")
					{
						board[x][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + i));
						board[x][y + i].hasFunc = true;
						if (board[x][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y - i >= 0)
				{
					if (board[x - i][y - i].color != "BLACK")
					{
						board[x - i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y - i));
						board[x - i][y - i].hasFunc = true;
						if (board[x - i][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y + i < 8)
				{
					if (board[x - i][y + i].color != "BLACK")
					{
						board[x - i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y + i));
						board[x - i][y + i].hasFunc = true;
						if (board[x - i][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y - i >= 0)
				{
					if (board[x + i][y - i].color != "BLACK")
					{
						board[x + i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y - i));
						board[x + i][y - i].hasFunc = true;
						if (board[x + i][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y + i < 8)
				{
					if (board[x + i][y + i].color != "BLACK")
					{
						board[x + i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y + i));
						board[x + i][y + i].hasFunc = true;
						if (board[x + i][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0)
				{
					if (board[x - i][y].color != "WHITE")
					{
						board[x - i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y));
						board[x - i][y].hasFunc = true;
						if (board[x - i][y].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8)
				{
					if (board[x + i][y].color != "WHITE")
					{
						board[x + i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y));
						board[x + i][y].hasFunc = true;
						if (board[x + i][y].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y - i >= 0)
				{
					if (board[x][y - i].color != "WHITE")
					{
						board[x][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - i));
						board[x][y - i].hasFunc = true;
						if (board[x][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y + i < 8)
				{
					if (board[x][y + i].color != "WHITE")
					{
						board[x][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + i));
						board[x][y + i].hasFunc = true;
						if (board[x][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y - i >= 0)
				{
					if (board[x - i][y - i].color != "WHITE")
					{
						board[x - i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y - i));
						board[x - i][y - i].hasFunc = true;
						if (board[x - i][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y + i < 8)
				{
					if (board[x - i][x + i].color != "WHITE")
					{
						board[x - i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y + i));
						board[x - i][y + i].hasFunc = true;
						if (board[x - i][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y - i >= 0)
				{
					if (board[x + i][y - i].color != "WHITE")
					{
						board[x + i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y - i));
						board[x + i][y - i].hasFunc = true;
						if (board[x + i][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y + i < 8)
				{
					if (board[x + i][y + i].color != "WHITE")
					{
						board[x + i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y + i));
						board[x + i][y + i].hasFunc = true;
						if (board[x + i][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	if (figurine == "Rook")
	{
		if (color == "BLACK" && this->color == "BLACK")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0)
				{
					if (board[x-i][y].color !="BLACK")
					{
						board[x - i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y));
						board[x - i][y].hasFunc = true;
						if (board[x - i][y].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8)
				{
					if (board[x+i][y].color !="BLACK")
					{
						board[x + i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y));
						board[x + i][y].hasFunc = true;
						if (board[x + i][y].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y - i >= 0)
				{
					if (board[x][y-i].color !="BLACK")
					{
						board[x][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - i));
						board[x][y - i].hasFunc = true;
						if (board[x][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y + i < 8)
				{
					if (board[x][y+i].color !="BLACK")
					{
						board[x][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + i));
						board[x][y + i].hasFunc = true;
						if (board[x][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0)
				{
					if (board[x-i][y].color !="WHITE")
					{
						board[x - i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y));
						board[x - i][y].hasFunc = true;
						if (board[x - i][y].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8)
				{
					if (board[x+i][y].color !="WHITE")
					{
						board[x + i][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y));
						board[x + i][y].hasFunc = true;
						if (board[x + i][y].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y - i >= 0)
				{
					if (board[x][y-i].color !="WHITE")
					{
						board[x][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y - i));
						board[x][y - i].hasFunc = true;
						if (board[x][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (y + i < 8)
				{
					if (board[x][y+i].color !="WHITE")
					{
						board[x][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x, y + i));
						board[x][y + i].hasFunc = true;
						if (board[x][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	if (figurine == "Knight")
	{
		if (color == "BLACK" && this->color == "BLACK")
		{
			if (x - 1 >= 0 && y - 2 >= 0 && board[x - 1][y - 2].color != "BLACK")
			{
				board[x - 1][y - 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y - 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y - 2));
				board[x - 1][y - 2].hasFunc = true;
			}
			if (x - 1 >= 0 && y + 2 < 8 && board[x - 1][y + 2].color != "BLACK")
			{
				board[x - 1][y + 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y + 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y + 2));
				board[x - 1][y + 2].hasFunc = true;
			}
			if (x - 2 >= 0 && y - 1 >= 0 && board[x - 2][y - 1].color != "BLACK")
			{
				board[x - 2][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 2][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 2, y - 1));
				board[x - 2][y - 1].hasFunc = true;
			}
			if (x - 2 >= 0 && y + 1 < 8 && board[x - 2][y + 1].color != "BLACK")
			{
				board[x - 2][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 2][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 2, y + 1));
				board[x - 2][y + 1].hasFunc = true;
			}
			if (x + 1 < 8 && y - 2 >= 0 && board[x + 1][y - 2].color != "BLACK")
			{
				board[x + 1][y - 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y - 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y - 2));
				board[x + 1][y - 2].hasFunc = true;
			}
			if (x + 1 < 8 && y + 2 < 8 && board[x + 1][y + 2].color != "BLACK")
			{
				board[x + 1][y + 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y + 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y + 2));
				board[x + 1][y + 2].hasFunc = true;
			}
			if (x + 2 < 8 && y - 1 >= 0 && board[x + 2][y - 1].color != "BLACK")
			{
				board[x + 2][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 2][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 2, y - 1));
				board[x + 2][y - 1].hasFunc = true;
			}
			if (x + 2 < 8 && y + 1 >= 0 && board[x + 2][y + 1].color != "BLACK")
			{
				board[x + 2][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 2][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 2, y + 1));
				board[x + 2][y + 1].hasFunc = true;
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			if (x - 1 >= 0 && y - 2 >= 0 && board[x - 1][y - 2].color != "WHITE")
			{
				board[x - 1][y - 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y - 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y - 2));
				board[x - 1][y - 2].hasFunc = true;
			}
			if (x - 1 >= 0 && y + 2 < 8 && board[x - 1][y + 2].color != "WHITE")
			{
				board[x - 1][y + 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y + 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y + 2));
				board[x - 1][y + 2].hasFunc = true;
			}
			if (x - 2 >= 0 && y - 1 >= 0 && board[x - 2][y - 1].color != "WHITE")
			{
				board[x - 2][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 2][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 2, y - 1));
				board[x - 2][y - 1].hasFunc = true;
			}
			if (x - 2 >= 0 && y + 1 < 8 && board[x - 2][y + 1].color != "WHITE")
			{
				board[x - 2][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 2][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 2, y + 1));
				board[x - 2][y + 1].hasFunc = true;
			}
			if (x + 1 < 8 && y - 2 >= 0 && board[x + 1][y - 2].color != "WHITE")
			{
				board[x + 1][y - 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y - 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y - 2));
				board[x + 1][y - 2].hasFunc = true;
			}
			if (x + 1 < 8 && y + 2 < 8 && board[x + 1][y + 2].color != "WHITE")
			{
				board[x + 1][y + 2].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y + 2].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y + 2));
				board[x + 1][y + 2].hasFunc = true;
			}
			if (x + 2 < 8 && y - 1 >= 0 && board[x + 2][y - 1].color != "WHITE")
			{
				board[x + 2][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 2][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 2, y - 1));
				board[x + 2][y - 1].hasFunc = true;
			}
			if (x + 2 < 8 && y + 1 >= 0 && board[x + 2][y - 1].color != "WHITE")
			{
				board[x + 2][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 2][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 2, y + 1));
				board[x + 2][y + 1].hasFunc = true;
			}
		}
	}
	if (figurine == "Bishop")
	{
		if (color == "BLACK" && this->color == "BLACK")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y - i >= 0)
				{
					if (board[x-i][y-i].color != "BLACK")
					{
						board[x - i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y - i));
						board[x - i][y - i].hasFunc = true;
						if (board[x - i][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y + i < 8)
				{
					if (board[x-i][y+i].color != "BLACK")
					{
						board[x - i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y + i));
						board[x - i][y + i].hasFunc = true;
						if (board[x - i][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y - i >= 0)
				{
					if (board[x+i][y-i].color !="BLACK")
					{
						board[x + i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y - i));
						board[x + i][y - i].hasFunc = true;
						if (board[x + i][y - i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y + i < 8)
				{
					if (board[x+i][y+i].color!="BLACK")
					{
						board[x + i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y + i));
						board[x + i][y + i].hasFunc = true;
						if (board[x + i][y + i].color == "WHITE")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y - i >= 0)
				{
					if (board[x-i][y-i].color !="WHITE")
					{
						board[x - i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y - i));
						board[x - i][y - i].hasFunc = true;
						if (board[x - i][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x - i >= 0 && y + i < 8)
				{
					if (board[x-i][x+i].color != "WHITE")
					{
						board[x - i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - i, y + i));
						board[x - i][y + i].hasFunc = true;
						if (board[x - i][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y - i >= 0)
				{
					if (board[x+i][y-i].color != "WHITE")
					{
						board[x + i][y - i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y - i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y - i));
						board[x + i][y - i].hasFunc = true;
						if (board[x + i][y - i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			for (int i = 1; i < 8; i++)
			{
				if (x + i < 8 && y + i < 8)
				{
					if (board[x+i][y+i].color != "WHITE")
					{
						board[x + i][y + i].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + i][y + i].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + i, y + i));
						board[x + i][y + i].hasFunc = true;
						if (board[x + i][y + i].color == "BLACK")
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	if (figurine == "Pawn")
	{
		if (color == "BLACK" && this->color == "BLACK")
		{
			if (x == 6)
			{
				if (board[x - 1][y].figurine == "None" && x - 1 >= 0)
				{
					board[x - 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x - 1][y].hasFunc = true;
					board[x - 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y));
					if (board[x - 2][y].figurine == "None" && x - 2 >= 0)
					{
						board[x - 2][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x - 2][y].hasFunc = true;
						board[x - 2][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 2, y));
					}
				}
			}
			else if (x - 1 >= 0 && board[x - 1][y].figurine == "None")
			{
				board[x - 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y].hasFunc = true;
				board[x - 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y));
			}
			if (x - 1 >= 0 && y - 1 >= 0 && board[x - 1][y - 1].color == "WHITE")
			{
				board[x - 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y - 1].hasFunc = true;
				board[x - 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y - 1));
			}
			if (x - 1 >= 0 && y + 1 < 8 && board[x - 1][y + 1].color == "WHITE")
			{
				board[x - 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x - 1][y + 1].hasFunc = true;
				board[x - 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x - 1, y + 1));
			}
		}
		if (color == "WHITE" && this->color == "WHITE")
		{
			if (x == 1)
			{
				if (board[x + 1][y].figurine == "None" && x + 1 < 8)
				{
					board[x + 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
					board[x + 1][y].hasFunc = true;
					board[x + 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y));
					if (board[x+2][y].figurine == "None" && x + 2 < 8)
					{
						board[x + 2][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
						board[x + 2][y].hasFunc = true;
						board[x + 2][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 2, y));
					}
				}
			}
			else if (x + 1 < 8 && board[x + 1][y].figurine == "None")
			{
				board[x + 1][y].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y].hasFunc = true;
				board[x + 1][y].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y));
			}
			if (x + 1 < 8 && y - 1 >= 0 && board[x + 1][y - 1].color == "BLACK")
			{
				board[x + 1][y - 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y - 1].hasFunc = true;
				board[x + 1][y - 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y - 1));
			}
			if (x + 1 < 8 && y + 1 < 8 && board[x + 1][y + 1].color == "BLACK")
			{
				board[x + 1][y + 1].button->get_style_context()->add_provider(css_red, GTK_STYLE_PROVIDER_PRIORITY_USER);
				board[x + 1][y + 1].hasFunc = true;
				board[x + 1][y + 1].button->signal_clicked().connect(sigc::bind<int, int, int, int>(sigc::mem_fun(*this, &GameWindow::SendMoves), x, y, x + 1, y + 1));
			}
		}
	}
}
void GameWindow::ShowBoard()
{
	set_title(color.c_str());
	start_button.hide();
	gridBox.remove(start_button);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Button* tile = new Button();
			if (board[i][j].color == "WHITE")
			{
				string path = "D:\\GTKwindow\\Debug\\";
				string fig = board[i][j].figurine;
				string color = board[i][j].color;
				path.append(color);
				path.append("_");
				path.append(fig);
				path.append(".png");
				Image* figurine = new Image(path);
				tile->set_image(*figurine);
				tile->signal_clicked().connect(sigc::bind<string,string,int,int>(sigc::mem_fun(*this, &GameWindow::ShowMoves),fig,color,i,j));

			}
			if (board[i][j].color == "BLACK")
			{
				string path = "D:\\GTKwindow\\Debug\\";
				string fig = board[i][j].figurine;
				string color = board[i][j].color;
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
	Glib::RefPtr<Gtk::CssProvider> css_white = Gtk::CssProvider::create();
	css_white->load_from_data("button {background-image: image(white);}");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j].button->get_style_context()->add_provider(css_white, GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
	}
	gridBox.show_all();
}