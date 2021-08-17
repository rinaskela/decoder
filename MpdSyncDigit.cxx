#include "MpdSyncDigit.h"

MpdSyncDigit::MpdSyncDigit() {
}

MpdSyncDigit::MpdSyncDigit(UInt_t iSerial, Long64_t iEvent, Long64_t t_sec, Long64_t t_ns) {
    fSerial = iSerial;
    fGlobalEvent = iEvent;
    fT_sec = t_sec;
    fT_ns = t_ns;
}

MpdSyncDigit::~MpdSyncDigit() {
}

ClassImp(MpdSyncDigit)
