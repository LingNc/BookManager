#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "utils.h"
#include "terminal.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))    //只能在主函数用,指针退化,无法得出

void display_title()
{
    update_terminal_size();
    int width = (terminal.width - 87) / 2;
    mvprintw(4, width, "                __         __  ___         __    _ __               _____              ");
    mvprintw(5, width, "         ____  / /_       /  |/  /_  __   / /   (_) /_  _________ _/ ___/__  _______   ");
    mvprintw(6, width, "        / __ \\/ __ \\     / /|_/ / / / /  / /   / / __ \\/ ___/ __ `/\\__ \\/ / / / ___/   ");
    mvprintw(7, width, "       / /_/ / / / /    / /  / / /_/ /  / /___/ / /_/ / /  / /_/ /___/ / /_/ (__  )    ");
    mvprintw(8, width, "       \\____/_/ /_/    /_/  /_/\\__, /  /_____/_/_.___/_/   \\__,_//____/\\__, /____/     ");
    mvprintw(9, width, "                              /____/                                  /____/           ");
    mvprintw(LINES - 3, 0, "按 <ENTER> 选择");
    mvprintw(LINES - 2, 0, "上下选择, 退格键退出");
    refresh();
}

Screen main_menu()
{
    char *main_choices[] = {
        "管理员入口",
        "学生入口",
        "选项",
        "关于",
        "退出",
    };
    clear();
    refresh();
    display_title();
        refresh();

    int n_choices = ARRAY_SIZE(main_choices);      // 菜单选项数量
    ITEM **items = creat_items(main_choices, n_choices);// 将字符串初始话为主菜单的选项

    MENU *menu = new_menu((ITEM **)items);    // item创建完毕,创建菜单
    WINDOW *menu_win = creat_win(12, 20, 14, (terminal.width - 20) / 2);
    wborder(menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

    init_menu_in_win(menu, menu_win, 9, 15, 1, 5);
    refresh_menu(menu, menu_win);   //刷新menu

    Screen next_screen;
    int input_c; // 用户输入的按键,不能为char, 有些按键的宏大于128
    while ((input_c = wgetch(menu_win)) != KEY_BACKSPACE)
    {
        switch (input_c)
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_RESIZE:
            clear();
            display_title();
            resize_main_menu(menu, &menu_win); 
            refresh();
            break; 
        case 10: // enter键
        {
            ITEM *cur = current_item(menu);
            const char *chioce_name = item_name(cur);
            if (strcmp(chioce_name, "退出") == 0)
            {
                // 返回主界面
                destroy_menu(menu, items, menu_win);
                next_screen = EXIT;
                return next_screen;
            }else if(strcmp(chioce_name, "管理员入口") == 0)
            {
                return ADMIN_MENU;
            }else if(strcmp(chioce_name, "学生入口") == 0)
            {
                return STUDENT_MENU;
            }else if(strcmp(chioce_name, "关于") == 0)
            {
                return ABOUT;
            }

            // void (*p)(char *);
            // if(strcmp(item_name(cur_item), "exit" ) == 0)
            //     return;
            // p = item_userptr(cur);
            // p((char *)item_name(cur));
            // post_menu(main_menu);
            // refresh();
        }
        break;
        }

    }
}


Screen admin_menu()
{
    char *admin_choices[] = {
        "图书管理",
        "学生管理",
        "借书记录管理",
        "退出登录",
    };
    // 清空屏幕
    clear();
    refresh();

    ITEM **items;  
    int n_choices; 

    n_choices = ARRAY_SIZE(admin_choices);
    items = creat_items(admin_choices, n_choices);

    MENU *menu = new_menu((ITEM **)items);
    WINDOW *menu_win = creat_win(terminal.height - 2, terminal.width / 4, 1, 1);
    keypad(menu_win, TRUE);
    

    init_menu_in_win(menu, menu_win,terminal.height - 4, terminal.width / 4 - 2, 1, 1);

    refresh_menu(menu, menu_win);

    WINDOW *info = creat_win(terminal.height - 2, terminal.width - (terminal.width / 4) - 3, 1, terminal.width / 4 + 2);
    wrefresh(info);
    
    Screen next_screen;
    int input_c; // 用户输入的按键,不能为char, 有些按键的宏大于128
    while ((input_c = wgetch(menu_win)) != KEY_F(1))
    {
        switch (input_c)
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_RESIZE:
            clear();
            resize_menu(menu, &menu_win);
            refresh_win(&info);
            break; 
        case 10: // enter键
        {
            ITEM *cur = current_item(menu);
            const char *chioce_name = item_name(cur);
            if (strcmp(chioce_name, "退出登录") == 0)
            {
                // 返回主界面
                destroy_menu(menu,items, menu_win);
                next_screen = BACK;
                return next_screen;
            }else if (strcmp(chioce_name, "图书管理") == 0)
            {
                next_screen = BOOK_MANAGE;
                return next_screen;
            }else if (strcmp(chioce_name, "学生管理") == 0)
            {
                next_screen = STUDENT_MANAGE;
                return next_screen;
            }else if (strcmp(chioce_name, "借书记录管理") == 0)
            {
                next_screen = BORROW_RECORDS;
                return next_screen;
            }

            // void (*p)(char *);
            // if(strcmp(item_name(cur_item), "exit" ) == 0)
            //     return;
            // p = item_userptr(cur);
            // p((char *)item_name(cur));
            // post_menu(main_menu);
            // refresh();
        }
        break;
        }
    }

}

Screen book_manage_info()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen stu_manage_info()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}
Screen borrow_records()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen stu_menu()
{
    char *stu_choices[] = {
        "搜索图书",
        "借书",
        "还书",
        "查看借书记录",
        "退出登录",
    };
    // 清空屏幕
    clear();
    refresh();

    ITEM **items;  
    int n_choices; 

    n_choices = ARRAY_SIZE(stu_choices);
    items = creat_items(stu_choices, n_choices);

    MENU *menu = new_menu((ITEM **)items);
    WINDOW *menu_win = creat_win(terminal.height - 2, terminal.width / 4, 1, 1);
    keypad(menu_win, TRUE);
    

    init_menu_in_win(menu, menu_win,terminal.height - 4, terminal.width / 4 - 2, 1, 1);

    refresh_menu(menu, menu_win);

    WINDOW *info = creat_win(terminal.height - 2, terminal.width - (terminal.width / 4) - 3, 1, terminal.width / 4 + 2);
    wrefresh(info);
    
    Screen next_screen;
    int input_c; // 用户输入的按键,不能为char, 有些按键的宏大于128
    while ((input_c = wgetch(menu_win)) != KEY_F(1))
    {
        switch (input_c)
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_RESIZE:
            clear();
            resize_menu(menu, &menu_win);
            refresh_win(&info);
            break; 
        case 10: // enter键
        {
            ITEM *cur = current_item(menu);
            const char *chioce_name = item_name(cur);
            if (strcmp(chioce_name, "退出登录") == 0)
            {
                // 返回主界面
                destroy_menu(menu,items, menu_win);
                next_screen = BACK;
                return next_screen;
            }else if (strcmp(chioce_name, "搜索图书") == 0)
            {
                next_screen = SEARCH_BOOKS;
                return next_screen;
            }else if (strcmp(chioce_name, "借书") == 0)
            {
                next_screen = BORROW_BOOK;
                return next_screen;
            }else if (strcmp(chioce_name, "还书") == 0)
            {
                next_screen = RETURN_BOOK;
                return next_screen;
            }else if (strcmp(chioce_name, "查看借书记录") == 0)
            {
                next_screen = VIEW_RECORDS;
                return next_screen;
            }

            // void (*p)(char *);
            // if(strcmp(item_name(cur_item), "exit" ) == 0)
            //     return;
            // p = item_userptr(cur);
            // p((char *)item_name(cur));
            // post_menu(main_menu);
            // refresh();
        }
        break;
        }
    }
}

Screen search_books()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen borrow_book()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen return_book()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen view_records()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"正在施工");
    wrefresh(win);
    wgetch(win);
    return BACK;
}

Screen about()
{
    WINDOW* win = newwin(terminal.height/2,terminal.width/2, terminal.height/4,terminal.width/4);
    box(win,0,0);
    mvwprintw(win,1,1,"Oh My Labrasys 是一款专为图书管理设计的软件，界面简洁直观，功能布局清晰。");
    mvwprintw(win,2,1,"还没完呢, 该怎么些, 后面的好难");
    wrefresh(win);
    wgetch(win);
    return BACK;
}