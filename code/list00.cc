#include <stdio.h>

///////////////////////////////////////  list.h  ////////////////////////////

#include <stddef.h>  // for size_t

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

 private:
  struct list_node : list_node_base
  {
    T value_;
  };

  list_node_base head_;
  size_t size_ = 0;
};

}  // namespace leanstl

///////////////////////////////////////  main()  ////////////////////////////

#include <string>
#include <vector>

template <typename T>
void print_size(const char* name)
{
  typedef leanstl::list<T> L;
  printf("\n");
  printf("T = %s\n", name);
  printf("     sizeof T:         %2zd\n", sizeof(T));
  printf("     sizeof list_node: %2zd\n", sizeof(typename L::list_node));
}

int main()
{
  printf("sizeof void*:          %2zd\n", sizeof(void*));
  printf("sizeof size_t:         %2zd\n", sizeof(size_t));
  printf("sizeof list_node_base: %2zd\n", sizeof(leanstl::list_node_base));
  printf("sizeof list:           %2zd\n", sizeof(leanstl::list<double>));
#define PRINT_SIZE(X) print_size<X>(#X)
  PRINT_SIZE(int);
  PRINT_SIZE(std::string);
  PRINT_SIZE(std::vector<int>);
}
