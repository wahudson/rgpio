// 2020-11-22  William A. Hudson

#ifndef rgRpiRev_P
#define rgRpiRev_P

//#include <iostream>
//#include <fstream>

//--------------------------------------------------------------------------
// rGPIO  rgRpiRev - Raspberry Pi Revision
//--------------------------------------------------------------------------

/*
* rgWord base class.
*    Idea is to cache a word value derived from external sources.
*    The flag tracks whether derivation is final, and should not be repeated.
*/
class rgWord {
  protected:
    uint32_t		WordVal;	// 32-bit value
    bool		Final;		// 1= no further derivation, 0= not

  public:
    rgWord();		// constructor

    uint32_t		get()			{ return  WordVal; }
    void		put( uint32_t v )	{ WordVal = v; }

    bool		is_final()		{ return  Final; }
    void		mark_final()		{ Final = 1; }
    void		clear_final()		{ Final = 0; }

  protected:	// Generic Field accessors:  (derived registers only)
    uint32_t		get_field(
	const uint32_t		pos,
	const uint32_t		mask
    );

    void		put_field(
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    );
};


class rgRpiRev_Code : public rgWord {
  private:
    const char*		InFile;		// rev code file name

  public:
    rgRpiRev_Code();			// constructor

    void		init_file( const char* v )	{ InFile = v; }
    const char*		init_file()			{ return  InFile; }

    uint32_t		find();		// find/return RevCode

  public:				// field access

    uint32_t	get_NewStyle_1()          { return  get_field( 23, 0x1    ); }
    void	put_NewStyle_1( uint32_t v )      { put_field( 23, 0x1, v ); }

    uint32_t	get_MemSize_3()           { return  get_field( 20, 0x7    ); }
    void	put_MemSize_3( uint32_t v )       { put_field( 20, 0x7, v ); }

    uint32_t	get_MfgNumber_4()         { return  get_field( 16, 0xf    ); }
    void	put_MfgNumber_4( uint32_t v )     { put_field( 16, 0xf, v ); }

    uint32_t	get_ChipNumber_4()        { return  get_field( 12, 0xf    ); }
    void	put_ChipNumber_4( uint32_t v )    { put_field( 12, 0xf, v ); }

  public:	// mostly internal use
    uint32_t		read_rev_code( const char*	ifile );
    uint32_t		read_rev_code( std::istream*	istm );
};


/*
* Main Class
*/
class rgRpiRev {
  public:
    enum Soc_enum {	// SOC chip identifiers
	soc_BCM2835 = 0,	// RPi1, Zero
	soc_BCM2836,		// RPi2
	soc_BCM2837,		// RPi3, and later RPi2
	soc_BCM2711,		// RPi4
	soc_MaxEnum = soc_BCM2711	// make identical to highest enum
    };

    class rgRpiRev_Soc : public rgWord {
      private:
	rgRpiRev_Code	*RevCode_ptr;
	bool		FailDerive;
      public:
	rgRpiRev_Soc();			// constructor

	void		init_ptr( rgRpiRev_Code *wp ) { RevCode_ptr = wp; }
	rgRpiRev_Code*	init_ptr()		      { return  RevCode_ptr; }

	Soc_enum	find();		// find/return SocEnum

	bool		is_fail()	{ return FailDerive; }

				// these hide rgWord functions of the same name
	Soc_enum	get()		{ return  (Soc_enum) WordVal; }
	void		put( Soc_enum v );

	void		mark_final()	{ Final = 1;  FailDerive = 0; }
	void		clear_final()	{ Final = 0;  FailDerive = 0; }
    };

    class rgRpiRev_Base : public rgWord {
      private:
	rgRpiRev_Soc	*SocEnum_ptr;
      public:
	rgRpiRev_Base();		// constructor

	void		init_ptr( rgRpiRev_Soc *wp ) { SocEnum_ptr = wp; }
	rgRpiRev_Soc*	init_ptr()		     { return  SocEnum_ptr; }

	uint32_t	find();		// find/return BaseAddr
    };

  public:	// word registers for user configure or query
    rgRpiRev_Code	RevCode;	// revision code
    rgRpiRev_Soc	SocEnum;	// chip id
    rgRpiRev_Base	BaseAddr;	// IO base address

    static rgRpiRev	Config;		// config data is this class

    rgRpiRev();		// constructor

  public:	// enum conversions
    static const char*		soc_enum2cstr( Soc_enum  soc );
    static Soc_enum		int2soc_enum(  int       num );
};

/*
* Note:  Nested classes solve dependency problems.
* Want rgRpiRev::Soc_enum defined in name space of a parent class.
* The derived word classes depend on each other, and on Soc_enum, and
* are not really needed outside of main class.
* Using forward class declarations resulted in "incomplete type" errors.
*/

#endif

