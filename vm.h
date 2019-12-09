#ifndef __VM__
#define __VM__

#include <string>

#include "prototype.h"
#include "consts.h"
#include "session.h"
#include "stat.h"

#include <Parallels.h>

namespace Collection
{
namespace String
{

struct List;

} // namespace String
} // namespace Collection

struct Vm: Prototype::Workflow
{
	Vm(const PRL_HANDLE& vm_, Mode mode_);

	const PRL_HANDLE& get() const;

	PRL_RESULT start() const;
	PRL_RESULT stop() const;

	PRL_RESULT clone();

	PRL_RESULT login(const std::string& user_, const std::string& pwd_, Session::Unit** dst_) const;

	PRL_RESULT connect_display() const;
	PRL_RESULT disconnect_display() const;

	PRL_RESULT press(int keycode_, bool down_) const;

	PRL_RESULT disable_network() const;
	PRL_RESULT enable_network() const;

	PRL_RESULT capture_screen_to_file(const std::string& name, PRL_IMAGE_FORMAT format_) const;
	PRL_RESULT capture_screen_to_buffer(PRL_IMAGE_FORMAT format_) const;

	PRL_RESULT statistics(Stat::Unit** dst_) const;

private:
	PRL_RESULT do_network(PRL_BOOL enable_) const;
};

struct Cfg: Prototype::Workflow
{
	Cfg(const PRL_HANDLE& vm_, Mode mode_);

	PRL_RESULT begin_edit();

	PRL_RESULT commit();
};

#endif

