# concurrent-stack implementation

This implementation was based on `std::atomic<T>`

For use available:
- push value to stack using: `concurrent_stack.push(value)`
- pop value from stack using: `auto value = concurrent_stack.pop()` 
- get top value from stack using: `auto value = concurrent_stack.top()` 
- stack empty information: `auto value = concurrent_stack.is_empty()` 

This repo is part of this project.
