// 2024-10-11  William A. Hudson
//
// rgpio rcon -- IO Control RPi5 - RaspberryPi GPIO utility.
//--------------------------------------------------------------------------

#ifndef y_rcon_P
#define y_rcon_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_rcon class
//--------------------------------------------------------------------------

class y_rcon {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_rcon(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

