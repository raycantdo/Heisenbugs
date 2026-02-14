#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Profile.hpp"
using namespace std;


class User
{
private:
    static int nextId;

    int userId;
    string userName;
    vector<Profile> userProfiles;
public:
    User() : userId(nextId++), userName("DefaultUser") {}
    User(const string& name)
    {
        userId = nextId++;
        userName = name;
    }
    ~User()
    {
        cout << "User destructor called for userId: " << userId << endl;
    }


    int getUserId() const
    {
        return userId;
    }
    string getUserName() const
    {
        return userName;
    }
    
    void setUserName(const string& name)
    {
        userName = name;
    }
 



    void createProfile(const Profile& profile)
    {
        userProfiles.push_back(profile);
    }
    void displayProfiles() const
    {
        for (const auto& profile : userProfiles)
        {
            profile.displayProfileInfo();
        }
        
    }
    void updateProfile(int index, const Profile& newProfile)
    {
        if (index >= 0 && index < userProfiles.size())
        {
            userProfiles[index] = newProfile;
        }
        else
        {
            cout << "Invalid profile index." << endl;
        }
    }

    void deleteProfile(int index)
    {
        if (index >= 0 && index < userProfiles.size())
        {
            userProfiles.erase(userProfiles.begin() + index);
        }
        else
        {
            cout << "Invalid profile index." << endl;
        }
    }
    void switchProfile(int index)
    {
        if (index >= 0 && index < userProfiles.size())
        {
            cout << "Switched to profile: " << userProfiles[index].getProfileName() << endl;
        }
        else
        {
            cout << "Invalid profile index." << endl;
        }
    }

    vector<Profile>& getProfiles()
    {
        return userProfiles;
    }
    const vector<Profile>& getProfiles() const
    {
        return userProfiles;
    }

};


inline int User::nextId = 1000;
#endif