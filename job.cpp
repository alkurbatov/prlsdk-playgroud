#include <algorithm>

#include "job.h"

namespace
{
namespace Search
{

struct byHandle
{
	byHandle(PRL_HANDLE handle_): m_handle(handle_)
	{}

	bool operator() (const Job::Unit& src_) const
	{
		return src_.m_handle == m_handle;
	}

private:
	PRL_HANDLE m_handle;
};

} // namespace Search
} // namespace

namespace Convert
{

std::string toString(PRL_JOB_STATUS src_)
{
	if (src_ == PJS_UNKNOWN)
		return "PJS_UNKNOWN";

	if (src_ == PJS_RUNNING)
		return "PJS_RUNNING";

	if (src_ == PJS_FINISHED)
		return "PJS_FINISHED";

	return "";
}

} // namespace Convert


namespace Job
{

Unit::Unit(PRL_HANDLE handle_, const std::string& name_, const std::string& descr_):
	Base(handle_, descr_), name(name_)
{
}

PRL_RESULT Unit::get_ret_code() const
{
	PRL_RESULT r;

	PrlJob_GetRetCode(m_handle, &r);

	return r;
}

List::List()
{
	pthread_mutex_init(&m_mutex, NULL);
}

List::~List()
{
	pthread_mutex_destroy(&m_mutex);
}

void List::add(const Unit& job_)
{
	pthread_mutex_lock(&m_mutex);

	m_jobs.push_back(job_);

	pthread_mutex_unlock(&m_mutex);
}

const Unit* List::get(PRL_HANDLE handle_)
{
	pthread_mutex_lock(&m_mutex);
	const Unit* r = NULL;

	std::vector<Unit>::const_iterator i;

	i = std::find_if(m_jobs.begin(), m_jobs.end(), Search::byHandle(handle_));
	if (i != m_jobs.end())
		r = &(*i);

	pthread_mutex_unlock(&m_mutex);
	return r;
}

void List::clear()
{
	m_jobs.clear();
}

List history;

} // namespace Job

