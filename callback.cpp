#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "callback.h"
#include "job.h"

namespace
{

struct Time
{
static std::string now()
{
	const boost::posix_time::time_duration t =
			boost::posix_time::microsec_clock::local_time().time_of_day();

	const long ms = t.total_milliseconds() -
	((t.hours() * 3600 + t.minutes() * 60 + t.seconds()) * 1000);

	return (boost::format("%1%:%2%:%3%.%4% ") %
			t.hours() % t.minutes() % t.seconds() % ms).str();
}

};

PRL_RESULT print(PRL_HANDLE src_, void *data_)
{
	PRL_HANDLE_TYPE t;
	Spy::Data* d = static_cast <Spy::Data*> (data_);

	PRL_RESULT r = PrlHandle_GetType(src_, &t);
	if (PRL_FAILED(r)) {
		std::cerr << Time::now() << d->src << ": Wtf?!" << std::endl;
		return PRL_ERR_FAILURE;
	}

	if(t == PHT_JOB) {
		std::cout << Time::now() << d->src << " #" << ++(d->counter);

		const Job::Unit* u = Job::history.get(src_);
		if (u == NULL)
		{
			std::cout << ": Unknown Job!" << std::endl;

			PRL_RESULT r;
			PrlJob_GetRetCode(src_, &r);

			std::cout << "RetCode: " << r << std::endl;

			return PRL_ERR_SUCCESS;
		}

		std::cout << ": Job " << u->name << std::endl;
		if (!u->m_descr.empty())
			std::cout << "Descr: " << u->m_descr << std::endl;

		std::cout << "RetCode: " << u->get_ret_code() << std::endl;

		return PRL_ERR_SUCCESS;
	}

	PRL_EVENT_TYPE v;
	r = PrlEvent_GetType(src_, &v);
	if (PRL_FAILED(r)) {
		std::cerr << Time::now() << d->src << ": Wtf?!" << std::endl;
		return PRL_ERR_FAILURE;
	}

	std::cout << Time::now() << d->src <<
		" #" << ++(d->counter) << ": Event, Type: " << event_type_to_string(v);

	switch(v) {
		case PET_JOB_FILE_COPY_PROGRESS_CHANGED:
		case PET_DSP_EVT_VM_ENCRYPT_PROGRESS_CHANGED:
		case PET_DSP_EVT_VM_DECRYPT_PROGRESS_CHANGED:
		{
			unsigned e;
			PRL_HANDLE p;

			//FIXME: error check.
			PrlEvent_GetParam(src_, 0, &p);

			//FIXME: error check.
			PrlEvtPrm_ToUint32(p, &e);

			std::string n = boost::lexical_cast<std::string>(e);
			std::cout << " - " << n;

			break;
		}

		case PET_VM_INF_END_BUNCH_COPYING:
			std::cout << " - 100%";
			break;

		case PET_VM_INF_START_BUNCH_COPYING:
			std::cout << " - Copying";
			break;

		case PET_DSP_EVT_VM_PERFSTATS:
			unsigned c = 0;
			r = PrlEvent_GetParamsCount(src_, &c);
			if (PRL_FAILED(r)) {
				std::cerr << Time::now() << d->src << ": Wtf?!" << std::endl;
				return PRL_ERR_FAILURE;
			}

			std::cout << std::endl;

			for (int i = 0; i < c; ++i) {
				PRL_HANDLE p;
				r = PrlEvent_GetParam(src_, i, &p);
				if (PRL_FAILED(r)) {
					std::cerr << Time::now() << d->src << ": Wtf?!" << std::endl;
					return PRL_ERR_FAILURE;
				}

				char b[1024];
				unsigned l = sizeof(b) - 1;
				//FIXME: check r!
				r = PrlEvtPrm_GetName(p, b, &l);

				std::vector<std::string>::const_iterator it;
				for (it = d->perf_filter.begin(); it != d->perf_filter.end(); ++it) {
					if (*it != b)
						continue;

					std::cout << " " << b;

					PRL_UINT64 v = 0;
					//FIXME: check r!
					r = PrlEvtPrm_ToUint64(p, &v);
					std::cout << ": " << v << std::endl;
					break;
				}
			}

			break;
	};

	std::cout << std::endl;

	return PRL_ERR_SUCCESS;
}

} // namespace

namespace Spy
{

Server::Server(const PRL_HANDLE& src_): m_src(src_)
{
	PrlHandle_AddRef(src_);

	m_data.src = "Server";
	m_data.counter = 0;

	PRL_RESULT e = PrlSrv_RegEventHandler(m_src, print, &m_data);
	if (PRL_FAILED(e))
		std::cerr << "Wtf?!" << std::endl;
}

Server::~Server()
{
	//FIXME: what if we will got fail?
	PrlSrv_UnregEventHandler(m_src, print, &m_data);
}

Vm::Vm(const PRL_HANDLE& src_, bool perf_):
	m_src(src_), m_perf(perf_)
{
	PrlHandle_AddRef(src_);

	m_data.src = "Vm";
	m_data.counter = 0;

	PRL_RESULT e = PrlVm_RegEventHandler(m_src, print, &m_data);
	if (PRL_FAILED(e))
		std::cerr << "Wtf?!" << std::endl;

	if (m_perf)
		PrlVm_SubscribeToPerfStats(m_src, "*");
}

Vm::~Vm()
{
	//FIXME: what if we will got fail?
	PrlVm_UnregEventHandler(m_src, print, &m_data);

	if (m_perf)
		PrlVm_UnsubscribeFromPerfStats(m_src);
}

void Vm::addPerfFilter(const std::string& filter_)
{
	m_data.perf_filter.push_back(filter_);
}

} //namespace Spy

