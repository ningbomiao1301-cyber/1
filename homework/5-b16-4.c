/*
 * 5-b16-4.c
 * 键盘输入10个学生的学号、姓名、成绩，按成绩降序打印全部学生（C方式）
 * 成绩相同的按输入原始顺序排（冒泡排序稳定）
 */
#include <stdio.h>
#include <string.h>

#define N 10

void input_students(char id[][8], char name[][9], int score[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("请输入第%d个学生的信息（学号 姓名 成绩）：\n", i + 1);
        scanf("%s %s %d", id[i], name[i], &score[i]);
    }
}

void sort_by_score_desc(char id[][8], char name[][9], int score[], int n) {
    int i, j;
    char tmp_id[8], tmp_name[9];
    int tmp_score;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (score[j] < score[j + 1]) {
                strcpy(tmp_id, id[j]);
                strcpy(id[j], id[j + 1]);
                strcpy(id[j + 1], tmp_id);

                strcpy(tmp_name, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], tmp_name);

                tmp_score = score[j];
                score[j] = score[j + 1];
                score[j + 1] = tmp_score;
            }
        }
    }
}

void output_all(char id[][8], char name[][9], int score[], int n) {
    int i;
    printf("\n全部学生名单（按成绩降序）：\n");
    for (i = 0; i < n; i++) {
        printf("%s %s %d\n", id[i], name[i], score[i]);
    }
}

int main(void) {
    char id[N][8];
    char name[N][9];
    int score[N];

    input_students(id, name, score, N);
    sort_by_score_desc(id, name, score, N);
    output_all(id, name, score, N);

    return 0;
}
