#include <conio.h>
#include <iostream>
#include <string>
using namespace std;

#define FilePrefix "/"
#define FileSuffix "/logger.log"

class Logger
{
public:
	Logger();
	~Logger();

private:

	string Filepath;

};

Logger::Logger()
{


}

Logger::~Logger()
{
}