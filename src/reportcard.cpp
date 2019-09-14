#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <numeric>
#include <iomanip>

/*
Class that generates a report card based on comma seperated values
*/
class ReportCard
{
private:
	std::map<int, std::pair<std::string, std::string> > courses;
	std::multimap<int, std::pair<int, int> > marks;
	std::map<int, std::pair<int, int> > tests;

	//Reads and parses csv file into a 2 dimensional vector
	std::vector<std::vector<std::string> > readFile(std::string file)
	{
		std::string record;
		std::vector<std::vector<std::string> > cells;
		std::ifstream iFile(file.c_str());
		std::getline(iFile, record);                             //skip first line

        if(iFile.is_open())
		{
			int i = 0;
			while (std::getline(iFile, record))
			{
				std::stringstream ss(record);
				std::string cell;
				cells.resize(i + 1);
				while (std::getline(ss, cell, ','))
				{
					cells[i].push_back(cell);
				}
				i++;
			}
			iFile.close();
		}
		else
		{
			std::cerr << "Error unable to open: " << file << std::endl;
		}
		return cells;
	}

	//Calculates the final grade for each course a student takes
	std::map<int, double> calcCourseGrades(int studentId)
	{
		std::map<int, double> courseGrades;

		auto range = marks.equal_range(studentId);                       // tests that the student took
		for (auto it = range.first; it != range.second; it++)
		{
			int testId = it->second.first;
			int weight = tests[testId].second;
			int courseId = tests[testId].first;
			int mark = it->second.second;
			courseGrades[courseId] += (mark * weight * 1.0) / 100;
		}
		return courseGrades;
	}

public:
	ReportCard(std::string c, std::string m, std::string t, std::string s, std::string path)
	{
		updateCourses(c);
		updateMarks(m);
		updateTests(t);
		createReportCard(s, path);

	}
	//Converts each files parsed data into a map
	/*********************************************************************/
	void updateCourses(std::string file)
	{
		std::vector<std::vector<std::string> > cells = readFile(file);
		for (int i = 0; i < cells.size(); i++)
		{
			int courseId = std::stoi(cells[i][0]);
			std::string name = cells[i][1];
			std::string teacher = cells[i][2];
			courses[courseId] = std::make_pair(cells[i][1], cells[i][2]);
		}
	}
	void updateMarks(std::string file)
	{
		std::vector<std::vector<std::string> > cells = readFile(file);
		for (int i = 0; i < cells.size(); i++)
		{
			int testId = std::stoi(cells[i][0]);
			int studentId = std::stoi(cells[i][1]);
			int mark = std::stoi(cells[i][2]);
			marks.insert({ studentId, std::make_pair(testId, mark) });
		}
	}
	void updateTests(std::string file)
	{
		std::vector<std::vector<std::string> > cells = readFile(file);
		for (int i = 0; i < cells.size(); i++)
		{
			int testId = std::stoi(cells[i][0]);
			int courseId = std::stoi(cells[i][1]);
			int weight = std::stoi(cells[i][2]);
			tests[testId] = std::make_pair(courseId, weight);
		}
	}
	/*********************************************************************/

	//Creates and outputs report card for all studnts on file
	void createReportCard(std::string stuFile, std::string outDest)
	{
	    std::string destination = outDest;
		std::ofstream oFile(destination.c_str());

		if(oFile.is_open())
		{
			std::vector<std::vector<std::string> > cells = readFile(stuFile);
			for (int i = 0; i < cells.size(); i++)
			{
				int studentId = std::stoi(cells[i][0]);
				if (marks.find(studentId) == marks.end())
				{
					std::cerr << "Student Id: " << studentId << " has not taken any courses" << std::endl;
					break;
				}

				std::string name = cells[i][1];
				std::map<int, double> courseGrades = calcCourseGrades(studentId);
				double totalAvg = std::accumulate(courseGrades.begin(), courseGrades.end(), 0.0,
					[](double total, std::pair<int, double> grade) { return total + grade.second; }) / courseGrades.size();

				oFile << std::fixed;
				oFile << std::setprecision(2);
				oFile << "Student Id: " << studentId << ", name: " << name << std::endl;
				oFile << "Total Average:\t" << totalAvg << "%\n" << std::endl;

				std::map<int, double>::iterator it;
				for (it = courseGrades.begin(); it != courseGrades.end(); it++)
				{
					int courseId = it->first;
					double finalGrade = it->second;
					oFile << "\tCourse: " << courses[courseId].first << ", Teacher " << courses[courseId].second << std::endl;
					oFile << "\tFinal Grade:  " << finalGrade << "% \n" << std::endl;
				}
				oFile << std::endl;
			}
			oFile.close();
		}
		else
		{
			std::cerr << "Error unable to open: " << outDest << std::endl;
		}
		
	}
};

int main(int argc, char *argv[])
{
	std::string coursesFile = "courses.csv";
	std::string marksFile = "marks.csv";
	std::string testsFile = "tests.csv";
	std::string studentsFile = "students.csv";
	
    if(argc == 1)
	    ReportCard card(coursesFile, marksFile, testsFile, studentsFile, "report-card");
	else if(argc == 2)
	    ReportCard card(coursesFile, marksFile, testsFile, studentsFile, argv[1]);

}
