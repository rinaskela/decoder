#include "MpdTof8Raw2Digit.h"

typedef std::map<std::pair<UInt_t, UChar_t>, UInt_t>::iterator Tof8PlMapIter;
typedef std::map<UInt_t, MpdTof8TDCParameters>::iterator Tof8TDCMapIter;
typedef std::pair<UInt_t, UChar_t> Tof8PlMapKey;
typedef std::pair<UInt_t, MpdTof8TDCParameters> Tof8TDCMapElem;

using namespace std;

int Tof8GlobalNameCounter = 0;

//Simple constructors for the MpdTof8Map2 element class
MpdTof8Map2::MpdTof8Map2(Short_t p, Short_t s, Bool_t sd) {
	plane = p;
	strip = s;
	side = sd;
}
MpdTof8Map2::MpdTof8Map2() {
	plane = 0;
	strip = 0;
	side = false;
}

//Main constructor for the MpdTof8TDCParameters()
MpdTof8TDCParameters::MpdTof8TDCParameters() {
	for(int i = 0; i<TOF8_CHANNEL_NUMBER; i++) {
		for(int j = 0; j<TOF8_BIN_NUMBER; j++) {
			INL[i][j] = 0;
		}
		ChannelMap[i] = MpdTof8Map2(0, 0, false);
		t[i] = -1;
	}
}

//A small functional class, which is used to compare two MpdTDCDigits
//It is used to sort by time the TDCDigits in the MpdTof8Raw2Digit::FillEvent()
bool _Tof8TDCCompare::operator()(const MpdTDCDigit& a, const MpdTDCDigit& b) {
	return a.GetValue()<b.GetValue();
}

//MpdTof8Raw2Digit main constructor
MpdTof8Raw2Digit::MpdTof8Raw2Digit() {
	init();
}
//MpdTof8Raw2Digit destructor
MpdTof8Raw2Digit::~MpdTof8Raw2Digit() {
}

//MpdTof8Raw2Digit init function (called in MpdTof8Raw2Digit constructors)
void MpdTof8Raw2Digit::init() {
	//Nothing here
}

//A simple function to convert Hptdcid (0 -- 9) and Hptdc's channel (0 -- 8) to "global" tdc channel (0 -- 72)
UShort_t MpdTof8Raw2Digit::ToGlobalChannel(UChar_t HptdcId, UChar_t channel) {
	return HptdcId*8+channel;
}

//Load mapping from the file
void MpdTof8Raw2Digit::setMapFromFile(TString mapFile) {
	UInt_t fserial, fchan, fplane, fstrip;
	char side_c;

	// Strip map 
	ff.open(mapFile.Data(), std::fstream::in);
	if(ff.fail()) {cerr << "Cannot open the file " << mapFile << endl; return;}
        else cout << "Reading TOF8 StripMap file " << mapFile << endl;
	while(!ff.eof()) {
		ff >> std::hex >> fserial >> std::dec >> fchan >> fplane >> fstrip >> side_c;
		//if(ff.eof()) {break;} //?????????????????????????????????????????????????????????????
		Tof8TDCMapIter TDCPair = TDCMap.find(fserial);
		// if no TDC with such serial found - add one
		if(TDCPair == TDCMap.end()) {
			TDCMap.insert(Tof8TDCMapElem(fserial, MpdTof8TDCParameters()));
			TDCPair = TDCMap.find(fserial);
		}
		MpdTof8Map2* elem = &((TDCPair->second).ChannelMap[fchan]);
		elem->plane = fplane;
		elem->strip = fstrip;
		if(side_c == 'L') {elem -> side = TOF8_LEFT;}
		else if (side_c == 'R') {elem->side = TOF8_RIGHT;}
		else {cerr << "Tof8 Error: side is not L or R" << endl; continue;}
	}
	ff.close();
}
//Load INL
void MpdTof8Raw2Digit::setINLFromFile(string INLFile) {
	fstream ff(INLFile.c_str(), std::fstream::in);
	if(ff.fail()) {cerr << "Failed to open " << INLFile << endl; return;}
	unsigned int TDCSerial;
	
	//Read the header from the file
	//The format of the header seems to be [TDC-THESERIAL-inl_corr]
	ff.ignore(10, '-');
	ff >> std::hex >> TDCSerial >> std::dec;
	ff.ignore(1000, '\n');
	
	//Find the TDC
	Tof8TDCMapIter TDCPair = TDCMap.find(TDCSerial);
	if(TDCPair == TDCMap.end()) {
		cerr << "Tof400: TDC " << std::setfill('0') << std::setw(8) << std::hex << TDCSerial << std::setfill(' ') << std::dec;
		cerr << " isn't in the placement map." << endl;
		ff.close(); return;
	}
	
	unsigned int chan_id = 0;
	unsigned int lines_num = 0;
	
	while(!ff.eof()) {
		string line; char dummy;
		
		std::getline(ff, line, '\n');
		if(ff.eof()) {break;} //??????????????????????????????????????????????????????????????????
		if(line == "") {continue;}
		istringstream ss(line);
		
		ss >> chan_id >> dummy;
		if(dummy != '=') {cerr << "Tof400: Wrong INL file format." << endl; ff.close(); return;}
		if(chan_id > TOF8_CHANNEL_NUMBER) {cerr << "Tof400: Wrong channel in in the INL file" << endl; ff.close(); return;}
		
		double* INLelem = (TDCPair->second).INL[chan_id];
		
		unsigned int i_bin = 0;
		while(ss.tellg() != -1) {
			if(i_bin > TOF8_BIN_NUMBER) {
				cerr << "Tof400: INL File contains too many bins in channel." << endl;
				ff.close(); return;
			}
			if(ss.peek()==',') {ss.ignore();}
			ss >> INLelem[i_bin]; i_bin++;
		}
		if(i_bin != TOF8_BIN_NUMBER) {
			cout << "Warning: wrong number of bins in the INL file for channel " << chan_id << " (" << i_bin << ")" << endl;
		}
		lines_num++;
	}
	
	if(lines_num != TOF8_CHANNEL_NUMBER) {
		cout << "Warning: wrong number of lines in the INL file (" << lines_num << endl;
	}
	cout << "Tof400: INL for TDC " << std::setfill('0') << std::setw(8) << std::hex << TDCSerial << std::setfill(' ') << std::dec << " loaded succesfully from INL file." << endl;
}

//Load INL from file
void MpdTof8Raw2Digit::saveINLToFile(string INLFile, unsigned int TDCSerial) {
	//Find the TDC
	Tof8TDCMapIter TDCPair = TDCMap.find(TDCSerial);
	if(TDCPair == TDCMap.end()) {
		cerr << "Tof400: TDC " << std::setfill('0') << std::setw(8) << std::hex << TDCSerial << std::setfill(' ') << std::dec;
		cerr << " isn't in the placement map." << endl;
		return;
	}
	
	fstream ff(INLFile.c_str(), std::fstream::out);
	ff << "[TDC-" << std::setfill('0') << std::setw(8) << std::hex << TDCSerial << std::dec << std::setfill(' ') << "-inl_corr]" << endl;
	
	
	for(int chan = 0; chan<TOF8_CHANNEL_NUMBER; chan++) {
		ff << chan << "=";
		double* INLelem = (TDCPair->second).INL[chan];
		for(int bin = 0; bin<TOF8_BIN_NUMBER; bin++) {
			ff << INLelem[bin];
			if(bin!=TOF8_BIN_NUMBER-1) {
				ff << ", ";
			}
		}
		if(chan!=TOF8_CHANNEL_NUMBER - 1) {ff << endl;}
	}
	
	ff.close();
}

//Main function. "Converts" the TObjArray *data of MpdTDCDigit to the TObjArray *output of MpdTof8Digit
void MpdTof8Raw2Digit::FillEvent(TClonesArray *data, map<UInt_t, Long64_t> *mapTS, TClonesArray *output) {
	//0. Initialize: clear all the tempory times in the MpdTof8TDCParameters
	Tof8TDCMapIter tdci = TDCMap.begin();
	while(tdci!=TDCMap.end()) {
		for(int i = 0; i<TOF8_CHANNEL_NUMBER; i++) {
			(tdci->second).t[i] = -1;
		}
		tdci++;
	}
	//1. Filter and sort the TDC Data

	int nOut = output->GetEntries();
	
	multiset<MpdTDCDigit, _Tof8TDCCompare> temp; //Create a multiset
	//Multiset automaticly sorts the elements in it.
	//_Tof8TDCCompare functional class is used to compare two MpdTDCDigit (see above)
	
	//Fill the multiset from the input array (it sorts the input array)
	TIter it = data->begin();
	while(it!=data->end()) {
		MpdTDCDigit* obj = static_cast<MpdTDCDigit*>(*it);
		if(obj->GetType()!=TOF8_TDC_TYPE) {++it; continue;} //Just skip it in case if TDC is not TDC72VHL (0xd0)
		temp.insert(*obj);
		++it;
	}
	
	//Now a loop over the temp multiset using iterator si
	multiset<MpdTDCDigit, _Tof8TDCCompare>::iterator si = temp.begin();
	
	while(si != temp.end()) {
		//Look for the crate serial and slot in the placement map
		Tof8TDCMapIter TDCPair = TDCMap.find(std::make_pair<UInt_t, UChar_t>(si->GetSerial(), si->GetChannel()));
		UInt_t TDC_Serial = TDCPair -> first;
		//In case if nothing was found
		if(TDCPair == TDCMap.end()) {
			//Display a warning and skip. This is quite a serious error, actually
			cerr << "Warning: TDC with ID " << std::hex << TDC_Serial << std::dec <<": no data" << endl;
			si++;
			continue;
		}
                map<UInt_t,Long64_t>::iterator itTS = mapTS->find(si->GetSerial());
                if (itTS == mapTS->end()){
                    si++;
                    continue;
                }
		Long64_t TimeShift = itTS->second;
                
		//Finally, get the MpdTof8TDCParameters instance for this TDC Serial
		MpdTof8TDCParameters* par = &(TDCPair->second);
		//And get it's channel using MpdTof8Raw2Digit::ToGlobalChannel (see above)
		UShort_t rchan = ToGlobalChannel(si->GetHptdcId(), si->GetChannel());
		
		//Get the time from the TDC value
		//t = (val + INL[channel][val % 1024]) * (24ns / 1024)
		//See TOF8_BIN_NUMBER and TOF8_MAX_TIME defines in the .h file
		double timeFromDigit = (si->GetValue() + par->INL[rchan][(si->GetValue()) % TOF8_BIN_NUMBER])* TOF8_MAX_TIME / double(TOF8_BIN_NUMBER);
		
		if(si->GetLeading()) {
			//If this is a leading TDC digit, just fill the temporary time in the MpdTof8TDCParameters.
			par->t[rchan] = timeFromDigit;
		} else {
			//If this is NOT a leading TDC Digit...
			if(par->t[rchan] != -1) {
				//BmbTof8TDCParameters' temporary time (par->t[rchan]) has been already set
				//This temporary time should store the leading time
				if( timeFromDigit < (par->t[rchan]) ) {
					//This is impossible:
					//Leading value is larger than the trailing time
					//The MpdTDCDigit array has already been sorted, so this shouldn't happen
					//But let's just make sure, this could be a serious error
					cout << "Error: negative dt" << endl;
					
					//Print all the TDC Digits we have loaded in the sorted order
					multiset<MpdTDCDigit, _Tof8TDCCompare>::iterator si2 = temp.begin();
					while(si2 != temp.end()) {
						cout << std::hex << si2->GetSerial() << " " << int(si2->GetSlot()) << " " << std::dec << int(si2->GetHptdcId()) << " " << int(si2->GetChannel()) << " " << (si2->GetLeading()?"L":"T") << si2->GetValue() << endl;
						si2++;
					}
					//Print where exactly the error has occured
					cout << "Error at: " << endl;
					cout << std::hex << si->GetSerial() << " " << int(si->GetSlot()) << " " << std::dec << int(si->GetHptdcId()) << " " << int(si->GetChannel()) << " " << (si->GetLeading()?"L":"T") << si->GetValue() << endl;					
					cout << std::hex << TDC_Serial << std::dec << " " << rchan << " " << (par->t[rchan]) << "--" << timeFromDigit << endl;
				} else {
					//So we've got the Leading and Trail times and everything seems to be okay
					//Find the MpdTof8Map2 mapping element, which stores the plane, strip and side.
					MpdTof8Map2* elem = &(par->ChannelMap[rchan]);
					
					//Create a new MpdTof8Digit:
					//Plane, strip, and side are loaded from the found MpdTof8Map2
					//Leading time is loaded from the temporary storage in the MpdTof8TDCParameters class
					//Trailing time is just the time in current TDC Digit
					//(See above: this piece of code is executed only if the current MpdTDCDigit is NOT leading)
					
					new((*output)[nOut]) MpdTof8Digit(elem->plane,elem->strip,elem->side,par->t[rchan] + TimeShift, timeFromDigit - (par->t[rchan]));
					nOut++;
				}
			}
			//We don't need to do anything in case if no leading time has occured yet (if par->t[rchan] == -1)
			
			//Finally, reset the temporary storage to prevent errors
			par->t[rchan] = -1;
		}
		si++;
	}	
}

ClassImp(MpdTof8Raw2Digit)
