#include "session.h"

#include "job.h"

namespace Session
{

Unit::Unit(PRL_HANDLE handle_, Mode mode_):
	Prototype::Workflow(handle_, mode_)
{}

PRL_RESULT Unit::run(const std::string& command_) const
{
	PRL_HANDLE j = PrlVmGuest_RunProgram(m_handle, command_.c_str(), NULL, NULL,
		PFD_ALL | PRPM_RUN_PROGRAM_IN_SHELL, 0, 1, 2);
	Job::history.add( Job::Unit(j, "VM_EXEC_JOB " + command_, m_descr) );

	if (m_mode == SYNC)
		return PrlJob_Wait(j, FOREVER);

	return PRL_ERR_SUCCESS;
}

PRL_RESULT Unit::logout() const
{
	PRL_HANDLE j = PrlVmGuest_Logout(m_handle, 0);
	Job::history.add( Job::Unit(j, "VM_LOGOUT_JOB ", m_descr) );

	if (m_mode == SYNC)
		return PrlJob_Wait(j, 3000);

	return PRL_ERR_SUCCESS;
}

} // namespace Session

