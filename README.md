
# 111062271_final

## ***Problem 1***
---
### ***Approach***
<hr class="hrlight">

> Constructor
#### 1. Sort the Graph Edge

<content>
將Graph Edge 根據Vertex建立相鄰串列(Adjoint List)，並依照Cost由小到大排序，此目的為了之後選取Edge時，只需選擇該Vetex第一條邊就是最小成本。
</content>

<summary>
Time complexity： $\footnotesize{O(E)+O(|E|log|E|) = O(|E|log|E|)}$<br>
Space complexity：$\footnotesize{O(|V|+|E|)}$
</summary>

<hr class="hrlight">

> Insert
#### 1. Create the Multicating Tree
<content>
將Vertex、Id資訊放入temporary MTid Tree，並利用class member bandwidth紀錄該tree的bandwidth以便之後使用
</content>

#### 2. Allocate the resource to the MTid Tree by Prim's algorithm

<content>
利用Prim's algorithm with priority queue (min heap)，
挑選當前minimum cost且remain bandwidth $\ge$ transmition cost的Edge
加入MTid Tree，並利用Contain vector紀錄該條邊對應的Graph edge index與更新Graph資訊，重複以上動作直到priority queue清空或已選取$\footnotesize{|V|-1}$條邊。

</content>

<summary>
Time complexity： $\footnotesize{O(|E|log|E|)}$<br>
Space complexity：$\footnotesize{O(|V|+|E|)}$
</summary>

<hr class="hrlight">

> Stop
#### 1. Release resource by id

<content>
在Forest尋找對應id的MTid Tree，並利用先前已記錄的bandwith與Contain vector釋放在Graph佔有的resource，
再erase相關紀錄釋放記憶體
</content>

<summary>
Time complexity： $\footnotesize{O(|V|+|E|)}$<br>
Space complexity：$\footnotesize{O(1)}$
</summary>

#### 2. Connect more Edge

<content>
計算每棵樹的need，即尚未連結點的個數，並使用Prim's algorithm with priority queue (min heap)，
將尚未滿足的MTid Tree以leaf nodes作為起始點，並將其關聯的edge符合以下條件
remain bandwidth $\ge$ transmition cost與edge destination node未包含在MTid Tree中
放入到priority queue中，重複添加Edge直到need == 0或priority queue為空
</content>

<summary>
Time complexity： $\footnotesize{O(|T||E|log|E|)}$ |T|為MTid Tree的個數<br>
Space complexity：$\footnotesize{O(|V|+|E|)}$
</summary>

<hr class="hrlight">

> Rearrange

#### 1. Release all of the resource
<content>
將所有MTid Tree 的資源release，並Update Graph
</content>

<summary>
Time complexity： $\footnotesize{O(|T|(|V|+|E|))}$ |T|為MTid Tree的個數<br>
Space complexity：$\footnotesize{O(1)}$
</summary>

#### 2. Allocate resource to each MTid tree in ascending order

<content>
根據Id number由小到大分配resource，使用與insert相同allocate方法
</content>

<summary>
Time complexity： $\footnotesize{O(|T||E|log|E|)}$ |T|為MTid Tree的個數<br>
Space complexity：$\footnotesize{O(|T|(|V|+|E|))}$
</summary>

---

### ***Struct***

<hr class="hrlight">

```cpp
#define bpair pair<int, int>  // <MTid's id, bandwidth>

struct edgeList
{
    int index; // 該edge對應graph的index
    int dest; // edge指向的destination
    int cost; // edge的cost
};

struct Contain {
    int id; // MTid tree 的 id
    vector<int> edges; // 所使用的edge對應graph edge index
};
```
---

### ***Class***

> Private member

```cpp
private:
	int size; // 紀錄vertex個數
	vector<edgeList>* adjList; // 排序graph edge後所建立的Adjoint List
	vector<vector<Contain>> contain; // 儲存MTid 所使用的graph edge index並依照source儲存 contain[source][i]
	vector<vector<bpair>> bandwidth; // 儲存MTid 所使用的bandwidth cost 並依照source儲存 bandwidth[source][i]
	Graph t_G; // 目前的Graph
	Forest t_F; // 目前的Forest
```
> public method

>insert

```cpp
void allocate(Graph &G, Tree &MTid, const int& s, const int& t, int& need)
```
<content2>
使用Prim's algorithm with priority queue (min heap)，呼叫<function>addEdge</function>添加Edge到MTid Tree
</content2>

#### input
<content2>
<variable>const int& s</variable>: source vertex<br>
<variable>const int& t</variable>: transmition cost<br>
<variable>int& need</variable>: 所需要Edge數量<br>
</content2>

<hr class="hrlight">

```cpp
void addEdge(Graph &G, Tree& MTid, edgeList it, int bandw)
```
<content2>
將此edge添加到MTid中，並更新Graph資訊
</content2>

#### input
<content2>
<variable>edgeList it</variable>: target edge，要添加的邊<br>
<variable>int bandw</variable>: bandwidth cost，代表該MTid所使用的bandwidth<br>
</content2>

<hr class="hrlight">

> stop

```cpp
void release(Graph &G, Tree &MTid, const int& bandw)
```
<content2>
釋放MTid所佔用的邊，並更新Graph資訊
</content2>

#### input
<content2>
<variable>const int& bandw</variable>: bandwidth cost，代表該MTid所使用的bandwidth<br>
</content2>

<hr class="hrlight">

```cpp
void allocateAddiction(Graph &G, Tree &MTid, const int& s, int& need, const int& vi)
```

<content2>
將該MTid額外添加更多的邊
</content2>

#### input
<content2>
<variable>const int& s</variable>: source vertex<br>
<variable>int& need</variable>: 所需要Edge數量<br>
<variable>const int& vi</variable>: contain[s][vi]，vi代表在contain[s]中對應MTid在哪個index位置<br>
</content2>

<hr class="hrlight">

```cpp
int getBandwith(const int& s, const int& id);
int getContainIndex(const int s, const int& id);
```
<content2>
分別代表取得該MTid在bandwidth與contain中所對應的index
</content2>

#### input
<content2>
<variable>const int& s</variable>: source vertex<br>
<variable>const int& id</variable>: MTid中的id<br>
</content2>

#### output
<content2>
在private member中的bandwidth與contain中所對應的index
</content2>

<hr class="hrlight">

## summary

<summary>
<function>Insert</function>使用了<function>allocate</function>、<function>addEdge</function></br>
<function>Stop</function>使用了
<function>release</function>、
<function>allocateAddiction</function>、
<function>getBandwith</function>、
<function>getContainIndex</function></br>
<function>Rearrange</function>使用了
<function>release</function>、
<function>allocate</function>、
<function>getBandwith</function>、
<function>getContainIndex</function></br>
</summary>

---
## ***Problem 2***
---
### ***Approach***
<hr class="hrlight">

> Constructor
#### 1. Sort the Graph Edge

<content>
將Graph Edge 根據Vertex建立相鄰串列(Adjoint List)，並依照Cost由小到大排序，此目的為了之後選取Edge時，只需選擇該Vetex第一條邊就是最小成本。
</content>

<summary>
Time complexity： $\footnotesize{O(E)+O(|E|log|E|) = O(|E|log|E|)}$<br>
Space complexity：$\footnotesize{O(|V|+|E|)}$
</summary>

<hr class="hrlight">

> Insert

#### 1. Create the Multicating Tree
<content>
將Vertex、Id資訊放入temporary MTid Tree，並利用class member bandwidth紀錄該tree的bandwidth以便之後使用
</content>

#### 2. Find all pairs shortest path (Metric Closure)

<content>
利用dijkstra algorithm with priority queue (min heap) 
尋找所有terminal vertex shotest path並紀錄shotest path所使用的edge，
其中如果Edge remain bandwidth < transmission cost 則自動忽略
</content>

<summary>
Time complexity： $\footnotesize{O(|V||E|log|V|)}$ where V is terminal vertex set<br>
Space complexity：$\footnotesize{O(|V|^2+|E|)}$ where V is terminal vertex set
</summary>

#### 3. Construct Approximation Steiner Tree with all pairs shortest path (Metric) and MST

<content>
利用Prim's algorithm with priority queue (min heap)，
從source出發，並挑選Metric中當前terminal vertex所對應最小cost distance 的 other terminal vertex，並將其所對應shortest path的edges加入MTid Tree中，重複以上動作直到priority queue清空或已選取$\footnotesize{|V|-1}$個Edges，若包含沒有所有terminal vertex則清空記憶體，返回false，反之成功則true
</content>

<summary>
Time complexity： $\footnotesize{O(|V||E|log|V|)}$ where V is terminal vertex set, and E is the number of Edge<br>
Space complexity：$\footnotesize{O(|V|^2+|E|)}$ where V is terminal vertex set, and E is the number of Edge
</summary>


<hr class="hrlight">

> Stop
#### 1. Release resource by id

<content>
在Forest尋找對應id的MTid Tree，並利用先前已記錄的bandwith與UsageEdge vector釋放在Graph佔有的resource，
再erase相關紀錄釋放記憶體
</content>

<summary>
Time complexity： $\footnotesize{O(|V|+|E|)}$<br>
Space complexity：$\footnotesize{O(1)}$
</summary>

#### 2. Find the maximum cost to Contruct MTid Tree

<content>
釋放記憶體後，從transmission cost最大的unsatisfied Tree查看是否能夠分配資源，並利用與Insert相同方法建立MTid (Approximation Steiner Tree)
</content>

<summary>
Time complexity： $\footnotesize{O(|T||V||E|log|V|)}$ where V is terminal vertex set and T is the number of Multicast Tree<br>
Space complexity：$\footnotesize{O(|T|(|V|^2+|E|))}$ where V is terminal vertex set  and T is the number of Multicast Tree
</summary>

<hr class="hrlight">

> Rearrange

#### 1. Release all of the resource
<content>
將所有MTid Tree 的資源release，並Update Graph
</content>

<summary>
Time complexity： $\footnotesize{O(|T|(|V|+|E|))}$ |T|為MTid Tree的個數<br>
Space complexity：$\footnotesize{O(1)}$
</summary>

#### 2. Allocate resource to each MTid tree in descending order by transmission cost

<content>
根據transmission cost由大到到重新分配resource，使用與Insert相同方法建立MTid (Approximation Steiner Tree)
</content>

<summary>
Time complexity： $\footnotesize{O(|T||V||E|log|V|)}$ where V is terminal vertex set and T is the number of Multicast Tree<br>
Space complexity：$\footnotesize{O(|T|(|V|^2+|E|))}$ where V is terminal vertex set  and T is the number of Multicast Tree
</summary>

---

### ***Struct***

<hr class="hrlight">

```cpp
struct edgeList
{
    int index; // 該edge對應graph的index
    int dest; // edge指向的destination
    int cost; // edge的cost
};

struct Contain {
    int id; // MTid tree 的 id
    vector<int> edges; // 所使用的edge對應graph edge index
};

struct Metric {
    int** distance; // 所有terminal set 的 distance table
    edgeList** edges; // 紀錄 terminal vertex 到 other terminal vertex 的最後一個Edge，之後利用查edges table回朔整條路徑
};
```
---

### ***Class***

> Private member

```cpp
private:
	int size; // 紀錄vertex個數
	vector<edgeList>* adjList; // 排序graph edge後所建立的Adjoint List
	vector<vector<Contain>> usageEdge; // 儲存MTid 所使用的graph edge index並依照source儲存 usageEdge[source][i]
	vector<vector<bpair>> bandwidth; // 儲存MTid 所使用的bandwidth cost 並依照source儲存 bandwidth[source][i]
	Graph t_G; // 目前的Graph
	Forest t_F; // 目前的Forest
```
> public method

>insert

```cpp
void dijkstra(Tree &MTid, Metric& metric , int s, const int& t)
```
<content2>
利用dijkstra algorithm 尋找shortest path，並將結果紀錄到Metric中
</content2>

#### input
<content2>
<variable>Metric& metric</variable>: all pair shortest path information (metric closure)<br>
<variable>int s</variable>: source vertex<br>
<variable>const int& t</variable>: transmission cost that use to filter the edge in graph<br>
</content2>

<hr class="hrlight">

```cpp
void addPath(Tree &MTid, Metric& metric, bool* contain, const int& s, const int& d, const int& t, const int& cid);
```
<content2>
將terminal node 之間shortest path 所對應的edges通通加入到MTid Tree中，如果已經加入則不重複加入。
其運作原理為透過Metric edges table 回朔整條路徑
</content2>

#### input
<content2>
<variable>Metric& metric</variable>: all pair shortest path information (metric closure)<br>
<variable>bool* contain</variable>: whether the path edge has already added into MTid Tree<br>
<variable>const int& s</variable>: source terminal vertex<br>
<variable>const int& d</variable>: destination terminal vertex<br>
<variable>const int& t</variable>: transmission cost that use to update Graph<br>
<variable>const int& cid</variable>: the index of the MTid in usageEdges vector, usageEdges[source][index]<br>
</content2>

<hr class="hrlight">

```cpp
bool MST(Tree &MTid, Metric& metric, const int& t)
```

<content2>
呼叫<function>dijkstra</function>尋找all pair shortest path (metric)，
並利用metric去建立MST (Approximation Steiner Tree)並將呼叫<function>addPath</function>將path都加入至MTid Tree中
</content2>


#### input
<content2>
<variable>Metric& metric</variable>: all pair shortest path information (metric closure)<br>
<variable>const int& t</variable>: transmission cost that use to update Graph<br>
</content2>

#### output
<content2>
<variable>bool</variable>: whether satisfy the request<br>
</content2>

<hr class="hrlight">

```cpp
bool steiner(Tree &MTid, Metric& metric, const int& t)
```
<content2>
初始化metric，並呼叫<function>dijkstra</function>數次，以此計算all pair shortest path，再呼叫<function>MST</function>
建立MTid，並得知是否建立成功，最後釋放記憶體
</content2>

#### input
<content2>
<variable>Metric& metric</variable>: all pair shortest path information (metric closure)<br>
<variable>const int& t</variable>: transmission cost that use to update Graph<br>
</content2>

#### output
<content2>
<variable>bool</variable>: whether satisfy the request<br>
</content2>

#### input
<content2>
<variable>const int& s</variable>: source vertex<br>
<variable>int& need</variable>: 所需要Edge數量<br>
<variable>const int& vi</variable>: contain[s][vi]，vi代表在contain[s]中對應MTid在哪個index位置<br>
</content2>

<hr class="hrlight">

> stop

```cpp
void release(Graph &G, Tree &MTid, const int& bandw)
```
<content2>
釋放MTid所佔用的邊，並更新Graph資訊
</content2>

#### input
<content2>
<variable>const int& bandw</variable>: bandwidth cost，代表該MTid所使用的bandwidth<br>
</content2>

<hr class="hrlight">

> get member variable

```cpp
int getBandwith(const int& s, const int& id);
int getUsageIndex(const int s, const int& id);
```
<content2>
分別代表取得該MTid在bandwidth與UsageEdge中所對應的index
</content2>

#### input
<content2>
<variable>const int& s</variable>: source vertex<br>
<variable>const int& id</variable>: MTid's id<br>
</content2>

#### output
<content2>
<variable>int</variable>: 在private member中的bandwidth與usageEdge中所對應的index
</content2>

---
Test Case Design
---

### ***Problem 1***

### ***Problem 2***

---
References
---

[1] [MMM](https://www.csie.ntu.edu.tw/~kmchao/tree10spr/Steiner.pdf)
[2]