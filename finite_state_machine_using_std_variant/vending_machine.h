#pragma once

#include <format>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "overload.h"

// Here are the states...
namespace state
{
    struct Idle {};
    struct AmountEntered { int amount{ 0 }; int availableChange{ 0 }; };
    struct ItemSelected { std::string item; int availableChange{ 0 }; };
    struct ChangeDispensed { int change{ 0 }; };
}

using VendingState =
    std::variant<state::Idle,
                 state::AmountEntered,
                 state::ItemSelected,
                 state::ChangeDispensed>;

// ...and here are the events
namespace event
{
    struct EnterAmount { int amount{ 0 }; };
    struct SelectItem { std::string item; };
    struct DispenseChange { };
    struct Reset { };
}

using PossibleEvent =
    std::variant<event::EnterAmount,
                 event::SelectItem,
                 event::DispenseChange,
                 event::Reset>;

// The Machine
class VendingMachine
{
    struct Item
    {
        std::string name;
        unsigned quantity{ 0 };
        int price{ 0 };
    };

public:
    void processEvent(const PossibleEvent& event)
    {
        state_ = std::visit(helper::overload {
            [this](const auto& state, const auto& evt) {
                return onEvent(state, evt);
            }
        }, state_, event);

    }

    // on event overloads.
    VendingState onEvent(const state::Idle& idle, const event::EnterAmount& amount)
    {
        std::cout << std::format("Idle -> EnterAmount: {}\n", amount.amount);
        return state::AmountEntered { amount.amount, amount.amount };
    }

    VendingState onEvent(const state::AmountEntered& current, const event::EnterAmount& amount)
    {
        std::cout << std::format("AmountEntered {} -> EnterAmount: {}\n", current.amount, amount.amount);
        return state::AmountEntered { current.amount + amount.amount, current.availableChange + amount.amount };
    }

    VendingState onEvent(const state::AmountEntered& amount_entered, const event::SelectItem& item)
    {
        std::cout << std::format("AmountEntered {} -> SelectItem: {}\n", amount_entered.amount, item.item);

        auto it = std::ranges::find(registry_, item.item, &Item::name);
        if (it == registry_.end())
            throw std::logic_error{ "Item not found in item registry." };

        if (it->quantity > 0 && it->price <= amount_entered.amount)
        {
            std::cout << "item found...\n";
            --(it->quantity);
            return state::ItemSelected{ item.item, amount_entered.availableChange - it->price };
        }

        return amount_entered;
    }

    VendingState onEvent(const state::ItemSelected& item_selected, const event::DispenseChange& change)
    {
        std::cout << std::format("ItemSelected -> DispenseChange: {}\n", item_selected.availableChange);
        return state::ChangeDispensed { item_selected.availableChange };
    }

    VendingState onEvent(const state::ChangeDispensed& change_dispensed, const event::Reset& reset)
    {
        std::cout << "ChangeDispensed -> Reset\n";
        return state::Idle {};
    }

    VendingState onEvent(const auto&, const auto&)
    {
        throw std::logic_error{ "Unsupported event transition" };
    }

    void reportCurrentState()
    {
        std::visit(
            helper::overload{
                [](const state::Idle& idle)
                {
                    std::cout << "Idle, waiting...\n";
                },
                [](const state::AmountEntered& amount_entered)
                {
                    std::cout << std::format("AmountEntered: {}, -> available change: {}\n", amount_entered.amount, amount_entered.availableChange);
                },
                [](const state::ItemSelected& item_selected)
                {
                    std::cout << std::format("ItemSelected: {}, -> available change: {}\n", item_selected.item, item_selected.availableChange);
                },
                [](const state::ChangeDispensed& change_dispensed)
                {
                    std::cout << std::format("ChangeDispensed: {}\n", change_dispensed.change);
                },
        }, state_);
    }

    void reportRegistry()
    {
        std::cout << "avaialble items: \n";
        for (auto&& item : registry_)
            std::cout << std::format(" {}, price {}, quantity {}\n", item.name, item.price, item.quantity);
    }

private:

    std::vector<Item> registry_ {
        Item {"Coke", 5, 50},
        Item {"Pepsi", 3, 45},
        Item {"Water", 4, 35},
        Item {"Snack", 5, 25}
    };

    VendingState state_;
};


