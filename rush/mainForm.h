#pragma once

#include "communication.h"
#include "Global.h"

namespace rush {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for mainForm
	/// </summary>
	public ref class mainForm : public System::Windows::Forms::Form
	{
	public:
		mainForm(void)
		{
			entryPtr = nullptr;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			entryPtr = new Communcation(0);
			pDataQueue = new CommandQueue(mainForm::formProcFunc);
			//
		}
		static int formProcFunc(pIQueueNode ptr)
		{
			wchar_t* wstr = (wchar_t*)(ptr->getValue());
			size_t len = wcslen(wstr);
			array<wchar_t>^ arr = gcnew array<wchar_t>(len);
			for(int i=0;i<len;i++)
			{
				arr[i] = wstr[i];
			}
			System::String^ str = gcnew System::String(arr);
			int i=0;
			return i;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~mainForm()
		{
			if (components)
			{
				delete components;
			}
			if(nullptr!=entryPtr)
			{
				delete entryPtr;
			}
			if(nullptr!=pDataQueue)
			{
				delete pDataQueue;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  textBoxSending;

	private: System::Windows::Forms::TextBox^  textBox2;
	protected: 
		pCommunication entryPtr;
		CommandQueue* pDataQueue;
	private: System::Windows::Forms::TextBox^  textBoxReceiving;
	protected: 
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBoxSending = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBoxReceiving = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(505, 433);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(76, 44);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &mainForm::button1_Click);
			// 
			// textBoxSending
			// 
			this->textBoxSending->Location = System::Drawing::Point(12, 12);
			this->textBoxSending->Multiline = true;
			this->textBoxSending->Name = L"textBoxSending";
			this->textBoxSending->Size = System::Drawing::Size(222, 341);
			this->textBoxSending->TabIndex = 1;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(12, 393);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(221, 21);
			this->textBox2->TabIndex = 2;
			this->textBox2->Enter += gcnew System::EventHandler(this, &mainForm::textBox2_Enter);
			this->textBox2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &mainForm::textBox2_KeyPress);
			// 
			// textBoxReceiving
			// 
			this->textBoxReceiving->Location = System::Drawing::Point(314, 15);
			this->textBoxReceiving->Multiline = true;
			this->textBoxReceiving->Name = L"textBoxReceiving";
			this->textBoxReceiving->Size = System::Drawing::Size(284, 337);
			this->textBoxReceiving->TabIndex = 3;
			// 
			// mainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(618, 514);
			this->Controls->Add(this->textBoxReceiving);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBoxSending);
			this->Controls->Add(this->button1);
			this->Name = L"mainForm";
			this->Text = L"mainForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			{
				Application::Exit();
			}
	private: System::Void textBox2_Enter(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(this->textBox2->Text->Length)
				 {
					 this->textBox2->Clear();
				 }
			 }

	private: System::Void textBox2_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
			 {
				 if(e->KeyChar == 13)
				 {
					if(this->textBox2->Text->Length)
					{
						entryPtr->SendAsyncMessage(this->textBox2->Text,(void*)(pDataQueue));
						
						this->textBoxSending->AppendText(this->textBox2->Text);
						this->textBoxSending->AppendText("\n");
						this->textBox2->Clear();
					}
				 }
			 }
};
}
