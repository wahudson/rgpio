// 2018-01-26  William A. Hudson
//
// rgpio uspi -- Universal SPI Master - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_uspi_P
#define y_uspi_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_uspi class
//--------------------------------------------------------------------------

class y_uspi {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_uspi(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

