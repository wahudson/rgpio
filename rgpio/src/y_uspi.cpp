// 2018-01-26  William A. Hudson
//
// rgpio uspi -- Universal SPI Master - RaspberryPi GPIO utility program.
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
#include "rgUniSpi.h"

#include "y_uspi.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class uspi_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		spi_ch[3];	// SPI module number requested

					// registers
    yOpVal		AuxEn;
    yOpVal		Cntl0;
    yOpVal		Cntl1;
    yOpVal		Stat;
    yOpVal		rx;
    yOpVal		rxh;
					// Aux fields
    yOpVal		SpiEnable_1;
					// Cntl0 fields
    yOpVal		Speed_12;
    yOpVal		ChipSelects_3;
    yOpVal		PostInMode_1;
    yOpVal		VariableCs_1;
    yOpVal		VariableWidth_1;
    yOpVal		DoutHoldTime_2;
    yOpVal		EnableSerial_1;
    yOpVal		InRising_1;
    yOpVal		ClearFifos_1;
    yOpVal		OutRising_1;
    yOpVal		InvertClk_1;
    yOpVal		OutMsbFirst_1;
    yOpVal		ShiftLength_6;
					// Cntl1 fields
    yOpVal		CsHighTime_3;
    yOpVal		TxEmptyIrq_1;
    yOpVal		DoneIrq_1;
    yOpVal		InMsbFirst_1;
    yOpVal		KeepInput_1;

    bool		tx;
    bool		txh;

    bool		reset;
    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    uspi_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    uspi_yOptLong	Opx  ( opx );
*/
uspi_yOptLong::uspi_yOptLong( yOption  *opx )
    : yOption( opx )
{
    spi_ch[0]   = 0;
    spi_ch[1]   = 0;
    spi_ch[2]   = 0;

    tx          = 0;
    txh         = 0;

    reset       = 0;
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
uspi_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--Cntl0="     )) { Cntl0.set( this->val() ); }
	else if ( is( "--Cntl1="     )) { Cntl1.set( this->val() ); }
	else if ( is( "--Stat="      )) { Stat.set(  this->val() ); }
	else if ( is( "--AuxEn="     )) { AuxEn.set( this->val() ); }

	else if ( is( "--rx="              )) { rx.set(              val() ); }
	else if ( is( "--rxh="             )) { rxh.set(             val() ); }

	else if ( is( "--SpiEnable_1="     )) { SpiEnable_1.set(     val() ); }

	else if ( is( "--Speed_12="        )) { Speed_12.set(        val() ); }
	else if ( is( "--ChipSelects_3="   )) { ChipSelects_3.set(   val() ); }
	else if ( is( "--PostInMode_1="    )) { PostInMode_1.set(    val() ); }
	else if ( is( "--VariableCs_1="    )) { VariableCs_1.set(    val() ); }
	else if ( is( "--VariableWidth_1=" )) { VariableWidth_1.set( val() ); }
	else if ( is( "--DoutHoldTime_2="  )) { DoutHoldTime_2.set(  val() ); }
	else if ( is( "--EnableSerial_1="  )) { EnableSerial_1.set(  val() ); }
	else if ( is( "--InRising_1="      )) { InRising_1.set(      val() ); }
	else if ( is( "--ClearFifos_1="    )) { ClearFifos_1.set(    val() ); }
	else if ( is( "--OutRising_1="     )) { OutRising_1.set(     val() ); }
	else if ( is( "--InvertClk_1="     )) { InvertClk_1.set(     val() ); }
	else if ( is( "--OutMsbFirst_1="   )) { OutMsbFirst_1.set(   val() ); }
	else if ( is( "--ShiftLength_6="   )) { ShiftLength_6.set(   val() ); }

	else if ( is( "-1"           )) { spi_ch[1]  = 1; }
	else if ( is( "-2"           )) { spi_ch[2]  = 1; }
	else if ( is( "--tx"         )) { tx         = 1; }
	else if ( is( "--txh"        )) { txh        = 1; }

	else if ( is( "--CsHighTime_3="    )) { CsHighTime_3.set(    val() ); }
	else if ( is( "--TxEmptyIrq_1="    )) { TxEmptyIrq_1.set(    val() ); }
	else if ( is( "--DoneIrq_1="       )) { DoneIrq_1.set(       val() ); }
	else if ( is( "--InMsbFirst_1="    )) { InMsbFirst_1.set(    val() ); }
	else if ( is( "--KeepInput_1="     )) { KeepInput_1.set(     val() ); }

	else if ( is( "--reset"      )) { reset      = 1; }
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

    if ( !( spi_ch[1] || spi_ch[2] ) ) {	// default
	spi_ch[1] = 1;
    }

    // Cntl0 bits
    if (                       Speed_12.Val > 0xfff ) {
	Error::msg( "require --Speed_12={0..4095}:  " ) <<
			       Speed_12.Val <<endl;
    }

    if (                       ChipSelects_3.Val > 0x7 ) {
	Error::msg( "require --ChipSelects_3={0..7}:  " ) <<
			       ChipSelects_3.Val <<endl;
    }

    if (                       PostInMode_1.Val > 1 ) {
	Error::msg( "require --PostInMode_1={0,1}:  " ) <<
			       PostInMode_1.Val <<endl;
    }

    if (                       VariableCs_1.Val > 1 ) {
	Error::msg( "require --VariableCs_1={0,1}:  " ) <<
			       VariableCs_1.Val <<endl;
    }

    if (                       VariableWidth_1.Val > 1 ) {
	Error::msg( "require --VariableWidth_1={0,1}:  " ) <<
			       VariableWidth_1.Val <<endl;
    }

    if (                       DoutHoldTime_2.Val > 0x3 ) {
	Error::msg( "require --DoutHoldTime_2={0,3}:  " ) <<
			       DoutHoldTime_2.Val <<endl;
    }

    if (                       EnableSerial_1.Val > 1 ) {
	Error::msg( "require --EnableSerial_1={0,1}:  " ) <<
			       EnableSerial_1.Val <<endl;
    }

    if (                       InRising_1.Val > 1 ) {
	Error::msg( "require --InRising_1={0,1}:  " ) <<
			       InRising_1.Val <<endl;
    }

    if (                       ClearFifos_1.Val > 1 ) {
	Error::msg( "require --ClearFifos_1={0,1}:  " ) <<
			       ClearFifos_1.Val <<endl;
    }

    if (                       OutRising_1.Val > 1 ) {
	Error::msg( "require --OutRising_1={0,1}:  " ) <<
			       OutRising_1.Val <<endl;
    }

    if (                       InvertClk_1.Val > 1 ) {
	Error::msg( "require --InvertClk_1={0,1}:  " ) <<
			       InvertClk_1.Val <<endl;
    }

    if (                       OutMsbFirst_1.Val > 1 ) {
	Error::msg( "require --OutMsbFirst_1={0,1}:  " ) <<
			       OutMsbFirst_1.Val <<endl;
    }

    if (                       ShiftLength_6.Val > 0x3f ) {
	Error::msg( "require --ShiftLength_6={0..63}:  " ) <<
			       ShiftLength_6.Val <<endl;
    }

    // Cntl1 bits
    if (                       CsHighTime_3.Val > 0x7 ) {
	Error::msg( "require --CsHighTime_3={0..7}:  " ) <<
			       CsHighTime_3.Val <<endl;
    }

    if (                       TxEmptyIrq_1.Val > 0x1 ) {
	Error::msg( "require --TxEmptyIrq_1={0..1}:  " ) <<
			       TxEmptyIrq_1.Val <<endl;
    }

    if (                       DoneIrq_1.Val > 1 ) {
	Error::msg( "require --DoneIrq_1={0..1}:  " ) <<
			       DoneIrq_1.Val <<endl;
    }

    if (                       InMsbFirst_1.Val > 1 ) {
	Error::msg( "require --InMsbFirst_1={0..1}:  " ) <<
			       InMsbFirst_1.Val <<endl;
    }

    if (                       KeepInput_1.Val > 1 ) {
	Error::msg( "require --KeepInput_1={0..1}:  " ) <<
			       KeepInput_1.Val <<endl;
    }

    if ( tx && txh ) {
	Error::msg( "require only one of:  --tx|--txh" ) << endl;
    }

    if ( ! (tx || txh) && (get_argc() > 0) ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }

}


/*
* Show option flags.
*/
void
uspi_yOptLong::print_option_flags()
{
    cout << "spi_ch[1]     = " << spi_ch[1]    << endl;
    cout << "spi_ch[2]     = " << spi_ch[2]    << endl;

    cout.fill('0');
    cout <<hex;
    cout << "--Cntl0       = 0x" <<setw(8) << Cntl0.Val       << endl;
    cout << "--Cntl1       = 0x" <<setw(8) << Cntl1.Val       << endl;
    cout << "--Stat        = 0x" <<setw(8) << Stat.Val        << endl;
    cout << "--rx          = 0x" <<setw(8) << rx.Val          << endl;
    cout << "--rxh         = 0x" <<setw(8) << rxh.Val         << endl;
    cout << "--AuxEn       = 0x" <<setw(8) << AuxEn.Val       << endl;

    cout <<dec;
    cout << "--SpiEnable_1     = " << SpiEnable_1.Val     << endl;

    cout << "--Speed_12        = " << Speed_12.Val        << endl;
    cout << "--VariableWidth_1 = " << VariableWidth_1.Val << endl;
    cout << "--ShiftLength_6   = " << ShiftLength_6.Val   << endl;
//#!! not complete

    cout << "--tx          = " << tx           << endl;
    cout << "--txh         = " << txh          << endl;

    cout << "--reset       = " << reset        << endl;
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
*/
void
uspi_yOptLong::print_usage()
{
    cout <<
    "    Universal SPI Master control\n"
    "usage:  " << ProgName << " uspi  [-1][-2] [options..] [--tx|--txh V..]\n"
    "    -1 -2               SPI number, default -1\n"
    "  modify full register:\n"
    "    --Cntl0=V           Control reg 0\n"
    "    --Cntl1=V           Control reg 1\n"
    "    --Stat=V            Status reg (RO)\n"
    "    --AuxEn=V           Auxiliary Enable reg\n"
    "  data transfer:\n"
    "    --rx=N              read {1:4} words from Fifo\n"
    "    --rxh=N             read {1:4} words from FifoH\n"
    "    --tx                write args to Fifo\n"
    "    --txh               write args to FifoH\n"
    "  modify AuxEn bits:\n"
    "    --SpiEnable_1=0     1= Spi access enabled\n"
    "  modify Cntl0 bits:\n"
    "    --Speed_12=0xfff    clock speed, freq=f0/(2*(speed+1))\n"
    "    --ChipSelects_3=0x7 pattern on active CS pins\n"
    "    --PostInMode_1=0    1= Post-Input mode, skip first clk\n"
    "    --VariableCs_1=0    1= CS pattern   from TX fifo [31:29]\n"
    "    --VariableWidth_1=0 1= shift length from TX fifo [28:24]\n"
    "    --DoutHoldTime_2=0  extra data out hold encoded {0..3}\n"
    "    --EnableSerial_1=0  1= enable shifting\n"
    "    --InRising_1=0      1= data In on clk rising, 0= falling\n"
    "    --ClearFifos_1=0    1= hold RX and TX fifos in reset\n"
    "    --OutRising_1=0     1= data Out on clk rising, 0= falling\n"
    "    --InvertClk_1=0     1= idle clk is high, 0= low\n"
    "    --OutMsbFirst_1=0   1= data Out start with MSB, 0= LSB\n"
    "    --ShiftLength_6=32  number of bits to shift {0..32}\n"
    "  modify Cntl1 bits:\n"
    "    --CsHighTime_3=0    additional clock cycles with CS high\n"
    "    --TxEmptyIrq_1=0    1= interrupt when Tx fifo is empty\n"
    "    --DoneIrq_1=0       1= interrupt when Spi is idle\n"
    "    --InMsbFirst_1=0    1= data In start with MSB, 0= LSB\n"
    "    --KeepInput_1=0     1= concatenate into receiver shift register\n"
    "  options:\n"
    "    --reset             init reset values before modifications\n"
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
uspi_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_uspi::y_uspi(
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
y_uspi::doit()
{

    try {
	uspi_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // SPI channels
	const int		SpiMax = 2;
	rgUniSpi*		Spx[SpiMax+1] = {NULL, NULL, NULL};
				// pointers to Spi objects, NULL if not used.

	if ( Opx.spi_ch[1] ) {
	    Spx[1] = new  rgUniSpi  ( 1, AddrMap );	// constructor
	}

	if ( Opx.spi_ch[2] ) {
	    Spx[2] = new  rgUniSpi  ( 2, AddrMap );	// constructor
	}

	if ( Opx.debug ) {
	    for ( int ii=1;  ii<=SpiMax;  ii++ )
	    {
		rgUniSpi	*spi = Spx[ii];
		int		n;

		if ( spi == NULL ) {
		    continue;
		}
		n = spi->get_spi_num();

		// Note cout does not know how to show (volatile uint32_t*).

		cout << "    " << n << ".GpioBase=       "
		     <<hex << (uint32_t*) spi->get_base_addr() <<endl;

		cout << "    " << n << ".AuxEn.addr=     "
		     <<hex << (uint32_t*) spi->AuxEn.addr() <<endl;

		cout << "    " << n << ".Cntl0.addr=     "
		     <<hex << (uint32_t*) spi->Cntl0.addr() <<endl;

		cout << "    " << n << ".diff_Cntl0=     "
		     <<hex
		     << (spi->Cntl0.addr() - spi->get_base_addr())*4 <<endl;

		cout <<dec <<endl;
	    }
	}


    // Process each device

	for ( int ii=1;  ii<=SpiMax;  ii++ )
	{
	    rgUniSpi		*spi = Spx[ii];
	    bool		md = 0;		// modify flag

	    spi = Spx[ii];

	    if ( spi == NULL ) {
		continue;
	    }

	    cout << "Spi" << spi->get_spi_num() << ":" <<endl;

	    string		ns = "   " + std::to_string( spi->get_spi_num() );

#define APPLX( X )    if ( Opx.X.Given ) { spi->X.put( Opx.X.Val );  md = 1; }
#define APPLY( X, Y ) if ( Opx.X.Given ) { Y( Opx.X.Val );  md = 1; }

	// Access Enable - hardware update first

	    APPLY( AuxEn,            spi->AuxEn.write )		// register

	    APPLY( SpiEnable_1,      spi->write_SpiEnable_1 )	// bit

	// Registers

	    if ( Opx.reset ) {
		Opx.trace_msg( "Init reset" );
		spi->init_put_reset();
		md = 1;
	    }
	    else {
		Opx.trace_msg( "Grab regs" );
		spi->grab_regs();
	    }

	    APPLX( Cntl0 )
	    APPLX( Cntl1 )
	    APPLX( Stat  )

	// Fields

	    APPLY( Speed_12,         spi->Cntl0.put_Speed_12         )
	    APPLY( ChipSelects_3,    spi->Cntl0.put_ChipSelects_3    )
	    APPLY( PostInMode_1,     spi->Cntl0.put_PostInMode_1     )
	    APPLY( VariableCs_1,     spi->Cntl0.put_VariableCs_1     )
	    APPLY( VariableWidth_1,  spi->Cntl0.put_VariableWidth_1  )
	    APPLY( DoutHoldTime_2,   spi->Cntl0.put_DoutHoldTime_2   )
	    APPLY( EnableSerial_1,   spi->Cntl0.put_EnableSerial_1   )
	    APPLY( InRising_1,       spi->Cntl0.put_InRising_1       )
	    APPLY( ClearFifos_1,     spi->Cntl0.put_ClearFifos_1     )
	    APPLY( OutRising_1,      spi->Cntl0.put_OutRising_1      )
	    APPLY( InvertClk_1,      spi->Cntl0.put_InvertClk_1      )
	    APPLY( OutMsbFirst_1,    spi->Cntl0.put_OutMsbFirst_1    )
	    APPLY( ShiftLength_6,    spi->Cntl0.put_ShiftLength_6    )

	    APPLY( CsHighTime_3,     spi->Cntl1.put_CsHighTime_3     )
	    APPLY( TxEmptyIrq_1,     spi->Cntl1.put_TxEmptyIrq_1     )
	    APPLY( DoneIrq_1,        spi->Cntl1.put_DoneIrq_1        )
	    APPLY( InMsbFirst_1,     spi->Cntl1.put_InMsbFirst_1     )
	    APPLY( KeepInput_1,      spi->Cntl1.put_KeepInput_1      )

	    if ( md ) {			// modify registers
		Opx.trace_msg( "Modify regs" );
		spi->push_regs();
	    }

	    if ( Opx.tx ) {
		char*		cp;
		uint32_t	vv;

		Opx.trace_msg( "Write Tx Fifo" );
		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << ns << ".write_Fifo:  0x" <<hex <<setw(8) << vv << endl;
		    spi->Fifo.write( vv );
		}
		cout << dec;
	    }
	    //#!! Note --tx works for only one Spi.

	    if ( Opx.txh ) {
		char*		cp;
		uint32_t	vv;

		Opx.trace_msg( "Write Tx FifoH" );
		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << ns << ".write_FifoH:  0x" <<hex <<setw(8) << vv << endl;
		    spi->FifoH.write( vv );
		}
		cout << dec;
	    }
	    //#!! Note --txh works for only one Spi.

	    if ( md || Opx.tx || Opx.txh ) {	// modify registers
		Opx.trace_msg( "Grab regs" );
		spi->grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
	     << ns << ".AuxEn    = 0x" <<setw(8) << spi->AuxEn.read()     <<endl
	     << ns << ".Cntl0    = 0x" <<setw(8) << spi->Cntl0.get()      <<endl
	     << ns << ".Cntl1    = 0x" <<setw(8) << spi->Cntl1.get()      <<endl
	     << ns << ".Stat     = 0x" <<setw(8) << spi->Stat.get()       <<endl
	     << ns << ".Peek     = 0x" <<setw(8) << spi->Peek.read()      <<endl
	     ;

	    cout.fill(' ');
	    cout <<dec
	     << ns << ".SpiEnable_1    = " << spi->read_SpiEnable_1()   <<endl
	     << ns << ".SpiIrq_1       = " << spi->read_SpiIrq_1()      <<endl

	     << " Cntl0" <<endl
	     << ns << ".Speed_12       = " << spi->Cntl0.get_Speed_12()        <<endl
	     << ns << ".ChipSelects_3  = " << spi->Cntl0.get_ChipSelects_3()   <<endl
	     << ns << ".PostInMode_1   = " << spi->Cntl0.get_PostInMode_1()    <<endl
	     << ns << ".VariableCs_1   = " << spi->Cntl0.get_VariableCs_1()    <<endl
	     << ns << ".VariableWidth_1= " << spi->Cntl0.get_VariableWidth_1() <<endl
	     << ns << ".DoutHoldTime_2 = " << spi->Cntl0.get_DoutHoldTime_2()  <<endl
	     << ns << ".EnableSerial_1 = " << spi->Cntl0.get_EnableSerial_1()  <<endl
	     << ns << ".InRising_1     = " << spi->Cntl0.get_InRising_1()      <<endl
	     << ns << ".ClearFifos_1   = " << spi->Cntl0.get_ClearFifos_1()    <<endl
	     << ns << ".OutRising_1    = " << spi->Cntl0.get_OutRising_1()     <<endl
	     << ns << ".InvertClk_1    = " << spi->Cntl0.get_InvertClk_1()     <<endl
	     << ns << ".OutMsbFirst_1  = " << spi->Cntl0.get_OutMsbFirst_1()   <<endl
	     << ns << ".ShiftLength_6  = " << spi->Cntl0.get_ShiftLength_6()   <<endl
	     << " Cntl1" <<endl
	     << ns << ".CsHighTime_3   = " << spi->Cntl1.get_CsHighTime_3()    <<endl
	     << ns << ".TxEmptyIrq_1   = " << spi->Cntl1.get_TxEmptyIrq_1()    <<endl
	     << ns << ".DoneIrq_1      = " << spi->Cntl1.get_DoneIrq_1()       <<endl
	     << ns << ".InMsbFirst_1   = " << spi->Cntl1.get_InMsbFirst_1()    <<endl
	     << ns << ".KeepInput_1    = " << spi->Cntl1.get_KeepInput_1()     <<endl
	     << " Status" <<endl
	     << ns << ".TxLevel_3      = " << spi->Stat.get_TxLevel_3()   <<endl
	     << ns << ".RxLevel_3      = " << spi->Stat.get_RxLevel_3()   <<endl
	     << ns << ".TxFull_1       = " << spi->Stat.get_TxFull_1()    <<endl
	     << ns << ".TxEmpty_1      = " << spi->Stat.get_TxEmpty_1()   <<endl
	     << ns << ".RxFull_1       = " << spi->Stat.get_RxFull_1()    <<endl
	     << ns << ".RxEmpty_1      = " << spi->Stat.get_RxEmpty_1()   <<endl
	     << ns << ".Busy_1         = " << spi->Stat.get_Busy_1()      <<endl
	     << ns << ".BitCount_6     = " << spi->Stat.get_BitCount_6()  <<endl
	     ;

	    cout.fill('0');
	    cout <<hex;

	    if ( Opx.rx.Val ) Opx.trace_msg( "Read Rx Fifo" );
	    for ( uint32_t jj = 1;  jj <= Opx.rx.Val;   jj++ ) {
	      cout << ns << ".read_Fifo:  0x" <<setw(8) << spi->Fifo.read()  <<endl;
	    }

	    if ( Opx.rxh.Val ) Opx.trace_msg( "Read Rx FifoH" );
	    for ( uint32_t jj = 1;  jj <= Opx.rxh.Val;  jj++ ) {
	      cout << ns << ".read_FifoH: 0x" <<setw(8) << spi->FifoH.read() <<endl;
	    }
	    cout <<dec;
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

