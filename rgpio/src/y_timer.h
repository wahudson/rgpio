// 2020-04-25  William A. Hudson
//
// rgpio timer -- System Timer - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_timer_P
#define y_timer_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_timer class
//--------------------------------------------------------------------------

class y_timer {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_timer(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

