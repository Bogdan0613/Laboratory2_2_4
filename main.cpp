#include <iostream>
#include <stack>
#include <cmath>

using namespace std;


typedef struct TreeNode{
    char info;
    TreeNode* left, * right;
} TreeNode;
typedef struct TreeNode* ExpTree;


ExpTree Create_Node(int info);
ExpTree constructTree(ExpTree T, char postfix[]);
int getPriority(char ch);
void Convert_In_To_Post(string infix, char postfix[]);
void resultExpression(char postfix[]);


void show_node(ExpTree T);
void preorder(ExpTree T);
void postorder(ExpTree T);
void inorder(ExpTree T);
void display(ExpTree tree, char postfix[]);
void line();
void print2DUtil(TreeNode *root, int space);
void print2D(TreeNode *root);

bool IsExpCorrect(string infix);
bool isOperator(char ch);
bool isDigit(char c);

int main()
{
    cout << "\n Enter Expression : ";
    string expression;
    cin >> expression;
    if (!IsExpCorrect(expression)) {
        cout << " Error! Try to enter again without special symbols " << endl;
        return 0;
    }
    char* postfix = (char*)malloc(sizeof(char) * expression.length());
    Convert_In_To_Post(expression, postfix);
    ExpTree tree1 = NULL;
    tree1 = constructTree(tree1, postfix);
    display(tree1, postfix);
    cout << "\n\t\t    ( Binary Expression Tree )" << endl;
    line();
    print2D(tree1);
    return 0;
}

bool IsExpCorrect(string infix){
    for (unsigned int i = 0; i < infix.length(); i++) {
        if (!isDigit(infix[i]) && !isOperator(infix[i]) && infix[i] != '(' && infix[i] != ')')
            return false;
    }
    return true;
}

int getPriority(char ch){
    switch (ch){
    case '^':
        return 4;
    case '%':
        return 3;
    case '/':
    case '*':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}

void Convert_In_To_Post(string infix, char IPN_expression[]){
    unsigned int counter1 = 0;
    stack<char> st;
    int postCount = 0;
    char element;
    while (counter1 < infix.length()){
        element = infix[counter1];
        if (element == '('){
            st.push(element);
            counter1++;
            continue;
        }
        if (element == ')'){
            while (!st.empty() && st.top() != '('){
                IPN_expression[postCount++] = st.top();
                st.pop();
            }
            if (!st.empty()){
                st.pop();
            }
            counter1++;
            continue;
        }

        if (getPriority(element) == 0){
            IPN_expression[postCount++] = element;
        }
        else{
            if (st.empty()){
                st.push(element);
            }
            else{
                while (!st.empty() && st.top() != '(' &&
                    getPriority(element) <= getPriority(st.top())){
                    IPN_expression[postCount++] = st.top();
                    st.pop();
                }
                st.push(element);
            }
        }
        counter1++;
    }

    while (!st.empty()){
        IPN_expression[postCount++] = st.top();
        st.pop();
    }
    IPN_expression[postCount] = '\0';
}

// Create a new Tree Node
ExpTree Create_Node(int info){
    TreeNode* temp;
    temp = (TreeNode*)malloc(sizeof(TreeNode));
    if (temp == NULL)
    {
        cout << "Out of space!\n";
        return (temp);
    }
    temp->left = NULL;
    temp->right = NULL;
    temp->info = info;
    return temp;
};

ExpTree constructTree(ExpTree tree, char postfix[]){
    int i = 0;
    stack<TreeNode*> st;
    TreeNode* temp_tree1;
    TreeNode* temp_tree2;
    while (postfix[i] != '\0'){
        if (!(postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/'
            || postfix[i] == '%' || postfix[i] == '^'))
        {
            tree = Create_Node(postfix[i]);
            st.push(tree);
        }
        else{
            tree = Create_Node(postfix[i]);
            temp_tree1 = st.top(); st.pop();
            temp_tree2 = st.top(); st.pop();
            tree->right = temp_tree1;
            tree->left = temp_tree2;
            st.push(tree);
        }
        i++;
    }
    return tree;
}

//Calculate the definition
void resultExpression(char postfix[]){
    stack<float> result;
    int k = 0;
    bool error = false;
    while (postfix[k] && !error){
        char ch = postfix[k];
        if (isDigit(ch)){
            result.push((float)postfix[k] - '0');
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^'){
            float num1 = result.top();
            result.pop();
            float num2 = result.top();
            result.pop();
            float calc = 0;
            switch (ch){
            case '+':
                calc = num2 + num1;
                break;
            case '-':
                calc = num2 - num1;
                break;
            case '*':
                calc = num2 * num1;
                break;
            case '/':
                if (num1 == 0){
                    cout << "\n Math Error: Can't Divide by Zero " << endl;
                    error = true;
                    break;
                }
                calc = num2 / num1;
                break;
            case '%':
                if (num1 == 0)
                {
                    cout << "\n Math Error: Can't Divide by Zero " << endl;
                    error = true;
                    break;
                }
                calc = ((int)num2) % ((int)num1);
                break;
            case '^':
                calc = pow(num2, num1);
                break;
            }
            result.push(calc);
        }
        k++;
    }
    if (!error)
        cout << " = " << result.top() << endl;
}

void show_node(ExpTree T){
    cout << T->info << " ";
}

void preorder(ExpTree T){
    if (T != NULL){
        show_node(T);
        preorder(T->left);
        preorder(T->right);
    }
}

void postorder(ExpTree T){
    if (T != NULL)
    {
        postorder(T->left);
        postorder(T->right);
        show_node(T);
    }
}

void inorder(ExpTree T){
    if (T != NULL)
    {
        inorder(T->left);
        show_node(T);
        inorder(T->right);
    }
}

void display(ExpTree tree, char postfix[]){
    line();
    cout << "\n Inverted Polish Notation : [ ";
    postorder(tree);
    cout << "]" << endl;
    line();
    cout << "\n Expression : ";
    inorder(tree);
    resultExpression(postfix);
}

void print2DUtil(TreeNode *root, int space){
    int counter = 10;
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += counter;
 
    // Process right child first
    print2DUtil(root->right, space);
 
    // Print current node after space
    // count
    cout<<endl;
    for (int i = counter; i < space; i++)
        cout<<" ";
    cout<<root->info<<"\n";
 
    // Process left child
    print2DUtil(root->left, space);
}
 
// Wrapper over print2DUtil()
void print2D(TreeNode *root){
    // Pass initial space count as 0
    print2DUtil(root, 0);
}

void line(){
    cout << "__________________________________________________________________" << endl;
}

bool isDigit(char ch){
    if (ch > 47 && ch < 57)
        return true;
    return false;
}

bool isOperator(char ch){
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^')
        return true;
    return false;
}
