#include <QSharedPointer>
#include "../Application.hpp"
int main(int argc, char *argv[])
{
	QSharedPointer<Application> app(Application::GetInstance(argc, argv));
	app->Init(1);
	app->exec();
	return 0;
}
