#ifndef __STAT__
#define __STAT__

#include <Parallels.h>

#include "prototype.h"

namespace Stat
{

struct Unit: Prototype::Base
{

	Unit(const PRL_HANDLE& stat_);

	void uptime() const;

	void ram() const;

	void swap() const;

	void cpu() const;

	void iface() const;

};

} // namespace Stat

#endif

