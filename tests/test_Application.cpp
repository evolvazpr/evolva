#include "../Application.hpp"

int main(int argc, char *argv[])
{
	Application* app = Application::GetInstance(argc, argv);
	app->Init(1);
	app->exec();
	delete(app);
	return 0;
}
