#include "Logging.h"
#include <Windows.h>

Logging::Logging(const std::string& filename)
{
	logFile.open(filename);
	if (!logFile.is_open())
	{
		std::cerr << "Error: Can't open log file." << std::endl;
	}
}

Logging::~Logging()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}

Logging& Logging::GetInstance(const std::string& filename)
{
	static Logging instance(filename);
	return instance;
}

/* I used chatgpt for this bc idk how the fuck the ,... argument works! funky number parameters. */
void Logging::Log(const char* format, ...)
{
	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	logFile << buffer;
	logFile.flush();

	va_end(args);

	OutputDebugStringA(buffer);

}

void Logging::LogLine(const char* format, ...)
{
	if (!logFile.is_open())
	{
		return;
	}

	va_list args;
	va_start(args, format);

	char buffer[4096];
	vsnprintf(buffer, sizeof(buffer), format, args);

	logFile << buffer << std::endl;
	logFile.flush();

	va_end(args);

	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
}