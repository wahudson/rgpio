// 2019-12-10  William A. Hudson

#ifndef rgClk_P
#define rgClk_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO  rgClk - Clock Manager enumerated class
//--------------------------------------------------------------------------
//    Note:  A function defined within the declaration of a class is inline
//    by default.

class rgClk_Cntl : public rgRegister {
  public:

    inline
    void	apply()			{ put_Passwd_8();  push(); }

    inline
    uint32_t	get_Passwd_8()        { return  get_field( 24, 0xff       ); }
    void	put_Passwd_8( uint32_t v )    { put_field( 24, 0xff, v    ); }
    void	put_Passwd_8()                { put_field( 24, 0xff, 0x5a ); }

    inline
    uint32_t	get_Mash_2()             { return  get_field(  9, 0x3    ); }
    void	put_Mash_2( uint32_t v )         { put_field(  9, 0x3, v ); }

    inline
    uint32_t	get_Flip_1()             { return  get_field(  8, 0x1    ); }
    void	put_Flip_1( uint32_t v )         { put_field(  8, 0x1, v ); }

    inline
    uint32_t	get_Busy_1()             { return  get_field(  7, 0x1    ); }
    void	put_Busy_1( uint32_t v )         { put_field(  7, 0x1, v ); }

    inline
    uint32_t	get_Kill_1()             { return  get_field(  5, 0x1    ); }
    void	put_Kill_1( uint32_t v )         { put_field(  5, 0x1, v ); }

    inline
    uint32_t	get_Enable_1()           { return  get_field(  4, 0x1    ); }
    void	put_Enable_1( uint32_t v )       { put_field(  4, 0x1, v ); }

    inline
    uint32_t	get_Source_4()           { return  get_field(  0, 0xf    ); }
    void	put_Source_4( uint32_t v )       { put_field(  0, 0xf, v ); }
};

class rgClk_Divr : public rgRegister {
  public:

    inline
    void	apply()			{ put_Passwd_8();  push(); }

    inline
    uint32_t	get_Passwd_8()        { return  get_field( 24, 0xff       ); }
    void	put_Passwd_8( uint32_t v )    { put_field( 24, 0xff, v    ); }
    void	put_Passwd_8()                { put_field( 24, 0xff, 0x5a ); }

    inline
    uint32_t	get_DivI_12()           { return  get_field( 12, 0xfff    ); }
    void	put_DivI_12( uint32_t v )       { put_field( 12, 0xfff, v ); }

    inline
    uint32_t	get_DivF_12()           { return  get_field(  0, 0xfff    ); }
    void	put_DivF_12( uint32_t v )       { put_field(  0, 0xfff, v ); }
};


/*
* Base Class.
*    Has registers, but no address initialization.
*    Intended for deriving a specific clock feature class.
*    Has no feature address members to avoid confusion.
*/
class rgClkMan {
  public:
				// Register data
    rgClk_Cntl		Cntl;		// Control
    rgClk_Divr		Divr;		// Divider

  private:
    long		WaitTime_ns;	// nanoseconds delay, 0= none
    int			WaitCount;	// limit wait cycles, 0= no wait
    int			BusyCount;	// number of cycles waited

  public:
    rgClkMan();			// constructor

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();
    void		apply_regs();		// with default password

		// Special functions

    bool		apply_nicely();		// apply all registers safely

    bool		wait_while_busy();

    inline int		wait_time_ns()		{ return WaitTime_ns; }
    inline void		wait_time_ns( int ns )	{ WaitTime_ns = ns; }

    inline int		wait_count_n()		{ return WaitCount; }
    inline void		wait_count_n( int n )	{ WaitCount = n; }

    inline int		get_busy_count()	{ return BusyCount; }

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );
};


class rgClk : public rgClkMan {
  public:
    // Clock manager identifiers.
    //   Clk0, Clk1, Clk2 are guaranteed to be integers {0, 1, 2} respectively.
    //   Values match class lookup tables, should remain constant.
    //   Extension should add new enums on end and adjust cm_MaxEnum.
    //   Intended for use as a packed array index.
    //
    enum rgClk_enum {
	cm_Clk0 = 0,
	cm_Clk1,
	cm_Clk2,
	cm_ClkPcm,
	cm_ClkPwm,
	cm_MaxEnum = cm_ClkPwm		// Maximum enum value
    };

  private:
    volatile uint32_t	*GpioBase;	// IO base address

    rgClk_enum		ClkEnum;	// Clock Number

    static
    const uint32_t	FeatureAddr  = 0x7e101000;	// BCM doc value

  public:
    rgClk(			// constructor
	rgClk_enum	clknum,		// clock identifier
	rgAddrMap	*xx
    );

    inline rgClk_enum	get_clk_enum()	  { return  ClkEnum; }

    const uint32_t	get_Cntl_offset( rgClk_enum clknum );

    const char*		cstr_clk_enum( rgClk_enum clknum );

    rgClk_enum		int2clk_enum( int num );

    inline
    volatile uint32_t*	get_base_addr()   { return  GpioBase; }

		// Test/Debug accessors

    inline uint32_t	get_bcm_address() { return FeatureAddr; }
};

#endif

