#include <stdio.h>

///////////////////////////////////////  list.h  ////////////////////////////

#include <stddef.h>  // for size_t
#include <utility>   // for std::move

namespace leanstl
{

struct list_node_base
{
  list_node_base* next;
  list_node_base* prev;
};

template <typename T>
class list
{
 public:
  list()
  {
    head_.next = &head_;
    head_.prev = &head_;
  }

  ~list()
  {
    for (list_node_base* n = head_.next; n != &head_;)
    {
      list_node* tod = static_cast<list_node*>(n);
      n = n->next;
      delete tod;
    }
  }

  list(const list& rhs) = delete;
  void operator=(const list& rhs) = delete;

  void push_front(T x)
  {
    list_node* n = new list_node(std::move(x));
    n->next = head_.next;
    n->next->prev = n;
    head_.next = n;
    n->prev = &head_;
    size_++;
  }

  T& front()
  {
    return static_cast<list_node*>(head_.next)->value_;
  }

 private:
  struct list_node : list_node_base
  {
    T value_;
    list_node(T&& x) : value_(std::move(x)) {}
  };

  list_node_base head_;
  size_t size_ = 0;
};

}  // namespace leanstl

///////////////////////////////////////  main()  ////////////////////////////

#include <string>

int main()
{
  leanstl::list<int> li;
  li.push_front(43);
  printf("front=%d\n", li.front());
  li.push_front(82);
  printf("front=%d\n", li.front());

  leanstl::list<std::string> ls;
  ls.push_front("hello");
  printf("front=%s\n", ls.front().c_str());
}
