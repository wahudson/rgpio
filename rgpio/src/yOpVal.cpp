// 2018-01-26  William A. Hudson
//
// Option Argument class.
//     Idea is each option is an object.
//--------------------------------------------------------------------------

#include <stdlib.h>	// strtoul()
#include <sstream>	// std::ostringstream
#include <stdexcept>

#include "yOpVal.h"


/*
* Constructor.
*/
yOpVal::yOpVal()
{
    Given = 0;
    Val   = 0;
}


/*
* Set object value (uint32_t Val) from a string.
*    Mark Given=1.
*    The char string is converted to uint32_t.
*    Throw exception if string is non-numeric.
* call:
*    Object.set( arg )
* exceptions:
*    std::invalid_argument  - option value is non-numeric
*    Typically arrange the catch to prepend the text:
*        "Error:  invalid argument:  "
*
* Note:  An alternative is to call Error::msg() and then return zero, which
*    would allow continued option processing until errors are checked.
*    The disadvantages are continued processing with a bad value and
*    testing is more complex with a global error state.
* Unfortunately the option name is not available for the error message.
*/
void
yOpVal::set( const char*  arg )
{
    char	*endptr;

    Given = 1;
    Val   = strtoul( arg, &endptr, 0 );

    if ( *endptr != '\0' ) {
	std::ostringstream	css;
	css << "non-numeric option value:  " << arg;
	throw std::invalid_argument ( css.str() );
    }
}

