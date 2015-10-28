#ifndef __LOG_V7__
#define __LOG_V7__

#include "types_v7.h"

typedef enum _E_LOG_RECORD_TYPE {
	E_LOG_COMMENT = 1,
	E_LOG_INFO = 2,
	E_LOG_WARNING = 3,
	E_LOG_CRASH = 4,
	E_LOG_EMPTY = 0
} E_LOG_RECORD_TYPE;

FUNC_DEF_EXTERN void FUNC_CALL_TYPE logAllowRec(int32 allowLog);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE logOpen(const char * fileName);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE logRec(E_LOG_RECORD_TYPE type, const char * format, ...);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE logRecLine(E_LOG_RECORD_TYPE type, const char * format, ...);
FUNC_DEF_EXTERN void FUNC_CALL_TYPE logClose();



#endif/*__LOG__*/