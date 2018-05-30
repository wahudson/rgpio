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
    yOpVal		AuxEnable;
    yOpVal		cntl0;
    yOpVal		cntl1;
    yOpVal		stat;
    yOpVal		rx;
    yOpVal		rxh;
					// Aux fields
    yOpVal		Spi_Enable_1;
					// Cntl0 fields
    yOpVal		Speed_12;
    yOpVal		ChipSelects_3;
    yOpVal		PostInMode_1;
    yOpVal		VariableCS_1;
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
    yOpVal		TxEmptyIRQ_1;
    yOpVal		DoneIRQ_1;
    yOpVal		InMsbFirst_1;
    yOpVal		KeepInput_1;

    bool		tx;
    bool		txh;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    uspi_yOptLong( yOption  *opx );	// constructor

    uint32_t		uint();

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

    int			doit();		//#!! not here
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

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Return arg as an uint32_t.
*/
uint32_t
uspi_yOptLong::uint()
{
    return  strtoul( this->val(), NULL, 0 );
}

/*
* Parse options.
*/
void
uspi_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--cntl0="     )) { cntl0.set( this->val() ); }
	else if ( is( "--cntl1="     )) { cntl1.set( this->val() ); }
	else if ( is( "--stat="      )) { stat.set(  this->val() ); }
	else if ( is( "--AuxEnable=" )) { AuxEnable.set(   val() ); }

	else if ( is( "--rx="              )) { rx.set(              val() ); }
	else if ( is( "--rxh="             )) { rxh.set(             val() ); }

	else if ( is( "--Spi_Enable_1="    )) { Spi_Enable_1.set(    val() ); }

	else if ( is( "--Speed_12="        )) { Speed_12.set(        val() ); }
	else if ( is( "--ChipSelects_3="   )) { ChipSelects_3.set(   val() ); }
	else if ( is( "--PostInMode_1="    )) { PostInMode_1.set(    val() ); }
	else if ( is( "--VariableCS_1="    )) { VariableCS_1.set(    val() ); }
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
	else if ( is( "--TxEmptyIRQ_1="    )) { TxEmptyIRQ_1.set(    val() ); }
	else if ( is( "--DoneIRQ_1="       )) { DoneIRQ_1.set(       val() ); }
	else if ( is( "--InMsbFirst_1="    )) { InMsbFirst_1.set(    val() ); }
	else if ( is( "--KeepInput_1="     )) { KeepInput_1.set(     val() ); }

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

    if (                       VariableCS_1.Val > 1 ) {
	Error::msg( "require --VariableCS_1={0,1}:  " ) <<
			       VariableCS_1.Val <<endl;
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

    if (                       TxEmptyIRQ_1.Val > 0x1 ) {
	Error::msg( "require --TxEmptyIRQ_1={0..1}:  " ) <<
			       TxEmptyIRQ_1.Val <<endl;
    }

    if (                       DoneIRQ_1.Val > 1 ) {
	Error::msg( "require --DoneIRQ_1={0..1}:  " ) <<
			       DoneIRQ_1.Val <<endl;
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
    cout << "--cntl0       = 0x" <<setw(8) << cntl0.Val       << endl;
    cout << "--cntl1       = 0x" <<setw(8) << cntl1.Val       << endl;
    cout << "--stat        = 0x" <<setw(8) << stat.Val        << endl;
    cout << "--rx          = 0x" <<setw(8) << rx.Val          << endl;
    cout << "--rxh         = 0x" <<setw(8) << rxh.Val         << endl;
    cout << "--AuxEnable   = 0x" <<setw(8) << AuxEnable.Val   << endl;

    cout <<dec;
    cout << "--Spi_Enable_1    = " << Spi_Enable_1.Val    << endl;

    cout << "--Speed_12        = " << Speed_12.Val        << endl;
    cout << "--VariableWidth_1 = " << VariableWidth_1.Val << endl;
    cout << "--ShiftLength_6   = " << ShiftLength_6.Val   << endl;
//#!! not complete

    cout << "--tx          = " << tx           << endl;
    cout << "--txh         = " << txh          << endl;

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
    "usage:  " << ProgName << " uspi  -N..  [options..] [--tx|--txh V..]\n"
    "    -N                  SPI number {1,2}\n"
    "  modify full register:\n"
    "    --cntl0=V           Control reg 0\n"
    "    --cntl1=V           Control reg 1\n"
    "    --stat=V            Status reg\n"
    "    --AuxEnable=V       AuxEnable reg\n"
    "  data transfer:\n"
    "    --rx=N              read {1:4} words from Fifo\n"
    "    --rxh=N             read {1:4} words from FifoH\n"
    "    --tx                write args to Fifo\n"
    "    --txh               write args to FifoH\n"
    "  modify Aux bit fields:\n"
    "    --Spi_Enable_1=0    1= Spi access enabled\n"
    "  modify Cntl0 bits:\n"
    "    --Speed_12=0xfff    clock speed, freq=f0/(2*(speed+1))\n"
    "    --ChipSelects_3=0x7 pattern on active CS pins\n"
    "    --PostInMode_1=0    1= Post-Input mode, skip first clk\n"
    "    --VariableCS_1=0    1= CS pattern   from TX fifo [31:29]\n"
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
    "    --TxEmptyIRQ_1=0    1= interrupt when Tx fifo is empty\n"
    "    --DoneIRQ_1=0       1= interrupt when Spi is idle\n"
    "    --InMsbFirst_1=0    1= data In start with MSB, 0= LSB\n"
    "    --KeepInput_1=0     1= concatenate into receiver shift register\n"
    "  options:\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
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

	if ( Opx.verbose ) {
	    cout << "Universal SPI Master:" << endl;
	}

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

		cout << "    " << n << ".addr_AuxEnable= "
		     <<hex << (uint32_t*) spi->addr_AuxEnable() <<endl;

		cout << "    " << n << ".addr_Cntl0=     "
		     <<hex << (uint32_t*) spi->addr_Cntl0() <<endl;

		cout << "    " << n << ".diff_Cntl0=     "
		     <<hex
		     << (spi->addr_Cntl0() - spi->get_base_addr())*4 <<endl;

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

	    string		ns = "  " + std::to_string( spi->get_spi_num() );

	    // direct access
	    if (       Opx.AuxEnable.Given ) {
		spi->write_AuxEnable(       Opx.AuxEnable.Val       );  md = 1;
	    }
	    if (       Opx.Spi_Enable_1.Given    ) {
		spi->write_Spi_Enable_1(    Opx.Spi_Enable_1.Val    );  md = 1;
	    }

	    if ( Opx.debug ) {
		cout << "  Grab regs" <<endl;
	    }
	    spi->grab_regs();

	    if ( Opx.cntl0.Given ) { spi->put_Cntl0( Opx.cntl0.Val );  md = 1; }
	    if ( Opx.cntl1.Given ) { spi->put_Cntl1( Opx.cntl1.Val );  md = 1; }
	    if ( Opx.stat.Given  ) { spi->put_Stat(  Opx.stat.Val  );  md = 1; }

	// Cntl0 bits

	    if (     Opx.Speed_12.Given ) {
		spi->put_Speed_12(        Opx.Speed_12.Val );         md = 1;
	    }

	    if (     Opx.ChipSelects_3.Given ) {
		spi->put_ChipSelects_3(   Opx.ChipSelects_3.Val );    md = 1;
	    }

	    if (     Opx.PostInMode_1.Given ) {
		spi->put_PostInMode_1(    Opx.PostInMode_1.Val );     md = 1;
	    }

	    if (     Opx.VariableCS_1.Given ) {
		spi->put_VariableCS_1(    Opx.VariableCS_1.Val );     md = 1;
	    }

	    if (     Opx.VariableWidth_1.Given ) {
		spi->put_VariableWidth_1( Opx.VariableWidth_1.Val );  md = 1;
	    }

	    if (     Opx.DoutHoldTime_2.Given ) {
		spi->put_DoutHoldTime_2(  Opx.DoutHoldTime_2.Val );   md = 1;
	    }

	    if (     Opx.EnableSerial_1.Given ) {
		spi->put_EnableSerial_1(  Opx.EnableSerial_1.Val );   md = 1;
	    }

	    if (     Opx.InRising_1.Given ) {
		spi->put_InRising_1(      Opx.InRising_1.Val );       md = 1;
	    }

	    if (     Opx.ClearFifos_1.Given ) {
		spi->put_ClearFifos_1(    Opx.ClearFifos_1.Val );     md = 1;
	    }

	    if (     Opx.OutRising_1.Given ) {
		spi->put_OutRising_1(     Opx.OutRising_1.Val );      md = 1;
	    }

	    if (     Opx.InvertClk_1.Given ) {
		spi->put_InvertClk_1(     Opx.InvertClk_1.Val );      md = 1;
	    }

	    if (     Opx.OutMsbFirst_1.Given ) {
		spi->put_OutMsbFirst_1(   Opx.OutMsbFirst_1.Val );    md = 1;
	    }

	    if (     Opx.ShiftLength_6.Given ) {
		spi->put_ShiftLength_6(   Opx.ShiftLength_6.Val );    md = 1;
	    }

	// Cntl1 bits

	    if (     Opx.CsHighTime_3.Given ) {
		spi->put_CsHighTime_3(    Opx.CsHighTime_3.Val );     md = 1;
	    }

	    if (     Opx.TxEmptyIRQ_1.Given ) {
		spi->put_TxEmptyIRQ_1(    Opx.TxEmptyIRQ_1.Val );     md = 1;
	    }

	    if (     Opx.DoneIRQ_1.Given ) {
		spi->put_DoneIRQ_1(       Opx.DoneIRQ_1.Val );        md = 1;
	    }

	    if (     Opx.InMsbFirst_1.Given ) {
		spi->put_InMsbFirst_1(    Opx.InMsbFirst_1.Val );     md = 1;
	    }

	    if (     Opx.KeepInput_1.Given ) {
		spi->put_KeepInput_1(     Opx.KeepInput_1.Val );      md = 1;
	    }

	    if ( md ) {			// modify registers
		if ( Opx.debug ) {
		    cout << "  Modify regs" <<endl;
		}
		spi->push_regs();
	    }

	    if ( Opx.tx ) {
		char*		cp;
		uint32_t	vv;
		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << "  put_tx:  0x" <<hex <<setw(8) << vv << endl;
		    spi->write_Fifo( vv );
		}
		cout << dec;
	    }
	    //#!! Note --tx works for only one Spi.

	    if ( Opx.txh ) {
		char*		cp;
		uint32_t	vv;
		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << "  put_txh:  0x" <<hex <<setw(8) << vv << endl;
		    spi->write_FifoH( vv );
		}
		cout << dec;
	    }
	    //#!! Note --txh works for only one Spi.

	    if ( md || Opx.tx || Opx.txh ) {	// modify registers
		if ( Opx.debug ) {
		    cout << "  Grab regs" <<endl;
		}
		spi->grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
	     << ns << ".AuxEnable= 0x" <<setw(8) << spi->read_AuxEnable() <<endl
	     << ns << ".Cntl0=     0x" <<setw(8) << spi->get_Cntl0()      <<endl
	     << ns << ".Cntl1=     0x" <<setw(8) << spi->get_Cntl1()      <<endl
	     << ns << ".Stat=      0x" <<setw(8) << spi->get_Stat()       <<endl
	     << ns << ".Peek=      0x" <<setw(8) << spi->read_Peek()      <<endl
	     ;

	    cout.fill(' ');
	    cout <<dec
	     << ns << ".Spi_Enable_1   = " << spi->read_Spi_Enable_1()   <<endl
	     << ns << ".Spi_IRQ_1      = " << spi->read_Spi_IRQ_1()      <<endl

	     << " Cntl0" <<endl
	     << ns << ".Speed_12       = " << spi->get_Speed_12()        <<endl
	     << ns << ".ChipSelects_3  = " << spi->get_ChipSelects_3()   <<endl
	     << ns << ".PostInMode_1   = " << spi->get_PostInMode_1()    <<endl
	     << ns << ".VariableCS_1   = " << spi->get_VariableCS_1()    <<endl
	     << ns << ".VariableWidth_1= " << spi->get_VariableWidth_1() <<endl
	     << ns << ".DoutHoldTime_2 = " << spi->get_DoutHoldTime_2()  <<endl
	     << ns << ".EnableSerial_1 = " << spi->get_EnableSerial_1()  <<endl
	     << ns << ".InRising_1     = " << spi->get_InRising_1()      <<endl
	     << ns << ".ClearFifos_1   = " << spi->get_ClearFifos_1()    <<endl
	     << ns << ".OutRising_1    = " << spi->get_OutRising_1()     <<endl
	     << ns << ".InvertClk_1    = " << spi->get_InvertClk_1()     <<endl
	     << ns << ".OutMsbFirst_1  = " << spi->get_OutMsbFirst_1()   <<endl
	     << ns << ".ShiftLength_6  = " << spi->get_ShiftLength_6()   <<endl
	     << " Cntl1" <<endl
	     << ns << ".CsHighTime_3   = " << spi->get_CsHighTime_3()    <<endl
	     << ns << ".TxEmptyIRQ_1   = " << spi->get_TxEmptyIRQ_1()    <<endl
	     << ns << ".DoneIRQ_1      = " << spi->get_DoneIRQ_1()       <<endl
	     << ns << ".InMsbFirst_1   = " << spi->get_InMsbFirst_1()    <<endl
	     << ns << ".KeepInput_1    = " << spi->get_KeepInput_1()     <<endl
	     << " Status" <<endl
	     << ns << ".TxLevel_3      = " << spi->get_TxLevel_3()   <<endl
	     << ns << ".RxLevel_3      = " << spi->get_RxLevel_3()   <<endl
	     << ns << ".TxFull_1       = " << spi->get_TxFull_1()    <<endl
	     << ns << ".TxEmpty_1      = " << spi->get_TxEmpty_1()   <<endl
	     << ns << ".RxFull_1       = " << spi->get_RxFull_1()    <<endl
	     << ns << ".RxEmpty_1      = " << spi->get_RxEmpty_1()   <<endl
	     << ns << ".Busy_1         = " << spi->get_Busy_1()      <<endl
	     << ns << ".BitCount_6     = " << spi->get_BitCount_6()  <<endl
	     <<endl;

	    cout.fill('0');
	    cout <<hex;
	    for ( uint32_t jj = 1;  jj <= Opx.rx.Val;   jj++ ) {
	      cout << ns << ".Fifo=  0x" <<setw(8) << spi->read_Fifo()  <<endl;
	    }
	    for ( uint32_t jj = 1;  jj <= Opx.rxh.Val;  jj++ ) {
	      cout << ns << ".FifoH= 0x" <<setw(8) << spi->read_FifoH() <<endl;
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

