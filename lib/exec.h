#ifndef		_EXEC_H_
#define		_EXEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		REBOOT		0
#define		SHOTDOWN	1
#define		RESUME_BAK	2

extern int exec_cmd(int cmd);

#ifdef __cplusplus
}
#endif

#endif
