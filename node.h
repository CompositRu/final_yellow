#pragma once

#include <memory>
#include <string>

#include "date.h"

enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual,
};

enum class LogicalOperation {
	Or,
	And,
};

template <class T>
bool Compare(Comparison cmp, T t1, T t2);

class Node{
public:
	Node(Comparison cmp = Comparison::Less) : cmp_(cmp) {};
	virtual bool Evaluate(const Date& date, const string& event) = 0;
	virtual ~Node() {};

	virtual void Print(const Date& date, const string& event) = 0;
protected:
	Comparison cmp_;
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode(Comparison cmp, const Date& date);
	bool Evaluate(const Date& date, const string& event) override;

	void Print(const Date& date, const string& event) override;
private:
	const Date date_;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode(Comparison cmp, const string& event);
	bool Evaluate(const Date& date, const string& event) override;

	void Print(const Date& date, const string& event) override;
private:
	const string event_;
};

class LogicalOperationNode : public Node {
public:
	LogicalOperationNode(LogicalOperation lo, shared_ptr<Node> left, shared_ptr<Node> right);
	bool Evaluate(const Date& date, const string& event) override;

	void Print(const Date& date, const string& event) override;
private:
	const LogicalOperation lo_;
	shared_ptr<Node> left_;
	shared_ptr<Node> right_;
};

class EmptyNode : public Node {
public:
	EmptyNode();
	bool Evaluate(const Date& date, const string& event) override;

	void Print(const Date& date, const string& event) override;
};
