#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "date.h"

using namespace std;

// конструктор выбрасывает исключение, если его аргументы некорректны
Date::Date(int new_year, int new_month, int new_day) {
        year = new_year;
        if (new_month > 12 || new_month < 1) {
                throw logic_error("Month value is invalid: " + to_string(new_month));
        }
        month = new_month;
        if (new_day > 31 || new_day < 1) {
                throw logic_error("Day value is invalid: " + to_string(new_day));
        }
        day = new_day;
}

int Date::GetYear() const {
        return year;
}
int Date::GetMonth() const {
        return month;
}
int Date::GetDay() const {
        return day;
}

// определить сравнение для дат необходимо для использования их в качестве ключей словаря
bool operator<(const Date& lhs, const Date& rhs) {
        // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
        // создадим вектор из года, месяца и дня для каждой даты и сравним их
        return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
               vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator <= (const Date& lhs, const Date& rhs){
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
	       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator >  (const Date& lhs, const Date& rhs){
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
	       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator >= (const Date& lhs, const Date& rhs){
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
	       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator == (const Date& lhs, const Date& rhs){
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
	       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

bool operator != (const Date& lhs, const Date& rhs){
	return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} !=
	       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

// даты будут по умолчанию выводиться в нужном формате
ostream& operator<<(ostream& stream, const Date& date) {
        stream << setw(4) << setfill('0') << date.GetYear() <<
                "-" << setw(2) << setfill('0') << date.GetMonth() <<
                "-" << setw(2) << setfill('0') << date.GetDay();
        return stream;
}
