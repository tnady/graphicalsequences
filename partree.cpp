#include "partree.h"
#include <fstream>
#include <time.h>
#include <omp.h>

void parchild (partree &deg, int lvl, int first, int second) {
    int wall = deg.getwall(lvl);
    char *degree = deg.getdegree(lvl);
    char *newdegree = new char[number];
    for (int i = 0; i < number; i++) newdegree[i] = degree[i];
    newdegree[first]--;
    newdegree[second]++;
    if (first == wall - 1) wall--;
    if (!deg.binpush(lvl, wall, newdegree)) delete[] newdegree;
}

void child (partree &deg, bool skip, int lvl, int first, int second) {
    int wall = deg.getwall(lvl);
    char *degree = deg.getdegree(lvl);
    char *newdegree = new char[number];
    for (int i = 0; i < number; i++) newdegree[i] = degree[i];
    newdegree[first]--;
    newdegree[second]--;
    if (wall > second) wall--;
    if (wall > first) wall--;
    bool del = true;
    if (skip) {
        if (deg.binpush(lvl + 1, wall, newdegree)) del = false;
    }
    else {
        if (newdegree[number - 1] > wall)
            if (deg.binpush(lvl + 1, wall, newdegree)) del = false;
    }
    if (del) delete[] newdegree;
}

void steps (partree &deg, bool skip, int lvl, int start) {
	int step = stop;
	char *degree = deg.getdegree(lvl);
	for (int i = number - 1; i > start; i--)
		if (degree[i] > step) {
            if (skip) child(deg, false, lvl, start, i);
            else {
                if (start + 1 == degree[i]) child(deg, true, lvl, start, i);
            }
            step = degree[i];
		}
}

void paralg (partree &deg) {
    #pragma omp parallel for
    for (int i = 0; i < deg.getlevel(); i++) {
        deg.nowloop(i);
		int s = 0;
        while (s < deg.getsizeseq(i)) {
            int wall = deg.getwall(i);
            char *degree = deg.getdegree(i);
            for (int j = 0; j < number - 1; j++)
                if (degree[j] > degree[j + 1]) {
                    int step = 0;
					if (degree[j] - 1 == degree[j + 1]) step = j + 1;
					else step = j;
                    for (int k = j + 1; k < number; k++)
                        if (degree[step] > degree[k]) {
                            parchild(deg, i, j, k);
                            step = k;
                        }
                }
			deg.nowstep(i);
            s++;
        }
		if (i < deg.getlastfull()) deg.clearlevel(i);
    }
}

void oldalg (partree &deg) {
    for (int i = deg.getlastfull(); i < deg.getlevel(); i++) {
		while (deg.getsizeseq(i) > 0) {
            deg.nowloop(i);
            int wall = deg.getwall(i);
            char *degree = deg.getdegree(i);
            if (wall < number - 1) {
                if (degree[wall] > degree[wall + 1]) steps(deg, true, i, wall);
                if (degree[wall] > stop && degree[wall] == degree[wall + 1] && (wall == number - 2 || degree[wall + 1] > degree[wall + 2])) child(deg, false, i, wall, wall + 1);
            }
            if (wall > 0) {
                if (wall < number) steps(deg, true, i, wall - 1);
                if (wall > 1 && degree[wall - 1] > stop) child(deg, false, i, wall - 2, wall - 1);
            }
            deg.pop(i);
        }
    }
}

void maxoldalg (partree &deg) {
    int level = deg.getlevel();
    if (!stop) level--;
    for (int i = 0; i < level; i++) {
		deg.nowloop(i);
        int s = 0;
        while (s < deg.getsizeseq(i)) {
            int wall = deg.getwall(i);
            char *degree = deg.getdegree(i);
            if (wall < number - 1) {
                if (degree[wall] > degree[wall + 1]) steps(deg, false, i, wall);
                if (degree[wall] > stop && degree[wall] == degree[wall + 1] && (wall == number - 2 || degree[wall + 1] > degree[wall + 2])) {
                    if (wall + 1 == degree[wall + 1]) child(deg, true, i, wall, wall + 1);
                }
            }
            if (wall > 0) {
                if (wall < number) steps(deg, false, i, wall - 1);
                if (wall > 1 && degree[wall - 1] > stop) {
                    if (wall - 1 == degree[wall - 1]) child(deg, true, i, wall - 2, wall - 1);
                }
            }
            deg.nowstep(i);
            s++;
        }
        if (!s) {
            deg.setlastfull(i - 1);
            break;
        }
    }
}

int process () {
    partree deg;
    maxoldalg(deg);
    paralg(deg);
    int quantity = 0;
    if (stop) {
		oldalg(deg);
        quantity = deg.getall();
	}
	else {
        quantity = deg.getall();
        if (deg.getodd()) {
			int last = deg.getquantity(deg.getlevel() - 1);
            quantity = (quantity - last)*2 + last;
        }
        else quantity *= 2;
    }
    return quantity;
}

int main () {
    std::fstream output;
	output.open ("result.txt", std::ios_base::out|std::ios_base::app);
	output << std::endl;
	output << "Number: " << number << std::endl;
	output << "Stop: " << stop << std::endl;
	clock_t t1 = 0;
	clock_t t2 = 0;
	int result = 0;
	if (number - 1 == stop) result = 1;
	if (number - 1 > stop) {
        t1 = clock();
        result = process();
        t2 = clock();
	}
	output << "Result: " << result << std::endl;
	output << "Time: " << (double)(t2 - t1)/(double)CLOCKS_PER_SEC << std::endl;
	output.close();
	return 0;
}
