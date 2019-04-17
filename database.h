#pragma once

#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <functional>

#include "date.h"

using namespace std;
using Predicate = function<bool(const Date&, const string&)>;

class Database {
public:
  void 		  	Add		   (const Date& date, const string& event);
  bool 		  	DeleteEvent(const Date& date, const string& event);
  int 		  	DeleteDate (const Date& date);
  set<string> 	Find	   (const Date& date) const;
  void 		  	Print	   (ostream& os) const;
  int 					RemoveIf(Predicate pred);
  vector<pair<Date, string>>  FindIf  (Predicate pred) const;
  string 				Last	(Date date);

  map<Date, set<string>> GetStorage() const;
private:
  map<Date, set<string>> storage;
};

Date ParseDate(istringstream& date_stream);
Date ParseDate(const string& date);

ostream& operator << (ostream& os, pair<Date, string> p);
ostream& operator << (ostream& os, vector<pair<Date, string>> v);

void TestDatabase();
