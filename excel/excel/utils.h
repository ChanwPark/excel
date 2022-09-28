#pragma once

#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include <string>
#include <time.h>
using std::string;
using namespace std;

namespace MyExcel {
	class Vector {
	private:
		string* data;
		int capacity;
		int length;

	public:
		//생성자

		Vector(int n = 1);

		//맨 뒤에 새로운 원소를 추가한다.
		void push_back(string s);

		//임의의 위치에 원소에 접근한다. 
		string operator[](int i);


		//x 번째 위치한 원소를 제거한다.
		void remove(int x);

		//현재 벡터의 크기를 구한다.
		int size();

		~Vector();
	};



	class Stack {
		struct Node
		{
			Node* prev;
			string s;

			Node(Node* prev, string s) : prev(prev), s(s) {}
		};

	private:
		Node* current;
		Node start;

	public:
		Stack();

		//최상단에 새로운 원소를 추가한다. 
		void push(string s);

		//최상단의 원소를 제거하고 반환한다.

		string pop();

		//최상단 원소를 반환한다 (제거 안함)

		string peek();

		//스택이 비어있는지의 유무를 반환한다. 

		bool is_empty();

		~Stack();
	};


	class Table;
	class Cell {
	public:
		Cell(string data, int x, int y, Table* table);
				
	
	protected:
		int x;
		int y;
		Table* table;
		string data; 

	public:
		virtual string stringify();
		virtual int to_numeric();
		//virtual string stringify() = 0;
		//virtual int to_numeric() = 0;		
	};
	class Cell;
	class Table
	{
	protected:
		//행 및 열의 최대 크기
		int max_row_size, max_col_size;

		//데이터를 보관하는 테이블
		//Cell* 을 보관하는 2차원 배열이라 생각하면 편하다. 

		Cell*** data_table;

	public:
		Table(int max_row_size, int max_col_size);

		~Table();

		//새로운 셀을 row 행 col 열에 등록한다.
		void reg_cell(Cell* c, int row, int col);


		//해당 셀의 정수값을 반환한다. 
		// s: 셀이름(Ex. A3, B6 같이)

		int to_numeric(const string& s);

		//행 및 열 변호로 셀을 호출한다. 
		int to_numeric(int row, int col);

		//해당 셀의 문자열을 반환한다. 
		string stringify(const string& s);
		string stringify(int row, int col);

		virtual string print_table() = 0;
	};

	class TxtTable : public Table {
		string repeat_char(int n, char c);

		//숫자로 된 열 번호를 A,B, ..... Z, AA,BB, ... 이런 순으로 매겨준다. 
		string col_num_to_str(int n);

	public:
		TxtTable(int row, int col);

		//텍스트로 표를 깨끗하게 출력해준다. 
		string print_table();
	};

	class HtmlTable : public Table {
	public:
		HtmlTable(int row, int col); 

		string print_table(); 
	};

	class CSVTable : public Table {
	public:
		CSVTable(int row, int col); 

		string print_table(); 
	};
}
#endif