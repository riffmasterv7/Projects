
#include "log_record.h"

#include <malloc.h>
#include <string.h>
#include <stdarg.h>

FILE * g_report = 0;
int32 g_allow = 0;

void FUNC_CALL_TYPE logAllowRec(int32 allowLog) {
	g_allow = allowLog;
}

void FUNC_CALL_TYPE logOpen(const char * fileName){
	if (fileName == 0) {
		g_allow = 0;

		fopen_s(&g_report, fileName, "w");

		if (g_report == 0) {
			g_allow = 0;
		}
	}
}

void FUNC_CALL_TYPE logRec(E_LOG_RECORD_TYPE type, const char * format, ...) {
	char * text = 0;

	if (g_allow && g_report) {
		va_list argPtr;
		va_start(argPtr,format);

		switch (type) {
			case E_LOG_COMMENT:
				text = "[COMMENT] ";
				break;
			case E_LOG_INFO:
				text = "[INFO] ";
				break;
			case E_LOG_WARNING:
				text = "[WARNING] ";
				break;
			case E_LOG_CRASH:
				text = "[ERROR] ";
				break;
			case E_LOG_EMPTY:
			default:
				text = "";
				break;
		}

		fputs(text, g_report);
		vfprintf(g_report, format, argPtr);
	}
}

void FUNC_CALL_TYPE logRecLine(E_LOG_RECORD_TYPE type, const char * format, ...) {
	char * text = 0;

	if (g_allow && g_report) {
		va_list argPtr;
		va_start(argPtr,format);

		switch (type) {
			case E_LOG_COMMENT:
				text = "[COMMENT] ";
				break;
			case E_LOG_INFO:
				text = "[INFO] ";
				break;
			case E_LOG_WARNING:
				text = "[WARNING] ";
				break;
			case E_LOG_CRASH:
				text = "[ERROR] ";
				break;
			case E_LOG_EMPTY:
			default:
				text = "";
				break;
		}

		fputs(text, g_report);
		vfprintf(g_report, format, argPtr);
		fputc('\n',g_report);
	}
}

void FUNC_CALL_TYPE logClose() {
	if (g_report != 0) {
		fclose(g_report);
		g_report = 0;
		g_allow = 0;
	}
}