#ifndef __JOB__
#define __JOB__

#include <vector>
#include <string>

#include <boost/utility.hpp>

#include "prototype.h"

#include <Parallels.h>
#include <PrlEnums.h>

namespace Job
{
namespace Convert
{

std::string toString(PRL_JOB_STATUS src_);

} // namespace Convert

struct Unit: Prototype::Base
{
	Unit(PRL_HANDLE handle_, const std::string& name_, const std::string& descr_ = "");

	PRL_RESULT get_ret_code() const;

	std::string name;
};

struct List: boost::noncopyable
{
	List();

	~List();

	void add(const Unit& job_);

	const Unit* get(PRL_HANDLE handle_);

	void clear();

private:
	pthread_mutex_t m_mutex;
	std::vector<Unit> m_jobs;
};

extern List history;

} // namespace Job

#endif

