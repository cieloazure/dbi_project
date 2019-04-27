#ifndef QUERYPLAN_H
#define QUERYPLAN_H

#include "Comparison.h"
#include "Pipe.h"
#include "Record.h"
#include "Schema.h"
#include "Function.h"

enum PlanNodeType
{
  BASE_NODE,
  DUPLICATE_REMOVAL,
  GROUP_BY,
  JOIN,
  PROJECT,
  SELECT_FILE,
  SELECT_PIPE,
  SUM,
  RELATION_NODE
};

class BaseNode;

struct Link {
  BaseNode *value;
  Pipe *pipe;
  int id;

  static int pool;

  Link() {
    value = NULL;
    pipe = NULL;
    id = -1;
  }

  Link(BaseNode *val) {
    value = val;
    id = Link::pool;
    Link::pool++;
  }
};

class BaseNode {
 protected:
 public:
  PlanNodeType nodeType;
  Schema *schema;
  Link left;
  Link right;
  Link parent;
  BaseNode() { nodeType = BASE_NODE; }
  virtual void dummy(){};
};

class RelationNode : public BaseNode
{
public:
  char *relName;
  Schema *schema;
  RelationNode()
  {
    nodeType = RELATION_NODE;
    relName = NULL;
    schema = NULL;
  }
  void dummy() {}
};

class JoinNode : public BaseNode
{
public:
  CNF *cnf;
  Record *literal;
  Schema *schema;
  JoinNode()
  {
    nodeType = JOIN;
    cnf = NULL;
    literal = NULL;
    schema = NULL;
  }
  void dummy() {}
};

class DuplicateRemovalNode : public BaseNode
{
public:
  DuplicateRemovalNode();
  void dummy() {}
};

class GroupByNode : public BaseNode
{
public:
  OrderMaker *o;
  Function *f;
  GroupByNode()
  {
    o = NULL;
    f = NULL;
  }
  void dummy() {}
};

class ProjectNode : BaseNode
{
public:
  int *keepMe;
  int numAttsInput;
  int numAttsOutput;
  ProjectNode()
  {
    keepMe = NULL;
    numAttsInput = 0;
    numAttsOutput = 0;
  }
  void dummy() {}
};

class SumNode : BaseNode
{
public:
  Function *f;
  SumNode()
  {
    f = NULL;
  }
  void dummy() {}
};

class QueryPlan
{
private:
  BaseNode *root;

public:
  QueryPlan(BaseNode *root);
  void Execute();
  void Print();
};
#endif