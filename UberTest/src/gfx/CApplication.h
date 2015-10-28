#ifndef CAPPLICATION
#define CAPPLICATION

#include <types_v7.h>

struct SApplicationPrivate;

class CDemo;

class CApplication
{
public:
	CApplication();
	~CApplication();

	bool initialize(int32 agrc, const char ** argv);
	void run(CDemo * demo);
	void shutdown();

private:

	SApplicationPrivate& mPrivate;

};

#endif





