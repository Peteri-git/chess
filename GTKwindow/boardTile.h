#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
struct boardTile {
	bool hasFunc;
	bool beenMoved;
	std::string color;
	std::string figurine;
	Gtk::Button *button;
};