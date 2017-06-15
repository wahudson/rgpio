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

    void		use_dev_mem();
    void		use_dev_gpiomem();
    void		use_fake_mem();

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

