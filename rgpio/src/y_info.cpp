// 2020-12-17  William A. Hudson
//
// rgpio info -- RPi Revision Information - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgRpiRev.h"

#include "yUtil.h"
#include "y_info.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class info_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    const char*		file;
    yOpVal		code;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    info_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    info_yOptLong	Opx  ( opx );
*/
info_yOptLong::info_yOptLong( yOption  *opx )
    : yOption( opx )
{
    file        = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
info_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--file="      )) { file       = this->val(); }
	else if ( is( "--code="      )) { code.set(    this->val() ); }
	else if ( is( "--verbose"    )) { verbose    = 1; }
	else if ( is( "-v"           )) { verbose    = 1; }
	else if ( is( "--debug"      )) { debug      = 1; }
	else if ( is( "--TESTOP"     )) { TESTOP     = 1; }
	else if ( is( "--help"       )) { this->print_usage();  exit( 0 ); }
	else if ( is( "-"            )) {                break; }
	else if ( is( "--"           )) { this->next();  break; }
	else {
	    Error::msg( "unknown option:  " ) << this->current_option() <<endl;
	}
    }

    if ( *file && code.Given ) {
	Error::msg( "choose only one:  --file= or --code=" ) << endl;
    }

    if ( get_argc() > 0 ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }
}


/*
* Show option flags.
*/
void
info_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--file        = " << file         << endl;
    cout << "--code        = " << code.Val     << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    cout <<std::dec;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }
}


/*
* Show usage.
*/
void
info_yOptLong::print_usage()
{
    cout <<
    "    RPi Revision Information\n"
    "usage:  " << ProgName << " info [options..]\n"
    "  decode:  (default is rgRpiRev::Global)\n"
    "    --file=/proc/cpuinfo  read revision code from file\n"
    "    --code=0x00000000     decode this revision code\n"
    "  options:\n"
    "    --help              show this usage\n"
//  "    -v, --verbose       verbose output, show registers in binary\n"
//  "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


/*
* Output a trace message.
*    This function provides custom configuration of:
*    Option for conditional output.  Output stream used.
*    Argument text should have no prefix or suffix new-line.
* call:
*    trace_msg( "text" );
*/
void
info_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_info::y_info(
    yOption		*opx,
    rgAddrMap		*rgx
) {
    AddrMap = rgx;
    MainOpx = opx;
}


//--------------------------------------------------------------------------
// Main body
//--------------------------------------------------------------------------

int
y_info::doit()
{

    {
	info_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgRpiRev		Rvx;	// for local config
	rgRpiRev*		rpx;	// pointer to global or local
	uint32_t		code;
	rgRpiRev::Soc_enum	soc_enum = rgRpiRev::soc_BCM2835;
	const char*		soc_name = "unknown";

	rpx = &rgRpiRev::Global;	// point at global

	if ( *Opx.file ) {
	    rpx = &Rvx;			// point at local
	    rpx->RevCode.init_file( Opx.file );
	}

	if ( Opx.code.Given ) {
	    rpx = &Rvx;			// point at local
	    rpx->RevCode.override( Opx.code.Val );
	}

	rgRpiRev_Code&		Rcx = rpx->RevCode;	// reference

	code = Rcx.find();		// may throw exception

	try {
	    soc_enum = rpx->SocEnum.find();
	    soc_name = rgRpiRev::soc_enum2cstr( soc_enum );
	}
	catch ( std::exception& e ) {
	    Error::msg( "SocEnum exception:  " ) << e.what() <<endl;
	}

	cout << "                    bit:      28   24   20   16   12    8    4    0" << endl;

	cout.fill('0');
	cout <<hex
	    << "  RevCode   = 0x" <<setw(8) << code << "    " <<
				    cstr_bits32( code ) <<endl;

	cout.fill(' ');
	cout <<dec
	    << "    OverVoltageDis_1 = " << Rcx.get_OverVoltageDis_1() <<endl
	    << "    OtpWriteDis_1    = " << Rcx.get_OtpWriteDis_1()    <<endl
	    << "    OtpReadDis_1     = " << Rcx.get_OtpReadDis_1()     <<endl
	    << "    WarrantyVoid_1   = " << Rcx.get_WarrantyVoid_1()   <<endl
	    << "    NewStyle_1       = " << Rcx.get_NewStyle_1()       <<endl
	    << "    MemSize_3        = " << Rcx.get_MemSize_3()        <<endl
	    << "    MfgNumber_4      = " << Rcx.get_MfgNumber_4()      <<endl
	    << "    ChipNumber_4     = " << Rcx.get_ChipNumber_4()     <<endl
	    << "    BoardType_8      = " << Rcx.get_BoardType_8()      <<endl
	    << "    BoardRev_4       = " << Rcx.get_BoardRev_4()       <<endl
	    ;

	cout.fill('0');
	cout <<hex
	    << "  SocEnum   = soc_" << soc_name <<endl
	    << "  BaseAddr  = 0x"   <<setw(8) << rpx->BaseAddr.find() <<endl
	    ;

	if ( ! Opx.code.Given ) {
	    cout << "  InFile    = " << Rcx.init_file() << endl;
	}

	if ( code == 0 ) {
	    cout << "NOT on RPi" <<endl;
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

