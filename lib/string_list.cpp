#include "string_list.h"

#include "log.h"

StringListView::StringListView(std::span<char> arena)
    : nodes_allocator_(arena),
      begin_(*nodes_allocator_.AllocateFor(&begin_)),
      end_(*nodes_allocator_.AllocateFor(&end_)),
      back_(*nodes_allocator_.AllocateFor(&back_)),
      string_allocator_(nodes_allocator_.FreeSpace()) {}

StringList::StringList(std::span<char> arena) : StringListView(arena) {
  begin_ = end_ = back_ = string_allocator_.begin().index_;
}

std::ostream& operator<<(std::ostream& lhs, const RingBufferString& rhs) {
  for (auto x : rhs) {
    lhs << static_cast<char>(x);
  }
  return lhs;
}

void StringList::Add(std::string_view new_string) {
  if (new_string.size() > string_allocator_.Capacity()) {
    LOG("no space for " << new_string);
    return;
  }

  auto begin = this->begin();

  std::optional<Range<RingBufferAllocator::Iterator>> range;
  while (!(range = string_allocator_.Allocate(new_string.size() + 1))) {
    auto old_begin = begin++;
    string_allocator_.Deallocate(std::distance(old_begin, begin));
  }

  auto string_end = std::copy(new_string.begin(), new_string.end(), range->begin());
  *string_end = '\0';
  ++string_end;

  back_ = range->begin().index_;

  if (begin_ == end_) {
    begin_ = back_;
  }
  end_ = string_end.index_;
}
