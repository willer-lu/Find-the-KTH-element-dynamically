#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<time.h>
const int BLACK = 0;
const int RED = 1;
//存储红黑树的结构的定义
typedef struct RBTree
{
    int key; //用来存储该节点的关键字信息
    int color; //用来存储节点的颜色信息
    struct RBTree* parent, * left, * right; // 用来存储该节点的父亲节点和孩子节点
}RBTree, * RBT_P;
//定义全局变量nil、 root来存储叶子节点和根节点
RBT_P nil, root;
int count = 0;
void InitRBT()
{
    root = nil = (RBT_P)malloc(sizeof(RBTree));
    (*root).key = -1;
    (*root).color = BLACK;
    (*root).left = (*root).parent = (*root).right = NULL;
}//实现红黑树的初始化
void Left_Rotate(RBT_P z)
{
    RBT_P x;
    x = z->right;
    x->parent = z->parent;
    if (z->parent->left == z) {
        z->parent->left = x;
    }
    else {
        x->parent->right = x;
    }
    if (nil == x->parent) {
        root = x;
    }
    if (nil != x->left) {
        x->left->parent = z;
    }
    z->right = x->left;
    z->parent = x;
    x->left = z;//step 3:使支点右子树的做儿子与支点z建立连接
}//以红黑树中的某一个节点z为支点进行左旋操作
void Right_Rotate(RBT_P z)
{
    RBT_P x;
    x = z->left;
    x->parent = z->parent;
    if (z->parent->left == z) {
        x->parent->left = x;
    }
    else {
        x->parent->right = x;
    }
    if (nil == x->parent) {
         root = x;
    }
    if (nil != x->right) {
        x->right->parent = z;
    }
    z->left = x->right;
    z->parent = x;
    x->right = z;
}//以红黑树中的某一节点z为支点进行右旋操作
RBT_P RBT_Search(int key)
{
    RBT_P x, y; //x用来存储当前节点
    y = x = root;
    int d; //存储差值
    while (x != nil)
    {
        y = x; //y存储x的父亲节点
        d = x->key - key;
        if (d > 0)
        {
            x = x->left;
        }
        else if (d < 0)
        {
            x = x->right;
        }
        else
        {
            return x; //返回与key相同的地址
        }
    } //while
    return y;
}
void Search()//在红黑树中查找节点key， 并输出
{
    printf("请输入您要查找的节点： ");
    int key;
    scanf("%d", &key);
    RBT_P y;
    y = RBT_Search(key);
    if (key != y->key)
    {
        printf("树中没有该节点！ \n");
    }
    else
    {
        printf("该节点： %d\n", y->key);
        if (y->color == BLACK)
        {
            printf("该节点的颜色： 黑色\n");
        }
        else
        {
            printf("该节点的颜色： 红色\n");
        }
        // printf ("该节点的左儿子： %d", y->left ->key);
    }
}


void RBT_Insert_Fixup(RBT_P z)// 插入恢复函数， 使插入节点的红黑树依然满足他的性质
{
    while ((z->parent) && z->parent->color == RED) //当插入的节点z不为根节点且z的父亲节点为红色时
    {
        if (z->parent == z->parent->parent->left) //当z的父亲节点为z的祖父节点的左儿子时
        {
            RBT_P y = z->parent->parent->right; //y指向z的叔叔节点
            if (y && y->color == RED) //叔叔节点的颜色为红色时
            //以下为情况 3
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else //叔叔节点的颜色为黑色时
            {
                if (z == z->parent->right) //z为其父亲节点的右儿子
                //以下为情况 4
                {
                    z = z->parent; //确立新的z
                    Left_Rotate(z); //左旋
                }
                else
                    //以下为情况 5
                {
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    Right_Rotate(z->parent->parent); //右旋
                }
            }
        }
        else //z的父亲节点是z的祖父节点的右儿子
        {
            RBT_P y;
            y = z->parent->parent->left; //y指向z的叔叔节点
            if (y && y->color == RED)
                //以下为情况 3
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                    //以下为情况 4
                {
                    z = z->parent; //
                    Right_Rotate(z);
                }
                else
                    //以下为情况 5
                {
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    Left_Rotate(z->parent->parent);
                }
            }
        }
    } //while
    root->color = BLACK; //根节点为黑色
}
void RBT_Insert(int key)//插入一个节点到其应该在的位置
{
    RBT_P z = (RBT_P)malloc(sizeof(RBTree));
    z->key = key;
    z->color = RED;
    z->left = z->parent = z->right = NULL; //创建z节点， 并且初始化数据
    RBT_P y = RBT_Search(key);
    if (y->key == key)
    {
        printf("该节点已经存在！ \n");
    }
    else
    {
        z->parent = nil;
        if (y == nil) //此时红黑树为空树
        {
            root = z; //根节点即为z
        }
        else
        {
            if (key < y->key) //z应该放在z的左儿子上
            {
                z->parent = y;
                y->left = z;
            }
            else //z应该放在y的右儿子上
            {
                y->right = z;
                z->parent = y;
            }
        }
        z->left = z->right = nil;
        RBT_Insert_Fixup(z);//之后调用 插入恢复函数
        printf("插入成功！ ");
        count++;
    }//if
}
RBT_P Tree_Successor(RBT_P y)
{
    RBT_P m, n;
    m = n = y->left;
    while (n != nil)
    {
        m = n;
        n = n->right;
    }
    return m;
}//找到小于节点z的节点（即z的左子树） 中最大的节点
RBT_P RBT_MIN(RBT_P pRoot) {
    while (nil != pRoot->left) {
        pRoot = pRoot->left;
    }
    return pRoot;
}
RBT_P RBTREE_SUCCESSOR(RBT_P pRoot) {//查找指定结点的后继节点
    if (nil != pRoot->right) {
        return RBT_MIN(pRoot->right);
    }
    RBT_P pParent = pRoot->parent;
    while ((nil != pParent) && (pRoot == pParent->right)) {
        pRoot = pParent;
        pParent = pRoot->parent;
    }
    return pParent;
}

void RBT_Delete_Fixup(RBT_P x)
{
    while (x->color == BLACK&& x != root)
    {
        if (x == x->parent->left) //x 为左子树
        {
            RBT_P brother = x->parent->right;
            if (brother->color == RED)
                //此时为情况 3
            {
                 brother->color = BLACK;
               x->parent->color = RED;
                Left_Rotate(x->parent);
                brother = x->parent->right;
            }
            if ((brother->left->color == BLACK) && (brother->right->color == BLACK))
                //此时为情况 4
            {
                brother->color = RED;
                x = x->parent;
            }
            else
            {
                if (brother->right->color == BLACK)
                    //此时为情况 5
                {
                    brother->color = RED;
                    brother->left->color = BLACK;
                    Right_Rotate(brother);
                    brother = x->parent->right;
                }
                //此时为情况 6
                 x->parent->color = BLACK;
               brother->color = x->parent->color;
                brother->right->color = BLACK;
                Left_Rotate(x->parent);
                x = root;
            }
        }
        else //x为右子树（）
        {
            RBT_P brother = x->parent->left;
            if (brother->color == RED)
                //此时为情况 3
            {
                 brother->color = BLACK;
               x->parent->color = RED;
                Right_Rotate(x->parent);
                brother = x->parent->left;
            }
            if ((brother->left->color == BLACK) && (brother->right->color == BLACK))
                //此时为情况 4
            {
                brother->color = RED;
                x = x->parent;
            }
            else
            {
                if (brother->left->color == BLACK)
                    //此时为情况 5
                {
                    brother->color = RED;
                    brother->right->color = BLACK;
                    Left_Rotate(brother);
                    brother = x->parent;
                }
                // 此时为情况 6 
                brother->left->color = BLACK;
                brother->color = x->parent->color;
                x->parent->color = BLACK;
               
                Right_Rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK; //此时为情况1、 2
}
void RBT_Delete(int key)
{
    RBT_P y = RBT_Search(key);
    RBT_P z, x;
    if (y->key != key) //节点不存在
    {
        printf("您所要删除的节点并不存在！ \n");
        // return 0;
    }
    else
    {
        if (y->left == nil || y->right == nil)
        {
            z = y; //情况 1、 2 要进行的操作
        }
        else //所要删除节点的左右子树都不是nil（叶子） 节点
        {
            z = Tree_Successor(y); // 情况 3要进行的操作
        }
        if (z->left != nil)
        {
            x = z->left;
        }
        else
        {
            x = z->right;
        }
        x->parent = y->parent;
        if (z->parent == nil)
        {
            root = x;
        }
        else
        {
            if (z == z->parent->left)
            {
                z->parent->left = x;
            }
            else
            {
                z->parent->right = x;
            }
        }
        if (y != z) //情况 3 要进行的操作
        {
            z->key = y->key;
        }
        if (z->color == BLACK) //实际要删除的节点的为黑色是进行恢复
        //因为若删除节点颜色为红色， 则不会更改原有
        //红黑树的任何性质
        {
            RBT_Delete_Fixup(x); //删除恢复函数
        }
        printf("成功删除!\n");
        count--;
    } //if
}// 删除某一个节点
void K_min_elem(int k) {
    int i;
    if (k > count)printf("k值过大");
    else {
        RBT_P x = RBT_MIN(root);
        for (i = 2; i <= k; i++)x = RBTREE_SUCCESSOR(x);
        printf("第 K小元素为：%d,元素颜色为：%s\n", x->key, x->color == BLACK ? "黑" : "红");
    }
}
void Get()
{
    InitRBT();
    int choose = 1, key;
    while (choose)
    {
        printf("请输入您要添加的节点:");
        scanf("%d", &key);
        RBT_Insert(key);
        count++;
        printf("是否继续输入：0.否1.是\n");
        scanf("%d",&choose);
    }
    printf("红黑树创建成功！\n");
    system("pause");
}
int main()
{
    InitRBT();
    int choose,num;
    system("cls");
    system("color F0");
    printf("请先创建一个红黑树，依次输入元素的整型数值,按要求进行输入\n");
    Get();
    fflush(stdin);
    while (1)
    {
        system("cls");
        printf(" ***********************************************\n");
        printf(" ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^ ^_^\n");
        printf(" ***********************************************\n");
        printf(" ______________________________________________\n");
        printf("┃                                            ┃\n");
        printf("┃           动态查找第k小元素问题            ┃\n");
        printf("┃____________________________________________┃\n");
        printf("***********************************************\n");
        printf("***********************************************\n");
        printf("**              1.插入元素                   **\n");
        printf("**              2.查找指定元素               **\n");
        printf("**              3.删除元素                   **\n");
        printf("**              4.查找第k小元素              **\n");
        printf("**              5.退出系统                   **\n");
        printf("***********************************************\n");
        printf("^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^  ^_^ ^_^\n");
        printf("***********************************************\n");
        printf("               选择操作：");
        scanf("%d", &choose);
        switch (choose)
        {
        case 1:
            system("cls");
            printf("请输入你要插入的元素:");
            scanf("%d", &num);
            RBT_Insert(num);
            system("pause");
            break;
        case 2:
            system("cls");
            Search();
            system("pause");
            break;
        case 3:
            system("cls");
            printf("请输入你要删除的元素:");
            scanf("%d", &num);
            RBT_Delete(num);
            system("pause");
            break;
        case 4: 
            system("cls");
            int k;
            printf("请输入K值：");
            scanf("%d", &k);
            K_min_elem(k);
            system("pause");
            break;
        case 5:
            printf("感谢您的使用");
            exit(0);
            break;
        default:break;
           
        }

    }
}