// 2021-02-09  William A. Hudson
//
// rgpio pull -- IO Pin Pull Up/Down for RPi4
//--------------------------------------------------------------------------

#ifndef y_pull_P
#define y_pull_P

#include "rgAddrMap.h"
#include "yOption.h"

//--------------------------------------------------------------------------
// y_pull class
//--------------------------------------------------------------------------

class y_pull {
  private:
    yOption		*MainOpx;
    rgAddrMap		*AddrMap;

  public:
    y_pull(			// constructor
	yOption		*opx,
	rgAddrMap	*rgx
    );

    int			doit();
};

#endif

