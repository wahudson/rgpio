// 2019-11-29  William A. Hudson
//
// rgpio pwm -- Pulse Width Modulation - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_pwm_P
#define y_pwm_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_pwm class
//--------------------------------------------------------------------------

class y_pwm {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_pwm(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

