#ifndef __PROTOTYPE__
#define __PROTOTYPE__

#include <string>

#include "consts.h"

#include <Parallels.h>

namespace Prototype
{

struct Base
{
	explicit Base(PRL_HANDLE handle_, const std::string& descr_ = "");
	Base(const Base& src_);

	~Base();

PRL_RESULT add_ref();

	void set_mark(const std::string& descr_);

	PRL_HANDLE m_handle;
	std::string m_descr;
};

struct Workflow: Base
{
	Workflow(PRL_HANDLE handle_, Mode mode_, const std::string& descr_ = "");

	void set_mode(Mode mode_);

	Mode m_mode;
};

} // namespace Prototype

#endif

