// 2018-01-04  William A. Hudson
//
// rgpio fsel -- Function Select pins - RaspberryPi GPIO utility program.
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
#include "rgFselPin.h"

#include "rgAltFuncName.h"

#include "yUtil.h"
#include "y_fsel.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class fsel_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		show;

    bool		Word0 = 0;	// -0
    bool		Word1 = 0;	// -1

    yOpVal		w0;
    yOpVal		w1;

    const char*		mode;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    rgFselPin::rgFsel_enum	mode_e;

    bool		ModifyBits = 0;

  public:
    fsel_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		cout_fsel_mask_head();
    void		cout_fsel_mask();
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    fsel_yOptLong	Opx  ( opx );
*/
fsel_yOptLong::fsel_yOptLong( yOption  *opx )
    : yOption( opx )
{
    show        = 0;

    mode        = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    mode_e      = rgFselPin::f_In;
}


/*
* Parse options.
*/
void
fsel_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--show"       )) { show       = 1; }

	else if ( is( "-0"           )) { Word0      = 1; }
	else if ( is( "-1"           )) { Word1      = 1; }

	else if ( is( "--w0="        )) { w0.set(      this->val() ); }
	else if ( is( "--w1="        )) { w1.set(      this->val() ); }

	else if ( is( "--mode="      )) { mode       = this->val(); }

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

    if ( *mode && (get_argc() > 0) ) {
	ModifyBits = 1;
    }

    if ( (get_argc() > 0) && (Word0 || Word1) ) {
	Error::msg( "disallow -0, -1 with bit number list" ) <<endl;
    }

    if ( (get_argc() > 0) && (w0.Given || w1.Given) ) {
	Error::msg( "disallow --w0, --w1 with bit number list" ) <<endl;
    }

    if ( !((get_argc() > 0) || Word0 || Word1 ) ) {	// no bit specified
	if ( ! *mode ) {
	    Word0 = 1;				// set default
	}
    }

    if ( (w0.Given || w1.Given) && ! *mode ) {
	Error::msg( "--w0, --w1 requires --mode" ) <<endl;
    }

    if ( *mode ) {
	try {
	    mode_e = rgFselPin::find_rgFsel_enum( mode );
	}
	catch ( range_error& e ) {
	    Error::msg( "unknown Fsel mode:  --mode=" ) << mode <<endl
		<< "    " <<  e.what() << endl;
	}

	if ( show ) {
	    Error::msg( "--mode not valid with --show" ) <<endl;
	}

	if ( Word0 || Word1 ) {
	    Error::msg( "disallow -0, -1 to modify full words" ) <<endl;
	}

	if ( !((get_argc() > 0) || w0.Given || w1.Given) ) {
	    Error::msg( "--mode requires --w0, --w1, or bit numbers" ) <<endl;
	}
    }
}


/*
* Show option flags.
*/
void
fsel_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--show        = " << show         << endl;
    cout << "-0            = " << Word0        << endl;
    cout << "-1            = " << Word1        << endl;

    cout.fill('0');
    cout <<std::hex;
    cout << "--w0          = 0x" <<setw(8) << w0.Val        <<endl;
    cout << "--w1          = 0x" <<setw(8) << w1.Val        <<endl;
    cout <<std::dec;
    cout.fill(' ');

    cout << "--mode        = " << mode         << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "mode_e        = " << rgFselPin::rgFsel_enum2cstr( mode_e ) <<endl;
    cout << "ModifyBits    = " << ModifyBits   << endl;
}


/*
* Show usage.
*/
void
fsel_yOptLong::print_usage()
{
    cout <<
    "    Function Select for GPIO pins\n"
    "usage:  " << ProgName << " fsel [options..]  [N..]\n"
    "    N                   bit number 0..53\n"
    "  show bit number groups:  (accumulate)\n"
    "    -0                  word 0, bits [31:0] (default)\n"
    "    -1                  word 1, bits [53:32]\n"
    "  modify:\n"
    "    --mode=In           set mode {In, Out, Alt0, .. Alt5}\n"
    "    --w0=0xffffffff     word 0 mask, bits [31:0]\n"
    "    --w1=0x003fffff     word 1 mask, bits [53:32]\n"
    "  options:\n"
    "    --show              show all alternate functions\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
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
fsel_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output fsel word mask and mode.
*    Heading coordinated with binary bit mask.
*/
void
cout_fsel_mask_head()	// bit number heading
{
    cout << "Bit Mask:         28   24   20   16   12    8    4    0   Mode"
	<< endl;
}

void
cout_fsel_mask(
    const char*			name,	// 2-character  w0, w1
    uint32_t			value,
    rgFselPin::rgFsel_enum	mode
)
{
    cout.fill('0');
    cout << "0x" <<hex <<setw(8) << value
	 << "  "   << name
	 << "  "   << cstr_bits32( value )
	 << "   "  << rgFselPin::rgFsel_enum2cstr( mode )
	 <<dec <<endl;
    cout.fill(' ');
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_fsel::y_fsel(
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
y_fsel::doit()
{

    {
	fsel_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgFselPin		Fpx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout << "+ FeatureAddr  = 0x"
		 <<setw(8) << Fpx.get_bcm_address() <<endl;
	    cout << "+ GpioBase     = "
		 <<setw(8) << (void*) Fpx.get_base_addr() <<endl;
	    cout <<dec;
	}

	const int		BitLimit = 64;
	int			bitarg[BitLimit];	// bit numbers
	int			bitcnt = 0;
	char			*arg;

    // Register groups

	if ( Opx.Word0 ) {
	    for ( int k=0;  k<=31;  k++ )
	    {
		bitarg[bitcnt++] = k;
	    }
	}

	if ( Opx.Word1 ) {
	    for ( int k=32;  k<=53;  k++ )
	    {
		bitarg[bitcnt++] = k;
	    }
	}

    // Argument bit list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;

	    n = strtol( arg, NULL, 0 );
	    if ( (n < 0) || (n > 53) ) {
		Error::msg( " bit arg out-of-range:  " ) << n <<endl;
		continue;
	    }

	    bitarg[bitcnt] = n;
	    bitcnt++;

	    if ( bitcnt > BitLimit ) {
		Error::msg( "max bit args:  " ) << BitLimit <<endl;
		break;
	    }
	}

	if ( Error::has_err() )  return 1;

    // Show all alternate functions
	if ( Opx.show ) {

	    const rgFselPin::rgFsel_enum	mode_tab[] = {
		rgFselPin::f_Alt0,
		rgFselPin::f_Alt1,
		rgFselPin::f_Alt2,
		rgFselPin::f_Alt3,
		rgFselPin::f_Alt4,
		rgFselPin::f_Alt5,
	    };

	    cout <<dec << "Bit ";
	    for ( int jj=0;  jj<=5;  jj++ )		// heading
	    {
		cout << " " << setw(11) <<left
		     << Fpx.rgFsel_enum2cstr( mode_tab[jj] );
	    }
	    cout <<endl;

	    for ( int ii=0;  ii<bitcnt;  ii++ )		// each bit
	    {
		int			bit;
		rgFselPin::rgFsel_enum	mode;

		bit = bitarg[ii];

		cout << " "  <<setw(2) <<right << bit;
		cout << " ";
		for ( int jj=0;  jj<=5;  jj++ )		// each alt mode
		{
		    mode = mode_tab[jj];
		    cout << " " << setw(11) <<left
			 << rgAltFuncName::cstr_altfunc_bit( mode, bit );
		}
		cout <<endl;
	    }

	    return 0;
	}

    // Modify Words
	if ( Opx.w0.Given || Opx.w1.Given ) {
	    Opx.trace_msg( "Modify words" );
	    cout_fsel_mask_head();
	}

	if ( Opx.w0.Given ) {
	    Fpx.modify_Fsel_w0( Opx.w0.Val, Opx.mode_e );
	    cout_fsel_mask( "w0", Fpx.read_Fsel_w0( Opx.mode_e ), Opx.mode_e );
	}

	if ( Opx.w1.Given ) {
	    Fpx.modify_Fsel_w1( Opx.w1.Val, Opx.mode_e );
	    cout_fsel_mask( "w1", Fpx.read_Fsel_w1( Opx.mode_e ), Opx.mode_e );
	}

    // Modify Bits
	if ( Opx.ModifyBits ) {
	    Opx.trace_msg( "Modify bits" );
	    for ( int ii=0;  ii<bitcnt;  ii++ )
	    {
		Fpx.modify_Fsel_bit( bitarg[ii], Opx.mode_e );
	    }
	}

    // Output Bits
	if ( bitcnt > 0 ) {
	    cout << "Bit  Mode  Function" <<endl;
	}

	for ( int ii=0;  ii<bitcnt;  ii++ )
	{
	    int				bit;
	    rgFselPin::rgFsel_enum	mode;

	    bit = bitarg[ii];
	    mode = Fpx.read_Fsel_bit( bit );

	    cout.fill(' ');
	    cout <<dec;
	    cout << " "  <<setw(2) <<right << bit
		 << "  " <<setw(4) <<left  << Fpx.rgFsel_enum2cstr( mode )
		 << "  " << rgAltFuncName::cstr_altfunc_bit( mode, bit )
		 <<endl;
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

