#include "../Application.hpp"

int main(int argc, char *argv[])
{
	Application* app = Application::GetInstance(argc, argv);
	app->Init();
	app->exec();
	delete(app);
	return 0;
}
