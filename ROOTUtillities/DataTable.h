#pragma once
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <vector>
#include <iomanip>
#define X_DATA RootUtilities::DataTable::varType::X
#define Y_DATA RootUtilities::DataTable::varType::Y
#define X_ERROR_DATA RootUtilities::DataTable::varType::XError
#define Y_ERROR_DATA RootUtilities::DataTable::varType::YError
namespace RootUtilities{

	class DataTable
	{
	public:
		enum class varType
		{
			X,
			Y,
			XError,
			YError
		};
		DataTable()
		{
			xErrorOn = false;
			yErrorOn = false;
			length = 0;
			x = nullptr;
			y = nullptr;
			xError = nullptr;
			yError = nullptr;
		}
		~DataTable()
		{
			free(x);
			x = nullptr;
			free(y);
			y = nullptr;
			free(xError);
			xError = nullptr;
			free(yError);
			yError = nullptr;
			length = 0;
			xName.clear();
			yName.clear();
		}
		int32_t get_length() const
		{
			return length;
		}
		double* get_x()
		{
			return x;
		}
		double* get_y()
		{
			return y;
		}
		double* get_xError()
		{
			return xError;
		}
		double* get_yError()
		{
			return yError;
		}
		double get_xMin()
		{
			double min;
			if (length > 0)
			{
				min = x[0];
			}
			else return 0;
			for (int32_t i = 0; i < length; ++i)
			{
				if (x[i] < min)
					min = x[i];
			}
			return min;
		}
		double get_xMax()
		{
			double max;
			if (length > 0)
			{
				max = x[0];
			}
			else return 0;
			for (int32_t i = 0; i < length; ++i)
			{
				if (x[i] > max)
					max = x[i];
			}
			return max;
		}
		std::string get_xName()
		{
			return xName;
		}
		std::string get_yName()
		{
			return yName;
		}
		void set_xName(const std::string& name)
		{
			xName = name;
		}
		void set_yName(const std::string& name)
		{
			yName = name;
		}
		template<class ... Args>
		int32_t load_from_csv(const std::string& fileName, const std::string& range, Args ...varsOrder) /*Range format - xx:xx, */
		{
			std::ifstream inf(fileName);
			if (!inf.is_open())
			{
				std::cout << "Can't open file!";
				return -1;
			}
			std::string::const_iterator i = std::find(range.begin(), range.end(), ':');
			if (i == range.end())
			{
				std::cout << "Incorrect format of range!";
				inf.close();
				return -1;
			}
			std::string topLeft(range.begin(),i);
			++i;
			std::string bottomRight(i, range.end());
			std::pair<int32_t, int32_t> topLeftCoords = str_to_coords(topLeft);
			std::pair<int32_t, int32_t> bottomRightCoords = str_to_coords(bottomRight);


			std::vector<varType> orderOfVars{ varsOrder... };
			if (orderOfVars.size() != (bottomRightCoords.first - topLeftCoords.first + 1))
			{
				std::cout << "Range doesn't match with flags!";
				inf.close();
				return -1;
			}
			length = bottomRightCoords.second - topLeftCoords.second + 1;
			x = static_cast<double*>(realloc(x, sizeof(double) * length));
			y = static_cast<double*>(realloc(y, sizeof(double) * length));
			xError = static_cast<double*>(realloc(xError, sizeof(double) * length));
			yError = static_cast<double*>(realloc(yError, sizeof(double) * length));

			int32_t linenumber = 1;
			
			while (!inf.eof())
			{	
				char line[256];
				if (linenumber >= topLeftCoords.second && linenumber <= bottomRightCoords.second)
				{
					for (int32_t i = 1; i <= bottomRightCoords.first; ++i)
					{
						inf.getline(line, 256, ';');
						if (i >= topLeftCoords.first)
						{
							comma_to_dot(line);
							switch (orderOfVars.at(i-topLeftCoords.first))
							{
							case X_DATA:
								x[linenumber - topLeftCoords.second] = std::stof(line);
								break;
							case Y_DATA:
								y[linenumber - topLeftCoords.second] = std::stof(line);
								break;
							case X_ERROR_DATA:
								xError[linenumber - topLeftCoords.second] = std::stof(line);
								break;
							case Y_ERROR_DATA:
								yError[linenumber - topLeftCoords.second] = std::stof(line);
								break;
							default:
								break;
							}
						}
						
					}
					inf.getline(line, 256);
				}
				else
				{
					inf.getline(line, 256);
				}
				++linenumber;
				
			}
			inf.close();
			
			return 0;
		}
		void print_table()
		{
			std::cout << "X - " << xName<<'\n';
			std::cout << "Y - " << yName << '\n';
			std::cout << std::scientific;
			std::cout << std::setprecision(3);
			std::cout << "    X        XError        Y        YError" << '\n';
			for (int32_t i = 0; i < length; ++i)
			{
				std::cout << x[i] << "  " << xError[i] << "  " << y[i] << "  " << yError[i] << '\n';
			}
		}
	private:
		double* x, * y, * xError, * yError;
		int32_t length;
		std::string xName, yName;
		bool xErrorOn, yErrorOn;

		std::pair<int32_t, int32_t> str_to_coords(const std::string& str)
		{
			std::pair<int32_t, int32_t> res;
			std::string::const_iterator i = str.cbegin();
			while (i != str.end())
			{
				if ((*i) >= '0' && (*i) <= '9')
				{
					break;
				}
				++i;
			}
			if (i - str.begin() == 1)
			{
				res.first = (*(i-1)) - 'A' + 1;
			}
			else
			{
				res.first = ((*(i - 2)) - 'A' + 1) * 26 + ((*(i - 1)) - 'A' + 1);
			}
			std::string tmp(i,str.end());
			res.second = std::stoi(tmp.c_str());
			return res;
		}
		void comma_to_dot(char* str)
		{
			for (int32_t i = 0; i < strlen(str); ++i)
			{
				if (str[i] == ',')
				{
					str[i] = '.';
				}
			}
		}
	};
}