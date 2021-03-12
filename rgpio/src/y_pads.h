// 2021-03-10  William A. Hudson
//
// rgpio pads -- Pads Control - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_pads_P
#define y_pads_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_pads class
//--------------------------------------------------------------------------

class y_pads {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_pads(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

