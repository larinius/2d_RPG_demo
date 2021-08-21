#include "precompiled.h"
#include "Events.h"

namespace rpg {
void Events::AddListener(EventsTypes e, const std::string listener) {
  queue.appendListener(guiButtonPress, [](const std::string s, bool b) {});

}
void Events::tutorial() {
  using EQ = eventpp::EventQueue<int, void(int)>;
  EQ queue;

  constexpr int stopEvent = 1;
  constexpr int otherEvent = 2;

  // Start a thread to process the event queue.
  // All listeners are invoked in that thread.
  std::thread thread([stopEvent, otherEvent, &queue]() {
    volatile bool shouldStop = false;
    queue.appendListener(stopEvent, [&shouldStop](int) { shouldStop = true; });
    queue.appendListener(otherEvent,
                         [](const int index) { std::cout << "Got event, index is " << index << std::endl; });

    while (!shouldStop) {
      queue.wait();

      queue.process();
    }
  });

  // Enqueue an event from the main thread. After sleeping for 10 milliseconds,
  // the event should have be processed by the other thread.
  queue.enqueue(otherEvent, 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout << "Should have triggered event with index = 1" << std::endl;

  queue.enqueue(otherEvent, 2);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout << "Should have triggered event with index = 2" << std::endl;

  {
    // EventQueue::DisableQueueNotify is a RAII class that
    // disables waking up any waiting threads.
    // So no events should be triggered in this code block.
    // DisableQueueNotify is useful when adding lots of events at the same time
    // and only want to wake up the waiting threads after all events are added.
    EQ::DisableQueueNotify disableNotify(&queue);

    queue.enqueue(otherEvent, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "Should NOT trigger event with index = 10" << std::endl;

    queue.enqueue(otherEvent, 11);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "Should NOT trigger event with index = 11" << std::endl;
  }
  // The DisableQueueNotify object is destroyed here, and has resumed
  // waking up waiting threads. So the events should be triggered.
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout << "Should have triggered events with index = 10 and 11" << std::endl;

  queue.enqueue(stopEvent, 1);
  thread.join();
}
}
