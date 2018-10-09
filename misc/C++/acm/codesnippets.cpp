// HDU-ACM 1000
#include <iostream>

using namespace std;

int main()
{
    int a, b;

    while(cin>>a>>b)
    {
        cout<<a+b<<endl;
    }

    return 0;
}

// HDU-ACM 1001
#include <iostream>

using namespace std;

int main()
{
    // 描述：输入一系列n，计算1~n的和
    // 限制条件：结果为32位整数
    int n;
    while(cin>>n)
    {
        // 若直接利用公式(n+1)*n/2，则计算(n+1)*n时可能会溢出32位整数
        int t = (n+1)/2;
        if(n+1-t == t)
        {
            cout<<t*n<<endl;
        }
        else
        {
            cout<<t*n+(n/2)<<endl;
        }
        cout<<endl;
    }

    return 0;
}

