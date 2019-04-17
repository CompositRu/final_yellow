#include <memory>
#include <string>

#include "node.h"

template <class T>
bool Compare(Comparison cmp, T t1, T t2){
	if (cmp == Comparison::Less){
		return t1 < t2;
	} else if (cmp == Comparison::LessOrEqual) {
		return t1 <= t2;
	} else if (cmp == Comparison::Greater) {
		return t1 > t2;
	} else if (cmp == Comparison::GreaterOrEqual) {
		return t1 >= t2;
	} else if (cmp == Comparison::Equal) {
		return t1 == t2;
	} else if (cmp == Comparison::NotEqual) {
		return t1 != t2;
	} else {
		return true;
	}
}

//------------------------Date_Node-----------------------------------------
DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date)
										: Node(cmp), date_(date) {}

bool DateComparisonNode::Evaluate(const Date& date, const string& event) {
	return Compare(cmp_, date, date_);
}

//-------------------------Event_Node----------------------------------------
EventComparisonNode::EventComparisonNode(Comparison cmp, const string& event)
										: Node(cmp), event_(event) {}

bool EventComparisonNode::Evaluate(const Date& date, const string& event) {
	return Compare(cmp_, event, event_);
}

//------------------------Logical_Node-----------------------------------------
LogicalOperationNode::LogicalOperationNode(LogicalOperation lo,
											shared_ptr<Node> left,
											shared_ptr<Node> right)
											: lo_(lo), left_(left), right_(right) {}

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) {
	if (lo_ == LogicalOperation::Or){
		return left_->Evaluate(date, event) || right_->Evaluate(date, event);
	} else {
		return left_->Evaluate(date, event) && right_->Evaluate(date, event);
	}
}

//-------------------------Empty_Node----------------------------------------
EmptyNode::EmptyNode() {}

bool EmptyNode::Evaluate(const Date& date, const string& event) {
	return true;
}

//----------------------------------------------------------------
void DateComparisonNode::Print(const Date& date, const string& event){
	string cmp;
	if (cmp_ == Comparison::Less){
		cmp = "<";
	} else if (cmp_ == Comparison::LessOrEqual) {
		cmp = "<=";
	} else if (cmp_ == Comparison::Greater) {
		cmp = ">";
	} else if (cmp_ == Comparison::GreaterOrEqual) {
		cmp = ">=";
	} else if (cmp_ == Comparison::Equal) {
		cmp = "==";
	} else if (cmp_ == Comparison::NotEqual) {
		cmp = "!=";
	}
	cerr << "(*" << date << "* " << cmp << " " << date_ << ")";
}

void EventComparisonNode::Print(const Date& date, const string& event){
	string cmp;
	if (cmp_ == Comparison::Less){
		cmp = "<";
	} else if (cmp_ == Comparison::LessOrEqual) {
		cmp = "<=";
	} else if (cmp_ == Comparison::Greater) {
		cmp = ">";
	} else if (cmp_ == Comparison::GreaterOrEqual) {
		cmp = ">=";
	} else if (cmp_ == Comparison::Equal) {
		cmp = "==";
	} else if (cmp_ == Comparison::NotEqual) {
		cmp = "!=";
	}
	cerr << "(*" << event << "* " << cmp << " " << event_ << ")";
}

void LogicalOperationNode::Print(const Date& date, const string& event){
	string lo;
	if (lo_ == LogicalOperation::Or){
		lo = "Or";
	} else {
		lo = "And";
	}
	left_->Print(date, event);
	cerr << " [" << left_->Evaluate(date, event) << "] ";
	cerr << " " << lo << " ";
	right_->Print(date, event);
	cerr << " [" << right_->Evaluate(date, event) << "] ";
}

void EmptyNode::Print(const Date& date, const string& event){
	cerr << "(empty)";
}
