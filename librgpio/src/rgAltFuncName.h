// 2018-01-05  William A. Hudson

#ifndef rgAltFuncName_P
#define rgAltFuncName_P

#include "rgFselPin.h"

//--------------------------------------------------------------------------
// rGPIO Alternate Function Names - documentation
//--------------------------------------------------------------------------

class rgAltFuncName {

  public:
    static const char*		AltFunc[][6];
    static const char*		AltDelta4[][6];
    static const int		altN_enum[];

  public:
    rgAltFuncName();			// constructor

    static const char*		cstr_altfunc_bit(
	rgFselPin::rgFsel_enum      mode,
	int                         bit
    );

};

#endif

