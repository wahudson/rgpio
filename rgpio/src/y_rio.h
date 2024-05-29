// 2024-05-25  William A. Hudson
//
// rgpio rio -- Register Input/Output (RIO) RPi5 - RaspberryPi GPIO utility.
//--------------------------------------------------------------------------

#ifndef y_rio_P
#define y_rio_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_rio class
//--------------------------------------------------------------------------

class y_rio {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_rio(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

