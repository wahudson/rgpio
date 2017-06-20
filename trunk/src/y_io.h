// 2017-06-18  William A. Hudson
//
// rgpio io -- IO pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_io_P
#define y_io_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_io class
//--------------------------------------------------------------------------

class y_io {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_io(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

