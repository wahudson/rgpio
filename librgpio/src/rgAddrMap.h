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

    static volatile uint32_t	FakeBlock[1024];	// page size 4096 byte
    uint32_t		BaseAddr;	// real IO base address

  public:
    rgAddrMap();			// constructor
    ~rgAddrMap();			// destructor

    uint32_t		bcm2rpi_addr( uint32_t  bcm_addr );

    std::string		text_debug();

    inline void		config_Debug(    bool v ) { Debug = v; };
    void		config_FakeNoPi( bool v );

    void		config_BaseAddr( uint32_t v );
    uint32_t		config_BaseAddr()		{ return  BaseAddr; }

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

				// Test and Debug (private)
    inline int		size_BlkCache() {
	return  BlkCache.size();
    };

    inline int		get_DevFD()	{ return  Dev_fd; }
};


#endif

