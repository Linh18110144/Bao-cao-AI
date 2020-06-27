
#include <iostream>

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include <time.h>

enum class Actions { left, right, up, down };
std::string action_names[] = { "left","right","up","down" };
#define SIDE 3 // Length of the board 


typedef struct Node_array
{
    int state[SIDE][SIDE];
    Actions action;
    struct Node_array* parent; // tree search

    int path_cost;
    int depth;
    struct Node_array* nextNode; // list


} Node;



typedef struct NodeList {
    unsigned int nodeCount;    //the number of nodes in the list
    Node* head;            //pointer to the first node in the list
    Node* tail;            //pointer to the last node in the list
};



void set_zero(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = 0;
}

void copy_array(int a[][SIDE], int b[][SIDE])//copy b to a
{
    int i, j;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            a[i][j] = b[i][j];
}

int is_equal(int a[][SIDE], int b[][SIDE]) // =1 thì giống nhau  ,  !=1 thì khác nhau
{
    int i, j, flag = 1;
    for (i = 0; i < SIDE; i++)
        for (j = 0; j < SIDE; j++)
            if (a[i][j] != b[i][j])
                flag = 0;
    return flag;
}

void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void print_array(int a[][SIDE])
{
    int i, j;
    for (i = 0; i < SIDE; i++)
    {
        for (j = 0; j < SIDE; j++)
            printf("%d  | ", a[i][j]);
        printf("\n");
        for (j = 0; j < SIDE; j++)
            printf("---|-");

        printf("\n");
    }
}

bool isLegalAction(Node* node, Actions action)  // Kiểm tra xem có di chuyển được k
{
    int index_i, index_j;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {         // tìm node 0
                index_i = i;
                index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    if (action == Actions::left && index_j == 0)  return false;    // kiểm tra các trường hợp không di chuyển được
    if (action == Actions::right && index_j == 2)  return false;

    switch (index_i)
    {
    case 0:
        if (action == Actions::up) return false;
        return true;
    case 2:
        if (action == Actions::down) return false;
        return true;
    default:
        return true;
    }
    return true;
}

Node* Child_node(Node* node, Actions action)     // Hàm trả về mảng sau khi di chuyển
{
    Node* child = (Node*)malloc(sizeof(Node));
    int index_i, index_j;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (node->state[i][j] == 0) {
                index_i = i;
                index_j = j;
                i = SIDE;
                break;
            }
        }
    }
    copy_array(child->state, node->state);          // copy mảng của node vào mảng của child
    child->action = action;
    child->parent = node;
    child->path_cost = node->path_cost + 1;
    if (action == Actions::left) {
        swap(child->state[index_i][index_j - 1], child->state[index_i][index_j]);
    }
    else if (action == Actions::right) {
        swap(child->state[index_i][index_j + 1], child->state[index_i][index_j]);
    }
    else if (action == Actions::up) {
        swap(child->state[index_i - 1][index_j], child->state[index_i][index_j]);
    }
    else if (action == Actions::down) {
        swap(child->state[index_i + 1][index_j], child->state[index_i][index_j]);
    }
    return child;
}

NodeList* FIFO_initial()     //    tạo danh sach node 
{
    NodeList* list;
    list = (NodeList*)malloc(sizeof(NodeList));
    list->nodeCount = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}
void FIFO_add(NodeList* list, Node* node)    //Them node node vao
{
    if (list->nodeCount <= 0) {
        list->head = node;
        list->tail = node;
        list->nodeCount += 1;
        return;
    }

    list->tail->nextNode = node;           //  ???


    list->tail = node;
    list->nodeCount += 1;
}
Node* FIFO_pop(NodeList* list)      // Lấy node đầu tiên của danh sách ra
{
    if (list->nodeCount <= 0)
    {
        return NULL;
    }
    Node* temp = list->head;
    list->nodeCount -= 1;
    if (list->nodeCount <= 0)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = temp->nextNode;   // trả về node sau head
    }
    return temp;
}

bool Goal_test(Node* node, Node* goal)   // kiểm tra 2 trang thái có trùng nhau không
{
    return is_equal(node->state, goal->state);
}

bool checkExist(NodeList* list, Node* node)
{
    Node* _node;
    int i = 0;
    _node = list->head;
    while (i < list->nodeCount) {
        if (Goal_test(node, _node))
        {
            return true;
        }
        _node = _node->nextNode;
        i++;
    }
    return false;
}

int pathcost(Node* node)
{
    int count = 1;
    int k = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (node->state[i][j] != 0 && node->state[i][j] != count)
            {
                k++;

            }
            count++;
        }
    return k;
}



NodeList* frontier = FIFO_initial();

int luuaction;

Node* aStarSearch(Node* root)
{
    Node* node = root;
    int Path_cost = 10000;

    FIFO_add(frontier, node);

    Node* child = (Node*)malloc(sizeof(Node));
    Node* luu = (Node*)malloc(sizeof(Node));

    for (int action = 0; action < 4; action++)
    {
        if (isLegalAction(node, (Actions)action))   // Kiểm tra có di chuyển được không
        { // nếu được
            child = (Child_node(node, (Actions)action));  // Sau khi di chuyển thì gán vào child

            if (checkExist(frontier, child) == false)
            {

                FIFO_add(frontier, child);

                if (Path_cost > (pathcost(child)))
                {

                    Path_cost = pathcost(child);
                    luu = child;
                    luuaction = action;
                }


            }



        }
    }
    printf("%d\n", luuaction);

    return luu;

}

Node* HUMAN(Node* node)
{
    int action;
    scanf_s("%d", &action);
    while (action != 0 && action != 1 && action != 2 && action != 3)
    {
        printf("Nhap lai huong di: ");
        scanf_s("%d", &action);
    }

    while (isLegalAction(node, (Actions)action) == 0)
    {
        printf("Nhap lai huong di chuyen: ");
        scanf_s("%d", &action);
    }
    node = (Child_node(node, (Actions)action));

    return node;
}

int tungxu()
{
    int nSecret;

    nSecret = rand() % 2;
    printf("Tung xu: %d\n", nSecret);
    return nSecret;

}

clock_t start, end;
double cpu_time_used;



int main()
{
    int a[3][3] = { 1,2,3,4,5,6,7,8,0 };
    Node* Goal, * Start;
    int i, j;
    Goal = (Node*)malloc(sizeof(Node));
    Start = (Node*)malloc(sizeof(Node));
    printf("Enter the goal state\n");
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            //std::cin >> Goal->state[i][j];
            Goal->state[i][j] = a[i][j];
    printf("Goal state:\n");
    print_array(Goal->state);

    Goal->parent = NULL;
    Goal->depth = 0;



    printf("Enter the current state\n");
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            std::cin >> Start->state[i][j];
    Start->parent = NULL;
    Start->path_cost = 0;
    printf("Start state:\n");
    print_array(Start->state);

    printf("Chu thich cach di chuyen: 0=left, 1=right, 2=up, 3=down \n");

    int choose;
    printf("Chon nguoi choi truoc hay may choi truoc\n");
    printf("1: Nguoi choi choi truoc\n0: May choi truoc\n");
    scanf_s("%d", &choose);

    int dem = 0;
    Node* test;



    if (choose == 1)
    {
        printf("Nguoi choi tung truoc\n");
        test = Start;

        if (Goal_test(test, Goal))
            printf("Dau cuoi giong nhau");
        while (Goal_test(test, Goal) == 0)
        {
            srand((int)time(0));
            while (tungxu() == 0)
            {
                dem++;
            }
            if (dem % 2 == 0)
            {
                printf("Nguoi choi di:");
                test = HUMAN(test);
                print_array(test->state);
                if (Goal_test(test, Goal))
                {
                    printf("Nguoi choi thang\n");
                    break;
                }
                dem = 0;


            }

            else
            {
                printf("May di:");
                frontier = FIFO_initial();
                test = aStarSearch(test);
                print_array(test->state);
                if (Goal_test(test, Goal))
                {
                    printf("May thang\n");
                    break;
                }
                dem = 1;

            }
            printf("\n");
        }

    }
    else
    {
        printf("May tung truoc\n");
        test = Start;

        if (Goal_test(test, Goal))
            printf("Dau cuoi giong nhau");
        while (Goal_test(test, Goal) == 0)
        {
            srand((int)time(0));
            while (tungxu() == 0)
            {
                dem++;
            }
            if (dem % 2 == 1)
            {
                printf("Nguoi choi di:");
                frontier = FIFO_initial();
                test = HUMAN(test);
                print_array(test->state);
                if (Goal_test(test, Goal))
                {
                    printf("Nguoi choi thang\n");
                    break;
                }
                dem = 1;


            }

            else
            {
                printf("May di:");
                frontier = FIFO_initial();
                test = aStarSearch(test);
                print_array(test->state);
                if (Goal_test(test, Goal))
                {
                    printf("May thang\n");
                    break;
                }
                dem = 0;

            }
            printf("\n");
        }


    }




    //call search function
   // breadthFirstSearch(Start, Goal);
   // aStarSearch(Start, Goal);

    return 0;
}






