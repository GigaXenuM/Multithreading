#include "multithreading/threadpool.h"
#include <iostream>

int int_sum(int a, int b)
{
    return a + b;
}

void void_sum(int &c, int a, int b)
{
    c = a + b;
}

void void_without_argument()
{
    std::cout << "void without argument!" << std::endl;
}

int createId()
{
    static int id{ 0 };
    return id++;
}

class A
{
public:
    void foo()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "A::foo id: " << createId() << std::endl;
    }
};

int main()
{
    Multithreading::ThreadPool pool(4);

    {
        int res;

        auto future1 = pool.addTask(int_sum, 2, 3);
        pool.addTask(void_without_argument).get();
        auto future2 = pool.addTask(void_sum, std::ref(res), 3, 4);

        auto future1Res{ future1.get() };
        std::cout << "Result1: " << future1Res << std::endl;

        future2.get();
        std::cout << "Result3: " << res << std::endl;
    }

    {
        A a1;
        pool.addTask(&A::foo, std::ref(a1)).get();
        A a2;
        pool.addTask(&A::foo, std::ref(a2)).get();
    }

    {
        std::vector<std::future<void>> futures;
        for (int i = 0; i < 10; ++i)
        {
            futures.push_back(pool.addTask([i]() { std::cout << "Task " << i << " executed\n"; }));
        }
        for (auto &f : futures)
            f.wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
