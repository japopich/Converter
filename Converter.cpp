/*************************************************************************/
/*																		 */
/*								Converter								 */
/*							  Jason Popich								 */
/*																		 */
/*			ASCII Space Delimited Tables to << Delimited Tables			 */
/*																		 */
/*																		 */
/*************************************************************************/
#include "stdafx.h"
#include "stdlib.h"
#include "Converter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#ifndef __cplusplus
typedef unsigned char bool;
static const bool False = 0;
static const bool True = 1;
#endif

using namespace std;

std::string word_delimiter(const std::string& word, int start, int bits) {
	if (!word.size()) {
		return "No valid input detected!";
	}
	std::stringstream ss;
	for (int i = start; i < start + bits; i++) {
		ss << word[i];
	}
	return ss.str();
}

std::string word_formatter_1D(std::string breakpoint_set_1, std::string data_string, int rows)
{
	std::stringstream ss;
	std::string delimiter = "<<";

	size_t pos = 0;
	size_t d_pos = 0;

	std::string token;
	std::string d_token;
	while ((pos = breakpoint_set_1.find(delimiter)) != std::string::npos) 
	{
		token = breakpoint_set_1.substr(0, pos);
		breakpoint_set_1.erase(0, pos + delimiter.length());

		if ((d_pos = data_string.find(delimiter)) != std::string::npos) 
		{
			d_token = data_string.substr(0, d_pos);
			data_string.erase(0, d_pos + delimiter.length());
		}

		ss << " << " << token << "<< " << d_token << "\n";

	}

	std::string s = ss.str();

	return s;
}

std::string word_formatter_2D(std::string breakpoint_set_1, std::string breakpoint_set_2, std::string data_string, int breakpoint_rows_num, int breakpoint_cols_num)
{
	bool breakpoint_1_done = false;
	bool row_data_first_pass = false;
	bool row_first_pass = false;
	
	std::stringstream ss;
	std::stringstream b_set_1;

	std:string d_token;
	std::string token;
	size_t pos = 0;
	size_t d_pos = 0;

	std::string delimiter = "<<";

	int row_count = 0;
	int col_count_data = 0;
	int row_count_data = 0;

	double data_function = 0;

	double	**data_array = new double*[breakpoint_rows_num];
	for (int i = 0; i < breakpoint_rows_num; i++)
	{
		data_array[i] = new double[breakpoint_cols_num];
	}

	while ((d_pos = data_string.find(delimiter)) != std::string::npos)
	{
		d_token = data_string.substr(0, d_pos);
		data_string.erase(0, d_pos + delimiter.length());

		if (col_count_data == breakpoint_rows_num)
		{
			col_count_data = 0;
			row_count_data++;
		}

		if (row_data_first_pass != false)
		{
			data_function = stod(d_token);

			data_array[col_count_data][row_count_data] = data_function;
			col_count_data++;
		}else
		{
			row_data_first_pass = true;
		}

	}

	ss << "\t" << breakpoint_set_2 << "\n";

	while ((pos = breakpoint_set_1.find(delimiter)) != std::string::npos)
	{
		token = breakpoint_set_1.substr(0, pos);
		breakpoint_set_1.erase(0, pos + delimiter.length());

		if (row_first_pass == true)
		{
			ss << " <<" << token;
			for (int j = 0; j < breakpoint_cols_num; j++)
			{
				ss << "<< " << data_array[row_count][j] << " ";
			}
			row_count++;
			ss << "\n";
		}
		else
		{
			row_first_pass = true;
		}
	}

	std::string s = ss.str();

	delete[] data_array;
	data_array = NULL;

	return s;
}


int main()
{

	std::string file_name;
	std::string type;
	std:string enter;

	double line_count = 0;

	double breakpoint_1_num = 0;
	double breakpoint_1_c = 0;

	double breakpoint_2_num = 0;
	double breakpoint_2_c = 0;

	double data_col_end = 0;

	bool is_1D_Table;								// Is it a 1D-Table? True or False 
	bool is_2D_Table;								// Is it a 2D-Table? True or False

	cout << "Enter File Name: ";
	cin >> file_name;

	ifstream input(file_name);
	std::ofstream out("Out.txt");

	if (!input.is_open())
	{
		cout << "\nError Opening File...\n" << endl;
		return 0;
	}

	cout << "What type of table is this? (1D or 2D): ";
	cin >> type;

	cout << "Enter file line count (Notepad++ or VS): ";
	cin >> line_count;

	if (type == "1D")
	{
		is_1D_Table = true;
		is_2D_Table = false;	

		cout << "How many Breakpoints? (Set 1): ";
		cin >> breakpoint_1_num;

		cout << "How many Breakpoints are on the last line of Breakpoint Set 1?: ";
		cin >> breakpoint_1_c;

		cout << "How many Data points are on the last line of the File?: ";
		cin >> data_col_end;

	}
	else if (type == "2D")
	{
		is_1D_Table = false;
		is_2D_Table = true;

		cout << "How many Breakpoints? (Set 1) [ROW]: ";
		cin >> breakpoint_1_num;

		cout << "How many Breakpoints are on the last line of Breakpoint Set 1?: ";
		cin >> breakpoint_1_c;

		cout << "How many Breakpoints? (Set 2) [COL]: ";
		cin >> breakpoint_2_num;

		cout << "How many Breakpoints are on the last line of Breakpoint Set 2?: ";
		cin >> breakpoint_2_c;

		cout << "How many Data points are on the last line of the File?: ";
		cin >> data_col_end;


	}

	stringstream breakpoint_1_stream;
	stringstream breakpoint_2_stream;
	stringstream data_stream;

	string breakpoint_set_1;
	string breakpoint_set_2;
	string data_set;

	//double input_data[43][5];
	double row = ((line_count - 1) / 2) + 1;
	double col = 5;									// Basically... Permanently 5 for every data source...

	double	**input_data = new double*[row];		// Dynamic Array with 2 Inputs [Row] [Col]
	for (int i = 0; i < row; i++)
	{
		input_data[i] = new double[col];
	}


	bool breakpoint_1_first = false;

	double breakpoint_1_r = (round(((breakpoint_1_num + 1) - breakpoint_1_c) / 5) + 1);

	//double breakpoint_1_num = ((breakpoint_1_r * 5) - breakpoint_1_c) - 1;

	double breakpoint_2_begin = breakpoint_1_r + 1;
	double breakpoint_2_r = (round(((breakpoint_2_num + 1) - breakpoint_2_c) / 5) + 1);
	if (is_1D_Table == true)
	{
		breakpoint_2_r = 0;
	}

	//double breakpoint_2_num = ((breakpoint_2_r * 5) - breakpoint_2_c) + 1;

	double data_row_begin = (breakpoint_1_r + breakpoint_2_r) + 1;
	double data_rows = (row - data_row_begin) + 1;

	if (input.is_open())
	{
		while (!input.eof())
		{
			for (int i = 0; i<row; i++)
			{
				for (int j = 0; j<col; j++)
				{
					if (i == (breakpoint_1_r - 1))
					{
						if (j == (breakpoint_1_c))
						{
							i++;
							j = 0;
						}
					}
					if (is_2D_Table == 1)
					{
						if (i == ((breakpoint_1_r + breakpoint_2_r) - 1))
						{
							if (j == (breakpoint_2_c))
							{
								i++;
								j = 0;

							}
						}
					}
					input >> input_data[i][j];
					//cout << "I: " << i <<" J: " << j << " " << input_data[i][j] << endl;
				}
			}
		}
		input.close();
	}
	else
	{
		cout << "Error Opening File..." << endl;
	}

	/*************************************************************************************/
	/*							SEPARATING BREAKPOINTS FROM TABLE		   				 */
	/*************************************************************************************/

	//----------------------------------//
	//		1D TABLES & 2D Tables		//
	//----------------------------------//

	for (int i = 0; i<breakpoint_1_r; i++)
	{
		if (i == (breakpoint_1_r - 1))
		{
			for (int j = 0; j<breakpoint_1_c; j++)
			{
				breakpoint_1_stream << "<< " << input_data[i][j] << " ";
			}
		}
		else
		{
			for (int j = 0; j<col; j++)
			{
				breakpoint_1_stream << "<< " << input_data[i][j] << " ";
			}

		}
	}

	breakpoint_1_stream << " <<";
	breakpoint_set_1 = breakpoint_1_stream.str();
	//cout << breakpoint_set_1;

	//----------------------------------//
	//		 ONLY FOR 2D TABLES			//
	//----------------------------------//

	if (is_2D_Table == 1)
	{
		for (int i = breakpoint_2_begin - 1; (i < breakpoint_2_begin + breakpoint_2_r); i++)
		{
			for (int j = 0; j<col; j++)
			{
				if (i != (breakpoint_1_r + breakpoint_2_r))
				{
					breakpoint_2_stream << "<< " << input_data[i][j] << " ";
					
					if (i == (breakpoint_2_begin + (breakpoint_2_r - 1)) - 1)
					{
						if (j == (breakpoint_2_c - 1))
						{
							j = col;
						}
					}
				}
			}
		}

		/*breakpoint_2_stream << " <<";*/
		breakpoint_set_2 = breakpoint_2_stream.str();
	}

	/*************************************************************************************/
	/*							PULLING OF DATA AFTER BREAKPOINTS		   				 */
	/*************************************************************************************/

	for (int i = data_row_begin - 1; (i < data_row_begin + data_rows); i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i != ((breakpoint_1_r + breakpoint_2_r) + data_rows))
			{
				data_stream << "<< " << input_data[i][j] << " ";

				if (i == ((breakpoint_2_begin + (breakpoint_2_r - 1) + data_rows) - 1))
				{
					if (j == (data_col_end - 1))
					{
						j = col;
					}
				}
			}
		}
	}

	data_stream << " <<";
	data_set = data_stream.str();

	//cout << data_set;

	/*************************************************************************************/
	/*						FORMATTING OF DATA AND OUTPUT				   				 */
	/*************************************************************************************/
	
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	//----------------------------------//
	//			  1D TABLES				//
	//----------------------------------//

	if (is_1D_Table == true)
	{
		cout << word_formatter_1D(breakpoint_set_1, data_set, row);
	}

	//----------------------------------//
	//			  2D TABLES				//
	//----------------------------------//

	if (is_2D_Table == true)
	{
		cout << word_formatter_2D(breakpoint_set_1, breakpoint_set_2, data_set, breakpoint_1_num, breakpoint_2_num);
	}

	std::cout.rdbuf(coutbuf);

	delete[] input_data;										// Clearing Memory of Dynamic Array
	input_data = NULL;

	return 0;
}
