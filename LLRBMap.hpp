/// Created by Firmament on 2021/4/26.

#ifndef LLRB_LLRBMAP_HPP
#define LLRB_LLRBMAP_HPP

#include "RBNode.hpp"

template<typename K, typename V>
class LLRBMap {
private:
  RBNode<K, V>* root_{ nullptr };

public:
  bool is_empty() const {
    return !root_;
  }

  bool contains_key(K const& key) const {
    return root_->get(key);
  }

  V& get(K const& key) {
    return *root_->get(key);
  }

  void insert(K const& key, V const& val) {
    if (is_empty()) {
      root_ = new RBNode(key, val, RBNode<K, V>::BLACK);
    } else {
      root_ = root_->put(key, val);
      root_->set_color(RBNode<K, V>::BLACK);
    }
  }

  void remove_min() {
    if (root_->left_is_red()) {
      root_->set_color(RBNode<K, V>::RED);
    }
    root_ = root_->remove_min();
    if (root_) {
      root_->set_color(RBNode<K, V>::BLACK);
    }
  }

  void remove(K const& key) {
    if (!contains_key(key)) { return; }
    if (!root_->left_is_red()) {
      root_->set_color(RBNode<K, V>::RED);
    }
    root_->remove(key);
    if (root_) {
      root_->set_color(RBNode<K, V>::BLACK);
    }
  }
};

#endif //LLRB_LLRBMAP_HPP
