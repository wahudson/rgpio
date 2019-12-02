// 2019-11-28  William A. Hudson

// rGPIO PWM - Pulse Width Modulator class
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.138-147  ch 9. Pulse Width Modulator
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgPwm.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgPwm	px  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgPwm::rgPwm(
    rgAddrMap		*xx
)
{
    GpioBase     = xx->get_mem_block( FeatureAddr );

        Cntl.init_addr( GpioBase +     Cntl_offset );
        Stat.init_addr( GpioBase +     Stat_offset );
     DmaConf.init_addr( GpioBase +  DmaConf_offset );
        Fifo.init_addr( GpioBase +     Fifo_offset );
    Ch1Range.init_addr( GpioBase + Ch1Range_offset );
     Ch1Data.init_addr( GpioBase +  Ch1Data_offset );
    Ch2Range.init_addr( GpioBase + Ch2Range_offset );
     Ch2Data.init_addr( GpioBase +  Ch2Data_offset );
}


//--------------------------------------------------------------------------
// Object state operations
//--------------------------------------------------------------------------

/*
* Initialize object control/status registers to the power-on reset state.
*    Intended for use with the put_*() field accessor functions, and can be
*    called multiple times.
*    Virtually all fields are zero, or are the inactive state.
*    Hardware registers are unchanged.
# #!! Need to validate values against hardware reset.
*/
void
rgPwm::init_put_reset()
{
    Cntl.put(     0x00000000 );
    Stat.put(     0x00000000 );
    DmaConf.put(  0x00000000 );
    Fifo.put(     0x00000000 );		// include Fifo object state
    Ch1Range.put( 0x00000020 );
    Ch1Data.put(  0x00000000 );
    Ch2Range.put( 0x00000020 );
    Ch2Data.put(  0x00000000 );

    Stat.put_FifoEmpty_1( 1 );

    DmaConf.put_DmaPanicLev_8( 7 );
    DmaConf.put_DmaReqLev_8(   7 );
}


/*
* Read hardware registers into the object (not Fifo).
*/
void
rgPwm::grab_regs()
{
    Cntl.grab();
    Stat.grab();
    DmaConf.grab();
    // NOT Fifo
    Ch1Range.grab();
    Ch1Data.grab();
    Ch2Range.grab();
    Ch2Data.grab();
}


/*
* Write hardware registers from the object (not Fifo).
*/
void
rgPwm::push_regs()
{
    Cntl.push();
    Stat.push();
    DmaConf.push();
    // NOT Fifo
    Ch1Range.push();
    Ch1Data.push();
    Ch2Range.push();
    Ch2Data.push();
}


//--------------------------------------------------------------------------
// Debug
//--------------------------------------------------------------------------

/*
* Show debug output.
* call:
*    self.show_debug( cout )
*/
void
rgPwm::show_debug( std::ostream& sout )
{
    sout.fill('0');
    sout <<hex

	 << "Pwm.Cntl=     0x"  <<setw(8) <<     Cntl.get() <<endl
	 << "Pwm.Stat=     0x"  <<setw(8) <<     Stat.get() <<endl
	 << "Pwm.DmaConf=  0x"  <<setw(8) <<  DmaConf.get() <<endl
	 << "Pwm.Ch1Range= 0x"  <<setw(8) << Ch1Range.get() <<endl
	 << "Pwm.Ch1Data=  0x"  <<setw(8) <<  Ch1Data.get() <<endl
	 << "Pwm.Ch2Range= 0x"  <<setw(8) << Ch2Range.get() <<endl
	 << "Pwm.Ch2Data=  0x"  <<setw(8) <<  Ch2Data.get() <<endl;
    sout <<dec;
    sout.fill(' ');
}

