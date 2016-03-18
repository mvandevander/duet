#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.09";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 9;
	static const long BUILD = 2013;
	static const long REVISION = 11139;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3441;
	#define RC_FILEVERSION 1,9,2013,11139
	#define RC_FILEVERSION_STRING "1, 9, 2013, 11139\0"
	static const char FULLVERSION_STRING[] = "1.9.2013.11139";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 13;
	

}
#endif //VERSION_H
