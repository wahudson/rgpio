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
rgRpiRev	rgRpiRev::Global;


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
    SocVal      = soc_BCM2835;
    RevCode_ptr = NULL;
    Final       = 0;
    Unknown     = 1;
}

/*
* Constructor sub-class.
* call:
*    rgRpiRev::rgRpiRev_Base	rx;
*/
rgRpiRev::rgRpiRev_Base::rgRpiRev_Base()
{
    BaseVal     = 0;
    SocEnum_ptr = NULL;
    Final       = 0;
    Unknown     = 1;
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
}

//--------------------------------------------------------------------------
// rgRpiRev_Soc  functions.
//--------------------------------------------------------------------------

/*
* Override SocVal
*    Set Final flag, clear Unknown flag.
* call:
*    override( rgRpiRev::soc_BCM2837 );
* exceptions:
*    std::runtime_error  - enum out of range
*/
void
rgRpiRev::rgRpiRev_Soc::override(
    rgRpiRev::Soc_enum	soc
)
{
    if (!( (0 <= soc) && (soc <= soc_MaxEnum) )) {
	std::ostringstream      css;
	css << "rgRpiRev::rgRpiRev_Soc::override() enum out of range:  " << soc;
	throw std::runtime_error ( css.str() );
    }

    SocVal     = soc;
    Final      = 1;
    Unknown    = 0;
}


/*
* Default SocVal
*    Clear Final flag, set Unknown flag.
* call:
*    defaultv( rgRpiRev::soc_BCM2837 );
* exceptions:
*    std::runtime_error  - enum out of range
*/
void
rgRpiRev::rgRpiRev_Soc::defaultv(
    rgRpiRev::Soc_enum	soc
)
{
    if (!( (0 <= soc) && (soc <= soc_MaxEnum) )) {
	std::ostringstream      css;
	css << "rgRpiRev::rgRpiRev_Soc::defaultv() enum out of range:  " << soc;
	throw std::runtime_error ( css.str() );
    }

    SocVal     = soc;
    Final      = 0;
    Unknown    = 1;
}

//--------------------------------------------------------------------------
// rgRpiRev_Code - read revision code functions.
//--------------------------------------------------------------------------

/*
* Read stream for RPi cpuinfo and return Revision Code word.
*    Match regular expression:  ^Revision.*:\s*[hex]$
*    where [hex] is a valid hex number string according to stoul().
*    A zero return value indicates not an RPi.  Errors throw exception.
* Typical revision line looks like:
*    Revision        : a22082
*    Revision\t: a22082
* call:
*    read_rev_code( &istm );
*    istm  = input file stream
* return:
*    ()  = revision code, 0= non-RPi, Revision line not found
* exceptions:
*    std::runtime_error		Errors in Revision line format.
*/
uint32_t
rgRpiRev_Code::read_rev_code(
    istream*		istm
)
{
    uint32_t		revcode = 0;	// return value if not found
    std::string		stln;
    size_t		ii;
    int			cx;

    while ( ! istm->eof() )
    {
	if ( (cx = istm->get()) != 'R' ) {
	    while ( (! istm->eof()) && (cx != '\n') )  {
		cx = istm->get();
	    }
	    continue;		// start of a line
	}

	std::getline( *istm, stln );		// discarding new-line

	if ( 0 != stln.compare( 0, 7, "evision" ) ) { continue; }
//	cout << stln <<endl;

	ii = stln.find_first_of( ':' );		// index of colon
	if ( ii == string::npos ) { continue; }	// no colon
	// Not validating white-space before the colon.

	string		stnu  (stln, ii+1);	// construct substring
//	cout << "+ stnu="<< stnu << "=" <<endl;

	try {
	    revcode = std::stoul( stnu, &ii, 16 );	// may throw
	}
	catch (...) {
	    revcode = 0;
	}

	if ( ii != stnu.length() ) {		// not end of string
	    revcode = 0;
	}

	if ( ! revcode ) {
	    std::ostringstream		css;
	    css << "read_rev_code() bad line:  'R" << stln << "'";
	    throw std::runtime_error ( css.str() );
	}

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
*    ()  = revision code, 0= non-RPi, Revision line not found
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
*    If read failed, then infer non-RPI, leave default code unchanged and
*    mark Unknown, otherwise mark Final and known.
* call:
*    find();
* return:
*    ()  = revision code, unchanged value if non-RPi
*    RealPi=1, Unknown=0  on success
*    RealPi=0, Unknown=1  on failure indicating non_RPi
* exceptions:
*    std::runtime_error		Errors reading InFile
*/
uint32_t
rgRpiRev_Code::find()
{
    uint32_t		code;			// revision code

    if ( ! Final ) {
	Final   = 1;
	RealPi  = 0;	// non-RPi
	Unknown = 1;	// flag failed
	// WordVal	// leave default unchanged

	code = read_rev_code( InFile );		// may throw exception

	if ( code ) {		// real-RPi
	    RealPi  = 1;
	    Unknown = 0;	// success
	    WordVal = code;
	}
    }

    return  WordVal;
}


/*
* Find SocEnum.
*    Normally return cached enum.
*    If not final, then derive enum from RevCode.
*    If derivation failed, then leave enum unchanged and mark final.
*    User must check is_unknown() to determine if result is valid.
* call:
*    find();
* return:
*    ()  = SOC chip id enum
*    Unknown :  0= all OK, 1= non-RPi or derivation error
* exceptions:
*    std::runtime_error		Error in derivation
* Note:  NewStyle_1 is not checked because ChipNumber_4 is correct either way.
*/
rgRpiRev::Soc_enum
rgRpiRev::rgRpiRev_Soc::find()
{
    Soc_enum		soc;
    uint32_t		num;			// chip number

    if ( ! Final ) {
	Final   = 1;
	Unknown = 1;		// flag failed
	// SocVal		// leave default unchanged

	RevCode_ptr->find();		// may throw exception

	if ( ! RevCode_ptr->is_unknown() ) {

	    num = RevCode_ptr->get_ChipNumber_4();

	    try {
		soc = rgRpiRev::int2soc_enum( num );
	    }
	    catch (...) {
		std::ostringstream      css;
		css << "rgRpiRev_Soc::find() ChipNumber has no enum:  " << num;
		throw std::runtime_error ( css.str() );
	    }

	    Final   = 1;
	    Unknown = 0;	// success
	    SocVal  = soc;
	}
	// otherwise no change to SocVal
    }

    return  SocVal;
}


/*
* Find BaseAddr.
*    Normally return cached address.
*    If not final, then derive address from SocEnum.
*    If derivation failed, then set address to zero and mark final.
* call:
*    find();
* return:
*    ()  = real base address, null if non-RPi or derivation error
* exceptions:
*    std::runtime_error		Error in derivation
*/
uint64_t
rgRpiRev::rgRpiRev_Base::find()
{
    Soc_enum		soc;
    uint64_t		addr;

    if ( ! Final ) {
	Final   = 1;
	Unknown = 1;
	BaseVal = 0;	// flag failed

	soc   = SocEnum_ptr->find();		// may throw exception

	if ( ! SocEnum_ptr->is_unknown() ) {
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
	    case  soc_BCM2712:
		    addr = 0x1f00000000;
		    break;
	    }
	    // No "default:" so compiler checks all enums are represented.

	    Final   = 1;
	    Unknown = 0;	// success
	    BaseVal = addr;
	}
    }

    return  BaseVal;
}

//--------------------------------------------------------------------------
// Simulation overrides
//--------------------------------------------------------------------------

/*
* Force simulation of given SocEnum or RevCode.
*    Force SocEnum to be Final and not Unknown.
*    Force RevCode and RealPi to be Final and not Unknown.
*    BaseAddr is not forced zero, so it can be derived in simulation.
*    All override() or defaultv() functions should be called first.
* call:
*    simulate_SocEnum( rgRpiRev::soc_BCM2837 );
*    simulate_RevCode( 0x00a22082 );
*    simulate();		// simulate existing default values
*/
void
rgRpiRev::simulate_SocEnum( Soc_enum soc )
{
    Global.SocEnum.override( soc );
    Global.RevCode.override_realpi( 0 );	// 0= simulation or non-RPi
}

void
rgRpiRev::simulate_RevCode( uint32_t code )
{
    Global.RevCode.override( code );
    Global.RevCode.override_realpi( 0 );	// 0= simulation or non-RPi
}

void
rgRpiRev::simulate()
{
    Global.RevCode.override_realpi( 0 );	// 0= simulation or non-RPi
}

//--------------------------------------------------------------------------
// IO Capability
//--------------------------------------------------------------------------

/*
* Initialize the IO Platform Capability flags.
*    Calls all the find() functions to be sure everything is derived.
*    All override() and simulate*() functions should be called first.
*    Any changes after this function is called will not be applied.
* call:
*    initialize_ioRPi();
* exceptions:
*    std::runtime_error		Error in derivation
*/
void
rgRpiRev::initialize_ioRPi()
{
    Soc_enum		soc;

    Global.IoRPi0 = 0;
    Global.IoRPi3 = 0;
    Global.IoRPi4 = 0;
    Global.IoRPi5 = 0;

    Global.RevCode.find();

    if ( Global.RevCode.get_realpi() && Global.RevCode.is_unknown() ) {
	std::ostringstream      css;
	css << "rgRpiRev::initialize_ioRPi() RevCode is Unknown";
	throw std::runtime_error ( css.str() );
    }

    soc = Global.SocEnum.find();

    if ( Global.RevCode.get_realpi() && Global.SocEnum.is_unknown() ) {
	std::ostringstream      css;
	css << "rgRpiRev::initialize_ioRPi() SocEnum is Unknown";
	throw std::runtime_error ( css.str() );
    }

    Global.BaseAddr.find();

    if ( Global.RevCode.get_realpi() && Global.BaseAddr.is_unknown() ) {
	std::ostringstream      css;
	css << "rgRpiRev::initialize_ioRPi() BaseAddr is Unknown";
	throw std::runtime_error ( css.str() );
    }

    switch ( soc ) {
    case  soc_BCM2835:
    case  soc_BCM2836:
	    Global.IoRPi0 = 1;
	    break;
    case  soc_BCM2837:
	    Global.IoRPi3 = 1;
	    break;
    case  soc_BCM2711:
	    Global.IoRPi4 = 1;
	    break;
    case  soc_BCM2712:
	    Global.IoRPi5 = 1;
	    break;
    }
    // No "default:" so compiler checks all enums are represented.
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
	case  soc_BCM2712:   strp = "BCM2712";   break;
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

