/***********************************************************
 * 第8章 排序
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏
// 2. 视频：b站视频《数据结构》by王卓

//导入相关的头文件
# include <stdio.h> // 标准输入输出流
# include <iostream> // 输入输出流
# include <fstream>  // 文件输入输出流
# include <cstring>  // 字符串操作函数
using namespace std; // 使用标准命名空间   
// Status定义
#define OK 1
#define ERROR 0 
#define OVERFLOW -2
#define True 1
#define False 0
typedef int Status; // Status是函数的类型，其值是函数结果状态代码

#define MAXSIZE 20 // 顺序表的最大长度
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
typedef struct RedType{ // 记录类型
    KeyType key; // 关键字类型
    InfoType otherinfo; // 其他数据域
} RedType; // RedType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。
typedef struct SqList{ // 顺序表类型
    RedType r[MAXSIZE+1]; // r[0]闲置或者做哨兵。r[1]~r[length]是线性表的元素
    int length; // 线性表的长度
} SqList; // SqList是顺序表，包含关键字和其他数据域。RedType是线性表的元素类型，包含关键字和其他数据域。


/*==============================================================
* 8.2 插入排序
- 基本思想：将一个记录插入到已经排好序的序列中，保持序列的有序性。（相当于扑克牌抓牌）
 *==============================================================*/


 /* -------------- 8.2.1 直接插入排序（Straight Insertion Sort）--------------- */

// 【算法8.1】直接插入排序
// 参考视频：第14周02--第8章排序2--8.2插入排序1--直接插入排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=160
// 参考课本：p2237-p238
// 时间复杂度:
//  - 最好：O(n)（顺序），
//  - 最坏：O(n^2)（逆序），
//  - 平均：O(n^2)（无序）。
// 空间复杂度：O(1)（只需要常数级的辅助空间）。
// 基本思路：将一个记录插入到已经排好序的序列中，保持序列的有序性，用到顺序查找法。
// 排序稳定性：稳定。
// 性能对于顺序、无序、逆序的区别：顺序最好，O(n)；无序次之，O(n^2)；逆序最差，O(n^2)。
// 存储结构：可以用顺序存储结构，也可以用链式存储结构。
// 适用情况：初始记录有序或者基本有序，n较小的情况。

void InsertSort(SqList &L){
    // 对顺序表L进行直接插入排序
    int i,j;
    for(i=2; i<=L.length; ++i){ // 从第2个元素开始(第1个不需要比较)，依次插入到前面已经排好序的部分。（将r[i]与r[i-1],r[i-2],...,r[1]进行从后往前的比较）
        if(L.r[i].key < L.r[i-1].key){ // 如果当前元素小于前一个元素，则进行插入排序。否则保留在原位置，不需要交换。（然后继续for循环）
            L.r[0] = L.r[i]; // 将当前元素保存到哨兵位置
            // L.r[i] = L.r[i-1]; // 将r[i-1]后移一位。
            // for (j=i-2; L.r[0].key < L.r[j].key; --j){ // 从后向前查找插入位置。注意是从j=i-2开始，因为涉及到[j+1]的赋值。
            //     L.r[j+1] = L.r[j]; // 如果当前元素小于前一个元素，则将前一个元素r[j]后移一位。（逐个后移直到找到插入位置）
            //     // 此时遇到哨兵元素L.r[0]（即j=0时），就自动停止了，因为L.r[0].key == L.r[j].key。
            // }
            // 以上5行是教材的代码，但是我认为可以简化成下面的4行，这样也方便和其他插入排序算法统一一下j的初始值为i-1：
            for(j=i-1; L.r[0].key < L.r[j].key; --j){ // 从后向前查找插入位置。注意是从j=i-1开始，因为涉及到[j+1]的赋值。
                L.r[j+1] = L.r[j]; // 如果当前元素小于前一个元素，则将前一个元素r[j]后移一位。（逐个后移直到找到插入位置）
                // 此时遇到哨兵元素L.r[0]（即j=0时），就自动停止了，因为L.r[0].key == L.r[j].key。
            }
            // 可以看出除了哨兵节点的赋值之外，其他的赋值（即元素的移动）都是索引下标更小的元素往后移动一位，赋值给索引下标更大的元素。
            L.r[j+1] = L.r[0]; // 将哨兵元素插入到正确的位置。注意是j+1不是j，因为j已经经历了一次--j。
        }
    }
}

 /* -------------- 8.2.2 折半插入排序(Binary Insertion Sort)--------------- */

// 【算法8.2】折半插入排序（二分插入排序）
// 参考视频：第14周03--第8章排序3--8.2插入排序2--折半插入排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=161
// 参考课本：p238-p239
// 时间复杂度:
//  - 最好：O(nlogn)（顺序），
//  - 最坏：O(n^2)（逆序），
//  - 平均：O(n^2)（无序）。
// 空间复杂度：O(1)（只需要常数级的辅助空间）。
// 基本思路：将一个记录插入到已经排好序的序列中，保持序列的有序性，用到二分查找法。（算是对直接插入排序的改进）
// 排序稳定性：稳定。
// 性能对于顺序、无序、逆序的区别：没有区别。如果是基本顺序，那么关键字比较次数反而比直接插入排序要多。
// 存储结构：不能用链式存储结构，因为链式存储结构没有随机访问的特性，不能用二分查找法。
// 适用情况：初始记录无序，n较大的情况

void BInsertSort(SqList &L){
    // 对顺序表L进行折半插入排序
    int i,j,low,high,mid;
    for(i=2; i<=L.length; ++i){ // 从第2个元素开始(第1个不需要比较)，依次插入到前面已经排好序的部分。
        L.r[0] = L.r[i]; // 将当前元素保存到哨兵位置
        // 下面用折半（二分）查找法查找插入位置（替代顺序查找法）
        low = 1; high = i-1; // low和high分别是查找范围的下限和上限，相当于在r[1]~r[i-1]中查找插入位置。
        while(low <= high){ // 二分查找插入位置
            mid = low + (high-low) / 2; // 取中间位置
            if(L.r[0].key < L.r[mid].key) high = mid - 1; // 如果当前元素小于中间元素，则在左半部分查找
            else low = mid + 1; // 如果当前元素大于中间元素，则在右半部分查找
        } // 循环结束，high+1是插入位置
        // 因为最后一次while循环时，mid=low=high, 如果r[0].key < r[mid].key, 则high=mid-1, 此时应该插入到mid也就是high+1的位置；
        //      如果r[0].key >= r[mid].key, 则low=mid+1, 此时应该插入到mid+1也就是high+1的位置。
        //      这种if和else的先后顺序（先用<号赋值high，然后再用≥号赋值low)也能保持排序的稳定性。
        for(j=i-1; j>=high+1; --j){ // 将r[high+1]到r[i-1]的元素都往后移动一位，变成r[high+2]到r[i]。
            L.r[j+1] = L.r[j]; 
            // 此时遇到哨兵元素L.r[0]（即j=0时），就自动停止了，因为L.r[0].key == L.r[j].key。
        } // 注意是j=i-1而不是i-2，因为BInsertSort没有像InsertSort一样，一开始就进行了L.r[i] = L.r[i-1]的赋值。
        L.r[high+1] = L.r[0]; // 将哨兵元素插入到正确的位置。注意是high+1不是high。此时其实high+1=j+1。
    }
}
    
/* -------------- 8.2.3 希尔排序（Shell Sort）--------------- */

// 【算法8.3】希尔排序（Shell Sort）
// 参考视频：第14周04--第8章排序4--8.2插入排序3--希尔排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=162
// 参考课本：p239-p241
// 时间复杂度：O(n^1.25)，空间复杂度：O(1)。
// 基本思路：是对直接插入排序的改进，先将整个待排元素序列分割成若干个子序列分别进行直接插入排序，在最后一次直接插入排序时，整个序列是基本有序的，能近似实现直接插入排序的最佳性能（O(n)）。
// 排序稳定性：不稳定。
// 性能对于顺序、无序、逆序的区别：没有区别。
// 存储结构：不能用链式存储结构，因为链式存储结构没有随机访问的特性，不方便跳着增量访问元素。
// 适用情况：初始记录无序，n较大的情况。

void ShellInsert(SqList &L, int dk){
    // 对顺序表L做1趟增量（步长）是dk的希尔插入排序。
    int i,j; // i是当前元素，j是前一个元素
    // 以下内容相当于直接插入排序的改进，只不过将dk从1抽象成了一个变量dt。
    for(i=dk+1; i<=L.length; ++i){ // 从第dk+1个元素开始(前面已经排好序的部分)，依次插入到前面已经排好序的部分。
        if(L.r[i].key < L.r[i-dk].key){ // 将L.r[i]插入有序增量子表。
            L.r[0] = L.r[i]; // 将当前元素暂存到L.r[0]。此时L.r[0]没有起到哨兵的作用，只是暂存。
            for(j=i-dk; j>0 && L.r[0].key < L.r[j].key; j-=dk){ // 从后向前查找插入位置。注意是从j=i-dk开始，因为涉及到[j+dk]的赋值。
                // 注意额外添加了一个条件j>0, 这样可以避免j<0时访问越界（因为此时dk!=1, L.r[0]失去了哨兵的作用）。
                L.r[j+dk] = L.r[j]; // 如果当前元素小于前一个元素，则将前一个元素r[j]后移dk位。
            }
            L.r[j+dk] = L.r[0]; // 将r[0]也就是原来的L.r[i]插入到正确的位置。注意是j+dk不是j，因为j已经经历了一次-dk。
        }
    }
}
void ShellSort(SqList &L, int dlta[], int t){
    // 按照增量序列dlta[]对顺序表L进行t趟希尔排序。
    // dlta[]是增量序列，t是增量序列的长度。dlta[0]>dlta[1]>...>dlta[t-1]=1,且 dlta所有元素互质。
    int i,j,k,dk; // dk是当前增量
    for(k=0; k<t; ++k){ // 遍历增量序列
        ShellInsert(L, dlta[k]); // 对顺序表L进行希尔排序
    }
}