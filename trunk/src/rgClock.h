// 2017-05-11  William A. Hudson

#ifndef rgClock_P
#define rgClock_P

//--------------------------------------------------------------------------
// rGPIO General Purpose Clock class
//--------------------------------------------------------------------------

class rgClock {
  private:
    volatile uint32_t	*GpioBase;	// IO base address

    uint32_t		ClkNum;		// Clock number {0,1,2}

    uint32_t		CtlReg;		// Control  register value
    uint32_t		DivReg;		// Divisior register value

    int			BusyCount;	// Number of read_Busy() calls in last
					//    wait_disable() or apply_regs().

  private:
    const uint32_t	FeatureAddr  = 0x7e101000;	// BCM doc

				// Register word offset in Page, Clock0
    const uint32_t	Ctl_offset   = 0x70 /4;
    const uint32_t	Div_offset   = 0x74 /4;

				// Bit field positions
    const uint32_t	Passwd_pos   = 24;	// ctl[31:24], div[31:24]
    const uint32_t	Mash_pos     = 9;	// ctl[10:9]
    const uint32_t	Flip_pos     = 8;	// ctl[8]
    const uint32_t	Busy_pos     = 7;	// ctl[7]
    const uint32_t	Kill_pos     = 5;	// ctl[5]
    const uint32_t	Enable_pos   = 4;	// ctl[4]
    const uint32_t	Source_pos   = 0;	// ctl[3:0]

    const uint32_t	DivI_pos     = 12;	// div[23:12]
    const uint32_t	DivF_pos     = 0;	// div[11:0]

  public:
    rgClock(			// constructor
	uint32_t	clknum,		// clock number {0,1,2}
	rgAddrMap	*xx = NULL
    );

    void		init_addr( rgAddrMap  *xx );

    inline volatile uint32_t*	get_base_addr()
    {
	return  GpioBase;
    }

    inline uint32_t	get_clock_num()
    {
	return  ClkNum;
    }

		// Direct low-level access
    volatile uint32_t*	addr_CtlReg();
    volatile uint32_t*	addr_DivReg();

    uint32_t		read_CtlReg();
    uint32_t		read_DivReg();

    void		raw_write_CtlReg( uint32_t  vv );
    void		raw_write_DivReg( uint32_t  vv );

    void		write_CtlReg( uint32_t  vv );
    void		write_DivReg( uint32_t  vv );

    uint32_t		read_Busy();
    uint32_t		read_Enable();

		// Direct control:  (modify register fields)
    void		enable_clock();
    void		disable_clock();

    bool		wait_disable();
    bool		wait_while_Busy(
			    uint32_t            wait_ns,
			    int                 num_times
			);

    void		kill_generator();

		// Object state operations
    void		grab_regs();
    void		raw_write_regs();
    void		apply_regs();

		// Object Field Accessors

    uint32_t		get_PasswdCtl();
    void		put_PasswdCtl( uint32_t  bit8 );

    uint32_t		get_Mash();
    void		put_Mash( uint32_t  bit2 );

    uint32_t		get_Flip();
    void		put_Flip( uint32_t  bit1 );

    uint32_t		get_Busy();
    void		put_Busy( uint32_t  bit1 );

    uint32_t		get_Kill();
    void		put_Kill( uint32_t  bit1 );

    uint32_t		get_Enable();
    void		put_Enable( uint32_t  bit1 );

    uint32_t		get_Source();
    void		put_Source( uint32_t  bit4 );

    uint32_t		get_PasswdDiv();
    void		put_PasswdDiv( uint32_t  bit8 );

    uint32_t		get_DivI();
    void		put_DivI( uint32_t  bit12 );

    uint32_t		get_DivF();
    void		put_DivF( uint32_t  bit12 );

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

    inline uint32_t		get_CtlReg() { return CtlReg; }
    inline uint32_t		get_DivReg() { return DivReg; }

    inline void			put_CtlReg( uint32_t reg32 )
    {
	CtlReg = reg32;
    }

    inline void			put_DivReg( uint32_t reg32 )
    {
	DivReg = reg32;
    }

    inline uint32_t		get_bcm_address()
    {
	return FeatureAddr;
    }

    // Number of read_Busy() calls in last wait_disable().
    inline int			get_BusyCount()
    {
	return BusyCount;
    }

};


#endif

