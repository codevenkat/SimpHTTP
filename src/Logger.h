// this is made as header-only because of the use of templates
#pragma once
#include <iostream>

class Logger
{
public:
	Logger(const char* filename)
	{
		m_fname = filename;
	}

	template<typename T>
	void log(int lvl, T logMsg, int line)
	{
		// format - [LEVEL] msg at line <line> of file <filename>\n
		std::string lvlStr;
		if (lvl == 1)
			lvlStr = "INFO";
		else if (lvl == 2)
			lvlStr = "WARN";
		else
			lvlStr = "ERROR";

		std::cout << "[" << lvlStr << "]" << " " << logMsg << " at line " << line << " of file " << m_fname << std::endl;
	}

private:
	const char* m_fname;
};
