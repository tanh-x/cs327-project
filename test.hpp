#include <iostream>
#include <vector>

using namespace std;

template <class T>
class circular_queue {
public:
    vector<T> v;
    int size;
    int front, back;

    int enqueue(T d) {
        if (front != back) {
            v[front++]  = d;
            if (front == size) {
                front = 0;
            }
            return 0;
        }

        throw "No space left in queue";
    }

    T dequeue() {
        if (((back + 1) % size) != front) {
            back++;
            if (back == size) {
                back = 0;
            }
        }else {
            throw "Nothing to dequeue";
        }

        return v[back];
    }

    circular_queue(int size) : v(size + 1), size(size + 1), front(0), back(size) {}
    ~circular_queue() {}

    template <typename U>
    friend ostream& operator<<(ostream& os, const circular_queue<U>& q);
};

template <typename T>
ostream& operator<<(ostream& os, const circular_queue<T>& q) {
    os << "{";
    int i = q.back;
    bool first = true;
    while ((i + 1) % q.size != q.front) {
        i = (i + 1) % q.size;
        if (!first) {
            os << ",";
        }
        os << q.v[i];
        first = false;
    }
    os << "}";
    return os;
}





int main() {
    circular_queue<char> cq(5);  // Create a queue of size 5
    cq.enqueue('A');
    cq.enqueue('B');
    cq.enqueue('C');
    cq.enqueue('D');

    cout << cq << endl;  // Output should be: {A,B,C,D}

    cq.dequeue();
    cq.enqueue('E');

    cout << cq << endl;  // Output should be: {B,C,D,E}

    return 0;

    return 0;
}