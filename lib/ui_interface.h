#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#ifdef _WIN32
	#define	API_DLL		__declspec(dllexport)	
#else
	#define	API_DLL		__attribute__ ((visibility("default")))
#endif

#ifdef _WIN32
	#ifdef __cplusplus
		extern "C" {
	#endif
#endif


#include "Error.h"
#include "command.h"
#include "APIDataInterface.hpp"

extern int CreatePrinter(char* path);
extern int DestroyPrinter(void *handle);

extern int PrintPause();
extern int PrintAbort();
extern int PrintImage(const char * file, PRINT_MODER *mode, JOB_INFO * job);

extern int ClearPrinterStatus(unsigned int err);
extern int GetPrinterStatus(void *param);
extern int GetPrinterProperty(struct MECHAINE* property);

extern int SetPrinterParam(UI_CMD cmd, void* param);
extern int GetPrinterParam(UI_CMD cmd, void* param);

extern int SendMotionCmd(UI_CMD cmd, void* parament);

extern int UpdateCalibrationParam();

extern int PrintStepCalibration(const char * media, const char * model, int res);
extern int SaveStepCalibration(const char * media, const char * model, int opt, int value);
extern int LoadStepCalibration(const char * media, const char * model, int opt, int &value);

extern int PrintCalibration(UI_CMD cmd, int option, int index, int type);
extern int LoadCalibrationParam(UI_CMD cmd, int res, int speed, int *data);
extern int SaveCalibrationParam(UI_CMD cmd, int res, int speed, int *data, int len);

extern int LoadPrintResList(int* print_res);
extern int AddPrintRes(const int print_res);

extern int LoadMediaList(char * list);
extern int AddMedia(const char* name);
extern int DeleteMedia(const char* name);
extern int SelectMedia(const char* name);
extern int GetCurrentMedia(char* name);


extern int LoadProductModels(char * models);
extern int AddProductModel(const char * model, struct PRODUCT_MODEL* param);
extern int DeleteProductModel(const char* model);
extern int EditProductModel(const char * model, struct PRODUCT_MODEL* param);
extern int SelectProductModel(const char* model);
extern int GetCurrentModel(char * model);
extern int GetProductModel(const char * model, struct PRODUCT_MODEL * param);

#ifdef _WIN32
	#ifdef __cplusplus
		}
	#endif
#endif

#endif


