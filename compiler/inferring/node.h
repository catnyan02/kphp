#pragma once

#include <vector>

#include "compiler/inferring/edge.h"
#include "compiler/threading/locks.h"
#include "compiler/types.h"

namespace tinf {

class TypeInferer;

class Node : public Lockable {
private:
  std::vector<Edge *> next_;
  std::vector<Edge *> rev_next_;
  volatile int recalc_state_;
  volatile int holder_id_;
public:
  const TypeData *type_;
  volatile int recalc_cnt_;
  int isset_flags;
  int isset_was;

  enum {
    empty_st,
    own_st,
    own_recalc_st
  };

  Node();

  int get_recalc_cnt();
  int get_holder_id();

  void add_edge(Edge *edge);
  void add_rev_edge(Edge *edge);

  inline vector<Edge *> &get_next() {
    return next_;
  }

  inline vector<Edge *> &get_rev_next() {
    return rev_next_;
  }

  bool try_start_recalc();
  void start_recalc();
  bool try_finish_recalc();

  const TypeData *get_type() const {
    return type_;
  }

  void set_type(const TypeData *type) {
    type_ = type;
  }

  virtual void recalc(TypeInferer *inferer) = 0;
  virtual string get_description() = 0;
};

}