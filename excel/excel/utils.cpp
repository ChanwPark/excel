#include "stdafx.h"
#include "utils.h"
#include <algorithm>

namespace MyExcel {

	Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {}


	//눈으로는 별로 이해가 안되니 사용 해봐야 알듯 

	void Vector::push_back(string s)
	{
		if (capacity <= length) {
			string* temp = new string[capacity * 2];
			for (int i = 0; i < length; i++) {
				temp[i] = data[i];
			}

			delete[] data;
			data = temp;
			capacity *= 2;
		}

		data[length] = s;
		length++;
	}

	std::string Vector::operator[](int i) { return data[i]; }

	void Vector::remove(int x)
	{
		for (int i = x + 1; i < length; i++) {
			data[i - 1] = data[i];
		}
		length--;
	}

	int Vector::size() { return length; }

	Vector::~Vector()
	{
		if (data) {
			delete[] data;
		}
	}

	Stack::Stack() : start(NULL, "") { current = &start; }

	void Stack::push(string s) {
		Node* n = new Node(current, s);
		current = n;
	}

	string Stack::pop() {
		if (current == &start) return "";

		string s = current->s;

		Node* prev = current;
		current = current->prev;

		//Delete poped node

		delete prev;
		return s;
	}

	string Stack::peek() { return current->s; }

	bool Stack::is_empty() {
		if (current == &start) return true;
		return false;
	}

	Stack::~Stack()
	{
		while (current != &start) {
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////Cell//////////////////////////////////////////////

	//StringCell

	Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

	StringCell::StringCell(string data, int x, int y, Table* t)
		: data(data), Cell(x, y, t) {}

	string StringCell::stringify() { return data;  }
	int StringCell::to_numeric() { return 0; }


	//NumberCell

	NumberCell::NumberCell(int data, int x, int y, Table* t)
		: data(data), Cell(x, y, t) {}


	string NumberCell::stringify() { return to_string(data); }
	int NumberCell::to_numeric() { return data; }

	//DateCell
	string DateCell::stringify() {
		char buf[50]; 
		tm temp; //변환한 지역 시각을 설정할 메모리 주소 
		localtime_s(&temp, &data); 

		strftime(buf, 50, "%F", &temp); 

		return string(buf); 
	}

	int DateCell::to_numeric() { return static_cast<int>(data); }

	DateCell::DateCell(string s, int x, int y, Table* t) : Cell(x, y, t) {
		//입력받는 Date 형식은 항상 yyyy-mm-dd 꼴이라 가정한다. 

		int year = atoi(s.c_str()); 
		int month = atoi(s.c_str() + 5);
		int day = atoi(s.c_str() + 8);

		tm timeinfo; 

		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;

		data = mktime(&timeinfo);
	}

	class Ex
	///////////////////////////////////////////////////////////////////////////////
	Table::Table(int max_row_size, int max_col_size)
		:max_row_size(max_row_size), max_col_size(max_col_size) {

		data_table = new Cell**[max_row_size];

		for (int i = 0; i < max_row_size; i++) {
			data_table[i] = new Cell*[max_col_size];
			for (int j = 0; j < max_col_size; j++) {
				data_table[i][j] = NULL;
			}
		}
	}


	Table::~Table() {
		for (int i = 0; i < max_row_size; i++) {
			for (int j = 0; j < max_col_size; j++) {
				if (data_table[i][j]) delete data_table[i][j];
			}
		}

		for (int i = 0; i < max_row_size; i++) {
			delete[] data_table[i];
		}

		delete data_table;

	}



	void Table::reg_cell(Cell* c, int row, int col) {
		if (!(row < max_row_size && col < max_col_size)) return;

		if (data_table[row][col]) {
			delete data_table[row][col];
		}

		data_table[row][col] = c;
	}

	int Table::to_numeric(const string& s) {
		//Cell 이름으로 받는다.

		int col = s[0] - 'A';
		int row = atoi(s.c_str() + 1) - 1;


		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->to_numeric();
			}
		}

		return 0;
	}

	int Table::to_numeric(int row, int col) {
		if (row < max_row_size && col < max_col_size && data_table[row][col]) {
			return data_table[row][col]->to_numeric();
		}


		return 0;
	}

	string Table::stringify(const string& s) {
		//Cell 이름으로 받는다.

		int col = s[0] - 'A';
		int row = atoi(s.c_str() + 1) - 1;

		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->stringify();
			}
		}

		return 0;
	}

	string Table::stringify(int row, int col) {
		if (row < max_row_size && col < max_col_size && data_table[row][col]) {
			return data_table[row][col]->stringify();
		}

		return "";
	}


	std::ostream& operator<<(std::ostream& o, Table& table) {
		o << table.print_table();
		return o;
	}

	TxtTable::TxtTable(int row, int col) : Table(row, col) {}


	//텍스트 표를 깨끗하게 출력해준다.

	string TxtTable::print_table() {
		string total_table;

		int* col_max_wide = new int[max_col_size];

		for (int i = 0; i < max_col_size; i++) {
			unsigned int max_wide = 2;
			for (int j = 0; j < max_row_size; j++) {
				if (data_table[j][i] &&
					data_table[j][i]->stringify().length() > max_wide) {
					max_wide = data_table[j][i]->stringify().length();
				}
			}
			col_max_wide[i] = max_wide;
		}

		//맨 상단에 열 정보 표시

		total_table += "    ";
		int total_wide = 4;
		for (int i = 0; i < max_col_size; i++) {
			if (col_max_wide[i]) {
				int max_len = max(2, col_max_wide[i]);
				total_table += " | " + col_num_to_str(i);
				total_table += repeat_char(max_len - col_num_to_str(i).length(), ' ');

				total_wide += (max_len + 3);
			}
		}


		total_table += "\n";
		//일단 기본적으로 최대 9999 번째 행 까지 지원한다고 생각한다. 

		for (int i = 0; i < max_row_size; i++) {
			total_table += repeat_char(total_wide, '-');
			total_table += "\n" + to_string(i + 1);
			total_table += repeat_char(4 - to_string(i + 1).length(), ' ');


			for (int j = 0; j < max_col_size; j++) {
				if (col_max_wide[j]) {
					int max_len = max(2, col_max_wide[j]);

					string s = "";

					if (data_table[i][j]) {
						s = data_table[i][j]->stringify();
					}

					total_table += " | " + s;
					total_table += repeat_char(max_len - s.length(), ' ');
				}
			}

			total_table += "\n";
		}

		return total_table;
	}


	string TxtTable::repeat_char(int n, char c) {
		string s = "";
		for (int i = 0; i < n; i++) s.push_back(c);

		return s;
	}

	//숫자로 된 열 번호를 A,B, ..... Z, AA,AB,.... 이런 순으로 매겨준다. 
	string TxtTable::col_num_to_str(int n) {
		string s = "";

		if (n < 26) {
			s.push_back('A' + n);
		}
		else {
			char first = 'A' + n / 26 - 1;
			char second = 'A' + n % 26;


			s.push_back(first);
			s.push_back(second);
		}

		return s;
	}


	HtmlTable::HtmlTable(int row, int col) : Table(row, col) {}


	string HtmlTable::print_table() {
		string s = "<table board = '1' cellpadding = '10'>";

		for (int i = 0; i < max_row_size; i++) {
			s += "<tr>";
			for (int j = 0; j < max_col_size; j++) {
				s += "<td>";

				if (data_table[i][j])  s += data_table[i][j]->stringify();
				s += "</td>";
			}
			s += "</tr>";
		}

		s += "</table>";

		return s;
	}


	CSVTable::CSVTable(int row, int col) : Table(row, col) {}

	string CSVTable::print_table() {
		string s = "";

		for (int i = 0; i < max_row_size; i++) {
			for (int j = 0; j < max_col_size; j++) {
				if (j >= 1) s += ","; 
				//CSV 파일 규칙에 따라 문자열에 큰따옴표가 포함되어 있다면 ""로
				//치환한다.

				string temp; 
				if (data_table[i][j]) temp = data_table[i][j]->stringify(); 

				for (int k = 0; k < temp.length(); k++) {
					if (temp[k] == '"') {
						//k의 위치에 " 한 개 더 집어 넣는다. 
						temp.insert(k, 1, '"'); 

						//이미 추가된 "를 다시 확읺나느 일이 없게 하기 위해 
						//k를 한칸 더 이동시킨다. 
						k++; 
					}
				}
				temp = '"' + temp + '"'; 
				s += temp; 
			}
			s += '\n'; 
		}
		return s; 
	}
}