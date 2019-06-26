//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uSchiff.h"
#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

kleinSchiff::kleinSchiff(){}

kleinSchiff::~kleinSchiff(){}


mittelSchiff::mittelSchiff(){}

mittelSchiff::~mittelSchiff(){}


grossSchiff::grossSchiff(){}

grossSchiff::~grossSchiff(){}


sgrossSchiff::sgrossSchiff(){}

sgrossSchiff::~sgrossSchiff(){}

int kleinSchiff::setSchiff(int col, int row, int gross, int flip){
	int izahler=0;                                      //zaählervariable
	//Prüfen ob kein Schiff in der Zelle und keins daneben ist
	if(frmMain->gridOwn->Cells[col][row] != "X"){
		switch (flip) {
			case 0:
				int bcol, brow;
				bool bOk;
                bOk=true;
				bcol = col + gross;
				if(frmMain->gridOwn->Cells[bcol][row] != "X"){
					brow = row + 1;
					if(frmMain->gridOwn->Cells[bcol][brow] != "X"){
						for(int i=0; i<=gross;i++){
							bcol -= 1;
							if(frmMain->gridOwn->Cells[bcol][brow] == "X"){
								gross=12;
								break;
							}
						}

						brow -= 1;
						if(frmMain->gridOwn->Cells[bcol][brow] != "X"){
							for (int i = 0; i <= gross+1; i++) {
								if(frmMain->gridOwn->Cells[bcol][brow-1] == "X"){
									gross=12;
									break;
								}
								bcol += 1;
							}

							if(col+gross<12){                              //Prüft ob Schiff ins Feld passt
								for(izahler=0;izahler<gross;izahler++){      //Schiffsgröße testen ob nicht mit andere Schiff kollidiert
									if(frmMain->gridOwn->Cells[col+izahler][row] == "X"){
										bOk=false;
										break;
									}
								}
								if(bOk){
									for(izahler=0;izahler<gross;izahler++){      //Soviele X setzen wie Schiff gross ist
										frmMain->gridOwn->Cells[col+izahler][row] = "X";

									}
									iRow=row;                                   //Dem Objekt die Kooridnaten geben wo es gesetzt wurde
									iCol=col;
									return 1;                                   //Bei Erfolg return 1 ansonst 0
								}
							}
						}
					}
				}
				break;
			case 1:
				brow=0;
				bcol=0;
                bOk=true;
				brow = row + gross;
				if(frmMain->gridOwn->Cells[col][brow] != "X"){
					bcol = col - 1;
					if(frmMain->gridOwn->Cells[bcol][brow] != "X"){
						for(int i=0; i<=gross;i++){
							brow -= 1;
							if(frmMain->gridOwn->Cells[bcol][brow] == "X"){
								gross=12;
								break;
							}
						}

						bcol += 1;
						if(frmMain->gridOwn->Cells[bcol][brow] != "X"){
							for (int i = 0; i <= gross+1; i++) {
								if(frmMain->gridOwn->Cells[bcol+1][brow] == "X"){
									gross=12;
									break;
								}
								brow += 1;
							}

							if(row+gross<12){                              //Prüft ob Schiff ins Feld passt
								for(izahler=0;izahler<gross;izahler++){      //Größe testen das nicht mit andere schiff kollidiert
									if(frmMain->gridOwn->Cells[col][row+izahler] == "X"){
										bOk=false;
										break;
									}
								}
								if(bOk){
									for(izahler=0;izahler<gross;izahler++){      //Soviele X setzen wie Schiff gross ist
										frmMain->gridOwn->Cells[col][row+izahler] = "X";
									}
									iRow=row;                                   //Dem Objekt die Kooridnaten geben wo es gesetzt wurde
									iCol=col;
									return 1;                                   //Bei Erfolg return 1 ansonst 0
								}
							}
						}
					}
				}
				break;
		}
	}
	return 0;                                                     //Wenn kein Schiff gesetzt wurde return 0
}


