//---------------------------------------------------------------------------

#ifndef uChatH
#define uChatH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TfrmChat : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *txbChat;
	TButton *btnSenden;
	TButton *btnSchliessen;
	TMemo *mChat;
	void __fastcall btnSchliessenClick(TObject *Sender);
	void __fastcall btnSendenClick(TObject *Sender);
	void __fastcall txbChatChange(TObject *Sender);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	String sMessage, sSend;
	__fastcall TfrmChat(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmChat *frmChat;
//---------------------------------------------------------------------------
#endif
