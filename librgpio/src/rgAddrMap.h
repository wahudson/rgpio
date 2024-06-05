// 2017-05-17  William A. Hudson

#ifndef rgAddrMap_P
#define rgAddrMap_P

#include <map>

//--------------------------------------------------------------------------
// rGPIO Address Map class
//--------------------------------------------------------------------------

class rgAddrMap {
  private:
    int			Dev_fd;		// file descriptor
    bool		FakeMem;	// 1= using fake memory, 0= not

    bool		FakeNoPi;	// fallback when not on RPi
					//     1= fake memory, 0= throw error

    const char		*ModeStr;	// memory mode string, NULL= unset,
					// "/dev/mem", "/dev/gpiomem", "fake_mem"

    std::map<uint32_t,void*>  BlkCache;	// cache of mapped memory blocks

//  int			Prot;		// mmap() prot field
    bool		Debug;		// debug trace on stderr

    static
    const uint32_t	MaxBlock_w = 4096;  // max mapped block size, in words
					    // RPi5 is 4 pages of 4096 bytes
    static
    volatile uint32_t	FakeBlock[MaxBlock_w];  // shared fake memory block

    uint64_t		BaseAddr;	// real IO base address
    uint32_t		DocBase;	// documentation base address
    uint32_t		BlockSize;	// allocation block size, byte

  public:
    rgAddrMap();			// constructor
    ~rgAddrMap();			// destructor

    uint64_t		bcm2rpi_addr( uint32_t  bcm_addr );

  public:	// test/debug/override
    std::string		text_debug();

    inline void		config_Debug(    bool v ) { Debug = v; };
    void		config_FakeNoPi( bool v );

    void		config_BaseAddr( uint64_t v );
    uint64_t		config_BaseAddr()		{ return  BaseAddr; }

    void		config_DocBase(   uint32_t v )	{ DocBase = v; }
    uint32_t		config_DocBase()		{ return  DocBase; }

    void		config_BlockSize( uint32_t v )	{ BlockSize = v; }
    uint32_t		config_BlockSize()		{ return  BlockSize; }

  public:
    void		open_dev_file(
	const char*		file,
	bool			drop_cap = 1
    );

    inline void		open_dev_mem() {
	this->open_dev_file( "/dev/mem" );
    };

    inline void		open_dev_gpiomem() {
	this->open_dev_file( "/dev/gpiomem", 0 );
    };

    inline void		open_fake_mem() {
	this->open_dev_file( "" );
    };

//    void		prot_readonly();
//    void		prot_readwrite();

    void		close_dev();


    volatile uint32_t*	get_mem_block( uint32_t  bcm_addr );
    volatile uint32_t*	get_mem_addr(  uint32_t  bcm_addr );

//    void		drop_capabilities();

    inline bool		is_fake_mem() {		// TRUE if using fake memory
	return FakeMem;
    };

  public:			// Test and Debug (private)
    inline int		size_BlkCache() {
	return  BlkCache.size();
    };

    inline int		get_DevFD()	{ return  Dev_fd; }
};


#endif

