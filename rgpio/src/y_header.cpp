// 2023-02-05  William A. Hudson
//
// rgpio header -- Header pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgFselPin.h"
#include "rgsIoCon.h"

#include "rgHeaderPin.h"
#include "rgAltFuncName.h"
#include "rgsFuncName.h"

#include "yUtil.h"
#include "y_header.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class header_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		gpio  = 0;
    bool		signal = 0;
    bool		power = 0;
    bool		row   = 0;

    const char*		mode;
    yOpVal		func;

    bool		show  = 0;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    rgFselPin::rgFsel_enum	mode_e;

    bool		ModifyBits = 0;

  public:
    header_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    header_yOptLong	Opx  ( opx );
*/
header_yOptLong::header_yOptLong( yOption  *opx )
    : yOption( opx )
{
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
header_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--show"       )) { show       = 1; }

	else if ( is( "--gpio"       )) { gpio       = 1; }
	else if ( is( "--signal"     )) { signal     = 1; }
	else if ( is( "--power"      )) { power      = 1; }
	else if ( is( "--row"        )) { row        = 1; }

	else if ( is( "--mode="      )) { mode       = this->val(); }
	else if ( is( "--func="      )) { func.set(    this->val() ); }

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

    if ( rgRpiRev::find_SocEnum() == rgRpiRev::soc_BCM2712 ) {	// RPi5
	if ( *mode ) {
	    Error::msg( "--mode not valid on RPi5" ) <<endl;
	}
    }
    else {
	if ( func.Given ) {
	    Error::msg( "--func is valid only on RPi5" ) <<endl;
	}
    }

    if ( get_argc() > 0 ) {
	if ( signal || power || row ) {
	    Error::msg( "argv not valid with --signal --power --row" ) <<endl;
	}
    }

    if ( gpio ) {
	if ( signal || row ) {
	    Error::msg( "--gpio not valid with --signal --row" ) <<endl;
	}
	// allow --power
    }

    if ( row ) {
	if ( signal || power ) {
	    Error::msg( "--row not valid with --signal --power" ) <<endl;
	}
    }

    if ( *mode ) {
	ModifyBits = 1;
	try {
	    mode_e = rgFselPin::find_rgFsel_enum( mode );
	}
	catch ( range_error& e ) {
	    Error::msg( "unknown pin function select:  --mode=" ) << mode <<endl
		<< "    " <<  e.what() << endl;
	}

	if ( power ) {
	    Error::msg( "--mode not valid with --power" ) <<endl;
	}

	if ( show ) {
	    Error::msg( "--mode not valid with --show" ) <<endl;
	}
    }

    if ( func.Given ) {
	ModifyBits = 1;
	if ( (func.Val < 0) || (func.Val > 31) ) {
	    Error::msg( "require --func={0..31}:  " ) << func.Val <<endl;
	}

	if ( power ) {
	    Error::msg( "--func not valid with --power" ) <<endl;
	}

	if ( show ) {
	    Error::msg( "--func not valid with --show" ) <<endl;
	}
    }
}


/*
* Show option flags.
*/
void
header_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--gpio        = " << gpio         << endl;
    cout << "--signal      = " << signal       << endl;
    cout << "--power       = " << power        << endl;
    cout << "--row         = " << row          << endl;

    cout << "--mode        = " << mode         << endl;
    cout << "--func        = " << func.Val     << endl;
    cout << "--show        = " << show         << endl;

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
header_yOptLong::print_usage()
{
    cout <<
    "    Header Pin Functions, by pin number on 40-pin header\n"
    "usage:  " << ProgName << " header [options..]  [N..]\n"
    "  header pin list formation:  (choose one)\n"
    "    [N..]               header pin numbers 1..40, default all\n"
    "    --gpio  [N..]       Gpio bit numbers   0..27, default all\n"
    "    --signal            signal header pins\n"
    "    --power             power  header pins\n"
    "    --row               header pins by row, odd first then even\n"
    "  modify:  (RPi4 or earlier)\n"
    "    --mode=In           set function mode {In, Out, Alt0, .. Alt5}\n"
    "  modify:  (RPi5)\n"
    "    --func=N            set FuncSel_5 field value {0..31}\n"
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
header_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Function Select Handler Class
//--------------------------------------------------------------------------
// A class to provide equivalent text across RPi5 thru RPi4 and earlier.

class yFsel {

  public:
    rgFselPin		*Fselp;
    rgsIoCon		*Fconp;

  public:
    yFsel( rgAddrMap *amx );		// constructor
    ~yFsel();

    string		show_alts_heading();
    string		show_alts_gpio( int gpio );

    static const rgFselPin::rgFsel_enum		ModeTable[];
};

/*
* Lookup table for rgFsel_enum by Alt integer {0..5}.  RPi4 and earlier.
*/
const rgFselPin::rgFsel_enum	yFsel::ModeTable[] = {
    rgFselPin::f_Alt0,
    rgFselPin::f_Alt1,
    rgFselPin::f_Alt2,
    rgFselPin::f_Alt3,
    rgFselPin::f_Alt4,
    rgFselPin::f_Alt5,
};

/*
* Constructor
* Note:  Operator 'new' throws exception 'bad_alloc' on failure.
*/
yFsel::yFsel( rgAddrMap *amx )
{
    rgRpiRev::initialize_ioRPi();

    if (      rgRpiRev::find_SocEnum() <= rgRpiRev::soc_BCM2711 ) {	// RPi4
	Fconp = NULL;
	Fselp = new rgFselPin ( amx );
    }
    else if ( rgRpiRev::find_SocEnum() == rgRpiRev::soc_BCM2712 ) {	// RPi5
	Fselp = NULL;
	Fconp = new rgsIoCon ( amx );
    }
    else {
	throw std::domain_error ( "require RPi5 (or earlier)" );
    }
}

/*
* Destructor
*/
yFsel::~yFsel()
{
    delete  Fselp;
//  delete  Fconp;	//#!! non-virtual destructor
    // warning: deleting object of polymorphic class type 'rgsIoCon' which
    // has non-virtual destructor might cause undefined behavior
    // [-Wdelete-non-virtual-dtor]
}

/*
* Heading for --show
*/
string
yFsel::show_alts_heading()
{
    std::ostringstream		css;

    if ( rgRpiRev::ioRPi5() ) {
	//     " a5= sRIO[], a6= pRIO[], a7= PIO[]"
	css << " a0         a1         a2         a3         a4         a8";
    }
    else {
	css << " Alt0       Alt1       Alt2       Alt3       Alt4       Alt5";
    }

    return  css.str();
}

/*
* All alternate functions for one gpio.
*/
string
yFsel::show_alts_gpio( int gpio )
{
    std::ostringstream		css;

    if ( rgRpiRev::ioRPi5() ) {

	for ( int jj=0;  jj<=8;  jj++ )         // each altnum
	{
	    if ( (5 <= jj) && (jj <= 7) ) { continue; } // skip 5,6,7
	    css << " " << setw(10) <<left
		<< (( gpio < 0 ) ? "--" :
				rgsFuncName::cstr_altfuncAN( jj, gpio ));
	}
    }
    else {
	rgFselPin::rgFsel_enum	mode;

	for ( int jj=0;  jj<=5;  jj++ )         // each alt mode
	{
	    mode = ModeTable[jj];
	    css << " " << setw(10) <<left
		<< (( gpio < 0 ) ? "--" :
				rgAltFuncName::cstr_altfunc_bit( mode, gpio ));
	}
    }
    // remove trailing <space> chars?

    return  css.str();
}

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_header::y_header(
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
y_header::doit()
{

    {
	header_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	yFsel			Gpx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    if ( rgRpiRev::ioRPi5() ) {
		cout << "+ FeatureAddr  = 0x" <<setw(8)
		     << Gpx.Fconp->get_bcm_address() <<endl;
	    }
	    else {
		cout << "+ FeatureAddr  = 0x" <<setw(8)
		     << Gpx.Fselp->get_bcm_address() <<endl;
	    }
	    cout <<dec;
	}

	const int		PinLimit = 64;
	int			pinlist[PinLimit + 1];	// pin numbers
	int			pincnt = 0;
	char			*arg;

    // Pin groups
	if ( Opx.get_argc() == 0 ) {

	    if ( Opx.gpio ) {
		for ( int k=0;  k <= rgHeaderPin::MaxGpio;  k++ )
		{
		    pinlist[pincnt++] = rgHeaderPin::gpio2pin_int( k );
		}
	    }

	    if ( Opx.signal ) {
		for ( int k=1;  k<=40;  k++ )
		{
		    if ( rgHeaderPin::pin2gpio_int( k ) >= 0 ) {
			pinlist[pincnt++] = k;
		    }
		}
	    }

	    if ( Opx.power ) {
		for ( int k=1;  k<=40;  k++ )
		{
		    if ( rgHeaderPin::pin2gpio_int( k ) < 0 ) {
			pinlist[pincnt++] = k;
		    }
		}
	    }

	    if ( Opx.row && (pincnt == 0) ) {
		for ( int k=1;  k<=40;  k+=2 )	// odd
		{
		    pinlist[pincnt++] = k;
		}

		for ( int k=2;  k<=40;  k+=2 )	// even
		{
		    pinlist[pincnt++] = k;
		}
	    }

	    if ( pincnt == 0 ) {	// default, none of above
		for ( int k=1;  k<=40;  k++ )	// all pins
		{
		    pinlist[pincnt++] = k;
		}
	    }
	    // PinLimit implied by loop count logic
	}

    // Argument pin list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;
	    char		*endptr;

	    n = strtol( arg, &endptr, 10 );

	    if ( Opx.gpio ) {
		if ( *endptr != '\0' ) {
		    Error::msg( "bit arg non-numeric:  " ) << arg <<endl;
		    break;
		}

		if ( (n < 0) || (n > 27) ) {
		    Error::msg( "bit arg out-of-range {0..27}:  " ) << n <<endl;
		    continue;
		}
		pinlist[pincnt++] = rgHeaderPin::gpio2pin_int( n );
	    }
	    else {
		if ( *endptr != '\0' ) {
		    Error::msg( "pin arg non-numeric:  " ) << arg <<endl;
		    break;
		}

		if ( (n < 1) || (n > 40) ) {
		    Error::msg( "pin arg out-of-range {1..40}:  " ) << n <<endl;
		    continue;
		}
		pinlist[pincnt++] = n;
	    }

	    if ( pincnt > PinLimit ) {
		Error::msg( "max pin args:  " ) << PinLimit <<endl;
		break;
	    }
	}

	if ( Error::has_err() )  return 1;

    // Show all alternate functions
	if ( Opx.show ) {
							// headings
	    if ( rgRpiRev::ioRPi5() ) {
		cout << "              a5= sRIO[], a6= pRIO[], a7= PIO[]" <<endl;
	    }
	    cout <<dec << " Pin  Gpio   ";
	    cout << Gpx.show_alts_heading() <<endl;

	    for ( int ii=0;  ii<pincnt;  ii++ )		// each pin
	    {
		int			pin;
		int			bit;
		const char*		gpio_str;

		pin = pinlist[ii];
		bit = rgHeaderPin::pin2gpio_int( pin );

		gpio_str = rgHeaderPin::pin2name_cstr( pin );

		int pw = (pin & 0x1) ? 2 : 4;	// indent even pin numbers

		cout << " "  <<setw(pw) <<right << pin <<setw(4 - pw) << "";
		cout << " "  <<setw(7)  <<left  << gpio_str;
		cout << Gpx.show_alts_gpio( bit );
		cout <<endl;
	    }

	    return 0;
	}

    // Modify Pins
	if ( Opx.ModifyBits ) {
	    int				bit;

	    Opx.trace_msg( "Modify pins" );
	    for ( int ii=0;  ii<pincnt;  ii++ )
	    {
		bit = rgHeaderPin::pin2gpio_int( pinlist[ii] );
		if ( bit >= 0 ) {	// is a Gpio
		    if ( rgRpiRev::ioRPi5() ) {
			rgsIo_Cntl&	iocon = Gpx.Fconp->IoCntl(bit);
			iocon.grab();
			iocon.put_FuncSel_5( Opx.func.Val );
			iocon.push();
		    }
		    else {
			Gpx.Fselp->modify_Fsel_bit( bit, Opx.mode_e );
		    }
		}
	    }
	}

    // Output Pins
	if ( pincnt > 0 ) {
	    cout << " Pin   Gpio    Mode  Function" <<endl;
	}

	for ( int ii=0;  ii<pincnt;  ii++ )
	{
	    int				pin;
	    int				bit;
	    rgFselPin::rgFsel_enum	mode;
	    uint32_t			func;
	    const char*			gpio_str;
	    const char*			mode_str;
	    const char*			func_str;

	    pin  = pinlist[ii];
	    bit  = rgHeaderPin::pin2gpio_int( pin );

	    if ( bit >= 0 ) {
		if ( rgRpiRev::ioRPi5() ) {
		    char		funcstr[5];
		    rgsIo_Cntl&		iocon = Gpx.Fconp->IoCntl(bit);
		           iocon.grab();
		    func = iocon.get_FuncSel_5();
		    sprintf( funcstr, "a%d", func );
		    gpio_str = rgHeaderPin::pin2name_cstr( pin );
		    mode_str = funcstr;
		    func_str = rgsFuncName::cstr_altfuncAN( func, bit );
		}
		else {
		    mode = Gpx.Fselp->read_Fsel_bit( bit );
		    gpio_str = rgHeaderPin::pin2name_cstr( pin );
		    mode_str = Gpx.Fselp->rgFsel_enum2cstr( mode );
		    func_str = rgAltFuncName::cstr_altfunc_bit( mode, bit );
		}
	    } else {
		gpio_str = "--";
		mode_str = "--";
		func_str = rgHeaderPin::pin2name_cstr( pin );
	    }

	    int pw = (pin & 0x1) ? 2 : 4;	// indent even pin numbers

	    cout.fill(' ');
	    cout <<dec;
	    cout << " "  <<setw(pw) <<right << pin <<setw(4 - pw) << ""
		 << "  " <<setw(6)  <<left  << gpio_str
		 << "  " <<setw(4)  <<left  << mode_str
		 << "  "                    << func_str
		 <<endl;
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

