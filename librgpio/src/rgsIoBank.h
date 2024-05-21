// 2024-05-02  William A. Hudson

#ifndef rgsIoBank_P
#define rgsIoBank_P

//--------------------------------------------------------------------------
// rgsIoBank - Abstract base class for IO Bank Features - RPi5
//--------------------------------------------------------------------------

class rgsIoBank {
  protected:
    int			BankNum    = 0;		// Bank number 0..2
    volatile uint32_t	*GpioBase  = NULL;	// virtual IO base address
    uint32_t		DocAddress = 0;		// documentation base address

  public:
    uint32_t		get_bank_num()		{ return  BankNum; }

    volatile uint32_t*	get_base_addr()		{ return  GpioBase;  }

    virtual uint32_t	get_bcm_address() = 0;	// FeatureAddr

    uint32_t		get_doc_address()	{ return  DocAddress; }

    uint32_t		get_doc_offset( volatile uint32_t *a ) {
				return  ((a - GpioBase) * 4);
			    }
};

#endif

