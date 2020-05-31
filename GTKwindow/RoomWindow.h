#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/listbox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/entry.h>
#include <thread>
#include <grpc/grpc.h>
#include "GameWindow.h"
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
using namespace Gtk;
using namespace std;
class RoomWindow : public Window
{
public:
	RoomWindow();
	virtual ~RoomWindow();
protected:
	void add_button_clicked();
	GameWindow* game = new GameWindow();
	void join_button_clicked();
	void listen();
	Box m_box;
	ComboBoxText comboB;
	Button join_button;
	Button add_button;
	Entry textbox;
	shared_ptr<GrpcGameService::RoomService::Stub> client;
	std::map<int, string> room_dic;
};
