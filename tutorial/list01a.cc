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

  // prevent deleting a pointer of list_node_base* which actually points to a list_node<T>
  // GCC had bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54812 on this, fixed in 4.9.0
 protected:
  ~list_node_base() = default;
};

template <typename T>
struct list_node : list_node_base
{
  T value_;
  list_node(T&& x) : value_(std::move(x)) {}
};

template <typename T>
class list
{
 public:
  list()
    : head_(0)
  {
    head_.next = &head_;
    head_.prev = &head_;
  }

  ~list()
  {
    for (list_node_base* n = head_.next; n != &head_;)
    {
      list_node<T>* tod = static_cast<list_node<T>*>(n);
      n = n->next;
      delete tod;
    }
  }

  list(const list& rhs) = delete;
  void operator=(const list& rhs) = delete;

  void push_front(T x)
  {
    list_node<T>* n = new list_node<T>(std::move(x));
    n->next = head_.next;
    n->next->prev = n;
    head_.next = n;
    n->prev = &head_;
    head_.value_++;
  }

  T& front()
  {
    return static_cast<list_node<T>*>(head_.next)->value_;
  }

 private:
  list_node<size_t> head_;
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
