#include "grpc/GameService.pb.h"
#include "grpc/GameService.grpc.pb.h"
struct boardTile {
	/*GrpcGameService::Color color;
	GrpcGameService::Figurine figurine;*/
	std::string color;
	std::string figurine;
	Gtk::Button *button;
};