#include "string_list.h"

#include "log.h"

StringListBase::StringListBase(std::span<char> arena)
    : nodes_allocator_(arena),
      begin_(nodes_allocator_.AllocateFor(begin_)),
      end_(nodes_allocator_.AllocateFor(end_)),
      back_(nodes_allocator_.AllocateFor(back_)) {}

StringList::StringList(std::span<char> arena) : StringListBase(arena), string_allocator_(nodes_allocator_.FreeSpace()) {
  *begin_ = *end_ = *back_ = {string_allocator_.begin(), string_allocator_.begin()};
}

std::ostream& operator<<(std::ostream& lhs, const RingBufferString& rhs) {
  for (auto x : rhs) {
    lhs << static_cast<char>(x);
  }
  return lhs;
}

void StringList::Add(std::string new_string) {
  if (new_string.size() > string_allocator_.Capacity()) {
    LOG("no space for " << new_string);
    return;
  }

  std::optional<Range<RingBufferAllocator::iterator>> range;
  while (!(range = string_allocator_.Allocate(new_string.size() + 1))) {
    string_allocator_.Deallocate({begin_->first, begin_->last});
    ++begin_;
  }

  *std::copy(new_string.begin(), new_string.end(), range->begin()) = char('\0');

  back_->first = range->begin();
  back_->last = range->end();

  if (*begin_ == *end_) {
    *begin_ = *back_;
  }
  end_->first = end_->last = back_->last;
}
