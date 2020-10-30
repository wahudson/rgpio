// 2020-04-19  William A. Hudson

#ifndef rgIoPins_P
#define rgIoPins_P

//--------------------------------------------------------------------------
// rGPIO  Io Pin Registers.
//--------------------------------------------------------------------------

class rgReg {		// Base class for any write-style.

  protected:
    volatile uint32_t	*Addr;		// register address

  public:
		// constructor
			rgReg()				{ Addr = 0; }

    inline
    volatile uint32_t*	addr()				{ return Addr; }

    inline uint32_t	read()				{ return *Addr; }

    inline void		write( uint32_t rval )		{ *Addr = rval; }

		// Initialization:  (private, used by feature constructor)
  public:
    inline void		init_addr( volatile uint32_t* aptr)  { Addr = aptr; }
};


class rgReg_set : public rgReg {	// set-only
  public:
    inline void		set( uint32_t rval )		{ *Addr = rval; }
};


class rgReg_clr : public rgReg {	// clear-only
  public:
    inline void		clr( uint32_t rval )		{ *Addr = rval; }
};


class rgReg_rw  : public rgReg {	// normal read-write
  public:
    inline void		set(    uint32_t mask )   { write( read() |  mask ); }
    inline void		clr(    uint32_t mask )   { write( read() & ~mask ); }
    void		modify( uint32_t mask,  uint32_t value );
};


//#!!  rgReg_3rw  generic virtual register
class rgReg_PinLevel : public rgReg {	// virtual register
  private:
//  volatile uint32_t	*Addr;		// inherited
    volatile uint32_t	*AddrSet;	// register address
    volatile uint32_t	*AddrClr;	// register address

  public:
		// constructor
			rgReg_PinLevel()	{ AddrSet = 0;  AddrClr= 0; }

    inline
    volatile uint32_t*	addr_set()			{ return AddrSet; }
    volatile uint32_t*	addr_clr()			{ return AddrClr; }

    inline void		set( uint32_t rval )		{ *AddrSet = rval; }
    inline void		clr( uint32_t rval )		{ *AddrClr = rval; }

    inline void		modify( uint32_t mask,  uint32_t rval ) {
			    *AddrSet = (mask &  rval);
			    *AddrClr = (mask & ~rval);
			}

		// Initialization:  (private, used by feature constructor)
  public:
    inline void		init_addr_set( volatile uint32_t* aptr)
			    { AddrSet = aptr; }
    inline void		init_addr_clr( volatile uint32_t* aptr)
			    { AddrClr = aptr; }
};


class rgIoPins {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

  public:
				// Virtual Registers
    rgReg_PinLevel	PinLevel_w0;
    rgReg_PinLevel	PinLevel_w1;

				// Real Registers
    rgReg_rw		Fsel0;
    rgReg_rw		Fsel1;
    rgReg_rw		Fsel2;
    rgReg_rw		Fsel3;
    rgReg_rw		Fsel4;
    rgReg_rw		Fsel5;

    rgReg_set		PinSet_w0;		// set-only, no read
    rgReg_set		PinSet_w1;

    rgReg_clr		PinClr_w0;		// clear-only, no read
    rgReg_clr		PinClr_w1;

    rgReg_rw		PinRead_w0;		// read-only
    rgReg_rw		PinRead_w1;

    rgReg_clr		EventStatus_w0;		// read/clear
    rgReg_clr		EventStatus_w1;

    rgReg_rw		DetectRise_w0;
    rgReg_rw		DetectRise_w1;

    rgReg_rw		DetectFall_w0;
    rgReg_rw		DetectFall_w1;

    rgReg_rw		DetectHigh_w0;
    rgReg_rw		DetectHigh_w1;

    rgReg_rw		DetectLow_w0;
    rgReg_rw		DetectLow_w1;

    rgReg_rw		DetectAsyncRise_w0;
    rgReg_rw		DetectAsyncRise_w1;

    rgReg_rw		DetectAsyncFall_w0;
    rgReg_rw		DetectAsyncFall_w1;

    rgReg_rw		PudProgMode;
    rgReg_rw		PudProgClk_w0;
    rgReg_rw		PudProgClk_w1;

  private:
    static const uint32_t	FeatureAddr  = 0x7e200000;	// BCM doc

  public:
    rgIoPins(			// constructor
	rgAddrMap	*xx
    );

    inline
    volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
    inline uint32_t	get_bcm_address()	{ return  FeatureAddr; }
};

#endif

