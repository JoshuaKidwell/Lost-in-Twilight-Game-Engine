#include "Animator.h"
#include <iostream>

Animator::Animator()
{
	full_w = 10;
	full_h = 10;
	sheet_sizes = { {10,10} };
	sheet_counts = { 1 };
	setPosSheet();
	current = getSheet(0, 0);
	looping_count = 0;
}

void Animator::setAnimator(int fullW, int fullH, std::vector<std::pair<int, int>> sheetSizes, std::vector<int> sheetCounts)
{
	full_w = fullW;
	full_h = fullH;
	sheet_sizes = sheetSizes;
	sheet_counts = sheetCounts;
	setPosSheet();
	current = getSheet(0, 0);
	looping_count = 0;
}

void Animator::setPosSheet()
{
	sheet_pos.clear();
	int x;
	int y;
	int start_y = 0;
	int mod_width;

	// this just gives you all positions of the animations
	for (int i = 0; i < sheet_sizes.size(); i++) {
		std::vector<std::pair<int, int>> single_sheet_pos;
		mod_width = 99999; //increase this if your width is bigger than 99999 pixels
		y = start_y;
		for (int c = 0; c < sheet_counts[i]; c++) {
			x = (sheet_sizes[i].first * c + c) % mod_width;
			if (x + sheet_sizes[i].first > full_w) {
				mod_width = c;
				x = (sheet_sizes[i].first * c + c) % mod_width;
			}
			if (x == 0 && c != 0) {
				y += sheet_sizes[i].second + 1;
			}
			if (y + sheet_sizes[i].second - 1 > full_h) {
				std::cout << "Error... Exceeding Height" << std::endl;
				break;
			}
			single_sheet_pos.push_back({ x,y });
		}
		sheet_pos.push_back(single_sheet_pos);
		start_y = y + sheet_sizes[i].second + 1;
	}
}

void Animator::loop(int order)
{
	//sets current to next count animation or loops to count = 0
	current.second = { sheet_sizes[order] };
	if (looping_count + 1 < sheet_pos[order].size()) {
		current.first = { sheet_pos[order][++looping_count] };
	}
	else {
		looping_count = 0;
		current.first = { sheet_pos[order][0] };
	}
}

std::pair<std::pair<int, int>, std::pair<int, int>> Animator::getSheet(int order, int count)
{
	return { sheet_pos[order][count], sheet_sizes[order] };
}
