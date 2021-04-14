#ifndef BIDIRECTIONAL_LIST_H_
#define BIDIRECTIONAL_LIST_H_

#include <cassert>
#include <initializer_list>
#include <vector>

namespace containers {

template<typename T>
class BiDirectionalList {
 public:
  struct Node {
   public:
    T value;

   private:
    explicit Node(
        const T& a, Node* prev_ptr, Node* next_ptr)
        : value{a}, prev_ptr_{prev_ptr}, next_ptr_{next_ptr} {}

    explicit Node(
        T&& a, Node* prev_ptr, Node* next_ptr)
        : value{std::move(a)}, prev_ptr_{prev_ptr}, next_ptr_{next_ptr} {}

    Node* prev_ptr_ = nullptr;
    Node* next_ptr_ = nullptr;
    friend class BiDirectionalList;
  };

  BiDirectionalList() = default;

  BiDirectionalList(const std::initializer_list<T>& init_list) {
    for (const T& elem : init_list) {
      PushBack(elem);
    }
  }
  BiDirectionalList(const BiDirectionalList& list) {
    Node* parent_ptr = list.begin_ptr_;
    while (parent_ptr != nullptr) {
      PushBack(parent_ptr->value);
      parent_ptr = parent_ptr->next_ptr_;
    }
  }
  BiDirectionalList(BiDirectionalList&& list) noexcept {
    begin_ptr_ = list.begin_ptr_;
    last_ptr_ = list.last_ptr_;
    size_ = list.size_;
    list.begin_ptr_ = nullptr;
    list.last_ptr_ = nullptr;
    list.size_ = 0;
  }

  int Size() const {
    return size_;
  }
  bool IsEmpty() const {
    return size_ == 0;
  }

  std::vector<T> ToVector() const {
    Node* cur = begin_ptr_;
    std::vector<T> result;
    result.reserve(size_);
    while (cur != nullptr) {
      result.push_back(cur->value);
      cur = cur->next_ptr_;
    }
    return result;
  }

  Node* Front() {
    assert(size_ != 0);
    return begin_ptr_;
  }
  const Node* Front() const {
    assert(size_ != 0);
    return begin_ptr_;
  }
  Node* Back() {
    assert(size_ != 0);
    return last_ptr_;
  }
  const Node* Back() const {
    assert(size_ != 0);
    return last_ptr_;
  }

  int Find(const T& value) const {
    Node* cur = begin_ptr_;
    for (int i = 0; i < size_; i++) {
      if (cur->value == value) {
        return i;
      }
      cur = cur->next_ptr_;
    }
    return -1;
  }

  std::vector<int> FindAll(const T& value) const {
    std::vector<int> result;
    Node* cur = begin_ptr_;
    for (int i = 0; i < size_; i++) {
      if (cur->value == value) {
        result.push_back(i);
      }
      cur = cur->next_ptr_;
    }
    return result;
  }

  void Erase(Node* element) {
    assert(element != nullptr);
    if (element == last_ptr_) {
      PopBack();
      return;
    }
    if (element == begin_ptr_) {
      PopFront();
      return;
    }
    element->next_ptr_->prev_ptr_ = element->prev_ptr_;
    element->prev_ptr_->next_ptr_ = element->next_ptr_;
    size_--;
    delete element;
  }

  void PopFront() {
    assert(begin_ptr_ != nullptr);
    Node* temp = begin_ptr_;
    begin_ptr_ = begin_ptr_->next_ptr_;
    delete temp;
    if (begin_ptr_ != nullptr) {
      begin_ptr_->prev_ptr_ = nullptr;
    } else {
      last_ptr_ = nullptr;
    }
    size_--;
  }

  void PopBack() {
    assert(last_ptr_ != nullptr);
    Node* temp = last_ptr_;
    last_ptr_ = last_ptr_->prev_ptr_;
    delete temp;
    if (last_ptr_ != nullptr) {
      last_ptr_->next_ptr_ = nullptr;
    } else {
      begin_ptr_ = nullptr;
    }
    size_--;
  }

  Node* operator[](int index) {
    return At(index);
  }
  const Node* operator[](int index) const {
    return At(index);
  }

  void InsertBefore(Node* element, const T& value) {
    assert(element != nullptr);
    size_++;
    if (element == begin_ptr_) {
      element->prev_ptr_ = new Node(value, nullptr, element);
      begin_ptr_ = element->prev_ptr_;
    } else {
      element->prev_ptr_->next_ptr_ =
          new Node(value, element->prev_ptr_, element);
      element->prev_ptr_ = element->prev_ptr_->next_ptr_;
    }
  }
  void InsertBefore(Node* element, T&& value) {
    assert(element != nullptr);
    size_++;
    if (element == begin_ptr_) {
      element->prev_ptr_ = new Node(std::move(value), nullptr, element);
      begin_ptr_ = element->prev_ptr_;
    } else {
      element->prev_ptr_->next_ptr_ =
          new Node(std::move(value), element->prev_ptr_, element);
      element->prev_ptr_ = element->prev_ptr_->next_ptr_;
    }
  }
  void InsertAfter(Node* element, const T& value) {
    assert(element != nullptr);
    size_++;
    if (element == last_ptr_) {
      element->next_ptr_ = new Node(value, element, nullptr);
      last_ptr_ = element->next_ptr_;
    } else {
      element->next_ptr_->prev_ptr_ =
          new Node(value, element, element->next_ptr_);
      element->next_ptr_ = element->next_ptr_->prev_ptr_;
    }
  }
  void InsertAfter(Node* element, T&& value) {
    assert(element != nullptr);
    size_++;
    if (element == last_ptr_) {
      element->next_ptr_ = new Node(std::move(value), element, nullptr);
      last_ptr_ = element->next_ptr_;
    } else {
      element->next_ptr_->prev_ptr_ =
          new Node(std::move(value), element, element->next_ptr_);
      element->next_ptr_ = element->next_ptr_->prev_ptr_;
    }
  }
  void PushBack(const T& value) {
    if (size_ == 0) {
      size_++;
      begin_ptr_ = new Node(value, nullptr, nullptr);
      last_ptr_ = begin_ptr_;
    } else {
      InsertAfter(last_ptr_, value);
    }
  }
  void PushBack(T&& value) {
    if (size_ == 0) {
      size_++;
      begin_ptr_ = new Node(std::move(value), nullptr, nullptr);
      last_ptr_ = begin_ptr_;
    } else {
      InsertAfter(last_ptr_, std::move(value));
    }
  }
  void PushFront(const T& value) {
    if (size_ == 0) {
      size_++;
      begin_ptr_ = new Node(value, nullptr, nullptr);
      last_ptr_ = begin_ptr_;
    } else {
      InsertBefore(begin_ptr_, value);
    }
  }
  void PushFront(T&& value) {
    if (size_ == 0) {
      size_++;
      begin_ptr_ = new Node(std::move(value), nullptr, nullptr);
      last_ptr_ = begin_ptr_;
    } else {
      InsertBefore(begin_ptr_, std::move(value));
    }
  }

  BiDirectionalList& operator=(const BiDirectionalList& list) {
    if (begin_ptr_ == list.begin_ptr_) {
      return (*this);
    }
    CleanTheList();
    Node* cur = list.begin_ptr_;
    while (cur != nullptr) {
      PushBack(cur->value);
      cur = cur->next_ptr_;
    }
    return (*this);
  }
  BiDirectionalList& operator=(BiDirectionalList&& list) {
    if (begin_ptr_ == list.begin_ptr_) {
      return (*this);
    }
    CleanTheList();
    begin_ptr_ = list.begin_ptr_;
    last_ptr_ = list.last_ptr_;
    size_ = list.size_;
    list.begin_ptr_ = nullptr;
    list.last_ptr_ = nullptr;
    list.size_ = 0;
    return (*this);
  }

  bool operator==(const BiDirectionalList& rhs) const {
    if (size_ != rhs.size_) {
      return false;
    }
    Node* lhs_elem = begin_ptr_;
    Node* rhs_elem = rhs.begin_ptr_;
    for (int i = 0; i < size_; i++) {
      if (lhs_elem->value != rhs_elem->value) {
        return false;
      }
      lhs_elem = lhs_elem->next_ptr_;
      rhs_elem = rhs_elem->next_ptr_;
    }
    return true;
  }
  bool operator!=(const BiDirectionalList& rhs) const {
    return !(*this == rhs);
  }

  ~BiDirectionalList() {
    while (!IsEmpty()) {
      PopBack();
    }
  }

 private:
  Node* begin_ptr_ = nullptr;
  Node* last_ptr_ = nullptr;
  int size_ = 0;

  Node* At(int index) const {
    assert(0 <= index && index < size_);
    Node* proper_ptr = begin_ptr_;
    for (int i = 0; i < index; i++) {
      proper_ptr = proper_ptr->next_ptr_;
    }
    return proper_ptr;
  }

  void CleanTheList() {
    size_ = 0;
    Node* cur = last_ptr_;
    while (cur != nullptr) {
      Node* temp = cur->prev_ptr_;
      delete cur;
      cur = temp;
    }
    begin_ptr_ = nullptr;
    last_ptr_ = nullptr;
  }
};

}  // namespace containers

#endif  // BIDIRECTIONAL_LIST_H_
