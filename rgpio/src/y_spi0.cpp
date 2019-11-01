// 2019-10-27  William A. Hudson
//
// rgpio spi0 -- SPI0 Master - RaspberryPi GPIO utility program.
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
#include "rgSpi0.h"

#include "y_spi0.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class spi0_yOptLong : public yOption {

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
    yOpVal		CntlStat;
    yOpVal		Fifo;
    yOpVal		ClkDiv;
    yOpVal		DmaLen;
    yOpVal		Lossi;
    yOpVal		DmaReq;

    yOpVal		rx;
    bool		tx;
					// CntlStat fields
    yOpVal		LossiWord_1;
    yOpVal		LossiDmaEn_1;
    yOpVal		CsPolarity_3;

    yOpVal		LossiEnable_1;
    yOpVal		ReadEnable_1;
    yOpVal		DmaEndCs_1;
    yOpVal		IrqRxHalf_1;
    yOpVal		IrqTxEmpty_1;
    yOpVal		DmaEnable_1;
    yOpVal		RunActive_1;
    yOpVal		CsPolarity_1;
    yOpVal		ClearRxTxFifo_2;
    yOpVal		ClockPolarity_1;
    yOpVal		ClockPhase_1;
    yOpVal		ChipSelectN_2;
					// ClkDiv fields
    yOpVal		ClockDiv_16;
					// DmaLen fields
    yOpVal		DmaDataLen_16;
					// Lossi  fields
    yOpVal		LossiHoldDly_4;
					// DmaReq fields
    yOpVal		DmaRxPanicLev_8;
    yOpVal		DmaRxReqLev_8;
    yOpVal		DmaTxPanicLev_8;
    yOpVal		DmaTxReqLev_8;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    spi0_yOptLong( yOption  *opx );	// constructor

    uint32_t		uint();

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );

    int			doit();		//#!! not here
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    spi0_yOptLong	Opx  ( opx );
*/
spi0_yOptLong::spi0_yOptLong( yOption  *opx )
    : yOption( opx )
{
    tx          = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Return arg as an uint32_t.
*/
uint32_t
spi0_yOptLong::uint()
{
    return  strtoul( this->val(), NULL, 0 );
}

/*
* Parse options.
*/
void
spi0_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--CntlStat="  )) { CntlStat.set( this->val() ); }
	else if ( is( "--ClkDiv="    )) { ClkDiv.set(   this->val() ); }
	else if ( is( "--DmaLen="    )) { DmaLen.set(   this->val() ); }
	else if ( is( "--Lossi="     )) { Lossi.set(    this->val() ); }
	else if ( is( "--DmaReq="    )) { DmaReq.set(   this->val() ); }

	else if ( is( "--rx="              )) { rx.set(              val() ); }
	else if ( is( "--tx"               )) { tx         = 1; }

	else if ( is( "--LossiWord_1="     )) { LossiWord_1.set(     val() ); }
	else if ( is( "--LossiDmaEn_1="    )) { LossiDmaEn_1.set(    val() ); }
	else if ( is( "--CsPolarity_3="    )) { CsPolarity_3.set(    val() ); }

	else if ( is( "--LossiEnable_1="   )) { LossiEnable_1.set(   val() ); }
	else if ( is( "--ReadEnable_1="    )) { ReadEnable_1.set(    val() ); }
	else if ( is( "--DmaEndCs_1="      )) { DmaEndCs_1.set(      val() ); }
	else if ( is( "--IrqRxHalf_1="     )) { IrqRxHalf_1.set(     val() ); }
	else if ( is( "--IrqTxEmpty_1="    )) { IrqTxEmpty_1.set(    val() ); }
	else if ( is( "--DmaEnable_1="     )) { DmaEnable_1.set(     val() ); }
	else if ( is( "--RunActive_1="     )) { RunActive_1.set(     val() ); }
	else if ( is( "--CsPolarity_1="    )) { CsPolarity_1.set(    val() ); }
	else if ( is( "--ClearRxTxFifo_2=" )) { ClearRxTxFifo_2.set( val() ); }
	else if ( is( "--ClockPolarity_1=" )) { ClockPolarity_1.set( val() ); }
	else if ( is( "--ClockPhase_1="    )) { ClockPhase_1.set(    val() ); }
	else if ( is( "--ChipSelectN_2="   )) { ChipSelectN_2.set(   val() ); }

	else if ( is( "--ClockDiv_16="     )) { ClockDiv_16.set(     val() ); }
	else if ( is( "--DmaDataLen_16="   )) { DmaDataLen_16.set(   val() ); }
	else if ( is( "--LossiHoldDly_4="  )) { LossiHoldDly_4.set(  val() ); }

	else if ( is( "--DmaRxPanicLev_8=" )) { DmaRxPanicLev_8.set( val() ); }
	else if ( is( "--DmaRxReqLev_8="   )) { DmaRxReqLev_8.set(   val() ); }
	else if ( is( "--DmaTxPanicLev_8=" )) { DmaTxPanicLev_8.set( val() ); }
	else if ( is( "--DmaTxReqLev_8="   )) { DmaTxReqLev_8.set(   val() ); }

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

    // CntlStat fields

    if (                       LossiWord_1.Val > 1 ) {
	Error::msg( "require --LossiWord_1={0,1}:  " ) <<
			       LossiWord_1.Val <<endl;
    }

    if (                       LossiDmaEn_1.Val > 1 ) {
	Error::msg( "require --LossiDmaEn_1={0,1}:  " ) <<
			       LossiDmaEn_1.Val <<endl;
    }

    if (                       CsPolarity_3.Val > 7 ) {
	Error::msg( "require --CsPolarity_3={0..7}:  " ) <<
			       CsPolarity_3.Val <<endl;
    }

    if (                       LossiEnable_1.Val > 1 ) {
	Error::msg( "require --LossiEnable_1={0,1}:  " ) <<
			       LossiEnable_1.Val <<endl;
    }

    if (                       ReadEnable_1.Val > 1 ) {
	Error::msg( "require --ReadEnable_1={0,1}:  " ) <<
			       ReadEnable_1.Val <<endl;
    }

    if (                       DmaEndCs_1.Val > 1 ) {
	Error::msg( "require --DmaEndCs_1={0,1}:  " ) <<
			       DmaEndCs_1.Val <<endl;
    }

    if (                       IrqRxHalf_1.Val > 1 ) {
	Error::msg( "require --IrqRxHalf_1={0,1}:  " ) <<
			       IrqRxHalf_1.Val <<endl;
    }

    if (                       IrqTxEmpty_1.Val > 1 ) {
	Error::msg( "require --IrqTxEmpty_1={0,1}:  " ) <<
			       IrqTxEmpty_1.Val <<endl;
    }

    if (                       DmaEnable_1.Val > 1 ) {
	Error::msg( "require --DmaEnable_1={0,1}:  " ) <<
			       DmaEnable_1.Val <<endl;
    }

    if (                       RunActive_1.Val > 1 ) {
	Error::msg( "require --RunActive_1={0,1}:  " ) <<
			       RunActive_1.Val <<endl;
    }

    if (                       CsPolarity_1.Val > 1 ) {
	Error::msg( "require --CsPolarity_1={0,1}:  " ) <<
			       CsPolarity_1.Val <<endl;
    }

    if (                       ClearRxTxFifo_2.Val > 3 ) {
	Error::msg( "require --ClearRxTxFifo_2={0..3}:  " ) <<
			       ClearRxTxFifo_2.Val <<endl;
    }

    if (                       ClockPolarity_1.Val > 1 ) {
	Error::msg( "require --ClockPolarity_1={0,1}:  " ) <<
			       ClockPolarity_1.Val <<endl;
    }

    if (                       ClockPhase_1.Val > 1 ) {
	Error::msg( "require --ClockPhase_1={0,1}:  " ) <<
			       ClockPhase_1.Val <<endl;
    }

    if (                       ChipSelectN_2.Val > 0x3 ) {
	Error::msg( "require --ChipSelectN_2={0..3}:  " ) <<
			       ChipSelectN_2.Val <<endl;
    }

    if (                       ClockDiv_16.Val > 0xffff ) {
	Error::msg( "require --ClockDiv_16 <= 0xffff:  " ) <<
			       ClockDiv_16.Val <<endl;
    }

    if (                       DmaDataLen_16.Val > 0xffff ) {
	Error::msg( "require --DmaDataLen_16 <= 0xffff:  " ) <<
			       DmaDataLen_16.Val <<endl;
    }

    if (                       LossiHoldDly_4.Val > 0xf ) {
	Error::msg( "require --LossiHoldDly_4 <= 0xf:  " ) <<
			       LossiHoldDly_4.Val <<endl;
    }

    if (                       DmaRxPanicLev_8.Val > 0xff ) {
	Error::msg( "require --DmaRxPanicLev_8 <= 0xff:  " ) <<
			       DmaRxPanicLev_8.Val <<endl;
    }

    if (                       DmaRxReqLev_8.Val > 0xff ) {
	Error::msg( "require --DmaRxReqLev_8 <= 0xff:  " ) <<
			       DmaRxReqLev_8.Val <<endl;
    }

    if (                       DmaTxPanicLev_8.Val > 0xff ) {
	Error::msg( "require --DmaTxPanicLev_8 <= 0xff:  " ) <<
			       DmaTxPanicLev_8.Val <<endl;
    }

    if (                       DmaTxReqLev_8.Val > 0xff ) {
	Error::msg( "require --DmaTxReqLev_8 <= 0xff:  " ) <<
			       DmaTxReqLev_8.Val <<endl;
    }

    if ( (! tx) && (get_argc() > 0) ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }
}


/*
* Show option flags.
*/
void
spi0_yOptLong::print_option_flags()
{
    cout.fill('0');
    cout <<hex;
    cout << "--CntlStat    = 0x" <<setw(8) << CntlStat.Val    << endl;
    cout << "--ClkDiv      = 0x" <<setw(8) << ClkDiv.Val      << endl;
    cout << "--DmaLen      = 0x" <<setw(8) << DmaLen.Val      << endl;
    cout << "--Lossi       = 0x" <<setw(8) << Lossi.Val       << endl;
    cout << "--DmaReq      = 0x" <<setw(8) << DmaReq.Val      << endl;

    cout << "--rx          = " << rx.Val       << endl;
    cout << "--tx          = " << tx           << endl;

    cout <<dec;
    cout << "--LossiWord_1     = " << LossiWord_1.Val     << endl;
    cout << "--LossiDmaEn_1    = " << LossiDmaEn_1.Val    << endl;
    cout << "--CsPolarity_3    = " << CsPolarity_3.Val    << endl;

    cout << "--LossiEnable_1   = " << LossiEnable_1.Val   << endl;
    cout << "--ReadEnable_1    = " << ReadEnable_1.Val    << endl;
    cout << "--DmaEndCs_1      = " << DmaEndCs_1.Val      << endl;
    cout << "--IrqRxHalf_1     = " << IrqRxHalf_1.Val     << endl;
    cout << "--IrqTxEmpty_1    = " << IrqTxEmpty_1.Val    << endl;
    cout << "--DmaEnable_1     = " << DmaEnable_1.Val     << endl;
    cout << "--RunActive_1     = " << RunActive_1.Val     << endl;
    cout << "--CsPolarity_1    = " << CsPolarity_1.Val    << endl;
    cout << "--ClearRxTxFifo_2 = " << ClearRxTxFifo_2.Val << endl;
    cout << "--ClockPolarity_1 = " << ClockPolarity_1.Val << endl;
    cout << "--ClockPhase_1    = " << ClockPhase_1.Val    << endl;
    cout << "--ChipSelectN_2   = " << ChipSelectN_2.Val   << endl;

    cout << "--ClockDiv_16     = " << ClockDiv_16.Val     << endl;
    cout << "--DmaDataLen_16   = " << DmaDataLen_16.Val   << endl;
    cout << "--LossiHoldDly_4  = " << LossiHoldDly_4.Val  << endl;

    cout << "--DmaRxPanicLev_8 = " << DmaRxPanicLev_8.Val << endl;
    cout << "--DmaRxReqLev_8   = " << DmaRxReqLev_8.Val   << endl;
    cout << "--DmaTxPanicLev_8 = " << DmaTxPanicLev_8.Val << endl;
    cout << "--DmaTxReqLev_8   = " << DmaTxReqLev_8.Val   << endl;

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
spi0_yOptLong::print_usage()
{
    cout <<
    "    SPI0 Master control\n"
    "usage:  " << ProgName << " spi0  [options..] [--tx V..]\n"
    "  modify full register:  (V= 32-bit value)\n"
    "    --CntlStat=V        Control and Status\n"
    "    --ClkDiv=V          Clock Divider\n"
    "    --DmaLen=V          DMA Data Length\n"
    "    --Lossi=V           LoSSI mode Control\n"
    "    --DmaReq=V          DMA DREQ Controls\n"
    "  data transfer:\n"
    "    --rx=N              read N entries from 16-entry Fifo\n"
    "    --tx                write arg values to Fifo\n"
    "  modify CntlStat bit fields:\n"
    "    --LossiWord_1=0     fifo write 1= 32-bit word, 0= 1-byte\n"
    "    --LossiDmaEn_1=0    enable DMA mode in LoSSI mode\n"
    "    --CsPolarity_3=0    chip select polarity CS[2:0]\n"
    "    --LossiEnable_1=0   1= LoSSI mode, 0= SPI mode\n"
    "    --ReadEnable_1=0    bidirectional mode 1= read, 0= write\n"
    "    --DmaEndCs_1=0      1= auto deassert chip select, 0= not\n"
    "    --IrqRxHalf_1=0     1= interrupt while RxHalf_1=1\n"
    "    --IrqTxEmpty_1=0    1= interrupt when TxEmpty_1=1\n"
    "    --DmaEnable_1=0     1= enable DMA operation\n"
    "    --RunActive_1=0     1= transfer active, 0= CS lines idle\n"
    "    --CsPolarity_1=0    chip select polarity, 1= active high\n"
    "    --ClearRxTxFifo_2=0 clear FIFO Rx={2,3}, Tx={1,3}, one-shot action\n"
    "    --ClockPolarity_1=0 SCLK polarity CPOL, 1= idle high\n"
    "    --ClockPhase_1=0    SCLK phase    CPHA, 1= ???\n"
    "    --ChipSelectN_2=0   chip select line number {0,1,2}\n"
    "  modify other bit fields:\n"
    "    --ClockDiv_16=N     SCLK frequency = sys_clock / N\n"
    "    --DmaDataLen_16=0   DMA number of bytes to transfer\n"
    "    --LossiHoldDly_4=N  output hold delay = (N+1) * sys_clock period\n"
    "  modify DmaReq bit fields:\n"
    "    --DmaRxPanicLev_8=N DMA read panic when Rx level > N\n"
    "    --DmaRxReqLev_8=N   DMA read DREQ  when Rx level > N\n"
    "    --DmaTxPanicLev_8=N DMA write panic when Tx level <= N\n"
    "    --DmaTxReqLev_8=N   DMA write DREQ  when Tx level <= N\n"
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
spi0_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_spi0::y_spi0(
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
y_spi0::doit()
{

    try {
	spi0_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	if ( Opx.verbose ) {
	    cout << "SPI0 Master:" << endl;
	}

    // SPI object

	rgSpi0		Spx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    // Note cout does not know how to show (volatile uint32_t*).

	    cout << "    " << "Spi0.GpioBase=       "
		 <<hex << (uint32_t*) Spx.get_base_addr() <<endl;

	    cout << "    " << "Spi0.CntlStat=       "
		 <<hex << (uint32_t*) Spx.addr_CntlStat() <<endl;

	    cout << "    " << "Spi0.diff_CntlStat=  "
		 <<hex
		 << (Spx.addr_CntlStat() - Spx.get_base_addr())*4 <<endl;

	    cout <<dec <<endl;
	}

	{
	    bool		md = 0;		// modify flag

	    Opx.trace_msg( "Grab regs" );
	    Spx.grab_regs();

	// Registers

	    if (    Opx.CntlStat.Given ) {
		Spx.put_CntlStat( Opx.CntlStat.Val );   md = 1;
	    }

	    if (    Opx.ClkDiv.Given ) {
		Spx.put_ClkDiv(   Opx.ClkDiv.Val   );   md = 1;
	    }

	    if (    Opx.DmaLen.Given ) {
		Spx.put_DmaLen(   Opx.DmaLen.Val   );   md = 1;
	    }

	    if (    Opx.Lossi.Given ) {
		Spx.put_Lossi(    Opx.Lossi.Val    );   md = 1;
	    }

	    if (    Opx.DmaReq.Given ) {
		Spx.put_DmaReq(   Opx.DmaReq.Val   );   md = 1;
	    }


	// CntlStat fields

	    if (    Opx.LossiWord_1.Given ) {
		Spx.put_LossiWord_1(     Opx.LossiWord_1.Val     );  md = 1;
	    }

	    if (    Opx.LossiDmaEn_1.Given ) {
		Spx.put_LossiDmaEn_1(    Opx.LossiDmaEn_1.Val    );  md = 1;
	    }

	    if (    Opx.CsPolarity_3.Given ) {
		Spx.put_CsPolarity_3(    Opx.CsPolarity_3.Val    );  md = 1;
	    }

	    // Read-only status

	    if (    Opx.LossiEnable_1.Given ) {
		Spx.put_LossiEnable_1(   Opx.LossiEnable_1.Val   );  md = 1;
	    }

	    if (    Opx.ReadEnable_1.Given ) {
		Spx.put_ReadEnable_1(    Opx.ReadEnable_1.Val    );  md = 1;
	    }

	    if (    Opx.DmaEndCs_1.Given ) {
		Spx.put_DmaEndCs_1(      Opx.DmaEndCs_1.Val      );  md = 1;
	    }

	    if (    Opx.IrqRxHalf_1.Given ) {
		Spx.put_IrqRxHalf_1(     Opx.IrqRxHalf_1.Val     );  md = 1;
	    }

	    if (    Opx.IrqTxEmpty_1.Given ) {
		Spx.put_IrqTxEmpty_1(    Opx.IrqTxEmpty_1.Val    );  md = 1;
	    }

	    if (    Opx.DmaEnable_1.Given ) {
		Spx.put_DmaEnable_1(     Opx.DmaEnable_1.Val     );  md = 1;
	    }

	    if (    Opx.RunActive_1.Given ) {
		Spx.put_RunActive_1(     Opx.RunActive_1.Val     );  md = 1;
	    }

	    if (    Opx.CsPolarity_1.Given ) {
		Spx.put_CsPolarity_1(    Opx.CsPolarity_1.Val    );  md = 1;
	    }

	    if (    Opx.ClearRxTxFifo_2.Given ) {
		Spx.put_ClearRxTxFifo_2( Opx.ClearRxTxFifo_2.Val );  md = 1;
	    }

	    if (    Opx.ClockPolarity_1.Given ) {
		Spx.put_ClockPolarity_1( Opx.ClockPolarity_1.Val );  md = 1;
	    }

	    if (    Opx.ClockPhase_1.Given ) {
		Spx.put_ClockPhase_1(    Opx.ClockPhase_1.Val    );  md = 1;
	    }

	    if (    Opx.ChipSelectN_2.Given ) {
		Spx.put_ChipSelectN_2(   Opx.ChipSelectN_2.Val   );  md = 1;
	    }

	// ClkDiv fields
	    if (    Opx.ClockDiv_16.Given ) {
		Spx.put_ClockDiv_16(     Opx.ClockDiv_16.Val     );  md = 1;
	    }

	// DmaLen fields
	    if (    Opx.DmaDataLen_16.Given ) {
		Spx.put_DmaDataLen_16(   Opx.DmaDataLen_16.Val   );  md = 1;
	    }

	// Lossi  fields
	    if (    Opx.LossiHoldDly_4.Given ) {
		Spx.put_LossiHoldDly_4(  Opx.LossiHoldDly_4.Val  );  md = 1;
	    }

	// DmaReq fields
	    if (    Opx.DmaRxPanicLev_8.Given ) {
		Spx.put_DmaRxPanicLev_8( Opx.DmaRxPanicLev_8.Val );  md = 1;
	    }

	    if (    Opx.DmaRxReqLev_8.Given ) {
		Spx.put_DmaRxReqLev_8(   Opx.DmaRxReqLev_8.Val   );  md = 1;
	    }

	    if (    Opx.DmaTxPanicLev_8.Given ) {
		Spx.put_DmaTxPanicLev_8( Opx.DmaTxPanicLev_8.Val );  md = 1;
	    }

	    if (    Opx.DmaTxReqLev_8.Given ) {
		Spx.put_DmaTxReqLev_8(   Opx.DmaTxReqLev_8.Val   );  md = 1;
	    }


	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify regs" );
		Spx.push_regs();
	    }

	    if ( Opx.tx ) {
		char*		cp;
		uint32_t	vv;

		Opx.trace_msg( "Write Tx Fifo" );

		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << "   write_Fifo:  0x" <<hex <<setw(8) << vv << endl;
		    Spx.write_Fifo( vv );
		}
		cout << dec;
	    }

	    if ( md || Opx.tx  ) {		// registers changed
		Opx.trace_msg( "Grab regs" );
		Spx.grab_regs();
	    }
	    // Add Opx.rx.Val if read_Fifo() is moved before this.

	    cout.fill('0');
	    cout <<hex
		<< "   CntlStat= 0x" <<setw(8) << Spx.read_CntlStat()  <<endl
		<< "   ClkDiv=   0x" <<setw(8) << Spx.read_ClkDiv()    <<endl
		<< "   DmaLen=   0x" <<setw(8) << Spx.read_DmaLen()    <<endl
		<< "   Lossi=    0x" <<setw(8) << Spx.read_Lossi()     <<endl
		<< "   DmaReq=   0x" <<setw(8) << Spx.read_DmaReq()    <<endl
		;

	    cout.fill(' ');
	    cout <<dec
		<< " CntlStat" <<endl
		<< "   LossiWord_1     = " << Spx.get_LossiWord_1()     <<endl
		<< "   LossiDmaEn_1    = " << Spx.get_LossiDmaEn_1()    <<endl
		<< "   CsPolarity_3    = " << Spx.get_CsPolarity_3()    <<endl

		<< "   RxFullStop_1    = " << Spx.get_RxFullStop_1()    <<endl
		<< "   RxHalf_1        = " << Spx.get_RxHalf_1()        <<endl
		<< "   TxHasSpace_1    = " << Spx.get_TxHasSpace_1()    <<endl
		<< "   RxHasData_1     = " << Spx.get_RxHasData_1()     <<endl
		<< "   TxEmpty_1       = " << Spx.get_TxEmpty_1()       <<endl
		<< "   LossiEnable_1   = " << Spx.get_LossiEnable_1()   <<endl
		<< "   ReadEnable_1    = " << Spx.get_ReadEnable_1()    <<endl
		<< "   DmaEndCs_1      = " << Spx.get_DmaEndCs_1()      <<endl
		<< "   IrqRxHalf_1     = " << Spx.get_IrqRxHalf_1()     <<endl
		<< "   IrqTxEmpty_1    = " << Spx.get_IrqTxEmpty_1()    <<endl
		<< "   DmaEnable_1     = " << Spx.get_DmaEnable_1()     <<endl

		<< "   RunActive_1     = " << Spx.get_RunActive_1()     <<endl
		<< "   CsPolarity_1    = " << Spx.get_CsPolarity_1()    <<endl
		<< "   ClearRxTxFifo_2 = " << Spx.get_ClearRxTxFifo_2() <<endl
		<< "   ClockPolarity_1 = " << Spx.get_ClockPolarity_1() <<endl
		<< "   ClockPhase_1    = " << Spx.get_ClockPhase_1()    <<endl
		<< "   ChipSelectN_2   = " << Spx.get_ChipSelectN_2()   <<endl
		<< " ClkDiv" <<endl
		<< "   ClockDiv_16     = " << Spx.get_ClockDiv_16()     <<endl
		<< " DmaLen" <<endl
		<< "   DmaDataLen_16   = " << Spx.get_DmaDataLen_16()   <<endl
		<< " Lossi"  <<endl
		<< "   LossiHoldDly_4  = " << Spx.get_LossiHoldDly_4()  <<endl
		<< " DmaReq" <<endl
		<< "   DmaRxPanicLev_8 = " << Spx.get_DmaRxPanicLev_8() <<endl
		<< "   DmaRxReqLev_8   = " << Spx.get_DmaRxReqLev_8()   <<endl
		<< "   DmaTxPanicLev_8 = " << Spx.get_DmaTxPanicLev_8() <<endl
		<< "   DmaTxReqLev_8   = " << Spx.get_DmaTxReqLev_8()   <<endl
		;

	    if ( Opx.rx.Val ) {
		Opx.trace_msg( "Read Rx Fifo" );
		cout.fill('0');
		cout <<hex;
		for ( uint32_t jj = 1;  jj <= Opx.rx.Val;  jj++ ) {
		    cout << "   read_Fifo:  0x" <<setw(8) << Spx.read_Fifo()
			 <<endl;
		}
		cout <<dec;
	    }

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

