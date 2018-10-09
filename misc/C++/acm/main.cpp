#include<iostream>
using namespace std;
int f[54]={0,1,1};
int main()
{
int A,B,n,q=1;
while(cin>>A>>B>>n&&A&&B&&n)
{
    for(int i=3;i<54;++i)
    {
        f[i]=(A*f[i-1]+B*f[i-2])% 7;
        if(i>4)
        {
            if(f[i-1]==f[3]&&f[i]==f[4])
            {
                q=i-4;  //要特别注意，可以想一下为什么？

            }
        }
    }
    cout<<f[n%q]<<endl;
}

return 0;
}
