// 2019-10-27  William A. Hudson
//
// rgpio spi0 -- SPI0 Master - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_spi0_P
#define y_spi0_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_spi0 class
//--------------------------------------------------------------------------

class y_spi0 {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_spi0(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

