#include<list>

std::list<int> operator+(const std::list<int>& set1, const std::list<int>& set2)
{
	if (set1.empty())
	{
		return set2;
	}

	if (set2.empty())
	{
		return set1;
	}

	std::list<int> result;
	auto it1 = set1.cbegin(), it2 = set2.cbegin();
	while (it1 != set1.cend() && it2 != set2.cend())
	{
		if (*it1 == *it2)
		{
			result.push_back(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			result.push_back(*it1);
			++it1;
		}
		else
		{
			result.push_back(*it2);
			++it2;
		}
	}

	for (; it1 != set1.cend(); ++it1)
	{
		result.push_back(*it1);
	}

	for (; it2 != set2.cend(); ++it2)
	{
		result.push_back(*it2);
	}

	return result;
}


std::list<int> operator*(const std::list<int>& set1, const std::list<int>& set2)
{
	std::list<int> emp;

	if (set1.empty() || set2.empty())
	{
		return emp;
	}
	std::list<int> result;
	auto it1 = set1.cbegin(), it2 = set2.cbegin();
	while (it1 != set1.cend() && it2 != set2.cend())
	{
		if (*it1 == *it2)
		{
			result.push_back(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return result;
}

std::list<int> operator-(const std::list<int>& set1, const std::list<int>& set2)
{
	std::list<int> emp;

	if (set1.empty())
	{
		return emp;
	}

	if (set2.empty())
	{
		return set1;
	}
	std::list<int> result;
	auto it1 = set1.cbegin(), it2 = set2.cbegin();
	while (it1 != set1.cend() && it2 != set2.cend())
	{
		if (*it1 == *it2)
		{
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			result.push_back(*it1);
			++it1;
		}
		else
		{
			++it2;
		}
	}

	for (; it1 != set1.cend(); ++it1)
	{
		result.push_back(*it1);
	}

	return result;
}
