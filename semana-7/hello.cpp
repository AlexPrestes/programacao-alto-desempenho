i#include <iostream>
#include <thread>

void say_this(int val) { std::cout << "This has value " << val << std::endl; }

void say_that(int val) { std::cout << "That has value " << val << std::endl; }

int main(int, char *[]) {
  // Create a thread to run say_this with val=0.
  std::thread thd1(say_this, 0);
  // Create a thread to run say_that with val=1
  std::thread thd2(say_that, 1);

  // Original (main) thread is still running this code.
  std::cout << "Now there are 3 threads running.\n";

  // Wait for thd1 to finish.
  thd1.join();
  // Wait for thd2 to finish.
  thd2.join();

  // Only the main thread remains.
  std::cout << "Now there is only one thread." << std::endl;
  return 0;
}
