#ifndef MPDRAWDATADECODER_H
#define MPDRAWDATADECODER_H 1


#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTimeStamp.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "MpdTDCDigit.h"
#include "MpdSyncDigit.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <list>
#include <map>

const size_t kWORDSIZE = sizeof (UInt_t);
const UInt_t kENDOFSPILL = 0x4A624A62;
const UInt_t kSYNC1 = 0x2A502A50;
const Short_t kNBYTESINWORD = 4;
const UInt_t kTDC72VXS = 0xD0;

class MpdRawDataDecoder{
public:
    MpdRawDataDecoder(TString file = "", TString outfile = "", TString outfile2 = "", ULong_t nEvents = 0, ULong_t period = 4);
    virtual ~MpdRawDataDecoder();

    Bool_t ConvertRawToRoot();
    Bool_t InitConverter(TString FileName);
    Bool_t InitConverter();
    Bool_t ProcessEvent(UInt_t *data, UInt_t len);
    Bool_t Process_TDC72VXS(UInt_t *d, UInt_t len, UInt_t serial);
    Bool_t FillSYNC_VXS(UInt_t serial, UInt_t w1, UInt_t w2);
    Bool_t InitDecoder();

private:
    FILE *fRawFileIn;
    TFile *fRootFileIn;
    TFile *fRootFileOut;
    TFile *fDigiFileOut;

    Long64_t fLengthRawFile;
    TString fRawFileName;
    TString fRootFileName;
    TString fDigiFileName;
    Bool_t isSpillStart;
    ULong_t fMaxEvent;
    UInt_t fNevents;
    UInt_t fDat; //current 32-bits word
    Long64_t fCurentPositionRawFile;
    UInt_t data[10000000];
    UInt_t fEventId;
    UInt_t fRunId;
    TTree *fRawTree;
    TClonesArray *sync;
    TClonesArray *tdc;
    UInt_t fPeriodId;

        //for event
    Long64_t fTime_s;
    Long64_t fTime_ns;
    //for run
    Long64_t fTimeStart_s;
    Long64_t fTimeStart_ns;
    Long64_t fTimeFinish_s;
    Long64_t fTimeFinish_ns;

        //Map to store pairs <Crate serial> - <crate time - T0 time>
    //map<UInt_t, Long64_t> fTimeShifts;
    Double_t fT0Time; //ns
    Double_t fT0Width; //ns
    //map<TTimeStamp, Int_t> leaps;
    TTimeStamp utc_valid;
    Int_t tai_utc_dif;
    Int_t fVerbose;
    TTimeStamp TimeFirstEv;

    TTimeStamp fRunEndTime;
};

#endif
