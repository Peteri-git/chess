#include "RoomWindow.h"
#include <iostream>
#include <iostream>
#include <grpc/grpc.h>
#include "GameWindow.h"
#include <gtkmm/application.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"

using namespace std;
using namespace grpc;
using namespace google::protobuf;
using namespace GrpcGameService;
using namespace Gtk;

RoomWindow::RoomWindow() : comboB(),join_button("Join Room!"),add_button("Add Room!") {
	auto ssl_options = grpc::SslCredentialsOptions();
	auto ssl = grpc::SslCredentials(ssl_options);
	auto channel = CreateChannel("wiki.kyberna.cz:5002", ssl);
	RoomService* rs = new RoomService();
	client = rs->NewStub(channel, grpc::StubOptions());
	set_title("RoomService");
	set_default_size(500, 10);
	set_resizable(false);
	add(m_box);
	m_box.pack_end(join_button);
	m_box.pack_end(add_button);
	m_box.set_margin_bottom(0);

	
	ClientContext ctx;
	RoomListRequest req;
	RoomListResponse resp;
	auto status = client->List(&ctx, req, &resp);
	if (status.ok()) {
		for (auto item : resp.rooms()) {
			int first = item.roomid();
			Room second = item;
			room_dic.insert(pair<int, string > (first, item.name().c_str()));
			comboB.append(item.name().c_str());
		}
	}
	comboB.set_active(1);
	
	add_button.signal_clicked().connect(sigc::mem_fun(*this, &RoomWindow::add_button_clicked));
	join_button.signal_clicked().connect(sigc::mem_fun(*this, &RoomWindow::join_button_clicked));
	
	m_box.add(comboB);
	m_box.add(textbox);
	textbox.show();
	join_button.show();
	comboB.show_all();
	add_button.show();
	m_box.show();
}
RoomWindow::~RoomWindow() {

}
void RoomWindow::add_button_clicked(){
	ClientContext ctx;
	RoomCreateRequest req;
	string foo = textbox.get_text().c_str();
	req.set_name(foo);
	RoomCreateResponse resp;
	auto status = client->Create(&ctx, req,&resp);
	if (status.ok())
	{
		Room tmp = resp.room();
		comboB.append(tmp.name().c_str());
	}
}
void RoomWindow::join_button_clicked() {
	ClientContext ctx;
	RoomJoinRequest req;
	for (auto const& x : room_dic)
	{
		string tmp = comboB.get_active_text().c_str();
		if (tmp == x.second)
		{
			Room* join = new Room();
			join->set_name(x.second);
			join->set_roomid(x.first);
			req.set_allocated_room(join);
		}
	}
	auto test = req.has_room();
	auto status = client->Join(&ctx, req);
	GameCommandResponse cmd;
	while (status->Read(&cmd))
	{
		if (cmd.has_start())
		{
			auto board =cmd.start().state().tiles();
			GameWindow* game = new GameWindow;
			game->board = board;
			game->show();
			break;
		}
		if (cmd.has_end())
		{

		}
		if (cmd.has_move())
		{

		}
	}
}