#include "MpdTDCDigit.h"

MpdTDCDigit::MpdTDCDigit() {
}

MpdTDCDigit::MpdTDCDigit(UInt_t iSerial, UChar_t iType, UChar_t iSlot, Bool_t iLeading, UChar_t iChannel, UChar_t iHptdcId, UInt_t iValue, UShort_t iTimestamp) {
    fSerial = iSerial;
    fType = iType;
    fSlot = iSlot;
    fLeading = iLeading;
    fHptdcId = iHptdcId;
    fChannel = iChannel;
    fValue = iValue;
    fTimestamp = iTimestamp;
}

MpdTDCDigit::~MpdTDCDigit() {
}

ClassImp(MpdTDCDigit)
