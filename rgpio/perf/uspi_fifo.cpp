// 2018-08-21  William A. Hudson
//
// Universal SPI observe fifo performance with process sleeps.
//     No hardware connection needed.
// Provide external configuration:
//   rgpio fsel --mode=Alt4  16 17 18 19 20 21		# not required
//   rgpio uspi -1 --SpiEnable_1=1
//   rgpio uspi -1 --Speed_12=200 --EnableSerial_1=1 --ShiftLength_6=16
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "rgAddrMap.h"
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

    static const int	TxSize = 4;		// transmit fifo depth
    uint32_t		txval[TxSize];		// transmit fifo values
    int			txval_n;		// num elements used

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

    txval_n = get_argc();
    if ( txval_n > TxSize ) {
	Error::msg( "limit 4 Tx_value args:  " ) << txval_n <<endl;
	txval_n = TxSize;
	return;
    }

    for ( int i=0;  i<txval_n;  i++ )
    {
	txval[i] = strtoul( next_arg(), NULL, 0 );
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

    cout.fill('0');
    for ( int i=0;  i<txval_n;  i++ )
    {
	cout << "txval:        = 0x" <<hex <<setw(8) << txval[i]  << endl;
    }
    cout.fill(' ');
}


/*
* Show usage.
*/
void
yOptLong::print_usage()
{
    cout <<
    "    Observe Spi1 fifo performance with process sleeps\n"
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

    const int		DatSize = 8 * 1024 * 1024;
    uint32_t		*memDat;
    memDat = new( uint32_t [DatSize+1] );

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

	if ( Error::has_err() )  return 1;

	rgAddrMap		Amx;			// constructor

	Amx.open_dev_mem();
	if ( Amx.is_fake_mem() ) {
	    cout << "Using Fake memory" <<endl;
	}

	rgUniSpi		Uspix  ( &Amx, 1 );	// constructor

	// Main Loop

	n_samp = Opx.nsamp_n;
	if ( n_samp > DatSize ) {
	    n_samp = DatSize;
	    Error::msg( "--nsamp exceed DatSize\n" );
	}
	cerr << "    n_samp= " << n_samp << endl;

	int			sample_cnt;	// total read samples
	int			fifo_cnt;	// number of Tx fifo writes
	int			overflow_cnt;	// number of Tx fifo empty

	for ( int jj=1;  jj<=Opx.repeat_n;  jj++ )	// repeat loop
	{
	    sample_cnt   = 0;
	    fifo_cnt     = 0;
	    overflow_cnt = 0;

	    rv = clock_gettime( CLKID, &tpA );

	    // initial fill of the 4-entry fifo
	    for ( int i=0;  i < 4;  i++ )
	    {
		Uspix.Fifo.write( 0x0f0f );
	    }

	    // Inner loop
	    //    This loop should never empty the Tx fifo, but a process
	    //    sleep might.
	    while ( sample_cnt < n_samp )
	    {
		Uspix.Stat.grab();

		if ( ! Uspix.Stat.get_TxFull_1() ) {
		    Uspix.Fifo.write( 0xfff );
		    fifo_cnt++;
		}

		if ( Uspix.Stat.get_TxEmpty_1() ) {
		    overflow_cnt++;
		}

		memDat[ sample_cnt ] = Uspix.Stat.get();

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
	    int			ns_fifo      = -1;
	    if ( sample_cnt ) {
		ns_sample    =         ( delta_ns / sample_cnt );
	    }
	    if ( fifo_cnt   ) {
		ns_fifo      =         ( delta_ns /   fifo_cnt );
	    }

	    cerr << "Rep[" << jj << "]" <<endl;
	    cerr << "    sample_cnt=   " << sample_cnt <<endl;
	    cerr << "    fifo_cnt=     "
		 <<left  <<setw(8)  << fifo_cnt  << "  "
		         <<setw(4)  << ns_fifo   << " ns/fifo_write"
		 <<endl;

	    cerr << "    overflow_cnt= " << overflow_cnt <<endl;

	    cerr << "    delta_ns "
		 <<setw(10) << delta_ns  << " ns,  "
		 <<setw(4)  << ns_sample << " ns/sample"
		 <<endl;

//	    cout << "    A.tv_sec  = " << tpA.tv_sec  << endl;
//	    cout << "    A.tv_nsec = " << tpA.tv_nsec << endl;
//	    cout << "    B.tv_sec  = " << tpB.tv_sec  << endl;
//	    cout << "    B.tv_nsec = " << tpB.tv_nsec << endl;
	}

	// output saved status
	if ( Opx.raw ) {
	    uint32_t		vv;

	    cout << " index  Stat        Tx Rx Bit" <<endl;

	    for ( int ii=0;  ii<n_samp;  ii++ )
	    {
		vv   = memDat[ii];
		Uspix.Stat.put( vv );

		cout.fill(' ');
		cout <<dec <<setw(6) << ii;
		cout.fill('0');
		cout << "  0x"  <<hex <<setw(8) << memDat[ii];
		cout << "  "    <<dec <<setw(1) << Uspix.Stat.get_TxLevel_3();
		cout << "  "          <<setw(1) << Uspix.Stat.get_RxLevel_3();
		cout << "  "          <<setw(2) << Uspix.Stat.get_BitCount_6();
		cout <<endl;
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

