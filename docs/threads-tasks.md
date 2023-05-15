# POCO Tasks
The POCO library has a task class and Task manager class. The TaskManager manages a collection of tasks and monitors their lifetime.

A TaskManager has a built-in NotificationCenter that is used to send out notifications on task progress and task states.

These allow you to create tasks instead of full threads and more complicated code.

For instance:

#include <Poco/Task.h>
#include <Poco/TaskManager.h>
#include <iostream>

class FibonacciTask: public Poco::Task
{
public:
    FibonacciTask(int n): Task("FibonacciTask"), _n(n) {}

    void runTask()
    {
        long long a = 0, b = 1;

        std::cout << "Fibonacci series: " << a << " " << b << " ";

        for (int i = 2; i <= _n; i++) {
            long long temp = a + b;
            a = b;
            b = temp;

            std::cout << b << " ";

            if (isCancelled()) {
                std::cout << "\nTask cancelled." << std::endl;
                break;
            }
        }

        std::cout << std::endl;
    }

private:
    int _n;
};

int main()
{
    Poco::TaskManager tm;
    tm.start(new FibonacciTask(10));

    // Do other things...

    tm.cancelAll();  // Optionally cancel the task
    tm.joinAll();    // Wait for all tasks to complete

    return 0;
}

To get data out requires some consideration, as seemingly dynamic memory is disposed of after runTask() completes. Therefore getter functions won't work
for dynamic memory variables.

Something like this works for instance. Here a std::vector is passed into the constructor to initialise a private std::vector member. This can be 
accessed after runTask() completes. 

 std::vector<long long> series;
 Poco::TaskManager tm;
 FibonacciTask *ft = new FibonacciTask(10, &series);
 tm.start(ft);

Can also add a progress monitor class, like this:

class ProgressHandler
{
        public:
                void onProgress(Poco::TaskProgressNotification* pNf)
                {
                        std::cout << pNf->task()->name()
                                << " progress: " << pNf->progress() << std::endl;
                        pNf->release();
                }
                void onFinished(Poco::TaskFinishedNotification* pNf)
                {
                        std::cout << pNf->task()->name() << " finished." << std::endl;
                        pNf->release();
                }
};


int main()
{

        Poco::TaskManager tm;
        FibonacciTask *ft = new FibonacciTask(10); //need to be sure its not been deleted
        tm.start(ft);
ProgressHandler pm;

tm.addObserver(
                Poco::Observer<ProgressHandler, Poco::TaskProgressNotification>
                (pm, &ProgressHandler::onProgress)
              );
tm.addObserver(
                Poco::Observer<ProgressHandler, Poco::TaskFinishedNotification>
                (pm, &ProgressHandler::onFinished)
              );
