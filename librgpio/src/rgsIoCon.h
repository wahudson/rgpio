// 2024-05-11  William A. Hudson

#ifndef rgsIoCon_P
#define rgsIoCon_P

#include "rgsRegAtom.h"
#include "rgsIoBank.h"

//--------------------------------------------------------------------------
// rgsIoCon - IO Control/Status Interface class for RPi5
//--------------------------------------------------------------------------

class rgsIo_Cntl : public rgsRegAtom {
  public:

    uint32_t	get_IrqOver_2()           { return  get_field( 30, 0x3    ); }
    void	put_IrqOver_2(       uint32_t v ) { put_field( 30, 0x3, v ); }

    uint32_t	get_EdgeReset_1()         { return  get_field( 28, 0x1    ); }
    void	put_EdgeReset_1(     uint32_t v ) { put_field( 28, 0x1, v ); }

    uint32_t	get_ImaskFiltHigh_1()     { return  get_field( 27, 0x1    ); }
    void	put_ImaskFiltHigh_1( uint32_t v ) { put_field( 27, 0x1, v ); }

    uint32_t	get_ImaskFiltLow_1()      { return  get_field( 26, 0x1    ); }
    void	put_ImaskFiltLow_1(  uint32_t v ) { put_field( 26, 0x1, v ); }

    uint32_t	get_ImaskFiltRise_1()     { return  get_field( 25, 0x1    ); }
    void	put_ImaskFiltRise_1( uint32_t v ) { put_field( 25, 0x1, v ); }

    uint32_t	get_ImaskFiltFall_1()     { return  get_field( 24, 0x1    ); }
    void	put_ImaskFiltFall_1( uint32_t v ) { put_field( 24, 0x1, v ); }

    uint32_t	get_ImaskHigh_1()         { return  get_field( 23, 0x1    ); }
    void	put_ImaskHigh_1(     uint32_t v ) { put_field( 23, 0x1, v ); }

    uint32_t	get_ImaskLow_1()          { return  get_field( 22, 0x1    ); }
    void	put_ImaskLow_1(      uint32_t v ) { put_field( 22, 0x1, v ); }

    uint32_t	get_ImaskRise_1()         { return  get_field( 21, 0x1    ); }
    void	put_ImaskRise_1(     uint32_t v ) { put_field( 21, 0x1, v ); }

    uint32_t	get_ImaskFall_1()         { return  get_field( 20, 0x1    ); }
    void	put_ImaskFall_1(     uint32_t v ) { put_field( 20, 0x1, v ); }

    uint32_t	get_InOver_2()            { return  get_field( 16, 0x3    ); }
    void	put_InOver_2(        uint32_t v ) { put_field( 16, 0x3, v ); }

    uint32_t	get_OutEnOver_2()         { return  get_field( 14, 0x3    ); }
    void	put_OutEnOver_2(     uint32_t v ) { put_field( 14, 0x3, v ); }

    uint32_t	get_OutOver_2()           { return  get_field( 12, 0x3    ); }
    void	put_OutOver_2(       uint32_t v ) { put_field( 12, 0x3, v ); }

    uint32_t	get_FilterM_7()           { return  get_field(  5, 0x7f    ); }
    void	put_FilterM_7(       uint32_t v ) { put_field(  5, 0x7f, v ); }

    uint32_t	get_FuncSel_5()           { return  get_field(  0, 0x1f    ); }
    void	put_FuncSel_5(       uint32_t v ) { put_field(  0, 0x1f, v ); }
};

class rgsIo_Stat : public rgsRegAtom {
  public:

    uint32_t	get_IrqToProc_1()         { return  get_field( 29, 0x1    ); }
    void	put_IrqToProc_1(     uint32_t v ) { put_field( 29, 0x1, v ); }

    uint32_t	get_IrqMasked_1()         { return  get_field( 28, 0x1    ); }
    void	put_IrqMasked_1(     uint32_t v ) { put_field( 28, 0x1, v ); }

    uint32_t	get_InFiltHigh_1()        { return  get_field( 27, 0x1    ); }
    void	put_InFiltHigh_1(    uint32_t v ) { put_field( 27, 0x1, v ); }

    uint32_t	get_InFiltLow_1()         { return  get_field( 26, 0x1    ); }
    void	put_InFiltLow_1(     uint32_t v ) { put_field( 26, 0x1, v ); }

    uint32_t	get_InFiltRise_1()        { return  get_field( 25, 0x1    ); }
    void	put_InFiltRise_1(    uint32_t v ) { put_field( 25, 0x1, v ); }

    uint32_t	get_InFiltFall_1()        { return  get_field( 24, 0x1    ); }
    void	put_InFiltFall_1(    uint32_t v ) { put_field( 24, 0x1, v ); }

    uint32_t	get_InHigh_1()            { return  get_field( 23, 0x1    ); }
    void	put_InHigh_1(        uint32_t v ) { put_field( 23, 0x1, v ); }

    uint32_t	get_InLow_1()             { return  get_field( 22, 0x1    ); }
    void	put_InLow_1(         uint32_t v ) { put_field( 22, 0x1, v ); }

    uint32_t	get_InRise_1()            { return  get_field( 21, 0x1    ); }
    void	put_InRise_1(        uint32_t v ) { put_field( 21, 0x1, v ); }

    uint32_t	get_InFall_1()            { return  get_field( 20, 0x1    ); }
    void	put_InFall_1(        uint32_t v ) { put_field( 20, 0x1, v ); }

    uint32_t	get_InToPeri_1()          { return  get_field( 19, 0x1    ); }
    void	put_InToPeri_1(      uint32_t v ) { put_field( 19, 0x1, v ); }

    uint32_t	get_InFiltered_1()        { return  get_field( 18, 0x1    ); }
    void	put_InFiltered_1(    uint32_t v ) { put_field( 18, 0x1, v ); }

    uint32_t	get_InOfPad_1()           { return  get_field( 17, 0x1    ); }
    void	put_InOfPad_1(       uint32_t v ) { put_field( 17, 0x1, v ); }

    uint32_t	get_InIsDirect_1()        { return  get_field( 16, 0x1    ); }
    void	put_InIsDirect_1(    uint32_t v ) { put_field( 16, 0x1, v ); }

    uint32_t	get_OutEnToPad_1()        { return  get_field( 13, 0x1    ); }
    void	put_OutEnToPad_1(    uint32_t v ) { put_field( 13, 0x1, v ); }

    uint32_t	get_OutEnOfPeri_1()       { return  get_field( 12, 0x1    ); }
    void	put_OutEnOfPeri_1(   uint32_t v ) { put_field( 12, 0x1, v ); }

    uint32_t	get_OutToPad_1()          { return  get_field(  9, 0x1    ); }
    void	put_OutToPad_1(      uint32_t v ) { put_field(  9, 0x1, v ); }

    uint32_t	get_OutOfPeri_1()         { return  get_field(  8, 0x1    ); }
    void	put_OutOfPeri_1(     uint32_t v ) { put_field(  8, 0x1, v ); }
};


class rgsIoCon : public rgsIoBank {
  private:
//  uint32_t		BankNum;	// Bank number 0..2
//  volatile uint32_t	*GpioBase;	// IO base address of Bank

    static const uint32_t  FeatureAddr = 0x400d0000;
			// Bank0, delta= 0x00004000, RP1 doc

    static const int	MaxBit = 27;	// Max register array index in any bank

  private:	// register per pin
    rgsIo_Cntl		CntlReg[MaxBit+1];
    rgsIo_Stat		StatReg[MaxBit+1];

  public:
    rgsIo_Cntl&		IoCntl( int bit );	// Register accessor
    rgsIo_Stat&		IoStat( int bit );	// Register accessor

  public:
    rgsIoCon(			// constructor
	rgAddrMap	*xx,
	uint32_t	bank_num = 0	// IO Bank number 0..2
    );

    uint32_t		get_bcm_address()	{ return  FeatureAddr; }
    uint32_t		get_MaxBit()		{ return  MaxBit; }

	// base class
//  uint32_t		get_bank_num()		{ return  BankNum; }
//  volatile uint32_t*	get_base_addr()		{ return  GpioBase; }
//  uint32_t		get_doc_address();
//  uint32_t		get_doc_offset();
};

#endif

