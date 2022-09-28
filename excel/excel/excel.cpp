// excel.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "utils.h"
#include <fstream> 
#include <iostream>


using namespace MyExcel; 

int main()
{
	TxtTable table(5, 5);
	std::ofstream out("text.txt"); 

	table.reg_cell(new Cell("Hello~", 0, 0, &table), 0, 0); 
	table.reg_cell(new Cell("C++", 0, 1, &table), 0, 1); 

	table.reg_cell(new Cell("Programming", 1, 1, &table), 1, 1); 
	std::cout << std::endl << table.print_table(); 
	out << table.print_table(); 


	HtmlTable table_html(5, 5); 
	std::ofstream outhtml("text.html");

	table_html.reg_cell(new Cell("Hell~", 0, 0, &table_html), 0, 0);
	table_html.reg_cell(new Cell("C++", 0, 1, &table_html), 0, 1);

	table_html.reg_cell(new Cell("Programming", 1, 1, &table_html), 1, 1);
	std::cout << std::endl << table_html.print_table();
	outhtml << table_html.print_table();


	CSVTable table_csv(5, 5);
	std::ofstream outcsv("text.csv");

	table_csv.reg_cell(new Cell("Hell~", 0, 0, &table_csv), 0, 0);
	table_csv.reg_cell(new Cell("C++", 0, 1, &table_csv), 0, 1);

	table_csv.reg_cell(new Cell("Programming", 1, 1, &table_csv), 1, 1);
	std::cout << std::endl << table_csv.print_table();

	outcsv << table_csv.print_table();
}

