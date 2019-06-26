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
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include "uSchiff.h"
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TGroupBox *grbOwn;
	TIdTCPClient *TCPClient;
	TGroupBox *grbSchiffe;
	TButton *btnReady;
	TLabel *lblGReady;
	TPanel *pnlGReady;
	TGroupBox *grbSEinstellungen;
	TLabel *lblHIP;
	TLabel *lblConnection;
	TEdit *txbHIP;
	TButton *btnConnect;
	TButton *btnDisconnect;
	TPanel *pnlConnection;
	TMainMenu *mMenu;
	TMenuItem *mChat;
	TMenuItem *mSpielregeln;
	TGroupBox *grbEnemy;
	TStringGrid *gridOwn;
	TStringGrid *gridEnemy;
	TPanel *pnlSchiff1;
	TPanel *pnlSchiff2;
	TPanel *pnlSchiff3;
	TPanel *pnlSchiff4;
	TPanel *pnlSchif1;
	TPanel *pnlSchif2;
	TPanel *pnlSchif3;
	TPanel *pnlSchif4;
	TLabel *lblASchiff;
	TLabel *lblSchiff;
	TButton *btnDelete;
	TRadioGroup *rgbSchiffslage;
	TTimer *tRead;
	TMemo *mLog;
	TLabel *lblSpiel;
	TPanel *pnlSpielen;
	TLabel *lblSpielen;
	void __fastcall btnConnectClick(TObject *Sender);
	void __fastcall mRegelnClick(TObject *Sender);
	void __fastcall mChatClick(TObject *Sender);
	void __fastcall btnReadyClick(TObject *Sender);
	void __fastcall gridOwnSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall createSchiff(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall TCPClientDisconnected(TObject *Sender);
	void __fastcall rgbSchiffslageClick(TObject *Sender);
	void __fastcall tReadTimer(TObject *Sender);
	void __fastcall gridEnemySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall TCPClientConnected(TObject *Sender);

    //void MessageRead(String sRx);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	bool bDelete, bDiscon;                               //Ob auf "Löschen" geklickt wurde
    int iFlip, iTreffer;                                  //Drehungsgrad
	int iSchiff1,iSchiff2,iSchiff3,iSchiff4;    //Anzahl der Schiffe pro Schiff
	String sPnlName;                            //Name vom angeklickten Panel
	kleinSchiff *kSchiff[4];                    //Objekte der Schiffsklasse
	mittelSchiff *mSchiff[3];
	grossSchiff *gSchiff[2];
	sgrossSchiff *sSchiff;
	String sMessage;                            //Text der vom Server kommt
	int isrow,iscol;                               //

	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
