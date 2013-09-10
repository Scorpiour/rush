#include "mainForm.h"

using namespace rush;
using namespace System::Runtime::InteropServices;


[STAThreadAttribute]
int main(array<System::String^>^args)
{


	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	mainForm^ myForm = gcnew mainForm;
	
	int obj = (GCHandle::ToIntPtr(GCHandle::Alloc(myForm))).ToInt32();

	myForm->updateThisPtr(obj);

	Application::Run(myForm);
	return 0;
}