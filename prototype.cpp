#include "prototype.h"

namespace Prototype {

Base::Base(PRL_HANDLE handle_, const std::string& descr_):
	m_handle(handle_), m_descr(descr_)
{}

Base::~Base()
{
	PrlHandle_Free(m_handle);
}

Base::Base(const Base& src_)
{
	if (this == &src_)
		return;

	m_handle = src_.m_handle;
	PrlHandle_AddRef(m_handle);

	m_descr = src_.m_descr;
}

PRL_RESULT Base::add_ref()
{
	//FIXME: what if it fails?
	return PrlHandle_AddRef(m_handle);
}

void Base::set_mark(const std::string& descr_)
{
	m_descr = descr_;
}

Workflow::Workflow(PRL_HANDLE handle_, Mode mode_, const std::string& descr_):
	Base(handle_, descr_), m_mode(mode_)
{
}

void Workflow::set_mode(Mode mode_)
{
	m_mode = mode_;
}

} // namespace Prototype

