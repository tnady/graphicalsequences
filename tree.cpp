#include "tree.h"
#include <fstream>
#include <time.h>

void child (tree &deg, int first, int second) {
    int wall = deg.peekwall();
    char *degree = deg.peekdegree();
    char *newdegree = new char[number];
    for (int i = 0; i < number; i++) newdegree[i] = degree[i];
    newdegree[first]--;
    newdegree[second]--;
    if (wall > second) wall--;
    if (wall > first) wall--;
    bool del = true;
    if (newdegree[number - 1] >= wall) {
        if (deg.binpush(deg.getcurrlevel() + 1, wall, newdegree)) del = false;
	}
    if (del) delete[] newdegree;
}

void steps (tree &deg, int start) {
	int step = stop;
    char *degree = deg.peekdegree();
	for (int i = number - 1; i > start; i--) {
		if (degree[i] > step) {
            child(deg, start, i);
            step = degree[i];
		}
	}
}

int algorithm (tree &deg) {
    int level = deg.getlevel();
    if (!stop) level--;
    for (int i = 0; i < level; i++) {
        deg.setcurrlevel(i);
        while (deg.getsizeseq() > 0) {
            int wall = deg.peekwall();
            char *degree = deg.peekdegree();
            if (wall < number - 1) {
                if (degree[wall] > degree[wall + 1]) steps(deg, wall);
                if (degree[wall] > stop && degree[wall] == degree[wall + 1] && (wall == number - 2 || degree[wall + 1] > degree[wall + 2])) child(deg, wall, wall + 1);
            }
            if (wall > 0) {
                if (wall < number) steps(deg, wall - 1);
                if (wall > 1 && degree[wall - 1] > stop) child(deg, wall - 2, wall - 1);
            }
            deg.pop();
        }
    }
    int quantity = deg.getall();
    if (!stop) {
        if (deg.getodd()) {
            deg.setcurrlevel(level);
            int last = deg.getquantity();
            quantity = (quantity - last)*2 + last;
        }
        else quantity *= 2;
	}
    return quantity;
}

int main () {
    std::fstream output;
	output.open("result.txt", std::ios_base::out|std::ios_base::app);
	output << std::endl;
	output << "Number: " << number << std::endl;
	output << "Stop: " << stop << std::endl;
	clock_t t1 = 0;
	clock_t t2 = 0;
	int result = 0;
	if (number - 1 == stop) result = 1;
	if (number - 1 > stop) {
        tree deg;
        t1 = clock();
        result = algorithm(deg);
        t2 = clock();
	}
	output << "Result: " << result << std::endl;
	output << "Time: " << (double)(t2 - t1)/(double)CLOCKS_PER_SEC << std::endl;
	output.close();
	return 0;
}
