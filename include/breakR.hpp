#ifndef BREAKR_HPP
#define BREAKR_HPP

#include <iostream>


#include <conio.h>
class breakR
{
private:
        
    
    int litre;
public:
    breakR():litre(0){}
    

    void drinkWater()
    {
         litre++;
    }
    int getTotalWater() const
    {
        return litre;
    }
    void checkStudyReminder(int step)
    {
        if(step && step % 1==0){
            if(step%2==0)
            {
                 std::cout << " Water reminder! \n";
            
            }
            
        
        else{
            std::cout << " Break reminder! \n";
        }
    }
}
    void taskCompleted(int t)
    {
        
        if(t% 2 == 1)
        {
            std::cout << "Time for hydration.\n";
        }
        else
        {
            std::cout << "Take a break and drink water.\n";
        }
        pressD(*this);
    }
    friend void showHydrationReport(const breakR& b);
    friend void pressD(breakR& b);
};
inline void showHydrationReport(const breakR& b)
{
    std::cout << "\n----- Hydration Report -----\n";
    std::cout << "Water consumed: "
              << b.getTotalWater() << " L\n";
}
inline void pressD(breakR& b)
{
    
    char input;
    cin>>input;
    
    if(input=='D'|| input=='d')
    {
        b.drinkWater();
    }
}


#endif
