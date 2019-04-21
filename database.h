#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <functional>

#include "date.h"

using namespace std;
using Predicate = function<bool(const Date&, const string&)>;

struct VectorSetStorage {
	vector<string> 	event_vector;
	set<string> 	event_set;
};

class Database {
public:
  void 		  	Add		   (const Date& date, const string& event);
  void 		  	Print	   (ostream& os) const;
  int 					RemoveIf(Predicate pred);
  vector<pair<Date, string>>  FindIf  (Predicate pred) const;
  string 				Last	(Date date) const;

  map<Date, vector<string>> GetStorage() const;
private:
  map<Date, VectorSetStorage> storage;
};

ostream& operator << (ostream& os, pair<Date, string> p);
ostream& operator << (ostream& os, vector<pair<Date, string>> v);

void TestDatabase();
void TestDateComparison();
