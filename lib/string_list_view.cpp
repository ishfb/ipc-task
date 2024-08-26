#include "string_list_view.h"

StringListView::StringListView(std::span<char> arena)
    : nodes_allocator_(arena),
      begin_(*nodes_allocator_.AllocateFor(&begin_)),
      end_(*nodes_allocator_.AllocateFor(&end_)),
      back_(*nodes_allocator_.AllocateFor(&back_)),
      string_allocator_(nodes_allocator_.FreeSpace()) {}

std::ostream& operator<<(std::ostream& lhs, const RingBufferString& rhs) {
  for (auto x : rhs) {
    lhs << static_cast<char>(x);
  }
  return lhs;
}

