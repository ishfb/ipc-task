#include "string_list.h"

#include "log.h"

StringList::StringList(std::span<char> arena) : StringListView(arena) {
  begin_ = string_allocator_.begin().index_;
  end_ = string_allocator_.begin().index_;
  back_ = string_allocator_.begin().index_;
  LOG("StringList: available memory " << string_allocator_.Capacity());
}

void StringList::Add(std::string_view new_string) {
  if (new_string.size() >= string_allocator_.Capacity()) {
    LOG("no space for " << new_string << ". Capacity = " << string_allocator_.Capacity()
                        << ", string size = " << new_string.size());
    return;
  }

  auto begin = this->begin();

  std::optional<Range<RingBufferAllocator::Iterator>> range;
  const size_t bytes_to_allocate = new_string.size() + 1;
  while (!(range = string_allocator_.Allocate(bytes_to_allocate))) {
    auto old_begin = begin++;
    const size_t bytes_to_deallocate = std::distance(old_begin.Base(), begin.Base());
    LOG("Failed to allocate " << bytes_to_allocate << " bytes. Deallocating " << bytes_to_deallocate << " bytes");
    string_allocator_.Deallocate(bytes_to_deallocate);
  }
  LOG("Allocated " << bytes_to_allocate << " bytes, free space: " << string_allocator_.FreeSpace());

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
