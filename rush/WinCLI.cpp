#include "mainForm.h"

using namespace rush;

[STAThreadAttribute]
int main(array<System::String^>^args)
{


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Application::Run(gcnew(mainForm));
	return 0;
}