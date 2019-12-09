#include <time.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "vm.h"
#include "job.h"
#include "collection.h"

Vm::Vm(const PRL_HANDLE& vm_, Mode mode_):
	Prototype::Workflow(vm_, mode_)
{
}

const PRL_HANDLE& Vm::get() const
{
	return m_handle;
}

PRL_RESULT Vm::start() const
{
	PRL_HANDLE j = PrlVm_Start(m_handle);
	Job::history.add( Job::Unit(j, "VM_START_JOB", m_descr) );

	if (m_mode == SYNC)
		return PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Vm::stop() const
{
	PRL_HANDLE j = PrlVm_Stop(m_handle, PRL_FALSE);
	Job::history.add( Job::Unit(j, "VM_STOP_JOB", m_descr) );

	if (m_mode == SYNC)
		return PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Vm::clone()
{
	srand(time(NULL));
	std::string n = boost::lexical_cast<std::string>(rand() % 1000 + 1);

	PRL_HANDLE j = PrlVm_Clone(m_handle, std::string("Clone-" + n).c_str(), "", PRL_FALSE);
	Job::history.add( Job::Unit(j, "VM_CLONE_JOB", m_descr) );

	if (m_mode == SYNC)
		return PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Vm::login(const std::string& user_, const std::string& pwd_,
		Session::Unit** dst_) const
{
	PRL_HANDLE j = PrlVm_LoginInGuest(m_handle, user_.c_str(), pwd_.c_str(), 0);
	Job::history.add( Job::Unit(j, "VM_LOGIN_IN_GUEST_JOB", m_descr) );

	if (m_mode == SYNC) {
		//FIXME: check error!
		PrlJob_Wait(j, FOREVER);

		PRL_HANDLE r;
		PrlJob_GetResult(j, &r);

		PRL_HANDLE s;
		PrlResult_GetParam(r, &s);

		*dst_ = new Session::Unit(s, m_mode);
	}

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Vm::connect_display() const
{
	return PRL_ERR_UNIMPLEMENTED;
}

PRL_RESULT Vm::disconnect_display() const
{
	return PRL_ERR_UNIMPLEMENTED;
}

PRL_RESULT Vm::press(int keycode_, bool down_) const
{
	return PRL_ERR_UNIMPLEMENTED;
}

PRL_RESULT Vm::disable_network() const
{
	return do_network(PRL_FALSE);
}

PRL_RESULT Vm::enable_network() const
{
	return do_network(PRL_TRUE);
}

PRL_RESULT Vm::capture_screen_to_file(const std::string& name, PRL_IMAGE_FORMAT format_) const
{
	return PRL_ERR_UNIMPLEMENTED;
}

PRL_RESULT Vm::capture_screen_to_buffer(PRL_IMAGE_FORMAT format_) const
{
	return PRL_ERR_UNIMPLEMENTED;
}

PRL_RESULT Vm::do_network(PRL_BOOL enable_) const
{
	PRL_UINT32 c = 0;

	PRL_RESULT e = PrlVmCfg_GetNetAdaptersCount(m_handle, &c);
	if (PRL_FAILED(e))
		return e;

	if (c == 0)
		return PRL_ERR_SUCCESS;

	PRL_HANDLE j = PrlVm_BeginEdit(m_handle);
	PrlJob_Wait(j, 10000);

	for (int i = 0; i < c; ++i) {
		PRL_HANDLE a;

		e = PrlVmCfg_GetNetAdapter(m_handle, i, &a);
		if (PRL_FAILED(e))
			return e;

		if (PRL_FAILED(PrlVmDev_SetConnected(a, enable_)))
			return e;
	}

	j = PrlVm_Commit(m_handle);

	if (m_mode == SYNC)
		return PrlJob_Wait(j, 10000);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Vm::statistics(Stat::Unit** dst_) const
{
	PRL_HANDLE j = PrlVm_GetStatistics(m_handle);
	Job::history.add( Job::Unit(j, "VM_GET_STATISTICS_JOB", m_descr) );

	if (m_mode == SYNC) {
		//FIXME: check error!
		PrlJob_Wait(j, FOREVER);

		PRL_HANDLE r;
		PrlJob_GetResult(j, &r);

		PRL_HANDLE s;
		PrlResult_GetParam(r, &s);

		*dst_ = new Stat::Unit(s);
	}

	return PRL_ERR_SUCCESS;
}

Cfg::Cfg(const PRL_HANDLE& vm_, Mode mode_):
	Prototype::Workflow(vm_, mode_)
{
}

PRL_RESULT Cfg::begin_edit()
{
	PRL_HANDLE j = PrlVm_BeginEdit(m_handle);
	Job::history.add( Job::Unit(j, "VM_BEGIN_EDIT_JOB") );

	if (m_mode == ASYNC)
		return PRL_ERR_UNIMPLEMENTED;

	//FIXME: check error!
	PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Cfg::commit()
{
	PRL_HANDLE j = PrlVm_Commit(m_handle);
	Job::history.add( Job::Unit(j, "VM_COMMIT_JOB") );

	if (m_mode == ASYNC)
		return PRL_ERR_UNIMPLEMENTED;

	//FIXME: check error!
	PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

