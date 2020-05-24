#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm\grid.h>
#include <google\protobuf\repeated_field.h>
#include <gtkmm/listbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include "boardTile.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"

class GameWindow : public Gtk::Window
{
public:
	GameWindow();
	virtual ~GameWindow();
	boardTile board[8][8];
protected:
	void ShowBoard();
	void ShowMoves(std::string figurine, std::string color);
	void on_button_clicked(Glib::ustring data);

	Gtk::Grid gridBox;
	Gtk::Button m_button1, m_button2, m_button3;
	Gtk::Image king;
};


