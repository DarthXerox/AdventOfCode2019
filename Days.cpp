#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>


//#include "Day1.hpp"

using namespace std;


struct Point{
    Point(int x, int y): x(x), y(y) {}
    friend std::ostream& operator<<(std::ostream& strm, Point& p);
    int x;
    int y;

    bool operator < (Point& p) {
        if (x == p.x) {
            if (y < p.y)
                return true;
        } else if (x < p.x) {
            return true;
        }
        return false;
    }

    int distance() {
        return abs(x) + abs(y);
    }

    int steps_to_reach(vector<Point>& coors) {
        return std::distance(coors.begin(), find(coors.begin(), coors.end(), *this)) + 1;
    }

    friend ostream& operator<< (std::ostream &strm, Point& p) {
        return strm << p.x << ", " << p.y;
    }

    bool operator== (const Point& p) {
        return x == p.x && y == p.y;
    }
};

struct TreeNode {
    string name;
    int depth;
    TreeNode* parent = nullptr;
    vector<TreeNode> children;

    TreeNode(vector<string>& parents, vector<string>& all_children, TreeNode* par, string n, int d) {
        name = n;
        depth = d;
        parent = par;
        for (int i = 0; i < parents.size(); ++i) {
            if (parents[i] == name) {
                TreeNode node(parents, all_children,this, all_children[i], depth + 1);
                children.push_back(node);
            }
        }
    }

    int count_total_depth() {
        int sum = depth;
        for (TreeNode& child : children) {
            sum += child.count_total_depth();
        }
        
        return sum;
    }


    static vector<TreeNode*> get_path_to_root(TreeNode* node, const string& str) {
        vector<TreeNode*> path;
        node->get_node_path(path, str);
        reverse(path.begin(), path.end());
        return path;
    }

    void get_path_rec(vector<TreeNode*>& path) {
        path.push_back(this);
        if (this->parent != nullptr) {
            this->parent->get_path_rec(path);
        }
    }

    bool get_node_path(vector<TreeNode*>& path, const string& str) {
        if (str == name) {
            path.push_back(this);
            return true;
        }
        for (TreeNode& child : children) {
            if (child.get_node_path(path, str)) {
                path.push_back(this);
                return true;
            }
        }
        return false;
    }

    TreeNode* get_node(const string& str) {
        if (str == name) {
            return this;
        }
        for (TreeNode& child : children) {
            TreeNode* ptr = child.get_node(str);
            if (ptr) {
                return ptr;
            }
        }

        return nullptr;
    }

    int distance(TreeNode* start, TreeNode* goal) {
        TreeNode* anc = common_ancestor(this);
        if (!anc) {
            cout << "FAIL, invalid common ancestor!!" << endl;
        }
        int common_anc_depth = anc->depth;
        int start_depth = start->depth;
        int goal_depth = goal->depth;
        return start_depth + goal_depth - 2 * common_anc_depth;
    }

    static TreeNode* common_ancestor(TreeNode* root) {
        vector<TreeNode*> path1 = get_path_to_root(root, "YOU");
        vector<TreeNode*> path2 = get_path_to_root(root, "SAN");

        if (path1.empty() || path2.empty()) {
            return nullptr;
        }
        TreeNode* common_ancestor = path1[0];
        for (int i = 1; i < path1.size(); ++i) {
            if (i >= path2.size() || path1[i]->name != path2[i]->name) {
                break;
            }
            common_ancestor = path1[i];
        }

        return common_ancestor;
    }
};



stringstream get_stringstream(std::ifstream& infile, char delimiter){
    std::string new_str;
    std::getline(infile, new_str);

    std::replace(new_str.begin(), new_str.end(), delimiter, ' ');
    std::stringstream ss(new_str);

    return ss;
}

stringstream get_stringstream(char* filename, char delimiter) {
    std::ifstream infile(filename);
    if (!infile.is_open() ) {
        std::cout<<"Can't open the file"<<std::endl;
    }

    return get_stringstream(infile, delimiter);
}

int mode(vector<int>& vec, int mode, int pos) {
    //cout << "Vec size: " << vec.size() << " position: " << pos << endl;
    switch (mode) {
        case 0:
            return vec[vec[pos]];
        case 1:
            return vec[pos];
        default:
            cout <<"Wrong mode: " << mode << " pos: " << pos <<  endl;
            return -1;
    }
}


int intcode_computer(vector<int>& nums, stringstream& ss, int& instruction_pos) {
    string str;
    //int instruction_pos = 0;
    int num = nums[instruction_pos];
    while (num != 99) {
        str = to_string(num);
        reverse(str.begin(), str.end());

        int instruction = stoi(str.substr(0, 1));

        vector<int> param_modes;
        if (str.size() > 2) {
            str = str.substr(2);
            for (int i = 0; i < 4; i++) {
                if (str.size() > i) {
                    param_modes.push_back(stoi(str.substr(i, i + 1)));
                } else {
                    param_modes.push_back(0);
                }
            }
        } else {
            param_modes = {0, 0, 0, 0};
        }

        int save_ins_pos = instruction_pos;
        int increment = 4;
        switch (instruction){
            case 1:
                nums[nums[instruction_pos + 3]] = mode(nums, param_modes[0], instruction_pos + 1) +
                                                  mode(nums, param_modes[1], instruction_pos + 2);
                break;
            case 2:
                nums[nums[instruction_pos + 3]] = mode(nums, param_modes[0], instruction_pos + 1) *
                                                  mode(nums, param_modes[1], instruction_pos + 2);
                break;
            case 3:
                ss >> nums[nums[instruction_pos + 1]];
                increment = 2;
                break;
            case 4:
                ss << mode(nums, param_modes[0], instruction_pos +1) << " ";
                instruction_pos += 2;
                return 1;
                //cout << mode(nums, param_modes[0], instruction_pos +1) << endl;
            case 5:
                if (mode(nums, param_modes[0], instruction_pos + 1) != 0) {
                    instruction_pos = mode(nums, param_modes[1], instruction_pos + 2);
                }
                increment = 3;
                break;
            case 6:
                if (mode(nums, param_modes[0], instruction_pos + 1) == 0) {
                    instruction_pos = mode(nums, param_modes[1], instruction_pos + 2);
                }
                increment = 3;
                break;
            case 7:
                if (mode(nums, param_modes[0], instruction_pos + 1) <
                    mode(nums, param_modes[1], instruction_pos + 2)) {
                    nums[nums[instruction_pos + 3]] = 1;
                } else {
                    nums[nums[instruction_pos + 3]] = 0;
                }
                break;
            case 8:
                if (mode(nums, param_modes[0], instruction_pos + 1) ==
                    mode(nums, param_modes[1], instruction_pos + 2)) {
                    nums[nums[instruction_pos + 3]] = 1;
                } else {
                    nums[nums[instruction_pos + 3]] = 0;
                }
                break;
            default:
                cout << "Wrong instruction num, exactly :"  << instruction << endl;
                return 0;
        }

        if (instruction_pos != save_ins_pos) {
            increment = 0;
        }

        instruction_pos += increment;
        num = nums[instruction_pos];
    }
    return 0;
}

struct Amp {
    vector<int> memory;
    int instruction_pointer = 0;

    Amp(vector<int> memory, stringstream& ss): memory(memory) {
        compute(ss);
    }

    Amp(vector<int> memory, int phase, stringstream& ss): memory(memory), instruction_pointer(0) {
        phase_compute(ss, phase);
    }

    int compute(stringstream& ss) {
        return intcode_computer(memory, ss, instruction_pointer);
    }

private:
    int phase_compute(stringstream& ss, int phase) {
        int top_num;
        ss >> top_num;
        ss << phase <<" "<< top_num<<" ";
        return compute(ss);
    }
};


int day1(char* filename) {
    std::ifstream infile(filename);
    int num, sum = 0;
    while (infile >> num) {
        sum += (num / 3) - 2;
    }
    std::cout << "The sum is: \n" << sum <<std::endl;

    return 0;
}

int day1_02(char* filename) {
    std::ifstream infile(filename);
    int num, sum = 0;
    while (infile >> num) {
        int new_fuel = (num / 3) - 2;
        while (new_fuel > 0) {
            sum += new_fuel;
            new_fuel = (new_fuel / 3) - 2;
        }
    }
    std::cout << "The sum is: \n" << sum <<std::endl;

    return 0;
}

int day2(char* filename) {
    std::stringstream ss = get_stringstream(filename, ',');

    int num;
    std::vector<int> numbers;
    while(ss >> num) {
        numbers.push_back(num);
    }

    int instruction_pointer = 0;
    while(numbers[instruction_pointer] != 99 || instruction_pointer >= numbers.size()) {
        if (numbers[instruction_pointer] == 1) {
            numbers[numbers[instruction_pointer + 3]] = numbers[numbers[instruction_pointer + 1]] + numbers[numbers[instruction_pointer + 2]] ;
        } else if (numbers[instruction_pointer] == 2) {
            numbers[numbers[instruction_pointer + 3]] = numbers[numbers[instruction_pointer + 1]] * numbers[numbers[instruction_pointer + 2]] ;
        }
        instruction_pointer += 4;
    }
    std::cout << "Ans is: \n" <<  numbers[0] << std::endl;
    return 0;
}

int day2_02(char* filename) {
    std::stringstream ss = get_stringstream(filename, ',');

    int num;
    std::vector<int> nums;
    while(ss >> num) {
        nums.push_back(num);
    }

    //THIS FUCKING KILLED ME, cuz i didnt read that shit properly... ah...
    std::vector<int> numbers = nums;


    for (int i = 0; i <= 99; i++) {
        for (int j = 0; j <= 99; j++) {
            numbers[1] = i;
            numbers[2] = j;

            int instruction_pointer = 0;
            while(numbers[instruction_pointer] != 99 || instruction_pointer >= numbers.size() - 3) {
                if (numbers[instruction_pointer] == 1) {
                    numbers[numbers[instruction_pointer + 3]] = numbers[numbers[instruction_pointer + 1]] + numbers[numbers[instruction_pointer + 2]] ;
                } else if (numbers[instruction_pointer] == 2) {
                    numbers[numbers[instruction_pointer + 3]] = numbers[numbers[instruction_pointer + 1]] * numbers[numbers[instruction_pointer + 2]] ;
                }
                instruction_pointer += 4;
            }
            if (numbers[0] == 19690720) {
                std::cout << "Next ans is: \n" <<  100 * numbers[1] + numbers[2] << std::endl;
                return 0; 
            }
            numbers = nums;
        }

    }
    

    std::cout << "Not found :/" << std::endl;
    return 1; 
}

vector<Point> get_coors(stringstream& ss) {
    vector<Point> coors;

    Point last = Point(0,0);
    string str;
    while (ss >> str){
        //cout << "first line " + str << end

        char c = str[0];
        switch (c) {
            case('U'):
                for (int i = last.y + 1; i <= last.y + stoi(str.substr(1)); i++) {
                    coors.emplace_back(last.x, i);
                }
                break;
            case('D'):
                for (int i = last.y - 1; i >= last.y - stoi(str.substr(1)); i--) {
                    coors.emplace_back(last.x, i);
                }
                break;
            case('R'):
                for (int i = last.x + 1; i <= last.x + stoi(str.substr(1)); i++) {
                    coors.emplace_back(i, last.y);
                }
                break;
            case('L'):
                for (int i = last.x - 1; i >= last.x - stoi(str.substr(1)); i--) {
                    coors.emplace_back(i, last.y);
                }
                break;
            default:
                cout << "WTF!!!!!!!!!!!!!!!!!!!" << endl;
        }
        last = coors.back();
    }

    return coors;
}


int day3(char* filename) {
    std::ifstream infile(filename);
    if (!infile.is_open() ) {
        std::cout<<"FFFFFFFFFFFFF"<<std::endl;
    }
    std::stringstream ss = get_stringstream(infile, ',');
    vector<Point> coors1 = get_coors(ss);

    ss = get_stringstream(infile, ',');
    vector<Point> coors2 = get_coors(ss);
    vector<Point> intersect;

    vector<Point> new2, new1;

    copy(coors1.begin(), coors1.end(), back_inserter(new1));
    copy(coors2.begin(), coors2.end(), back_inserter(new2));

    sort(coors1.begin(), coors1.end());
    sort(coors2.begin(), coors2.end());
    set_intersection(coors1.begin(), coors1.end(), coors2.begin(), coors2.end(),
                      back_inserter(intersect));

    int min = intersect[0].distance(),
    min_steps = intersect[0].steps_to_reach(new1) + intersect[0].steps_to_reach(new2), ms;
    for (Point p : intersect) {
        if (p.distance() < min) {
            min = p.distance();
        }

        ms = p.steps_to_reach(new1) + p.steps_to_reach(new2);
        if (ms < min_steps) {
            min_steps = ms;
        }
    }

    cout << "Shortest distance is: \n" << min << endl << "Least amount of steps to get to\n" << min_steps << endl;
    return 0;
}


int day4() {
    int counter = 0, part2_counter = 0;
    for (int i = 382345; i <= 843167; i++) {
        vector<int> digits;
        int j = i;
        while (j > 0) {
            digits.push_back(j % 10);
            j /= 10;
        }
        reverse(digits.begin(), digits.end());
        vector<int> new_digits(digits.begin(), digits.end());
        sort(new_digits.begin(), new_digits.end());

        if (new_digits == digits) {
            set<int> digit_set (digits.begin(), digits.end());

            if (digit_set.size() < digits.size()) {
                counter++;

                for (int num : digits) {
                    int occurence = 0;
                    for (int num2 : digits) {
                        if (num == num2) {
                            occurence++;
                        }
                    }
                    if (occurence == 2) {
                        part2_counter++;
                        break;
                    }
                }
            }
        }
    }

    cout << "Number of passwords is: " << counter << endl;
    cout << "Number of second part passwords is: " << part2_counter << endl;

    return 0;
}


int day5(char* filename, int id) {
    stringstream ss = get_stringstream(filename, ',');

    int num;
    std::vector<int> nums;
    while(ss >> num) {
        nums.push_back(num);
    }

    stringstream outputs;
    outputs << id << " ";
    Amp amplifier(nums,outputs);
    while(amplifier.compute(outputs));
    cout << outputs.str() << endl;

    return 0;
}

int day6(char* filename) {
    std::ifstream infile(filename);
    if (!infile.is_open() ) {
        std::cout<<"Couldn't open the file"<<std::endl;
        return 1;
    }
    vector<string> parents, children;
    string line;

    getline(infile, line);
    while (!line.empty()) {
        parents.push_back(line.substr(0, 3));
        children.push_back(line.substr(4, 7));

        getline(infile, line);
    }


    TreeNode tree(parents, children, nullptr, "COM", 0);
    cout << "Total orbits: " << tree.count_total_depth() << endl;

    TreeNode* you = tree.get_node("YOU");
    TreeNode* santa = tree.get_node("SAN");

    cout << "Distance between objects orbited by YOU and SAN: ";
    cout << tree.distance(you, santa) - 2 << endl; // only between objects that you orbit and santa orbits

    return 0;
}

int day7(char* filename) {
    stringstream ss = get_stringstream(filename, ',');

    int num;
    std::vector<int> nums;
    while(ss >> num) {
        nums.push_back(num);
    }

    stringstream for_inputs;
    vector<int> phases = {0, 1, 2, 3, 4};
    vector<int> final_outputs;
    for (int i : phases ) {
        vector<int> v2(phases);
        v2.erase(find(v2.begin(), v2.end(), i));

        for (int j : v2) {
            vector<int> v3(v2);
            v3.erase(find(v3.begin(), v3.end(), j));

            for (int k : v3) {
                vector<int> v4(v3);
                v4.erase(find(v4.begin(), v4.end(), k));

                for (int l : v4) {
                    vector<int> v5(v4);
                    v5.erase(find(v5.begin(), v5.end(), l));

                    for (int m : v5) {
                        for_inputs << 0 << " ";
                        Amp amp1(nums, i, for_inputs);
                        Amp amp2(nums, j, for_inputs);
                        Amp amp3(nums, k, for_inputs);
                        Amp amp4(nums, l, for_inputs);
                        Amp amp5(nums, m, for_inputs);
                        int res;
                        for_inputs >> res;
                        final_outputs.push_back(res);
                    }
                }
            }
        }
    }
    int max = final_outputs[0];
    for (int val : final_outputs) {
        if (val > max) {
            max = val;
        }
    }

    cout <<  "First part max is: " << max << endl;

    phases = {5, 6, 7, 8, 9};
    final_outputs.clear();
    for (int i : phases ) {
        vector<int> v2(phases);
        v2.erase(find(v2.begin(), v2.end(), i));

        for (int j : v2) {
            vector<int> v3(v2);
            v3.erase(find(v3.begin(), v3.end(), j));

            for (int k : v3) {
                vector<int> v4(v3);
                v4.erase(find(v4.begin(), v4.end(), k));

                for (int l : v4) {
                    vector<int> v5(v4);
                    v5.erase(find(v5.begin(), v5.end(), l));

                    for (int m : v5) {
                        for_inputs << 0 << " ";
                        Amp amp1(nums, i, for_inputs);
                        Amp amp2(nums, j, for_inputs);
                        Amp amp3(nums, k, for_inputs);
                        Amp amp4(nums, l, for_inputs);
                        Amp amp5(nums, m, for_inputs);
                        do {
                            amp1.compute(for_inputs);
                            amp2.compute(for_inputs);
                            amp3.compute(for_inputs);
                            amp4.compute(for_inputs);
                        } while (amp5.compute(for_inputs));
                        int res;
                        for_inputs >> res;
                        final_outputs.push_back(res);
                    }
                }
            }
        }
    }

    max = final_outputs[0];
    for (int val : final_outputs) {
        if (val > max) {
            max = val;
        }
    }

    cout <<  "Second part max is: " << max << endl;
    return 0;
}

int day8(char* filename) {
    std::ifstream infile(filename);
    if (!infile.is_open() ) {
        std::cout<<"Can't open the file"<<std::endl;
    }
    string input;
    getline(infile, input);

    const int width = 25, height = 6;
    int pos = 0, min_zero = width * height;
    string min_str;
    vector<string> layers;

    while (pos <  input.size()) {
        string layer = input.substr(pos, height * width);
        pos += height * width;

        int zero_count = count(layer.begin(), layer.end(), '0');
        if (zero_count < min_zero) {
            min_zero = zero_count;
            min_str = layer;
        }
        layers.push_back(layer);
    }

    cout << "Result is: " << count(min_str.begin(), min_str.end(), '1') *
                          count(min_str.begin(), min_str.end(), '2') << endl;

    // render msg
    string rendered_message;
    for (int i = 0; i < layers[0].size(); ++i) {
        int layer_index = 0;
        while (layers[layer_index][i] == '2') {
            ++layer_index;
        }
        rendered_message += layers[layer_index][i];
    }

    // print msg
    for (int i = 0; i < rendered_message.size(); ++i) {
        if (rendered_message[i] == '1') {
            cout << "X";
        } else {
            cout << " ";
        }
        if ((i + 1) % 25 == 0) {
            cout << endl;
        }
    }

    return 0;
}
