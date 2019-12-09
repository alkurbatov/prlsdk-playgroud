#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>
#include <pthread.h>

#include "vm.h"
#include "job.h"
#include "server.h"
#include "callback.h"
#include "consts.h"
#include "test.h"

PRL_RESULT init()
{
	std::cout << "Do Init" << std::endl;

	return PrlApi_InitEx(PARALLELS_API_VER, PAM_SERVER, 0, 0);
}

void deinit()
{
	std::cout << "Do deinit" << std::endl;

	PrlApi_Deinit();
}

PRL_HANDLE Connect;

int do_test(char* argv[])
{
	std::cout << "Connecting..." << std::endl;

	std::auto_ptr<Server> s(Server::connect(argv[1], argv[2], argv[3]));
	if (s.get() == NULL)
		return -1;

	std::cout << "Connected!" << std::endl;

	std::cout << "Searching..." << std::endl;

	PRL_HANDLE v;
	if (PRL_FAILED(s->find(argv[4], v)))
		return -1;

	std::cout << "Found!" << std::endl;

	Vm a(v, SYNC);

//	Spy::Server ps(s->get());
//	Spy::Vm p(v, true);
//	p.addPerfFilter("guest.ram.usage");
//	p.addPerfFilter("guest.ram.cached");
//	p.addPerfFilter("ws.reclaimable");

/** Tests */

	std::cout << "Run tests!" << std::endl;

//	Test::Display::connect_on_stopped(v);
//	Test::Exec::basic(s.get(), argv[4]);
//	Test::Exec::multythread(s.get(), argv[4]);

//	Test::Stat::show<Server>(*s);
//	Test::Stat::show<Vm>(a);

	//FIXME: add a kind of wrap for tests?

	sleep(10000);

	PrlHandle_Free(v);
	return 0;
}

int main(int argc, char* argv[])
{
	if PRL_FAILED(init())
		return -1;

	do_test(argv);

	// Just an ugly workaround: some callback can be active.
	sleep(2);

	Job::history.clear();
	deinit();

	std::cout << "Done!" << std::endl;
	return 0;
}

