//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
void MessageFilter(String sCmd, String sText, TIdContext *Context){
	try{
		if(sCmd == "B"){
			if(sText == "B1" && frmMain->iReady<2){
				frmMain->iReady++;
			}else if(sText == "B0" && frmMain->iReady>0){
				frmMain->iReady--;
			}
		}else if(sCmd == "C"){
			//Client X schickt spalte
			//Client Y addressieren und
			//splate und row an Client Y schicken
			if(Context == (TIdContext*) frmMain->ClientList->Items[1]){
				Context = (TIdContext*) frmMain->ClientList->Items[0];    			//Client Connection setzen
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}
			Context->Connection->IOHandler->WriteLn(sText);             			//zu client schreiben
		}else if(sCmd == "R"){
			//Client X schickt row
			//row an Client Y schicken
			if(Context == (TIdContext*) frmMain->ClientList->Items[1]){
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}
			Context->Connection->IOHandler->WriteLn(sText);
		}else if(sCmd == "T"){
			//Client Y schickt T
			//Client Y Y senden
			//Client X addressieren
			//Client X "T" senden
			Context->Connection->IOHandler->WriteLn("Y");

			if(Context == (TIdContext*) frmMain->ClientList->Items[0]){
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}
			Context->Connection->IOHandler->WriteLn(sText);
		}else if(sCmd == "V"){
			//Gleich wie bei "T" nur mit V = versenkt
			Context->Connection->IOHandler->WriteLn("Y");

			if(Context == (TIdContext*) frmMain->ClientList->Items[0]){
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}

			Context->Connection->IOHandler->WriteLn(sText);
		}else if(sCmd == "W"){
			//Gleich wie bei "T" nur mit W = Wasser
			Context->Connection->IOHandler->WriteLn("Y");

			if(Context == (TIdContext*) frmMain->ClientList->Items[0]){
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}

			Context->Connection->IOHandler->WriteLn(sText);
		}else if(sCmd == "K"){
			//Gleiche wie bei "T" nur mit K = Keine Schiffe mehr
			Context->Connection->IOHandler->WriteLn("L");

			if(Context == (TIdContext*) frmMain->ClientList->Items[0]){
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}

			Context->Connection->IOHandler->WriteLn("G");
		}else if(sCmd == "M"){
            //Chat nachrichten
            if(Context == (TIdContext*) frmMain->ClientList->Items[0]){
				Context = (TIdContext*) frmMain->ClientList->Items[1];
			}else{
				Context = (TIdContext*) frmMain->ClientList->Items[0];
			}
			Context->Connection->IOHandler->WriteLn(sText);
		}
	}catch(Exception *e){
		ShowMessage("FEHLER BEIM FILTERN!");
	}
}

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	bInit=false;
	bStart=false;                                                      			//Startvariable
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSStartClick(TObject *Sender)
{
	try{
		TCPServer->Active = true;           									//Server starten
		if(TCPServer->Active) pnlStatus->Color = clLime;        				//Wenn Server Aktiv dann Status setzen
        ClientList = new TList;
	}catch(Exception *e){
		ShowMessage("Fehler beim aktivieren des Servers");      				//Bei Fehler -> Meldung ausgeben
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnSStopClick(TObject *Sender)
{
	try{
		if(TCPServer->Active) TCPServer->Active = false;    					//Bei Stop klick Server stoppen wenn Aktiv ist
		if(bInit) delete ClientList;
		bInit=false;
		bStart=false;
		pnlStatus->Color = clRed;
		iConnections=0;
		iReady=0;
		mLog->Clear();
		lblConnections->Caption = "Connected : 0";
		lblReady->Caption = "Ready : 0";
	}catch(Exception *e){
		ShowMessage("Fehler beim Stoppen");                 					//Bei Fehler meldung ausgeben
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TCPServerExecute(TIdContext *AContext)
{
	try{
		String sMessage = AContext->Connection->IOHandler->ReadLn();        	//Wenn Server nachricht bekommt dann lesen
		mLog->Lines->Add(sMessage);
		String sSwitch = sMessage.SubString(0,1);

		//Wenn einer der Nachrichten kommt
		if(sSwitch == "B" || sSwitch == "C" || sSwitch == "R" || sSwitch == "T" || sSwitch == "V" || sSwitch == "W" || sSwitch == "K" || sSwitch == "M"){
			MessageFilter(sSwitch, sMessage, AContext);                         //Dann Funktion aufrufen
		}

		if(!bStart){                                                            //Wenn spiel noch nicht gestarte ist
			if(iReady==2){                                                      //Wenn 2 Clients Ready dann
				if(!bInit){                                                     //Clientanzahl noch nicht initialisiert
					ClientList = TCPServer->Contexts->LockList();            	//Client anzahl holen
					bInit=true;
				}

				for(icount=0;icount < ClientList->Count ; icount++){            //An jeden Client ready senden
					AContext = (TIdContext*) ClientList->Items[icount];         //Client Connection setzen
					AContext->Connection->IOHandler->WriteLn("B1");             //zu client schreiben
				}
				AContext->Connection->IOHandler->WriteLn("Y");                  //zu client schreiben
				bStart=true;                                                    //Ready wwurde schon geschickt
			}
		}

		lblReady->Caption = "Ready : " + IntToStr(iReady);

	}catch(Exception *e){
		ShowMessage("Fehler beim Lesen/Schreiben!");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::TCPServerConnect(TIdContext *AContext)
{
	//Connections + 1
	iConnections++;
	lblConnections->Caption = "Connected : " + IntToStr(iConnections);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TCPServerDisconnect(TIdContext *AContext)
{
	iConnections--;
    if(iReady>0) iReady--;
	lblConnections->Caption = "Connected : " + IntToStr(iConnections);
	lblReady->Caption = "Ready : " + IntToStr(iReady);
	if(bInit){
		delete ClientList;
		bInit=false;
		bStart=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(MessageDlg("Wirklich beenden ?", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel ,0) == mrCancel){
		CanClose = false;
	}
}
//---------------------------------------------------------------------------

