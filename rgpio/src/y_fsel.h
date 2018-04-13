// 2018-01-04  William A. Hudson
//
// rgpio fsel -- Function Select pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_fsel_P
#define y_fsel_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_fsel class
//--------------------------------------------------------------------------

class y_fsel {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_fsel(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

