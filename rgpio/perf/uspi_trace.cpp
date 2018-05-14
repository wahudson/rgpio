// 2018-05-12  William A. Hudson
//
// Trace Universal SPI Master operation.
//     Use GPIO read in a tight loop to observe Uspi1 signal trace.
// Provide external configuration:
//   rgpio fsel --mode=Alt4  16 17 18 19 20 21
//   rgpio uspi -1 --Spi_Enable_1=1
//   rgpio uspi -1 --Speed_12=200 --EnableSerial_1=1 --ShiftLength_6=8 ...
//   rgpio clock --enable=1 --source=5 --mash=0 --divi=1000  0
//   rgpio fsel --mode=Alt0  4
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
    "    Trace Universal SPI Master signals.\n"
    "usage:  " << ProgName << " [options]  [Tx_value ..]\n"
    "    Tx_value            SPI transmit fifo value, 32-bits\n"
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

	if ( Error::has_err() )  return 1;

	rgAddrMap		Amx;			// constructor

	Amx.open_dev_mem();
	if ( Amx.is_fake_mem() ) {
	    cout << "Using Fake memory" <<endl;
	}

	rgIoPin			Gpx  ( &Amx );		// constructor
//	rgClock			Ckx  ( 0, &Amx );	// constructor
	rgUniSpi		Uspix  ( 1, &Amx );	// constructor

	volatile uint32_t*	pinread = Gpx.addr_PinRead_w0();


    // Main Loop
	n_samp = Opx.nsamp_n;
	if ( n_samp > DatSize ) {
	    n_samp = DatSize;
	    Error::msg( "--nsamp exceed DatSize\n" );
	}
	cerr << "    n_samp= " << n_samp << endl;

	int			sample_cnt;	// total read samples

	for ( int jj=1;  jj<=Opx.repeat_n;  jj++ )	// repeat loop
	{
	    sample_cnt = 0;

	    rv = clock_gettime( CLKID, &tpA );

	    for ( int i=0;  i < Opx.txval_n;  i++ )
	    {
		Uspix.write_Fifo( Opx.txval[i] );
	    }

	// Inner loop
	    while ( sample_cnt < n_samp )
	    {
		memDat[ sample_cnt ] = *pinread;

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
	    uint32_t		vv;
	    uint32_t		ce2;
	    uint32_t		ce1;
	    uint32_t		ce0;
	    uint32_t		miso;
	    uint32_t		mosi;
	    uint32_t		sclk;
	    uint32_t		clk0;

	    cout << " index  read        CE   SCLK MOSI MISO Clk0" <<endl;

	    for ( int ii=0;  ii<n_samp;  ii++ )
	    {
		vv   = memDat[ii];
		ce2  = (vv >> 16) & 0x1;
		ce1  = (vv >> 17) & 0x1;
		ce0  = (vv >> 18) & 0x1;
		miso = (vv >> 19) & 0x1;
		mosi = (vv >> 20) & 0x1;
		sclk = (vv >> 21) & 0x1;
		clk0 = (vv >>  4) & 0x1;

		cout.fill(' ');
		cout <<dec <<setw(6) << ii;
		cout.fill('0');
		cout << "  0x"  <<hex <<setw(8) << memDat[ii];
		cout << "  "          <<setw(1) << ce2 << ce1 << ce0;
		cout << "  "          <<setw(1) << sclk;
		cout << "    "        <<setw(1) << mosi;
		cout << "    "        <<setw(1) << miso;
		cout << "    "        <<setw(1) << clk0;
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

