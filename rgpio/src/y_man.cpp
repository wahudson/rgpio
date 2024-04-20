// 2020-08-22  William A. Hudson
//
// rgpio man -- man pager - RaspberryPi GPIO utility program.
//    Show man pages in the tool rgpio/man/ directory tree.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept
#include <string.h>	// strcmp()
#include <unistd.h>	// readlink()

using namespace std;

#include "Error.h"
#include "yOption.h"

#include "rgAddrMap.h"

#include "y_man.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class man_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		list;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    man_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    man_yOptLong	Opx  ( opx );
*/
man_yOptLong::man_yOptLong( yOption  *opx )
    : yOption( opx )
{
    list        = 0;
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
man_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--list"       )) { list       = 1; }
	else if ( is( "-l"           )) { list       = 1; }
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

    // allow any arguments be applied to man command
}


/*
* Show option flags.
*/
void
man_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<dec;

    cout << "--list        = " << list         << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }
}


/*
* Show usage.
*    Register fields ordered similar to BCM doc.
*/
void
man_yOptLong::print_usage()
{
    cout <<
    "    man pager for librgpio manuals\n"
    "usage:  " << ProgName << " man [options] [section] page\n"
    "  page:         manpage name, e.g. rgpio-clk, rgClk\n"
    "  section:\n"
    "    1                   rgpio commands,         e.g.  rgpio-clk\n"
    "    3                   librgpio classes,       e.g.  rgClk\n"
    "    7                   peripheral description, e.g.  rgClk\n"
    "  options:\n"
    "    -l, --list          list available man pages\n"
    "    --help              show this usage\n"
//  "    -v, --verbose       verbose output\n"
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
man_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------
// Note:  AddrMap is unused, but is present for uniformity with other
// sub-commands.

y_man::y_man(
    yOption		*opx,
    rgAddrMap		*rgx
) {
    AddrMap = rgx;
    MainOpx = opx;
}
//#!! Note:  Passing in main program MainOpx as the base class yOption
// does not give access to all main program arguments.  Not quite what we want.


//--------------------------------------------------------------------------
// Main body
//--------------------------------------------------------------------------

int
y_man::doit()
{

    {
	man_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // Find this executing program file, e.g. /home/pi/work/bin/rgpio
	const int		bufsize = 254;
	char			buff[ bufsize + 2 ];
	int			nn;

	nn = readlink( "/proc/self/exe", buff, bufsize );

	if ( nn <= -1 ) {
	    Error::msg( "readlink( /proc/self/exe ) failed:  " )
		<< strerror( errno ) <<endl;
	    throw std::runtime_error ( "bail" );
	}

	buff[bufsize] = '\0';
	if ( nn < bufsize ) {
	    buff[nn] = '\0';
	}
	else {
	    Error::msg( "symlink too long:  /proc/self/exe -> " )
		<< buff << "..." <<endl;
	    throw std::runtime_error ( "bail" );
	}

    // Trim off trailing file and bin/ directory, modify buff
	char			*end;

	end = strrchr( buff, '/' );	// trim file rgpio
	if ( end ) {
	    *end = '\0';
	}

	end = strrchr( buff, '/' );	// trim bin
	if ( end ) {
	    *end = '\0';
	}
	else {
	    Error::msg( "tooldir has no /:  " ) << buff <<endl;
	    throw std::runtime_error ( "bail" );
	}

	if ( Opx.debug ) {
	    cout << "tooldir= " << buff <<endl;
	}

    // Form man command, see also:  man(1), manpath(1)
	string			cmd;

	if ( Opx.list ) {
	    cmd = "ls -1 -L ";
	    cmd += buff;
	    cmd += "/man/man?/ ";
	}
	else {
	    cmd = "man --manpath=";
	    cmd += buff;
	    cmd += "/man ";

	    if ( Opx.get_argc() == 0 ) {	// default manpage
		cmd += "rgpio";
	    }
	    else {				// additional arguments
		char*		arg;
		while ( ( arg = Opx.next_arg() ) )
		{
		    cmd += " ";
		    cmd += arg;
		}
	    }
	}

	cerr << "+ " << cmd <<endl;

	system( cmd.c_str() );

    }

    return ( Error::has_err() ? 1 : 0 );
}

