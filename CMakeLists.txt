set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/decoder
)

Set(SYSTEM_INCLUDE_DIRECTORIES
 ${ROOT_INCLUDE_DIR} 
)

include_directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
 ${ROOT_LIBRARY_DIR}
 ${FAIRROOT_LIBRARY_DIR}
)
 
link_directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
MpdRawDataDecoder.cxx
MpdTof8Raw2Digit.cxx
MpdTof8Digit.cxx
MpdTDCDigit.cxx
MpdSyncDigit.cxx
MpdEventHeader.cxx
)

Set(HEADERS
MpdRawDataDecoder.h
MpdTof8Raw2Digit.h
MpdTof8Digit.h
MpdTDCDigit.h
MpdSyncDigit.h
MpdEnums.h
MpdEventHeader.h
)
Set(LINKDEF DecoderLinkDef.h)
Set(LIBRARY_NAME Decoder)
Set(DEPENDENCIES Core Base)

GENERATE_LIBRARY()
