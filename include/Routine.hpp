#pragma once
#include <vector>
#include <iostream>
#include<functional>
#include<algorithm>
template<typename T>
class Routine{
    private:
    std::vector<T>schedule;
    public:
    void addItem(const T& item)
    {
        schedule.push_back(item);
    }
    void showRoutine() const
    {
        for(const auto& item:schedule)
        {
            std::cout<<item<<std::endl;
        }
    }
    void sortRoutine(std::function<bool(const T&, const T&)> comp) 
    { 
        std::sort(schedule.begin(), schedule.end(), comp); 
    }
    const std::vector<T>& getSchedule() const {
        return schedule;
    }
    
    
};
