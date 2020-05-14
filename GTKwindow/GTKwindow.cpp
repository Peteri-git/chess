#include "helloworld.h"
#include <gtkmm/application.h>
#include "RoomWindow.h"
int main(int argc, char* argv[])
{
	auto app = Gtk::Application::create(argc, argv);

	HelloWorld helloworld;
	RoomWindow roomWindow;

	return app->run(roomWindow);
}