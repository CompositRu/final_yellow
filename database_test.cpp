#include "test_runner.h"
#include "condition_parser.h"
#include "database.h"
#include <sstream>

using namespace std;

void TestDatabase() {
  {
    //Test 1
    Database db;
    db.Add({2017,1,1}, "Happy NY");
    map<Date, vector<string>> expected;
    expected[{2017,1,1}].push_back("Happy NY");
    AssertEqual(db.GetStorage(), expected, "Add 1 element");
  }
  {
    //Test 2
    Database db;
    db.Add({2016,1,1}, "1");
    db.Add({2016,12,31}, "2");
    db.Add({2017,1,1}, "3");
    db.Add({2017,3,1}, "4");
    db.Add({2017,7,1}, "5");

    istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
    auto condition = ParseCondition(is);
    Predicate predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    int count = db.RemoveIf(predicate);

    AssertEqual(count, 2, "Remove 2 elements");
    map<Date, vector<string>> expected;
    expected[{2016,1,1}].push_back("1");
    expected[{2016,12,31}].push_back("2");
    expected[{2017,7,1}].push_back("5");
    AssertEqual(db.GetStorage(), expected,"Remove 2 elements");
  }
  {
    //Test 3
    Database db;
    db.Add({2016,1,1}, "1");
    db.Add({2016,12,31}, "2");
    db.Add({2017,1,1}, "3");
    db.Add({2017,3,1}, "4");
    db.Add({2017,7,1}, "5");
    istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
    auto condition = ParseCondition(is);
    Predicate predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    const auto entries = db.FindIf(predicate);
    AssertEqual(entries.size(), 2, "Find 2 elements");
    vector<pair<Date, string>> expected;
    expected.push_back(pair<Date, string>({2017,1,1},"3"));
    expected.push_back(pair<Date, string>({2017,3,1},"4"));
    AssertEqual(entries, expected,"Find 2 elements");
  }


}
