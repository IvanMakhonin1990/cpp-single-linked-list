#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type> class SingleLinkedList {
  // Узел списка
  struct Node {
    Node() = default;
    Node(const Type &val, Node *next) : value(val), next_node(next) {}
    Type value;
    Node *next_node = nullptr;
  };

public:
  SingleLinkedList() = default;
  
  SingleLinkedList(std::initializer_list<Type> values) {
    if (!IsEmpty() || 0 == values.size()) {
      return;
    }
    SingleLinkedList *tmp = new SingleLinkedList();
    Node *pos = &(tmp->head_);
    for (auto v = values.begin(); v != values.end(); ++v) {
      pos->next_node = new Node(*v, nullptr);
      pos = pos->next_node;
    }
    this->swap(*tmp);
  }

  SingleLinkedList(const SingleLinkedList &other) {
    if (!IsEmpty() || head_.next_node == other.head_.next_node) {
      return;
    }
    SingleLinkedList *tmp = new SingleLinkedList();
    Node *pos = &(tmp->head_);
    for (auto o = other.begin(); o != other.end(); ++o) {
      pos->next_node = new Node(*o, nullptr);
      pos = pos->next_node;
    }
    this->swap(*tmp);
  }

  SingleLinkedList &operator=(const SingleLinkedList &rhs) {

    SingleLinkedList *tmp = new SingleLinkedList();
    Node *pos = &(tmp->head_);
    for (auto o = rhs.begin(); o != rhs.end(); ++o) {
      pos->next_node = new Node(*o, nullptr);
      pos = pos->next_node;
    }
    this->swap(*tmp);
    return *this;
  }

  // Обменивает содержимое списков за время O(1)
  void swap(SingleLinkedList &other) noexcept {
    if (head_.next_node != other.head_.next_node) {
      auto tmp = other.head_.next_node;
      other.head_.next_node = head_.next_node;
      head_.next_node = tmp;
    }
  }

  // Возвращает количество элементов в списке за время O(1)
  [[nodiscard]] size_t GetSize() const noexcept {
    if (IsEmpty()) {
      return 0;
    }
    size_t size = 0;
    Node *n = head_.next_node;
    while (n) {
      ++size;
      n = n->next_node;
    }
    return size;
  }

  // Сообщает, пустой ли список за время O(1)
  [[nodiscard]] bool IsEmpty() const noexcept {
    return nullptr == head_.next_node;
  }

  void PushFront(const Type &value) {
    Node *n = new Node(value, head_.next_node);
    head_.next_node = n;
  }

  void PushBack(const Type &value) {
    Node *n = new Node(value, head_.next_node);
    auto pos = head_.next_node;
    while (pos->next_value) {
      pos = pos->next_value;
    }
    pos->next_value = n;
  }

  // Очищает список за время O(N)
  void Clear() noexcept {
    if (IsEmpty()) {
      return;
    }
    Node *n = head_.next_node;
    Node *p = head_.next_node;
    head_.next_node = nullptr;
    while (p) {
      p = p->next_node;
      delete n;
      n = p;
    }
  }

  void PopFront() noexcept {
    if (IsEmpty()) {
      return;
    }
    auto p = head_.next_node;
    head_.next_node = p->next_node;
    delete p;
  }

  ~SingleLinkedList() { Clear(); }

  // Шаблон класса «Базовый Итератор».
  // Определяет поведение итератора на элементы односвязного списка
  // ValueType — совпадает с Type (для Iterator) либо с const Type (для
  // ConstIterator)
  template <typename ValueType> class BasicIterator {
    // Класс списка объявляется дружественным, чтобы из методов списка
    // был доступ к приватной области итератора
    friend class SingleLinkedList;

    // Конвертирующий конструктор итератора из указателя на узел списка
    explicit BasicIterator(Node *node) { node_ = node; }

  public:
    // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого
    // итератора

    // Категория итератора — forward iterator
    // (итератор, который поддерживает операции инкремента и многократное
    // разыменование)
    using iterator_category = std::forward_iterator_tag;
    // Тип элементов, по которым перемещается итератор
    using value_type = Type;
    // Тип, используемый для хранения смещения между итераторами
    using difference_type = std::ptrdiff_t;
    // Тип указателя на итерируемое значение
    using pointer = ValueType *;
    // Тип ссылки на итерируемое значение
    using reference = ValueType &;

    BasicIterator() = default;

    // Конвертирующий конструктор/конструктор копирования
    // При ValueType, совпадающем с Type, играет роль копирующего конструктора
    // При ValueType, совпадающем с const Type, играет роль конвертирующего
    // конструктора
    BasicIterator(const BasicIterator<Type> &other) noexcept {
      node_ = other.node_;
    }

    // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при
    // наличии пользовательского конструктора копирования, явно объявим оператор
    // = и попросим компилятор сгенерировать его за нас
    BasicIterator &operator=(const BasicIterator &rhs) = default;

    // Оператор сравнения итераторов (в роли второго аргумента выступает
    // константный итератор) Два итератора равны, если они ссылаются на один и
    // тот же элемент списка либо на end()
    [[nodiscard]] bool
    operator==(const BasicIterator<const Type> &rhs) const noexcept {
      return node_ == rhs.node_;
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool
    operator!=(const BasicIterator<const Type> &rhs) const noexcept {
      return node_ != rhs.node_;
    }

    // Оператор сравнения итераторов (в роли второго аргумента итератор)
    // Два итератора равны, если они ссылаются на один и тот же элемент списка
    // либо на end()
    [[nodiscard]] bool
    operator==(const BasicIterator<Type> &rhs) const noexcept {
      return node_ == rhs.node_;
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool
    operator!=(const BasicIterator<Type> &rhs) const noexcept {
      return node_ != rhs.node_;
    }

    // Оператор прединкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает ссылку на самого себя Инкремент итератора, не
    // указывающего на существующий элемент списка, приводит к неопределённому
    // поведению
    BasicIterator &operator++() noexcept {

      (*this)++;
      return *this;
    }

    // Оператор постинкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает прежнее значение итератора Инкремент итератора,
    // не указывающего на существующий элемент списка, приводит к
    // неопределённому поведению
    BasicIterator operator++(int) noexcept {
      Node *node = node_;
      // BasicIterator<Type> oldValue = BasicIterator<const Type>(*this);
      node_ = node_->next_node;
      return BasicIterator<Type>(node);
    }

    // Операция разыменования. Возвращает ссылку на текущий элемент
    // Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] reference operator*() const noexcept { return node_->value; }

    // Операция доступа к члену класса. Возвращает указатель на текущий элемент
    // списка Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] pointer operator->() const noexcept {
      return &(node_->value);
    }

  private:
    Node *node_ = nullptr;
  };

public:
  using value_type = Type;
  using reference = value_type &;
  using const_reference = const value_type &;

  // Итератор, допускающий изменение элементов списка
  using Iterator = BasicIterator<Type>;
  // Константный итератор, предоставляющий доступ для чтения к элементам списка
  using ConstIterator = BasicIterator<const Type>;

  // Возвращает итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  [[nodiscard]] Iterator begin() noexcept { return Iterator(head_.next_node); }

  // Возвращает итератор, указывающий на позицию, следующую за последним
  // элементом односвязного списка Разыменовывать этот итератор нельзя — попытка
  // разыменования приведёт к неопределённому поведению
  [[nodiscard]] Iterator end() noexcept { return Iterator(nullptr); }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  // Результат вызова эквивалентен вызову метода cbegin()
  [[nodiscard]] ConstIterator begin() const noexcept {
    return Iterator(head_.next_node);
  }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению Результат
  // вызова эквивалентен вызову метода cend()
  [[nodiscard]] ConstIterator end() const noexcept { return Iterator(nullptr); }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен cend()
  [[nodiscard]] ConstIterator cbegin() const noexcept { return begin(); }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator cend() const noexcept { return end(); }

  // Возвращает итератор, указывающий на позицию перед первым элементом
  // односвязного списка. Разыменовывать этот итератор нельзя - попытка
  // разыменования приведёт к неопределённому поведению
  [[nodiscard]] Iterator before_begin() noexcept { return Iterator(&head_); }

  // Возвращает константный итератор, указывающий на позицию перед первым
  // элементом односвязного списка. Разыменовывать этот итератор нельзя -
  // попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
    return ConstIterator(const_cast<Node *>(&head_));
  }

  // Возвращает константный итератор, указывающий на позицию перед первым
  // элементом односвязного списка. Разыменовывать этот итератор нельзя -
  // попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator before_begin() const noexcept {
    return ConstIterator(&head_);
  }

  /*
   * Вставляет элемент value после элемента, на который указывает pos.
   * Возвращает итератор на вставленный элемент
   * Если при создании элемента будет выброшено исключение, список останется в
   * прежнем состоянии
   */
  Iterator InsertAfter(ConstIterator pos, const Type &value) {
    auto n = new Node(value, pos.node_->next_node);
    pos.node_->next_node = n;
    return Iterator(n);
  }
  /*
   * Удаляет элемент, следующий за pos.
   * Возвращает итератор на элемент, следующий за удалённым
   */
  Iterator EraseAfter(ConstIterator pos) noexcept {
    if (nullptr != pos.node_->next_node) {
      auto p = pos.node_->next_node;
      pos.node_->next_node = pos.node_->next_node->next_node;
      delete p;
      return Iterator(pos.node_->next_node);
    }
    return Iterator(nullptr);
  }

private:
  // Фиктивный узел, используется для вставки "перед первым элементом"
  Node head_;
  size_t size_;
};

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  if (lhs.begin() == rhs.begin() || (lhs.IsEmpty() && rhs.IsEmpty())) {
    return true;
  }
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  // return true;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return lhs < rhs || lhs == rhs;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return lhs > rhs || lhs == rhs;
}

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
  lhs.swap(rhs);
}

// внешние функции разместите здесь
