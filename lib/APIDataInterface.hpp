
#ifndef		API_DATA_INTERFACE_		
#define		API_DATA_INTERFACE_

//#include		"type.h"
//#include		"Error.h"
//#include		"command.h"


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
	FORWARD
};

enum FeatherLevel{
	Quick,
	Low,
	Middle,
	All
};

struct IMG_SIZE{
	unsigned int Width;
	unsigned int Height;
};

struct PICTURE_INFO{
	unsigned int Height;						//\image height
	unsigned int Width;						//\image width
	unsigned int xResolution;				//\image x resolution
	unsigned int yResolution;				//\image y resolution

	unsigned int Bpp;						//\BitPerPixel
	unsigned int BytePerLine;				//\byte per line
	unsigned int ColorNumber;				//\color number

	char RipColorOrder[32];		//\rip
};

struct IMAGE_CLIP{
	double x;								//\x clip position
	double y;								//\y clip position
	double w;								//\clip width
	double h;								//\clip height
};

struct FEATHER_CFG{
	unsigned short Percent[4];
	unsigned char Shade;
	unsigned char Type;
	unsigned char rev1[2];

	unsigned int rev2[13];
};

struct FEATHER_TYPE{
	unsigned short	Percent;				//\0-100;
	unsigned char	Shade;					//\uniform;shade;
	unsigned char	Type;					//\head/normal

	unsigned char	xParticle;				//\no used
	unsigned char	yParticle;				//\no used

	unsigned char	rev[2];
};

struct PRINT_MODER{
	unsigned int xPass;						//\1,2,3,4
	unsigned int Speed;						//\high/mid/low/user 
	FEATHER_TYPE Feather;					//\ feather type 

	unsigned int MultiInkVolume;				//\ multi ink
	unsigned int Layer;
	unsigned int Mode;					//\fix color;step
	unsigned int Interpolation;				//\x div
	
	unsigned int rev[8];
};

struct IMAGE_TITLE{
	unsigned int xNumber;					//\x tile number, must > 0
	unsigned int yNumber;					//\y tile number, must > 1

	float xInterval;						//\x0---x1 interval
	float yInterval;						//\y0---y1 interval

	unsigned int rev[12];					//\default=0
};

struct BASE_SETTING{
    unsigned int xSkipBlank; //\Bit[31]=true:skip
    unsigned int ySkipBlank; //\Bit[31]=true:skip

    unsigned char xMirror; //\Bit[31]=true:mirror
    unsigned char xReverse; //\bit0=0:<<--;bit0=1:-->>;
    unsigned char yReverse; //\Bit[31]=true:reverse

    unsigned char Bidirection; //\1=bidirection print;0=unidirection
    unsigned int AutoOrigin; //\auto adjust the print origin

    unsigned int rev[12]; //\default=0
};

/*
 *
*/
struct JOB_INFO{
	IMAGE_TITLE Title;
	IMAGE_CLIP  Clip;

	const char * File;					//\file path
};

//cmd=CMD_MODE_ORIGIN
struct ORIGIN_SET{
	unsigned int GetMode;					//\0:set by ui; 1:mesure auto
	unsigned int Coord;					//\the start pos

	unsigned int rev[14];
};
//cmd=MOTION_CMD_MOVE
struct MOVE{
	unsigned int Axis;					//\AXIS_X/AXIS_Y/AXIS_Z...
	unsigned int Dir;					//\positive; forward
	unsigned int Speed;					//\speed
	unsigned int Distance;					//\unit:pulse
};

//cmd=MOTION_CMD_GO_HOME
struct GO_HOME{
	unsigned int Axis;					//\AXIS_X/AXIS_Y/AXIS_Z...
	unsigned int Speed;					//\speed
};

//MOTION_CMD_MEASURE
struct MEASURE{
	float	xPosition;
	float	yPosition;
	float	Width;
	float	Height;
};

//cmd=CMD_MOTION_GET_COORD
//cmd=CMD_MOTION_GET_COORD_PULSE
union COORD{
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

struct CALIBRATION{
	int bidirection;
	int y_step;
	int left_horizontal[256];
	int right_horizontal[256];
	int verticality[16];
	char overlap[4][16];
};

struct UI_CALIBRATION{
	int * left_horizontal;					//\left horizontal calibration value
	int * right_horizontal;					//\right horizontal calibration value
	int * verticality;					//\vertiflity value,unit:nozzle
	int * overlap;						//\nozzle overlap
	
	int bidirection;					//\no used, default 0
	int rev[11];						//\no used, default 0
};

struct PRODUCT_MODEL{
	unsigned char  xPass;
	unsigned char  FeatherLevel;					//\quick;low/midle/high
	unsigned short Feather;						//\percent
	unsigned char  Interpolation; 
	unsigned char  MultiInkVolume; 
	unsigned char rev[26];

	char name[128];
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

struct STATUS{
    unsigned int error_code;
    char context[63];
};

enum PRINT_SPEED{
	LowSpeed,
	MidleSpeed,
	HighSpeed
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

struct STRIP{
	unsigned int Type;					//\no used
	unsigned int Position;					//\left,right
	unsigned int Attribute;					//\the same height with image;
	float Width;						//\width of strip
	float Offset;						//\distance far form image

	int rev[11];
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

struct PrinterParam{
	BASE_SETTING Base;
	PRINT_MODER PrintMode;
	STRIP Stripe;
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

struct JOBINFO
{
	int	JobId;
	int	JobType;//

	int	ColorNum;
	int	ColorDepth;

	int	ResY;
	int	ResX;
	int	ImageXRes;
	int	ImageYRes;

	int	PassNum;
	int	BandNum;
	int	PassHeight;
	int	BandHeight;
	
	int	PureDataHeight;

	int	PageWidth;
	int	PageHeight;

	int	JobWidth;
	int	JobHeight;
};


struct _BANDINFO
{
	unsigned int	HeaderSize;
	unsigned int	HeaderFlg;
	unsigned int	BandType;
	int		BandId;

	JOBINFO		JobInfo;	//the same for every band

	unsigned int	DataOffset;
	unsigned int	PackageSize;
	unsigned int	BandSize;
	unsigned int	CompressSize;

	int		BandX;
	unsigned int 	BandY;
	unsigned int	BandWidth;
	unsigned int	BandDir;

	int		NextBandX;
	unsigned int 	NextBandY;
	unsigned int	NextBandWidth;
	unsigned int	NextBandDir;

	int		ShiftX;
	int		ShiftY;
	unsigned int	BandResY;
	unsigned int	StepDistance;

	

	unsigned short rev;
	unsigned short HeaderCrc;
};

#endif
