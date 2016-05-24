#include "tree.h"

class partree : public tree {
    private:
        int lastfull; //after paralg start here
        sequence **now; //move next no pop

    public:
        partree () : tree () {
            now = new sequence *[level];
            for (int i = 0; i < level; i++) now[i] = head[i];
            lastfull = 0;
        }

        ~partree () {
            delete[] now;
        }

        int getlastfull () {
            return lastfull;
        }

        void setlastfull (int lvl) {
            lastfull = lvl;
        }

        int getsizeseq (int lvl) {
            return sizeseq[lvl];
        }

        int getquantity (int lvl) {
            return quantity[lvl];
        }

        void pop (int lvl) {
            sequence *temp = head[lvl];
            head[lvl] = head[lvl]->next;
            delete[] temp->degree;
            delete temp;
            sizeseq[lvl]--;
        }

		void clearlevel (int lvl) {
			while (sizeseq[lvl]) {
				sequence *temp = head[lvl];
				head[lvl] = head[lvl]->next;
				delete[] temp->degree;
				delete temp;
				sizeseq[lvl]--;
            }
		}

		void nowstep (int lvl) {
			now[lvl] = now[lvl]->next;
		}

		void nowloop (int lvl) {
			now[lvl] = head[lvl];
		}

        int getwall (int lvl) {
            return now[lvl]->wall;
        }

        char *getdegree (int lvl) {
            return now[lvl]->degree;
        }
};
