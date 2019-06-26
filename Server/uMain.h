//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdContext.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TIdTCPServer *TCPServer;
	TButton *btnSStart;
	TButton *btnSStop;
	TLabel *lblStatus;
	TPanel *pnlStatus;
	TLabel *lblConnections;
	TLabel *lblReady;
	TMemo *mLog;
	TLabel *lblIP;
	void __fastcall btnSStartClick(TObject *Sender);
	void __fastcall btnSStopClick(TObject *Sender);
	void __fastcall TCPServerExecute(TIdContext *AContext);
	void __fastcall TCPServerConnect(TIdContext *AContext);
	void __fastcall TCPServerDisconnect(TIdContext *AContext);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	int iConnections, iReady, icount;
	bool bInit, bStart;
    TList *ClientList;
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
