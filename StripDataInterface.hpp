#ifndef STRIP_DATA_INTERFACE_H
#define STRIP_DATA_INTERFACE_H


enum STRIP_POS{
	NOSTRIP		= 0,
	LEFT_SIDE	= 1,
	RIGHT_SIDE	= 2,
	BOTH_SIDE	= 3
};

enum STRIP_TYPE{

};

struct STRIP{
	unsigned int Type;					//\no used
	unsigned int Position;					//\left,right
	unsigned int Attribute;					//\the same height with image;
	float Width;						//\width of strip
	float Offset;						//\distance far form image

	int rev[11];
};

#endif
