#include <gtkmm/application.h>
#include "RoomWindow.h"
int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create(argc, argv);

	RoomWindow roomWindow;

	return app->run(roomWindow);
}