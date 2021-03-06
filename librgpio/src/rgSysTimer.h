// 2020-04-25  William A. Hudson

#ifndef rgSysTimer_P
#define rgSysTimer_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO  System Timer class
//--------------------------------------------------------------------------

class rgSysTimer_Stat : public rgRegister {	//#!! read-clear
  public:

    inline
    uint32_t	get_Match3_1()           { return  get_field(  3, 0x1    ); }
    void	put_Match3_1( uint32_t v )       { put_field(  3, 0x1, v ); }

    uint32_t	get_Match2_1()           { return  get_field(  2, 0x1    ); }
    void	put_Match2_1( uint32_t v )       { put_field(  2, 0x1, v ); }

    uint32_t	get_Match1_1()           { return  get_field(  1, 0x1    ); }
    void	put_Match1_1( uint32_t v )       { put_field(  1, 0x1, v ); }

    uint32_t	get_Match0_1()           { return  get_field(  0, 0x1    ); }
    void	put_Match0_1( uint32_t v )       { put_field(  0, 0x1, v ); }
};

class rgSysTimer_TimeW0 : public rgRegister {
};

class rgSysTimer_TimeW1 : public rgRegister {
};

class rgSysTimer_TimeDw {	// Virtual 64-bit timer counter register
  public:
    rgRegister		W1B;		// second W1 (private)

  public:
    rgRegister		W1;		// 32-bit registers
    rgRegister		W0;

  public:
    void		grab();		// grab W1, W0, W1B
    uint64_t		get64();	// get coherent 64-bit value

					// read coherent 64-bit value
    uint64_t		grab64()	{ grab();  return  get64(); }
};


class rgSysTimer {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

  public:
				// Virtual register
    rgSysTimer_TimeDw	TimeDw;

				// Real Registers
    rgSysTimer_Stat	Stat;		// Status  (read-clear)
    rgSysTimer_TimeW0	TimeW0;		// Counter low  word (RO)
    rgSysTimer_TimeW1	TimeW1;		// Counter high word (RO)

    rgRegister		Cmp0;		// Compare values
    rgRegister		Cmp1;
    rgRegister		Cmp2;
    rgRegister		Cmp3;

  private:
    static const uint32_t	FeatureAddr  = 0x7e003000;	// BCM doc value

  public:
    rgSysTimer(			// constructor
	rgAddrMap	*xx
    );

    inline
    volatile uint32_t*	get_base_addr()  { return  GpioBase; }

		// Object state operations
//  void		init_put_reset();

    void		grab_regs();
//  void		push_regs();

		// Test/Debug accessors

    inline uint32_t	get_bcm_address() { return FeatureAddr; }
};

#endif

