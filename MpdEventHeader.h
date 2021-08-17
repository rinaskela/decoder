#ifndef MPDEVENTHEADER_H
#define MPDEVENTHEADER_H

#include "MpdEnums.h"
#include "MpdTrigInfo.h"

#include "FairEventHeader.h"
#include "FairRootManager.h"

#include "TTimeStamp.h"
#include "MpdTrigUnion.h"

#include <map>
#include <vector>
using namespace std;

class MpdEventHeader : public FairEventHeader {
private:
    /** Event Header branch name **/
    TString fHeaderName;    //!

    /** Event Id **/
    UInt_t fEventId;
    /** Period Id **/
    UInt_t fPeriodId;
    /** Event Time in TTimeStamp **/
    TTimeStamp fEventTimeTS;
    /** Event Type (payload = 0 or pedestal = 1) **/
    MpdEventType fEventType;
    /** Tripped Gems (1 bit for 1 GEM module) **/
    Bool_t fTripWord;
    /** start of new spill **/
    Bool_t fSpillStart;
    /** T0 information for current event**/
    Double_t fStartSignalTime; //ns
    Double_t fStartSignalWidth; //ns
    MpdTrigInfo* fTrigInfo;
    UInt_t fTrigUnion;

    map<UInt_t, Long64_t> fTimeShift;

public:
    /** Default constructor */
    MpdEventHeader();

    /** Constructor */
    MpdEventHeader(UInt_t run_id, UInt_t event_id, TTimeStamp event_time, MpdEventType event_type, Bool_t trip, MpdTrigInfo* trig_info);

    /** Constructor */
    MpdEventHeader(UInt_t run_id, UInt_t event_id, TTimeStamp event_time, MpdEventType event_type, Bool_t trip, MpdTrigInfo* trig_info, map<UInt_t, Long64_t> time_shift);

    /** Destructor */
    virtual ~MpdEventHeader();

    virtual void Register(Bool_t Persistence = kTRUE)
    {
        FairRootManager::Instance()->Register(fHeaderName.Data(), "EvtHeader", this, Persistence);
    }

    void Clear();

    /** Get Event Header branch name */
    TString GetHeaderName() { return fHeaderName; }

    /** Get the run ID for this run */
    UInt_t GetEventId() { return fEventId; }

    /** Get the period ID for this run */
    UInt_t GetPeriodId() { return fPeriodId; }

    /** Get the time for this event */
    TTimeStamp GetEventTimeTS() { return fEventTimeTS; }

    /** Get the type of this event */
    MpdEventType GetEventType() { return fEventType; }

    /** Get the trip word for this event */
    Bool_t GetTripWord() { return fTripWord; }

    /** Get wether new spill is started */
    Bool_t GetSpillStart() { return fSpillStart; }

    Double_t GetStartSignalTime() { return fStartSignalTime; }
    Double_t GetStartSignalWidth() { return fStartSignalWidth; }

    /** Get the spill statistics */
    MpdTrigInfo* GetTrigInfo() { return fTrigInfo; }

    /** Get the trigger state */
    MpdTrigUnion GetTrigState() {MpdTrigUnion u; u.storage = fTrigUnion; return u; }

    map<UInt_t, Long64_t> GetTimeShift() { return fTimeShift; }


    /** Set Event Header branch name */
    void SetHeaderName(TString header_name) { fHeaderName = header_name; }

    /** Set the event ID for this run
     * @param evid : unique event id
     */
    void SetEventId(UInt_t event_id) { fEventId = event_id; }

    /** Set the event ID for this run
     * @param evid : unique event id
     */
    void SetPeriodId(UInt_t period_id) { fPeriodId = period_id; }

    /** Set the time for this event in */
    void SetEventTimeTS(TTimeStamp event_time) { fEventTimeTS = event_time; }

    /** Set the type for this event
     * @param type : type (0 or 1)
     */
    void SetEventType(MpdEventType event_type) { fEventType = event_type; }

    void SetTripWord(Bool_t flag) { fTripWord = flag; }

    void SetSpillStart(Bool_t flag) { fSpillStart = flag; }

    void SetStartSignalInfo(Double_t time, Double_t width)
    {
        fStartSignalTime = time;
        fStartSignalWidth = width;
    }

    void SetTrigInfo(MpdTrigInfo* trig_info)
    {
        if (fTrigInfo) delete fTrigInfo;
        fTrigInfo = new MpdTrigInfo(trig_info);
    }

    /** Set the trigger state */
    void SetTrigState(MpdTrigUnion &v) { fTrigUnion = v.storage; }

    void SetTimeShift(map <UInt_t, Long64_t> time_shift) { fTimeShift = time_shift; }

    ClassDef(MpdEventHeader, 2)
};

#endif /* MpdEVENTHEADER_H */
