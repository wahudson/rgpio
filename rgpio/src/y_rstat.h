// 2024-10-17  William A. Hudson
//
// rgpio rstat -- IO Status RPi5 - RaspberryPi GPIO utility.
//--------------------------------------------------------------------------

#ifndef y_rstat_P
#define y_rstat_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_rstat class
//--------------------------------------------------------------------------

class y_rstat {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_rstat(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

