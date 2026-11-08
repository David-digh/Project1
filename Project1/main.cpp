

#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>
#include <vector>
#include <chrono>

#include "mystack.h"   
#include "linkstack.h"  
#include "cicuqueue.h"  
#include "linkqueue.h"  
#include "increstack.h" 
#include "increqueue.h" 

using namespace std;
using namespace std::chrono;

// ===================================================================
// 适配层：把 9 种容器收敛成同样的三个动作
//   doPush(c, x)  入
//   doPop(c, v)   出，元素通过 v 带出来
//   doEmpty(c)    判空

// ===================================================================

// ---------- 入 ----------
template <class C>
inline void doPush(C& c, int x)     { c.push(x); }          
inline void doPush(increqueue<int>& c, int x) { c.push_back(x); }  

// ---------- 出 ----------
template <class C>
inline void doPop(C& c, int& v)     { c.pop(v); }           
inline void doPop(increqueue<int>& c, int& v) { c.pop_front(v); }   
inline void doPop(std::stack<int>& c, int& v) { v = c.top(); c.pop(); }     
inline void doPop(std::stack<int, std::vector<int> >& c, int& v) { v = c.top(); c.pop(); }
inline void doPop(std::queue<int>& c, int& v) { v = c.front(); c.pop(); }

// ---------- 判空 ----------
template <class C>
inline bool doEmpty(C& c)           { return c.isempty(); }
inline bool doEmpty(std::stack<int>& c) { return c.empty(); }                   
inline bool doEmpty(std::stack<int, std::vector<int> >& c) { return c.empty(); }
inline bool doEmpty(std::queue<int>& c) { return c.empty(); }

// ===================================================================
// 场景 A：连续入 n 次，再连续出 n 次
//   new/delete 都在计时区外，只测容器操作本身
// ===================================================================
template <class C>
double benchA(int n)
{
    C* c = new C;                   // 数组类约 3.8MB，必须放堆上
    int v;
    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < n; ++i) doPush(*c, i);
    for (int i = 0; i < n; ++i) doPop(*c, v);
    auto t1 = high_resolution_clock::now();
    delete c;
    return duration<double, milli>(t1 - t0).count();
}

// ===================================================================
// 场景 B：n 次随机混合操作（约一半入、一半出），入的值也是随机的
//   cnt 用来守住"空不出、满不入"，保证所有操作合法
// ===================================================================
template <class C>
double benchB(int n, int cap, unsigned seed)
{
    C* c = new C;
    int v;
    int cnt = 0;
    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < n; ++i)
    {
        seed = seed * 1103515245u + 12345u;     // 自己算随机数，省掉 rand() 的开销
        bool doIns = ((seed >> 16) & 1u) != 0;
        if (cnt == 0) doIns = true;                     // 空的只能入
        else if (cap > 0 && cnt >= cap) doIns = false;  // 满的只能出
        if (doIns) { doPush(*c, (int)(seed % 1000000)); ++cnt; }
        else { doPop(*c, v); --cnt; }
    }
    auto t1 = high_resolution_clock::now();
    delete c;
    return duration<double, milli>(t1 - t0).count();
}

// ===================================================================
// 参与对比的 9 种实现（任务一 + 任务二的成果清单）
// ===================================================================
void show_structures()
{
    cout << "========== 参与对比的 9 种实现 ==========" << endl;
    cout << "任务一：6 种手写数据结构" << endl;
    cout << "  顺序栈 mystack      数组实现，容量 1000005" << endl;
    cout << "  链栈   linkstack    不带头结点，容量不限" << endl;
    cout << "  链队   linkqueue    带头结点，容量不限" << endl;
    cout << "  循环队列 circuqueue 数组实现，容量 " << MAXSIZE - 1 << endl;
    cout << "  递增栈 increstack   基于 std::stack，入栈自动有序" << endl;
    cout << "  递增队 increqueue   基于 std::deque，入队自动有序" << endl;
    cout << "任务二：3 种 STL 实现（含 1 组底层容器对照）" << endl;
    cout << "  std::stack<int>             底层 deque" << endl;
    cout << "  std::stack<int, vector<int>> 底层 vector（对照：同样是 STL，只换底层容器）" << endl;
    cout << "  std::queue<int>             底层 deque" << endl;
}

// ===================================================================
// 任务三：时间性能对比
// ===================================================================
void performance()
{
    cout << endl << "========== 任务三：时间性能对比 ==========" << endl;
    cout << "场景 A：连续入 n 次，再连续出 n 次" << endl;
    cout << "场景 B：n 次随机混合入/出" << endl;
    cout << "计时单位：毫秒(ms)，chrono::high_resolution_clock" << endl;
    cout << "说明：场景 A 入的是递增的 i。递增栈/递增队遇到递增序列每次直接压入(O(1))；" << endl;
    cout << "      场景 B 入的是随机值，递增结构每次要挪动元素(O(n))，两列不能混着看。" << endl;

    const int sizes[3] = { 10000, 100000, 1000000 };
    const int CAP_MYSTACK = 1000005;        // mystack.h 里的 STACKSIZE
    const int CAP_CIRCU = MAXSIZE - 1;      // 循环队列留一格判满

    cout << fixed << setprecision(3);

    for (int k = 0; k < 3; ++k)
    {
        int n = sizes[k];
        cout << endl << "----- 规模 n = " << n << " -----" << endl;
        cout << "  实现                场景A(ms)      场景B(ms)" << endl;

        cout << "  顺序栈 mystack      " << setw(14) << benchA<mystack<int> >(n)
             << "  " << setw(14) << benchB<mystack<int> >(n, CAP_MYSTACK, 12345) << endl;

        cout << "  链栈   linkstack    " << setw(14) << benchA<linkstack<int> >(n)
             << "  " << setw(14) << benchB<linkstack<int> >(n, 0, 12345) << endl;

        cout << "  链队   linkqueue    " << setw(14) << benchA<linkqueue<int> >(n)
             << "  " << setw(14) << benchB<linkqueue<int> >(n, 0, 12345) << endl;

        cout << "  循环队列 circuqueue " << setw(14) << benchA<circuqueue<int> >(n)
             << "  " << setw(14) << benchB<circuqueue<int> >(n, CAP_CIRCU, 12345) << endl;

        cout << "  递增栈 increstack   " << setw(14) << benchA<increstack<int> >(n)
             << "  " << setw(14) << benchB<increstack<int> >(n, 0, 12345) << endl;

        cout << "  递增队 increqueue   " << setw(14) << benchA<increqueue<int> >(n)
             << "  " << setw(14) << benchB<increqueue<int> >(n, 0, 12345) << endl;

        cout << "  STL    std::stack   " << setw(14) << benchA<std::stack<int> >(n)
             << "  " << setw(14) << benchB<std::stack<int> >(n, 0, 12345) << endl;

        cout << "  STL    stack+vector " << setw(14) << benchA<std::stack<int, std::vector<int> > >(n)
             << "  " << setw(14) << benchB<std::stack<int, std::vector<int> > >(n, 0, 12345) << endl;

        cout << "  STL    std::queue   " << setw(14) << benchA<std::queue<int> >(n)
             << "  " << setw(14) << benchB<std::queue<int> >(n, 0, 12345) << endl;
    }
}

// ===================================================================
// 任务三：内存占用对比（按类型大小估算，n = 1e6）
// ===================================================================
void memory_report()
{
    const long long n = 1000000;
    const double MB = 1024.0 * 1024.0;

    cout << endl << "========== 内存占用估算（n = 1000000）==========" << endl;
    cout << fixed << setprecision(2);

    cout << "  顺序栈 mystack      : " << sizeof(mystack<int>) << " 字节 = "
         << sizeof(mystack<int>) / MB << " MB（固定数组，与 n 无关）" << endl;
    cout << "  循环队列 circuqueue : " << sizeof(circuqueue<int>) << " 字节 = "
         << sizeof(circuqueue<int>) / MB << " MB（固定数组，与 n 无关）" << endl;

    cout << "  链栈   linkstack    : n × " << sizeof(node<int>) << " 字节 = "
         << n * (long long)sizeof(node<int>) / MB << " MB" << endl;
    cout << "  链队   linkqueue    : (n+1) × " << sizeof(node<int>) << " 字节 = "
         << (n + 1) * (long long)sizeof(node<int>) / MB << " MB（多一个头结点）" << endl;

    cout << "  递增栈 increstack   : n × sizeof(int) + 块管理开销 = "
         << n * (long long)sizeof(int) / MB << " MB 起" << endl;
    cout << "  递增队 increqueue   : n × sizeof(int) + 块管理开销 = "
         << n * (long long)sizeof(int) / MB << " MB 起" << endl;
    cout << "  STL    std::stack   : n × sizeof(int) + 块管理开销 = "
         << n * (long long)sizeof(int) / MB << " MB 起" << endl;
    cout << "  STL    stack+vector : 前 2^20 个槽 = " << (1 << 20) * (long long)sizeof(int) / MB
         << " MB（vector 按 2 倍扩容，容量可能比元素数多一倍）" << endl;
    cout << "  STL    std::queue   : n × sizeof(int) + 块管理开销 = "
         << n * (long long)sizeof(int) / MB << " MB 起" << endl;
}

// ===================================================================
// 主函数
// ===================================================================
int main()
{
    show_structures();   // 任务一 + 任务二：9 种实现清单
    performance();       // 任务三：时间
    memory_report();     // 任务三：内存
    return 0;
}