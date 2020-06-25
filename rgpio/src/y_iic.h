// 2020-06-17  William A. Hudson
//
// rgpio iic -- Inter-IC (I2C) Master - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_iic_P
#define y_iic_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_iic class
//--------------------------------------------------------------------------

class y_iic {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_iic(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

