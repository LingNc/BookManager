#ifndef _student_lib
#define _student_lib

#include "Tools/String.h"
#include "Tools/Type.h"
#include "Tools/Vector.h"
#include "Book.h"

#define MAX_AVAILABLE 10

typedef struct Student Student;
typedef Student *student;

struct Student {
    size_t id;
    string name;
    string class;
    string department;
    int borrowedCount;
    string borrowedDate;
    string returnDate;
    vector books; // 动态数组，存储借阅的图书信息

    // 序列化数据
    string _serialize;
    _init_default_func(student);
};

student new_student();

#endif