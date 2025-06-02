## 理解模板类型推导

```cpp
template<typename T>
void f(ParamType param);

f(expr); // 以某表达式调用 f
```

在编译期，编译器通过 expr 推导两个类型：一个是 T 的类型，另一个是 ParamType 的类型，这两个类型往往不一样。因为，ParamType 常会包含了一些修饰词，如 const 或引用符号等限定词。例如：

```cpp
template<typename T>
void f(const T& param); // ParamType 是 const T&

int x = 0;
f(x);
```

在此例中，T 被推导为 `int`，而 ParamType 则被推导为 `const int&` 。

我们会很自然的认为 T 的类型就是 expr 的类型。但是，这一点并不总是成立。T 的类型推导结果，不仅依赖 expr 的类型，还依赖于 ParamType 的形式。具体分三种情况讨论：

- ParamType 具有指针或引用类型，但不是万能引用
- ParamType 是一个万能引用
- ParamType 既非指针也非引用

### 情况1：ParamType 具有指针或引用类型，但不是万能引用

这种情况最简单，在这种情况下，类型推导会这样运作：

- 若 expr 是引用类型，先将引用部分忽略
- 对 expr 的类型和 ParamType 的类型执行模式匹配，来决定 T 的类型

```cpp
template<typename T>
void f(T& param); // ParamType 是 T&

int x = 27;
const int cx = x;
const int& rx = x;

f(x); // T 的类型是 int，param 的类型是 int&
f(cx); // T 的类型是 const int，param 的类型是 const int&
f(rx); // T 的类型是 const int，param 的类型是 const int&
```

rx 的引用属性在类型推导时被忽略了。

```cpp
emplate<typename T>
void f(const T& param); // ParamType 是 const T&

int x = 27;
const int cx = x;
const int& rx = x;

f(x); // T 的类型是 int，param 的类型是 int&
f(cx); // T 的类型是 int，param 的类型是 const int&
f(rx); // T 的类型是 int，param 的类型是 const int&
```

### 情况2：ParamType 是一个万能引用

类型推导运作如下：

- 如果 expr 是个左值，T 和 ParamType 都会被推导为左值引用。这个结果具有双重奇特之处：首先，这是在模板类型推导中，T 被推导为引用类型的唯一情况。其次，尽管在声明时使用的是右值引用的语法，它的类型推导结果却是左值引用。
- 如果 expr 是个右值，则应用**情况1**的规则

```cpp
template<typename T>
void f(T&& param); // ParamType 是 T&&

int x = 27;
const int cx = x;
const int& rx = x;

f(x); // x 是个左值，所以 T 的类型是 int&，param 的类型是 int&
f(cx); // cx 是个左值, 所以 T 的类型是 const int&，param 的类型是 const int&
f(rx); // rx 是个左值, 所以 T 的类型是 const int&，param 的类型是 const int&
f(27); // 27 是个右值，所以 T 的类型是 int，param 的类型是 int&&
```

### 情况3：ParamType 既非指针也非引用

这种情况下就是按值传递，这意味着，无论传入什么，param 都是它的一个副本，推导规则如下：

- 若 expr 具有引用类型，则忽略其引用属性（导致用 auto 声明引用类型时必须加 `&` 的原因）
- 若 expr 是个 const 对象，也忽略之。若 expr 是个 volatile 对象，同忽略之。

```cpp
template<typename T>
void f(T param); // ParamType 是 T

int x = 27;
const int cx = x;
const int& rx = x;
const char* const ptr = "Fun with pointers";

f(x); // T 和 param 的类型都是 int
f(cx); // T 和 param 的类型都是 int
f(rx); // T 和 param 的类型都是 int
f(ptr); // T 和 param 的类型都是指向 const char* 的指针类型，指针本身的 const 属性被去除，但它指向对象的 const 属性并未被去除
```

### 数组实参

在很多语境下，数组会退化成指涉到其首元素的指针。因此，按值传递给函数模板的数组类型将被推导为指针类型。

难点来了，尽管函数无法声明真正的数组类型的形参（因为会退化为指针），它们却能够将形参声明为数组的引用。

```cpp
const char name[] = "J.P.Briggs";
template<typename T>
void f(T& param);
//此时T的类型会被推导成实际的数组类型，这个类型还会包含数组的长度。
f(name);
```

可以利用声明数组引用这一能力创造出一个模板，用来推导数组含有的元素个数：

```cpp
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}
```

### 函数实参

函数类型也会退化成函数指针，我们针对数组类型推导的一切讨论都适用于函数及其向函数指针的退化。

## 理解 auto 类型推导

除了一个奇妙的例外情况外，auto 类型推导就是模板类型推导。

当某个变量采用 auto 来声明时，auto 就扮演了模板中 T 的角色，而变量的修饰词则扮演的是 ParamType 的角色。所以也存在三种情况：

- 情况1：类型修饰词是指针或引用，但不是万能引用
- 情况2：类型修饰词是万能引用
- 情况3：类型修饰词既非指针也非引用

对于大括号初始化表达式的处理方式，是 auto 类型推导和模板类型推导的**唯一不同之处**。当采用 auto 声明的变量使用大括号初始化表达式进行初始化时，推导所得的类型是 std::initializer 的一个实例类型。但是，如果向对应的模板传入一个同样的初始化表达式，类型推导就会失败，代码将不能通过编译：

```cpp
auto x = {11, 23, 9}; // x 的类型是 std::initializer_list<int>
template<typename T>
void f(T param);

f({11, 23, 9})； // 错误，无法推导 T 的类型
```

C++14 允许使用 auto 来说明函数返回值需要推导，而且 C++14 中 lambda 式也会在形参声明中用到 auto。然而这些 auto 用法是使用模板类型推导而非 auto 类型推导。

```cpp
auto CreateInitList()
{
    return {1, 2, 3}; //错误，无法完成类型推导
}
```

## 理解 decltype

decltype 能告诉你该名字或表达式的类型。与模板和 auto 类型推导过程相反，decltype 一般只会返回给定名字或表达式的确切类型而已。

C++11中，decltype 的主要作用大概就在于声明那些返回值类型依赖于形参类型的函数模板。

```cpp
//此处的auto与类型推导没任何关系，它只是说明此处使用了C++11中的返回值类型尾序语法
//此语法的好处在于指定返回值类型时可以使用函数形参
template<typename Container, typename Index>
auto authAndAccess(Container& c, Index i) -> decltype(c[i])
{
    authenticateUser();
    return c[i];
}
```

C++11允许对单表达式的 lambda 式的返回值类型进行推导，而 C++14 则将这个允许范围扩张到了一切 lambda 式和一切函数。

```cpp
template<typename Container, typename Index> //c++14
auto authAndAccess(Container& c, Index i) // 不甚正确
{
    authenticateUser();
    return c[i]; // 实施的是模板类型推导，operator[] 会返回 T&，但是引用会被忽略
}

template<typename Container, typename Index> //c++14
decltype(auto) authAndAccess(Container& c, Index i)
{
    authenticateUser();
    return c[i];
}

// decltype(auto) 并不限于在函数的返回值类型处使用，还可以用于变量声明。
Widget w;
const Widget& cw = w;
auto myWidget1 = cw; // myWidget1 的类型为 Widget
decltype(auto) myWidget2 = cw // myWidget2 的类型为 const Widget&
```

**需要注意的是**：只要一个左值表达式不仅是一个类型的为 T 的名字时，它就得出一个 T& 类型。

```cpp
int x = 0;
decltype((x)); // 返回的是 int& 类型
```

## 掌握查看类别推导结果的方法

### 编译器诊断信息

想要让编译器显示其推导出的类别，一条有效的途径是使用该类别导致某些编译错误。

```cpp
template<typename T>
class TD; // 只声明不定义，只要试图具现该模版，就会诱发一条错误信息

TD<decltype(x)> xType; // 诱发包含 x 类别的错误信息
```

### 运行时输出

```cpp
// 调用 typeid 会得到一个 std::type_info 对象，而后者拥有一个成员函数 name
// 该函数产生一个代表类别的 C-style 的字符串（const char*）
// C++ 标准规定，name 中处理类别的方式仿佛是向函数模版按值传递形参一样
std::cout << typeid(x).name() << '\n';
```