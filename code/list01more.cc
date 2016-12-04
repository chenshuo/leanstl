#include <stdio.h>

///////////////////////////////////////  list.h  ////////////////////////////

#include <assert.h>
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
    insert_node(head_.next, n);
    size_++;
  }

  void push_back(T x)
  {
    list_node* n = new list_node(std::move(x));
    insert_node(&head_, n);
    size_++;
  }

  T& front()
  {
    assert(!empty());
    return static_cast<list_node*>(head_.next)->value_;
  }

  T& back()
  {
    assert(!empty());
    return static_cast<list_node*>(head_.prev)->value_;
  }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

 private:
  struct list_node : list_node_base
  {
    T value_;
    // list_node(const T& x) : value_(x) {}
    list_node(T&& x) : value_(std::move(x)) {}
  };

  void insert_node(list_node_base* pos, list_node* n)
  {
    n->next = pos;
    n->prev = pos->prev;
    pos->prev->next = n;
    pos->prev = n;
  }

  list_node_base head_;
  size_t size_ = 0;
};

}  // namespace leanstl

///////////////////////////////////////  main()  ////////////////////////////

class Copyable
{
 public:
  Copyable() { printf("Copyable::ctor %p\n", this); }
  ~Copyable() { printf("Copyable::dtor %p\n", this); }
  Copyable(const Copyable&) { printf("Copyable::copy ctor %p\n", this); }
  // Copyable(Copyable&&) = delete;
};

class Moveable
{
 public:
  Moveable() { printf("Moveable::ctor %p\n", this); }
  ~Moveable() { printf("Moveable::dtor %p\n", this); }
  Moveable(Moveable&&) { printf("Moveable::move ctor %p\n", this); }
  Moveable(const Moveable&) = delete;
};

int main()
{
  leanstl::list<int> li;
  li.push_front(43);
  printf("front=%d, back=%d, size=%zd\n", li.front(), li.back(), li.size());
  li.push_front(82);
  printf("front=%d, back=%d, size=%zd\n", li.front(), li.back(), li.size());
  li.push_back(19);
  printf("front=%d, back=%d, size=%zd\n", li.front(), li.back(), li.size());

  leanstl::list<Copyable> lc;
  lc.push_front(Copyable());

  leanstl::list<Moveable> lm;
  // lm.push_front(Moveable());

  Moveable m;
  lm.push_front(std::move(m));

  /*
  leanstl::list<std::unique_ptr<double>> lu;
  lu.push_front(std::unique_ptr<double>(new double(5.0)));
  printf("front=%f\n", *lu.front());
  */
}
