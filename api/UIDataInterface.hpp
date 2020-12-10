#ifndef		UI_DATA_INTERFACE_H
#define		UI_DATA_INTERFACE_H

enum MOTION_AXIS{
	AXIS_X				=0x0001,
	AXIS_Y				=0x0002,
	AXIS_Z				=0x0004,
	AXIS_4				=0x0008,
	AXIS_MASK			=0x000F
};

enum PARAM_DIR{
	READ,
	WRITE
};


enum MOTION_DIR{
	POSITIVE,
	NEGATIVE
};

enum MOTION_X_DIR{
	LEFT,
	RIGHT
};

enum MOTION_Y_DIR{
	FORWARD,
	BACKWARD
};

enum MOTION_Z_DIR{
	UPWARD,
	DOWNWARD
};

enum FeatherLevel{
	Quick,
	Low,
	Middle,
	All
};

struct IMAGE_ROI{
	int x;								//\x clip position
	int y;								//\y clip position
	int w;								//\clip width
	int h;								//\clip height
};

struct POSTION{
	int x;
	int y;
};

struct IMG_SIZE{
	unsigned int Width;
	unsigned int Height;
};

struct IMAGE_CLIP{
	double x;								//\x clip position
	double y;								//\y clip position
	double w;								//\clip width
	double h;								//\clip height
};

struct IMAGE_TITLE{
	unsigned int xNumber;					//\x tile number, must > 0
	unsigned int yNumber;					//\y tile number, must > 1
	unsigned int rev[2];					//\default=0

	double xInterval;						//\x0---x1 interval
	double yInterval;						//\y0---y1 interval

	double xCoord;
	double yCoord;
};


/*
 *
*/
struct JOB_INFO{
	IMAGE_TITLE Title;
	IMAGE_CLIP  Clip;
};

struct BASE_SETTING{
	unsigned int xSkipBlank;				//\Bit[31]=true:skip
	unsigned int ySkipBlank;				//\Bit[31]=true:skip

	unsigned char xMirror;					//\Bit[31]=true:mirror
	unsigned char xReverse;					//\bit0=0:<<--;bit0=1:-->>;
	unsigned char yReverse;					//\Bit[31]=true:reverse
	unsigned char Bidirection;				//\1=bidirection print;0=unidirection

	unsigned int AutoOrigin;                                //\auto adjust the print origin

	unsigned int rev[12];					//\default=0
};

struct FEATHER_CFG{
	unsigned short Percent[4];
	unsigned char Shade;
	unsigned char Type;
	unsigned char rev1[2];

	unsigned int rev2[13];
};

//cmd=CMD_MODE_ORIGIN
struct ORIGIN_SET{
	unsigned int GetMode;					//\0:set by ui; 1:mesure auto
	unsigned int Coord;					//\the start pos: unit:mm

	unsigned int rev[14];
};
//cmd=CMD_MOTION_MOVE
struct MOVE{
	unsigned int Axis;					//\AXIS_X/AXIS_Y/AXIS_Z...
	unsigned int Dir;					//\positive; forward
	unsigned int Speed;					//\speed
	unsigned int Distance;					//\unit:pulse
};

//cmd=CMD_MOTION_PRINT
struct MOTION_PRINT{
	int xStart;
	int xEnd;
	int BandX;
	int BandWidth;

	unsigned char xDir;
	unsigned char xSpeed;
	unsigned char yDir;
	unsigned char ySpeed;

	int yStep;
	
	int rev;
};

//cmd=CMD_MOTION_GO_HOME
struct GO_HOME{
	unsigned int Axis;					//\AXIS_X/AXIS_Y/AXIS_Z...
	unsigned int Speed;					//\speed
};

//CMD_MOTION_MEASURE
struct MEASURE{
	float	xPosition;
	float	yPosition;
	float	Width;
	float	Height;
};

//cmd=CMD_MOTION_GET_COORD
//cmd=CMD_MOTION_GET_COORD_PULSE
union uCOORD{
	struct COORD_F{						//\unit:inch
		double xPosition;				//\x position
		double yPosition;				//\y position
		double zPosition;				//\z position
		double fPosition;				//\foure axis positoin
	}Coord_f;
	struct COORD_P{						//\unit:pulse
		long long   xPosition;				
		long long   yPosition;				
		long long   zPosition;				
		long long   fPosition;				
	}Coord_Pulse;
};

struct PRODUCT_MODEL{
	unsigned char  xPass;
	unsigned char  Interpolation; 
	unsigned char  MultiInkVolume; 
	unsigned char  FeatherLevel;					//\quick;low/midle/high
	unsigned short Feather;						//\percent
	unsigned short rev0;
	unsigned int   rev1[14];
};

struct INK_PUMP{
	int Id;
	float InkSupplyPressure;
	float InkCyclePressure;
	int PumpEnable;
	unsigned short InkQuantity[16];
	int rev[4];
};

struct CLEAN{
	int PressInkTime;					//\ms
	int SuckInkSpeed;					//\mm/s
	int INKCapMoveSpeed;					//\mm/s
	int TankHeight;						//\mm/s

	int FlashHeight;
	int InkTankPos;
	int HeadCupHeight;	
	int rev[12];						//\default 0
};

enum PRINT_SPEED{
	HighSpeed,
	MidleSpeed,
	LowSpeed
};

struct PRINT_MOVE{
	struct SCANNING_MOVE{
		int Speed;
		int AccDistance;
	}Scanning[3];

	int CurSpeed;
	int rev[9];
};

struct FLASH{
	unsigned int	Open;					//
	unsigned int	Pwm;					//
	unsigned int	Period;					//
	unsigned int	SprayTimeBeforePrint;			//	

	int rev[12];
};

struct LIMIT{
	float	PlatSpace;
	float	Width;
	float	Height;

	int rev[13];
};

struct MEDIA{
	float	xPosition;
	float	yPosition;
	float	Width;
	float	Height;

	int rev[12];
};

struct UV_CTRL{
	unsigned char LeftEnable;
	unsigned char RightEnable;
	unsigned char Power;
	unsigned char rev;
};
struct UV_INFO{
	int Width;
	int Lenght;
	int LeftDistance;
	int RightDistance;

	int PrintMode; 
	int SolidifyMode;
	UV_CTRL Ctrl[2];	

	int rev[8];
};

struct MECHAINE{
	int Len;
	int PrintHeadType;						//\print head type
	int HeadBoardType;						//\print head type

	int PrinterColorNum;
	int PrinterGroupNum;

	int xResolution;
	int yResolution;

	int NozzleNum;
	int ColumnPerHead;						//\column number of one head

	char PrintColor[64];						//\K;C;M;Y;Lm;Lc;W0;W1

	unsigned char Gray;
	unsigned char GraySelfAdapt;

	unsigned char rev0[30];
};


struct USER_DEFINED{
	unsigned int PumpWarning	:1;
	unsigned int rev		:31;

	unsigned int NoPaper1		:1;
	unsigned int NoPaper2		:1;
	unsigned int PaperWeightLoss	:1;
	unsigned int rev2		:29;

	unsigned int rev3[14];
};


struct STATUS_REPORT{
    unsigned int error_code;
    char context[60];
};


#endif
