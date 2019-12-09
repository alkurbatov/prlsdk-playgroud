#ifndef __SERVER__
#define __SERVER__

#include <string>

#include <Parallels.h>

#include "stat.h"

struct Server
{
	static Server* connect(const std::string& ip_,
			const std::string& user_, const std::string& pwd_);

	static Server* connectLocal();

	~Server();

	const PRL_HANDLE& get() const;

	PRL_RESULT find(const std::string& id_, PRL_HANDLE& dst_) const;

	PRL_RESULT statistics(Stat::Unit** dst_) const;

private:
	Server(const PRL_HANDLE& srv_);

	PRL_HANDLE m_srv;
};

#endif

