// 2020-06-17  William A. Hudson
//
// rgpio iic -- Inter-IC (I2C) Master - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept
#include <string.h>	// strcmp()

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgIic.h"

#include "y_iic.h"

const int		IicMax = 7;	// maximum iic unit number

//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class iic_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		iic_ch[IicMax+1] = {0,0,0,0,0,0,0,0};  // unit requested

					// registers
    yOpVal		Cntl;
    yOpVal		Stat;
    yOpVal		DatLen;
    yOpVal		Addr;
    yOpVal		ClkDiv;
    yOpVal		Delay;
    yOpVal		ClkStr;

    yOpVal		rx;		// Receive N entries
    bool		tx;		// Transmit arguments

					// Cntl fields
    yOpVal		IicEnable_1;
    yOpVal		IrqRxHalf_1;
    yOpVal		IrqTxHalf_1;
    yOpVal		IrqDone_1;
    yOpVal		StartTrans_1;
    yOpVal		ClearFifo_2;
    yOpVal		ReadPacket_1;
					// Stat fields
    yOpVal		ClkTimeout_1;
    yOpVal		AckErr_1;
    yOpVal		TransDone_1;
					// Other register fields
    yOpVal		DataLen_16;
    yOpVal		SlaveAddr_7;
    yOpVal		ClkDiv_16;
    yOpVal		Fall2Out_16;
    yOpVal		Rise2In_16;
    yOpVal		TimeOut_16;

    bool		reset;
    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    iic_yOptLong( yOption  *opx );	// constructor

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
*    iic_yOptLong	Opx  ( opx );
*/
iic_yOptLong::iic_yOptLong( yOption  *opx )
    : yOption( opx )
{
    tx          = 0;

    reset       = 0;
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
iic_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--Cntl="      )) { Cntl.set(     this->val() ); }
	else if ( is( "--Stat="      )) { Stat.set(     this->val() ); }
	else if ( is( "--DatLen="    )) { DatLen.set(   this->val() ); }
	else if ( is( "--Addr="      )) { Addr.set(     this->val() ); }
	else if ( is( "--ClkDiv="    )) { ClkDiv.set(   this->val() ); }
	else if ( is( "--Delay="     )) { Delay.set(    this->val() ); }
	else if ( is( "--ClkStr="    )) { ClkStr.set(   this->val() ); }

	else if ( is( "--rx="        )) { rx.set(       this->val() ); }
	else if ( is( "--tx"         )) { tx         = 1; }
	else if ( is( "-0"           )) { iic_ch[0]  = 1; }
	else if ( is( "-1"           )) { iic_ch[1]  = 1; }
	else if ( is( "-2"           )) { iic_ch[2]  = 1; }
	else if ( is( "-3"           )) { iic_ch[3]  = 1; }
	else if ( is( "-4"           )) { iic_ch[4]  = 1; }
	else if ( is( "-5"           )) { iic_ch[5]  = 1; }
	else if ( is( "-6"           )) { iic_ch[6]  = 1; }
	else if ( is( "-7"           )) { iic_ch[7]  = 1; }

	else if ( is( "--IicEnable_1="  )) { IicEnable_1.set(  this->val() ); }
	else if ( is( "--IrqRxHalf_1="  )) { IrqRxHalf_1.set(  this->val() ); }
	else if ( is( "--IrqTxHalf_1="  )) { IrqTxHalf_1.set(  this->val() ); }
	else if ( is( "--IrqDone_1="    )) { IrqDone_1.set(    this->val() ); }
	else if ( is( "--StartTrans_1=" )) { StartTrans_1.set( this->val() ); }
	else if ( is( "--ClearFifo_2="  )) { ClearFifo_2.set(  this->val() ); }
	else if ( is( "--ReadPacket_1=" )) { ReadPacket_1.set( this->val() ); }

	else if ( is( "--ClkTimeout_1=" )) { ClkTimeout_1.set( this->val() ); }
	else if ( is( "--AckErr_1="     )) { AckErr_1.set(     this->val() ); }
	else if ( is( "--TransDone_1="  )) { TransDone_1.set(  this->val() ); }

	else if ( is( "--DataLen_16="  ))  { DataLen_16.set(   this->val() ); }
	else if ( is( "--SlaveAddr_7="  )) { SlaveAddr_7.set(  this->val() ); }
	else if ( is( "--ClkDiv_16="    )) { ClkDiv_16.set(    this->val() ); }
	else if ( is( "--Fall2Out_16="  )) { Fall2Out_16.set(  this->val() ); }
	else if ( is( "--Rise2In_16="   )) { Rise2In_16.set(   this->val() ); }
	else if ( is( "--TimeOut_16="   )) { TimeOut_16.set(   this->val() ); }

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

    if ( !( iic_ch[0] || iic_ch[1] || iic_ch[2] || iic_ch[3] ||
	    iic_ch[4] || iic_ch[5] || iic_ch[6] || iic_ch[7]  ) ) {
	iic_ch[0] = 1;		// default
    }

    if ( (rgRpiRev::find_SocEnum() <= rgRpiRev::soc_BCM2837) &&
	(iic_ch[3] || iic_ch[4] || iic_ch[5] || iic_ch[6] || iic_ch[7])
    ) {
	Error::msg( "require IIC number {-0, -1, -2} for "   )
		<< rgRpiRev::cstr_SocEnum() <<endl;
    }

    check_f1( "IicEnable_1",    IicEnable_1.Val    );
    check_f1( "IrqRxHalf_1",    IrqRxHalf_1.Val    );
    check_f1( "IrqTxHalf_1",    IrqTxHalf_1.Val    );
    check_f1( "IrqDone_1",      IrqDone_1.Val      );
    check_f1( "StartTrans_1",   StartTrans_1.Val   );

    if (                       ClearFifo_2.Val > 0x3   ) {
	Error::msg( "require --ClearFifo_2={0..3}:  "   ) <<
			       ClearFifo_2.Val <<endl;
    }
    check_f1( "ReadPacket_1",   ReadPacket_1.Val   );

    check_f1( "ClkTimeout_1",   ClkTimeout_1.Val   );
    check_f1( "AckErr_1",       AckErr_1.Val       );
    check_f1( "TransDone_1",    TransDone_1.Val    );

    if (                       DataLen_16.Val > 0xffff ) {
	Error::msg( "require --DataLen_16={0..65535}:  " ) <<
			       DataLen_16.Val <<endl;
    }

    if (                       SlaveAddr_7.Val > 0x7f   ) {
	Error::msg( "require --SlaveAddr_7={0..127}:  "   ) <<
			       SlaveAddr_7.Val <<endl;
    }

    if (                       ClkDiv_16.Val   > 0xffff ) {
	Error::msg( "require --ClkDiv_16={0..65535}:  "   ) <<
			       ClkDiv_16.Val <<endl;
    }

    if (                       Fall2Out_16.Val > 0xffff ) {
	Error::msg( "require --Fall2Out_16={0..65535}:  " ) <<
			       Fall2Out_16.Val <<endl;
    }

    if (                       Rise2In_16.Val > 0xffff ) {
	Error::msg( "require --Rise2In_16={0..65535}:  " ) <<
			       Rise2In_16.Val <<endl;
    }

    if (                       TimeOut_16.Val  > 0xffff ) {
	Error::msg( "require --TimeOut_16={0..65535}:  "  ) <<
			       TimeOut_16.Val <<endl;
    }

    if ( (! tx) && (get_argc() > 0) ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }

    if ( (tx) && (get_argc() == 0) ) {
	Error::msg( "--tx requires arg values" ) << endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
iic_yOptLong::check_f1(
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
iic_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--Stat        = 0x" <<setw(8) << Stat.Val        << endl;

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
iic_yOptLong::print_usage()
{
    cout <<
    "    I2C Master\n"
    "usage:  " << ProgName << " iic [options..] [--tx V..]\n"
    "    -0 -1 -2            IIC number, default -0\n"
    "    -3 .. -7            IIC number, extended for RPi4\n"
    "  modify full register:  (V= 32-bit value)\n"
    "    --Cntl=V            Control\n"
    "    --Stat=V            Status (read-clear)\n"
    "    --DatLen=V          Data Length\n"
    "    --Addr=V            Slave Address\n"
    "    --ClkDiv=V          Clock Divider\n"
    "    --Delay=V           Data Delay\n"
    "    --ClkStr=V          Clock Stretch Timeout\n"
    "  data transfer:\n"
    "    --rx=N              read N entries from 16-entry Fifo\n"
    "    --tx                write arg values to Fifo\n"
    "  Cntl:\n"
    "    --IicEnable_1=0     Enable the interface\n"
    "    --IrqRxHalf_1=0     1= interrupt while RxHalf_1=1\n"
    "    --IrqTxHalf_1=0     1= interrupt while TxHalf_1=1\n"
    "    --IrqDone_1=0       1= interrupt while TransDone_1=1\n"
    "    --StartTrans_1=0    1= start new transfer\n"
    "    --ClearFifo_2=0     clear FIFO {1,2,3}, redundant bits\n"
    "    --ReadPacket_1=0    1= Read transfer, 0= write transfer\n"
    "  Stat:  (write 1 to clear)\n"
    "    --ClkTimeout_1=0    1= slave clock stretch timeout\n"
    "    --AckErr_1=0        1= slave not acknowledged its address\n"
    "    --TransDone_1=0     1= transfer complete\n"
    "  DatLen:\n"
    "    --DataLen_16=N      data transfer length in bytes\n"
    "  Addr:\n"
    "    --SlaveAddr_7=N     slave address\n"
    "  ClkDiv:\n"
    "    --ClkDiv_16=N       clock divider\n"
    "  Delay:\n"
    "    --Fall2Out_16=N     clock to output delay in core clock cycles\n"
    "    --Rise2In_16=N      clock to input  delay in core clock cycles\n"
    "  ClkStr:\n"
    "    --TimeOut_16=N      clock stretch timeout in SCL cycles\n"
    "  options:\n"
    "    --reset             init reset values before modifications\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
//  "    --debug             debug output\n"
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
iic_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_iic::y_iic(
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
y_iic::doit()
{

    try {
	iic_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // IIC channel objects
	rgIic*			Icx[IicMax+1] = {0,0,0,0,0,0,0,0};
				// pointers to IIC objects, NULL if not used.

	for ( int ii=0;  ii<=IicMax;  ii++ )
	{
	    if ( Opx.iic_ch[ii] ) {
		Icx[ii] = new  rgIic  ( ii, AddrMap );	// constructor
	    }
	    // construct all first, in case of exception
	}

    // Process each device
	for ( int ii=0;  ii<=IicMax;  ii++ )
	{
	    rgIic		*icx;
	    bool		md = 0;		// modify flag

	    icx = Icx[ii];

	    if ( icx == NULL ) {
		continue;
	    }

	    rgIic_Stat	Sx = icx->Stat;		// saved copy
	    string	ns = "   " + std::to_string( icx->get_iic_num());

	// Heading

	    cout << "Iic" << icx->get_iic_num() << ":" <<endl;

	// Reset

	    if ( Opx.reset ) {
		Opx.trace_msg( "Init reset" );
		icx->init_put_reset();
		md = 1;
	    }
	    else {
		Opx.trace_msg( "Grab regs" );
		icx->grab_regs();
		Sx = icx->Stat;			// copy of initial grab
		icx->Stat.put( 0 );		// Stat is read-clear
	    }

#define APPLY( X, Y ) if ( Opx.X.Given ) { icx->Y( Opx.X.Val );  md = 1; }

	// Registers

	    APPLY( Cntl,      Cntl.put      )
	    APPLY( Stat,      Stat.put      )
	    APPLY( DatLen,    DatLen.put    )
	    APPLY( Addr,      Addr.put      )
	    APPLY( ClkDiv,    ClkDiv.put    )
	    APPLY( Delay,     Delay.put     )
	    APPLY( ClkStr,    ClkStr.put    )

	// Fields

	    APPLY( IicEnable_1,    Cntl.put_IicEnable_1    )
	    APPLY( IrqRxHalf_1,    Cntl.put_IrqRxHalf_1    )
	    APPLY( IrqTxHalf_1,    Cntl.put_IrqTxHalf_1    )
	    APPLY( IrqDone_1,      Cntl.put_IrqDone_1      )
	    APPLY( StartTrans_1,   Cntl.put_StartTrans_1   )
	    APPLY( ClearFifo_2,    Cntl.put_ClearFifo_2    )
	    APPLY( ReadPacket_1,   Cntl.put_ReadPacket_1   )

	    APPLY( ClkTimeout_1,   Stat.put_ClkTimeout_1   )
	    APPLY( AckErr_1,       Stat.put_AckErr_1       )
	    APPLY( TransDone_1,    Stat.put_TransDone_1    )

	    APPLY( DataLen_16,     DatLen.put_DataLen_16   )

	    APPLY( SlaveAddr_7,      Addr.put_SlaveAddr_7  )

	    APPLY( ClkDiv_16,      ClkDiv.put_ClkDiv_16    )

	    APPLY( Fall2Out_16,     Delay.put_Fall2Out_16  )
	    APPLY( Rise2In_16,      Delay.put_Rise2In_16   )

	    APPLY( TimeOut_16,     ClkStr.put_TimeOut_16   )

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify regs" );
		icx->Cntl.push();
		icx->Stat.push();
		icx->DatLen.push();
		icx->Addr.push();
		icx->ClkDiv.push();
		icx->Delay.push();
		icx->ClkStr.push();
	    }
	    else {
		icx->Stat.put( Sx.get() );	// restore original grab
	    }

	// Tx FIFO

	    if ( Opx.tx ) {
		char*           cp;
		uint32_t        vv;

		Opx.trace_msg( "Write Tx Fifo" );
		while ( (cp = Opx.next_arg()) )
		{
		    vv = strtoul( cp, NULL, 0 );
		    cout.fill('0');
		    cout << ns << ".write_Fifo:  0x" <<hex <<setw(8) << vv << endl;
		    icx->Fifo.write( vv );
		}
		cout << dec;
		md = 1;
	    }
	    //#!! Note --tx works for only one IIC channel.

	// Rx FIFO

	    if ( Opx.rx.Val ) {
		Opx.trace_msg( "Read Rx Fifo" );
		cout.fill('0');
		cout <<hex;
		for ( uint32_t jj = 1;  jj <= Opx.rx.Val;  jj++ ) {
		    cout << ns << ".read_Fifo:  0x" <<setw(8)
			 << icx->Fifo.read() <<endl;
		}
		cout <<dec;
		md = 1;
	    }

	// Output

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Grab regs" );
		icx->grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
		<< ns << ".Cntl    = 0x" <<setw(8) << icx->Cntl.get()   <<endl
		<< ns << ".Stat    = 0x" <<setw(8) << icx->Stat.get()   <<endl
		<< ns << ".DatLen  = 0x" <<setw(8) << icx->DatLen.get() <<endl
		<< ns << ".Addr    = 0x" <<setw(8) << icx->Addr.get()   <<endl
		<< ns << ".ClkDiv  = 0x" <<setw(8) << icx->ClkDiv.get() <<endl
		<< ns << ".Delay   = 0x" <<setw(8) << icx->Delay.get()  <<endl
		<< ns << ".ClkStr  = 0x" <<setw(8) << icx->ClkStr.get() <<endl
		;

#define OUTD( X, Y )  ns << X << icx->Y <<endl

	    cout.fill(' ');
	    cout <<dec
		<< " Cntl" <<endl
		<< OUTD( ".IicEnable_1   = ", Cntl.get_IicEnable_1()   )
		<< OUTD( ".IrqRxHalf_1   = ", Cntl.get_IrqRxHalf_1()   )
		<< OUTD( ".IrqTxHalf_1   = ", Cntl.get_IrqTxHalf_1()   )
		<< OUTD( ".IrqDone_1     = ", Cntl.get_IrqDone_1()     )
		<< OUTD( ".StartTrans_1  = ", Cntl.get_StartTrans_1()  )
		<< OUTD( ".ClearFifo_2   = ", Cntl.get_ClearFifo_2()   )
		<< OUTD( ".ReadPacket_1  = ", Cntl.get_ReadPacket_1()  )
		<< " Stat" <<endl
		<< OUTD( ".ClkTimeout_1  = ", Stat.get_ClkTimeout_1()  )
		<< OUTD( ".AckErr_1      = ", Stat.get_AckErr_1()      )
		<< OUTD( ".RxFull_1      = ", Stat.get_RxFull_1()      )
		<< OUTD( ".TxEmpty_1     = ", Stat.get_TxEmpty_1()     )
		<< OUTD( ".RxHasData_1   = ", Stat.get_RxHasData_1()   )
		<< OUTD( ".TxHasSpace_1  = ", Stat.get_TxHasSpace_1()  )
		<< OUTD( ".RxHalf_1      = ", Stat.get_RxHalf_1()      )
		<< OUTD( ".TxHalf_1      = ", Stat.get_TxHalf_1()      )
		<< OUTD( ".TransDone_1   = ", Stat.get_TransDone_1()   )
		<< OUTD( ".TransActive_1 = ", Stat.get_TransActive_1() )
		<< " DatLen" <<endl
		<< OUTD( ".DataLen_16    = ", DatLen.get_DataLen_16()  )
		<< " Addr" <<endl
		<< OUTD( ".SlaveAddr_7   = ", Addr.get_SlaveAddr_7()   )
		<< " ClkDiv" <<endl
		<< OUTD( ".ClkDiv_16     = ", ClkDiv.get_ClkDiv_16()   )
		<< " Delay" <<endl
		<< OUTD( ".Fall2Out_16   = ", Delay.get_Fall2Out_16()  )
		<< OUTD( ".Rise2In_16    = ", Delay.get_Rise2In_16()   )
		<< " ClkStr" <<endl
		<< OUTD( ".TimeOut_16    = ", ClkStr.get_TimeOut_16()  )
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
}

