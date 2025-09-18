#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <set>

struct Point
{
	int x, y;
};

struct Polygon
{
	std::vector< Point > points;
};

bool operator==(const Point& a, const Point& b) {
	return (a.x == b.x) && (a.y == b.y);
}

//Функции чтения
Polygon lineToPolygon(std::string str)
{
	int pos = str.find(' ');
	int n = std::stoi(str.substr(0, pos));
	str = str.substr(pos, -1);

	int x, y;
	Polygon p;

	for (int i = 0; i < n; i++)
	{
		str = str.substr(2, -1);
		pos = str.find(';');
		x = stoi(str.substr(0, pos));
		str = str.substr(pos + 1, -1);
		pos = str.find(')');
		y = stoi(str.substr(0, pos));
		str = str.substr(pos + 1, -1);

		p.points.push_back(Point{ x, y });

	}
	return p;
}

void readPolygons(std::string fname, std::vector<Polygon>& output)
{
	std::ifstream file(fname);
	std::string str;
	while (std::getline(file, str))
	{
		Polygon p = lineToPolygon(str);
		output.push_back(p);
	}
}

//Служебные функции(используются в других)
double area(const Polygon& P)
{
	//Найдём площаь отдельной фигуры по формуле Гаусса
	double s = 0;
	std::vector<Point> points = P.points;
	std::vector<int> dets;
	int n = points.size();
	for (int i = 0; i < n-1; i++)
	{
		dets.push_back(points[i].x * points[i + 1].y - points[i + 1].x * points[i].y);
	}
	dets.push_back(points[n-1].x * points[0].y - points[0].x * points[n-1].y);
	s = accumulate(dets.begin(), dets.end(), 0.0);
	s =  std::abs(s) / 2;
	
	return s;
}

double areaVector(const std::vector<Polygon>& P)
//Найдём сумму площадей полигонов в векторе
{
	std::vector<double> temp;
	std::transform(
		P.begin(),
		P.end(),
		std::back_inserter(temp),
		[](const Polygon& p) { return area(p); }
	);
	return std::accumulate(temp.begin(), temp.end(), 0.0);
}

bool areaComp(Polygon a, Polygon b)
{
	return (area(a) < area(b));
}

bool vertexComp(Polygon a, Polygon b)
{
	return (a.points.size() < b.points.size());
}

bool xComp(Point a, Point b)
{
	return a.x < b.x;
}

bool yComp(Point a, Point b)
{
	return a.y < b.y;
}


bool isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

Point cordReverse(const Point &pt)
{
	Point res;
	res.x = pt.y;
	res.y = pt.x;

	return res;
}

void printBool(bool arg)
{
	if (arg)
		std::cout << "<TRUE>" << std::endl;
	else
		std::cout << "<FALSE>" << std::endl;
}

void findBox(const Polygon& p, int& mnX, int& mnY, int& mxX, int& mxY)
{
	Point mxXpoint = *std::max_element(p.points.begin(), p.points.end(),
		xComp);
	mxX = mxXpoint.x;
	Point mnXpoint = *std::min_element(p.points.begin(), p.points.end(),
		xComp);
	mnX = mnXpoint.x;
	Point mxYpoint = *std::max_element(p.points.begin(), p.points.end(),
		yComp);
	mxY = mxYpoint.y;
	Point mnYpoint = *std::min_element(p.points.begin(), p.points.end(),
		yComp);
	mnY = mnYpoint.y;
}

bool polygonInBox(const Polygon& p, Point lmn, Point rmx)
{
	bool res;
	res = std::all_of(p.points.begin(), p.points.end(), [&lmn, &rmx](Point pt)
		{
			return ((pt.x >= lmn.x) && (pt.y >= lmn.y) && (pt.x <= rmx.x) && (pt.y <= rmx.y));
		});
	return res;
}

//Функции из задания (общие)
double areaEO(const std::vector<Polygon>& P, bool mode) //mode - режим работы : true - чёт, false - нечёт 
{
	//Реализация комманд AREA EVEN/ODD
	std::vector<Polygon> temp;
	std::copy_if(P.begin(), P.end(), std::back_inserter(temp), [&mode](Polygon p) {return ((p.points.size() % 2) != mode);});
	return areaVector(temp);
}

double areaMean(const std::vector<Polygon>& P)
{
	//Реализация команды AREA MEAN
	if (P.size() == 0)
		return 0.0;
	double s = areaVector(P);
	return s / P.size();
}

double areaNOV(const std::vector<Polygon>& P, int n)
{
	//Реализация команды AREA <NUMBER-OF-VERTEXES>
	std::vector<Polygon> temp;
	std::copy_if(P.begin(),
		P.end(),
		std::back_inserter(temp),
		[&n](Polygon p)
		{
			return (p.points.size() == n ); 
		});
	return areaVector(temp);
}

double maxArea(const std::vector<Polygon>& P)
{
	//Реализация команды MAX AREA
	Polygon pMax = *std::max_element(P.begin(), P.end(), areaComp);
	return area(pMax);
}

double  minArea(const std::vector<Polygon>& P)
{
	//Реализация команды MIN AREA
	Polygon pMin = *std::min_element(P.begin(), P.end(), areaComp);
	return area(pMin);
}

int maxVertexes(const std::vector<Polygon>& P)
{
	//Реализация команды MAX VERTEXES
	Polygon pMax = *std::max_element(P.begin(), P.end(), vertexComp);
	return pMax.points.size();
}

int minVertexes(const std::vector<Polygon>& P)
{
	//Реализация команды MIN VERTEXES
	Polygon pMin = *std::min_element(P.begin(), P.end(), vertexComp);
	return pMin.points.size();
}

int countEO(const std::vector<Polygon>& P, bool mode)
{
	//Реализация команды COUNT EVEN/ODD
	int c = count_if(P.begin(), P.end(), [&mode](Polygon p) {return ((p.points.size() % 2) != mode);});
	return c;
}

int countNOV(const std::vector<Polygon>& P, int n)
{
	//Реализация команды COUNT <NUMBER-OF-VERTEXES>
	int c = count_if(P.begin(), P.end(), [&n](Polygon p) {return (p.points.size() == n); });
	return c;
}

//Функции из задания (12 вариант)

int perms(const std::vector<Polygon>& vp, Polygon p)
{
	//Реализация команды PERMS
	int res = 0;
	std::vector<Polygon> cvp;

	std::copy_if(vp.begin(), vp.end(), std::back_inserter(cvp), [&p](Polygon _p) {return (_p.points.size() == p.points.size()); });

	if (cvp.size() != 0)
	{
		res = std::count_if(cvp.begin(), cvp.end(), [&p](Polygon _p)
			{
				return std::all_of(_p.points.begin(), _p.points.end(), [&p](Point pt)
					{
						return (
							(std::find(p.points.begin(), p.points.end(), pt) != p.points.end())
							|| ((std::find(p.points.begin(), p.points.end(), cordReverse(pt)) != p.points.end())));
					});
			});
	}

	return res;
}

bool inframe(const std::vector<Polygon>& vp, Polygon p)
{
	//Реализация команды INFRAME
	Polygon combined;

	std::for_each(vp.begin(), vp.end(), [&combined](Polygon _p)
		{
			combined.points.insert(combined.points.end(), _p.points.begin(), _p.points.end());
		});

	int mnX, mnY, mxX, mxY;
	findBox(combined, mnX, mnY, mxX, mxY);

	return polygonInBox(p, Point{ mnX, mnY }, Point{ mxX, mxY});
}

int main(int args, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	std::cout << std::fixed << std::setprecision(1);

	if (args < 2)
	{
		std::cout << "Ошибка: Неправильные аргументы" << std::endl;
		return 1;
	}

	std::string filename = argv[1];
	std::vector<Polygon> vp;
	readPolygons(filename, vp);
	if (vp.size() == 0)
	{
		std::cout << "Ошибка: Файл пуст или его не существует" << std::endl;
		return -1;
	}

	std::string cmd;
	while (std::getline(std::cin, cmd))
	{
		std::string cmdS = cmd.substr(0, cmd.find(' '));
		if (cmdS.length() == cmd.length())
		{
			std::cout << "<INVALID COMMAND>" << std::endl;
		}
		else
		{
			cmd = cmd.substr(cmd.find(' ') + 1, -1);
			
			if (cmdS == "AREA")
			{
				if (cmd == "EVEN")
				{
					std::cout << areaEO(vp, 1) << std::endl;
				}
				else if (cmd == "ODD")
				{
					std::cout << areaEO(vp, 0) << std::endl;
				}
				else if (isNumber(cmd))
				{
					std::cout << areaNOV(vp, std::stoi(cmd)) << std::endl;
				}
				else
				{
					std::cout << "<INVALID COMMAND>" << std::endl;
				}
			}
			else if (cmdS == "MAX")
			{
				if (cmd == "AREA")
				{
					std::cout << maxArea(vp) << std::endl;
				}
				else if (cmd == "VERTEXES")
				{
					std::cout << maxVertexes(vp) << std::endl;
				}
				else
					std::cout << "<INVALID COMMAND>" << std::endl;
			}
			else if (cmdS == "MIN")
			{
				if (cmd == "AREA")
				{
					std::cout << minArea(vp) << std::endl;
				}
				else if (cmd == "VERTEXES")
				{
					std::cout << minVertexes(vp) << std::endl;
				}
				else
					std::cout << "<INVALID COMMAND>" << std::endl;
			}
			else if (cmdS == "COUNT")
			{
				if (cmd == "EVEN")
				{
					std::cout << countEO(vp, 1) << std::endl;
				}
				else if (cmd == "ODD")
				{
					std::cout << countEO(vp, 0) << std::endl;
				}
				else if (isNumber(cmd))
				{
					std::cout << countNOV(vp, std::stoi(cmd)) << std::endl;
				}
				else
					std::cout << "<INVALID COMMAND>" << std::endl;
			}
			else if (cmdS == "PERMS")
			{
				try
				{
					Polygon tmp = lineToPolygon(cmd);
					std::cout << perms(vp, tmp) << std::endl;
				}
				catch (int errcode)
				{
					std::cout << "<INVALID COMMAND>" << std::endl;
				}
			}
			else if (cmdS == "INFRAME")
			{
				try
				{
					Polygon tmp = lineToPolygon(cmd);
					printBool(inframe(vp, tmp));
				}
				catch (int errcode)
				{
					std::cout << "<INVALID COMMAND>" << std::endl;
				}
			}
			else
			std::cout << "<INVALID COMMAND>" << std::endl;
		}
	}
	std::cout << "<END OF INPUT>" << std::endl;
	return 0;
}
