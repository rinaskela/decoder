/**
 * \file MpdEnums.h

 **/

#ifndef MPDENUMS_H_
#define MPDENUMS_H_

/**
 * \enum Define type of hit on detector station.
 */
enum MpdHitType {
    kMPDSTRIPHIT = 0, /**< Strip hit. */
    kMPDPIXELHIT = 1, /**< Pixel hit. */
    kMPDMIXHIT = 2 /**< Mixed type of hits, i.e. both strip and pixel hits. **/
};

/**
 * \enum Define status code.
 */
enum MpdStatus {
    kMPDSUCCESS = 0, /**< Success. */
    kMPDERROR = 1, /**< Error. */
    kMPDTIMEOUT = 2,
    kMPDFINISH = 3
};

/**
 * \enum Define reconstructed track quality.
 */
enum MpdTrackQa {
    kMPDGOOD = 0, /**< Good track. */
    kMPDBAD = 1, /**< Bad track. */
    kMPDGOODMERGE = 2 /**< Correctly merged track. */
};

/**
 * \enum Define system identificator.
 */
enum MpdSystemId {
    kMPDGEM = 0, /**< GEM detector. */
    kMPDTOF = 2, /**< TOF detector. */
    kMPDSTS = 4 /**< STS detector. */
};

/**
 * \enum Define status of TOF hit merging
 */
enum MpdTofHitQA {
    kMPDTOFGOOD = 0, /**< merged TOF hit. */
    kMPDTOFBAD = 1, /**< not merged TOF hit. */
};

/**
 * \enum Define type of event
 */
enum MpdTriggerType {
    kMPDBEAM = 6, /**< "beam"  BC1 + BC2 + T0 + VC */
    kMPDMINBIAS = 1, /**< "min.bias" BC1 + BC2 + T0 + VC + FD */
};

/**
 * \enum Define type of event
 */
enum MpdEventType {
    kMPDPEDESTAL = 0, /**< pedestal event. */
    kMPDPAYLOAD = 1, /**< spill event. */
};

/**
 * \enum Define worker state
 */
enum MpdWorkerState {
    kMPDWAIT = 0,
    kMPDWORK = 1,
    kMPDRECON = 2,
};

/**
 * \enum Define experimental setup
 */
enum MpdSetup {
    kMPDSETUP = 0,
    kSRCSETUP = 1,
};

/**
 * \enum Define ADC strip decoding algorithm
 */
enum MpdADCDecoMode {
    kMPDADCMK = 0,
    kMPDADCSM = 1,
};

#endif /* MpdENUMS_H_ */
