const int number = 10; //vertices
const int stop = 0; //geq

class tree {
    protected:
        struct sequence {
            int wall; //right element
            char *degree; //values
            sequence *next;
            sequence *left;
            sequence *right;
        };

        bool odd; //count half
        int level; //tree levels
        int currlevel; //sequences in process here
        int *sizeseq; //current
        int *quantity; //result
        sequence **head;
        sequence **tail;
        sequence **root; //binary tree

    public:
        tree () {
            level = 0;
            if (stop) {
                for (int i = 1; i < number; i++) level += i%2? i:i - 1;
			}
			else {
                for (int i = 1; i < number; i++) level += i;
                level++;
                odd = level%2;
                level = level/2 + odd;
            }
            sizeseq = new int[level];
            quantity = new int[level];
            head = new sequence *[level];
            tail = new sequence *[level];
            root = new sequence *[level];
            for (int i = 0; i < level; i++) {
                sizeseq[i] = 0;
                quantity[i] = 0;
                head[i] = new(sequence);
                tail[i] = head[i];
                root[i] = head[i];
            }
            currlevel = 0;
            head[0]->wall = number;
            head[0]->degree = new char[number];
            for (int i = 0; i < number; i++) head[0]->degree[i] = number - 1;
            head[0]->next = 0;
            head[0]->left = 0;
            head[0]->right = 0;
			sizeseq[0] = 1;
            quantity[0] = 1;
        }

        ~tree () {
            for (int i = 0; i < level; i++) {
                while (sizeseq[i]) {
                    sequence *temp = head[i];
                    head[i] = head[i]->next;
                    delete[] temp->degree;
                    delete temp;
                    sizeseq[i]--;
                }
			}
            delete[] sizeseq;
            delete[] quantity;
            delete[] head;
            delete[] tail;
            delete[] root;
        }

        bool getodd () {
            return odd;
        }

        int getlevel () {
            return level;
        }

        int getcurrlevel () {
            return currlevel;
        }

        void setcurrlevel (int lvl) {
            currlevel = lvl;
        }

        int getsizeseq () {
            return sizeseq[currlevel];
        }

        int getquantity () {
            return quantity[currlevel];
        }

        int getall () {
            int all = 0;
            for (int i = 0; i < level; i++) all += quantity[i];
            return all;
        }

        bool binpush (int lvl, int setwall, char *setdegree) {
            if (sizeseq[lvl]) {
                sequence *current = root[lvl];
                bool look = true;
                while (look) {
                    bool check = true;
                    for (int i = 0; i < number; i++) {
                        if (current->degree[i] > setdegree[i]) {
                            if (!current->left) {
                                current->left = new(sequence);
                                look = false;
                            }
                            current = current->left;
                            check = false;
                            break;
                        }
                        if (current->degree[i] < setdegree[i]) {
                            if (!current->right) {
                                current->right = new(sequence);
                                look = false;
                            }
                            current = current->right;
                            check = false;
                            break;
                        }
                    }
                    if (check) return false;
                }
                current->wall = setwall;
                current->degree = setdegree;
                current->next = 0;
                current->left = 0;
                current->right = 0;
                tail[lvl]->next = current;
                tail[lvl] = current;
            }
            else {
                head[lvl]->wall = setwall;
                head[lvl]->degree = setdegree;
                head[lvl]->next = 0;
                head[lvl]->left = 0;
                head[lvl]->right = 0;
            }
            sizeseq[lvl]++;
            quantity[lvl]++;
            return true;
        }

        void pop () {
            sequence *temp = head[currlevel];
            head[currlevel] = head[currlevel]->next;
            delete[] temp->degree;
            delete temp;
            sizeseq[currlevel]--;
        }

        int peekwall () {
            return head[currlevel]->wall;
        }

        char *peekdegree () {
            return head[currlevel]->degree;
        }
};
