/*
 * 5-b16-1.c
 * 键盘输入10个学生的学号、姓名、成绩，按学号降序打印及格名单（C方式）
 * 学号：char id[10][8]（最长7位+'\0'）
 * 姓名：char name[10][9]（最长4个汉字GBK=8字节+'\0'）
 * 排序：冒泡排序（稳定），按学号字符串降序
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

void sort_by_id_desc(char id[][8], char name[][9], int score[], int n) {
    int i, j;
    char tmp_id[8], tmp_name[9];
    int tmp_score;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (strcmp(id[j], id[j + 1]) < 0) {
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

void output_passed(char id[][8], char name[][9], int score[], int n) {
    int i;
    printf("\n及格名单（按学号降序）：\n");
    for (i = 0; i < n; i++) {
        if (score[i] >= 60) {
            printf("%s %s %d\n", id[i], name[i], score[i]);
        }
    }
}

int main(void) {
    char id[N][8];
    char name[N][9];
    int score[N];

    input_students(id, name, score, N);
    sort_by_id_desc(id, name, score, N);
    output_passed(id, name, score, N);

    return 0;
}
