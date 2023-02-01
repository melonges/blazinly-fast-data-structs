#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
template <typename K, typename V> class Node {
public:
  Node() : value{}, key{}, prev(nullptr), next(nullptr){};
  Node(K k, V v) : value(std::move(v)), key(k){};
  K key;
  V value;
  Node<K, V> *prev;
  Node<K, V> *next;

  ~Node() { cout << "delete value " << value << endl; }
};

template <typename K, typename V> class LRUCache {
  int capacity;
  Node<K, V> *left;
  Node<K, V> *right;
  unordered_map<K, Node<K, V> *> cache;

  void _insert(Node<K, V> *n) {
    auto prev = right->prev;
    right->prev = n;
    prev->next = n;
    n->prev = prev;
    n->next = right;
  }

  void _remove(Node<K, V> *n) {
    auto prev = n->prev;
    auto next = n->next;
    prev->next = next;
    next->prev = prev;
  }

public:
  explicit LRUCache(int c) {
    capacity = c;
    left = new Node<K, V>();
    right = new Node<K, V>();
    left->next = right;
    right->prev = left;
  }

  ~LRUCache() {
    auto curr = left;
    Node<K, V> *next;
    while (curr && curr->next) {
      next = curr->next;
      delete curr;
      curr = next;
    }
  }

  void insert(K key, V value) {
    if (includes(key)) {
      auto el = cache[key];
      el->value = value;
      _remove(el);
      _insert(el);
      return;
    }
    auto el = new Node<K, V>(key, value);
    _insert(el);
    cache[key] = el;

    if (cache.size() > capacity) {
      auto d = left->next;
      auto next = d->next;
      left->next = next;
      next->prev = left;
      cache.erase(d->key);
      delete d;
    }
  }

  V get(K key) {
    if (includes(key)) {
      _remove(cache[key]);
      _insert(cache[key]);
      return cache[key]->value;
    }
    return {};
  }

  bool includes(K key) { return cache.find(key) != cache.end(); }
};

int main(int argc, char *argv[]) {
  auto cache = LRUCache<int, int>(3);
  cache.insert(1, 4);
  std::cout << cache.includes(1) << std::endl;
  cache.insert(1, 3);
  cache.insert(2, 3);
  cache.insert(3, 3);
  cache.insert(6, 1);
  std::cout << cache.get(1) << std::endl;
  std::cout << cache.get(3) << std::endl;
  auto cache1 = LRUCache<int, string>(2);
  cache1.insert(1, "aboba");
  cache1.insert(3, "baba_kapa");
  cache1.insert(4, "elon_musk");
  std::cout << cache1.get(1) << std::endl;
  std::cout << cache1.get(4) << std::endl;
  return 0;
}
