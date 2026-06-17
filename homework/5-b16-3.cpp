/*
 * 5-b16-3.cpp
 * 键盘输入10个学生的学号、姓名、成绩，按学号升序打印全部学生（string类）
 * 允许使用 C++ string 类和 #include <string>
 */
#include <iostream>
#include <string>
using namespace std;

#define N 10

void input_students(string id[], string name[], int score[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "请输入第" << (i + 1) << "个学生的信息（学号 姓名 成绩）：" << endl;
        cin >> id[i] >> name[i] >> score[i];
    }
}

void sort_by_id_asc(string id[], string name[], int score[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (id[j] > id[j + 1]) {
                string tmp_id   = id[j];
                string tmp_name = name[j];
                int tmp_score   = score[j];
                id[j]   = id[j + 1];   name[j]   = name[j + 1];   score[j]   = score[j + 1];
                id[j+1] = tmp_id;      name[j+1] = tmp_name;       score[j+1] = tmp_score;
            }
        }
    }
}

void output_all(string id[], string name[], int score[], int n) {
    cout << endl << "全部学生名单（按学号升序）：" << endl;
    for (int i = 0; i < n; i++) {
        cout << id[i] << " " << name[i] << " " << score[i] << endl;
    }
}

int main() {
    string id[N], name[N];
    int score[N];

    input_students(id, name, score, N);
    sort_by_id_asc(id, name, score, N);
    output_all(id, name, score, N);

    return 0;
}
