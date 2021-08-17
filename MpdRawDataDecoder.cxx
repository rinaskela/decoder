#include "MpdRawDataDecoder.h"

using namespace std;

MpdRawDataDecoder::MpdRawDataDecoder(TString file, TString outfile, TString outfile2, ULong_t nEvents, ULong_t period) {
    //eventHeaderDAQ = NULL;
    //eventHeader = NULL;
    fTime_ns = 0;
    fTime_s = 0;
    //fT0Time = 0.0;
    fRawTree = NULL;
    //fDigiTree = NULL;
    fRootFileIn = NULL;
    fRootFileOut = NULL;
    fRawFileIn = NULL;
    fDigiFileOut = NULL;
    sync = NULL;
    //hrb = NULL;
    tdc = NULL;
    //land = NULL;
    fRawFileName = file;
    fEventId = 0;
    fNevents = 0;
    fMaxEvent = nEvents;
    fPeriodId = period;
    if (fRawFileName != "") {
        //        fRunId = GetRunIdFromFile(fRawFileName);
        //        fDigiFileName = (outfile == "") ? Form("bmn_run%04d_digi.root", fRunId) : outfile;
        //        Int_t lastSlash = fDigiFileName.Last('/');
        //        TString digiPath = (lastSlash == TString::kNPOS) ? "" : TString(fDigiFileName(0, lastSlash + 1));
        //        fRootFileName = Form("%sbmn_run%04d_raw.root", digiPath.Data(), fRunId);
        fRootFileName = outfile;
        fDigiFileName = outfile2;
    }
    fDat = 0;
    fTimeStart_s = 0;
    fTimeStart_ns = 0;
    tai_utc_dif = 0;
    fVerbose = 0;
    isSpillStart = kFALSE;
    //InitUTCShift();
    //InitMaps();
    TimeFirstEv.SetNanoSec(0);
    TimeFirstEv.SetSec(0);
    cout<<"Constructor Done!"<<endl;

}

MpdRawDataDecoder::~MpdRawDataDecoder() {
}

Bool_t MpdRawDataDecoder::InitConverter(TString FileName) {
    printf( "\n================ CONVERTING ================\n" );
    fRawFileName = FileName;
    fRawFileIn = fopen(fRawFileName, "rb");
    if (fRawFileIn == NULL) {
        printf("\n!!!!!\ncannot open file %s\nConvertRawToRoot are stopped\n!!!!!\n\n", fRawFileName.Data());
        return 1;
    }
    return InitConverter();
}

Bool_t MpdRawDataDecoder::InitConverter() {
    fRawTree = new TTree("MPD_RAW", "MPD_RAW");
    sync = new TClonesArray("MpdSyncDigit");
    tdc = new TClonesArray("MpdTDCDigit");
    //eventHeaderDAQ = new BmnEventHeader();

    fRawTree->Branch("SYNC", &sync);
    fRawTree->Branch("TDC", &tdc);
    //fRawTree->Branch("BmnEventHeader.", &eventHeaderDAQ);
    cout << "InitConverter Done!"<<endl;
    return 0;
}

Bool_t MpdRawDataDecoder::ConvertRawToRoot() {
    if (InitConverter(fRawFileName) == 1)
        return 1;
    fseeko64(fRawFileIn, 0, SEEK_END);
    fLengthRawFile = ftello64(fRawFileIn);
    rewind(fRawFileIn);
    fRootFileOut = new TFile(fRootFileName, "recreate");
    isSpillStart = kTRUE;

    while (kTRUE) {
        if (fMaxEvent > 0 && fNevents == fMaxEvent) break;
        fread(&fDat, kWORDSIZE, 1, fRawFileIn);
                printf("read word 0x%08X\n", fDat);
        fCurentPositionRawFile = ftello64(fRawFileIn);
        if (fCurentPositionRawFile >= fLengthRawFile) 
        {
            cout << "110 break" << endl;
            break;
        }
        if (fDat == kENDOFSPILL) {
                        printf("EOS!!\n");
            isSpillStart = kTRUE;
        }
        if (fDat == kSYNC1) { //search for start of event
            // read number of bytes in event
                                    printf("word# 0: 0x%08X\n", fDat);
            if (fread(&fDat, kWORDSIZE, 1, fRawFileIn) != 1) continue;
                                    printf("word# 1: 0x%08X \t", fDat);
            fDat = fDat / kNBYTESINWORD + 1; // bytes --> words // +1 for "event number" word
                                    printf("length of data block %d words \n", fDat);
            if (fDat >= 100000) { // what the constant?
                printf("Wrong data size: %d:  skip this event\n", fDat);
                fread(data, kWORDSIZE, fDat, fRawFileIn);
            } else {
                //read array of current event data and process them
                if (fread(data, kWORDSIZE, fDat, fRawFileIn) != fDat) continue;
                fEventId = data[0];
                                for (Int_t ii = 0; ii < fDat; ii++) {
                                    printf("word# %d: 0x%08X\n", ii + 2, data[ii]);
                                }
                                                //getchar();
                if (fEventId <= 0) continue; // skip bad events
                ProcessEvent(data, fDat);
                if (data[0] != (fNevents + 1)) // Just a check to see if somehow ProcessEvent messed up our counting
                    printf( "***Extreme warning, events are not synced: %i, %i***\n" , fEventId, fNevents + 1);
                fNevents++;
                fRawTree->Fill();
            }
        }
    cout << "ConvertRawToRoot Done!" << endl;
    }
    fRawTree->Write();
    fRootFileOut->Close();
    fclose(fRawFileIn);
    return 0;
}


Bool_t MpdRawDataDecoder::ProcessEvent(UInt_t *d, UInt_t len) {
    sync->Delete();
    tdc->Delete();
    Long64_t idx = 1;

    while (idx < len) {
        UInt_t serial = d[idx++]; //[1]
        UInt_t id = (d[idx] >> 24); //[2]
        UInt_t payload = (d[idx++] & 0xFFFFFF) / kNBYTESINWORD; //[2]
        UInt_t payload2;
                printf("Event %d:\n serial = 0x%06X\n id = Ox%02X\n payload = %d\n", fEventId, serial, id, payload);
        if (payload > 2000000) {
            printf("[WARNING] Event %d:\n serial = 0x%06X\n id = Ox%02X\n payload = %d\n", fEventId, serial, id, payload);
            break;
        }
        switch (id) {
            case kTDC72VXS:
            {
                //                UInt_t tempidx = (UInt_t) idx;

                UInt_t d0, d1, d2, d3;
                d0 = d[idx++]; //d[0] MStream Header https://afi.jinr.ru/MpdDeviceRawDataFormat
                d1 = d[idx++]; //d[1] Event timestamp, TAI seconds https://afi.jinr.ru/TDC72VXS_DataFormat#STAT_BLK
                d2 = d[idx++]; //d[2] Event timestamp, TAI flags https://afi.jinr.ru/TDC72VXS_DataFormat#STAT_BLK
                d3 = d[idx++]; //d[3] TDC dataBlock header  https://afi.jinr.ru/TDC72VXS_DataFormat
                FillSYNC_VXS(serial, d1, d2); // WR(?) time for TDC72_VXS
                                printf("   word 0x%08X  MStream Header\n", d0);
                                printf("   word 0x%08X  Event timestamp, TAI seconds\n", d1);
                                printf("   word 0x%08X  Event timestamp, TAI flags\n", d2);
                                printf("   word 0x%08X  TDC dataBlok header \n", d3);

                payload2 = (d3 & 0xFFFF) / kNBYTESINWORD; //d[3] Data Block (type 0) https://afi.jinr.ru/TDC72VXS_DataFormat#STAT_BLK
                UInt_t DataType = (d3 & 0xF0000000) >> 28;
                                printf("Data Block word 0x%08X Number words in TDC72VXS = %d\n", d[idx - 1], payload2);
                Process_TDC72VXS(&d[idx], payload2, serial);
                idx -= 4; //???
                //                                printf("word 0x%08X\n", d[idx]);
                //                                getchar();
                break;
            }
        }
        idx += payload;
        cout << "ProcessEvent Done!" << endl;
    }
    return 0;
}

Bool_t MpdRawDataDecoder::Process_TDC72VXS(UInt_t *d, UInt_t len, UInt_t serial) {

    Int_t idx = 0;
    UInt_t modId = kTDC72VXS;
    UInt_t type = d[idx] >> 28;

    while (type != 7) { //data will be finished when module trailer appears
        if (type == 6) {
            fprintf(stderr,"Warning: TDC (serial 0x%08X tdcID %d) error code: 0x%04X\n",
                    serial, ((d[idx] >> 24) & 0xF), d[idx] & 0x7FFF); //(d[idx] & ((1<<15) - 1)));
            return 1;
        }
        if (type == 4 || type == 5) { // 4 - leading, 5 - trailing

            UInt_t time = d[idx] & 0x1FFFFF;
            UInt_t channel = (d[idx] >> 21) & 0x7F;
            TClonesArray &ar_tdc = *tdc;
            new(ar_tdc[tdc->GetEntriesFast()]) MpdTDCDigit(serial, modId, 0, (type == 4), channel, 0, time);
        }
        idx++; //go to the next DATA-word
        type = d[idx] >> 28;
    }
    return 0;
}

Bool_t MpdRawDataDecoder::FillSYNC_VXS(UInt_t serial, UInt_t w1, UInt_t w2) { //M-Stream Subtype 0 Payload  https://afi.jinr.ru/MStream_2_1
    UInt_t ts_t0_s = w1;
    UInt_t ts_t0_ns = w2 & 0x0FFFFFFF;
    UInt_t TaiFlag = (w2 & 0xF0000000) >> 28;
    UInt_t GlobalEvent = fEventId;

    if (TimeFirstEv.GetSec() == 0 && TimeFirstEv.GetNanoSec() == 0) {
        TimeFirstEv.SetNanoSec(ts_t0_ns);
        TimeFirstEv.SetSec(ts_t0_s);
    }

    fTime_ns = ts_t0_ns - TimeFirstEv.GetNanoSec();
    fTime_s = ts_t0_s - TimeFirstEv.GetSec();
    TClonesArray & ar_sync = *sync;
    new(ar_sync[sync->GetEntriesFast()]) MpdSyncDigit(serial, GlobalEvent, ts_t0_s, ts_t0_ns);

    return 0;
}

Bool_t MpdRawDataDecoder::InitDecoder() {

        if (kTRUE) {
        tof8 = new TClonesArray("MpdTof1Digit");
        fDigiTree->Branch("TOF1", &tof1);
        fTof1Mapper = new BmnTof1Raw2Digit();
        Bool_t FlagTemp = fTof1Mapper->setRun(fPeriodId, fRunId);
        if (FlagTemp == kFALSE) {
            if (fTof1StripMapFileName.Sizeof() > 1) {
                TString dir = Form("%s%s", getenv("VMCWORKDIR"), "/input/");
                fTof400Mapper->setMapFromFile(dir + fTof400StripMapFileName.Data());
            } else
                cout << "Map for TOF400 are not loaded" << endl;
        }
    }
}

Bool_t MpdRawDataDecoder::DecodeDataToDigi() {
    fRawTree = nullptr;
    fRawTreeSpills = nullptr;
    tdc = nullptr;
    sync = nullptr;
    msc = nullptr;

    printf(ANSI_COLOR_RED "================= DECODING =================\n" ANSI_COLOR_RESET);

    fRootFileIn = new TFile(fRootFileName, "READ");
    if (fRootFileIn->IsOpen() == false) {
        printf("\n!!!!\ncannot open file %s \nDecodeDataToDigi has been stopped\n!!!!\n", fRootFileName.Data());
        return 1;
    } else {
        printf("\nINPUT ROOT FILE: ");
        printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, fRootFileName.Data());
        printf("\nOUTPUT DIGI FILE: ");
        printf(ANSI_COLOR_BLUE "%s\n\n" ANSI_COLOR_RESET, fDigiFileName.Data());
    }
    fRawTree = (TTree *) fRootFileIn->Get("MPD_RAW");
    fRawTree->SetBranchAddress("TDC", &tdc);
    fRawTree->SetBranchAddress("SYNC", &sync);

    fDigiFileOut = new TFile(fDigiFileName, "recreate");
    fNevents = (fMaxEvent > fRawTree->GetEntries() || fMaxEvent == 0) ? fRawTree->GetEntries() : fMaxEvent;
    Double_t fSize = 0.0;
    UInt_t runId = 0;
    vector<UInt_t> startTripEvent;
    vector<UInt_t> endTripEvent;

    printf("\n[INFO]");
    printf(ANSI_COLOR_BLUE " Main loop over events:\n" ANSI_COLOR_RESET);
    for (UInt_t iEv = 0; iEv < fNevents; ++iEv) {
        ClearArrays();

        fRawTree->GetEntry(iEv);

        BmnEventHeader* headDAQ = eventHeaderDAQ;
        curEventType = headDAQ->GetEventType();
        fEventId = headDAQ->GetEventId();
        FillTimeShiftsMap();
        if (!headDAQ) continue;

        if (iEv == 0) {

            fSize = Double_t(fLengthRawFile / 1024. / 1024.);

        }
        Bool_t isTripEvent = kFALSE;
        for (Int_t iTrip = 0; iTrip < startTripEvent.size(); ++iTrip) {
            if (headDAQ->GetEventId() > startTripEvent[iTrip] && headDAQ->GetEventId() < endTripEvent[iTrip]) {
                isTripEvent = kTRUE;
                break;
            }
        }
        if (fVerbose == 1) {
            //            if (iEv % 5000 == 0) cout << "Digitization event #" << fEventId << "/" << fNevents << "; Spill #" << fSpillCntr << endl;
        } else if (fVerbose == 0)
            DrawBar(iEv, fNevents);

        if (fTrigMapper) {
            fTrigMapper->FillEvent(tqdc_tdc, tqdc_adc);
            fTrigMapper->FillEvent(tdc);
            fT0Time = 0.;
            GetT0Info(fT0Time, fT0Width);
        }
        eventHeader->SetRunId(headDAQ->GetRunId());
        eventHeader->SetEventId(headDAQ->GetEventId());
        eventHeader->SetPeriodId(headDAQ->GetPeriodId());
        eventHeader->SetEventTimeTS(TTimeStamp(time_t(fTime_s), fTime_ns));
        eventHeader->SetEventTime(TTimeStamp(time_t(fTime_s), fTime_ns).AsDouble());
        eventHeader->SetEventType(curEventType);
        eventHeader->SetTripWord(isTripEvent);
        eventHeader->SetTrigInfo(headDAQ->GetTrigInfo());
        eventHeader->SetTimeShift(fTimeShifts);
        eventHeader->SetStartSignalInfo(fT0Time, fT0Width);
        eventHeader->SetSpillStart(headDAQ->GetSpillStart());
        fTof1Mapper->FillEvent(tdc, &fTimeShifts, tof8);
        if (fMSCMapper) fMSCMapper->SumEvent(msc, eventHeader, spillHeader, fPedEvCntrBySpill);

        fDigiTree->Fill();
        prevEventType = curEventType;
    }
//                    if (fSiliconMapper) {
//                        fSiliconMapper->DrawDebugHists("sil-sig-cms.pdf");
//                        fSiliconMapper->ClearDebugHists();
//                    }

    fRunEndTime = TTimeStamp(time_t(fTime_s), fTime_ns);
    DigiRunHeader * runHeader = new DigiRunHeader(fPeriodId, fRunId, fRunStartTime, fRunEndTime);
    if (fMSCMapper) fMSCMapper->FillRunHeader(runHeader);
    fDigiFileOut->WriteObject(runHeader, fDigiRunHdrName.Data());

    printf(ANSI_COLOR_RED "\n=============== RUN" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE " %04d " ANSI_COLOR_RESET, fRunId);
    printf(ANSI_COLOR_RED "SUMMARY ===============\n" ANSI_COLOR_RESET);
    printf("START (event 1):\t%s\n", fRunStartTime.AsString());
    printf("FINISH (event %d):\t%s\n", fNevents, fRunEndTime.AsString());
    printf(ANSI_COLOR_RED "================================================\n" ANSI_COLOR_RESET);

    fDigiTree->Write();
    fDigiTreeSpills->Write();
    DisposeDecoder();
    fDigiFileOut->Write();
    fDigiFileOut->Close();
    delete runHeader;
    fRootFileIn->Close();

    return kBMNSUCCESS;
}

Bool_t MpdRawDataDecoder::FillTimeShiftsMap() {
    if (fT0Map == NULL) return 1;
    Long64_t t0time = 0;
    for (Int_t i = 0; i < sync->GetEntriesFast(); ++i) {
        MpdSyncDigit* syncDig = (MpdSyncDigit*) sync->At(i);
        if (syncDig->GetSerial() == fT0Map->serial) {
            t0time = syncDig->GetTime_ns() + syncDig->GetTime_sec() * 1000000000LL;
            fTime_s = syncDig->GetTime_sec();
            fTime_ns = syncDig->GetTime_ns();
            if (fEventId == 1) {
                fRunStartTime = TTimeStamp(time_t(fTime_s), fTime_ns);
            }
            break;
        }
    }
    for (Int_t i = 0; i < sync->GetEntriesFast(); ++i) {
        MpdSyncDigit* syncDig = (MpdSyncDigit*) sync->At(i);
        Long64_t syncTime = (t0time == 0.0) ? 0 : syncDig->GetTime_ns() + syncDig->GetTime_sec() * 1000000000LL;
        fTimeShifts.insert(pair<UInt_t, Long64_t>(syncDig->GetSerial(), syncTime - t0time));
    }
    return 0;
}
