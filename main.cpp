#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <stdio.h>

struct  Events{
    int hours,minutes;
    int ID;
    std::string client_name;
    int table_num;
    Events() :
        hours(0), minutes(0), ID(0), client_name(""), table_num(0) {}
    Events(int _hours, int _minutes,int id,std::string _clientname,int tablenum): 
        hours(_hours) , minutes(_minutes), ID(id), client_name(_clientname), table_num(tablenum){}
    void operator=(std::string const& str1) {
        client_name = str1;
    }
    void print() {
        printf("%02d:%02d %d %s", hours, minutes, ID, client_name.c_str());
        if (table_num != 0)
            std::cout << " " << table_num;
        std::cout << "\n";
    }
};
struct  Tables {
    int hours, minutes;
    bool istaken; //1 if table ii taken, 0 if free
    int alltime; 
    int price;
    Tables() :
        hours(0), minutes(0), istaken(false), alltime(0),price(0) {}
    Tables(int _hours, int _minutes, bool _istaken, int _alltime,int _price) :
        hours(_hours), minutes(_minutes), istaken(_istaken), alltime(_alltime), price(_price) {}
    
    
};
bool isDig(std::string const& str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
bool is_good_name(std::string const& str) {
    for (char c: str)
    {
        if (!(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9') && !(c=='_') && !(c=='-'))
            return false;

    }
    return true;
}
int check_format(int& N, int& M, int& start_time_h, int& start_time_m, int& end_time_h, int& end_time_m, int& price, std::vector<std::string>& file_lines, std::vector<Events> &list_events) {
    //check format of count of tables
    if (file_lines[0].size() != 0) {
        file_lines[0].pop_back();
        if (isDig(file_lines[0])) {
            N = std::stoi(file_lines[0]);
            if (!(N > 0)) {
                std::cout << file_lines[0] << "\nFormat error. Enter positive integer number of tables" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << file_lines[0] << "\nFormat error. Enter positive integer number of tables" << std::endl;
            return 1;
        }
    }
    //check format of time start and and
    if (file_lines[1].size() != 0) {
        file_lines[1].pop_back();
        if (file_lines[1].size() == 11 && isDig(file_lines[1].substr(0, 2)) && file_lines[1].substr(2, 1) == ":" && isDig(file_lines[1].substr(3, 2)) && file_lines[1].substr(5, 1) == " " && isDig(file_lines[1].substr(6, 2)) && file_lines[1].substr(8, 1) == ":" && isDig(file_lines[1].substr(9, 2))) {

            start_time_h = std::stoi(file_lines[1].substr(0, 2));
            start_time_m = std::stoi(file_lines[1].substr(3, 2));
            end_time_h = std::stoi(file_lines[1].substr(6, 2));
            end_time_m = std::stoi(file_lines[1].substr(9, 2));
            if (!(start_time_h >= 0 && start_time_h < 24 && end_time_h >= 0 && end_time_h < 24 && start_time_m >= 0 && start_time_m < 60 && end_time_m >= 0 && end_time_m < 60)) {
                std::cout << file_lines[1] << "\nFormat error. Enter the time as XX:XX XX:XX in 24 format" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << file_lines[1] << "\nFormat error. Enter the time as XX:XX XX:XX in 24 format" << std::endl;
            return 1;
        }
    }

    //check format of price
    if (file_lines[2].size() != 0) {
        file_lines[2].pop_back();
        if (isDig(file_lines[2])) {
            price = std::stoi(file_lines[2]);
            if (!(price > 0)) {
                std::cout << file_lines[2] << "\nFormat error. Enter positive integer number of price" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << file_lines[2] << "\nFormat error. Enter positive integer number of price" << std::endl;
            return 1;
        }
    }

    M = file_lines.size() - 3;
    int time_h = 0;   //temp. number of hours for each event
    int time_m = 0;   //temp. number of minutes for each event
    int id = 0;       //temp. id
    int table_num = 0;  //temp. number of table
    for (int i = 3; i < file_lines.size(); i++) {

        if (file_lines[i].size() != 0)
            file_lines[i].pop_back();

        std::vector<std::string> temp;

        std::istringstream str_events(file_lines[i]);
        std::string word;

        while (str_events >> word) {
            temp.push_back(word);
        }
        int count_space = 0;
        for (char c : file_lines[i]) {
            if (c == ' ') {
                count_space++;
            }
        }
        //check a count of spaces
        if (!(temp.size() - count_space == 1) || temp.size() < 3 || temp.size() > 4) {
            std::cout << file_lines[i] << "\nFormat error. Enter data in the format TIME ID_EVENT EVENT_BODY" << std::endl;
            return 1;

        }

        //check format of time
        if (temp[0].size() == 5 && isDig(temp[0].substr(0, 2)) && temp[0].substr(2, 1) == ":" && isDig(temp[0].substr(3, 2))) {
            time_h = std::stoi(temp[0].substr(0, 2));
            time_m = std::stoi(temp[0].substr(3, 2));
            list_events.push_back(Events());

            if (!(time_h >= 0 && time_h < 24 && time_m >= 0 && time_m < 60)) {
                std::cout << file_lines[i] << "\nFormat error. Enter the time as XX:XX XX:XX in 24 format" << std::endl;
                return 1;
            }
            if (i > 3 && (!(time_h > list_events[i - 3 - 1].hours) && !((time_h == list_events[i - 3 - 1].hours) && (time_m >= list_events[i - 3 - 1].minutes)))) {
                std::cout << file_lines[i] << "\nFormat error. All events must take place sequentially in time" << std::endl;
                return 1;
            }
            list_events[i - 3].hours = time_h;
            list_events[i - 3].minutes = time_m;

        }
        else
        {
            std::cout << file_lines[i] << "\nFormat error. Enter the time as XX:XX XX:XX in 24 format" << std::endl;
            return 1;
        }

        //check format ID
        if (isDig(temp[1])) {
            id = std::stoi(temp[1]);
            if (id < 1 || id>4) {
                std::cout << file_lines[i] << "\nFormat error. Enter positive integer ID from 1 to 4" << std::endl;
                return 1;
            }
            list_events[i - 3].ID = id;
        }
        else
        {
            std::cout << file_lines[i] << "\nFormat error. Enter positive integer ID from 1 to 4" << std::endl;
            return 1;
        }

        //check format client name
        if (is_good_name(temp[2])) {
            list_events[i - 3] = temp[2];
        }
        else
        {
            std::cout << file_lines[i] << "\nFormat error. Client names are a combination of characters from the alphabet a..z, 0..9, _, -" << std::endl;
            return 1;
        }
        if (list_events[i - 3].ID == 2 && !(temp.size() == 4)) {
            std::cout << file_lines[i] << "\nFormat error. There is not number of table" << std::endl;
            return 1;
        }
        else if (list_events[i - 3].ID == 2)
        {
            if (isDig(temp[3])) {
                table_num = std::stoi(temp[3]);
                if (table_num < 1 || table_num>N) {
                    std::cout << file_lines[i] << "\nFormat error. Enter table number from 1 to " << N << std::endl;
                    return 1;
                }
                list_events[i - 3].table_num = table_num;
            }
            else
            {
                std::cout << file_lines[i] << "\nFormat error. Enter table number from 1 to " << N << std::endl;
                return 1;
            }
        }



    }
    return 0;
}

void data_output(std::unordered_map<std::string,int> & map_client, std::vector<Tables> &list_tables, std::vector<Events>& list_events, int& N, int& start_time_h, int& start_time_m, int& end_time_h, int& end_time_m, int &price ) {
    Events error = Events();   //Error event
    Events event11 = Events();  //Outgoing event: client left
    Events event12 = Events();  //Outgoing event: the client sat down at the table
    std::queue<std::string> client_queue;  //queue of clients
    int count_tables = 0;  //number of tables used
    int num = 0;
    int time_client = 0; //working time at one client's table
    for (int i = 0; i <= N; i++) {
        list_tables.push_back(Tables());
    }
    //Format output
    printf("%02d:%02d\n", start_time_h, start_time_m);
    for (int i = 0; i < list_events.size(); i++) {
        list_events[i].print();
        switch (list_events[i].ID)
        {
        case(1):
            if (list_events[i].hours<start_time_h || list_events[i].hours > end_time_h || ((list_events[i].hours == start_time_h) && (list_events[i].minutes < start_time_m)) || ((list_events[i].hours == end_time_h) && (list_events[i].minutes > end_time_m))) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "NotOpenYet", 0);
                error.print();
            }
            else if (map_client.find(list_events[i].client_name) != map_client.end()) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "YouShallNotPass", 0);
                error.print();
            }
            else {
                map_client[list_events[i].client_name] = 0;
            }
            break;
        case(2):
            if (map_client.find(list_events[i].client_name) == map_client.end()) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "ClientUnknown", 0);
                error.print();
            }
            else if (list_tables[list_events[i].table_num].istaken) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "PlaceIsBusy", 0);
                error.print();
            }
            else {
                if (map_client[list_events[i].client_name] != 0) {
                    time_client = (list_events[i].hours * 60 + list_events[i].minutes) - ((list_tables[map_client[list_events[i].client_name]].hours) * 60 + list_tables[map_client[list_events[i].client_name]].minutes);
                    list_tables[map_client[list_events[i].client_name]].alltime += time_client;
                    list_tables[map_client[list_events[i].client_name]].hours = 0;
                    list_tables[map_client[list_events[i].client_name]].minutes = 0;
                    list_tables[map_client[list_events[i].client_name]].istaken = false;
                    if (time_client % 60 == 0) {
                        list_tables[map_client[list_events[i].client_name]].price += (time_client / 60) * price;
                    }
                    else
                    {
                        list_tables[map_client[list_events[i].client_name]].price += ((time_client / 60) + 1) * price;
                    }
                    count_tables--;
                    while (!client_queue.empty() && map_client[client_queue.front()] != 0)
                        client_queue.pop();
                    if (client_queue.size() > 0) {
                        map_client[client_queue.front()] = map_client[list_events[i].client_name];
                        list_tables[map_client[list_events[i].client_name]].hours = list_events[i].hours;
                        list_tables[map_client[list_events[i].client_name]].minutes = list_events[i].minutes;
                        list_tables[map_client[list_events[i].client_name]].istaken = true;
                        event12 = Events(list_events[i].hours, list_events[i].minutes, 12, client_queue.front(), map_client[client_queue.front()]);
                        event12.print();
                        count_tables++;
                        client_queue.pop();

                    }
                }
                map_client[list_events[i].client_name] = list_events[i].table_num;
                list_tables[list_events[i].table_num].hours = list_events[i].hours;
                list_tables[list_events[i].table_num].minutes = list_events[i].minutes;
                list_tables[list_events[i].table_num].istaken = true;
                count_tables++;

            }

            break;
        case(3):
            if (N - count_tables > 0) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "ICanWaitNoLonger", 0);
                error.print();

                if (client_queue.size() + 1 > N) {
                    event11 = Events(list_events[i].hours, list_events[i].minutes, 11, list_events[i].client_name, 0);
                    event11.print();
                    map_client.erase(list_events[i].client_name);
                }
            }
            else if (client_queue.size() + 1 > N) {
                event11 = Events(list_events[i].hours, list_events[i].minutes, 11, list_events[i].client_name, 0);
                event11.print();
                map_client.erase(list_events[i].client_name);
            }
            else
                client_queue.push(list_events[i].client_name);
            break;

        case(4):
            if (map_client.find(list_events[i].client_name) == map_client.end()) {
                error = Events(list_events[i].hours, list_events[i].minutes, 13, "ClientUnknown", 0);
                error.print();
            }
            else if (map_client[list_events[i].client_name] != 0) {
                time_client = (list_events[i].hours * 60 + list_events[i].minutes) - ((list_tables[map_client[list_events[i].client_name]].hours) * 60 + list_tables[map_client[list_events[i].client_name]].minutes);
                list_tables[map_client[list_events[i].client_name]].alltime += time_client;
                list_tables[map_client[list_events[i].client_name]].hours = 0;
                list_tables[map_client[list_events[i].client_name]].minutes = 0;
                list_tables[map_client[list_events[i].client_name]].istaken = false;
                if (time_client % 60 == 0) {
                    list_tables[map_client[list_events[i].client_name]].price += (time_client / 60) * price;
                }
                else
                {
                    list_tables[map_client[list_events[i].client_name]].price += ((time_client / 60) + 1) * price;
                }
                count_tables--;
                while (!client_queue.empty() && map_client[client_queue.front()] != 0)
                    client_queue.pop();
                if (client_queue.size() > 0) {
                    map_client[client_queue.front()] = map_client[list_events[i].client_name];
                    list_tables[map_client[list_events[i].client_name]].hours = list_events[i].hours;
                    list_tables[map_client[list_events[i].client_name]].minutes = list_events[i].minutes;
                    list_tables[map_client[list_events[i].client_name]].istaken = true;
                    event12 = Events(list_events[i].hours, list_events[i].minutes, 12, client_queue.front(), map_client[client_queue.front()]);
                    event12.print();
                    count_tables++;
                    client_queue.pop();

                }
                map_client.erase(list_events[i].client_name);
            }
            else
            {
                map_client.erase(list_events[i].client_name);
            }
            break;
        default:
            break;
        }
    }

    std::map<std::string, int> ordered_list_client(map_client.begin(), map_client.end());
    for (const auto& it : ordered_list_client) {
        printf("%02d:%02d %d %s\n", end_time_h, end_time_m, 11, it.first.c_str());
        if (map_client[it.first.c_str()] != 0) {
            time_client = (end_time_h * 60 + end_time_m) - ((list_tables[map_client[it.first.c_str()]].hours) * 60 + list_tables[map_client[it.first.c_str()]].minutes);
            list_tables[map_client[it.first.c_str()]].alltime += time_client;
            if (time_client % 60 == 0) {
                list_tables[map_client[it.first.c_str()]].price += (time_client / 60) * price;
            }
            else
            {
                list_tables[map_client[it.first.c_str()]].price += ((time_client / 60) + 1) * price;
            }
        }
    }

    printf("%02d:%02d\n", end_time_h, end_time_m);
    int hours = 0, minutes = 0;
    for (int i = 1; i <= N; i++) {
        printf("%d %d %02d:%02d\n", i, list_tables[i].price, list_tables[i].alltime / 60, list_tables[i].alltime % 60);
    }
}
int main(int argc, char* argv[])
{
    int N = 0; //number of tables
    int M = 0; //number of events
    int start_time_h = 0;
    int start_time_m = 0;
    int end_time_h = 0;
    int end_time_m = 0;
    int price = 0;
    std::vector<std::string> file_lines;
    std::string file_line;

    //Reading file
    if (argc != 2) {
        std::cout << "\nYou need enter file name after .exe name";
        return 1;
    }
    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
    
    while (std::getline(f, file_line)) {
        file_lines.push_back(file_line);
    }
    f.close();

    if (file_lines[file_lines.size() - 1].size() == 1)
        file_lines.pop_back();

    std::vector<Events> list_events;  //list of input events
    std::unordered_map<std::string, int> map_client; //existing clients
    std::vector<Tables> list_tables;   //table data

    //Format check input
    if (check_format(N, M, start_time_h, start_time_m, end_time_h, end_time_m, price, file_lines, list_events) == 1) {
        return 1;
    }

    //Format output
    data_output(map_client, list_tables, list_events, N, start_time_h, start_time_m, end_time_h, end_time_m, price);
    
    getchar();
    return 0;
}