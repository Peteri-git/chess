#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <google\protobuf\repeated_field.h>
#include <gtkmm/listbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
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
	google::protobuf::RepeatedPtrField<GrpcGameService::GameTile> board;
protected:

	void on_button_clicked(Glib::ustring data);

	// Child widgets:
	Gtk::Box m_box1;
	Gtk::Button m_button1, m_button2;
	Gtk::Image king;// = Gtk::Image("C:\\Users\\Petr\\source\\repos\\GTKwindow\\Debug\\black_king.png");
};


