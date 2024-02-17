#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>

class Animator
{
public:
	Animator();
	Animator(const Animator&);
	void setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts);
	void setPosSheet();
	void loop(int order);
	std::pair<std::pair<int, int>, std::pair<int, int>> getSheet(int order, int count);
	std::pair<std::pair<int, int>, std::pair<int, int>> current;
	int looping_count;
	Animator& operator=(Animator&);
private:
	int full_w;
	int full_h;
	std::vector<std::pair<int, int>> sheet_sizes;
	std::vector<int> sheet_counts;
	std::vector<std::vector<std::pair<int, int>>> sheet_pos;
};

#endif