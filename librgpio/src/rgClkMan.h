// 2019-12-10  William A. Hudson

#ifndef rgClkMan_P
#define rgClkMan_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rGPIO  rgClkMan - Clock Manager base class
//--------------------------------------------------------------------------

class rgClk_Cntl : public rgRegister {
  public:

    inline
    uint32_t	get_Passwd_8()           { return  get_field( 24, 0xff    ); };
    void	put_Passwd_8( uint32_t v )       { put_field( 24, 0xff, v ); };

    inline
    uint32_t	get_Mash_2()             { return  get_field(  9, 0x3    ); };
    void	put_Mash_2( uint32_t v )         { put_field(  9, 0x3, v ); };

    inline
    uint32_t	get_Flip_1()             { return  get_field(  8, 0x1    ); };
    void	put_Flip_1( uint32_t v )         { put_field(  8, 0x1, v ); };

    inline
    uint32_t	get_Busy_1()             { return  get_field(  7, 0x1    ); };
    void	put_Busy_1( uint32_t v )         { put_field(  7, 0x1, v ); };

    inline
    uint32_t	get_Kill_1()             { return  get_field(  5, 0x1    ); };
    void	put_Kill_1( uint32_t v )         { put_field(  5, 0x1, v ); };

    inline
    uint32_t	get_Enable_1()           { return  get_field(  4, 0x1    ); };
    void	put_Enable_1( uint32_t v )       { put_field(  4, 0x1, v ); };

    inline
    uint32_t	get_Source_4()           { return  get_field(  0, 0xf    ); };
    void	put_Source_4( uint32_t v )       { put_field(  0, 0xf, v ); };

};

class rgClk_Divr : public rgRegister {
  public:

    inline
    uint32_t	get_Passwd_8()           { return  get_field( 24, 0xff    ); };
    void	put_Passwd_8( uint32_t v )       { put_field( 24, 0xff, v ); };

    inline
    uint32_t	get_DivI_12()           { return  get_field( 12, 0xfff    ); };
    void	put_DivI_12( uint32_t v )       { put_field( 12, 0xfff, v ); };

    inline
    uint32_t	get_DivF_12()           { return  get_field(  0, 0xfff    ); };
    void	put_DivF_12( uint32_t v )       { put_field(  0, 0xfff, v ); };

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

  public:
    rgClkMan();			// constructor

		// Object state operations
    void		init_put_reset();

    void		grab_regs();
    void		push_regs();

		// Test/Debug accessors

    void		show_debug( std::ostream&  sout );

};

#endif

