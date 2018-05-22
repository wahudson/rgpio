// 2018-05-18  William A. Hudson
//
// Utility functions.
//--------------------------------------------------------------------------

#include "yUtil.h"


/*
* Format a 32-bit word into a binary bit string.
*    Returns a char string representing val[31:0] in binary, space seperated
*    into groups of 4 bits.  e.g.
*     3           2            1           0  bit index
*    1098 7654 3210 9876 5432 1098 7654 3210  [31:0]
*    ---- ---- ---- ---- ---- ---- ---- ----
*   "0001 0010 0011 0100 0101 0110 0111 1000" value in binary
* call:
*    cstr_bits32( 0x12345678 )
* return:
*    () = pointer to a char string, internal static storage.  Caller must
*        use or copy the string before the next call changes it.
*/
char*
cstr_bits32( uint32_t  val )
{
    static char		bits[40];
    int			jj = 39;	// index into bits[], count down

    bits[jj--] = '\0';			// null char terminates string

    while ( 1 )
    {
	for ( int i=1;  i<=4;  i++ )	// 4-bit chunks
	{
	    bits[jj--] = (val & 0x1) + '0';
	    val = val >> 1;
	}

	if ( jj <= 0 )  break;
	bits[jj--] = ' ';		// space seperator
    }

    return  bits;
}

