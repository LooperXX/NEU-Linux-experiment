//
// Created by looper on 18-10-14.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <dnet.h>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <curses.h> //这个就是我们要用到的额外的ui库
#include <stdlib.h>
#include <zconf.h>
#include <vector>
#include <map>
#include <set>
#include <signal.h>
#include <limits>

#define MAX_X 50   //场地宽
#define MAX_Y 30  //场地长
#define CORNER_X 4  //左上角x坐标
#define CORNER_Y 2   //左上角y坐标

using namespace std;

class point {
public:
    point(int xx = 0, int yy = 0) {
        x = xx;
        y = yy;
    }

    string point_string() {
        return to_string(x) + "," + to_string(y);
    }

    int x;
    int y;
};

int tcp_socket;
int menu_num = 3;
string thread_id_s;
string nickName;
map<int, pair<string, string>> playerMap; // 用户编号 <用户线程号 (用户名称，用户房间号)>
vector<int> linkList;
vector<int> link_list;
map<int, pair<string, string>> wait_list; // 等待列表编号　用户线程号,用户名称
map<string, vector<point>> snakes; // <线程号,蛇>
map<string, point> tail;
map<string, string> dir;// <线程号,方向>
point food;
bool food_state;
vector<point> foods;
vector<string> menu;

int Length = 4;  //初始蛇长
int lie = 4;
int life = 1;    //是否还活着
int input = 0;   //记录键盘按键的ascii
pthread_t key_id;    //声明一个linux线程，按键等待线程
pthread_t wait_id;

void FoodCheck();//检查是否吃到了食物

void FoodProduce();//生成一个食物

void Initializition();//初始化线程，进行蛇的初始设定，创建第一个食物　画场地

void SnakeHeadMovement();//移动蛇

void DeathCheck();//检查是否满足死亡条件

void Paint();//画蛇画食物

void *waitForKey(void *);//这个是另一个线程的函数定义

void drawDot(int x, int y, char s);//画点

void clearDot(int x, int y);//清楚点喽

void end();//程序的结束工作

void GameOn();

void update_playerMap();

void print_playerMap(int);

void print_wait_list(int);

vector<string> splitWithStl(const string &, const string &);

void update();

void snake2foods(string);

void init_menu();

void printMenu(int);

bool has_in_linkList(int);

void *waitForStart(void *);

void kill_waitForStart();

int main(int ac, char *av[]) {
    // connect to the server
    struct sockaddr_in addr{};
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); // IPv4 使用 AF_INET
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av[2])); // 从终端输入端口号；网络字节序转换，htons 适用于    16 位，htonl 适用于 32 位
    addr.sin_addr.s_addr = inet_addr(av[1]);
    if (connect(tcp_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("cannot connect");
        exit(1);
    }
    char thread_id[100];
    read(tcp_socket, thread_id, sizeof(thread_id));
    thread_id_s = thread_id;
    cout << "[Info]Your id : " << thread_id_s << endl;
    cout << "[Info]Please input your NickName : ";
    cin >> nickName;
    string nickName_ = "0;" + nickName;
    write(tcp_socket, nickName_.data(), nickName_.size());
    bool flag = false;
    init_menu();
    while (!flag) {
        int key = 0;
        int input_choice = 0;
        int input;
        do {
            printMenu(key);
            input = cin.get();
            if (input == 'q') {
                break;
            } else if (input == 'w') {
                key = (key - 1 + menu_num + 1) % (menu_num + 1);
            } else if (input == 's') {
                key = (key + 1) % (menu_num + 1);
            } else if (input == '\r') {
                input_choice = key;
                break;
            }
        } while (1);

        char buf[1024] = {0};
        switch (input_choice) {
            case 0: {
                update_playerMap();
                getch();
                clear();
                break;
            }
            case 1: {
                link_list.clear();
                linkList.clear();
                update_playerMap();
                int input_link;
                int key_link = 0;

                do {
                    print_playerMap(key_link);
                    input_link = cin.get();
                    if (input_link == 'q') {
                        break;
                    } else if (input_link == 'w') {
                        key_link = (key_link - 1 + link_list.size()) % link_list.size();
                    } else if (input_link == 's') {
                        key_link = (key_link + 1) % link_list.size();
                    } else if (input_link == '\r') {
                        if (!has_in_linkList(link_list[key_link])) {
                            linkList.push_back(link_list[key_link]);
                        }
                    }
                } while (1);

                if (linkList.empty()) {
                    clear();
                    cout << "[Info]Invite nobody." << endl;
                    getch();
                    clear();
                    break;
                }

                string s = "2;";
                for (auto &itor : linkList) {
                    if (playerMap[itor].first != thread_id_s)
                        s += playerMap[itor].first + ";";
                }
                //cout << s << endl;
                write(tcp_socket, s.data(), s.size());
                read(tcp_socket, buf, sizeof(buf));
                //cout << buf << endl;
                string buf_s = buf;
                if (buf_s == "LinkS") {
                    //cout << "[Info]Please wait other players." << endl;
                    pthread_create(&wait_id, nullptr, waitForStart, nullptr);
                    char res[1024];
                    int n = read(tcp_socket, res, sizeof(res));
                    if (n <= 0) {
                        kill_waitForStart();
                        clear();
                        cout << "[Wrong]MainClient GameOn Error" << endl;
                        getch();
                        clear();
                        break;
                    }
                    string res_s = res;
                    //cout << res_s << endl;
                    if (res_s == "No") {
                        kill_waitForStart();
                        clear();
                        cout << "[Wrong]Link Failed" << endl;
                        getch();
                        clear();
                        break;
                    } else if (res_s == "GameOn") {
                        //TODO:发出邀请了 等待一起游戏～
                        kill_waitForStart();
                        clear();
                        GameOn();
                    }
                } else {
                    clear();
                    cout << "[Wrong]Server reject your link." << endl;
                    getch();
                    clear();
                }
                break;
            }
            case 2: {
                update_playerMap();
                clear();
                printMenu(2);
                string req = "3;";
                write(tcp_socket, req.data(), req.size());
                read(tcp_socket, buf, 1024);
                req = buf;
                if (req == "NoInvitations") {
                    cout << "\n[Info]No Invitations" << endl;
                    getch();
                    clear();
                    break;
                }
                vector<string> inviters = splitWithStl(req, ";");
                //cout << "Wait for you to accept" << endl;
                wait_list.clear();
                int i = 0;
                for (auto &itor : inviters) {
                    if (itor.empty())
                        continue;
                    string id = itor.substr(0, itor.find(','));
                    string name = itor.substr(itor.find(',') + 1);
                    for (auto &item : playerMap) {
                        if (item.second.first == id) {
                            //cout << item.first << "\t" << name << endl;
                            wait_list[i++] = make_pair(id, name);
                            break;
                        }
                    }
                }
                int wait_list_index = 0;
                do {
                    print_wait_list(wait_list_index);
                    input = cin.get();
                    if (input == 'q') {
                        wait_list_index = -1;
                        break;
                    } else if (input == 'w') {
                        wait_list_index = (wait_list_index - 1 + wait_list.size()) % wait_list.size();
                    } else if (input == 's') {
                        wait_list_index = (wait_list_index + 1) % wait_list.size();
                    } else if (input == '\r') {
                        break;
                    }
                } while (1);


                req = "4;";
                //cout << "[Info]Please input the index of the player you want to link with : ";
                if (wait_list_index == -1) {
                    req += "No;";
                    write(tcp_socket, req.data(), req.size());
                    clear();
                    cout << "[Info]You reject all the invitations" << endl;
                    getch();
                    clear();
                    break;
                } else {
                    req += wait_list[wait_list_index].first + ";";
                    write(tcp_socket, req.data(), req.size());
                    pthread_create(&wait_id, nullptr, waitForStart, nullptr);
                    // TODO: 接受邀请了 开始一起游戏
                    char buf2[100] = {0};
                    int n = read(tcp_socket, buf2, sizeof(buf2));
                    if (n <= 0) {
                        kill_waitForStart();
                        clear();
                        cout << "[Error]GameOn Error" << endl;
                        getch();
                        clear();
                        break;
                    }
                    string buf2_s = buf2;
                    //cout << buf2_s << endl;
                    if (buf2_s == "GameOn") {
                        kill_waitForStart();
                        clear();
                        GameOn();
                    } else {
                        kill_waitForStart();
                        clear();
                        cout << "[Error]GameOn Error2" << endl;
                        getch();
                        clear();
                    }
                }
                break;
            }
            case 3:
                echo();
                endwin();
                cout << "\nMy star guide your path.Goodbye my friend." << endl;
                flag = true;
                break;
            default:
                cout << "[Error]Input Wrong" << endl;
                break;
        }
    }
    write(tcp_socket, "-1;Goodbye", 10);
    close(tcp_socket);
    exit(1);
}

void kill_waitForStart() {
    pthread_cancel(wait_id);
    pthread_join(wait_id, nullptr);
}

void printMenu(int key) {
    for (int i = 0; i < menu.size(); i++) {
        mvprintw(i, 0, (to_string(i) + " : ").data());
        if (i != key) {
            mvprintw(i, 4, menu[i].data());
        } else {
            attrset(A_BLINK | COLOR_PAIR(1));
            mvprintw(i, 4, menu[i].data());
            attroff(A_BLINK | COLOR_PAIR(1));
        }
    }
    refresh();//刷新画布
}

void init_menu() {
    menu.emplace_back("Show online player");
    menu.emplace_back("Link Start");
    menu.emplace_back("Check Invitations");
    menu.emplace_back("Quit");
    initscr();//curses初始化
    noecho();//默认不将输入的字符显示在屏幕上
    start_color();
    curs_set(0);
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    clear();
}

bool has_in_linkList(int index) {
    for (auto &itor : linkList) {
        if (itor == index)
            return true;
    }
    return false;
}

void print_wait_list(int key) {
    clear();
    printMenu(2);
    mvprintw(menu_num + 1, 0, "Wait_index\tUser_name");
    for (int i = 0; i < wait_list.size(); i++) {
        if (i != key) {
            mvprintw(menu_num + 2 + i, 0, (to_string(i) + "\t\t" + wait_list[i].second).data());
        } else {
            attrset(A_BLINK | COLOR_PAIR(2));
            mvprintw(menu_num + 2 + i, 0, (to_string(i) + "\t\t" + wait_list[i].second).data());
            attroff(A_BLINK | COLOR_PAIR(2));
        }
        i++;
    }
    refresh();
}

void print_playerMap(int key) {
    clear();
    printMenu(1);
    mvprintw(menu_num + 1, 0, "User_index\tUser_name\tUser_roomNum");
    int k = 1;
    for (auto &itor : playerMap) {
        if (itor.second.first == thread_id_s) {
            continue;
        }
        link_list.push_back(itor.first);
        string str = itor.second.second;
        string name = str.substr(0, str.find(','));
        string room = str.substr(str.find(',') + 1);
        string res = to_string(itor.first) + "\t\t" + name + "\t\t" + room;
        if (!has_in_linkList(itor.first)) {
            if (link_list[key] == itor.first) {
                attrset(A_BLINK | COLOR_PAIR(2));
                mvprintw(menu_num + 1 + k, 0, res.data());
                attroff(A_BLINK | COLOR_PAIR(2));
            } else {
                mvprintw(menu_num + 1 + k, 0, res.data());
            }
        } else {
            attrset(A_BOLD);
            mvprintw(menu_num + 1 + k, 0, res.data());
            attroff(A_BOLD);
        }
        k++;
    }

    refresh();
}

void update_playerMap() {
    char buf[1024] = {0};
    write(tcp_socket, "1;", 10);
    int n = read(tcp_socket, buf, sizeof(buf));
    if (n <= 0) {
        //cout << "[Error]Server Error" << endl;
        exit(1);
    }
    //cout << buf << endl;
    string list = buf;
    vector<string> list_ = splitWithStl(list, ";");
    int i = 1;
    playerMap.clear();
    mvprintw(menu_num + 1, 0, "User_index\tUser_name\tUser_roomNum");
    for (auto &itor : list_) {
        if (!itor.empty()) {
            string str = itor.substr(itor.find(',') + 1);
            string pid = itor.substr(0, itor.find(','));
            string name = str.substr(0, str.find(','));
            string room = str.substr(str.find(',') + 1, str.find(';'));
            string res = to_string(i) + "\t\t" + name + "\t\t" + room;
            mvprintw(menu_num + 1 + i, 0, res.data());
            //cout << i << "\t" << itor.substr(itor.find(',') + 1) << endl;
            playerMap[i++] = make_pair(pid, name + "," + room);
        }
    }
    refresh();
}

vector<string> splitWithStl(const string &str, const string &pattern) {
    vector<string> resVec;

    if (str.empty()) {
        return resVec;
    }
    //方便截取最后一段数据
    string strs = str + pattern;

    size_t pos = strs.find(pattern);
    size_t size = strs.size();

    while (pos != string::npos) {
        string x = strs.substr(0, pos);
        resVec.push_back(x);
        strs = strs.substr(pos + 1, size);
        pos = strs.find(pattern);
    }

    return resVec;
}

void GameOn() {
    clear();
    snakes.clear();
    food_state = false;
    life = 1;
    Initializition();
    refresh();//刷新food画布
    while (life) {
        Paint();
        usleep(300000);       //刷新频率是0.3秒
        update();
        SnakeHeadMovement();  //移动蛇
        DeathCheck();         //判断是否死亡
    }
    //　TODO:游戏结束
    end();
    clear();
}

void update() {
    string move;
    if (food_state) {
        move += food.point_string() + ";";
        food_state = false;
    } else {
        move += "FoodStill;";
    };
    move += dir[thread_id_s] + ";";
    //mvprintw(MAX_Y + CORNER_Y + 3, 4, move.data());
    write(tcp_socket, move.data(), move.size());
    char buf[1024];
    read(tcp_socket, buf, sizeof(buf));
    string buf_ = buf;
    //mvprintw(MAX_Y + CORNER_Y + 4, 4, buf);
    vector<string> buf_v = splitWithStl(buf_, ";");
    food.x = atoi(buf_v[0].substr(0, buf_v[0].find(',')).data());
    food.y = atoi(buf_v[0].substr(buf_v[0].find(',') + 1).data());
    vector<string> dir_v = splitWithStl(buf_v[1], ",");
    for (auto &itor : dir_v) {
        dir[itor.substr(0, itor.find('|'))] = itor.substr(itor.find('|') + 1);
    }
}

void *waitForStart(void *para) {
    clear();
    int i = 0;
    while (1) {
        move(i, i + i);
        if (i % 2 == 1)
            standout();
        addstr("Please wait other players");
        if (i % 2 == 1)
            standend();
        refresh();
        sleep(1);
        move(i, i + i);
        addstr("                         ");
        i = (i + 1) % LINES;
    }
}

void *waitForKey(void *para) {
    while (life) {
        usleep(1000); //为什么要加这个，不知道什么原因，在curses下，如果建了这个线程并且不加这句话的话就会出现花屏现象。很难看
        input = getch();
        switch (input) {
            case 119:
                dir[thread_id_s] = "w";
                break;
            case 100:
                dir[thread_id_s] = "d";
                break;
            case 115:
                dir[thread_id_s] = "s";
                break;
            case 97:
                dir[thread_id_s] = "a";
                break;
            default:
                dir[thread_id_s] = "k";
                break;
        }
        //drawDot(-2, -2, input);
    }
}

void end() {
    pthread_cancel(key_id);
    pthread_join(key_id, nullptr);
    move(0, 0);
    addstr("Press any key to quit!");
    refresh();
    getch();
}

//食物的随机产生

void FoodProduce() {
    bool flag = false;
    srand(time(nullptr));
    do {
        food.x = (rand() % ((MAX_X - 2) / 2)) * 2 + 2; // 2 to MAX_X-2  and is 偶数
        food.y = rand() % (MAX_Y - 1) + 1;  // 1 to MAX_Y-1
        for (auto &itor : snakes) {
            for (auto &item : itor.second) {
                if (food.x == item.x && food.y == item.y)
                    flag = true;
            }
        }
    } while (flag);/*直到没有重合*/
}

//蛇身和食物的初始化 初始化的蛇身为4节长度

void Initializition() {
    //接受初始化数据
    char buf[1024] = {0};
    write(tcp_socket, "OK", 5);
    int n = read(tcp_socket, buf, sizeof(buf));
    if (n <= 0) {
        cout << "[Error]Client can't receive the init data" << endl;
    }
    string buf_ = buf;
    //cout << buf_ << endl;
    vector<string> buf_v = splitWithStl(buf_, ";");
    food.x = atoi(buf_v[0].substr(0, buf_v[0].find(',')).data());
    food.y = atoi(buf_v[0].substr(buf_v[0].find(',') + 1).data());
    vector<string> buf_v_snake = splitWithStl(buf_v[1], ",");
    for (auto &itor : buf_v_snake) {
        if (!itor.empty()) {
            string name = itor.substr(0, itor.find('|'));
            int snake_y = atoi(itor.substr(itor.find('|') + 1).data());
            vector<point> snakeBody;
            snakeBody.reserve(Length);
            for (int i = Length * 2; i > 0; i -= 2) {
                snakeBody.emplace_back(lie + i, snake_y);
            }
            snakes[name] = snakeBody;
            dir[name] = "k";
        }
    }
//    initscr();//curses初始化
//    noecho();//默认不将输入的字符显示在屏幕上
//    start_color();
//    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    int ret;
    ret = pthread_create(&key_id, nullptr, waitForKey, nullptr);//创建线程
    if (ret != 0) {
        exit(1);
    }
    for (int i = 0; i <= MAX_X; i += 2) { //画围墙
        drawDot(i, 0, '+');
        drawDot(i, MAX_Y, '+');
    }

    for (int i = 0; i <= MAX_Y; i++) {
        drawDot(0, i, '+');
        drawDot(MAX_X, i, '+');
    }
}


void SnakeHeadMovement() {
    int directionX, directionY;/*定义原本蛇前进的方向，可通过蛇头坐标减去蛇的第二部分*/
    int newX, newY;
    for (auto &itor : snakes) {
        newX = itor.second[0].x;
        newY = itor.second[0].y;

        directionX = itor.second[0].x - itor.second[1].x;
        directionY = itor.second[0].y - itor.second[1].y;

        if (dir[itor.first] == "w" && directionY <= 0)//不走回头路
            newY--;
        else if (dir[itor.first] == "s" && directionY >= 0)
            newY++;
        else if (dir[itor.first] == "a" && directionX <= 0)
            newX -= 2; /*因为字符高是宽的两倍*/
        else if (dir[itor.first] == "d" && directionX >= 0)
            newX += 2;
        else {
            newX += directionX;
            newY += directionY;
        }
        itor.second.insert(itor.second.begin(), point(newX, newY));
    }
    FoodCheck();
    for (auto &itor : snakes) {
        tail[itor.first] = point(itor.second[itor.second.size() - 1].x, itor.second[itor.second.size() - 1].y);
        itor.second.pop_back();
    }
}

//判断是否吃到食物,以及吃到后长度变长还有产生新的食物

void FoodCheck() {
    for (auto &itor :snakes) {
        if (food.x == itor.second[0].x && food.y == itor.second[0].y) {
            itor.second.emplace_back(0, 0);
            food_state = true;
            FoodProduce();
            break;
        }
    }
    for (auto &itor : snakes) {
        for (auto &item : foods) {
            if (item.x == itor.second[0].x && item.y == itor.second[0].y)
                itor.second.emplace_back(0, 0);
        }
    }
}

void snake2foods(string str) {
    for (auto &itor : snakes[str]) {
        foods.push_back(itor);
    }
    snakes.erase(str);
    dir.erase(str);
}

//判断是否死亡

void DeathCheck() {
    bool flag = false;
    for (auto &itor : snakes) {
        if (itor.first == thread_id_s) {//　本蛇
            if (itor.second[0].x <= 1 || itor.second[0].x >= MAX_X || itor.second[0].y <= 0 ||
                itor.second[0].y >= MAX_Y) {
                life = 0;
                break;
            } else {// 撞到自己或别人了
                for (auto &item : snakes) {
                    if (item.first == itor.first) {// 自己
                        for (int i = 3; i < item.second.size(); i++) {
                            if (itor.second[0].x == itor.second[i].x && itor.second[0].y == itor.second[i].y) {
                                life = 0;
                                flag = true;
                                break;
                            }
                        }
                    } else { // 别人
                        for (auto &i : item.second) {
                            if (itor.second[0].x == i.x && itor.second[0].y == i.y) {
                                life = 0;
                                flag = true;
                                break;
                            }
                        }
                    }
                    if (flag)
                        break;
                }
            }
        } else {//　别的蛇
            bool toFood = false;
            if (itor.second[0].x <= 1 || itor.second[0].x >= MAX_X || itor.second[0].y <= 0 ||
                itor.second[0].y >= MAX_Y) {
                snake2foods(itor.first);
            } else {// 撞到自己或别人了
                for (auto &item : snakes) {
                    if (item.first == itor.first) {// 自己
                        for (int i = 3; i < item.second.size(); i++) {
                            if (itor.second[0].x == itor.second[i].x && itor.second[0].y == itor.second[i].y) {
                                snake2foods(itor.first);
                                toFood = true;
                                break;
                            }
                        }
                    } else { // 别人
                        for (auto &i : item.second) {
                            if (itor.second[0].x == i.x && itor.second[0].y == i.y) {
                                snake2foods(itor.first);
                                toFood = true;
                                break;
                            }
                        }
                    }
                    if (toFood)
                        break;
                }
            }
        }
        if (flag)
            break;
    }
    if (life == 0) {
        write(tcp_socket, "GameOver;", 20);
        mvprintw(1, 0, "Continue to exert oneself!");
    } else if (snakes.size() == 1) {
        write(tcp_socket, "Winner;", 20);
        life = 0;
        mvprintw(1, 0, "You are the winner!");
    }
}

//排序和打印

void Paint() {
    for (auto &itor : snakes) {
        clearDot(tail[itor.first].x, tail[itor.first].y);
        if (itor.first == thread_id_s) {
            attrset(A_BOLD);
        }
        drawDot(itor.second[0].x, itor.second[0].y, '@');
        for (int i = 1; i < itor.second.size(); i++) {
            drawDot(itor.second[i].x, itor.second[i].y, '*');
        }
        if (itor.first == thread_id_s) {
            attroff(A_BOLD);
        }
    }
    attrset(A_BLINK | COLOR_PAIR(1));
    drawDot(food.x, food.y, '$');
    attroff(A_BLINK | COLOR_PAIR(1));
    move(0, 0);
    refresh();//刷新画布
}

void drawDot(int x, int y, char s) {
    move(y + CORNER_Y, x + CORNER_X);
    addch(s);
}

void clearDot(int x, int y) {
    move(y + CORNER_Y, x + CORNER_X);
    addch(' ');
}

