// 2018-01-19  William A. Hudson
//
// Measure Clock system read/write performance.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "rgAddrMap.h"
#include "rgIoPin.h"
#include "rgClock.h"
#include "rgClk.h"
#include "rgUniSpi.h"

#include "Error.h"
#include "yOption.h"
//#include "yMan.h"

#define CLKID	CLOCK_MONOTONIC_RAW


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		raw;
    const char*		nsamp;
    const char*		repeat;

    bool		verbose;
    bool		debug;
    bool		man;
    bool		TESTOP;

  public:	// data values

    int			nsamp_n;		// number of samples
    int			repeat_n;		// repeat loop

  public:
    yOptLong( int argc,  char* argv[] );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

};


/*
* Constructor.  Init options with default values.
*    Pass in the main() argc and argv parameters.
* call:
*    yOptLong	Opx  ( argc, argv );
*    yOptLong	Opx = yOptLong::yOptLong( argc, argv );
*/
yOptLong::yOptLong( int argc,  char* argv[] )
    : yOption( argc, argv )
{
    raw         = 0;
    nsamp       = "";
    repeat      = "";

    verbose     = 0;
    debug       = 0;
    man         = 0;
    TESTOP      = 0;

    nsamp_n     = 100;
    repeat_n    = 1;
}


/*
* Parse options.
*/
void
yOptLong::parse_options()
{
    while ( this->next() )
    {
	if      ( is( "--raw"        )) { raw        = 1; }
	else if ( is( "--nsamp="     )) { nsamp      = this->val(); }
	else if ( is( "--repeat="    )) { repeat     = this->val(); }

	else if ( is( "--verbose"    )) { verbose    = 1; }
	else if ( is( "-v"           )) { verbose    = 1; }
	else if ( is( "--debug"      )) { debug      = 1; }
	else if ( is( "--man"        )) { man        = 1; }
	else if ( is( "--TESTOP"     )) { TESTOP     = 1; }
	else if ( is( "--help"       )) { this->print_usage();  exit( 0 ); }
	else if ( is( "-"            )) {                break; }
	else if ( is( "--"           )) { this->next();  break; }
	else {
	    Error::msg( "unknown option:  " ) << this->current_option() <<endl;
	}
    }

    string	nsamp_s   ( nsamp );
    string	repeat_s  ( repeat );

    if ( nsamp_s.length() ) {
	nsamp_n = stoi( nsamp_s );
    }

    if ( repeat_s.length() ) {
	repeat_n = stoi( repeat_s );
    }

    if ( get_argc() > 0 ) {
	Error::msg( "extra arguments:  " ) << next_arg() <<endl;
    }
}


/*
* Show option flags.
*/
void
yOptLong::print_option_flags()
{
    cout << "--raw         = " << raw          << endl;
    cout << "--nsamp       = " << nsamp        << endl;
    cout << "--repeat      = " << repeat       << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "nsamp_n       = " << nsamp_n      << endl;
    cout << "repeat_n      = " << repeat_n     << endl;
}


/*
* Show usage.
*/
void
yOptLong::print_usage()
{
    cout <<
    "    Measure Raspberry Pi process performance.\n"
    "usage:  " << ProgName << " [options]\n"
    "  output forms:  (default is none)\n"
    "    --raw               raw data\n"
    "  options:\n"
    "    --nsamp=N           number of samples in inner loop\n"
    "    --repeat=N          repeat data read loop N times\n"
    "    --help              show this usage\n"
//  "  # --man               show manpage and exit\n"
//  "  # -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


//--------------------------------------------------------------------------
// Main program
//--------------------------------------------------------------------------

int
main( int	argc,
      char	*argv[]
) {
    int			rv;
    struct timespec	tpA;
    struct timespec	tpB;
//  struct timespec	tpL;		// time point in loop
//  int			tpold;		// last time point in loop
    int			n_samp;		// inner loop number of samples

    const int		DatSize = 1 * 1024 * 1024;
    int32_t		*memDat;
    memDat = new( int32_t [DatSize+1] );

    try {
	yOptLong		Opx  ( argc, argv );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Opx.man ) {
	    Error::msg( "--man not implemented\n" );
//	    yMan		Manx;
//	    Manx.show_manpage( Opx.ProgName );
	    return ( 0 );
	}

//	unsigned		ilevel;		// GPIO read value
	int			sample_cnt;	// total read samples

	if ( Error::has_err() )  return 1;

	rgAddrMap		Amx;			// constructor

	Amx.open_dev_mem();
	if ( Amx.is_fake_mem() ) {
	    cout << "Using Fake memory" <<endl;
	}

	rgIoPin			Gpx  ( &Amx );		// constructor
	rgClock			Ckx  ( 0, &Amx );	// constructor
	rgClk			Cky  ( rgClk::cm_Clk0, &Amx );	// constructor
	rgUniSpi		Uspix  ( 1, &Amx );	// constructor
	uint32_t		vv;

	volatile uint32_t*	pinread = Gpx.addr_PinRead_w0();
	volatile uint32_t*	pinset  = Gpx.addr_PinSet_w0();

//	volatile uint32_t*	ck0ctl     = Ckx.addr_CtlReg();
	volatile uint32_t*	ck0ctl     = Cky.Cntl.addr();
	volatile uint32_t*	uspi_Cntl0 = Uspix.Cntl0.addr();

    // Configure
	Cky.wait_time_ns( 0 );
	Cky.wait_count_n( 10 );

    // Make vars used
	vv = *pinread;
	*pinset = 0x00000000;
	vv = *ck0ctl;
	vv = *uspi_Cntl0;
	rv = vv;	// used

	if ( Opx.debug ) {
	    cout << "    ck0ctl= " << (uint32_t*)ck0ctl << endl;
	}

    // Main Loop
	n_samp = Opx.nsamp_n;
	if ( n_samp > DatSize ) {
	    n_samp = DatSize;
	    Error::msg( "--nsamp exceed DatSize\n" );
	}
	cerr << "    n_samp= " << n_samp << endl;

	for ( int jj=1;  jj<=Opx.repeat_n;  jj++ )	// repeat loop
	{
	    sample_cnt = 0;

	    rv = clock_gettime( CLKID, &tpA );

	    while ( sample_cnt < n_samp )		// inner loop
	    {
//		rv = clock_gettime( CLKID, &tpL );
//		memDat[ sample_cnt ] = tpL.tv_nsec - tpold;
//		tpold = tpL.tv_nsec;

//		Ckx.read_CtlReg();
//		Ckx.grab_regs();
//		vv = Ckx.read_Busy();
//		vv = *ck0ctl;		// 370 ns/sample  -O3: 313 ns/sample
//		*ck0ctl = 0x00000000;	// 157 ns/sample
//		Cky.Cntl.grab();	// 420 ns/sample  -O3: 370 ns/sample
//		vv = Cky.Cntl.read();	// 420 ns/sample
//		Cky.wait_while_busy();	// 365 ns/BusyCount  -O3: 313 ns/

		Cky.Cntl.grab();
		vv = Cky.Cntl.get_Busy_1();	// -O3: 313 ns/sample

//		*pinset = 0x00000000;
//		vv = *pinread;
//		vv = Gpx.read_PinLevel_w0();

//		vv = *uspi_Cntl0;

		sample_cnt++;
	    }

	    rv = clock_gettime( CLKID, &tpB );

	    if ( rv ) { cerr << "Error:  clock_gettime() failed" << endl; }

	    int64_t	delta_ns =
		((tpB.tv_sec  - tpA.tv_sec) * ((int64_t) 1000000000)) +
		 (tpB.tv_nsec - tpA.tv_nsec);
		// Note 4 seconds overflows a 32-bit int.
		// Use careful promotion to 64-bit integer to avoid overflow.

	    int			ns_sample    = -1;
	    if ( sample_cnt ) {
		ns_sample    =         ( delta_ns / sample_cnt );
	    }

	    cerr << "Rep[" << jj << "]" <<endl;
	    cerr << "    sample_cnt= " << sample_cnt <<endl;

	    cerr << "    delta_ns"
		 <<setw(10) << delta_ns  << " ns,  "
		 <<setw(4)  << ns_sample << " ns/sample"
		 <<endl;

//	    cout << "    A.tv_sec  = " << tpA.tv_sec  << endl;
//	    cout << "    A.tv_nsec = " << tpA.tv_nsec << endl;
//	    cout << "    B.tv_sec  = " << tpB.tv_sec  << endl;
//	    cout << "    B.tv_nsec = " << tpB.tv_nsec << endl;
	}

	if ( Opx.raw ) {
	    for ( int ii=0;  ii<n_samp;  ii++ )
	    {
		cout << setw(6) << ii << setw(10) << memDat[ii] << endl;
	    }
	}

    }
    catch ( std::exception& e ) {
	Error::msg() << e.what() <<endl;
    }
    catch (...) {
	Error::msg( "unexpected exception\n" );
    }

    return ( Error::has_err() ? 1 : 0 );
}

