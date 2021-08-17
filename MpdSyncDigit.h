#ifndef MPDSyncDIGIT_H
#define MPDSyncDIGIT_H

#include "TNamed.h"

using namespace std;

class MpdSyncDigit : public TObject {
public:

    /** Default constructor **/
    MpdSyncDigit();

    /** Constructor to use **/
    MpdSyncDigit(UInt_t iSerial, Long64_t iEvent, Long64_t t_sec, Long64_t t_ns);

    UInt_t GetSerial() const {
        return fSerial;
    }

    Long64_t GetEvent() const {
        return fGlobalEvent;
    }

    Long64_t GetTime_sec()const {
        return fT_sec;
    }

    Long64_t GetTime_ns() const {
        return fT_ns;
    }

    /** Destructor **/
    virtual ~MpdSyncDigit();

    //private:
    UInt_t fSerial;
    Long64_t fGlobalEvent;
    Long64_t fT_sec;
    Long64_t fT_ns;
    ClassDef(MpdSyncDigit, 1);
};

#endif /* MPDSyncDIGIT_H */

