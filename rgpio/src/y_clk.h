// 2019-12-15  William A. Hudson
//
// rgpio clk -- Clock control - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#ifndef y_clk_P
#define y_clk_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_clk class
//--------------------------------------------------------------------------

class y_clk {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_clk(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

