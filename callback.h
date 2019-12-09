#ifndef __CALLBACK__
#define __CALLBACK__

#include <boost/utility.hpp>

#include <Parallels.h>

namespace Spy
{
struct Data
{
	std::string src;
	unsigned counter;
	std::vector<std::string> perf_filter;
};

struct Server: boost::noncopyable
{
	explicit Server(const PRL_HANDLE& src_);
	~Server();

private:
	PRL_HANDLE m_src;
	Data m_data;
};

struct Vm: boost::noncopyable
{
	explicit Vm(const PRL_HANDLE& src_, bool perf_);
	~Vm();

	void addPerfFilter(const std::string& filter_);

private:
	PRL_HANDLE m_src;
	bool m_perf;

	Data m_data;
};

} // namespace Spy

#endif
