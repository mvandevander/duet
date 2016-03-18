#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "10";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.09";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 2;
	static const long BUILD = 417;
	static const long REVISION = 2293;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 712;
	#define RC_FILEVERSION 0,2,417,2293
	#define RC_FILEVERSION_STRING "0, 2, 417, 2293\0"
	static const char FULLVERSION_STRING[] = "0.2.417.2293";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 49;
	

}
#endif //VERSION_H
