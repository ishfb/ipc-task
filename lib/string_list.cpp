#include "string_list.h"

#include "log.h"

StringList::StringList(std::span<char> arena) : StringListView(arena) {
  begin_ = end_ = back_ = string_allocator_.begin().index_;
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
    string_allocator_.Deallocate(std::distance(old_begin.Base(), begin.Base()));
  }

  begin_ = begin.Base().index_;

  auto string_end = std::copy(new_string.begin(), new_string.end(), range->begin());
  *string_end = '\0';
  ++string_end;

  back_ = range->begin().index_;

  if (begin_ == end_) {
    begin_ = back_;
  }
  end_ = string_end.index_;
}
