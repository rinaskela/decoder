#ifndef MPDTOF8RAW2DIGIT_H
#define	MPDTOF8RAW2DIGIT_H 

#include "TString.h"
#include "TClonesArray.h"
#include "MpdTDCDigit.h"
#include "MpdTof8Digit.h"
#include <iostream>
#include "Riostream.h"
#include <cstdlib>
#include <UniDbDetectorParameter.h>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <fstream>
#include <sstream>

//Side of the strip is stored as a bool variable
#define TOF8_LEFT true
#define TOF8_RIGHT false

#define TOF8_CHANNEL_NUMBER 72
#define TOF8_BIN_NUMBER 1024
#define TOF8_MAX_TIME (24.) //In ns
#define TOF8_TDC_TYPE (0xd0) //TDC72VHL

//A simple class to compare the TDCDigits. See .cxx code
struct _Tof8TDCCompare {
	bool operator()(const MpdTDCDigit& a, const MpdTDCDigit& b);
};

//Map element
struct MpdTof8Map2 {
	Short_t plane;
	Short_t strip;
	Bool_t side;
	MpdTof8Map2(Short_t, Short_t, Bool_t);
	MpdTof8Map2();
};

//TDC parameters
struct MpdTof8TDCParameters {
	double INL[TOF8_CHANNEL_NUMBER][TOF8_BIN_NUMBER]; //INL
	MpdTof8Map2 ChannelMap[TOF8_CHANNEL_NUMBER]; //A MpdTof1Map2 for every channel
	double t[TOF8_CHANNEL_NUMBER]; //To store the value temporarily. See .cxx code
	MpdTof8TDCParameters(); //Simple constructor
};

class MpdTof8Raw2Digit {
	public:		
		MpdTof8Raw2Digit(); //MpdTof8Raw2Digit main constructor
		MpdTof8Raw2Digit(int nPeriod, int nRun); //Calls setRun(...)
		~MpdTof8Raw2Digit(); //Destructor
		
		Bool_t setRun(int nPerion, int nRun); //Load mapping and INL from the DB for run #nRun in period #nPeriod
		void setMapFromFile(TString placementMapFile, TString mapFile); //Load mapping from two files
		void saveMapToFile(std::string placementMapFile, std::string mapFile); //Save the mapping to two files
		
		void setINLFromFile(std::string INLFile);	//Load INL from an INI file
		void saveINLToFile(std::string INLFile, unsigned int TDCSerial); //Save INL for TDCSerial to an INI file
		
		void print(){} //Prints some info
		
		void FillEvent(TClonesArray *data, map<UInt_t, Long64_t> *mapTS,TClonesArray *Tof8digit); //
		
		static UShort_t ToGlobalChannel(UChar_t HptdcId, UChar_t channel);
                
	private:
		ClassDef(MpdTof8Raw2Digit, 1);
		void init(); //MpdTof8Raw2Digit init function (called in MpdTof8Raw2Digit constructors)
		int RunIndex, PeriodIndex; //To store the RunIndex and PeriodIndex
		std::map<std::pair<UInt_t, UChar_t>, UInt_t> PlacementMap;	//Stores the placement map
		std::map<UInt_t, MpdTof8TDCParameters> TDCMap;			//Stores the loaded main mapping
		void plmap_insert(UInt_t Serial, UChar_t Slot, UInt_t TDCSerial); //See .cxx code
		
		//std::map provides a way to find TDC by Serial and Slot really fast (O(logN))
		//MpdTof8Parameters could also be found so fast (O(logN))
		//All other operations (extracting the INL, for example) are already fast because of the structure - O(1)
};
#endif	/* MpdTOF8RAW2DIGIT_H */


