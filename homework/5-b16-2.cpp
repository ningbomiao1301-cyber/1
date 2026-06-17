/*
 * 5-b16-2.cpp
 * 键盘输入10个学生的学号、姓名、成绩，按成绩升序打印不及格名单（string类）
 * 成绩相同的按输入原始顺序排（冒泡排序稳定）
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

void sort_by_score_asc(string id[], string name[], int score[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (score[j] > score[j + 1]) {
                string tmp_id   = id[j];
                string tmp_name = name[j];
                int tmp_score   = score[j];
                id[j]   = id[j + 1];   name[j]   = name[j + 1];   score[j]   = score[j + 1];
                id[j+1] = tmp_id;      name[j+1] = tmp_name;       score[j+1] = tmp_score;
            }
        }
    }
}

void output_failed(string id[], string name[], int score[], int n) {
    cout << endl << "不及格名单（按成绩升序）：" << endl;
    for (int i = 0; i < n; i++) {
        if (score[i] < 60) {
            cout << id[i] << " " << name[i] << " " << score[i] << endl;
        }
    }
}

int main() {
    string id[N], name[N];
    int score[N];

    input_students(id, name, score, N);
    sort_by_score_asc(id, name, score, N);
    output_failed(id, name, score, N);

    return 0;
}
