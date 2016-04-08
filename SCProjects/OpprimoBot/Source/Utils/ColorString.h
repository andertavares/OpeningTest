#ifndef __COLORSTRING_H__
#define __COLORSTRING_H__

#include <string>

using namespace std;

class ColorString {
private:
	ColorString();
	~ColorString();

public:
	static const string CYAN; 
	static const string YELLOW;
	static const string WHITE;
	static const string GREY;
	static const string RED;
	static const string GREEN;
	static const string BLUE;
	static const string PURPLE;
	static const string ORANGE;
	static const string BROWN;
	static const string TEAL;
};

#endif