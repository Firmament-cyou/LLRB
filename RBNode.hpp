/// Created by Firmament on 2021/4/26.

#ifndef LLRB_RBNODE_HPP
#define LLRB_RBNODE_HPP

#include <algorithm>
#include <iostream>

template<typename K, typename V>
class RBNode {
private:
  K key_;
  V val_;
  RBNode<K, V>* left_;
  RBNode<K, V>* right_;
  bool color_;

  RBNode<K, V>* flip_colors() {
    color_ = !color_;
    left_->color_ = !left_->color_;
    right_->color_ = !right_->color_;
    return this;
  }

  RBNode<K, V>* rotate_left() {
    auto ret = right_;
    std::swap(color_, ret->color_);
    right_ = ret->left_;
    ret->left_ = this;
    return ret;
  }

  RBNode<K, V>* rotate_right() {
    auto ret = left_;
    std::swap(color_, ret->color_);
    left_ = ret->right_;
    ret->right_ = this;
    return ret;
  }

  // Assuming that h is red and both h.left and h.left.left
  // are black, make h.left or one of its children red.
  RBNode<K, V>* move_red_left() {
    flip_colors();
    if (not_null_red(right_->left_)) {
      right_ = right_->rotate_right();
      return rotate_left()->flip_colors();
    }
    return this;
  }

  // Assuming that h is red and both h.right and h.right.left
  // are black, make h.right or one of its children red.
  RBNode<K, V>* move_red_right() {
    flip_colors();
    if (not_null_red(left_->left_)) {
      return rotate_right()->flip_colors();
    }
    return this;
  }

  // Fix-up any right-leaning links
  RBNode<K, V>* fix_up() {
    auto ret = this;
    if (not_null_red(right_) && !not_null_red(left_)) {
      ret = ret->rotate_left();
    }
    if (not_null_red(left_) && not_null_red(left_->left_)) {
      ret = ret->rotate_right();
    }
    if (not_null_red(left_) && not_null_red(right_)) {
      ret = ret->flip_colors();
    }
    return ret;
  }

  // Auxiliary function, for reducing the length of code
  static bool not_null_red(RBNode<K, V> const* ptr) {
    return ptr && ptr->is_red();
  }

  RBNode<K, V>* get_min() {
    return left_ ? left_->get_min() : this;
  }

public:
  static bool constexpr RED = true;
  static bool constexpr BLACK = false;

  RBNode(K const& key, V const& val, bool color) :
  key_(key), val_(val), color_(color), left_(nullptr), right_(nullptr) {}

  bool is_red() const {
    return color_;
  }

  void set_color(bool const color) {
    color_ = color;
  }

  V* get(K const& key) {
    if (key == key_) {
      return &val_;
    } else if (key < key_) {
      return left_ ? left_->get(key) : nullptr;
    } else {
      return right_ ? right_->get(key) : nullptr;
    }
  }

  RBNode<K, V>* put(K const& key, V const& val) {
    if (key == key_) {
      val_ = val;
    } else if (key < key_) {
      left_ = left_ ? left_->put(key, val) : new RBNode(key, val, RED);
    } else {
      right_ = right_ ? right_->put(key, val) : new RBNode(key, val, RED);
    }
    return fix_up();
  }

  // Important, this->color_ is red or this->left_->color is red
  RBNode<K, V>* remove_min() {
    if (!left_) {
      return nullptr;
    }
    auto cur = !not_null_red(left_) && !not_null_red(left_->left_) ?
        move_red_left() : this;
    cur->left_ = cur->left_->remove_min();
    return cur->fix_up();
  }

  RBNode<K, V>* remove(K const& key) {
    auto ret = this;
    if (key < key_) {
      if (!not_null_red(left_) && !not_null_red(left_->left_)) {
        ret = move_red_left();
      }
      ret->left_ = ret->left_->remove(key);
    } else {
      // Extremely exciting!
      if (not_null_red(left_)) {
        ret = rotate_right();
      }
      if (key == ret->key_ && !ret->right_) {
        return nullptr;
      }
      if (!not_null_red(ret->right_) && !not_null_red(ret->right_->left_)) {
        ret = ret->move_red_right();
      }
      if (key == ret->key_) {
        auto min_node = ret->right_->get_min();
        ret->key_ = min_node->key_;
        ret->val_ = min_node->val_;
        ret->right_ = ret->right_->remove_min();
      } else {
        ret->right_ = ret->right_->remove(key);
      }
      return ret->fix_up();
    }
  }

  // Only for root node during deleting
  bool left_is_red() const {
    return not_null_red(left_);
  }
};

#endif //LLRB_RBNODE_HPP
