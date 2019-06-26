//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uChat.h"
#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmChat *frmChat;
//---------------------------------------------------------------------------
__fastcall TfrmChat::TfrmChat(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmChat::btnSchliessenClick(TObject *Sender)
{
	frmChat->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmChat::btnSendenClick(TObject *Sender)
{
	try{
		sSend = txbChat->Text;
        txbChat->Text = "";
		frmMain->TCPClient->IOHandler->WriteLn("M"+sSend);
		mChat->Lines->Add("Du : " + sSend);
	}catch (Exception *e){
		ShowMessage("CHATFEHLER");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmChat::txbChatChange(TObject *Sender)
{
	if(txbChat->Text != "") btnSenden->Enabled = true;
	else btnSenden->Enabled = false;
}
//---------------------------------------------------------------------------

