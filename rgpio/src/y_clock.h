// 2018-01-14  William A. Hudson
//
// rgpio clock -- Clock control - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_clock_P
#define y_clock_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_clock class
//--------------------------------------------------------------------------

class y_clock {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_clock(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

