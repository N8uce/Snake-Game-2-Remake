//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{

}
//������ ���� ����������� ����
void __fastcall TForm3::Button1Click(TObject *Sender)
{
Close();	
}
//��� �������� ���� ���������� ����� � ����
void __fastcall TForm3::FormCreate(TObject *Sender)
{
Form3->Color = RGB(239, 228, 176);
}
//---------------------------------------------------------------------------

