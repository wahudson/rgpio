// 2020-09-10  William A. Hudson
//
// rgpio pud -- IO Pin Pull Up/Down - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_pud_P
#define y_pud_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_pud class
//--------------------------------------------------------------------------

class y_pud {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_pud(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

