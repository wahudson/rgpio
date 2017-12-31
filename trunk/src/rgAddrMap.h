// 2017-05-17  William A. Hudson

#ifndef rgAddrMap_P
#define rgAddrMap_P

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
    int			Prot;		// mmap() prot field
    bool		Debug;		// debug trace on stderr

    volatile uint32_t	FakeBlock[4096];


  public:
    rgAddrMap();
    rgAddrMap( int init );

    uint32_t		bcm2rpi_addr(
			    uint32_t            addr
    );

    std::string		text_debug();

    void		config_FakeNoPi( const bool v );

    void		open_dev_file( const char *file );

    inline void		open_dev_mem() {
	this->open_dev_file( "/dev/mem" );
    };

    inline void		open_dev_gpiomem() {
	this->open_dev_file( "/dev/gpiomem" );
    };

    inline void		open_fake_mem() {
	this->open_dev_file( "" );
    };

//    void		prot_readonly();
//    void		prot_readwrite();

    void		close_dev();


    volatile uint32_t*	get_mem_block( int p_addr );

    void		drop_capabilities();

    inline bool		is_fake_mem() {		// TRUE if using fake memory
	return FakeMem;
    };
};


#endif

