# SAT问题--DPLL算法--数独应用

### Ⅰ、SAT问题

1. **文字（literal）**：原子命题及其否定称为文字。其可以使用布尔变量进行表示，其值为真 或 假。

   e.g.  p , r , q 和 ¬ p , ¬ r , ¬ q 都是文字

2. **子句（clause）** 子句可以是简单析取式：仅由有限个文字构成的析取式称为子句或简单析取式。

   e.g. p ∨ q ∨ ¬ r  即为一个子句。

3. **合取范式（Conjunctive Normal Form, CNF）**：仅由有限个简单析取式构成的合取式称为合取范式。

   e.g.  ( − x 1 ∨ − x 2 ∨ x 3 ) ∧ ( − x 3 ∨ x 4 ) 

**可满足性问题（Satisfiability）** 是用来解决给定的真值方程式，是否存在一组变量赋值，使问题为可满足。布尔可满足性问题（Boolean satisfiability problem SAT）属于决定性问题，也是第一个被证明属于NP完全的问题。

- 非确定性问题：有的问题无法通过直接的算式计算（比如质因数分解）得到，只能通过间接的猜算来得到结果。
- 而且可以告知你"猜算"的答案正确与否的算法，假如其可以在多项式时间内算出来，其就是NP问题。
- 如果这个问题的所有可能答案，都可以在多项式的时间内进行正确与否的演算，即NPC（Non-deterministic Polynomial Complete）问题。

### Ⅱ、DPLL算法

DPLL（Davis-Putnam-Logemann-Loveland）算法，是一种完备的、以回溯为基础的算法，用于解决CNF-SAT问题。DPLL算法是一种搜索算法，搜索所有可能的赋值排列。

#### 算法思想

简单地讲，算法会在公式中选择一个变量（literal），将其赋值为**True**，化简赋值后的公式，如果简化的公式是可满足的（递归地判断），那么原公式也是可满足的。

否则就反过来将该变量赋值为**False**，再执行一遍递归的判定，若也不能满足，那么原公式便是不可满足的，回溯回到上一个可以排定为真的情况，继续选择下一个变量。

根据上述思想，在最坏的情况下，其回溯会导致一次复杂度为指数时间的计算。

DPLL算法主要使用两种基本处理策略：

#### 单位传播

定义 **单位子句（unit clause）** 为只有一个文字的子句，例如  p 或者 ¬ p。如果一个CNF为真，则在其每个子句内，至少有一个变量为真。假如出现了单位子句的情况，而**单位子句又只有一个变量，所以这个变量也必须为真**。如果一个合取范式包含一个单位子句 p，那么该CNF中的其他子句就可以被 p化简。具体做法如下：

1. 如果某个子句中含有 p，那么就把这个**子句**移除。因为此时这个子句已经为真了，其他变量的取值已经不重要。

   e.g.( ( p ∨ q ∨ r ) ∧ (  x  ∨ y  )  ∧  r )如上面的式子，有单位子句r，则 r 的值一定为 真 ，则CNF的第一个子句也必须为真。经过UP得到 ( ( x ∨ y ) ∧ r ) 

2. 如果某个子句中含有 ¬ p ，那么就把这个子句中的 ¬ p 移除。因为这个子句如果为真，其取值已经不取决于 ¬ p，因此删掉。
   ( ( p ∨ q ∨ ¬ r ) ∧ (  x  ∨ ¬ r  )  ∧  r )如上面的式子，有单位子句r，则 r 的值一定为 真 ，则CNF的第一个子句中的 非r 不影响其子句为真的情况，可以移除，同理对第二个子句进行同样的操作。经过UP得到 ( ( p ∨ q ) ∧ x  ∧  r )

这样，最后发现CNF为空，就说明所有子句都满足了，即该CNF是可满足的；如果发现CNF中有一个子句为空，则表示了此次赋值尝试是失败的。如果每次赋值（即猜测）都出现了这种情况，那么该CNF就是不可满足的。

#### 分裂策略

按某种策略选取一个文字*L*.如果*L*取真值，则根据单子句传播策略，可将*S*化成*S*2；若*L*取假值（即¬L成立）时，*S*可化成*S*1.

交错使用上述两种策略可不断地对公式化简，并最终达到终止状态，其执行过程可表示为一棵二叉搜索树。

基于单子句传播与分裂策略的DPLL算法可以描述为一个的递归过程, DPLL算法也可用非递归实现。

```c++
DPLL(S) :{/* S为公式对应的子句集。若其满足，返回TURE；否则返回FALSE. */
while(S中存在单子句) {//单子句传播
    在S中选一个单子句L；
    依据单子句规则，利用L化简S；
    if S = Φ return(TRUE);
    else if (S中有空子句 ) return（FALSE）；
}
基于某种策略选取变元v；//策略对DPLL性能影响很大
    if DPLL（S∪v）return(TURE);//在第一分支中搜索
    return DPLL(S∪¬v);//回溯到对v执行分支策略的初态进入另一分支
}
```

#### CNF文件格式

在每个CNF文件的开始，由‘c’开头的是若干注释说明行；

‘p’开头的行说明公式的总体信息，包括：范式为CNF；公式有200个布尔变元，由1到200的整数表示；320个子句。

之后每行对应一个子句，0为结束标记。46表示第46号变元，且为正文字；-46则是对应的负文字，文字之间以空格分隔。

![image-20240903151956166](C:\Users\lzh\AppData\Roaming\Typora\typora-user-images\image-20240903151956166.png)

### Ⅲ、应用于数独

lz也是在阅读了上面的讲解后，加深了理解。开篇有个例子，就是数独，其也是**NPC**问题，便希望将数独用 CDLL 算法求解出来。再阅读了多篇GitHub后，写出了自己的代码，并且自认为其是正确的解法（为什么是自认为，因为在GitHub上我用心去看的都是错误的。。。有小bug，也有理解就出现错误的，所以说，如果我错了，希望各位可以dd我，指正我）。

#### 生成数独

首先是生成合法的9×9数独，对于需要求解的问题数独将已经生成的合法的数独挖去几个方块，就生成了数独问题。

那么生成数独的策略即为先随机生成第一行，再根据数独的要求一个个填充剩下的9×9方格的空缺。数独的合法性要求如下：

1. 行不能出现重复的数字
2. 列不能出现重复的数字
3. 在3×3的子方格中也不能出现重复的数字

实现代码

```cpp
#include "Global.h"
using namespace std;

void ShowProblem(int a[][9]) {//打印数独
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (a[i][j] != 0) printf("%d ", a[i][j]); 
      else printf("_ ");
    }
    printf("\n");
  }
}
void GenerateFirstLine(int a[]) {//产生首行
  srand((unsigned) time(NULL));//设置随机数种子为真随机数 
  for (int i = 0; i < 9; ++i) {
    a[i] = rand() % 9 + 1;
    int j = 0;
    while (j < i) {
      if (a[i] == a[j]) {
        a[i] = rand() % 9 + 1; //生成随机的不同的9个数字
        j = 0;
      } 
      else j++;
    }
  }
}

int CompleteSudoku(int a[][9], int x, int y) {//补全数独
//排除法，先看此位置不能填几
  if (x < 9 and y < 9) {
    int check[10];//用来表示没使用过的即合法的数字。0合法 ，1非法
    memset(check, 0, sizeof(check));//填充函数memset(指针，值，数量)，对check数组进行初始化
    for (int i = 0; i < y; ++i) {
      check[a[x][i]] = 1; //在第x行出现的数字非法
    }
    for (int i = 0; i < x; ++i) {
      check[a[i][y]] = 1;// 在第y列出现的数字非法
    }
    for (int i = x / 3 * 3; i < x / 3 * 3 + 3; ++i) {//在x,y处出现的值不能在其3×3的小方格内出现
      for (int j = y / 3 * 3; j < y / 3 * 3 + 3; ++j) {
        if (i == x or j == y) {
          continue;//跳过x，y位置
        }
        check[a[i][j]] = 1;
      }
    }
    int fl = 0;//0错1对
    for (int i = 1; i <= 9 and fl == 0; ++i) { //从check中合法的值开始一个个选取
      if (check[i] == 0) {
        fl = 1;
        a[x][y] = i;
        if (y == 8 and x != 8) {//所选方格到了最后一列就向下一行第一个移动
          if (CompleteSudoku(a, x + 1, 0) == 0) return 0;
          else fl = 0;
        } 
        else if (y != 8) {//即如果所选方格没在最后一列时，向后移动即可
          if (CompleteSudoku(a, x, y + 1) == 0) return 0;
          else fl = 0;
        }
      }
    }
    if (fl == 0) {//出错
      a[x][y] = 0;
      return 1;
    }
  }
  return 0;
}

void CreateSudoku(int a[][9]) {//产生完整的数独
  GenerateFirstLine(a[0]);
  CompleteSudoku(a, 1, 0);//从第二行第一个开始补全
}

void CreateProle(int a[][9], int b[][9], int empty_number) {
  // a为原来完整的数独，b为我们需要的数独问题
  srand((unsigned) time(NULL));//设置随机数种子为 真随机 数
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      b[i][j] = a[i][j];//把a赋值给b，b现在是完整的数独
    }
  }
  for (int i = 0; i < empty_number; ++i) {//随机清除数字
    int x = rand() % 9;
    int y = rand() % 9;
    if (b[x][y] != 0)  b[x][y] = 0; 
    else  i--;
  }
}

void CreateSudokuProblem(int empty_number) {
  int sudoku[9][9] = {0};
  int sudoku_backup[9][9] = {0};
  CreateSudoku(sudoku);
  CreateProle(sudoku, sudoku_backup, empty_number);
  ShowProblem(sudoku_backup);
  string path = "..\\sudoku.cnf";
  Sudoku2CNF(path, sudoku_backup, empty_number);
}
```

#### 数独 to CNF

生成数独后，由于我们的SAT Solver只能处理CNF，因此如何将数独转化成CNF才是这个问题中最困难的点。

首先我们需要确立变量的表示， X 111为一个变量，表示第一行第一列的方块的值为1，那么同理X 558表示第5行第5列的方块的值为8。

有了数独如何表示成变量后，那么构建CNF的规则如何制定呢。上述华盛顿大学的文章中，在末尾处其实已经提到了。其也就是我们生成数独时需要遵守的规则。
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/46938c8eaa8d8a022979517e6164666c.png)
现在让我用专业的逻辑表达式来表示4条规则（第四条规则可能表达不太对，思想下面会解释）

![image-20240831093824227](C:\Users\lzh\AppData\Roaming\Typora\typora-user-images\image-20240831093824227.png)

对于以上4条规则，其分别代表了每个方块只有一个取值，行不能重复，列不能重复和每个3×3的子方块不能重复这四条规则。可能最后一条规则不是很好理解，我用图的方式表示。

最后一条规则，表示了1-9的z的取值中，我们行有0-2的i ，列有0-2的j
共9 子3×3的方块，在每个方块中行有1-3的x ，列有1-3的y共9 子1×1的方块。而 k 和 l 则表示其不能和我们选中的方块在同一行或者同一列。
以下图为例，当我们的x=1 y=2时，行列不能重复的原则已经被红线所覆盖，那么我们现在就需要去完成（2，1），（3，1），（2，3），（3，3）这四个点的值不能和（1，2）一致。观察发现，（k，l）的值不可能与（x，y）所重复，因此得此规则。
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9539db67d6ab09467c201b00b692295a.png)
再根据以上情况，我们算出转换后的CNF共有多少子句。
规则1：生成了 81 - 数独空缺的点（数独上已有的点）。
规则2：9 × 9 ×（1 + 2 + 3 +…8）= 2916
规则3：9 × 9 ×（1 + 2 + 3 +…8）= 2916
规则4：9 × 3 ×3 × 9 ×4 = 2916
所以，该CNF总共的变量为 9行 × 9列 ×9个可能的取值，729。子句个数为
8829 - 数独空缺的点。
根据以上规则编写代码如下。

```cpp
void Sudoku2CNF(string path, int a[][9], int empty_number) {
  ofstream cnf_file;
  cnf_file.open(path, ios::out);//定义输入文件
  if (!cnf_file)  printf("can't open!\n");
  cnf_file << "p" << " " << "cnf" << " " << 729 << " " << 8829 + 81 - empty_number << endl;//根据已有的数字进行子句的添加
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (a[i][j] != 0) 
        cnf_file << (i + 1) * 100 + (j + 1) * 10 + a[i][j] << " " << 0 << endl;
    }
  }
  //单个方块内只能出现1-9中的其中一个
  for (int i = 1; i <= 9; ++i) {
    for (int j = 1; j <= 9; ++j) {
      for (int z = 1; z <= 9; ++z) {
        cnf_file << i * 100 + j * 10 + z << " ";
      }
      cnf_file << 0 << endl;
    }
  }
  //行不能出现重复的规则
  for (int z = 1; z <= 9; ++z) {
    for (int j = 1; j <= 9; ++j) {
      for (int i = 1; i <= 8; ++i) {
        for (int k = i + 1; k <= 9; ++k) {
          cnf_file << 0 - (i * 100 + j * 10 + z) << " " << 0 - (k * 100 + j * 10 + z) << " " << 0 << endl;
        }
      }
    }
  }
  //列不能出现重复的规则
  for (int z = 1; z <= 9; ++z) {
    for (int i = 1; i <= 9; ++i) {
      for (int j = 1; j <= 8; ++j) {
        for (int k = j + 1; k <= 9; ++k) {
          cnf_file << 0 - (i * 100 + j * 10 + z) << " " << 0 - (i * 100 + k * 10 + z) << " " << 0 << endl;
        }
      }
    }
  }
  //每个 3 ×3的小方格内不能有重复的数字
  for (int z = 1; z <= 9; ++z) {
    for (int i = 0; i <= 2; ++i) {
      for (int j = 0; j <= 2; ++j) {
        for (int x = 1; x <= 3; ++x) {
          for (int y = 1; y <= 3; ++y) {
            for (int k = 1; k <= 3; ++k) {
              for (int l = 1; l <= 3; ++l) {
                if (x == k or y == l) {
                  continue;
                } else {
                  cnf_file << 0 - (k * 100 + l * 10 + z) << " " << 0 - (x * 100 + y * 10 + z) << " " << 0 << endl;
                }
              }
            }
          }
        }
      }
    }
  }
  cnf_file.close();
}
```

#### 注意点

之后我们存储的CNF 的变量个数为729个，但在CNF的子句中出现了999这样的数字，代表了9行9列其值为9.由于999 > 729。在程序的编写过程中，如果我们用正常的DPLL SAT来解决会出现越界的情况，因此需要对原有的DPLL SAT进行一些修改，将999 对应到 729， 将111 映射到 1。此处在程序实现上很简单不再过多赘述，提出这个点即可。

上述内容均在[Github](https://github.com/HJNODM/DPLL-SAT-Solver)，如果对这个内容感兴趣，欢迎阅读。

### Ⅳ、算法升级

优化：对变量出现的次数进行排序，**每次赋值时都将出现次数最大的变量首先赋值**，这样影响到的子句更多，更有可能回出现单位子句。

为加深其理解可以看这个[University of Washington | CSE 442](https://cse442-17f.github.io/Conflict-Driven-Clause-Learning/)。其中有该算法和 其升级版的CDCL的可视化交互操作与讲解，更加便于理解。

### CDCL算法

Conflict Driven Clause Learning 冲突驱动的子句学习

对于CDLL其回溯只能到上一层，但有时候错误早已出现，回溯一层显得太疲软。如果算法能根据冲突来进行更高的回溯，那么SAT的运行效率回非常高。这就是CDCL（Conflict-Driven Clause Learning）算法。其命名就是根据冲突来推动的子句学习算法。算法的理解还是请看上面的华盛顿大学的那片文章，具体内容看这位博主的文章 [合取范式可满足性问题：CDCL（Conflict-Driven Clause Learning）算法详解](https://blog.csdn.net/qaqwqaqwq/article/details/126020807) 或者这个 [Conflict-driven clause learning (CDCL) SAT solvers](https://users.aalto.fi/~tjunttil/2020-DP-AUT/notes-sat/cdcl.html)

### 参考文献

1. Clarke, E.M., Henzinger, Th.A., Veith, H., Bloem, R. (Eds.). Handbook of Model Checking. 2018.
2. [Conflict Driven Clause Learning University of Washington | CSE 442](https://cse442-17f.github.io/Conflict-Driven-Clause-Learning/)
3. [Conflict-driven clause learning (CDCL) SAT solvers](https://users.aalto.fi/~tjunttil/2020-DP-AUT/notes-sat/cdcl.html)
4. [Billy1900/DPLL-Algorithm](https://github.com/Billy1900/DPLL-Algorithm)
5. [sukrutrao / SAT-Solver-DPLL](https://github.com/sukrutrao/SAT-Solver-DPLL)
6. [DPLL algorithm - Wikipedia](https://en.wikipedia.org/wiki/DPLL_algorithm)
7. [Unit propagation - Wikipedia](https://en.wikipedia.org/wiki/Unit_propagation)