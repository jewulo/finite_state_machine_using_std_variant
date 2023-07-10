// finite_state_machine_using_std_variant.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// https://www.cppstories.com/2023/finite-state-machines-variant-vending-cpp/

#include <iostream>

#include "vending_machine.h"

void VendingMachineTest()
{
    VendingMachine vm;
    vm.reportRegistry();

    try {
        vm.processEvent(event::EnterAmount { 30 });
        vm.processEvent(event::EnterAmount { 30 });
        vm.reportCurrentState();
        vm.processEvent(event::SelectItem { "Coke" });
        vm.reportRegistry();
        vm.reportCurrentState();
        vm.processEvent(event::DispenseChange {});
        vm.reportCurrentState();
        vm.processEvent(event::Reset{});
        vm.reportCurrentState();
    }
    catch (std::exception& ex) {
        std::cout << "Exception! " << ex.what() << '\n';
    }
}

int main()
{
    VendingMachineTest();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
