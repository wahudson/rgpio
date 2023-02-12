// 2023-02-05  William A. Hudson
//
// rgpio header -- Header Pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_header_P
#define y_header_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_header class
//--------------------------------------------------------------------------

class y_header {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_header(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

