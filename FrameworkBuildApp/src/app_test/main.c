
#include <CApplication.h>

#include "../app_test/CCityDemo.h"

int32 main(int32 argc, const char ** argv)
{
	CApplication app;

	if (app.initialize(argc, argv))
	{
		CCityDemo * demo = new CCityDemo;

		app.run(demo);

		app.shutdown();

		delete demo;
	}
	
}
