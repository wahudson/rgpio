// 2020-03-23  William A. Hudson

#ifndef yVersion_P
#define yVersion_P

//--------------------------------------------------------------------------
// rgpio Version class
//--------------------------------------------------------------------------

class yVersion {

  public:
    inline static  const char *	get_Version() {
	return
	    #include "./version.rgpio"
	;
	// unique file name to not find librgpio version.inc
    };

};

#endif

