#include "Admin.h"
#include "User.h"

int Admin::get_Index(string user_name){
    for(int i =0;i<=users.size();i++){
        if(users[i]==user_name){
            return i;
        }else{
            continue;
        }

    }





}
void Admin::delete_User(User deleted_User_Name ){

    int position= Admin::get_Index(deleted_User_Name);

   Admin::users.erase(i);

}
void Admin::suspend_User(string suspended_User_Name ){
  int position=  int position= Admin::get_Index(suspended_User_Name);
     users.at(position).susbended_Flag=true;
}
void Admin::adjust_User_Balance(User adjusted_user){



}