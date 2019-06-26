//---------------------------------------------------------------------------

#ifndef uSchiffH
#define uSchiffH
//---------------------------------------------------------------------------

class kleinSchiff{
	public:
	kleinSchiff();
	~kleinSchiff();

	int setSchiff(int, int, int, int);       	   	// set Funktion

	bool bSet=false;                                //Schiff gesetzt ?
	int iRow=0;                                     //Koordinaten
	int iCol=0;
	int iGross=1;                                   //Schiffsgröße
	int iLage=0;                                    //Lage 0=horizontal, 1=vertikal
};

class mittelSchiff : public kleinSchiff{
	public:
	mittelSchiff();
	~mittelSchiff();

	int iGross=2;
};

class grossSchiff : public kleinSchiff{
	public:
	grossSchiff();
	~grossSchiff();

	int iGross=3;
};

class sgrossSchiff : public kleinSchiff{
	public:
	sgrossSchiff();
	~sgrossSchiff();

	int iGross=4;
};

#endif
