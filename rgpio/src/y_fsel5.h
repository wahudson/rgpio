// 2024-05-22  William A. Hudson
//
// rgpio fsel5 -- IO Function Select RPi5 - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_fsel5_P
#define y_fsel5_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_fsel5 class
//--------------------------------------------------------------------------

class y_fsel5 {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_fsel5(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

