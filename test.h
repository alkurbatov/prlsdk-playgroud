#ifndef __TEST__
#define __TEST__

#include <string>
#include <iostream>

#include "server.h"
#include "vm.h"
#include "consts.h"

#include <Parallels.h>

namespace Test
{
namespace Display
{

void connect_on_stopped(PRL_HANDLE ve_);

} // namespace Display

namespace Exec
{

void basic(Server* srv_, const std::string& target_);
void multythread(Server* srv_, const std::string& target_);

} //namespace Exec

namespace Stat
{

template <class TSource>
int show(const TSource& src_)
{
	::Stat::Unit* g = NULL;
	if (PRL_FAILED(src_.statistics(&g)))
		return -2;

	std::cout << std::endl << std::endl;
	std::cout << "Got statistics handle!" << std::endl;

	g->uptime();
	g->ram();
	g->swap();
	g->cpu();
	g->iface();

	delete g;

	return 0;
}

} // namespace Stat

namespace Cfg
{
namespace DevNodes
{

bool fail_get_on_vm(const Server& srv_, const std::string& src_);

int get(Vm& ve_);

int set(Vm& ve_);

} // namespace DevNodes
} // namespace Cfg

} // namespace Test

#endif

