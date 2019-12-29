#pragma once

#include <atomic>
#include <optional>

template <typename  T>
class concurrent_stack
{
public:
	concurrent_stack() : top_(nullptr) {}

	void push(const T& value) noexcept
	{
		stack_node<T>* new_node = new stack_node<T>(value, this->top_.load());

		while (!this->top_.compare_exchange_weak(new_node->next, new_node));
	}
	std::optional<T> pop() noexcept
	{
		stack_node<T>* top = this->top_.load();

		if (top == nullptr)
			return std::optional<T>();

		stack_node<T>* new_front = top->next;

		while (!this->top_.compare_exchange_weak(top, new_front))
		{
			if (top == nullptr)
				return std::optional<T>();

			new_front = top->next;
		}

		T value = top->value;
		delete top;

		return std::optional<T>(value);
	}
	std::optional<T> top() const noexcept
	{
		stack_node<T>* top = this->top_.load(std::memory_order_acquire);

		if (top == nullptr)
			return std::optional<T>();

		return std::optional<T>(top->value);
	}

	[[nodiscard]] bool is_empty() const noexcept
	{
		return this->top_.load() == nullptr;
	}

private:

	template <typename X>
	struct stack_node
	{
		X value;
		stack_node<X>* next;

		explicit stack_node(const X& value) : value(value), next(nullptr) {}
		explicit stack_node(const X& value, stack_node<X>* next) : value(value), next(next) {}
	};

	std::atomic<stack_node<T>*> top_;
};