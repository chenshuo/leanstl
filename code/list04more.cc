#include <stdio.h>

///////////////////////////////////////  list.h  ////////////////////////////

#include <assert.h>  // for size_t
#include <stddef.h>  // for size_t
#include <utility>   // for std::move and initializer_list

namespace leanstl
{

struct list_node_base
{
  list_node_base* next;
  list_node_base* prev;
};

class list_base
{
 protected:
  list_base()
  {
    init_head();
  }

  ~list_base()
  {
    if (size_ == 0)
    {
      assert(head_.next == &head_);
      assert(head_.prev == &head_);
    }
  }

  void init_head()
  {
    head_.next = &head_;
    head_.prev = &head_;
  }

  list_node_base head_;
  size_t size_ = 0;
};

template <typename T>
class list : private list_base
{
 public:
  list() {}

  ~list()
  {
    for (list_node_base* n = head_.next; n != &head_;)
    {
      list_node* tod = static_cast<list_node*>(n);
      n = n->next;
      delete tod;
    }
  }

  list(const list& rhs) : list(rhs.begin(), rhs.end()) {}
  list(list&& rhs) { take_over(rhs); }
  list& operator=(list rhs) { swap(rhs); return *this; }

  explicit list(size_t n)
  {
    for (size_t i = 0; i < n; ++i)
      insert_node(&head_, new list_node);
    size_ = n;
  }

  list(size_t n, const T& value)
  {
    for (size_t i = 0; i < n; ++i)
      push_back(value);
  }

  template <typename InputIterator>
  list(InputIterator first, InputIterator last)
  {
    initialize(first, last, typename std::is_integral<InputIterator>::type());
  }

  list(std::initializer_list<T> il) : list(il.begin(), il.end()) {}

  void swap(list& rhs)
  {
    list tmp(std::move(rhs));
    rhs.take_over(*this);
    take_over(tmp);
  }

  class iterator
  {
   public:
    iterator(list_node_base* n = nullptr) : node_(n) {}
    T& operator*() { return *operator->(); }
    T* operator->() { return &static_cast<list_node*>(node_)->value_; }
    iterator& operator++() { node_ = node_->next; return *this; }
    iterator operator++(int) { iterator old = *this; operator++(); return old; }
    iterator& operator--() { node_ = node_->prev; return *this; }
    iterator operator--(int) { iterator old = *this; operator--(); return old; }
    bool operator==(iterator rhs) const { return node_ == rhs.node_; }
    bool operator!=(iterator rhs) const { return node_ != rhs.node_; }

   private:
    list_node_base* node_;
  };

  class const_iterator
  {
   public:
    const_iterator(const list_node_base* n = nullptr) : node_(n) {}
    const T& operator*() { return *operator->(); }
    const T* operator->() { return &static_cast<const list_node*>(node_)->value_; }
    const_iterator& operator++() { node_ = node_->next; return *this; }
    const_iterator operator++(int) { const_iterator old = *this; operator++(); return old; }
    const_iterator& operator--() { node_ = node_->prev; return *this; }
    const_iterator operator--(int) { const_iterator old = *this; operator--(); return old; }
    bool operator==(const_iterator rhs) const { return node_ == rhs.node_; }
    bool operator!=(const_iterator rhs) const { return node_ != rhs.node_; }

   private:
    const list_node_base* node_;
  };
  // TODO: test operator==()

  iterator begin() { return iterator(head_.next); }
  const_iterator begin() const { return const_iterator(head_.next); }
  const_iterator cbegin() const { return const_iterator(head_.next); }
  iterator end() { return iterator(&head_); }
  const_iterator end() const { return const_iterator(&head_); }
  const_iterator cend() { return const_iterator(&head_); }

  void push_front(T x)
  {
    list_node* n = new list_node(std::move(x));
    insert_node(head_.next, n);
    size_++;
  }

  void push_back(const T& x)
  {
    list_node* n = new list_node(x);
    insert_node(&head_, n);
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
    list_node() : value_() {}
    list_node(const T& x) : value_(x) {}
    list_node(T&& x) : value_(std::move(x)) {}
  };

  void insert_node(list_node_base* pos, list_node* n)
  {
    n->next = pos;
    n->prev = pos->prev;
    pos->prev->next = n;
    pos->prev = n;
  }

  void take_over(list& rhs)
  {
    assert(size_ == 0);
    assert(head_.next == &head_);
    assert(head_.prev == &head_);

    if (rhs.size_ > 0)
    {
      head_.next = rhs.head_.next;
      head_.prev = rhs.head_.prev;
      head_.next->prev = &head_;
      head_.prev->next = &head_;
      rhs.init_head();
      size_ = rhs.size_;
      rhs.size_ = 0;
    }
  }

  template <typename InputIterator>
  void initialize(InputIterator first, InputIterator last, std::false_type)
  {
    for (; first != last; ++first)
      push_back(*first);
  }

  template <typename Integer>
  void initialize(Integer n, Integer value, std::true_type)
  {
    for (Integer i = 0; i < n; ++i)
      push_back(value);
  }
};

}  // namespace leanstl

///////////////////////////////////////  main()  ////////////////////////////

#include <memory>
#include <string>

int main()
{
  leanstl::list<int> li(3, 8);
  li.push_front(43);
  li.push_front(82);
  li.push_back(19);

  leanstl::list<int> lic(li);
  for (auto x : lic)
    printf("%d\n", x);

  leanstl::list<int> lim(std::move(li));

  lic = lim;

  int arr[3] = { 1, 2, 3};
  leanstl::list<int> lia(arr, arr+3);
  for (auto x : lia)
    printf("%d\n", x);

  leanstl::list<std::string> ls;
  ls.push_front("hello");
  printf("front=%s\n", ls.front().c_str());

  leanstl::list<std::unique_ptr<double>> lu(5);
  lu.push_front(std::unique_ptr<double>(new double(5.0)));  // make_unique in C++14
  printf("front=%f\n", *lu.front());
}
