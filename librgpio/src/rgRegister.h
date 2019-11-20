// 2019-11-12  William A. Hudson

#ifndef rgRegister_P
#define rgRegister_P

//--------------------------------------------------------------------------
// rGPIO Register base class.
//--------------------------------------------------------------------------

class rgRegister {

  private:
    volatile uint32_t	*Addr;		// register address
    uint32_t		RegVal;		// register value

  public:
    rgRegister();			// constructor

		// Address of register

    inline
    volatile uint32_t*	addr()				{ return Addr; }

		// Direct hardware register access

    inline uint32_t	read()				{ return *Addr; }

    inline void		write( uint32_t rval )		{ *Addr = rval; }

		// Object state operations

    inline void		grab()				{ RegVal = *Addr; }
    inline void		push()				{ *Addr = RegVal; }

    inline uint32_t	get()				{ return RegVal; }
    inline void		put( uint32_t rval)		{ RegVal = rval; }

		// Generic Field accessors:  (derived registers only)
  protected:
    uint32_t		get_field(
	const uint32_t		pos,
	const uint32_t		mask
    );

    void		put_field(
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    );

		// Initialization:  (private, used by feature constructor)
  public:
    inline void		init_addr( volatile uint32_t* aptr)  { Addr = aptr; }

};

#endif

