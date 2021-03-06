//file: full path to raw file
//nEvents: if 0 then decode all events
//doConvert: convert RAW --> ROOT before decoding or use file converted before
// #include "TString.h"
// #include "TSystem.h"
// #include "TStopwatch.h"
#include "MpdRawDataDecoder.h"
R__ADD_INCLUDE_PATH($VMCWORKDIR)
#include "macro/mpd/mpdloadlibs.C"

void MpdDataToRoot(TString file = "mpd_run_MPD-TOF-STAND-1_119.data", TString outfile = "decoded.root", Long_t nEvents = 0, Bool_t doConvert = kTRUE, Bool_t doHoldRawRoot = kFALSE) {

    mpdloadlibs();
//     // gSystem->Load("lib/libDecoder.so");
     gSystem->ExpandPathName(file);
     gSystem->ExpandPathName(outfile);

//     Int_t iVerbose = 1; ///<- Verbosity level: 0 - Progress Bar; 1 - short info on passed events
     UInt_t period = 7;

     TStopwatch timer;
     timer.Start();

     MpdRawDataDecoder* decoder = new MpdRawDataDecoder(file, outfile, nEvents, period);
//     // use kSRCSETUP for Short-Range Correlation program and kBMNSETUP otherwise
//     //BmnSetup stp = (decoder->GetRunId() >= 2041 && decoder->GetRunId() <= 3588) ? kSRCSETUP : kBMNSETUP;
//     //decoder->SetBmnSetup(stp);
//     //decoder->SetVerbose(iVerbose);
//     /*
//     Bool_t setup[11]; //array of flags to determine BM@N setup
//     //Just put "0" to exclude detector from decoding
//     setup[0]  = 1; // TRIGGERS
//     setup[1]  = 1; // MWPC
//     setup[2]  = 1; // SILICON
//     setup[3]  = 1; // GEM
//     setup[4]  = 1; // TOF-400
//     setup[5]  = 1; // TOF-700
//     setup[6]  = 1; // DCH
//     setup[7]  = 1; // ZDC
//     setup[8]  = 1; // ECAL
//     setup[9]  = 1; // LAND
//     setup[10] = 1; // CSC
//     decoder->SetDetectorSetup(setup);
//     decoder->SetAdcDecoMode(period < 6 ? kBMNADCSM : kBMNADCMK);

//     TString PeriodSetupExt = Form("%d%s.txt", period, ((stp == kBMNSETUP) ? "" : "_SRC"));
//     decoder->SetTrigPlaceMapping(TString("Trig_PlaceMap_Run") + PeriodSetupExt);
//     decoder->SetTrigChannelMapping(TString("Trig_map_Run") + PeriodSetupExt);
//     decoder->SetSiliconMapping(TString("SILICON_map_run") + PeriodSetupExt);
//     decoder->SetGemMapping(TString("GEM_map_run") + PeriodSetupExt);
//     decoder->SetCSCMapping(TString("CSC_map_period") + PeriodSetupExt);
//     decoder->SetMSCMapping(TString("MSC_map_Run") + PeriodSetupExt);
//     // in case comment out the line decoder->SetTof400Mapping("...")  
//     // the maps of TOF400 will be read from DB (only for JINR network)
//     decoder->SetTOF700ReferenceRun(-1);
//     decoder->SetTof700Geom(TString("TOF700_geometry_run") + PeriodSetupExt);
//     decoder->SetTof400Mapping(TString("TOF400_PlaceMap_RUN") + PeriodSetupExt, TString("TOF400_StripMap_RUN") + PeriodSetupExt);
//     if (decoder->GetRunId() >= 4278 && decoder->GetPeriodId() == 7)
//         decoder->SetTof700Mapping(TString("TOF700_map_period_") + Form("%d_from_run_4278.txt", period));
//     else
//         decoder->SetTof700Mapping(TString("TOF700_map_period_") + Form("%d.txt", period));
//     decoder->SetZDCMapping("ZDC_map_period_5.txt");
// //    decoder->SetZDCCalibration("zdc_muon_calibration.txt");
//     decoder->SetECALMapping(TString("ECAL_map_period_") + PeriodSetupExt);
//     decoder->SetECALCalibration("");
//     decoder->SetMwpcMapping(TString("MWPC_map_period") + ((period == 6 && decoder->GetRunId() < 1397) ? 5 : PeriodSetupExt));
//     decoder->SetLANDMapping("land_mapping_jinr_triplex.txt");
//     decoder->SetLANDPedestal("r0030_land_clock.hh");
//     decoder->SetLANDTCal("r0030_land_tcal.hh");
//     decoder->SetLANDDiffSync("r352_cosmic1.hh");
//     decoder->SetLANDVScint("neuland_sync_2.txt");
//     decoder->InitMaps(); /// <- should be run after all mappings set
//     */
     std::cout<< "Created Decoder object, starting to decode"<< std::endl;
     if (doConvert) decoder->ConvertRawToRoot(); // Convert raw data in .data format into adc-,tdc-, ..., sync-digits in .root format
    
//     //Bool_t decoStatus = decoder->DecodeDataToDigi(); // Decode data into detector-digits using current mappings.
    
     timer.Stop();
//     /*
//     if (decoStatus == kBMNSUCCESS) {
//         if (!doHoldRawRoot) gSystem->Exec(TString::Format("rm -f %s", decoder->GetRootFileName().Data()));
//         printf("\tMacro finished successfully!\n\n"); // marker of successfully execution for software testing systems
//         Double_t rtime = timer.RealTime();
//         Double_t ctime = timer.CpuTime();
//         printf("Real time %f s, CPU time %f s\n", rtime, ctime);
//     }
//     */
     std::cout<<"Decoding complete!"<< std::endl;
     delete decoder;
}
