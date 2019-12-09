#include <iostream>

#include "stat.h"

namespace Stat
{

Unit::Unit(const PRL_HANDLE& stat_): Prototype::Base(stat_)
{
}

void Unit::uptime() const
{
	PRL_UINT64 t = 0;
	if (PRL_SUCCEEDED(PrlStat_GetOsUptime(m_handle, &t)))
		std::cout << "OS uptime: " << t / 60 << " minutes"<< std::endl;

	t = 0;
	if (PRL_SUCCEEDED(PrlStat_GetDispUptime(m_handle, &t)))
		std::cout << "Disp uptime: " << t / 60 << " minutes"<< std::endl;
}

void Unit::ram() const
{
	std::cout << "Ram" << std::endl;

	PRL_UINT64 s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetTotalRamSize(m_handle, &s)))
		std::cout << "Ram Total: " << s << std::endl;

	s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetUsageRamSize(m_handle, &s)))
		std::cout << "Ram Usage: " << s << std::endl;

	s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetFreeRamSize(m_handle, &s)))
		std::cout << "Ram Free: " << s << std::endl;

	s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetRealRamSize(m_handle, &s)))
		std::cout << "Ram Real: " << s << std::endl;
}

void Unit::swap() const
{
	std::cout << "Swap" << std::endl;

	PRL_UINT64 s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetTotalSwapSize(m_handle, &s)))
		std::cout << "Swap Total: " << s << std::endl;

	s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetUsageSwapSize(m_handle, &s)))
		std::cout << "Swap Usage: " << s << std::endl;

	s = 0;
	if (PRL_SUCCEEDED(PrlStat_GetFreeSwapSize(m_handle, &s)))
		std::cout << "Swap Free: " << s << std::endl;
}

void Unit::cpu() const
{
	PRL_UINT32 c = 0;
	if (PRL_FAILED(PrlStat_GetCpusStatsCount(m_handle, &c)))
		return;

	std::cout << "CpuStat count: " << c << std::endl;

	for (PRL_UINT32 i = 0; i < c; ++i) {
		std::cout << "Cpu " << i << std::endl;

		PRL_HANDLE s = NULL;
		if (PRL_FAILED(PrlStat_GetCpuStat(m_handle, i, &s)))
			return;
		//FIXME: report error?

		PRL_UINT32 p = 0;
		if (PRL_SUCCEEDED(PrlStatCpu_GetCpuUsage(s, &p)))
			std::cout << "Usage: " << p << "%" << std::endl;

		PRL_UINT64 t = 0;
		if (PRL_SUCCEEDED(PrlStatCpu_GetTotalTime(s, &t)))
			std::cout << "Total time: " << t << std::endl;

		t = 0;
		if (PRL_SUCCEEDED(PrlStatCpu_GetUserTime(s, &t)))
			std::cout << "User time: " << t << std::endl;

		t = 0;
		if (PRL_SUCCEEDED(PrlStatCpu_GetSystemTime(s, &t)))
			std::cout << "System time: " << t << std::endl;

		PrlHandle_Free(s);
	}
}

void Unit::iface() const
{
	PRL_UINT32 c = 0;
	if (PRL_FAILED(PrlStat_GetIfacesStatsCount(m_handle, &c)))
		return;

	std::cout << "Ifaces count: " << c << std::endl;

	for (PRL_UINT32 i = 0; i < c; ++i) {
		std::cout << "Iface" << i << std::endl;

		PRL_HANDLE s = NULL;
		if (PRL_FAILED(PrlStat_GetIfaceStat(m_handle, i, &s)))
			return;
		//FIXME: report error?

		PRL_CHAR n[1024];
		PRL_UINT32 l = sizeof(n) - 1;;
		if (PRL_SUCCEEDED(PrlStatIface_GetSystemName(s, n, &l)))
			std::cout << "System name: " << n << std::endl;

		PRL_UINT64 t = 0;
		if (PRL_SUCCEEDED(PrlStatIface_GetInDataSize(m_handle, &t)))
			std::cout << "In data: " << t << std::endl;

		t = 0;
		if (PRL_SUCCEEDED(PrlStatIface_GetOutDataSize(m_handle, &t)))
			std::cout << "Out data: " << t << std::endl;

		t = 0;
		if (PRL_SUCCEEDED(PrlStatIface_GetInPkgsCount(m_handle, &t)))
			std::cout << "In Pkgs: " << t << std::endl;

		t = 0;
		if (PRL_SUCCEEDED(PrlStatIface_GetOutPkgsCount(m_handle, &t)))
			std::cout << "Out Pkgs: " << t << std::endl;
	}
}

} // namespace Stat

