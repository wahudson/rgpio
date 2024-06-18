// 2024-06-12  William A. Hudson
//
// rgpio rpad -- IO Pads RPi5 - RaspberryPi GPIO utility.
//--------------------------------------------------------------------------

#ifndef y_rpad_P
#define y_rpad_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_rpad class
//--------------------------------------------------------------------------

class y_rpad {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_rpad(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

