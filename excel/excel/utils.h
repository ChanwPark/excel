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
		//������

		Vector(int n = 1);

		//�� �ڿ� ���ο� ���Ҹ� �߰��Ѵ�.
		void push_back(string s);

		//������ ��ġ�� ���ҿ� �����Ѵ�. 
		string operator[](int i);


		//x ��° ��ġ�� ���Ҹ� �����Ѵ�.
		void remove(int x);

		//���� ������ ũ�⸦ ���Ѵ�.
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

		//�ֻ�ܿ� ���ο� ���Ҹ� �߰��Ѵ�. 
		void push(string s);

		//�ֻ���� ���Ҹ� �����ϰ� ��ȯ�Ѵ�.

		string pop();

		//�ֻ�� ���Ҹ� ��ȯ�Ѵ� (���� ����)

		string peek();

		//������ ����ִ����� ������ ��ȯ�Ѵ�. 

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
		//�� �� ���� �ִ� ũ��
		int max_row_size, max_col_size;

		//�����͸� �����ϴ� ���̺�
		//Cell* �� �����ϴ� 2���� �迭�̶� �����ϸ� ���ϴ�. 

		Cell*** data_table;

	public:
		Table(int max_row_size, int max_col_size);

		~Table();

		//���ο� ���� row �� col ���� ����Ѵ�.
		void reg_cell(Cell* c, int row, int col);


		//�ش� ���� �������� ��ȯ�Ѵ�. 
		// s: ���̸�(Ex. A3, B6 ����)

		int to_numeric(const string& s);

		//�� �� �� ��ȣ�� ���� ȣ���Ѵ�. 
		int to_numeric(int row, int col);

		//�ش� ���� ���ڿ��� ��ȯ�Ѵ�. 
		string stringify(const string& s);
		string stringify(int row, int col);

		virtual string print_table() = 0;
	};

	class TxtTable : public Table {
		string repeat_char(int n, char c);

		//���ڷ� �� �� ��ȣ�� A,B, ..... Z, AA,BB, ... �̷� ������ �Ű��ش�. 
		string col_num_to_str(int n);

	public:
		TxtTable(int row, int col);

		//�ؽ�Ʈ�� ǥ�� �����ϰ� ������ش�. 
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