//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
#include "uChat.h"
#include "uSchiff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------

void FillCell(TStringGrid* Grid, int Col, int Row, TColor BColor){
  String sCell;
  TRect Rect;
  Rect = Grid->CellRect(Col, Row);
  Grid->Canvas->Brush->Color = BColor;
  Grid->Canvas->FillRect(Rect);
  Grid->Canvas->Font->Color = clBlack;
  sCell = Grid->Cells[Col][Row];
  Grid->Canvas->TextOutW(Rect.Left + 2, Rect.Top + 2, sCell);
}

void MessageRead(String sRx){
	String sR = sRx.SubString(0,1);
	int ileng=0;

	if(sR == "B"){     		//Gegner Ready
		if(sRx == "B1"){
			frmMain->pnlGReady->Color = clLime;
			frmMain->mLog->Lines->Add("Gegner ist bereit!");
		}else{
			frmMain->pnlGReady->Color = clRed;
		}
	}else if(sR == "L"){	//Spiel verloren
		frmMain->mLog->Lines->Add("Du hast verloren!");
		ShowMessage("Du hast verloren!");
		frmMain->gridOwn->Enabled = false;               //Spielfeld disablen
		frmMain->gridEnemy->Enabled = false;
		frmMain->pnlGReady->Color = clRed;             	//Gegner nciht ready
		frmMain->pnlSpielen->Color = clRed;
		//Application->MessageBoxW(L"Gewonnen!\nNeues Spiel ?",L"Spiel beendet",MB_YESNO);
	}else if(sR == "G"){	//Spiel verloren
		frmMain->mLog->Lines->Add("Du hast gewonnen");
		ShowMessage("Du hast gewonnen!");
		frmMain->gridOwn->Enabled = false;               //Spielfeld disablen
		frmMain->gridEnemy->Enabled = false;
		frmMain->pnlGReady->Color = clRed;             	//Gegner nciht ready
		frmMain->pnlSpielen->Color = clRed;
		//Application->MessageBoxW(L"Gewonnen!\nNeues Spiel ?",L"Spiel beendet",MB_YESNO);
	}else if(sR == "C"){	//spalte
		frmMain->mLog->Lines->Add(sRx);
		ileng = sRx.Length();                       //Länge vom Text
		sR = sRx.SubString(2, ileng);               //Spalte rausholen
		frmMain->iscol = StrToInt(sR);                        //In Int konventieren
	}else if(sR == "R"){  	//Reihe
		frmMain->mLog->Lines->Add(sRx);
		ileng = sRx.Length();
		sR = sRx.SubString(2, ileng);
		frmMain->isrow = StrToInt(sR);
		if(frmMain->gridOwn->Cells[frmMain->iscol][frmMain->isrow] == "X"){
			frmMain->TCPClient->IOHandler->WriteLn("T");
            FillCell(frmMain->gridOwn, frmMain->iscol, frmMain->isrow, clRed);
			frmMain->iTreffer++;
			if(frmMain->iTreffer == 20){
				frmMain->TCPClient->IOHandler->WriteLn("K");                    //Keine Schiffe mehr
			}
		}else{
			frmMain->TCPClient->IOHandler->WriteLn("W");
			FillCell(frmMain->gridOwn, frmMain->iscol, frmMain->isrow, clAqua);
		}
	}else if(sR == "Y"){   	//Client ist an der Reihe
		frmMain->mLog->Lines->Add("Du bist dran");
		frmMain->pnlSpielen->Color = clLime;
		frmMain->isrow=0;
        frmMain->iscol=0;
		frmMain->gridEnemy->Enabled = true;
	}else if(sR == "T"){	//Treffer
		frmMain->mLog->Lines->Add("Treffer!");
		frmMain->gridEnemy->Cells[frmMain->iscol][frmMain->isrow] = "O";
		FillCell(frmMain->gridEnemy,frmMain->iscol, frmMain->isrow, clRed);
	}else if(sR == "V"){   	//Schiff versenkt
		frmMain->mLog->Lines->Add("Schiff versenkt!");
	}else if(sR == "W"){   	//Wasser getroffen
		frmMain->mLog->Lines->Add("Wasser getroffen du opfer");
		frmMain->gridEnemy->Cells[frmMain->iscol][frmMain->isrow] = "W";
		FillCell(frmMain->gridEnemy, frmMain->iscol, frmMain->isrow, clAqua);
	}else if(sR == "M"){
		ileng = sRx.Length();
		String sIncome;
		sIncome = sRx.SubString(2,ileng);
		frmChat->mChat->Lines->Add("Gegner : " + sIncome);
        ShowMessage("Neue Nachricht");
	}
}

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	iSchiff1=4;         //Anzahl von Schiffen
	iSchiff2=3;
	iSchiff3=2;
	iSchiff4=1;
	pnlSchif1->Caption = IntToStr(iSchiff1);    //Anzahl anzeigen
	pnlSchif2->Caption = IntToStr(iSchiff2);
	pnlSchif3->Caption = IntToStr(iSchiff3);
	pnlSchif4->Caption = IntToStr(iSchiff4);
	iFlip=0;                                    //Schiffslage auf horizontal = 0 vertikal = 1
	bDiscon=false;
	iscol = 0;
	isrow = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnConnectClick(TObject *Sender)
{
	TCPClient->Host = txbHIP->Text;             //eingeben IP Adresse nehemen
	try{
		TCPClient->Connect();                   //mit Server verbinden
	}catch(Exception *e){
		//Ansonst Fehlermeldung
		pnlConnection->Color = clRed;
		ShowMessage("Fehler beim Verbinden mit Server!\nKorrekte IP?");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mRegelnClick(TObject *Sender)
{
	//Regelfenster anzeigen
	Application->MessageBoxW(L" - Die Schiffe dürfen nicht aneinander stoßen. \n - Die Schiffe dürfen nicht über Eck gebaut sein oder Ausbuchtungen besitzen. \n - Die Schiffe dürfen auch am Rand liegen. \n - Die Schiffe dürfen nicht diagonal aufgestellt werden. \n - Jeder verfügt über insgesamt zehn Schiffe (in Klammern die Größe): \n \t - ein Schlachtschiff (4 Kästchen) \n \t - zwei Kreuzer (je 3 Kästchen) \n \t - drei Zerstörer (je 2 Kästchen) \n \t - vier U-Boote (je 1 Kästchen)",L"Spielregeln",MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mChatClick(TObject *Sender)
{
	//Chatfenster öffnen
	try{
		frmChat->Show();
	}catch(Exception *e){   //Bei Exception fehler ausgeben
		ShowMessage("Fehler beim Öffnen des Chats");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnReadyClick(TObject *Sender)
{
	//Bei Readyklick prüfen ob alle Schiffe gelegt sind
	if(iSchiff1==0 && iSchiff2==0 && iSchiff3==0 && iSchiff4==0){
		//Wenn alle Schiffe gelegt checken ob User wirklich ready ist
		if(MessageDlg("Bereit ?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo,0) == mrYes){
			gridOwn->Enabled = false;                                   //Spielfeld ausschalten
			btnReady->Enabled = false;                                  //Buttons disablen
			rgbSchiffslage->Enabled = false;
			btnDelete->Enabled = false;
			TCPClient->IOHandler->WriteLn("B1");            //Serrver text schreiben
		}
	}else{
		//Ansonst fehlermeldung ausgeben
		ShowMessage("Fehler beim Schreiben!");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::gridOwnSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	int icount=0;           //Laufvariabel
	if(bDelete){		 	//Schiff löschen
		if(gridOwn->Cells[ACol][ARow] == "X"){  //Wenn Angeklickte Zelle Schiff enthält
			for(icount=0;icount<4;icount++){
				if(kSchiff[icount]->iCol == ACol && kSchiff[icount]->iRow == ARow){
					for(int icoun=0;icoun<kSchiff[icount]->iGross;icoun++){
						if(kSchiff[icount]->iLage == 0){
							gridOwn->Cells[ACol+icoun][ARow] = "";
						}else{
							gridOwn->Cells[ACol][ARow+icoun] = "";
						}
					}
                    kSchiff[icount]->bSet = false;
					kSchiff[icount]->iRow = 0;
					kSchiff[icount]->iCol = 0;
					iSchiff1++;
					break;
				}
				if(icount<3){
					if(mSchiff[icount]->iCol == ACol && mSchiff[icount]->iRow == ARow){
						for(int icoun=0;icoun<mSchiff[icount]->iGross;icoun++){
							if(mSchiff[icount]->iLage == 0){
								gridOwn->Cells[ACol+icoun][ARow] = "";
							}else{
								gridOwn->Cells[ACol][ARow+icoun] = "";
							}
						}
						mSchiff[icount]->bSet = false;
						mSchiff[icount]->iRow = 0;
						mSchiff[icount]->iCol = 0;
						iSchiff2++;
						break;
					}
				}
				if(icount<2){
					if(gSchiff[icount]->iCol == ACol && gSchiff[icount]->iRow == ARow){
						for(int icoun=0;icoun<gSchiff[icount]->iGross;icoun++){
							if(gSchiff[icount]->iLage == 0){
								gridOwn->Cells[ACol+icoun][ARow] = "";
							}else{
								gridOwn->Cells[ACol][ARow+icoun] = "";
							}
						}
						gSchiff[icount]->bSet = false;
						gSchiff[icount]->iRow = 0;
						gSchiff[icount]->iCol = 0;
						iSchiff3++;
						break;
					}
				}
				if(icount<1){
					if(sSchiff->iCol == ACol && sSchiff->iRow == ARow){
						for(int icoun=0;icoun<sSchiff->iGross;icoun++){
							if(sSchiff->iLage == 0){
								gridOwn->Cells[ACol+icoun][ARow] = "";
							}else{
								gridOwn->Cells[ACol][ARow+icoun] = "";
							}
						}
						sSchiff->bSet = false;
						sSchiff->iRow = 0;
						sSchiff->iCol = 0;
						iSchiff4++;
						break;
					}
				}
			}
		}
		bDelete=false;
		gridOwn->Cursor = crDefault;
	}else{                      //Ansonst Schiff setzen
		if(sPnlName=="Klein"){              //Kleines Schiff setzen
			if(iSchiff1>0){                 //Prüfen ob Schiffe vorhanden
				for(icount=0;icount<4;icount++){            //Alle Schiffobjekte prüfen ob gesetzt sind
					if(!(kSchiff[icount]->bSet)){           //Wenn nicht gesetztes Schiff gefunden wurde, dass setzten
						if(kSchiff[icount]->setSchiff(ACol, ARow, 1, iFlip)){       //Funktion für Schiff setzten in uSchiff.cpp
							kSchiff[icount]->bSet = true;                           //Objekt Schiffgesetzt auf True
							kSchiff[icount]->iLage = iFlip;
							iSchiff1--;                                             //Schiffanzahl verringer
						}
						break;
					}
				}
			}

		}else if(sPnlName=="Mittel"){       //Mittleres Schiff setzen
			if(iSchiff2>0){
				for(icount=0;icount<3;icount++){
					if(!(mSchiff[icount]->bSet)){
						if(mSchiff[icount]->setSchiff(ACol, ARow, 2, iFlip)){
							mSchiff[icount]->bSet = true;
							mSchiff[icount]->iLage = iFlip;
							iSchiff2--;
						}
						break;
					}
				}
			}

		}else if(sPnlName=="Groß"){             //Großes Schiff setzten
			if(iSchiff3>0){
				for(icount=0;icount<2;icount++){
					if(!(gSchiff[icount]->bSet)){
						if(gSchiff[icount]->setSchiff(ACol, ARow, 3, iFlip)){
							gSchiff[icount]->bSet=true;
							gSchiff[icount]->iLage = iFlip;
							iSchiff3--;
						}
						break;
					}
				}
			}

		}else if(sPnlName=="Sehr Groß"){        //Sehr Großes schiff setzen
			if(iSchiff4){
				if(sSchiff->setSchiff(ACol, ARow, 4, iFlip)){
					sSchiff->bSet=true;
                    sSchiff->iLage = iFlip;
					iSchiff4--;
				}
			}

		}
	}

	pnlSchif1->Caption = IntToStr(iSchiff1);        //Schiffanzahl aktualisieren
	pnlSchif2->Caption = IntToStr(iSchiff2);
	pnlSchif3->Caption = IntToStr(iSchiff3);
	pnlSchif4->Caption = IntToStr(iSchiff4);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	//Beim Schließen der Anwendung User fragen
	if (MessageDlg("Wirklich beenden ?", mtConfirmation, TMsgDlgButtons() << mbOK << mbCancel,0) == mrCancel){
		CanClose = false;                   	//Bei Cancel Programm nicht schließen
	}else{
		TCPClientDisconnected(btnDisconnect);   //Bevor Programm schließt Client disconnecten und alles reseten
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::createSchiff(TObject *Sender)
{
	TPanel *pnlCode = static_cast<TPanel*>(Sender);     //Wenn man auf Panel klickt Object benutzen
	sPnlName = pnlCode->Caption;                        //Namen von dem Objekt
	lblSchiff->Caption = sPnlName;                      //Ausgewähltes Schiff anzeigen
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnDeleteClick(TObject *Sender)
{
	if(bDelete){
		bDelete=false;
		//Mauszeiger reseten
		gridOwn->Cursor = crDefault;
	}else{
		bDelete=true;
		//Mauszeiger wechseln
		gridOwn->Cursor = crNo;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TCPClientDisconnected(TObject *Sender)
{
	try{
		if(TCPClient->Connected()){
			TCPClient->Disconnect();
		}
		pnlConnection->Color = clRed;
		btnConnect->Enabled = true;             //connect button enablen
		btnDisconnect->Enabled = false;         //Disconnect disablen
		btnReady->Enabled = false;              //Ready disablen
		pnlGReady->Color = clRed;             	//Gegner nciht ready
		pnlSpielen->Color = clRed;
		gridOwn->Enabled = false;               //Spielfeld disablen
		gridEnemy->Enabled = false;
		rgbSchiffslage->Enabled = false;               //Drehenbutton disablen
		btnDelete->Enabled = false;             //Löschenbutton disablen

		int icount=0;                           //Laufvariable
		int icounts=0;                          //Laufvariable
		for(icount=0;icount<11;icount++){       //Spielfeld leeren
			for(icounts=0;icounts<11;icounts++){
				gridOwn->Cells[icount][icounts] = "";
				gridEnemy->Cells[icount][icounts] = "";
			}
		}

		iSchiff1=4;                         //Schiffeanzahl zurücksetzten
		iSchiff2=3;
		iSchiff3=2;
		iSchiff4=1;
		pnlSchif1->Caption = IntToStr(iSchiff1);    //Schiffanzahl ausgeben
		pnlSchif2->Caption = IntToStr(iSchiff2);
		pnlSchif3->Caption = IntToStr(iSchiff3);
		pnlSchif4->Caption = IntToStr(iSchiff4);

		if(!bDiscon){
			for(icount=0;icount<4;icount++){        //kleinSchiff löschen
				delete kSchiff[icount];
			}

			for(icount=0;icount<3;icount++){        //mittelSchiff löschen
				delete mSchiff[icount];
			}

			for(icount=0;icount<2;icount++){        //grossSchiff löschen
				delete gSchiff[icount];
			}

			delete sSchiff;           				//sehr gross Schiff löschen
			bDiscon=true;
		}

		iFlip=0;                                //Drehungsgrad reseten
		rgbSchiffslage->ItemIndex = 0;
	}catch(Exception *e){
		ShowMessage("Fehler beim Discconnecten");
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::rgbSchiffslageClick(TObject *Sender)
{
	iFlip = rgbSchiffslage->ItemIndex;              //Richtung für Schiffslage festlegen
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tReadTimer(TObject *Sender)
{
	//timer welcher prüft ob nachrichten vom server da sind
	try{
		if(TCPClient->Connected()){                             //Prüfen ob client connected ist
            pnlConnection->Color = clLime;
			if(!(TCPClient->IOHandler->InputBufferIsEmpty())){  //Prüfen ob text zum lesen da ist
				sMessage = TCPClient->IOHandler->ReadLn();      //Text lesen
				MessageRead(sMessage);
			}
		}else{
			TCPClientDisconnected(btnDisconnect);
        }
	}catch(Exception *e){
		ShowMessage("Fehler beim Lesen!");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::gridEnemySelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	//Prüfen ob Feld schon angeklickt
	//Wenn nicht dann prüfen ob Schiff drin ist
	//Wenn doch dann Feld nicht disablen
	//Feld disablen
	if(gridEnemy->Cells[ACol][ARow] == ""){
		String sSend = "C" + IntToStr(ACol);
		TCPClient->IOHandler->WriteLn(sSend);
		sSend = "R" + IntToStr(ARow);
		TCPClient->IOHandler->WriteLn(sSend);
		pnlSpielen->Color = clRed;
		gridEnemy->Enabled = false;
		iscol = ACol;
		isrow = ARow;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TCPClientConnected(TObject *Sender)
{
	try{
		int icount=1;       //Laufvariable
		pnlConnection->Color = clLime;          //Connectionstatus
		gridOwn->Enabled = true;                //Spielfeld aktivieren
		btnReady->Enabled = true;               //Ready button aktivieren
		pnlGReady->Color = clRed;
		pnlSpielen->Color = clRed;
		btnDisconnect->Enabled = true;          //Disconnect button aktivieren
		btnConnect->Enabled = false;            //Connect button deaktivieren
		rgbSchiffslage->Enabled = true;
		btnDelete->Enabled = true;
        bDiscon=false;

		for(icount=1;icount<11;icount++){       //Spielfeld definieren
			gridOwn->Cells[0][icount] = IntToStr(icount);
			gridOwn->Cells[icount][0] = IntToStr(icount);

			gridEnemy->Cells[0][icount] = IntToStr(icount);
			gridEnemy->Cells[icount][0] = IntToStr(icount);
		}

		for(icount=0;icount<4;icount++){        //kleinSchiff definieren
			kSchiff[icount] = new kleinSchiff();
		}

		for(icount=0;icount<3;icount++){        //mittelSchiff definieren
			mSchiff[icount] = new mittelSchiff();
		}

		for(icount=0;icount<2;icount++){        //grossSchiff definieren
			gSchiff[icount] = new grossSchiff();
		}

		sSchiff = new sgrossSchiff();           //sehr gross Schiff definieren

		iFlip=0;                                //Drehungsgrad reseten
		rgbSchiffslage->ItemIndex = 0;
	}catch(Exception *e){
		TCPClientDisconnected(btnDisconnect);
		ShowMessage("Verbindungsfehler");
	}
}
//---------------------------------------------------------------------------

