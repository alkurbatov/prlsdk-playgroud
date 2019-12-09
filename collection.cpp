#include <iostream>
#include "collection.h"

namespace Collection
{
namespace String
{

List::List()
{
	//FIXME: what if it fails?
	PrlApi_CreateStringsList(&m_handle);
}

List::~List()
{
	PrlHandle_Free(m_handle);
}

PRL_HANDLE List::get() const
{
	return m_handle;
}

PRL_HANDLE_PTR List::get_ptr()
{
	return &m_handle;
}

PRL_UINT32 List::count() const
{
	PRL_UINT32 c = 0;

	//FIXME: log fail somehow?
	PrlStrList_GetItemsCount(m_handle, &c);

	return c;
}

void List::show() const
{
	PRL_UINT32 c = this->count();

	for (int i = 0; i < c; ++i)
	{
		PRL_CHAR b[256];
		PRL_UINT32 s = sizeof(b);

		PRL_RESULT e = PrlStrList_GetItem(m_handle, i, b, &s);
		if (PRL_FAILED(e))
			//FIXME: log me?
			continue;

		std::cout << i << ": " <<  b << std::endl;
	}
}

PRL_RESULT List::add(const std::string& src_)
{
	return PrlStrList_AddItem(m_handle, src_.c_str());
}

} // namespace String
} // namespace Collection

