// 2020-12-17  William A. Hudson
//
// rgpio info -- RPi Revision Information - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_info_P
#define y_info_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_info class
//--------------------------------------------------------------------------

class y_info {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_info(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

