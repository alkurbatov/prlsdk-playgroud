#ifndef __SESSION__
#define __SESSION__

#include <string>

#include "prototype.h"
#include "consts.h"

#include <Parallels.h>

namespace Session
{

struct Unit: Prototype::Workflow {
	Unit(PRL_HANDLE handle_, Mode mode_);

	PRL_RESULT run(const std::string& command_) const;

	PRL_RESULT logout() const;
};

} // namespace Session

#endif

