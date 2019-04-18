#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "database.h"
#include "date.h"

using namespace std;

void Database::Add(const Date& date, const string& event) {
    bool flag = true;
	for (const auto& item : storage[date]){
    	if (event == item){
    		flag = false;
    	}
    }
	if (flag){
		storage[date].push_back(event);
	}
}

bool Database::DeleteEvent(const Date& date, const string& event) {
	if (storage.count(date) > 0) {
	    auto it = find(begin(storage[date]), end(storage[date]), event);
		if (it != end(storage[date])){
			storage[date].erase(it);
			return true;
		}
	}
	return false;
}

int Database::DeleteDate(const Date& date) {
        if (storage.count(date) == 0) {
                return 0;
        } else {
                const int event_count = storage[date].size();
                storage.erase(date);
                return event_count;
        }
}

vector<string> Database::Find(const Date& date) const {
        if (storage.count(date) > 0) {
                return storage.at(date);
        } else {
                return {};
        }
}

void Database::Print(ostream& os) const {
        for (const auto& item : storage) {
                for (const string& event : item.second) {
                        os << item.first << " " << event << endl;
                }
        }
}

int Database::RemoveIf(Predicate pred){
	int count = 0;

	//Delete dates
	for (auto it = begin(storage); it != end(storage);){
		if (pred(it->first, "")){
			count += it->second.size();
			it = storage.erase(it);
		} else {
			it++;
		}
	}

	//Delete element in vectors
	for (auto& [data, event_vector] : storage) {
		for (auto it = begin(event_vector); it != end(event_vector); ){
			if (pred(data, *it)){
				count++;
				it = event_set.erase(it);
			} else {
				it++;
			}
		}
	}

	//Delete if vector is empty
	//....

	return count;
}

vector<pair<Date, string>> Database::FindIf(Predicate pred) const{
	vector<pair<Date, string>> result;
	for (const auto& [data, event_set] : storage) {
		for (auto it = begin(event_set); it != end(event_set); it++){
			if (pred(data, *it)){
				result.push_back(make_pair(data, *it));
			}
		}
	}
	return result;
}

string Database::Last(Date date){
//	auto it = storage.lower_bound(date);
	return "12";
}

map<Date, vector<string>> Database::GetStorage() const {
  return storage;
}

Date ParseDate(istringstream& date_stream) {
        string date = date_stream.str();
        bool ok = true;

        int year;
        ok = ok && (date_stream >> year);
        ok = ok && (date_stream.peek() == '-');
        date_stream.ignore(1);

        int month;
        ok = ok && (date_stream >> month);
        ok = ok && (date_stream.peek() == '-');
        date_stream.ignore(1);

        int day;
        ok = ok && (date_stream >> day);
        ok = ok && date_stream.eof();

        if (!ok) {
                throw logic_error("Wrong date format: " + date);
        }
        return Date(year, month, day);
}

Date ParseDate(const string& date) {
    stringstream date_stream(date);
    bool ok = true;

    int year;
    ok = ok && (date_stream >> year);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    ok = ok && (date_stream >> month);
    ok = ok && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    ok = ok && (date_stream >> day);
    ok = ok && date_stream.eof();

    if (!ok) {
            throw logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

ostream& operator << (ostream& os, pair<Date, string> p){
	os << p.first << " " << p.second;
	return os;
}

ostream& operator << (ostream& os, vector<pair<Date, string>> v){
	for (const auto& item : v){
		os << item << endl;
	}
	return os;
}
