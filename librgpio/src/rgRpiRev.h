// 2020-11-22  William A. Hudson

#ifndef rgRpiRev_P
#define rgRpiRev_P

//#include <iostream>
//#include <fstream>

//--------------------------------------------------------------------------
// rGPIO  rgRpiRev - Raspberry Pi Revision
//--------------------------------------------------------------------------

/*
* rgFlag base class.
*    Idea is to provide flags for caching derived data stored elsewhere.
*    Final   = Derivation is final, use the cached value.
*    Unknown = Cached value is unknown.
*/
class rgFlag {
  protected:
    bool		Final;		// 1= no further derivation, 0= not
    bool		Unknown;	// 1= unknown, 0= known good value

  public:
    rgFlag()		{ Final = 0;  Unknown = 1; } // constructor

    bool		is_final()		{ return  Final; }
    bool		is_unknown()		{ return  Unknown; }

  public:	// (private) testing
    void		putFU( bool f, bool u )	{ Final = f;  Unknown = u; }

    void		mark_final()		{ Final = 1;  Unknown = 0; }
    void		clear_final()		{ Final = 0;  Unknown = 0; }
};


/*
* rgWord base class.
*    Provides storage and generic field accessors for derived classes.
*/
class rgWord {
  protected:
    uint32_t		WordVal;	// 32-bit value

  public:
    rgWord()		{ WordVal = 0; }	// constructor

    uint32_t		get()			{ return  WordVal; }
    void		put( uint32_t v )	{ WordVal = v; }

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


class rgRpiRev_Code : public rgWord, public rgFlag {
  private:
    const char*		InFile;		// rev code file name

  public:
    rgRpiRev_Code();			// constructor

    void		init_file( const char* v )	{ InFile = v; }
    const char*		init_file()			{ return  InFile; }

  public:	// user
    uint32_t		find();		// find/return RevCode

    void		override( uint32_t v )	{
				    WordVal = v;  Final = 1;  Unknown = 0; }

  public:				// field access

    uint32_t	get_OverVoltageDis_1()    { return  get_field( 31, 0x1    ); }
    void	put_OverVoltageDis_1( uint32_t v) { put_field( 31, 0x1, v ); }

    uint32_t	get_OtpWriteDis_1()       { return  get_field( 30, 0x1    ); }
    void	put_OtpWriteDis_1( uint32_t v )   { put_field( 30, 0x1, v ); }

    uint32_t	get_OtpReadDis_1()        { return  get_field( 29, 0x1    ); }
    void	put_OtpReadDis_1( uint32_t v )    { put_field( 29, 0x1, v ); }

    uint32_t	get_WarrantyVoid_1()      { return  get_field( 25, 0x1    ); }
    void	put_WarrantyVoid_1( uint32_t v )  { put_field( 25, 0x1, v ); }

    uint32_t	get_NewStyle_1()          { return  get_field( 23, 0x1    ); }
    void	put_NewStyle_1( uint32_t v )      { put_field( 23, 0x1, v ); }

    uint32_t	get_MemSize_3()           { return  get_field( 20, 0x7    ); }
    void	put_MemSize_3( uint32_t v )       { put_field( 20, 0x7, v ); }

    uint32_t	get_MfgNumber_4()         { return  get_field( 16, 0xf    ); }
    void	put_MfgNumber_4( uint32_t v )     { put_field( 16, 0xf, v ); }

    uint32_t	get_ChipNumber_4()        { return  get_field( 12, 0xf    ); }
    void	put_ChipNumber_4( uint32_t v )    { put_field( 12, 0xf, v ); }

    uint32_t	get_BoardType_8()         { return  get_field(  4, 0xff    ); }
    void	put_BoardType_8( uint32_t v )     { put_field(  4, 0xff, v ); }

    uint32_t	get_BoardRev_4()          { return  get_field(  0, 0xf    ); }
    void	put_BoardRev_4( uint32_t v )      { put_field(  0, 0xf, v ); }

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

    class rgRpiRev_Soc : public rgFlag {
      private:
	Soc_enum	SocVal;
	rgRpiRev_Code	*RevCode_ptr;

      public:
	rgRpiRev_Soc();			// constructor

	void		init_ptr( rgRpiRev_Code *wp ) { RevCode_ptr = wp; }
	rgRpiRev_Code*	init_ptr()		      { return  RevCode_ptr; }

	Soc_enum	get()		{ return  SocVal; }

      public:	// user
	Soc_enum	find();		// find/return SocEnum
	const char*	cstr()		{ return  soc_enum2cstr( find() ); }

	void		override( Soc_enum v );
    };

    class rgRpiRev_Base : public rgFlag {
      private:
	uint64_t	BaseVal;
	rgRpiRev_Soc	*SocEnum_ptr;

      public:
	rgRpiRev_Base();		// constructor

	void		init_ptr( rgRpiRev_Soc *wp ) { SocEnum_ptr = wp; }
	rgRpiRev_Soc*	init_ptr()		     { return  SocEnum_ptr; }

	uint64_t	get()		{ return  BaseVal; }

      public:	// user
	uint64_t	find();		// find/return BaseAddr

	void		override( uint64_t v )	{
				    BaseVal = v;  Final = 1;  Unknown = 0; }
    };

  public:	// word registers for user override or query
    rgRpiRev_Code	RevCode;	// revision code
    rgRpiRev_Soc	SocEnum;	// chip id
    rgRpiRev_Base	BaseAddr;	// IO base address

    static rgRpiRev	Global;		// global config data is this class

    rgRpiRev();		// constructor

  public:	// preferred user level short-hand accessors
    static Soc_enum	find_SocEnum()	{ return  Global.SocEnum.find();  }
    static uint32_t	find_BaseAddr()	{ return  Global.BaseAddr.find(); }

    static const char*	cstr_SocEnum()	{ return  Global.SocEnum.cstr();  }

  public:	// enum conversions
    static const char*	soc_enum2cstr( Soc_enum  soc );
    static Soc_enum	int2soc_enum(  int       num );
};

/*
* Note:  Nested classes solve dependency problems.
* Want rgRpiRev::Soc_enum defined in name space of a parent class.
* The derived word classes depend on each other, and on Soc_enum, and
* are not really needed outside of main class.
* Using forward class declarations resulted in "incomplete type" errors.
*/

#endif

