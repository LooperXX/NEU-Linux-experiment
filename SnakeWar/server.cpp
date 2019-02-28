//
// Created by looper on 18-10-14.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <cstdio>
#include <zconf.h>
#include <bits/stdc++.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <CLucene.h>

#define MAX_X 50   //场地宽
#define MAX_Y 30  //场地长

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

int n;
map<pthread_t, pair<string, int>> playerMap; // 用户线程号 用户昵称 用户房间号
map<pthread_t, map<pthread_t, int>> invitations;// 用户线程号 邀请者的线程号 拒绝/未处理/接受
map<int, map<pthread_t, string>> rooms; //房间号 <房间的用户的线程号,蛇的移动方向>
map<int, int> update_flags;
map<int, int> init_flags;
map<int, point> foods;// 房间号 食物的位置
string playerMap_s;
pthread_mutex_t playerMap_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rooms_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t invitations_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foods_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t init_flags_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t update_flags_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_flag = PTHREAD_COND_INITIALIZER;
pthread_cond_t game_flag = PTHREAD_COND_INITIALIZER;
pthread_cond_t init_flag = PTHREAD_COND_INITIALIZER;
pthread_cond_t update_flag = PTHREAD_COND_INITIALIZER;


void *child_thread(void *);

vector<string> splitWithStl(const string &, const string &);

string playerMapToString();

pthread_t stringTopthread_t(const string &);

int main(int ac, char *av[]) {
    srand((unsigned) time(nullptr));
    int tcp_socket;
    struct sockaddr_in addr{};
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av[1])); // 从终端输入端口号；网络字节序转换，htons 适用于    16 位，htonl 适用于 32 位
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(tcp_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("cannot bind");
        exit(1); // 被占用或无该端口权限
    }
    listen(tcp_socket, 1);

    while (1) {
        int fd = accept(tcp_socket, nullptr, nullptr);
//        if (playerMap.size() > 3) {
//            cout << "[Error]Too many rooms!" << endl;
//            write(fd, "[Error]Too many rooms!", 25);
//            close(fd);
//        } else {
            pthread_t t;
            pthread_create(&t, nullptr, child_thread, &fd);
            pthread_mutex_lock(&playerMap_lock);
            pthread_mutex_lock(&invitations_lock);
            playerMap[t] = make_pair("NickName", -1);
            map<pthread_t, int> invitations_map;
            invitations[t] = invitations_map;
            pthread_mutex_unlock(&invitations_lock);
            pthread_mutex_unlock(&playerMap_lock);
            cout << "[Info]New server " << t << " has been created";
            cout << ", No. " << playerMap.size() << endl;
//        }
    }

}

void *child_thread(void *fd_) {
    int fd = *(int *) fd_;
    int myRoom = -1;
    pthread_t cur_tid = pthread_self();
    string thread_id = to_string(cur_tid);
    write(fd, thread_id.data(), 100);
    // Operation
    bool flag = false;
    while (!flag) {
        char buf[1024] = {0};
        n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            cout << "[Wrong]Server Done Error" << endl;
            break;
        }
        string op = buf;
        cout << "[Op]" + op << endl;
        vector<string> op_;
        op_ = splitWithStl(op, ";");
        if (op_[0] == "0") {
            pthread_mutex_lock(&playerMap_lock);
            playerMap[cur_tid].first = op_[1];
            playerMap_s = playerMapToString();
            pthread_mutex_unlock(&playerMap_lock);
        } else if (op_[0] == "1") {
            playerMap_s = playerMapToString();
            cout << playerMap_s << endl;
            write(fd, playerMap_s.data(), playerMap_s.size());
        } else if (op_[0] == "2") {
            bool linkFlag = true;
            pthread_mutex_lock(&playerMap_lock);
            // 用户是否存在 若存在 是否已在房间内
            for (int i = 1; i < op_.size() - 1; i++) {
                pthread_t index = stringTopthread_t(op_[i]);
                if (!playerMap.count(index))
                    linkFlag = false;
                else if (playerMap[index].second != -1)
                    linkFlag = false;
            }
            pthread_mutex_unlock(&playerMap_lock);

            if (linkFlag) { // 所有的待连接用户都存在且都不在房间内
                pthread_mutex_lock(&playerMap_lock);
                pthread_mutex_lock(&rooms_lock);
                int roomNum = static_cast<int>(rooms.size());
                playerMap[cur_tid].second = roomNum;
                map<pthread_t, string> map_;
                map_[cur_tid] = "";
                rooms[roomNum] = map_;
                myRoom = roomNum;
                pthread_mutex_unlock(&rooms_lock);
                pthread_mutex_lock(&invitations_lock);
                int invNum = 0;// 待处理邀请数
                map<pthread_t, int> wait_res;// 邀请状态列表
                for (int i = 1; i < op_.size() - 1; i++) { // 记录邀请
                    pthread_t index = stringTopthread_t(op_[i]);
                    invitations[index].insert(make_pair(cur_tid, 0));
                    wait_res[index] = 0;
                    invNum++;
                }
                pthread_mutex_unlock(&invitations_lock);
                playerMap_s = playerMapToString();
                pthread_mutex_unlock(&playerMap_lock);

                if (!invNum) {
                    write(fd, "No", 5);
                    break;
                }

                write(fd, "LinkS", 10);
                cout << "[Success]Link Waiting" << endl;
                bool wait_flag = false;
                while (1) {
                    pthread_mutex_lock(&invitations_lock);
                    pthread_cond_wait(&cond_flag, &invitations_lock);
                    for (auto &itor : wait_res) {
                        if (itor.second == 0 && invitations[itor.first][cur_tid] != 0) {
                            invNum--;
                            itor.second = invitations[itor.first][cur_tid];
                            if (itor.second == 1) {
                                wait_flag = true;
                                pthread_mutex_lock(&playerMap_lock);
                                pthread_mutex_lock(&rooms_lock);
                                // 加入房间
                                int roomNum = playerMap[cur_tid].second;
                                playerMap[itor.first].second = roomNum;
                                rooms[roomNum][itor.first] = "";
                                pthread_mutex_unlock(&rooms_lock);
                                pthread_mutex_unlock(&playerMap_lock);
                            }
                            invitations[itor.first].erase(cur_tid);
                        }
                    }
                    pthread_mutex_unlock(&invitations_lock);
                    if (!invNum) // 都处理完了
                        break;
                }
                if (wait_flag) {
                    pthread_cond_broadcast(&game_flag);
                    // TODO:我是主服务器
                    //初始化蛇头的行
                    int i = rand() % (MAX_Y - 1) + 1;  // 1 to MAX_Y - 1
                    for (auto &itor : rooms[myRoom]) {
                        itor.second = to_string(i);
                        i = (i + 3) % (MAX_Y - 1) + 1; // 1 to MAX_Y - 1
                    }
                    //初始化食物的位置
                    foods[myRoom].x = (rand() % ((MAX_X - 2) / 2)) * 2 + 2; // 2 to MAX_X-2  偶数  列
                    bool food_init;
                    do {
                        food_init = false;
                        foods[myRoom].y = rand() % (MAX_Y - 1) + 1;
                        for (auto &itor : rooms[myRoom]) {
                            if (itor.second == to_string(foods[myRoom].y))
                                food_init = true;

                        }
                    } while (food_init);
                    // 初始化初始字符串
                    string init_map = foods[myRoom].point_string() + ";";
                    for (auto &itor : rooms[myRoom]) {
                        init_map += to_string(itor.first) + "|" + itor.second + ",";
                    }
                    init_map += ";";
                    for (auto &itor : rooms[myRoom]) {
                        itor.second = init_map;
                    }

                    cout << "[主服务器]向次服务器发送init完成的信息" << endl;

                    pthread_mutex_lock(&init_flags_lock);
                    init_flags[myRoom]++;
                    pthread_mutex_unlock(&init_flags_lock);

                    while (1) {
                        if (init_flags[myRoom] == rooms[myRoom].size()) {
                            pthread_cond_broadcast(&init_flag);
                            break;
                        }
                    }

                    cout << "[主服务器]Game On" << endl;
                    write(fd, "GameOn", 10);
                    char ok[1024] = {0};
                    n = read(fd, ok, sizeof(ok));
                    if (n <= 0) {
                        cout << "[Wrong]MainServer GameOn Error" << endl;
                        break;
                    }
                    string ok_ = ok;
                    if (ok_ == "OK")
                        cout << "[主服务器]GameOn Ready to receive init" << endl;
                    write(fd, init_map.data(), init_map.size());
                    cout << "[主服务器]" << endl;
                    while (1) {
                        char newAction[1024] = {0};
                        n = read(fd, newAction, sizeof(newAction));
                        if (n <= 0) {
                            cout << "[Wrong]MainServer NewAction Error" << endl;
                            break;
                        }
                        string action = newAction;
                        cout << "[主服务器]" << action << endl;
                        vector<string> action_ = splitWithStl(action, ";");
                        if (action_[0] == "GameOver") { // 我先挂了　游戏还不一定结束
                            pthread_mutex_lock(&rooms_lock);
                            rooms[myRoom].erase(cur_tid);
                            pthread_mutex_unlock(&rooms_lock);
                            pthread_mutex_lock(&playerMap_lock);
                            playerMap[cur_tid].second = -1;
                            cout << "[主服务器]" << cur_tid << "回到" << playerMap[cur_tid].second << endl;
                            pthread_mutex_unlock(&playerMap_lock);
                            myRoom = -1;
                            cout << "一位勇士离开了战场" << endl;
                            break;
                        } else if (action_[0] == "Winner") {
                            pthread_mutex_lock(&rooms_lock);
                            rooms.erase(myRoom);
                            pthread_mutex_unlock(&rooms_lock);
                            pthread_mutex_lock(&playerMap_lock);
                            playerMap[cur_tid].second = -1;
                            cout << "[主服务器]" << cur_tid << "回到" << playerMap[cur_tid].second << endl;
                            cout << cur_tid << "is the winner of room " << myRoom << endl;
                            pthread_mutex_unlock(&playerMap_lock);
                            foods.erase(myRoom);
                            init_flags.erase(myRoom);
                            update_flags.erase(myRoom);
                            myRoom = -1;
                            cout << "一片战场结束了战斗" << endl;
                            break;
                        } else {
                            rooms[myRoom][cur_tid] = action_[1];
                            pthread_mutex_lock(&update_flags_lock);
                            update_flags[myRoom]++;
                            if (update_flags[myRoom] != rooms[myRoom].size()) {
                                cout << "[Info]wait for update" << endl;
                                pthread_cond_wait(&update_flag, &update_flags_lock);
                                pthread_mutex_unlock(&update_flags_lock);
                            } else {
                                pthread_mutex_unlock(&update_flags_lock);
                                if (action_[0] != "FoodStill")
                                    foods[myRoom] = point(atoi(action_[0].substr(0, action_[0].find(',')).data()),
                                                          atoi(action_[0].substr(action_[0].find(',') + 1).data()));
                                cout << "[Info]update" << endl;
                                update_flags[myRoom] = 0;
                                pthread_cond_broadcast(&update_flag);
                            }
                            string food_dir;
                            food_dir += foods[myRoom].point_string() + ";";
                            for (auto &itor : rooms[myRoom]) {
                                food_dir += to_string(itor.first) + "|" + itor.second + ",";
                            }
                            food_dir += ";";
                            write(fd, food_dir.data(), food_dir.size());
                            cout << food_dir << endl;
                        }
                    }
                    //　TODO:游戏结束
                } else {// 没有小伙伴一起嗨皮
                    write(fd, "No", 5);
                    pthread_mutex_lock(&playerMap_lock);
                    playerMap[cur_tid].second = -1;
                    pthread_mutex_lock(&invitations_lock);
                    pthread_mutex_lock(&rooms_lock);
                    rooms.erase(myRoom);
                    pthread_mutex_unlock(&rooms_lock);

                }
            } else {
                write(fd, "LinkF", 10);
                cout << "[Error]Link failed" << endl;
            }
        } else if (op_[0] == "3") {
            string str = "";
            pthread_mutex_lock(&invitations_lock);
            pthread_mutex_lock(&playerMap_lock);
            for (auto &itor : invitations[cur_tid]) {
                str += to_string(itor.first) + "," + playerMap[itor.first].first + ";";
            }
            pthread_mutex_unlock(&playerMap_lock);
            pthread_mutex_unlock(&invitations_lock);
            if (str == "") {
                str = "NoInvitations";
            }
            write(fd, str.data(), str.size());
        } else if (op_[0] == "4") {
            if (op_[1] == "No") {
                //拒绝接受 通知被阻塞的线程
                pthread_mutex_lock(&invitations_lock);
                for (auto &itor : invitations[cur_tid]) {
                    itor.second = -1;
                }
                pthread_mutex_unlock(&invitations_lock);
                pthread_cond_broadcast(&cond_flag);

                cout << "[Info]Client reject all invitations." << endl;
            } else {
                //邀请接受 通知被阻塞的线程
                pthread_mutex_lock(&invitations_lock);
                pthread_t pid = stringTopthread_t(op_[1]);
                invitations[cur_tid][pid] = 1;
                pthread_mutex_unlock(&invitations_lock);
                pthread_cond_broadcast(&cond_flag);
                cout << "[次服务器]Client accept the invitation." << endl;
                pthread_mutex_lock(&rooms_lock);
                pthread_cond_wait(&game_flag, &rooms_lock);
                pthread_mutex_unlock(&rooms_lock);
                myRoom = playerMap[cur_tid].second;
                cout << "[次服务器]Client get in the room " << myRoom << endl;
                pthread_mutex_lock(&init_flags_lock);
                init_flags[myRoom]++;
                pthread_mutex_unlock(&init_flags_lock);
                // TODO:我是次服务器
                do {
                    pthread_mutex_lock(&foods_lock);
                    pthread_cond_wait(&init_flag, &foods_lock);
                    cout << "[次服务器]收到主服务器init完成的信息" << endl;
                    if (foods.count(myRoom)) {
                        pthread_mutex_unlock(&foods_lock);
                        cout << "[次服务器]Game On" << endl;
                        break;
                    }
                    pthread_mutex_unlock(&foods_lock);
                } while (1);
                write(fd, "GameOn", 10);
                char ok[1024] = {0};
                n = read(fd, ok, sizeof(ok));
                if (n <= 0) {
                    cout << "[Wrong]Server GameOn Error" << endl;
                    break;
                }
                string ok_ = ok;
                if (ok_ == "OK")
                    cout << "[次服务器]GameOn Ready to receive init" << endl;
                write(fd, rooms[myRoom][cur_tid].data(), rooms[myRoom][cur_tid].size());
                cout << "[次服务器]" << endl;
                while (1) {
                    char newAction[1024] = {0};
                    n = read(fd, newAction, sizeof(newAction));
                    if (n <= 0) {
                        cout << "[Wrong]Server NewAction Error" << endl;
                        break;
                    }
                    string action = newAction;
                    cout << "[次服务器]" + action << endl;
                    vector<string> action_ = splitWithStl(action, ";");
                    if (action_[0] == "GameOver") { // 我先挂了　游戏还不一定结束
                        pthread_mutex_lock(&rooms_lock);
                        rooms[myRoom].erase(cur_tid);
                        pthread_mutex_unlock(&rooms_lock);
                        pthread_mutex_lock(&playerMap_lock);
                        playerMap[cur_tid].second = -1;
                        cout << "[次服务器]" << cur_tid << "回到" << playerMap[cur_tid].second << endl;
                        pthread_mutex_unlock(&playerMap_lock);
                        myRoom = -1;
                        cout << "一位勇士离开了战场" << endl;
                        break;
                    } else if (action_[0] == "Winner") {
                        pthread_mutex_lock(&rooms_lock);
                        rooms.erase(myRoom);
                        pthread_mutex_unlock(&rooms_lock);
                        pthread_mutex_lock(&playerMap_lock);
                        playerMap[cur_tid].second = -1;
                        cout << "[次服务器]" << cur_tid << "回到" << playerMap[cur_tid].second << endl;
                        cout << cur_tid << "is the winner of room " << myRoom << endl;
                        pthread_mutex_unlock(&playerMap_lock);
                        foods.erase(myRoom);
                        init_flags.erase(myRoom);
                        update_flags.erase(myRoom);
                        myRoom = -1;
                        cout << "一片战场结束了战斗" << endl;
                        break;
                    } else {
                        rooms[myRoom][cur_tid] = action_[1];
                        pthread_mutex_lock(&update_flags_lock);
                        update_flags[myRoom]++;
                        if (update_flags[myRoom] != rooms[myRoom].size()) {
                            cout << "[Info]wait for update" << endl;
                            pthread_cond_wait(&update_flag, &update_flags_lock);
                            pthread_mutex_unlock(&update_flags_lock);
                        } else {
                            pthread_mutex_unlock(&update_flags_lock);
                            if (action_[0] != "FoodStill")
                                foods[myRoom] = point(atoi(action_[0].substr(0, action_[0].find(',')).data()),
                                                      atoi(action_[0].substr(action_[0].find(',') + 1).data()));
                            cout << "[Info]update" << endl;
                            update_flags[myRoom] = 0;
                            pthread_cond_broadcast(&update_flag);
                        }
                        string food_dir;
                        food_dir += foods[myRoom].point_string() + ";";
                        for (auto &itor : rooms[myRoom]) {
                            food_dir += to_string(itor.first) + "|" + itor.second + ",";
                        }
                        food_dir += ";";
                        write(fd, food_dir.data(), food_dir.size());
                        cout << food_dir << endl;
                    }
                }//　TODO:游戏结束
            }
        } else if (op_[0] == "-1") {
            if (op_[1] == "Goodbye") {
                cout << "[Info]Server " << cur_tid << " Done " << endl;
            }
        } else {
            flag = true;
            cout << "[Wrong]Server Done Error" << endl;
        }
    }
    close(fd);
    pthread_mutex_lock(&playerMap_lock);
    pthread_mutex_lock(&invitations_lock);
    for (auto &itor : playerMap) {
        if (itor.first == cur_tid) {
            playerMap.erase(cur_tid);
            invitations.erase(cur_tid);
            playerMap_s = playerMapToString();
            pthread_mutex_unlock(&invitations_lock);
            pthread_mutex_unlock(&playerMap_lock);
            if (myRoom != -1) {
                pthread_mutex_lock(&rooms_lock);
                rooms.erase(myRoom);
                pthread_mutex_unlock(&rooms_lock);
            }
            cout << "[Info]Server " << cur_tid << " has been removed" << endl;
            break;
        }
    }
    pthread_join(cur_tid, nullptr);
    return nullptr;
}

pthread_t stringTopthread_t(const string &str) {
    for (auto &itor : playerMap) {
        if (to_string(itor.first) == str)
            return itor.first;
    }
    return 0;
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

string playerMapToString() {
    string str = "";
    for (auto &itor : playerMap) {
        str += to_string(itor.first) + "," + itor.second.first + "," + to_string(itor.second.second) + ";";
    }
    return str;
}