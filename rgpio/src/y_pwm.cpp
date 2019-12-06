// 2019-11-29  William A. Hudson
//
// rgpio pwm -- Pulse Width Modulator - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept
#include <string.h>	// strcmp()

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgPwm.h"

#include "y_pwm.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class pwm_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values
					// registers
    yOpVal		Cntl;
    yOpVal		Stat;
    yOpVal		DmaConf;
    yOpVal		Ch1Range;
    yOpVal		Ch1Data;
    yOpVal		Ch2Range;
    yOpVal		Ch2Data;

    bool		tx;
					// Cntl fields
    yOpVal		Ch2_MsEnable_1;
    yOpVal		Ch2_UseFifo_1;
    yOpVal		Ch2_Invert_1;
    yOpVal		Ch2_IdleBit_1;
    yOpVal		Ch2_Repeat_1;
    yOpVal		Ch2_SerMode_1;
    yOpVal		Ch2_Enable_1;

    yOpVal		ClearFifo_1;

    yOpVal		Ch1_MsEnable_1;
    yOpVal		Ch1_UseFifo_1;
    yOpVal		Ch1_Invert_1;
    yOpVal		Ch1_IdleBit_1;
    yOpVal		Ch1_Repeat_1;
    yOpVal		Ch1_SerMode_1;
    yOpVal		Ch1_Enable_1;

					// Stat fields
    yOpVal		Ch2_Active_1;
    yOpVal		Ch1_Active_1;
    yOpVal		BusError_1;
    yOpVal		Ch2_GapErr_1;
    yOpVal		Ch1_GapErr_1;
    yOpVal		FifoReadErr_1;
    yOpVal		FifoWriteErr_1;
    yOpVal		FifoEmpty_1;
    yOpVal		FifoFull_1;

					// DmaConf fields
    yOpVal		DmaEnable_1;
    yOpVal		DmaPanicLev_8;
    yOpVal		DmaReqLev_8;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    pwm_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    pwm_yOptLong	Opx  ( opx );
*/
pwm_yOptLong::pwm_yOptLong( yOption  *opx )
    : yOption( opx )
{
    tx          = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
pwm_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--Cntl="      )) { Cntl.set(     this->val() ); }
	else if ( is( "--Stat="      )) { Stat.set(     this->val() ); }
	else if ( is( "--DmaConf="   )) { DmaConf.set(  this->val() ); }
	else if ( is( "--Ch1Range="  )) { Ch1Range.set( this->val() ); }
	else if ( is( "--Ch1Data="   )) { Ch1Data.set(  this->val() ); }
	else if ( is( "--Ch2Range="  )) { Ch2Range.set( this->val() ); }
	else if ( is( "--Ch2Data="   )) { Ch2Data.set(  this->val() ); }

	else if ( is( "--tx"         )) { tx         = 1; }

	else if ( is( "--Ch2_MsEnable_1="  )) { Ch2_MsEnable_1.set(  val() ); }
	else if ( is( "--Ch2_UseFifo_1="   )) { Ch2_UseFifo_1.set(   val() ); }
	else if ( is( "--Ch2_Invert_1="    )) { Ch2_Invert_1.set(    val() ); }
	else if ( is( "--Ch2_IdleBit_1="   )) { Ch2_IdleBit_1.set(   val() ); }
	else if ( is( "--Ch2_Repeat_1="    )) { Ch2_Repeat_1.set(    val() ); }
	else if ( is( "--Ch2_SerMode_1="   )) { Ch2_SerMode_1.set(   val() ); }
	else if ( is( "--Ch2_Enable_1="    )) { Ch2_Enable_1.set(    val() ); }

	else if ( is( "--ClearFifo_1="     )) { ClearFifo_1.set(     val() ); }

	else if ( is( "--Ch1_MsEnable_1="  )) { Ch1_MsEnable_1.set(  val() ); }
	else if ( is( "--Ch1_UseFifo_1="   )) { Ch1_UseFifo_1.set(   val() ); }
	else if ( is( "--Ch1_Invert_1="    )) { Ch1_Invert_1.set(    val() ); }
	else if ( is( "--Ch1_IdleBit_1="   )) { Ch1_IdleBit_1.set(   val() ); }
	else if ( is( "--Ch1_Repeat_1="    )) { Ch1_Repeat_1.set(    val() ); }
	else if ( is( "--Ch1_SerMode_1="   )) { Ch1_SerMode_1.set(   val() ); }
	else if ( is( "--Ch1_Enable_1="    )) { Ch1_Enable_1.set(    val() ); }

	else if ( is( "--Ch2_Active_1="    )) { Ch2_Active_1.set(    val() ); }
	else if ( is( "--Ch1_Active_1="    )) { Ch1_Active_1.set(    val() ); }
	else if ( is( "--BusError_1="      )) { BusError_1.set(      val() ); }
	else if ( is( "--Ch2_GapErr_1="    )) { Ch2_GapErr_1.set(    val() ); }
	else if ( is( "--Ch1_GapErr_1="    )) { Ch1_GapErr_1.set(    val() ); }
	else if ( is( "--FifoReadErr_1="   )) { FifoReadErr_1.set(   val() ); }
	else if ( is( "--FifoWriteErr_1="  )) { FifoWriteErr_1.set(  val() ); }
	else if ( is( "--FifoEmpty_1="     )) { FifoEmpty_1.set(     val() ); }
	else if ( is( "--FifoFull_1="      )) { FifoFull_1.set(      val() ); }

	else if ( is( "--DmaEnable_1="     )) { DmaEnable_1.set(     val() ); }
	else if ( is( "--DmaPanicLev_8="   )) { DmaPanicLev_8.set(   val() ); }
	else if ( is( "--DmaReqLev_8="     )) { DmaReqLev_8.set(     val() ); }

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

    check_f1( "Ch2_MsEnable_1",  Ch2_MsEnable_1.Val  );
    check_f1( "Ch2_UseFifo_1",   Ch2_UseFifo_1.Val   );
    check_f1( "Ch2_Invert_1",    Ch2_Invert_1.Val    );
    check_f1( "Ch2_IdleBit_1",   Ch2_IdleBit_1.Val   );
    check_f1( "Ch2_Repeat_1",    Ch2_Repeat_1.Val    );
    check_f1( "Ch2_SerMode_1",   Ch2_SerMode_1.Val   );
    check_f1( "Ch2_Enable_1",    Ch2_Enable_1.Val    );

    check_f1( "ClearFifo_1",     ClearFifo_1.Val     );

    check_f1( "Ch1_MsEnable_1",  Ch1_MsEnable_1.Val  );
    check_f1( "Ch1_UseFifo_1",   Ch1_UseFifo_1.Val   );
    check_f1( "Ch1_Invert_1",    Ch1_Invert_1.Val    );
    check_f1( "Ch1_IdleBit_1",   Ch1_IdleBit_1.Val   );
    check_f1( "Ch1_Repeat_1",    Ch1_Repeat_1.Val    );
    check_f1( "Ch1_SerMode_1",   Ch1_SerMode_1.Val   );
    check_f1( "Ch1_Enable_1",    Ch1_Enable_1.Val    );

    check_f1( "Ch2_Active_1",    Ch2_Active_1.Val    );
    check_f1( "Ch1_Active_1",    Ch1_Active_1.Val    );
    check_f1( "BusError_1",      BusError_1.Val      );
    check_f1( "Ch2_GapErr_1",    Ch2_GapErr_1.Val    );
    check_f1( "Ch1_GapErr_1",    Ch1_GapErr_1.Val    );
    check_f1( "FifoReadErr_1",   FifoReadErr_1.Val   );
    check_f1( "FifoWriteErr_1",  FifoWriteErr_1.Val  );
    check_f1( "FifoEmpty_1",     FifoEmpty_1.Val     );
    check_f1( "FifoFull_1",      FifoFull_1.Val      );

    check_f1( "DmaEnable_1",     DmaEnable_1.Val     );

    if (                       DmaPanicLev_8.Val > 0xff ) {
	Error::msg( "require --DmaPanicLev_8={0..255}:  " ) <<
			       DmaPanicLev_8.Val <<endl;
    }

    if (                       DmaReqLev_8.Val   > 0xff ) {
	Error::msg( "require --DmaReqLev_8={0..255}:  " ) <<
			       DmaReqLev_8.Val   <<endl;
    }

    if ( (! tx) && (get_argc() > 0) ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Ch2_MsEnable_1", Ch2_MsEnable_1.Val );
*/
void
pwm_yOptLong::check_f1(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 1 ) {
	Error::msg( "require --" ) << p << "={0,1}:  " << v << endl;
    }
}


/*
* Show option flags.
*/
void
pwm_yOptLong::print_option_flags()
{
    cout.fill('0');
    cout <<hex;
    cout << "--Cntl        = 0x" <<setw(8) << Cntl.Val        << endl;
    cout << "--Stat        = 0x" <<setw(8) << Stat.Val        << endl;
    cout << "--DmaConf     = 0x" <<setw(8) << DmaConf.Val     << endl;
    cout << "--Ch1Range    = 0x" <<setw(8) << Ch1Range.Val    << endl;
    cout << "--Ch1Data     = 0x" <<setw(8) << Ch1Data.Val     << endl;
    cout << "--Ch2Range    = 0x" <<setw(8) << Ch2Range.Val    << endl;
    cout << "--Ch2Data     = 0x" <<setw(8) << Ch2Data.Val     << endl;

    cout << "--tx          = " << tx           << endl;

    cout <<dec;
    cout << "--Ch2_MsEnable_1  = " << Ch2_MsEnable_1.Val  << endl;
    cout << "--Ch2_UseFifo_1   = " << Ch2_UseFifo_1.Val   << endl;
    cout << "--Ch2_Invert_1    = " << Ch2_Invert_1.Val    << endl;
    cout << "--Ch2_IdleBit_1   = " << Ch2_IdleBit_1.Val   << endl;
    cout << "--Ch2_Repeat_1    = " << Ch2_Repeat_1.Val    << endl;
    cout << "--Ch2_SerMode_1   = " << Ch2_SerMode_1.Val   << endl;
    cout << "--Ch2_Enable_1    = " << Ch2_Enable_1.Val    << endl;

    //#!! incomplete

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
*    Channels ordered with Ch2 first, to match fields.
*/
void
pwm_yOptLong::print_usage()
{
    cout <<
    "    PWM Pulse Width Modulator control\n"
    "usage:  " << ProgName << " pwm  [options..] [--tx V..]\n"
    "  modify full register:  (V= 32-bit value)\n"
    "    --Cntl=V            Control\n"
    "    --Stat=V            Status\n"
    "    --DmaConf=V         DMA Configuration\n"
    "    --Ch2Range=V        cycle length M\n"
    "    --Ch2Data=V         data pattern or mark length N\n"
    "    --Ch1Range=V        ...\n"
    "    --Ch1Data=V         ...\n"
    "  send pattern:\n"
    "    --tx                write arg values to Fifo\n"
    "  modify Cntl bit fields:\n"
    "    --ClearFifo_1=0     1= clears fifo, single shot (WO)\n"
    "    --Ch2_MsEnable_1=0  1= mark/space mode, 0= pwm mode\n"
    "    --Ch2_UseFifo_1=0   1= fifo, 0= Data register\n"
    "    --Ch2_Invert_1=0    1= invert, 0= normal\n"
    "    --Ch2_IdleBit_1=0   value output for padding or idle\n"
    "    --Ch2_Repeat_1=0    1= repeat last data in fifo, 0= not\n"
    "    --Ch2_SerMode_1=0   1= serializer mode, 0= MsEnable mode\n"
    "    --Ch2_Enable_1=0    1= enable channel, 0= disabled\n"
    "    --Ch1_* ...         same as Ch2_* above\n"
    "  modify Stat bit fields:  (write 1 to clear Err flag)\n"
    "    --Ch2_Active_1=0    1= transmitting, 0=not, (RO)\n"
    "    --Ch1_Active_1=0    ...\n"
    "    --BusError_1=0      error writing to registers, bus overrun\n"
    "    --Ch2_GapErr_1=0    gap in transmission between fifo entries\n"
    "    --Ch1_GapErr_1=0    ...\n"
    "    --FifoReadErr_1=0   fifo read when empty\n"
    "    --FifoWriteErr_1=0  fifo write when full\n"
    "    --FifoEmpty_1=0     fifo is empty (RO)\n"
    "    --FifoFull_1=0      fifo is full  (RO)\n"
    "  modify DmaConf bit fields:\n"
    "    --DmaEnable_1=0     1= DMA start, 0= disabled\n"
    "    --DmaPanicLev_8=N   DMA panic when Fifo level > N\n"
    "    --DmaReqLev_8=N     DMA DREQ  when Fifo level > N\n"
    "  options:\n"
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
pwm_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_pwm::y_pwm(
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
y_pwm::doit()
{

    try {
	pwm_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	if ( Opx.verbose ) {
	    cout << "PWM:" << endl;
	}

    // PWM object

	rgPwm		Pwx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    // Note cout does not know how to show (volatile uint32_t*).

	    cout << "    " << "Pwm.GpioBase=       "
		 <<hex << (uint32_t*) Pwx.get_base_addr() <<endl;

	    cout << "    " << "Pwm.Stat.addr=      "
		 <<hex << (uint32_t*) Pwx.Stat.addr() <<endl;

	    cout << "    " << "Pwm.diff_Stat=      "
		 <<hex
		 << (Pwx.Stat.addr() - Pwx.get_base_addr())*4 <<endl;

	    cout <<dec <<endl;
	}

	{
	    bool		md = 0;		// modify flag

	    Opx.trace_msg( "Grab regs" );
	    Pwx.grab_regs();

#define APPLX( X ) if ( Opx.X.Given ) { Pwx.X.put( Opx.X.Val );  md = 1; }

	// Registers

	    APPLX( Cntl     )
	    APPLX( Stat     )
	    APPLX( DmaConf  )
	    // not Fifo
	    APPLX( Ch1Range )
	    APPLX( Ch1Data  )
	    APPLX( Ch2Range )
	    APPLX( Ch2Data  )

#define APPLY( X, Y ) if ( Opx.X.Given ) { Pwx.Y( Opx.X.Val );  md = 1; }

	// Cntl fields

	    APPLY( Ch2_MsEnable_1,  Cntl.put_Ch2_MsEnable_1  )
	    APPLY( Ch2_UseFifo_1,   Cntl.put_Ch2_UseFifo_1   )
	    APPLY( Ch2_Invert_1,    Cntl.put_Ch2_Invert_1    )
	    APPLY( Ch2_IdleBit_1,   Cntl.put_Ch2_IdleBit_1   )
	    APPLY( Ch2_Repeat_1,    Cntl.put_Ch2_Repeat_1    )
	    APPLY( Ch2_SerMode_1,   Cntl.put_Ch2_SerMode_1   )
	    APPLY( Ch2_Enable_1,    Cntl.put_Ch2_Enable_1    )

	    APPLY( ClearFifo_1,     Cntl.put_ClearFifo_1     )

	    APPLY( Ch1_MsEnable_1,  Cntl.put_Ch1_MsEnable_1  )
	    APPLY( Ch1_UseFifo_1,   Cntl.put_Ch1_UseFifo_1   )
	    APPLY( Ch1_Invert_1,    Cntl.put_Ch1_Invert_1    )
	    APPLY( Ch1_IdleBit_1,   Cntl.put_Ch1_IdleBit_1   )
	    APPLY( Ch1_Repeat_1,    Cntl.put_Ch1_Repeat_1    )
	    APPLY( Ch1_SerMode_1,   Cntl.put_Ch1_SerMode_1   )
	    APPLY( Ch1_Enable_1,    Cntl.put_Ch1_Enable_1    )

	// Stat fields

	    APPLY( Ch2_Active_1,    Stat.put_Ch2_Active_1    )
	    APPLY( Ch1_Active_1,    Stat.put_Ch1_Active_1    )
	    APPLY( BusError_1,      Stat.put_BusError_1      )
	    APPLY( Ch2_GapErr_1,    Stat.put_Ch2_GapErr_1    )
	    APPLY( Ch1_GapErr_1,    Stat.put_Ch1_GapErr_1    )
	    APPLY( FifoReadErr_1,   Stat.put_FifoReadErr_1   )
	    APPLY( FifoWriteErr_1,  Stat.put_FifoWriteErr_1  )
	    APPLY( FifoEmpty_1,     Stat.put_FifoEmpty_1     )
	    APPLY( FifoFull_1,      Stat.put_FifoFull_1      )

	// DmaConf fields

	    APPLY( DmaEnable_1,     DmaConf.put_DmaEnable_1     )
	    APPLY( DmaPanicLev_8,   DmaConf.put_DmaPanicLev_8   )
	    APPLY( DmaReqLev_8,     DmaConf.put_DmaReqLev_8     )

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify regs" );
		Pwx.push_regs();
	    }

	    if ( Opx.tx ) {
		char*		cp;
		uint32_t	vv;

		Opx.trace_msg( "Write Fifo" );

		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << "   write Fifo:  0x" <<hex <<setw(8) << vv << endl;
		    Pwx.Fifo.write( vv );
		}
		cout << dec;
	    }

	    if ( md || Opx.tx  ) {		// registers changed
		Opx.trace_msg( "Grab regs" );
		Pwx.grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
		<< "   Cntl=     0x" <<setw(8) << Pwx.Cntl.read()     <<endl
		<< "   Stat=     0x" <<setw(8) << Pwx.Stat.read()     <<endl
		<< "   DmaConf=  0x" <<setw(8) << Pwx.DmaConf.read()  <<endl
		<< "   Fifo=     0x" <<setw(8) << Pwx.Fifo.read()     <<endl
		<< "   Ch2Range= 0x" <<setw(8) << Pwx.Ch2Range.read() <<endl
		<< "   Ch2Data=  0x" <<setw(8) << Pwx.Ch2Data.read()  <<endl
		<< "   Ch1Range= 0x" <<setw(8) << Pwx.Ch1Range.read() <<endl
		<< "   Ch1Data=  0x" <<setw(8) << Pwx.Ch1Data.read()  <<endl
		;
		// Fifo for debug, is (WO)
		//#!! use get() for consistency with fields?

	    cout.fill(' ');
	    cout <<dec
		<< " Cntl" <<endl
		<< "   Ch2_MsEnable_1  = " << Pwx.Cntl.get_Ch2_MsEnable_1()  <<endl
		<< "   Ch2_UseFifo_1   = " << Pwx.Cntl.get_Ch2_UseFifo_1()   <<endl
		<< "   Ch2_Invert_1    = " << Pwx.Cntl.get_Ch2_Invert_1()    <<endl
		<< "   Ch2_IdleBit_1   = " << Pwx.Cntl.get_Ch2_IdleBit_1()   <<endl
		<< "   Ch2_Repeat_1    = " << Pwx.Cntl.get_Ch2_Repeat_1()    <<endl
		<< "   Ch2_SerMode_1   = " << Pwx.Cntl.get_Ch2_SerMode_1()   <<endl
		<< "   Ch2_Enable_1    = " << Pwx.Cntl.get_Ch2_Enable_1()    <<endl
		<< "   ClearFifo_1     = " << Pwx.Cntl.get_ClearFifo_1()     <<endl
		<< "   Ch1_MsEnable_1  = " << Pwx.Cntl.get_Ch1_MsEnable_1()  <<endl
		<< "   Ch1_UseFifo_1   = " << Pwx.Cntl.get_Ch1_UseFifo_1()   <<endl
		<< "   Ch1_Invert_1    = " << Pwx.Cntl.get_Ch1_Invert_1()    <<endl
		<< "   Ch1_IdleBit_1   = " << Pwx.Cntl.get_Ch1_IdleBit_1()   <<endl
		<< "   Ch1_Repeat_1    = " << Pwx.Cntl.get_Ch1_Repeat_1()    <<endl
		<< "   Ch1_SerMode_1   = " << Pwx.Cntl.get_Ch1_SerMode_1()   <<endl
		<< "   Ch1_Enable_1    = " << Pwx.Cntl.get_Ch1_Enable_1()    <<endl
		<< " Stat" <<endl
		<< "   Ch2_Active_1    = " << Pwx.Stat.get_Ch2_Active_1()    <<endl
		<< "   Ch1_Active_1    = " << Pwx.Stat.get_Ch1_Active_1()    <<endl
		<< "   BusError_1      = " << Pwx.Stat.get_BusError_1()      <<endl
		<< "   Ch2_GapErr_1    = " << Pwx.Stat.get_Ch2_GapErr_1()    <<endl
		<< "   Ch1_GapErr_1    = " << Pwx.Stat.get_Ch1_GapErr_1()    <<endl
		<< "   FifoReadErr_1   = " << Pwx.Stat.get_FifoReadErr_1()   <<endl
		<< "   FifoWriteErr_1  = " << Pwx.Stat.get_FifoWriteErr_1()  <<endl
		<< "   FifoEmpty_1     = " << Pwx.Stat.get_FifoEmpty_1()     <<endl
		<< "   FifoFull_1      = " << Pwx.Stat.get_FifoFull_1()      <<endl
		<< " DmaConf" <<endl
		<< "   DmaEnable_1     = " << Pwx.DmaConf.get_DmaEnable_1()   <<endl
		<< "   DmaPanicLev_8   = " << Pwx.DmaConf.get_DmaPanicLev_8() <<endl
		<< "   DmaReqLev_8     = " << Pwx.DmaConf.get_DmaReqLev_8()   <<endl
		;

	}

    }
    catch ( std::exception& e ) {
	Error::msg( "exception caught:  " ) << e.what() <<endl;
    }
    catch (...) {
	Error::msg( "unexpected exception\n" );
    }

    return ( Error::has_err() ? 1 : 0 );
    //#!! return value?
}

