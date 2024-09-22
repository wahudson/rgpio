// 2024-04-10  William A. Hudson

#ifndef rgsFuncName_P
#define rgsFuncName_P

//--------------------------------------------------------------------------
// rgsFuncName  Alternate Function Names for RPi5
//--------------------------------------------------------------------------

class rgsFuncName {

  private:
    static const char*		FuncName[][9];

  public:
    rgsFuncName();			// constructor

    static const char*		cstr_altfuncAN(
	int			    altnum,
	int                         bit
    );
};

#endif

