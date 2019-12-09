#ifndef __COLLECTION__
#define __COLLECTION__

#include <Parallels.h>
#include <string>

namespace Collection
{
namespace String
{

struct List
{

List();

~List();

PRL_HANDLE get() const;

PRL_HANDLE_PTR get_ptr();

PRL_UINT32 count() const;

void show() const;

PRL_RESULT add(const std::string& src);

private:
	PRL_HANDLE m_handle;
};

} // namespace String
} // namespace Collection

#endif

