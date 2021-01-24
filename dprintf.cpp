#include <stdarg.h>
#include <stdio.h>
#include <openvr_driver.h>
#include <windows.h>

// disable vc fopen warning
#pragma warning(disable : 4996)  

void dprintf(const char *fmt, ...)
{
	va_list args;
	char buffer[2048];

	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	static FILE *factory_log;
	if (factory_log == 0)
	{
		factory_log = fopen("c:\\temp\\input_driver_shim_log.txt", "wt");
		if (!factory_log)
		{
			factory_log = fopen("c:\\temp\\input_driver_shim.txt", "wt");
		}
	}

	if (factory_log)
	{
		fprintf(factory_log, "%s", buffer);
		fflush(factory_log);
	}

	OutputDebugStringA(buffer);
}