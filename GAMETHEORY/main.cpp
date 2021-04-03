#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>

using namespace std;

struct paper
{
	void* oppo;
	bool its_pick;
	bool opp_pick;
};

class Person
{
public:
	int point;
	bool action;
	vector<paper> history;
	bool (*behavior)(Person* A, Person* B);
};

bool angel(Person* A, Person* B)
{
	return true;
}
bool devil(Person* A, Person* B)
{
	return false;
}
bool T4T(Person* A, Person* B)
{	
	for (int i = A->history.size() - 1; i >= 0; i--)
		if (A->history[i].oppo == B)
			if (A->history[i].opp_pick)
				return true;
			else
				return false;
	return true;
}
bool chaos(Person* A, Person* B)
{
	time_t t;
	const char temp = rand() % 2;
	return temp;
}
bool fried(Person* A, Person* B)
{
	for (int i = 0; i < A->history.size(); i++)
	{
		if (A->history[i].oppo == B)
		{
			return false;
		}
	}
	return true;
}

int* ndom(int size, int range)
{
	int  num = 0;
	int* rlist = (int*)malloc(sizeof(int) * size);
	std::random_device rd;
	std::mt19937::result_type seed = time(NULL);
	std::mt19937 gen(seed);
	std::uniform_int_distribution<unsigned> distrib(0, range-1);

	memset(rlist, -1, sizeof(int) * size);
	while (num < size)
	{
		bool overlap = false;
		int temp = distrib(gen);
		for (int i = 0; i < size; i++)
		{
			if (rlist[i] == temp)
			{
				overlap = true;
				break;
			}
		}
		if (!overlap)
		{
			rlist[num] = temp;
			num++;
		}
	}
	return rlist;
}

void policy(Person* A, Person* B)
{

	A->action = A->behavior(A, B);
	B->action = B->behavior(B, A);

	if (A->action && B->action)
	{
		A->point += 3;
		B->point += 3;
		paper pA;
		pA.oppo = B;
		pA.its_pick = 1;
		pA.opp_pick = 1;
		A->history.push_back(pA);
		paper pB;
		pB.oppo = A;
		pB.its_pick = 1;
		pB.opp_pick = 1;
		B->history.push_back(pB);
	}
	else if (!A->action && !B->action)
	{
		A->point += 1;
		B->point += 1;
		paper pA;
		pA.oppo = B;
		pA.its_pick = 0;
		pA.opp_pick = 0;
		A->history.push_back(pA);
		paper pB;
		pB.oppo = A;
		pB.its_pick = 0;
		pB.opp_pick = 0;
		B->history.push_back(pB);
	}
	else if (!A->action && B->action)
	{
		A->point += 5;
		B->point += 0;
		paper pA;
		pA.oppo = B;
		pA.its_pick = 0;
		pA.opp_pick = 1;
		A->history.push_back(pA);
		paper pB;
		pB.oppo = A;
		pB.its_pick = 1;
		pB.opp_pick = 0;
		B->history.push_back(pB);
	}
	else if (A->action && !B->action)
	{
		A->point += 0;
		B->point += 5;
		paper pA;
		pA.oppo = B;
		pA.its_pick = 1;
		pA.opp_pick = 0;
		A->history.push_back(pA);
		paper pB;
		pB.oppo = A;
		pB.its_pick = 0;
		pB.opp_pick = 1;
		B->history.push_back(pB);
	}
}

int main()
{
	int a =0, b=0, c=0, d=0, e=0;
	for (int i = 0; i < 500; i++)
	{
		Person type1, type2, type3, type4, type5;
		type1.behavior = angel;
		type2.behavior = devil;
		type3.behavior = T4T;
		type4.behavior = chaos;
		type5.behavior = fried;
	
		int k = 0;
		vector<Person*> _list;
		_list.push_back(&type1);
		_list.push_back(&type2);
		_list.push_back(&type3);
		_list.push_back(&type4);
		_list.push_back(&type5);
	

		while (k <10000)
		{
			Person* A, *B;
			int* random_list = ndom(2, _list.size());

			A = _list[random_list[0]];
			B = _list[random_list[1]];

			policy(A, B);
			free(random_list);
			k++;
		}
		std::printf("[%d]\t[A]\t%d\t[B]\t%d\t[C]\t%d\t[D]\t%d[E]\t%d\n", i, type1.point, type2.point, type3.point, type4.point, type5.point);
		a += type1.point;
		b += type2.point;
		c += type3.point;
		d += type4.point;
		e += type5.point;
	}
	float _a = a / 200;
	float _b = b / 200;
	float _c = c / 200;
	float _d = d / 200;
	float _e = e / 200;
	std::printf("[AVG]\t[A]\t%f\t[B]\t%f\t[C]\t%f\t[D]\t%f[E]\t%f\n", _a, _b, _c, _d, _e);

return 0;
}
