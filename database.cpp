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
	auto [iter, flag] = storage[date].event_set.insert(event);


//	bool flag = true;
//	for (const auto& item : storage[date] ){
//    	if (event == item){
//    		flag = false;
//    	}
//    }
	if (flag){
		storage[date].event_vector.push_back(event);
	}
}

void Database::Print(ostream& os) const {
	for (const auto& item : storage) {
		for (const string& event : item.second.event_vector) {
			os << item.first << " " << event << endl;
		}
	}
}

int Database::RemoveIf(Predicate pred){
	int count = 0;

	for (auto& it_map = begin(storage); it_map != end(storage);) {
		auto& date = it_map->first;
		auto& event_vector = it_map->second.event_vector;
		int s1 = event_vector.size();
		auto it = stable_partition(event_vector.begin(), event_vector.end(),
									[pred, date](string event){
										return !pred(date, event);
									});
		event_vector.erase(it, event_vector.end());
		int s2 = event_vector.size();
		count += s1 - s2;

		//надо ещё из set удалить

		if (event_vector.empty()){
			it_map = storage.erase(it_map);
		} else {
			it_map++;
		}
	}
	return count;
}

vector<pair<Date, string>> Database::FindIf(Predicate pred) const{
	vector<pair<Date, string>> result;
	for (const auto& [data, set_event] : storage) {
		auto& event_vector = set_event.event_vector;
		auto& event_set	   = set_event.event_set;
		for (auto it = begin(event_vector); it != end(event_vector); it++){
			if (pred(data, *it)){
				result.push_back(make_pair(data, *it));
			}
		}
	}
	return result;
}

string Database::Last(Date date) const{
	auto it = storage.lower_bound(date);
	auto& storage_date = it->first;
	auto& event_vector = it->second.event_vector;
	auto& event_set	   = it->second.event_set;

	if (storage_date == date){
		stringstream os;
		os << storage_date << " " << event_vector.back();
		return os.str();
	} else { // (it->first > date)
		if (storage_date == storage.begin()->first){
			throw invalid_argument("");
		} else {
			it--;
			stringstream os;
			os << storage_date << " " << event_vector.back();
			return os.str();
		}
	}
}

map<Date, vector<string>> Database::GetStorage() const {
	map<Date, vector<string>> result;
	for (auto& it_map = begin(storage); it_map != end(storage); it_map++){
		result.insert(make_pair(it_map->first, it_map->second.event_vector));
	}
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
