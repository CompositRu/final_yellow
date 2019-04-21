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

//	//Delete dates
//	for (auto it = begin(storage); it != end(storage);){
////		cerr << "data delete  ";
//		if (pred(it->first, "")){
//			count += it->second.size();
//			it = storage.erase(it);
//		} else {
//			it++;
//		}
//	}

	//Delete element in vectors
	for (auto it_map = begin(storage); it_map != end(storage);) {
		auto& date = it_map->first;
		auto& event_vector = it_map->second;
		for (auto it_event = begin(event_vector); it_event != end(event_vector); ){
//тут можно удалять ф-ией ремувиф, чтобы ускорить
			//			cerr << "event delete  ";
			if (pred(date, *it_event)){
//				cout << "Erase: {" << date << "} {" << *it_event << "} " << endl;
				count++;
				it_event = event_vector.erase(it_event);
			} else {
//				cout << "Don't erase: {" << date << "} {" << *it_event << "} " << endl;
				it_event++;
			}
		}
		if (event_vector.empty()){
			it_map = storage.erase(it_map);
		} else {
			it_map++;
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

string Database::Last(Date date) const{
	auto it = storage.lower_bound(date);
	if (it->first == date){
		stringstream os;
		os << it->first << " " << it->second.back();
		return os.str();
	} else { // (it->first > date)
		if (it->first == storage.begin()->first){
			throw invalid_argument("");
		} else {
			it--;
			stringstream os;
			os << it->first << " " << it->second.back();
			return os.str();
		}
	}
}

map<Date, vector<string>> Database::GetStorage() const {
  return storage;
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
