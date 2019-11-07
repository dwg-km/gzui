
#ifndef		_ERROR_
#define		_ERROR_

//
/**************************************
 *     7   6   5   4      3   2   1   0
 *     
 * BIT[7:4]: system status.
 *			
 * Bit[3]: Error Source. 0.Software; 1.Firmware;
 * Bit[2:0]
*/
#define		DOMAIN_DLL			(0x00 | 0x01)
#define		DOMAIN_LIB			(0x00 | 0x02)

#define		DOMAIN_FW			0x08
#define		DOMAIN_CB_ARM		(DOMAIN_FW | 0x01)
#define		DOMAIN_CB_FPGA		(DOMAIN_FW | 0x02)

#define		DOMAIN_MT_DSP		(DOMAIN_FW | 0x03)

#define		DOMAIN_HB_ARM		(DOMAIN_FW | 0x04)
#define		DOMAIN_HB_FPGA		(DOMAIN_FW | 0x05)

#define		DOMAIN_EXT			(DOMAIN_FW | 0x06)

/*
#define		SUCCSEED			0x00000000
#define		ERROR				0x80000000



//
#define		SUCCESS				0
#define		RET_ERROR			-1
#define		NULL_POINTER		1

*/


#endif
