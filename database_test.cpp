#include "test_runner.h"
#include "condition_parser.h"
#include "database.h"
#include <sstream>

using namespace std;

void TestDatabase() {
  {
    //Test 1 add
    Database db;
    db.Add({2017,1,1}, "Happy NY");
    map<Date, vector<string>> expected;
    expected[{2017,1,1}].push_back("Happy NY");
    AssertEqual(db.GetStorage(), expected, "Condition 1");
  }
  {
    //Test 2 delete date
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

    AssertEqual(count, 2, "Condition 2");
    map<Date, vector<string>> expected;
    expected[{2016,1,1}].push_back("1");
    expected[{2016,12,31}].push_back("2");
    expected[{2017,7,1}].push_back("5");
    AssertEqual(db.GetStorage(), expected,"Condition 2");
  }
  {
    //Test 3 find date
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
    AssertEqual(entries.size(), 2, "Condition 3");
    vector<pair<Date, string>> expected;
    expected.push_back(pair<Date, string>({2017,1,1},"3"));
    expected.push_back(pair<Date, string>({2017,3,1},"4"));
    AssertEqual(entries, expected,"Condition 3");
  }
  {
	//Test 4 last
	Database db;
	db.Add({2017,1,1}, "New Year");
	db.Add({2017,3,8}, "Holiday");
	db.Add({2017,1,1}, "Holiday");
	AssertEqual(db.Last({2017,01,01}), "2017-01-01 Holiday", "Last condition 1");
	AssertEqual(db.Last({2017,06,01}), "2017-03-08 Holiday", "Last condition 2");
	bool flag = false;
	try{
		db.Last({2016,12,31});
	}catch (invalid_argument&){
		flag = true;
	}
	Assert(flag, "Condition 4");
  }
  {
    //Test 5 delete date
    Database db;
    db.Add({2017,6,1}, "1st of June");
    db.Add({2017,7,8}, "8th of July");
    db.Add({2017,7,8}, "Someone's birthday");

    istringstream is("date == 2017-07-08");
    auto condition = ParseCondition(is);
    Predicate predicate = [condition](const Date& date, const string& event) {
      return condition->Evaluate(date, event);
    };
    int count = db.RemoveIf(predicate);

    AssertEqual(count, 2, "Condition 5");
    map<Date, vector<string>> expected;
    expected[{2017,6,1}].push_back("1st of June");
    AssertEqual(db.GetStorage(), expected,"Condition 5");
  }
  {
	  //Test 6 delete event
	  Database db;
      db.Add({2017, 1, 1}, "first");
      db.Add({2017, 1, 1}, "second");
      db.Add({2017, 1, 1}, "third");
      db.Add({2017, 1, 1}, "fourth");
      db.Add({2017, 1, 1}, "five");

      istringstream is(R"(event == "second" OR event == "fourth")");
      auto condition = ParseCondition(is);
      Predicate predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);

      AssertEqual(count, 2, "Condition 6");
      map<Date, vector<string>> expected;
      expected[{2017, 1, 1}].push_back("first");
      expected[{2017, 1, 1}].push_back("third");
      expected[{2017, 1, 1}].push_back("five");
      AssertEqual(db.GetStorage(), expected,"Condition 6");
  }
  {
	  //Test 7 delete event
	  Database db;
      db.Add({2017, 1, 1}, "first");
      db.Add({2017, 1, 1}, "second");
      db.Add({2017, 1, 1}, "third");
      db.Add({2017, 1, 1}, "fourth");
      db.Add({2017, 1, 1}, "five");

      istringstream is(R"(event != "second")");
      auto condition = ParseCondition(is);
      Predicate predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);

      AssertEqual(count, 4, "Condition 7");
      map<Date, vector<string>> expected;
      expected[{2017, 1, 1}].push_back("second");

      AssertEqual(db.GetStorage(), expected,"Condition 7");
  }
  //надо добавить сложные тесты, где комбинируются методы
  //например:
  //add, add, del, add, last
}

void TestDateComparison() {
  {
	Assert(Date{2017, 11, 18} == Date{2017, 11, 18}, "Compare date 1");
	Assert(Date{2017, 11, 18} != Date{2017, 11, 19}, "Compare date 2");

	Assert(Date{2017, 11, 18} < Date{2017, 11, 19}, "Compare date 3");
	Assert(Date{2017, 10, 18} < Date{2017, 11, 19}, "Compare date 4");
	Assert(Date{2016, 11, 18} < Date{2017, 11, 19}, "Compare date 5");
	Assert(Date{2017, 11, 20} > Date{2017, 11, 19}, "Compare date 6");
	Assert(Date{2017, 12, 18} > Date{2017, 11, 19}, "Compare date 7");
	Assert(Date{2018, 11, 18} > Date{2017, 11, 19}, "Compare date 8");

	Assert(Date{2017, 11, 18} >= Date{2017, 11, 18}, "Compare date 9");
	Assert(Date{2017, 11, 18} <= Date{2017, 11, 18}, "Compare date 10");

	Assert(Date{2017, 11, 18} <= Date{2017, 11, 19}, "Compare date 11");
	Assert(Date{2017, 10, 18} <= Date{2017, 11, 19}, "Compare date 12");
	Assert(Date{2016, 11, 18} <= Date{2017, 11, 19}, "Compare date 13");
	Assert(Date{2017, 11, 20} >= Date{2017, 11, 19}, "Compare date 14");
	Assert(Date{2017, 12, 18} >= Date{2017, 11, 19}, "Compare date 15");
	Assert(Date{2018, 11, 18} >= Date{2017, 11, 19}, "Compare date 16");
  }
}
