// 2024-04-20  William A. Hudson

#ifndef rgsRegAtom_P
#define rgsRegAtom_P

#include "rgRegister.h"

//--------------------------------------------------------------------------
// rgsRegAtom - Atomic Register base class for RPi5
//--------------------------------------------------------------------------
//    Assume memory block size is 4096 words (16 kByte) (0x1000 words).
//    Doc is Byte address, Code is word address (uint32_t pointer).

class rgsRegAtom : public rgRegister {

  public:
		// Address of register
    volatile uint32_t*  addr_flip()		{ return  (addr() + 0x0400); }
    volatile uint32_t*  addr_set()		{ return  (addr() + 0x0800); }
    volatile uint32_t*  addr_clr()		{ return  (addr() + 0x0c00); }

		// Read, _peek without side effect, _set _clr are normal
    inline uint32_t	read_peek()		{ return *(addr() + 0x0400); }
    inline uint32_t	read_set()		{ return *(addr() + 0x0800); }
    inline uint32_t	read_clr()		{ return *(addr() + 0x0c00); }

		// Write hardware special operation
    inline void		write_flip( uint32_t rval ) {*(addr() + 0x0400) = rval;}
    inline void		write_set(  uint32_t rval ) {*(addr() + 0x0800) = rval;}
    inline void		write_clr(  uint32_t rval ) {*(addr() + 0x0c00) = rval;}

		// Read special to object
    inline void		grab_peek()		{ put( *(addr() + 0x0400) ); }
    inline void		grab_set()		{ put( *(addr() + 0x0800) ); }
    inline void		grab_clr()		{ put( *(addr() + 0x0c00) ); }

		// Write special from object
    inline void		push_flip()		{ *(addr() + 0x0400) = get(); }
    inline void		push_set()		{ *(addr() + 0x0800) = get(); }
    inline void		push_clr()		{ *(addr() + 0x0c00) = get(); }
};

#endif

