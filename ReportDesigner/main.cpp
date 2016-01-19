#include "reportdesigner.h"
#include "application.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);
	ReportDesigner w;
	w.show();
	return a.exec();
}
