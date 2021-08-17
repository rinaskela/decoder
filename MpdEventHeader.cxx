#include "MpdEventHeader.h"

// -----   Default constructor   -------------------------------------------

MpdEventHeader::MpdEventHeader() :
FairEventHeader(),
fHeaderName("MpdEventHeader."),
fEventId(0),
fPeriodId(0),
fEventTimeTS(TTimeStamp()),
fEventType(kMpdPAYLOAD),
fTripWord(kFALSE),
fSpillStart(kFALSE),
fStartSignalTime(0),
fStartSignalWidth(0),
fTrigUnion(0) {
    fTrigInfo = new MpdTrigInfo();
}

MpdEventHeader::MpdEventHeader(UInt_t run_id, UInt_t event_id, TTimeStamp event_time, MpdEventType event_type, Bool_t trip, MpdTrigInfo* trig_info) :
FairEventHeader(),
fHeaderName("MpdEventHeader."),
fEventId(event_id),
fEventTimeTS(event_time),
fEventType(event_type),
fTripWord(trip),
fSpillStart(kFALSE),
fStartSignalTime(0),
fStartSignalWidth(0),
fTrigInfo(trig_info),
fTrigUnion(0) {
    SetRunId(run_id);
    SetEventTime(event_time.AsDouble());
}

MpdEventHeader::MpdEventHeader(UInt_t run_id, UInt_t event_id, TTimeStamp event_time, MpdEventType event_type, Bool_t trip, MpdTrigInfo* trig_info, map<UInt_t, Long64_t> time_shift) :
FairEventHeader(),
fHeaderName("MpdEventHeader."),
fEventId(event_id),
fEventTimeTS(event_time),
fEventType(event_type),
fTripWord(trip),
fSpillStart(kFALSE),
fStartSignalTime(0),
fStartSignalWidth(0),
fTrigInfo(trig_info),
fTimeShift(time_shift),
fTrigUnion(0) {
    SetRunId(run_id);
    SetEventTime(event_time.AsDouble());
}

// -----   Destructor   ----------------------------------------------------

MpdEventHeader::~MpdEventHeader() {
    delete fTrigInfo;
}

void MpdEventHeader::Clear() {
    fHeaderName = "";
    fEventId = 0;
    fPeriodId = 0;
    fEventTimeTS = TTimeStamp();
    fEventType = (MpdEventType) 0;
    fTripWord = kFALSE;
    fSpillStart = kFALSE;
    fStartSignalTime = 0.0;
    fStartSignalWidth = 0.0;
    if (fTrigInfo) {
        delete fTrigInfo;
        fTrigInfo = new MpdTrigInfo();
    }
    fTrigUnion = 0;
}


ClassImp(MpdEventHeader)
