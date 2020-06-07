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
#include <glibmm.h>
#include <gtkmm\messagedialog.h>
#include <grpcpp/security/credentials.h>

#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"

class GameWindow : public Gtk::Window
{
public:
	GameWindow();
	virtual ~GameWindow();
	Glib::Dispatcher dispatcher;
	GrpcGameService::GameMove lastMove;
	boardTile board[8][8];
	std::string color;
	std::string UpdateFigurine = "";
	std::shared_ptr<grpc::ClientReader<GrpcGameService::GameCommandResponse>> status;
	std::shared_ptr<GrpcGameService::RoomService::Stub> client;
	std::basic_string<char, std::char_traits<char>, std::allocator<char>> gameid;
	
protected:
	void ShowBoard();
	void UpdateMoves();
	boardTile copy_board[8][8];
	void Promotion();
	void Turn();
	void SendMoves(int oldX, int oldY, int newX, int newY, std::string figurine);
	void ShowMoves(std::string figurine, std::string color,int x, int y);
	int turn = 0;
	Gtk::Grid gridBox;
	Gtk::MessageDialog* msgBox = new Gtk::MessageDialog("The game ended!");
	Gtk::Button start_button;
	Gtk::Image king;
};


