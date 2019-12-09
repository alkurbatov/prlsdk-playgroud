#include "server.h"
#include "job.h"

Server* Server::connect(const std::string& ip_, const std::string& user_, const std::string& pwd_)
{
	PRL_HANDLE s;

	if (PRL_FAILED(PrlSrv_Create(&s)))
		return NULL;

	PRL_HANDLE j = PrlSrv_LoginEx(s,
		ip_.c_str(), user_.c_str(), pwd_.c_str(),
		0, 0, 0, PSL_HIGH_SECURITY, PACF_NON_INTERACTIVE_MODE);

	if (PRL_FAILED(PrlJob_Wait(j, 1000)))
		return NULL;

	return new Server(s);
}

//FIXME: actually it does not work
Server* Server::connectLocal()
{
	PRL_HANDLE s;

	if (PRL_FAILED(PrlSrv_Create(&s)))
		return NULL;

	PRL_HANDLE j = PrlSrv_LoginLocal(s, "", 0, PSL_HIGH_SECURITY);

	if (PRL_FAILED(PrlJob_Wait(j, 1000)))
		return NULL;

	return new Server(s);
}

Server::Server(const PRL_HANDLE& srv_): m_srv(srv_)
{}

Server::~Server()
{
	PrlHandle_Free(m_srv);
}

const PRL_HANDLE& Server::get() const
{
	return m_srv;
}

PRL_RESULT Server::find(const std::string& id_, PRL_HANDLE& dst_) const
{
	PRL_HANDLE r;
	PRL_RESULT e, f;
	PRL_HANDLE j = PrlSrv_GetVmConfig(m_srv, id_.c_str(), PGVC_SEARCH_BY_UUID | PGVC_SEARCH_BY_NAME);

	e = PrlJob_Wait(j, 10000);
	if (PRL_FAILED(e))
		return e;

	e = PrlJob_GetRetCode(j, &f);
	if (PRL_FAILED(e))
		return e;
	if (PRL_FAILED(f))
		return f;

	e = PrlJob_GetResult(j, &r);
	if (PRL_FAILED(e))
		return e;

	return PrlResult_GetParam(r, &dst_);
}

PRL_RESULT Server::statistics(Stat::Unit** dst_) const
{
	PRL_HANDLE j = PrlSrv_GetStatistics(m_srv);
	Job::history.add( Job::Unit(j, "SRV_GET_STATISTICS_JOB", "") );

	//FIXME: check error!
	PrlJob_Wait(j, FOREVER);

	PRL_HANDLE r;
	PrlJob_GetResult(j, &r);

	PRL_HANDLE s;
	PrlResult_GetParam(r, &s);

	*dst_ = new Stat::Unit(s);

	return PRL_ERR_SUCCESS;
}

