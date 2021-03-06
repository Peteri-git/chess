#include "RoomWindow.h"
#include <iostream>
#include <future>
#include <grpc/grpc.h>
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
	//m_box.set_margin_bottom(0);

	
	ClientContext ctx;
	RoomListRequest req;
	RoomListResponse resp;
	auto status = client->List(&ctx, req, &resp);
	if (status.ok()) {
		for (auto item : resp.rooms()) {
			int first = item.roomid();
			Room second = item;
			room_dic.insert(pair<int, string > (first, item.name().c_str()));
			string tmp = item.name().c_str();
			auto count = item.playercount();
			comboB.append(item.name().c_str());
		}
	}
	//comboB.set_active(1);
	
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
void RoomWindow::listen() {
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
	auto status = client->Join(&ctx, req);
	GameCommandResponse cmd;
	string victoryColor;
	while (status->Read(&cmd))
	{
		if (cmd.has_start())
		{
			auto color = cmd.start().color();
			auto board = cmd.start().state().tiles();
			auto gameid = cmd.start().gameid();


			game->client = client;
			game->gameid = gameid;
			game->color = GrpcGameService::Color_Name(color);

			for (auto const& item : board) {
				boardTile fgt;
				fgt.color = GrpcGameService::Color_Name(item.figurinecolor());
				fgt.figurine = GrpcGameService::Figurine_Name(item.figurine());
				game->board[item.position().row()][item.position().column()] = fgt;
			}
			for (int i = 2; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					boardTile fgt;
					fgt.color = "NONE";
					fgt.figurine = "None";
					fgt.hasFunc = false;
					game->board[i][j] = fgt;
				}
			}
		}
		if (cmd.has_move())
		{
			if (game->board[cmd.move().to().row()][cmd.move().to().column()].figurine == "King")
			{
				break;
				//victoryColor = game->board[cmd.move().from().row()][cmd.move().from().column()].color;
			}
			else
			{
				game->lastMove = cmd.move();
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if ((cmd.move().from().row() == 6 && cmd.move().to().row() == 7) || (cmd.move().from().row() == 1 && cmd.move().to().row() == 0))
						{
							if (game->board[cmd.move().from().row()][cmd.move().from().column()].figurine == "Pawn")
							{
								game->UpdateFigurine = "Queen";
							}
						}
						if ((cmd.move().from().row() == 7 && cmd.move().from().column() == 4 && cmd.move().to().row() == 7 && cmd.move().to().column() == 2 && game->board[cmd.move().from().row()][cmd.move().from().column()].figurine == "King") ||
							(cmd.move().from().row() == 7 && cmd.move().from().column() == 4 && cmd.move().to().row() == 7 && cmd.move().to().column() == 6 && game->board[cmd.move().from().row()][cmd.move().from().column()].figurine == "King") ||
							(cmd.move().from().row() == 0 && cmd.move().from().column() == 4 && cmd.move().to().row() == 0 && cmd.move().to().column() == 2 && game->board[cmd.move().from().row()][cmd.move().from().column()].figurine == "King") ||
							(cmd.move().from().row() == 0 && cmd.move().from().column() == 4 && cmd.move().to().row() == 0 && cmd.move().to().column() == 6 && game->board[cmd.move().from().row()][cmd.move().from().column()].figurine == "King"))
						{
							game->castling = true;
						}
					}
				}
				game->dispatcher();
			}
		}
	}
	game->close();
	/*victoryColor.append(" won!");
	set_title(victoryColor);*/
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
		room_dic.insert(pair<int, string>(tmp.roomid(), tmp.name().c_str()));
		comboB.append(tmp.name().c_str());
		textbox.set_text("");
	}
}
void RoomWindow::join_button_clicked() {
	std::thread t1(&RoomWindow::listen, this);
	t1.detach();
	game->show();
}