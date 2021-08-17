//------------------------------------------------------------------------------------------------------------------------
#ifndef __MPDTOF8DIGIT_H
#define	__MPDTOF8DIGIT_H 1

#include "TObject.h"
#include <iostream>
//------------------------------------------------------------------------------------------------------------------------
class MpdTof8Digit : public TObject
{
    Float_t fAmplitude;
    Float_t fTime;
    Short_t fPlane;
    Short_t fStrip;
    Short_t fSide;

public:
    MpdTof8Digit();
    MpdTof8Digit(Short_t plane, Short_t strip, Short_t side, Float_t t, Float_t a);
    MpdTof8Digit(const MpdTof8Digit*, Float_t t, Float_t a);   
    virtual ~MpdTof8Digit();

    Short_t GetPlane()     const { return fPlane;     }
    Short_t GetStrip()     const { return fStrip;     }
    Short_t GetSide()      const { return fSide;      }
    Float_t GetAmplitude() const { return fAmplitude; }
    Float_t GetTime()      const { return fTime;      }
    
    void SetPlane    (Short_t v) { fPlane = v; }
    void SetStrip    (Short_t v) { fStrip = v; }
    void SetSide     (Short_t v)  { fSide = v; }
    void SetAmplitude(Float_t v){ fAmplitude = v; }
    void SetTime     (Float_t v){ fTime = v; }

	void	print(const char* comment = nullptr, std::ostream& os = std::cout)const;


    ClassDef(MpdTof8Digit, 2);
};
//------------------------------------------------------------------------------------------------------------------------
#endif	
