// 2020-08-22  William A. Hudson
//
// rgpio man -- man pager - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_man_P
#define y_man_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_man class
//--------------------------------------------------------------------------

class y_man {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_man(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

