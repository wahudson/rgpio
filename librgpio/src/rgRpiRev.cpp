// 2020-11-22  William A. Hudson

// rgRpiRev - Raspberry Pi Revision
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <fstream>	// file stream
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgRpiRev.h"


//==========================================================================
//## rgWord::  Base Class
//==========================================================================

/*
* Constructor.
* call:
*    rgWord		rx;
*/
rgWord::rgWord()
{
    WordVal = 0;
    Final   = 0;
}

/*
* Word bits are numbered [31:0], left to right.
* Field position is the bit number of the field LSB.
* Mask is a contiguous bit-field of ones, and is the maximum field value.
* These are protected because safety relies on proper arguments.
* Intended only for use by derived registers.
*/

/*
* Extract field value out of Word.  (protected)
* call:
*    value = get_field( 20, 0xfff )	// bits [31:20]
*/
uint32_t
rgWord::get_field(
    const uint32_t		pos,	// field bit position of LSB
    const uint32_t		mask	// mask field size
)
{
    return  ( (WordVal >> pos) & mask );
}


/*
* Insert field value into Word.  (protected)
*    Value is range checked.
* call:
*    put_field( 16, 0xff, value )	// bits [23:16]
* exception:
*    range_error  if ( value > mask )
*/
void
rgWord::put_field(
    const uint32_t		pos,	// field bit position of LSB
    const uint32_t		mask,	// mask field size
    const uint32_t		value	// value to insert
)
{
    if ( value > mask ) {
	std::ostringstream	css;
	css << "rgWord::put_field():  value exceeds 0x" <<hex << mask
	    << ":  0x" <<hex << value;
	throw std::range_error ( css.str() );
    }

    WordVal = (WordVal & ~(mask << pos)) | (value << pos);
}


//==========================================================================
//## rgRpiRev - Main and sub-classes - Constructors
//==========================================================================

/*
* Class Data.
*     Initialized by the constructors.
*/
rgRpiRev	rgRpiRev::Config;

/*
* Constructor sub-class.
* call:
*    rgRpiRev_Code		rx;
*/
rgRpiRev_Code::rgRpiRev_Code()
{
    InFile = "/proc/cpuinfo";
}

/*
* Constructor sub-class.
* call:
*    rgRpiRev::rgRpiRev_Soc	rx;
*/
rgRpiRev::rgRpiRev_Soc::rgRpiRev_Soc()
{
    RevCode_ptr = NULL;
    FailDerive  = 0;
}

/*
* Constructor sub-class.
* call:
*    rgRpiRev::rgRpiRev_Base	rx;
*/
rgRpiRev::rgRpiRev_Base::rgRpiRev_Base()
{
    SocEnum_ptr = NULL;
}

/*
* Constructor main class.
* call:
*    rgRpiRev		rx;
*/
rgRpiRev::rgRpiRev()
{
    SocEnum.init_ptr(  &RevCode );
    BaseAddr.init_ptr( &SocEnum );

//    SocEnum.put(  rgRpiRev::soc_BCM2836 );
//    BaseAddr.put( 0xabbabaab );

}

//--------------------------------------------------------------------------
// rgRpiRev_Soc  class functions.
//--------------------------------------------------------------------------
// These override rgWord functions accessing (uint32_t WordVal).
// Alternative is to make rgWord a template class.

/*
* Put WordVal
*    Range check here ensures that WordVal can never be invalid on get().
*    Clears FailDerive flag.
* call:
*    put( soc );
*    soc = soc enum
* exceptions:
*    std::runtime_error
*/
void
rgRpiRev::rgRpiRev_Soc::put(
    rgRpiRev::Soc_enum	soc
)
{
    if (!( (0 <= soc) && (soc <= soc_MaxEnum) )) {
	std::ostringstream      css;
	css << "rgRpiRev::rgRpiRev_Soc::put() enum out of range:  " << soc;
	throw std::runtime_error ( css.str() );
    }

    WordVal    = soc;	// promote to uint32_t by assignment
    FailDerive = 0;
}


//--------------------------------------------------------------------------
// rgRpiRev_Code - read revision code functions.
//--------------------------------------------------------------------------

/*
* Read stream for RPi cpuinfo and return Revision Code word.
*    Match regular expression:  ^Revision.*:\s*[hex]$
*    where [hex] is a valid hex number string according to stoul().
* Typical revision line looks like:
*    Revision        : a22082
*    Revision\t: a22082
* call:
*    read_rev_code( &istm );
*    istm  = input file stream
* return:
*    ()  = revision code, 0= failure
* exceptions:
*    std::runtime_error
*/
uint32_t
rgRpiRev_Code::read_rev_code(
    istream*		istm
)
{
    uint32_t		revcode = 0;	// return value if not found
    std::string		stc;
    size_t		ii;

    while ( ! istm->eof() )
    {
	if ( istm->get() != 'R' ) {
	    while ( ! istm->eof() && (istm->get() != '\n') )  {}
	    continue;		// start of a line
	}

	std::getline( *istm, stc );		// discarding new-line

	if ( 0 != stc.compare( 0, 7, "evision" ) ) { continue; }
//	cout << stc <<endl;

	ii = stc.find_first_of( ':' );		// index of colon
	if ( ii == string::npos ) { continue; }	// no colon
//	cout << "find ii= " << ii <<endl;
	// Not validating white-space before the colon.

	stc.erase( 0, ii+1 );
//	cout << "+ erased=" << stc << "=" <<endl;

	revcode = std::stoul( stc, &ii, 16 );	// may throw
	//#!! try {} catch

	if ( stc[ii] != 0 ) {		// not end of string
	    revcode = 0;
	    std::ostringstream		css;
	    css << "read_rev_code() unexpected trailing char:  " << stc;
	    throw std::runtime_error ( css.str() );
	}
	//#!! should we care here?

	break;
    }

    return  revcode;
}


/*
* Read file for RPi cpuinfo and return Revision Code word.
* call:
*    read_rev_code();
*    ifile  = input file name
* return:
*    ()  = revision code, 0= failure
* exceptions:
*    std::runtime_error
*/
uint32_t
rgRpiRev_Code::read_rev_code(
    const char*		ifile
)
{
    uint32_t		revcode = 0;	// return value if not found

    std::ifstream	istm  ( ifile );

//  cout << (( istm.is_open() ) ? " IS" : "NOT") << " open:  " << ifile <<endl;
//  cout << (( istm.good()    ) ? " IS" : "NOT") << " good:  " << ifile <<endl;
//  cout << (( istm.eof()     ) ? " IS" : "NOT") << " eof:   " << ifile <<endl;

    if ( ! istm.is_open() ) {
	std::ostringstream		css;
	css << "read_rev_code() cannot open file:  " << ifile;
	throw std::runtime_error ( css.str() );
    }

    revcode = read_rev_code( &istm );

    istm.close();

    return  revcode;
}

//--------------------------------------------------------------------------
// rgRpiRev - Primary find() functions.
//--------------------------------------------------------------------------

/*
* Find RevCode.
*    Normally return cached code.
*    If not final, then read from InFile, normally "/proc/cpuinfo".
*    If derivation failed, then set RevCode to zero and mark final.
* call:
*    find();
* return:
*    ()  = revision code, null if derivation failed
* exceptions:
*    std::runtime_error
*/
uint32_t
rgRpiRev_Code::find()
{
    uint32_t		code = 0;	// default fail

    if ( ! Final ) {
	Final   = 1;
	WordVal = 0;	// flag failed

	code = read_rev_code( InFile );		// may throw exception
//	cout << "find:  " << code <<endl;

	if ( code ) {
	    WordVal = code;
	}
	else {
	    std::ostringstream      css;
	    css << "Extract Revision failed from file:  " << InFile;
	    throw std::runtime_error ( css.str() );
	}
    }

    return  WordVal;
}


/*
* Find SocEnum.
*    Normally return cached enum.
*    If not final, then derive enum from RevCode.
*    If derivation failed, then leave enum unchanged and mark final.
*    User must check is_fail() to determine if result is valid.
* call:
*    find();
* return:
*    ()  = SOC chip id enum
* exceptions:
*    std::runtime_error
*/
rgRpiRev::Soc_enum
rgRpiRev::rgRpiRev_Soc::find()
{
    Soc_enum		soc = soc_BCM2837;	// default RPi3 ??
    uint32_t		code;			// revision code
    uint32_t		num;			// chip number

    if ( Final ) {
	return  get();
    }

    Final = 1;
    FailDerive = 1;	// flag failed

    code = RevCode_ptr->find();			// may throw exception

    if ( code ) {
	num = RevCode_ptr->get_ChipNumber_4();

	if ( num < rgRpiRev::soc_MaxEnum ) {
	    soc  = (rgRpiRev::Soc_enum) num;	// cast to enum
	    put( soc );
	    FailDerive = 0;	// not failed
	}
	else {
	    std::ostringstream      css;
	    css << "rgRpiRev_Soc::find() ChipNumber has no enum:  " << num;
	    throw std::runtime_error ( css.str() );
	}
    }
    else {
	std::ostringstream      css;
	css << "rgRpiRev_Soc::find() RevCode failed";
	throw std::runtime_error ( css.str() );
    }

    return  get();
}


/*
* Find BaseAddr.
*    Normally return cached address.
*    If not final, then derive address from SocEnum.
*    If derivation failed, then set address to zero and mark final.
* call:
*    find();
* return:
*    ()  = real base address, null if derivation failed
* exceptions:
*    std::runtime_error
*/
uint32_t
rgRpiRev::rgRpiRev_Base::find()
{
    Soc_enum		soc;
    uint32_t		addr;

    if ( ! Final ) {
	Final   = 1;
	WordVal = 0;	// flag failed

	soc   = SocEnum_ptr->find();		// may throw exception

	if ( SocEnum_ptr->is_fail() ) {
	    std::ostringstream      css;
	    css << "rgRpiRev_Base::find() derive SocEnum failed";
	    throw std::runtime_error ( css.str() );
	}
	else {
	    switch ( soc ) {
	    case  soc_BCM2835:
		    addr = 0x20000000;
		    break;
	    case  soc_BCM2836:
	    case  soc_BCM2837:
		    addr = 0x3f000000;
		    break;
	    case  soc_BCM2711:
		    addr = 0xfe000000;
		    break;
	    }

	    WordVal = addr;
	}
    }

    return  WordVal;
}

//--------------------------------------------------------------------------
// Soc_enum  Conversions
//--------------------------------------------------------------------------

/*
* Return SOC chip name string given an Soc_enum.
* call:
*    soc_enum2cstr( soc );
* return:
*    ()  = enum name string, without prefix "soc_"
* exception:
*    Throw range_error if enum is invalid.
*/
const char*
rgRpiRev::soc_enum2cstr(
    Soc_enum		soc
)
{
    const char*		strp = NULL;	// return pointer

    switch ( soc ) {
	case  soc_BCM2835:   strp = "BCM2835";   break;
	case  soc_BCM2836:   strp = "BCM2836";   break;
	case  soc_BCM2837:   strp = "BCM2837";   break;
	case  soc_BCM2711:   strp = "BCM2711";   break;
    }
    // No "default:" so compiler checks all enums are represented.

    if ( ! strp ) {
	std::ostringstream      css;
	css << "rgRpiRev::soc_enum2cstr() invalid Soc_enum:  " << soc;
	throw std::range_error ( css.str() );
    }

    return  strp;
}


/*
* Return Soc_enum for an int.
*    The int value does not change, but the type does.
* call:
*    int2soc_enum( 2 );
* return:
*    ()  = rgRpiRev::Soc_enum	enum value
* exception:
*    Throw range_error if int is invalid, not in {0, .. ,soc_MaxEnum}.
*/
rgRpiRev::Soc_enum
rgRpiRev::int2soc_enum(
    int			num
)
{
    if (!( (0 <= num) && (num <= soc_MaxEnum) )) {
	std::ostringstream      css;
	css << "rgRpiRev::int2soc_enum() int out of range:  " << num;
	throw std::range_error ( css.str() );
    }

    return  (Soc_enum) num;
}

