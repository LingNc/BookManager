#include "Book.h"
#include "Tools/String.h"
#include <string.h>
#include <stdlib.h>

// 函数声明
const char *_book_data(book this);
int _book_in_data(book this, const char *data);
book _book_copy(book this, book other);
int _book_cmp(book this, book other);
void _book_free(book this);
book new_book();

// data获得序列化数据实现
const char *_book_data(book this) {
    this->_serialize->clear(this->_serialize);
    size_t len = sizeof(size_t) + this->ISBN->length(this->ISBN) + this->name->length(this->name) +
                 this->author->length(this->author) + this->publisher->length(this->publisher) +
                 this->time->length(this->time) + sizeof(this->status);
    this->_serialize->append_n(this->_serialize, (const char *)&len, sizeof(len));

    this->_serialize->append_n(this->_serialize, this->ISBN->c_str(this->ISBN), this->ISBN->length(this->ISBN));
    this->_serialize->append_n(this->_serialize, this->name->c_str(this->name), this->name->length(this->name));
    this->_serialize->append_n(this->_serialize, this->author->c_str(this->author), this->author->length(this->author));
    this->_serialize->append_n(this->_serialize, this->publisher->c_str(this->publisher), this->publisher->length(this->publisher));
    this->_serialize->append_n(this->_serialize, this->time->c_str(this->time), this->time->length(this->time));
    this->_serialize->append_n(this->_serialize, (const char *)&this->status, sizeof(this->status));

    return this->_serialize->c_str(this->_serialize);
}

// 反序列化数据实现
int _book_in_data(book this, const char *data) {
    size_t ptr = 0;
    size_t len = 0;
    memcpy(&len, data + ptr, sizeof(len));
    ptr += sizeof(len);

    this->ISBN->assign_cstr(this->ISBN, data + ptr);
    ptr += this->ISBN->length(this->ISBN);

    this->name->assign_cstr(this->name, data + ptr);
    ptr += this->name->length(this->name);

    this->author->assign_cstr(this->author, data + ptr);
    ptr += this->author->length(this->author);

    this->publisher->assign_cstr(this->publisher, data + ptr);
    ptr += this->publisher->length(this->publisher);

    this->time->assign_cstr(this->time, data + ptr);
    ptr += this->time->length(this->time);

    memcpy(&this->status, data + ptr, sizeof(this->status));
    ptr += sizeof(this->status);

    return 1; // 表示成功
}

// 复制图书数据实现
book _book_copy(book this, book other) {
    this->ISBN->assign(this->ISBN, other->ISBN);
    this->name->assign(this->name, other->name);
    this->author->assign(this->author, other->author);
    this->publisher->assign(this->publisher, other->publisher);
    this->time->assign(this->time, other->time);
    this->status = other->status;
    return this;
}

// 比较图书数据实现
int _book_cmp(book this, book other) {
    if (other->ISBN->length(other->ISBN) > 0 && strcmp(this->ISBN->c_str(this->ISBN), other->ISBN->c_str(other->ISBN)) != 0) {
        return 1;
    }
    if (other->name->length(other->name) > 0 && strcmp(this->name->c_str(this->name), other->name->c_str(other->name)) != 0) {
        return 1;
    }
    if (other->author->length(other->author) > 0 && strcmp(this->author->c_str(this->author), other->author->c_str(other->author)) != 0) {
        return 1;
    }
    if (other->publisher->length(other->publisher) > 0 && strcmp(this->publisher->c_str(this->publisher), other->publisher->c_str(other->publisher)) != 0) {
        return 1;
    }
    if (other->time->length(other->time) > 0 && strcmp(this->time->c_str(this->time), other->time->c_str(other->time)) != 0) {
        return 1;
    }
    return 0;
}

// 释放图书数据实现
void _book_free(book this) {
    // 如果有动态分配的内存，在这里释放
    // 目前没有动态分配的内存，所以不需要做任何事情
}

// 初始化图书
book new_book() {
    book this = (book)malloc(sizeof(Book));
    if (!this) {
        perror("Book: this 指针分配失败");
        exit(EXIT_FAILURE);
    }
    this->ISBN = new_string();
    this->name = new_string();
    this->author = new_string();
    this->publisher = new_string();
    this->time = new_string();
    this->_serialize= new_string();
    this->status = 0;
    this->data = _book_data;
    this->in_data = _book_in_data;
    this->copy = _book_copy;
    this->cmp = _book_cmp;
    this->free = _book_free;
    return this;
}