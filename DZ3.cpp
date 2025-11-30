#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cmath>
#include <cctype>
#include <stdexcept>
using namespace std;

bool isOperator(char c){
    return c=='+'||c=='-'||c=='*'||c=='/';
}
int priority(const string& op){
    if(op=="+"||op=="-")
        return 1;
    if(op=="*"||op=="/")
        return 2;
    return 0;
}
bool isFunction(const string& s){
    return s=="sin"||s=="cos"||s=="tg"||s=="ctg"||s=="exp";
}
double applyFunction(const string& func,double x){
    if(func=="sin")
        return sin(x);
    if(func=="cos")
        return cos(x);
    if(func=="tg")
        return tan(x);
    if(func=="ctg")
        return 1.0/tan(x);
    if(func=="exp")
        return exp(x);
    throw runtime_error("Ошибка функции");
}

double applyOperator(const string& op,double a,double b){
    if(op=="+")
        return a+b;
    if(op=="-")
        return a-b;
    if(op=="*")
        return a*b;
    if(op=="/"){
        if(b==0)
            throw runtime_error("Деление на ноль");
        return a/b;
    }
    throw runtime_error("Ошибка оператора");
}

queue<string> toRPN(const string& expr){
    queue<string> out;
    stack<string> ops;
    int i=0;
    while(i<expr.size()){
        if(isspace(expr[i])){
            i++;
            continue;
        }
        if(isdigit(expr[i])||expr[i]=='.'){
            string num;
            while(i<expr.size()&&(isdigit(expr[i])||expr[i]=='.'))
                num+=expr[i++];
            out.push(num);
            continue;
        }
        if(expr[i]=='x'){
            out.push("x");
            i++;
            continue;
        }
        if(isalpha(expr[i])){
            string f;
            while(i<expr.size()&&isalpha(expr[i]))
                f+=expr[i++];
            if(!isFunction(f))
                throw runtime_error("Неизвестная функция");
            ops.push(f);
            continue;
        }
        if(isOperator(expr[i])){
            string op(1,expr[i]);
            while(!ops.empty()&&priority(ops.top())>=priority(op)){
                out.push(ops.top());
                ops.pop();
            }
            ops.push(op);
            i++;
            continue;
        }
        if(expr[i]=='('){
            ops.push("(");
            i++;
            continue;
        }
        if(expr[i]==')'){
            while(!ops.empty()&&ops.top()!="("){
                out.push(ops.top());
                ops.pop();
            }
            if(ops.empty())
                throw runtime_error("Ошибка скобок");
            ops.pop();
            if(!ops.empty()&&isFunction(ops.top())){
                out.push(ops.top());
                ops.pop();
            }
            i++;
            continue;
        }
        throw runtime_error("Недопустимый символ");
    }
    while(!ops.empty()){
        if(ops.top()=="(")throw runtime_error("Ошибка скобок");
        out.push(ops.top());
        ops.pop();
    }
    return out;
}

double evalRPN(queue<string> rpn,double xvalue){
    stack<double> st;
    while(!rpn.empty()){
        string t=rpn.front(); rpn.pop();
        if(t=="x"){
            st.push(xvalue);
        }
        else if(isFunction(t)){
            if(st.empty())
                throw runtime_error("Ошибка функции");
            double a=st.top();
            st.pop();
            st.push(applyFunction(t,a));
        }
        else if(t.size()==1&&isOperator(t[0])){
            if(st.size()<2)
                throw runtime_error("Ошибка оператора");
            double b=st.top();
            st.pop();
            double a=st.top();
            st.pop();
            st.push(applyOperator(t,a,b));
        }
        else st.push(stod(t));
    }
    if(st.size()!=1)
        throw runtime_error("Ошибка вычисления");
    return st.top();
}

int main(){
    try{
        string expr;
        cout<<"Введите выражение: ";
        getline(cin,expr);
        bool hasX=expr.find('x')!=string::npos;
        double xvalue=0;
        if(hasX){
            cout<<"Введите значение x: ";
            cin>>xvalue;
        }
        auto rpn=toRPN(expr);
        double res=evalRPN(rpn,xvalue);
        cout<<"Результат: "<<res<<endl;
    }
    catch(exception& e){
        cout<<"Ошибка: "<<e.what()<<endl;
    }
}