//--------------------------------------------------------------------------------------------------------------------------------------
#include "MpdTof8Digit.h"

ClassImp(MpdTof8Digit)
//--------------------------------------------------------------------------------------------------------------------------------------
MpdTof8Digit::MpdTof8Digit()
: fPlane(-1), fStrip(-1), fSide(-1), fAmplitude(-1), fTime(-1)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
MpdTof8Digit::MpdTof8Digit(Short_t plane, Short_t strip, Short_t side,Float_t t,Float_t a)
: fPlane(plane), fStrip(strip), fSide(side), fAmplitude(a), fTime(t)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
MpdTof8Digit::MpdTof8Digit(const MpdTof8Digit *ptr, Float_t t, Float_t a)
: fPlane(ptr->fPlane), fStrip(ptr->fStrip), fSide(ptr->fSide), fAmplitude(a), fTime(t)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
MpdTof8Digit::~MpdTof8Digit()
{

}
//--------------------------------------------------------------------------------------------------------------------------------------
void	MpdTof8Digit::print(const char* comment, std::ostream& os)const
{
	os<<" [MpdTof8Digit] "; if(nullptr != comment) os<<comment;
	os<<"  detID: "<<fPlane<<", stripID: "<<fStrip<<", stripSide: "<<fSide<<", time: "<<fTime<<", ampl.: "<<fAmplitude<<std::endl;
}
//--------------------------------------------------------------------------------------------------------------------------------------

